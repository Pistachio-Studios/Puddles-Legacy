#include <cmath>
#include <map>


#ifndef PI
#define PI 3.1415926545
#endif

//SDL version still testing, the code below is extracted from @nicolausYes project

double easeInSine(double t) {
    return SDL_min(sin(1.5707963 * t), 1.0);
}

double easeOutSine(double t) {
    return SDL_min(1 + sin(1.5707963 * (--t)),1);
}

double easeInOutSine(double t) {
    return SDL_min(0.5 * (1 + sin(3.1415926 * (t - 0.5))), 1);
}

double easeInQuad(double t) {
    return SDL_min(t * t, 1.0);
}

double easeOutQuad(double t) {
    return SDL_min(t * (2 - t), 1);
}

double easeInOutQuad(double t) {
    return SDL_min(t < 0.5 ? 2 * t * t : t * (4 - 2 * t) - 1, 1);
}

double easeInCubic(double t) {
    return SDL_min(t * t * t, 1);
}

double easeOutCubic(double t) {
    return SDL_min(1 + (--t) * t * t, 1);
}

double easeInOutCubic(double t) {
    return SDL_min(t < 0.5 ? 4 * t * t * t : 1 + (--t) * (2 * (--t)) * (2 * t), 1);
}

double easeInExpo(double t) {
    return SDL_min((pow(2, 8 * t) - 1) / 255, 1);
}

double easeOutExpo(double t) {
    return SDL_min(1 - pow(2, -8 * t), 1);
}

double easeInOutExpo(double t) {
    if (t < 0.5) {
        return SDL_min((pow(2, 16 * t) - 1) / 510, 1);
    }
    else {
        return SDL_min(1 - 0.5 * pow(2, -16 * (t - 0.5)), 1);
    }
}

double easeInBounce(double t) {
    return SDL_min(pow(2, 6 * (t - 1)) * abs(sin(t * PI * 3.5)), 1);
}

double easeOutBounce(double t) {
    return SDL_min(1 - pow(2, -6 * t) * abs(cos(t * PI * 3.5)), 1);
}

double easeInOutBounce(double t) {
    if (t < 0.5) {
        return SDL_min(8 * pow(2, 8 * (t - 1)) * abs(sin(t * PI * 7)), 1);
    }
    else {
        return SDL_min(1 - 8 * pow(2, -8 * t) * abs(sin(t * PI * 7)), 1);
    }
}
