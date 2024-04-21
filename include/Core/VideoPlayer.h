#ifndef __VIDEOPLAYER_H__
#define __VIDEOPLAYER_H__

#include "Core/Module.h"
/* #include <stdbool.h>
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/frame.h> // revisar */
#include <SDL2/SDL.h>

class AVCodecContext;
class AVPacket;
class AVFrame;
class SDL_Rect;
class SDL_Texture;
class SDL_Renderer;
class AVFormatContext;
class AVCodec;
class AVCodecParameters;

class VideoPlayer : public Module
{
public:

    VideoPlayer();

    VideoPlayer(bool startEnabled);

    // Destructor
    virtual ~VideoPlayer();

    // Called after Awake
    bool Start();

    // Called every frame
    bool PreUpdate();

    // Called every frame
    bool Update(float dt);

    // Called before quitting
    bool CleanUp();

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
    SDL_Texture* texture;
    SDL_Rect rect;

};

#endif // __VIDEOPLAYER_H__