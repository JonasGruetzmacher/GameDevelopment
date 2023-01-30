#ifndef ANIMATION_H
#define ANIMATION_H

#include "SDL/include/SDL_rect.h"
#include "App.h"
#define MAX_FRAMES 25

class Animation
{
public:
    float speed = 0.15f;
    SDL_Rect frames[MAX_FRAMES];
    bool loop = true;
    // Allows the animation to keep going back and forth
    bool pingpong = false;

private:
    float currentFrame = 0.0f;
    int totalFrames = 0;
    int loopCount = 0;
    int pingpongDirection = 1;

public:

    void PushBack(const SDL_Rect& rect)
    {
        frames[totalFrames++] = rect;
    }

    void Reset()
    {
        currentFrame = 0;
    }

    bool HasFinished()
    {
        return !loop && !pingpong && loopCount > 0;
    }

    void Update()
    {
        currentFrame += speed * app->dt * 0.1;
        if (currentFrame >= totalFrames)
        {
            currentFrame = (loop || pingpong) ? 0.0f : totalFrames - 1;
            ++loopCount;

            if (pingpong)
                pingpongDirection = -pingpongDirection;
        }
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