#ifndef __EASINGS_H__
#define __EASINGS_H__

#include <cmath>
#include <map>

#define PI 3.14159265358979323846

class Easings {
public:
    static double easeInSine(double t) {
        return SDL_min(sin(1.5707963 * t), 1.0);
    }

    static double easeOutSine(double t) {
        return SDL_min(1 + sin(1.5707963 * (--t)),1);
    }

    static double easeInOutSine(double t) {
        return SDL_min(0.5 * (1 + sin(3.1415926 * (t - 0.5))), 1);
    }

    static double easeInQuad(double t) {
        return SDL_min(t * t, 1.0);
    }

    static double easeOutQuad(double t) {
        return SDL_min(t * (2 - t), 1);
    }

    static double easeInOutQuad(double t) {
        return SDL_min(t < 0.5 ? 2 * t * t : t * (4 - 2 * t) - 1, 1);
    }

    static double easeInCubic(double t) {
        return SDL_min(t * t * t, 1);
    }

    static double easeOutCubic(double t) {
        return SDL_min(1 + (--t) * t * t, 1);
    }

    static double easeInOutCubic(double t) {
        return SDL_min(t < 0.5 ? 4 * t * t * t : 1 + (--t) * (2 * (--t)) * (2 * t), 1);
    }

    static double easeInExpo(double t) {
        return SDL_min((pow(2, 8 * t) - 1) / 255, 1);
    }

    static double easeOutExpo(double t) {
        return SDL_min(1 - pow(2, -8 * t), 1);
    }

    static double easeInOutExpo(double t) {
        if (t < 0.5) {
            return SDL_min((pow(2, 16 * t) - 1) / 510, 1);
        }
        else {
            return SDL_min(1 - 0.5 * pow(2, -16 * (t - 0.5)), 1);
        }
    }

    static double easeInBounce(double t) {
        return SDL_min(pow(2, 6 * (t - 1)) * abs(sin(t * PI * 3.5)), 1);
    }

    static double easeOutBounce(double t) {
        return SDL_min(1 - pow(2, -6 * t) * abs(cos(t * PI * 3.5)), 1);
    }

    static double easeInOutBounce(double t) {
        if (t < 0.5) {
            return SDL_min(8 * pow(2, 8 * (t - 1)) * abs(sin(t * PI * 7)), 1);
        }
        else {
            return SDL_min(1 - 8 * pow(2, -8 * t) * abs(sin(t * PI * 7)), 1);
        }
    }
};

#endif // __EASINGS_H__
