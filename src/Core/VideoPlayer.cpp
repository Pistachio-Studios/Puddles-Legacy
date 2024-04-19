#include "Core/VideoPlayer.h"
#include "Core/App.h"
#include "Core/DebugUI.h"
#include <imgui.h>
#include <tracy/Tracy.hpp>
#include <Utils/Log.h>

VideoPlayer::VideoPlayer()
{
}

VideoPlayer::VideoPlayer(bool startEnabled)
{
}

bool VideoPlayer::Start()
{
	return false;
}

bool VideoPlayer::PreUpdate()
{
	return false;
}

bool VideoPlayer::Update(float dt)
{
	return false;
}

bool VideoPlayer::CleanUp()
{

    /*clean_audio_device:
    SDL_CloseAudioDevice(auddev);
    clean_texture:
    SDL_DestroyTexture(texture);
    clean_renderer:
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(screen);
    clean_packet_frame:
    av_packet_free(&packet);
    av_frame_free(&vframe);
    av_frame_free(&aframe);
    clean_codec_context:
    avcodec_free_context(&vidCtx);
    avcodec_free_context(&audCtx);
    clean_format_context:
    avformat_close_input(&pFormatCtx);
    avformat_free_context(pFormatCtx);*/
	return false;
}

//void display(AVCodecContext* ctx, AVPacket* pkt, AVFrame* frame, SDL_Rect* rect,
//    SDL_Texture* texture, SDL_Renderer* renderer, double fpsrend)
//{
//    time_t start = time(NULL);
//    if (avcodec_send_packet(ctx, pkt) < 0) {
//        perror("send packet");
//        return;
//    }
//    if (avcodec_receive_frame(ctx, frame) < 0) {
//        perror("receive frame");
//        return;
//    }
//    int framenum = ctx->frame_number;
//    if ((framenum % 1000) == 0) {
//        printf("Frame %d (size=%d pts %d dts %d key_frame %d"
//            " [ codec_picture_number %d, display_picture_number %d\n",
//            framenum, frame->pkt_size, frame->pts, frame->pkt_dts, frame->key_frame,
//            frame->coded_picture_number, frame->display_picture_number);
//    }
//    SDL_UpdateYUVTexture(texture, rect,
//        frame->data[0], frame->linesize[0],
//        frame->data[1], frame->linesize[1],
//        frame->data[2], frame->linesize[2]);
//    SDL_RenderClear(renderer);
//    SDL_RenderCopy(renderer, texture, NULL, rect);
//    SDL_RenderPresent(renderer);
//    time_t end = time(NULL);
//    double diffms = difftime(end, start) / 1000.0;
//    if (diffms < fpsrend) {
//        uint32_t diff = (uint32_t)((fpsrend - diffms) * 1000);
//        printf("diffms: %f, delay time %d ms.\n", diffms, diff);
//        SDL_Delay(diff);
//    }
//}
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
