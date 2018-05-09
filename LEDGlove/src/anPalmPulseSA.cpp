#include "include/anPalmPulseSA.h"
// #include "include/types.h"

// Refer to fft.h/fft.cpp
extern bool analyzerRun;
extern volatile bool FFTBufferReady;
extern short singleSidedFFT[ N ];

anPalmPulseSA::anPalmPulseSA( npDisplay* pDisplay, mode_t mode, int frames, opt_t opts, scale_t customScaling ) 
: npAnimation( pDisplay, mode, frames, opts, customScaling ) { 
    analyzerRun = 1;    // used by T4 ISR
    InitFFT();
}

anPalmPulseSA::~anPalmPulseSA() { 
    analyzerRun = 0;
}

// STRICTLY FOR TESTING //
int anPalmPulseSA::Draw() {
    // Main animation loop   
    const uint8_t palmPix = 20;
    const uint8_t fingerPix = 3;   
    
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
    
    counter ctrMain ( 150, ON );
    
    // [0, 2]    
    vector<pixel> pinky; // array of pixel vectors for each frequency bar to be drawn
    
    // [5, 3]
    vector<pixel> ring; // array of pixel vectors for each frequency bar to be drawn
    
    // [6, 8]
    vector<pixel> middle; // array of pixel vectors for each frequency bar to be drawn
    
    // [11, 9]
    vector<pixel> index; // array of pixel vectors for each frequency bar to be drawn
    
    
    pinky.push_back( 
        pixel( 
            pinkyPix[0],
            rgbwGetByAngle ( 0 ),
            255 
        )
    ); 
            
    pinky.push_back( 
        pixel( 
            pinkyPix[1],
            rgbwGetByAngle ( 0 ),
            255 
        )
    );
            
    pinky.push_back( 
        pixel( 
            pinkyPix[2],
            rgbwGetByAngle ( 0 ),
            255 
        )
    );
    
    // 
    
    ring.push_back( 
        pixel( 
            ringPix[0],
            rgbwGetByAngle ( 80 ),
            255 
        )
    ); 
            
    ring.push_back( 
        pixel( 
            ringPix[1],
            rgbwGetByAngle ( 80 ),
            255 
        )
    );
    
    //
    
    middle.push_back( 
        pixel( 
            middlePix[0],
            rgbwGetByAngle ( 120 ),
            255 
        )
    ); 
            
    middle.push_back( 
        pixel( 
            middlePix[1],
            rgbwGetByAngle ( 120 ),
            255 
        )
    );
                
    
    while ( ( framesDrawn < frames ) || modeFlags.test( MODE_REPEAT ) ) {        
        if ( !skip ) {                
            
            if ( ctrMain.Done() ) {
                ctrMain.Reset();
        
                Blit( pinky );
                Blit( ring );
                Blit( middle );
                Blit( index );
            
                RefreshDisplay( FB_CLEAR );
                
            }          
        } // not skipped 
    } // end main loop         
    
    return ( MODE_NEXT );
}