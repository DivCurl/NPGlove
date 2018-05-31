#ifndef AN_VARIABLE_STROBE_H
#define	AN_VARIABLE_STROBE_H

#include "npAnimation.h"
#include "types.h"
#include "fft.h"

class anVariablePulseSA : public npAnimation {
public:
    anVariablePulseSA();
    anVariablePulseSA( npDisplay* pDisplay, mode_t = MODE_NULL, int frames = 0, opt_t = OPT_NULL, scale_t = SCALE_1_1 );
    ~anVariablePulseSA();
    int Draw();
        
private:
    hsv_t h_color;
    // sprite index, middle, ring, pinky, palm;    
    float angle, palmAngle = 0.f;    
    int step;       
};

#endif
