#include "include/anPalmPulseSA.h"
// #include "include/types.h"

// Refer to fft.h/fft.cpp
extern bool analyzerRun;
extern volatile bool FFTBufferReady;
extern short singleSidedFFT[ N ];
float volAvg, avgIndex, avgMiddle, avgRing, avgPinky, avgPalm;
float peakVol, peakIndex, peakMiddle, peakRing, peakPinky, peakPalm;


anPalmPulseSA::anPalmPulseSA( npDisplay* pDisplay, mode_t mode, int frames, opt_t opts, scale_t customScaling ) 
: npAnimation( pDisplay, mode, frames, opts, customScaling ) { 
    analyzerRun = 1;   
    InitFFT();
}

anPalmPulseSA::~anPalmPulseSA() { 
    analyzerRun = 0;
}

// STRICTLY FOR TESTING //
int anPalmPulseSA::Draw() {
    int strobe = 0;    
    counter ctrFade ( 50, ON );
    counter ctrColorShift ( 50, ON );
    counter ctrMain ( 10, ON );
    counter ctrStrobe ( 25, ON );
    // const int samples = 28; // @ 5K effective freq, 161hz / bin
    const uint8_t palmPix = 20;
    const uint8_t fingerPix = 3; 
    const int dbMin = -60;
    const int dbMax = -20;
    uint8_t palm_y_coords[ palmPix ];    
            
    vector<pixel> pinky; 
    vector<pixel> ring; 
    vector<pixel> middle; 
    vector<pixel> index; 
    vector<pixel> palm;
    
    // Init palm y-coords
    for ( int i = 0; i < palmPix; ++i ) {
        palm_y_coords[ i ] = i + 12;
    }
                    
    while ( ( framesDrawn < frames ) || modeFlags.test( MODE_REPEAT ) ) {        
        if ( CheckAnimSwitch() ) {
            return ( MODE_NEXT );
        }
         
        /*
        if ( ret == MODE_PREV || ret == MODE_NEXT ) {
            break;  // break while loop and return to main signaling next/prev animation to be drawn
        } 
        */   

        if ( FFTBufferReady ) {
            ComputeFFT();                
        }  

        if ( ctrStrobe.Done() ) {
            ctrStrobe.Reset();
            strobe ^= 1;
        }

        if ( ctrColorShift.Done() ) {
            ctrColorShift.Reset();
            angle += 5.0;

            if ( angle > 360 ) {
                angle -= 360;
            }
        }

        volAvg = 0;
        peakVol = -60.f;
        // Get total volume avg and peak for index
        for ( int i = 3; i <= 7; ++i ) {
            volAvg += singleSidedFFT[ i ];
            if ( singleSidedFFT[ i ] > peakVol ) {
                peakVol = singleSidedFFT[ i ];
            }
        }
        volAvg /= 7.f;         
        avgIndex = Remap( volAvg, dbMin, dbMax, 0, 5 );
        peakIndex = Remap( peakVol, -60, -15, 0, 5 );

        volAvg = 0;
        peakVol = -60.f;
        // Get total volume avg and peak for middle
        for ( int i = 8; i <= 12; ++i ) {
            volAvg += singleSidedFFT[ i ];
            if ( singleSidedFFT[ i ] > peakVol ) {
                peakVol = singleSidedFFT[ i ];
            }
        }
        volAvg /= 7.f;        
        avgMiddle = Remap( volAvg, dbMin, dbMax, 0, 5 );
        peakMiddle = Remap( peakVol, dbMin, dbMax, 0, 6 );

        volAvg = 0;
        peakVol = -60.f;
        // Get total volume avg and peak for ring
        for ( int i = 13; i <= 16; ++i ) {
            volAvg += singleSidedFFT[ i ];
            if ( singleSidedFFT[ i ] > peakVol ) {
                peakVol = singleSidedFFT[ i ];
            }
        }
        volAvg /= 7.f;       
        avgRing = Remap( volAvg, dbMin, dbMax, 0, 5 );
        peakRing = Remap( peakVol, dbMin, dbMax, 0, 6 );

        volAvg = 0;
        peakVol = -60.f;
        // Get total volume avg and peak for pinky
        for ( int i = 17; i < 20; ++i ) {
            volAvg += singleSidedFFT[ i ];
            if ( singleSidedFFT[ i ] > peakVol ) {
                peakVol = singleSidedFFT[ i ];
            }
        }
        volAvg /= 7.f;     
        avgPinky = Remap( volAvg, dbMin, dbMax, 0, 5 );
        peakPinky = Remap( peakVol, dbMin, dbMax, 0, 6 );

        volAvg = 0;
        peakVol = -60.f;
        // Get total volume avg and peak for palm
        for ( int i = 1; i <= 25; ++i ) {
            volAvg += singleSidedFFT[ i ];
            if ( singleSidedFFT[ i ] > peakVol ) {
                peakVol = singleSidedFFT[ i ];
            }
        }
        volAvg /= 28.f;     
        // avgPalm = Remap( volAvg, dbMin, dbMax, 0, 20 );
        peakPalm = Remap( peakVol, -55, 0, 0, 10 );

        // Drop the peaks by one
        if ( ctrFade.Done() ) {
            ctrFade.Reset();

            if ( index.size() > 0 ) {
                index.pop_back();            
            }
            if ( middle.size() > 0 ) {
                middle.pop_back();            
            }
            if ( ring.size() > 0 ) {
                ring.pop_back();            
            }
            if ( pinky.size() > 0 ) {
                pinky.pop_back();            
            }                
            if ( palm.size() > 0 ) {
                palm.pop_back();            
            }
        }            

        if ( ctrMain.Done() ) { 
            ctrMain.Reset();       

            if ( peakIndex > 1 ) { 
                index.clear();                
                // angle = rand() % 360;

                for ( int i = 0; i <= peakIndex; i++ ) {
                    // Add a new bar so long as we're not already max'd out
                    if ( index.size() < fingerPix ) {
                        index.push_back( 
                            pixel( 
                                indexPix[ index.size() ],
                                rgbwGetByAngle ( angle + ( i * 2.0 ) ),
                                255 
                            )
                        );                            
                    }
                }
            }

            if ( peakMiddle > 1 ) { 
                middle.clear();                     
                // angle = rand() % 360;

                for ( int i = 0; i <= peakMiddle; i++ ) {
                    // Add a new bar so long as we're not already max'd out
                    if ( middle.size() < fingerPix ) {
                        middle.push_back( 
                            pixel( 
                                middlePix[ middle.size() ],
                                rgbwGetByAngle ( angle + ( i * 2.0 ) + 20 ),
                                255 
                            )
                        );                            
                    }
                }
            }

            if ( peakRing > 1 ) { 
                ring.clear();                     
                // angle = rand() % 360;

                for ( int i = 0; i <= peakRing; i++ ) {
                    // Add a new bar so long as we're not already max'd out
                    if ( ring.size() < fingerPix ) {
                        ring.push_back( 
                            pixel( 
                                ringPix[ ring.size() ],
                                rgbwGetByAngle ( angle + ( i * 2.0 ) + 40 ),
                                255 
                            )
                        );                            
                    }
                }
            }

            if ( peakPinky > 1 ) { 
                pinky.clear();                     
                // angle = rand() % 360;

                for ( int i = 0; i <= peakPinky; i++ ) {
                    // Add a new bar so long as we're not already max'd out
                    if ( pinky.size() < fingerPix ) {
                        pinky.push_back( 
                            pixel( 
                                pinkyPix[ pinky.size() ],
                                rgbwGetByAngle ( angle + ( i * 2.0 ) + 60 ),
                                255 
                            )
                        );                            
                    }
                }
            }

            if ( peakPalm > 3 ) { 
                palm.clear();         

                // Shuffle up the coords to make things interesting
                for ( int i = 0; i < peakPalm; i++ ) {
                    int rnd = rand() % 20;
                    int tmp = palm_y_coords[ i ];
                    palm_y_coords[ i ] = palm_y_coords[ rnd ];
                    palm_y_coords[ rnd ] = tmp;                        
                }

                for ( int i = 0; i <= peakPalm; i++ ) {
                    // Add a new bar so long as we're not already max'd out
                    if ( palm.size() < palmPix ) {
                        palm.push_back( 
                            pixel( 
                                // palmPix[ palm.size() ],
                                coord2d_t { 0, palm_y_coords[ i ] },
                                rgbwGetByAngle ( rand() % 360, rand() % 30 ),
                                255 
                            )
                        );                            
                    }
                }
            }
        }

        if ( !strobe ) {
            Blit( pinky );
            Blit( ring );
            Blit( middle );
            Blit( index );
            Blit( palm );                
        }

        RefreshDisplay( FB_CLEAR );                  

    } // end main loop         
    
    return ( MODE_NEXT );
}