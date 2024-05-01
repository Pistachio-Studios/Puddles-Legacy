#ifndef __ANIMATION_H__
#define __ANIMATION_H__

#include "Utils/SString.h"

#include <SDL_render.h>
#include "SDL2/SDL_rect.h"

#define MAX_FRAMES 30

class Animation
{
public:
	SString name;
	SDL_Texture* texture = nullptr;
	float speed = 1.0f;
	SDL_Rect frames[MAX_FRAMES];
	bool loop = true;
	// Allows the animation to keep going back and forth
	bool pingpong = false;
	bool flip = false;
	//int durations[MAX_FRAMES]; //03 creamos array de duraciones

	float currentFrame = 0.0f;
	int totalFrames = 0;

private:
	int loopCount = 0;
	int pingpongCount = 0;
	int pingpongDirection = 1;
	//float timeStep = 1000 / 60; //04 tiempo que se le resta a la duración
	//float timeLeft = 0.0f; //04 tiempo de visualización que le queda al frame actual

public:

	void PushBack(const SDL_Rect& rect)
	{
		frames[totalFrames++] = rect;
	}
	//Pushback para tener en cuenta la duración
	//void PushBack(const SDL_Rect& rect, int duration)
	//{
	//	//03 guardamos la duración de cada frame 
	//	durations[totalFrames] = duration;
	//	if (totalFrames == 0) timeLeft = duration; //first time
	//	frames[totalFrames++] = rect;
	//}

	void Reset()
	{
		currentFrame = 0;
	}
	
	void ResetLoopCount()
	{
		loopCount = 0;
	}


	bool HasFinished()
	{
		return !loop && !pingpong && loopCount > 0;
	}

	void Update(float dt)
	{
		currentFrame += speed * (dt/1000); //revisar esto no se muy bien como multiplicar por dt aqui por no ser 0.016 sino 16.666
		if (currentFrame >= totalFrames)
		{
			currentFrame = (loop || pingpong) ? 0.0f : totalFrames - 1;
			++loopCount;
			

			if (pingpong) {
				pingpongDirection = -pingpongDirection;
				if (currentFrame == 0) {
					pingpong = false;
				}
			}
				
		}
	}
	//Update para tener en cuenta duraciones, todas las animaciones tendrían que estar hechas desde el tmx para esto
	//void Update()
	//{
	//	//06 cuando baja de 0 pasamos a dibujar el siguiente frame 
	//	if (timeLeft <= 0) {
	//		currentFrame++;
	//		if (currentFrame >= totalFrames)
	//		{
	//			currentFrame = (loop || pingpong) ? 0 : totalFrames - 1;
	//			++loopCount;
	//			if (pingpong)
	//				pingpongDirection = -pingpongDirection;
	//		}
	//		//06 Le asignamos a la variable timeLeft la duración del nuevo frame
	//		timeLeft += durations[currentFrame];
	//	}
	//	//05 vamos restando a la duración del frame
	//	timeLeft -= timeStep * speed;
	//}

	int GetLoopCount() const 
	{
		return loopCount;
	}
	
	int GetPingPongCount() const 
	{
		return pingpongCount;
	}
	
	float GetCurrentFrameCount() const
	{
		return currentFrame;
	}

	const SDL_Rect& GetCurrentFrame() const
	{
		int actualFrame = currentFrame;
		if (pingpongDirection == -1)
			actualFrame = totalFrames - currentFrame;

		return frames[actualFrame];
	}
};

#endif