#ifndef AN_PALM_STROBE_H
#define	AN_PALM_STROBE_H

#include "npAnimation.h"
#include "types.h"

class anPalmStrobe : public npAnimation {
public:
    anPalmStrobe();
    anPalmStrobe( npDisplay* pDisplay, mode_t = MODE_NULL, int frames = 0, opt_t = OPT_NULL, scale_t = SCALE_1_1 );
    ~anPalmStrobe();
    int Draw();
        
private:
    hsv_t h_color;
    sprite index, middle, ring, pinky, palm;    
    float angle;    
    int step;       
};

#endif
