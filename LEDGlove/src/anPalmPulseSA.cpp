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
    counter ctrFade ( 150, ON );
    // counter ctrMain ( 100, ON );
    const int avgSamples = 25;
    const uint8_t palmPix = 20;
    const uint8_t fingerPix = 3;   
    int volPeakFinger, volPeakPalm;
    float avgVol = 0.0f;
    float peakVol = 0.f;
    
        
    vector<pixel> pinky; // array of pixel vectors for each frequency bar to be drawn  
    vector<pixel> ring; // array of pixel vectors for each frequency bar to be drawn
    vector<pixel> middle; // array of pixel vectors for each frequency bar to be drawn
    vector<pixel> index; // array of pixel vectors for each frequency bar to be drawn
    vector<pixel> palm; // array of pixel vectors for each frequency bar to be drawn
    
    /*
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
    */
    
    /*
    for ( int i = 0; i < 2; i++ ) {
        index.push_back( 
            pixel( 
                indexPix[ i ],
                rgbwGetByAngle ( 300 ),
                255 
            )
        );
    }
    */
                    
    while ( ( framesDrawn < frames ) || modeFlags.test( MODE_REPEAT ) ) {        
        if ( !skip ) {                
            
            // if ( ctrMain.Done() ) {
            //    ctrMain.Reset();
                
                avgVol = 0.f;
                peakVol = -60.f;

                if ( FFTBufferReady ) {
                    ComputeFFT();                
                }  

                // Get average volume
                for ( int i = 1; i <= avgSamples; ++i ) {
                    avgVol += singleSidedFFT[ i  ];
                    if ( singleSidedFFT[ i ] > peakVol ) {
                        peakVol = singleSidedFFT[ i ];
                    }
                }

                avgVol /= avgSamples;                    
                // testAvg = avgVol;
                // testPeak = peakVol;

                // numSplats = (int) Remap ( avgVol, -60, 0, 0, 5 ) ;
                volPeakFinger = (int) Remap ( peakVol, -55, -5, 0, 5 ) ;
                               

                // Evaluate if bars need to be resized
                if ( volPeakFinger > 1 ) { 
                    pinky.clear();  
                    ring.clear();
                    middle.clear();
                    index.clear(); 
                    float pinkyAngle = rand() % 360;
                    float ringAngle = rand() % 360;
                    float middleAngle = rand() % 360;
                    float indexAngle = rand() % 360;
                    for ( int i = 0; i <= volPeakFinger; i++ ) {
                        // Add a new bar so long as we're not already max'd out
                        if ( pinky.size() < fingerPix ) {
                            
                            pinky.push_back( 
                                pixel( 
                                    pinkyPix[ pinky.size() ],
                                    rgbwGetByAngle ( pinkyAngle ),
                                    255 
                                )
                            );
                            
                            ring.push_back( 
                                pixel( 
                                    ringPix[ ring.size() ],
                                    rgbwGetByAngle ( ringAngle ),
                                    255 
                                )
                            );
                            
                            middle.push_back( 
                                pixel( 
                                    middlePix[ middle.size() ],
                                    rgbwGetByAngle ( middleAngle ),
                                    255 
                                )
                            );
                            
                            index.push_back( 
                                pixel( 
                                    indexPix[ index.size() ],
                                    rgbwGetByAngle ( indexAngle ),
                                    255 
                                )
                            );
                        }
                    }
                } else {
                    // Pop one pixel off the top
                    if ( ctrFade.Done() ) {
                        ctrFade.Reset();
                        if ( pinky.size() > 0 ) {
                            pinky.pop_back();
                            ring.pop_back();
                            middle.pop_back();
                            index.pop_back();
                        }
                    }
                }

                Blit( pinky );
                Blit( ring );
                Blit( middle );
                Blit( index );
                // Blit( palm );
                

                RefreshDisplay( FB_CLEAR );
                
            // }          
        } // not skipped 
    } // end main loop         
    
    return ( MODE_NEXT );
}