#ifndef RGBCOLOR_H
#define	RGBCOLOR_H

#define _SUPPRESS_PLIB_WARNING
#define _DISABLE_OPENADC10_CONFIGPORT_WARNING

#include <p32xxxx.h>
#include <plib.h>  
#include <xc.h>
#include <sys/attribs.h>
// #include <cstdlib>
// #include <cmath>
#include <math.h>
#include "types.h"

#define RGB_RED             0
#define RGB_ORANGE          30
#define RGB_YELLOW          60
#define RGB_SPRING_GREEN    90
#define RGB_GREEN           120
#define RGB_CYAN            180
#define RGB_BLUE            240
#define RGB_VIOLET          270
#define RGB_MAGENTA         300

rgbw_t rgbwGetByAngle( float angle, uint8_t whiteTemp = 0 );
hsv_t RGBtoHSV( rgbw_t &color );
rgbw_t HSVtoRGB( hsv_t &color );

#endif
