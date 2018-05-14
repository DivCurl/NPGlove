#ifndef TYPES_H
#define	TYPES_H

#include <cstdint>

#define PI 3.141592654f
#define PI_OVER_180 ( PI / 180.0f )

struct coord2d_t {
    int x;
    int y;
    
    coord2d_t( int x = 0, int y = 0 ) : x( x ), y( y ) { }
    
    coord2d_t& operator=( const coord2d_t& coord ) {
        x = coord.x;
        y = coord.y;
        return ( *this );
    }   
    
    coord2d_t& operator+=( const coord2d_t& coord ) {
        x += coord.x;
        y += coord.y;
        return ( *this );
    }
    
    coord2d_t& operator++() {
        x++;
        y++;
        return ( *this );
    }
    
    coord2d_t& operator--() {
        x--;
        y--;
        return ( *this );
    }
    
    coord2d_t& operator-=( const coord2d_t& coord ) {
        x -= coord.x;
        y -= coord.y;
        return ( *this );
    }

    coord2d_t operator+( const coord2d_t& coord ) const {
        return coord2d_t( coord.x + x, coord.y + y );
    }
    
    coord2d_t operator-( const coord2d_t& coord ) const {
        return coord2d_t( coord.x - x, coord.y - y );
    }

    bool operator==( const coord2d_t& coord ) const {
        return ( x == coord.x && y == coord.y );
    }
    
    bool operator>( const coord2d_t& coord ) const {
        return ( x > coord.x && y > coord.y );
    }
    
    bool operator>=( const coord2d_t& coord ) const {
        return ( x >= coord.x && y >= coord.y );
    }
    
    bool operator<=( const coord2d_t& coord ) const {
        return ( x <= coord.x && y <= coord.y );
    }
}; 

struct gModes_t {
    bool colorPacket;
    bool msgPending;
    uint64_t msg;
};

struct rgb_t {
    uint8_t r;
    uint8_t g; 
    uint8_t b;
};

struct rgbw_t {
    uint8_t r;
    uint8_t g; 
    uint8_t b;
    uint8_t w;
};

struct hsv_t {
    float h;
    float s;
    float v;
    uint8_t w;
};

enum opt_t { 
    OPT_NULL,
    OPT_CUSTOM_SCALE
};    // Option flags

enum mode_t { 
    MODE_NULL, 
    MODE_OFF,       // Force all pixels to off 
    MODE_STBY, 
    MODE_PAUSE,     
    MODE_REPEAT, 
    MODE_NEXT, 
    MODE_PREV, 
    MODE_STROBE, 
    MODE_FADEIN, 
    MODE_FADEOUT 
}; // Mode flags

enum scale_t { 
    SCALE_1_1,
    SCALE_1_2,
    SCALE_1_3,
    SCALE_1_4,
    SCALE_1_5,
    SCALE_1_6,
    SCALE_1_7,
    SCALE_1_8,
    SCALE_1_9,
    SCALE_1_10
};

enum { 
    FRAME_SKIP, 
    FRAME_DRAWN 
};

enum colorMode_t { 
    RGB, 
    RGBW 
};

enum fbBlendMode_t {
    FB_BLEND,   // blend with remnant framebuffer
    FB_CLEAR    // clear remnant framebuffer 
};

enum { 
    DIR_UP, 
    DIR_DOWN, 
    DIR_LEFT, 
    DIR_RIGHT, 
    DIR_MAX 
};

enum { 
    OFF,
    ON
};

enum { 
    ID_AN_TEST = -1, 
    ID_AN_NULL, 
    ID_AN_PALM_STROBE,
    ID_AN_PALM_PULSE_SA,
    ID_AN_SPLATTER_SA,
    ID_AN_MAX 
};

enum { 
    ORIGIN_BOTTOM_LEFT, 
    ORIGIN_BOTTOM_RIGHT, 
    ORIGIN_TOP_LEFT, 
    ORIGIN_TOP_RIGHT 
};

enum { 
    COLLIDE_NULL = 0x00,
    COLLIDE_RIGHT = 0x01,
    COLLIDE_TOP = 0x02,
    COLLIDE_LEFT = 0x04,
    COLLIDE_BOTTOM = 0x08,
    COLLIDE_LEFT_RIGHT = COLLIDE_LEFT | COLLIDE_RIGHT            
};

// Glove-specific
const coord2d_t pinkyPix [ 3 ] = {    
    { 0, 0 },
    { 0, 1 },
    { 0, 2 }        
}; 

const coord2d_t ringPix [ 3 ] = {    
    { 0, 5 },
    { 0, 4 },
    { 0, 3 }        
}; 

const coord2d_t middlePix [ 3 ] = {    
    { 0, 6 },
    { 0, 7 },
    { 0, 8 }        
}; 

const coord2d_t indexPix [ 3 ] = {    
    { 0, 11 },
    { 0, 10 },
    { 0, 9 }        
}; 

const coord2d_t palmColumnPix [ 4 ] [ 5 ] = {    
    { { 0, 12 }, { 0, 12 }, { 0, 20 }, { 0, 27 }, { 0, 28 } },
    { { 0, 13 }, { 0, 18 }, { 0, 21 }, { 0, 26 }, { 0, 29 } },
    { { 0, 14 }, { 0, 17 }, { 0, 22 }, { 0, 25 }, { 0, 30 } },
    { { 0, 15 }, { 0, 16 }, { 0, 23 }, { 0, 24 }, { 0, 31 } }
}; 

    

#endif

