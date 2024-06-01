#ifndef __VIDEOPLAYER_H__
#define __VIDEOPLAYER_H__

#include "Core/Module.h"
#include <SDL2/SDL.h>
#include <queue>

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

class VideoPlayer : public Module
{
public:

    VideoPlayer();

    VideoPlayer(bool startEnabled);

    // Destructor
    virtual ~VideoPlayer();

    bool Awake(pugi::xml_node& conf);

    // Called after Awake
    bool Start(const char* file); //modified to accept video file

    // Called every frame
    bool Update(float dt);

    // Called before quitting
    bool CleanUp();

    //New method to change file
    bool ChangeVideo(const char* newFile);

    bool OpenCodecContext(int* index);
    bool OpenVideoCodecContext(int index);
    bool OpenAudioCodecContext(int index);
    bool ConvertPixels(int videoIndex, int audioIndex);
    bool AllocImage(AVFrame* dstFrame);
    void RenderCutscene();
    void ProcessAudio();



private:
    int streamIndex = -1;
    AVFormatContext* formatContext = nullptr;
    AVCodecContext* videoCodecContext = nullptr;
    AVCodecContext* audioCodecContext = nullptr;

    SDL_AudioDeviceID audioDevice;
    int audioStreamIndex;

    SDL_Texture* renderTexture = nullptr;
    SDL_Texture* texture1;
    SDL_Texture* texture2;
    SDL_Rect renderRect;
    bool running = false;

    std::queue<AVPacket> audioBuffer;

};

#endif // __VIDEOPLAYER_H__