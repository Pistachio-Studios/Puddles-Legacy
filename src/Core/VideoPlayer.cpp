#include "Core/VideoPlayer.h"
#include "Core/App.h"
#include "Core/Render.h"
#include "Core/DebugUI.h"

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
    /* pFormatCtx = avformat_alloc_context();
    char bufmsg[1024];
    if (avformat_open_input(&pFormatCtx, "/home/hugo/Documentos/GitHub/Proyecto2/bin/Assets/Video/Intro/example.mp4", NULL, NULL) < 0) {
        sprintf(bufmsg, "Cannot open %s", "/home/hugo/Documentos/GitHub/Proyecto2/bin/Assets/Video/Intro/example.mp4");
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
    rect.h = sheight; */

	return true;
}

bool VideoPlayer::PreUpdate()
{
	return true;
}

bool VideoPlayer::Update(float dt)
{
   /*  while (av_read_frame(pFormatCtx, packet) >= 0)
        {
            if (packet->stream_index == vidId) {
                display(vidCtx, packet, vframe, &rect, texture, app->render->renderer, fpsrendering);
            }
            av_packet_unref(packet);
        } */

	return true;
}

bool VideoPlayer::CleanUp()
{
/*     SDL_CloseAudioDevice(auddev);
    SDL_DestroyTexture(texture);
    av_packet_free(&packet);
    av_frame_free(&vframe);
    av_frame_free(&aframe);
    avcodec_free_context(&vidCtx);
    avcodec_free_context(&audCtx);
    avformat_close_input(&pFormatCtx);
    avformat_free_context(pFormatCtx); */
	return true;
}

/* void display(AVCodecContext* ctx, AVPacket* pkt, AVFrame* frame, SDL_Rect* rect,
    SDL_Texture* texture, SDL_Renderer* renderer, double fpsrend)
{
    time_t start = time(NULL);
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
    SDL_UpdateYUVTexture(texture, rect,
        frame->data[0], frame->linesize[0],
        frame->data[1], frame->linesize[1],
        frame->data[2], frame->linesize[2]);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, rect);
    SDL_RenderPresent(renderer);
    time_t end = time(NULL);
    double diffms = difftime(end, start) / 1000.0;
    if (diffms < fpsrend) {
        uint32_t diff = (uint32_t)((fpsrend - diffms) * 1000);
        printf("diffms: %f, delay time %d ms.\n", diffms, diff);
        SDL_Delay(diff);
    }
} */
//
//void playaudio(AVCodecContext* ctx, AVPacket* pkt, AVFrame* frame,
//    SDL_AudioDeviceID auddev)
//{
//    if (avcodec_send_packet(ctx, pkt) < 0) {
//        perror("send packet");
//        return;
//    }
//    if (avcodec_receive_frame(ctx, frame) < 0) {
//        perror("receive frame");
//        return;
//    }
//    int size;
//    int bufsize = av_samples_get_buffer_size(&size, ctx->channels, frame->nb_samples, frame->format, 0);
//    bool isplanar = av_sample_fmt_is_planar(frame->format) == 1;
//    for (int ch = 0; ch < ctx->channels; ch++) {
//        if (!isplanar) {
//            if (SDL_QueueAudio(auddev, frame->data[ch], frame->linesize[ch]) < 0) {
//                perror("playaudio");
//                printf(" %s\n", SDL_GetError());
//                return;
//            }
//        }
//        else {
//            if (SDL_QueueAudio(auddev, frame->data[0] + size * ch, size) < 0) {
//                perror("playaudio");
//                printf(" %s\n", SDL_GetError());
//                return;
//            }
//        }
//    }
//}
