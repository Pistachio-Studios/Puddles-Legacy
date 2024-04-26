#include "Core/VideoPlayer.h"
#include "Core/App.h"
#include "Core/Render.h"
#include "Core/DebugUI.h"
#include <SDL2/SDL.h>
#include <ctime>

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/imgutils.h>
#include <libavutil/avutil.h>
#include <libswscale/swscale.h>
#include <libswresample/swresample.h>
}

VideoPlayer::VideoPlayer() :Module()
{
    name.Create("videoPlayer");
}

VideoPlayer::VideoPlayer(bool startEnabled) : Module(startEnabled)
{
    name.Create("videoPlayer");
}

VideoPlayer::~VideoPlayer() {}

bool VideoPlayer::Start()
{
    pFormatCtx = avformat_alloc_context();
    char bufmsg[1024];
    if (avformat_open_input(&pFormatCtx, "Assets/Video/Intro/example.mp4", NULL, NULL) < 0) {
        sprintf(bufmsg, "Cannot open %s", "Assets/Video/Intro/example.mp4");
        perror(bufmsg);
        avformat_close_input(&pFormatCtx);
        avformat_free_context(pFormatCtx);;
    }
    if (avformat_find_stream_info(pFormatCtx, NULL) < 0) {
        perror("Cannot find stream info. Quitting.");
        avformat_close_input(&pFormatCtx);
        avformat_free_context(pFormatCtx);;
    }
    bool foundVideo = false, foundAudio = false;
    for (int i = 0; i < pFormatCtx->nb_streams; i++) {
        AVCodecParameters *localparam = pFormatCtx->streams[i]->codecpar;
        AVCodec *localcodec = (AVCodec *)avcodec_find_decoder(localparam->codec_id);
        if (localparam->codec_type == AVMEDIA_TYPE_VIDEO && !foundVideo) {
            vidCodec = localcodec;
            vidpar = localparam;
            vidId = i;
            AVRational rational = pFormatCtx->streams[i]->avg_frame_rate;
            fpsrendering = 1.0 / ((double)rational.num / (double)(rational.den));
            foundVideo = true;
        } else if (localparam->codec_type == AVMEDIA_TYPE_AUDIO && !foundAudio) {
            audCodec = localcodec;
            audpar = localparam;
            audId = i;
            foundAudio = true;
        }
        if (foundVideo && foundAudio) { break; }
    }
    vidCtx = avcodec_alloc_context3(vidCodec);
    audCtx = avcodec_alloc_context3(audCodec);
    if (avcodec_parameters_to_context(vidCtx, vidpar) < 0) {
        perror("vidCtx");
        avcodec_free_context(&vidCtx);
        avcodec_free_context(&audCtx);;
    }
    if (avcodec_parameters_to_context(audCtx, audpar) < 0) {
        perror("audCtx");
        avcodec_free_context(&vidCtx);
        avcodec_free_context(&audCtx);;
    }
    if (avcodec_open2(vidCtx, vidCodec, NULL) < 0) {
        perror("vidCtx");
        avcodec_free_context(&vidCtx);
        avcodec_free_context(&audCtx);;
    }
    if (avcodec_open2(audCtx, audCodec, NULL) < 0) {
        perror("audCtx");
        avcodec_free_context(&vidCtx);
        avcodec_free_context(&audCtx);;
    }

    vframe = av_frame_alloc();
    aframe = av_frame_alloc();
    packet = av_packet_alloc();
    swidth = vidpar->width;
    sheight = vidpar->height;

    texture = SDL_CreateTexture(app->render->renderer, SDL_PIXELFORMAT_IYUV,
        SDL_TEXTUREACCESS_STREAMING | SDL_TEXTUREACCESS_TARGET,
        swidth, sheight);
    if (!texture) {
        perror("texture");
        SDL_DestroyTexture(texture);
    }
    rect.x = 0;
    rect.y = 0;
    rect.w = swidth;
    rect.h = sheight;

    // Audio setup
    SDL_zero(want);
    SDL_zero(have);
    want.samples = audpar->sample_rate;
    want.channels = audpar->channels;
    want.format = audpar->format;
    want.freq = audpar->sample_rate;
    
    auddev = SDL_OpenAudioDevice(NULL, 0, &want, &have, 0);

    if (auddev == 0) {
        SDL_Log("Failed to open audio: %s", SDL_GetError());
    } else {
        if (have.format != want.format) {
            SDL_Log("We didn't get the wanted audio format.");
        }
        SDL_PauseAudioDevice(auddev, 0); /* start audio playing. */
    }

	return true;
}

bool VideoPlayer::PreUpdate()
{
	return true;
}

bool VideoPlayer::Update(float dt)
{
    while (av_read_frame(pFormatCtx, packet) >= 0)
    {
        if (packet->stream_index == vidId) {
            display(vidCtx, packet, vframe, &rect, texture, app->render->renderer, fpsrendering);
        } else if (packet->stream_index == audId) {
            playaudio(audCtx, packet, aframe, auddev);
        }
        av_packet_unref(packet);
    }

	return true;
}

bool VideoPlayer::CleanUp()
{
    SDL_CloseAudioDevice(auddev);
    SDL_DestroyTexture(texture);
    av_packet_free(&packet);
    av_frame_free(&vframe);
    av_frame_free(&aframe);
    avcodec_free_context(&vidCtx);
    avcodec_free_context(&audCtx);
    avformat_close_input(&pFormatCtx);
    avformat_free_context(pFormatCtx);
	return true;
}

void VideoPlayer::display(AVCodecContext* ctx, AVPacket* pkt, AVFrame* frame, SDL_Rect* rect,
    SDL_Texture* texture, SDL_Renderer* renderer, double fpsrend)
{
    time_t start = SDL_GetTicks64();
    if (avcodec_send_packet(ctx, pkt) < 0) {
        perror("send packet");
        return;
    }
    if (avcodec_receive_frame(ctx, frame) < 0) {
        perror("receive frame");
        return;
    }
    int framenum = ctx->frame_number;
    if ((framenum % 1000) == 0) {
        printf("Frame %d (size=%d pts %d dts %d key_frame %d"
            " [ codec_picture_number %d, display_picture_number %d\n",
            framenum, frame->pkt_size, frame->pts, frame->pkt_dts, frame->key_frame,
            frame->coded_picture_number, frame->display_picture_number);
    }
    if (SDL_UpdateYUVTexture(texture, rect,
            frame->data[0], frame->linesize[0],
            frame->data[1], frame->linesize[1],
            frame->data[2], frame->linesize[2]) < 0) {
        printf("SDL_UpdateYUVTexture Error: %s\n", SDL_GetError());
        return;
    }
    if (SDL_RenderClear(renderer) < 0) {
        printf("SDL_RenderClear Error: %s\n", SDL_GetError());
        return;
    }
    if (SDL_RenderCopy(renderer, texture, NULL, rect) < 0) {
        printf("SDL_RenderCopy Error: %s\n", SDL_GetError());
        return;
    }
    SDL_RenderPresent(renderer);
    time_t end = SDL_GetTicks64();
    double diffms = difftime(end, start) / 1000.0;
    if (diffms < fpsrend) {
        uint32_t diff = (uint32_t)((fpsrend - diffms) * 1000);
        printf("diffms: %f, delay time %d ms.\n", diffms, diff);
        SDL_Delay(diff);
    }
}

void VideoPlayer::playaudio(AVCodecContext* ctx, AVPacket* pkt, AVFrame* frame, SDL_AudioDeviceID auddev)
{
    if (avcodec_send_packet(ctx, pkt) < 0) {
        SDL_Log("Error in send packet: %s", SDL_GetError());
        return;
    }
    if (avcodec_receive_frame(ctx, frame) < 0) {
        SDL_Log("Error in receive frame: %s", SDL_GetError());
        return;
    }

    SwrContext* swr = swr_alloc_set_opts(NULL, av_get_default_channel_layout(ctx->channels), AV_SAMPLE_FMT_S16, ctx->sample_rate,
        av_get_default_channel_layout(ctx->channels), (AVSampleFormat)frame->format, ctx->sample_rate, 0, NULL);

    if (!swr || swr_init(swr) < 0) {
        SDL_Log("Failed to initialize resampler: %s", SDL_GetError());
        return;
    }

    uint8_t* buffer = NULL;
    int numSamples = av_rescale_rnd(swr_get_delay(swr, ctx->sample_rate) + frame->nb_samples, ctx->sample_rate, ctx->sample_rate, AV_ROUND_UP);
    av_samples_alloc(&buffer, NULL, ctx->channels, numSamples, AV_SAMPLE_FMT_S16, 0);

    numSamples = swr_convert(swr, &buffer, numSamples, (const uint8_t**)frame->data, frame->nb_samples);

    if (SDL_QueueAudio(auddev, buffer, numSamples * ctx->channels * av_get_bytes_per_sample(AV_SAMPLE_FMT_S16)) < 0) {
        SDL_Log("Error in playaudio: %s", SDL_GetError());
    }

    if (buffer) {
        av_freep(&buffer);
    }

    swr_free(&swr);
}