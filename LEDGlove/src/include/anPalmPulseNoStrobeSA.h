#ifndef AN_PALM_PULSE_NO_STROBE_H
#define	AN_PALM_PULSE_NO_STROBE_H

#include "npAnimation.h"
#include "fft.h"

class anPalmPulseNoStrobeSA : public npAnimation {
public:
    anPalmPulseNoStrobeSA();
    anPalmPulseNoStrobeSA( npDisplay* pDisplay, mode_t = MODE_NULL, int frames = 0, opt_t = OPT_NULL, scale_t = SCALE_1_1 );
    ~anPalmPulseNoStrobeSA();
    int Draw();
        
private:
    hsv_t h_color;
    // sprite index, middle, ring, pinky, palm;    
    float angle;    
    int step;       
};

#endif
