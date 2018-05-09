#ifndef AN_PALM_PULSE_H
#define	AN_PALM_PULSE_H

#include "npAnimation.h"
#include "fft.h"

class anPalmPulseSA : public npAnimation {
public:
    anPalmPulseSA();
    anPalmPulseSA( npDisplay* pDisplay, mode_t = MODE_NULL, int frames = 0, opt_t = OPT_NULL, scale_t = SCALE_1_1 );
    ~anPalmPulseSA();
    int Draw();
        
private:
    hsv_t h_color;
    sprite index, middle, ring, pinky, palm;    
    float angle;    
    int step;       
};

#endif
