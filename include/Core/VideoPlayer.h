#ifndef __VIDEOPLAYER_H__
#define __VIDEOPLAYER_H__

#include "Module.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <windows.h>
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/frame.h> // revisar
#include <SDL2/SDL.h>

class VideoPlayer : public Module
{
public:

    VideoPlayer();

    VideoPlayer(bool startEnabled);

    // Destructor
    virtual ~VideoPlayer();

    // Called after Awake
    bool Start() override;

    // Called every frame
    bool PreUpdate() override;

    // Called every frame
    bool Update(float dt) override;

    // Called before quitting
    bool CleanUp() override;

    void display(AVCodecContext*, AVPacket*, AVFrame*, SDL_Rect*, SDL_Texture*, SDL_Renderer*, double);

    void playaudio(AVCodecContext* ctx, AVPacket* pkt, AVFrame* frame, SDL_AudioDeviceID auddev);

private:
    // ffmpeg part
    AVFormatContext* pFormatCtx;
    int vidId = -1, audId = -1;
    double fpsrendering = 0.0;
    AVCodecContext* vidCtx, * audCtx;
    AVCodec* vidCodec, * audCodec;
    AVCodecParameters* vidpar, * audpar;
    AVFrame* vframe, * aframe;
    AVPacket* packet;

    //sdl part
    int swidth, sheight;
    SDL_Window* screen;
    SDL_Renderer* renderer;
    SDL_Texture* texture;
    SDL_Rect rect;
    SDL_AudioDeviceID auddev;
    SDL_AudioSpec want, have;

};

#endif // __VIDEOPLAYER_H__