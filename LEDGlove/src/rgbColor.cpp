#include "./include/rgbColor.h"

using namespace std;

rgbw_t rgbwGetByAngle( float angle, uint8_t whiteTemp ) {
    if ( whiteTemp == 255 ) {   // only if white color is max
        return ( rgbw_t { 0, 0, 0, 255 } );
    }
    
    uint8_t r, g, b;
    // double angle = fmod( inAngle, 360 );
    /*
    while ( angle >= 360.0f ) {
        angle -= 360.0f;
    }
    */
    angle = fmodf( angle, 360 );
    // 0 deg = 255, 0 0
    // 60 deg = 255, 255, 0
    // 120 deg = 0, 255, 0
    // 180 deg = 0, 255, 255
    // 240 deg = 0, 0 255
    // 300 deg = 255, 0, 255
    // delta-x = 255   
   
    
    // increase g value, hold red at max
    if ( ( angle >= 0 ) && ( angle < 60 ) ) {
        r = 255;
        g = ( angle * 4.25 );
        b = 0;
        
        return ( rgbw_t { r, g, b, whiteTemp } );
    }

    // decrease red value, hold g at max
    if ( ( angle >= 60 ) && ( angle < 120 ) ) {
        r = ( 255 - (angle - 60) * 4.25 );
        g = 255.;
        b = 0;
        
        return ( rgbw_t { r, g, b, whiteTemp } );
    }

    // increase blue value, hold g at max
    if ( ( angle >= 120 ) && ( angle < 180 ) ) {
        r = 0;
        g = 255;
        b = ( (angle - 120) * 4.25 );
        
        return ( rgbw_t { r, g, b, whiteTemp } );
    }

    // decrease G value, hold B at max
    if ( ( angle >= 180 ) && ( angle < 240 ) ) {
        r = 0;
        g = ( 255 - (angle-180) * 4.25 );
        b = 255;
        
        return ( rgbw_t { r, g, b, whiteTemp } );
    }

    // increase R value, hold B at max
    if ( ( angle >= 240 ) && ( angle < 300 ) ) {
        r = ( (angle - 240) * 4.25 );
        g = 0;
        b = 255;
        
        return ( rgbw_t { r, g, b, whiteTemp } );
    }

    // decrease B value, hold R at max
    if ( ( angle >= 300 ) && ( angle < 360 ) ) {
        r = 255;
        g = 0;
        b = ( 255 - (angle - 300) * 4.25 );
        
        return ( rgbw_t { r, g, b, whiteTemp } );
    }  
}

rgbw_t HSVtoRGB( hsv_t &color ) {
   float c, x, m, _r, _g, _b;
   uint8_t r, g, b;
   
   // Ensure hue angle is within [ 0, 360 )
   if ( color.h >= 360.f ) {
       color.h = fmodf( color.h, 360.f );
   }
    
    c = color.v * color.s;
    m = color.v - c;
    
    /*
    tmp = fmodf( ( color.h / 60 ), 2 );
    asm volatile ("nop");
    tmp = 1 - fabs( tmp - 1 );
    asm volatile ("nop");
    tmp *= c;
    asm volatile ("nop");
    x = tmp;
    */
    x = c * ( 1 - fabs( fmodf( ( color.h / 60 ), 2 ) - 1 ) );    
    
    if ( 0.f <= color.h && color.h < 60.f ) {
        _r = c;
        _g = x;
        _b = 0;
    }     
    if ( 60.f <= color.h && color.h < 120.f ) {
        _r = x;
        _g = c;
        _b = 0;        
    } 
    if ( 120.f <= color.h && color.h < 180.f ) {
        _r = 0;
        _g = c;
        _b = x;
    } 
    if ( 180.f <= color.h && color.h < 240.f ) {
        _r = 0;
        _g = x;
        _b = c;        
    }
    if ( 240.f <= color.h && color.h < 300.f ) {
        _r = x;
        _g = 0;
        _b = c;        
    }
    if ( 300.f <= color.h && color.h < 360.f ) {
        _r = c;
        _g = 0;
        _b = x;
    }
    
    r = ( _r + m ) * 255.0;
    g = ( _g + m ) * 255.0;
    b = ( _b + m ) * 255.0;   
            
    return ( rgbw_t { r, g, b, color.w }  );
}

hsv_t RGBtoHSV( rgbw_t &color ) {
    float hue;
    
    float r = color.r / 255.f;
    float g = color.g / 255.f;
    float b = color.b / 255.f;    
    float cMin, cMax, cDelta, sat, val;
    
    // Determine cMax
    if ( r >= g && r >= b ) {
        cMax = r;
    }     
    if ( g >= r && g >= b ) {
        cMax = g;
    }    
    if ( b >= r && b >= g ) {
        cMax = b;
    }
    
    if ( r <= g && r <= b ) {
        cMin = r;
    }     
    if ( g <= r && g <= b ) {
        cMin = g;
    }    
    if ( b <= r && b <= g ) {
        cMin = b;
    }
    
    cDelta = cMax - cMin;
    
    // Determine hue (angle in degrees)
    if ( cMax == r ) {
        hue = 60.f * ( fmodf( ( g - b ) / cDelta, 6 ) );
    }
    
    if ( cMax == g ) {
        hue = 60.f * ( ( ( b - r ) ) / cDelta + 2 );
    }
    
    if ( cMax == b ) {
        hue = 60.f * ( ( ( r - g  )) / cDelta + 4 );
    }
    
    // We don't deal in negative angles 'round these parts
    if ( hue < 0 ) {
        hue += 360.f;
    }
    
    // Determine saturation (not currently used)
    if ( cMax == 0.f ) {
        sat = 0;
    } else {
        sat = ( cDelta / cMax );
    }
    
    // Determine value (white)
    val = cMax;
    
    return ( hsv_t { hue, sat, val, color.w } ); 
}

 