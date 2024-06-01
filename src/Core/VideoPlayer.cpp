#include "Core/VideoPlayer.h"
#include "Core/App.h"
#include "Core/Render.h"
#include "Core/DebugUI.h"
#include "Utils/Log.h"

VideoPlayer::VideoPlayer(bool enabled) : Module(enabled)
{
    name.Create("VideoPlayer");
}

VideoPlayer::~VideoPlayer()
{}

bool VideoPlayer::Awake(pugi::xml_node& config)
{
    LOG("Loading VideoPlayer");
    bool ret = true;

    return ret;
}

bool VideoPlayer::Start(const char* file)
{
    // File path of the video to be played
    //const char* file =  "Assets/Video/Logo/Logo-pistachio.mp4";
    //const char* file1 = "Assets/Video/Intro/example.mp4";

    // Allocate memory for the format context
    formatContext = avformat_alloc_context();

    // Open the input video file with FFMPEG
    if (avformat_open_input(&formatContext, file, NULL, NULL) < 0) {
        printf("Failed to open input file");
        avformat_close_input(&formatContext);
        avformat_free_context(formatContext);
        return false;
    }

    // Find input stream information
    if (avformat_find_stream_info(formatContext, NULL) < 0) {
        printf("Failed to find input stream information");
        avformat_close_input(&formatContext);
        avformat_free_context(formatContext);
        return false;
    }

    // Dump format information
    av_dump_format(formatContext, 0, file, 0);

    // Open codec context for video and audio streams
    OpenCodecContext(&streamIndex);

    // Create SDL texture for rendering video frames
    renderTexture = SDL_CreateTexture(app->render->renderer, SDL_PIXELFORMAT_YV12,
        SDL_TEXTUREACCESS_STREAMING, videoCodecContext->width, videoCodecContext->height);
    if (!renderTexture) {
        printf("Failed to create texture - %s\n", SDL_GetError());
        return false;
    }

    // Set up SDL rectangle for video rendering
	renderRect = { 0, 0, videoCodecContext->width, videoCodecContext->height };

    // Set the module state to running
    running = true;
    return true;
}

bool VideoPlayer::ChangeVideo(const char* newFile)
{
    LOG("Changing video to %s", newFile);
    //Detener reproduccion actual y limpar recursos
    running = false;
    CleanUp();

    //Iniciar el nuevo video
    return Start(newFile);
}

bool VideoPlayer::OpenCodecContext(int* index)
{
    // Find video stream in the file
    int videoIndex = av_find_best_stream(formatContext, AVMEDIA_TYPE_VIDEO, -1, -1, NULL, 0);
    if (videoIndex < 0) {
        printf("Failed to find video stream in input file\n");
        return true;
    }

    // Find audio stream in the file
    int audioIndex = av_find_best_stream(formatContext, AVMEDIA_TYPE_AUDIO, -1, -1, NULL, 0);
    if (audioIndex < 0) {
        printf("Failed to find audio stream in input file\n");
        // No audio stream found, continue without audio
    }

    // Open video codec context
    if (OpenVideoCodecContext(videoIndex)) {
        return true;
    }

    // Open audio codec context if audio stream found
    if (audioIndex >= 0 && OpenAudioCodecContext(audioIndex)) {
        return true;
    }

    *index = videoIndex;

    return false;
}

bool VideoPlayer::OpenVideoCodecContext(int videoIndex)
{
    // Find the decoder for the video codec
    const AVCodec* codec = avcodec_find_decoder(formatContext->streams[videoIndex]->codecpar->codec_id);
    if (!codec) {
        printf("Failed to find video codec!\n");
        return true;
    }

    // Allocate a codec context for the decoder
    videoCodecContext = avcodec_alloc_context3(codec);
    if (!videoCodecContext) {
        printf("Failed to allocate the video codec context\n");
        return true;
    }

    // Copy video codec parameters to the decoder context
    if (avcodec_parameters_to_context(videoCodecContext, formatContext->streams[videoIndex]->codecpar) < 0) {
        printf("Failed to copy video codec parameters to decoder context!\n");
        return true;
    }

    // Open the video codec
    if (avcodec_open2(videoCodecContext, codec, NULL) < 0) {
        printf("Failed to open video codec\n");
        return true;
    }

    return false;
}

Uint32 audioTimerCallback(Uint32 interval, void* param)
{
    VideoPlayer* player = static_cast<VideoPlayer*>(param);
    if (player) {
        player->ProcessAudio();
    }
    return interval;
}

bool VideoPlayer::OpenAudioCodecContext(int audioIndex)
{
	// Find the decoder for the audio codec
    const AVCodec* codec = avcodec_find_decoder(formatContext->streams[audioIndex]->codecpar->codec_id);
    if (!codec) {
        printf("Failed to find audio codec!\n");
        return true;
    }

	// Allocate a codec context for the decoder
    audioCodecContext = avcodec_alloc_context3(codec);
    if (!audioCodecContext) {
        printf("Failed to allocate the audio codec context\n");
        return true;
    }

	// Copy audio codec parameters to the decoder context
    if (avcodec_parameters_to_context(audioCodecContext, formatContext->streams[audioIndex]->codecpar) < 0) {
        printf("Failed to copy audio codec parameters to decoder context!\n");
        return true;
    }

	// Open the audio codec
    if (avcodec_open2(audioCodecContext, codec, NULL) < 0) {
        printf("Failed to open audio codec\n");
        return true;
    }

	// Set up SDL audio device
    SDL_AudioSpec wantedSpec, obtainedSpec;
    wantedSpec.freq = audioCodecContext->sample_rate;
    switch (audioCodecContext->sample_fmt) {
    case AV_SAMPLE_FMT_U8:
    case AV_SAMPLE_FMT_U8P:
        wantedSpec.format = AUDIO_U8;
        break;
    case AV_SAMPLE_FMT_S16:
    case AV_SAMPLE_FMT_S16P:
        wantedSpec.format = AUDIO_S16SYS;
        break;
    case AV_SAMPLE_FMT_S32:
    case AV_SAMPLE_FMT_S32P:
        wantedSpec.format = AUDIO_S32SYS;
        break;
    case AV_SAMPLE_FMT_FLT:
    case AV_SAMPLE_FMT_FLTP:
        wantedSpec.format = AUDIO_F32SYS;
        break;
    }
    wantedSpec.channels = 2;
    wantedSpec.silence = 0;
    wantedSpec.samples = audioCodecContext->frame_size;
    wantedSpec.callback = NULL;
    wantedSpec.userdata = NULL;

	// Open the audio device
    audioDevice = SDL_OpenAudioDevice(NULL, 0, &wantedSpec, &obtainedSpec, 0);
    if (audioDevice == 0) {
        printf("Failed to open audio device %s\n", SDL_GetError());
        return true;
    }

    SDL_AddTimer((1000 * wantedSpec.samples) / wantedSpec.freq, audioTimerCallback, this);

	// Start audio playback
    SDL_PauseAudioDevice(audioDevice, 0);

    system("cls");
    return false;
}

void VideoPlayer::ProcessAudio()
{
    // Allocate memory for an audio frame
    AVFrame* frame = av_frame_alloc();
    if (!frame) {
        return;
    }

    Uint8* stream;
    AVPacket packet;

    // Process each packet in the audio buffer
    if (!audioBuffer.empty()) {
        // Get the first packet from the audio buffer
        packet = audioBuffer.front();
        audioBuffer.pop();

        // Send the packet to the audio decoder
        int ret = avcodec_send_packet(audioCodecContext, &packet);
        if (ret < 0) {
            av_frame_free(&frame);
            return;
        }

        // Receive decoded audio frame
        ret = avcodec_receive_frame(audioCodecContext, frame);
        if (ret < 0) {
            av_frame_free(&frame);
            return;
        }

        // Get the pointer to the audio data
        stream = reinterpret_cast<Uint8*>(frame->data[0]);

        // Queue the audio data for playback
        SDL_QueueAudio(audioDevice, stream, frame->linesize[0]);

        // Unreference the frame
        av_frame_unref(frame);
    }
    
    // Free memory allocated for the audio frame
    av_frame_free(&frame); 
}

bool VideoPlayer::ConvertPixels(int videoIndex, int audioIndex)
{
    // Calculate time per frame based on the average frame rate of the video stream
    int time = 1000 * formatContext->streams[videoIndex]->avg_frame_rate.den
                    / formatContext->streams[videoIndex]->avg_frame_rate.num;
   
    AVPacket packet; 
    
    // Allocate memory for source and destination frames
    AVFrame* srcFrame = av_frame_alloc();
    if (!srcFrame) {
        return false;
    }
    AVFrame* dstFrame = av_frame_alloc();
    if (!dstFrame) {
        av_frame_free(&srcFrame);
        return false;
    }

    // Allocate memory for the image data of the destination frame
    AllocImage(dstFrame);

    // Create a scaling context to convert the pixel format of the video frames
    struct SwsContext* sws_ctx = sws_getContext(
        videoCodecContext->width, videoCodecContext->height, videoCodecContext->pix_fmt,
        videoCodecContext->width, videoCodecContext->height, AV_PIX_FMT_YUV420P,
        SWS_BILINEAR, NULL, NULL, NULL);

    bool videoFinished = true;  // Assuming the video has finished unless we find more packets to process -added

    // Loop through each packet in the video stream
    while (av_read_frame(formatContext, &packet) >= 0 && running) 
    {
        videoFinished = false;  // Found a packet, so the video is not finished - added

        if (packet.stream_index == videoIndex) 
        {
            // Send packet to video decoder
            avcodec_send_packet(videoCodecContext, &packet);
            // Receive decoded frame
            int ret = avcodec_receive_frame(videoCodecContext, srcFrame);
            if (ret) continue;

            // Scale the source frame to the destination frame
            sws_scale(sws_ctx, (uint8_t const* const*)srcFrame->data,
                srcFrame->linesize, 0, videoCodecContext->height,
                dstFrame->data, dstFrame->linesize);

            // Update the SDL texture with the scaled YUV data
            SDL_UpdateYUVTexture(renderTexture, &renderRect,
                dstFrame->data[0], dstFrame->linesize[0],
                dstFrame->data[1], dstFrame->linesize[1],
                dstFrame->data[2], dstFrame->linesize[2]
            );

            // Render the video
            RenderCutscene();

            // Wait for the specified time before processing the next frame
            SDL_Delay(time);

            // Unreference the packet to release its resources
            av_packet_unref(&packet);
        }
        else if (packet.stream_index == audioIndex) 
        {
            // Push to the audio buffer
            audioBuffer.push(packet);
        }
    }

    // Free memory allocated for the destination frame
    av_freep(&dstFrame->data[0]);

    // Free memory allocated for source and destination frames
    av_frame_free(&srcFrame);
    av_frame_free(&dstFrame);

    //return false;
    return videoFinished;  // Return true if the video has finished - added
}

bool VideoPlayer::AllocImage(AVFrame* image)
{
    // Set the pixel format, width, and height of the image frame
    image->format = AV_PIX_FMT_YUV420P;
    image->width = videoCodecContext->width;
    image->height = videoCodecContext->height;

    // Allocate memory for the image frame
    av_image_alloc(image->data, image->linesize,
        image->width, image->height, (AVPixelFormat)image->format, 32);

    return false;
}

void VideoPlayer::RenderCutscene()
{
    SDL_RenderClear(app->render->renderer);

    SDL_RenderCopy(app->render->renderer, renderTexture, NULL, NULL);

    SDL_RenderPresent(app->render->renderer);
}

bool VideoPlayer::Update(float dt)
{
    return true;
}

bool VideoPlayer::CleanUp()
{
    LOG("Freeing cutscene player");

	// Close the vide and audio codec contexts
    avcodec_close(videoCodecContext);
    avcodec_free_context(&videoCodecContext);
    videoCodecContext = nullptr;

    avcodec_close(audioCodecContext);
    avcodec_free_context(&audioCodecContext);
    audioCodecContext = nullptr;

	// Close and free the format contexts
    avformat_close_input(&formatContext);
	avformat_free_context(formatContext);
    formatContext = nullptr;

	// Close audio device
    SDL_CloseAudioDevice(audioDevice);
    audioDevice = 0;

    // Destroy textures
	SDL_DestroyTexture(renderTexture);
    renderTexture = nullptr;
    
    //Clean cola de audio
    while (!audioBuffer.empty()) {
        AVPacket packet = audioBuffer.front();
        av_packet_unref(&packet);
        audioBuffer.pop();
    }

    return true;
}