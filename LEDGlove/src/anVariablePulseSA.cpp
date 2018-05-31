#include "include/anVariablePulseSA.h"
// #include "include/matrix.h"


using namespace std;

// Refer to fft.h/fft.cpp
extern bool analyzerRun;
extern volatile bool FFTBufferReady;
extern short singleSidedFFT[ N ];
// float testAvg, testPeak;

anVariablePulseSA::anVariablePulseSA( npDisplay* pDisplay, mode_t mode, int frames, opt_t opts, scale_t customScaling ) 
: npAnimation( pDisplay, mode, frames, opts, customScaling ) {

    // analyzerRun = 1;   
    // InitFFT();
}

anVariablePulseSA::~anVariablePulseSA() { 
    // analyzerRun = 0;
}

int anVariablePulseSA::Draw() {

    counter ctrStrobe;
    // counter strobeRateLim( 10, ON );
    counter ctrColorFade ( 10, ON );
    int strobe = 0;
    int fadeOffset = 250;
    int fadeRate = 0;
    float avgVol, loAvgVol, midAvgVol, hiAvgVol = -60;
    const int avgSamples = 10;
    
    vector<pixel> fingers;
    // vector<pixel> palm;
    
    while ( ( framesDrawn < frames ) || modeFlags.test( MODE_REPEAT ) ) {                      
        if ( CheckAnimSwitch() ) {
            return ( MODE_NEXT );
        }
        
        if ( FFTBufferReady ) {
            ComputeFFT();                
        }  
        
        if ( ctrStrobe.Done() ) {
                ctrStrobe.Reset();
                strobe ^= 1;
        }
        
        if ( ctrColorFade.Done() ) {
            ctrColorFade.Reset();                        
            fingers.clear();
            // palm.clear();            
            fadeOffset = 0;
            
            for ( int i = 0; i < 3; i++ ) {            
                fingers.push_back( 
                    pixel( 
                        indexPix[ i ],
                        rgbwGetByAngle ( angle + fadeOffset ),
                        255 - ( i * 125 )
                    )
                );
                fadeOffset += 3;
            }
            
            for ( int i = 0; i < 3; i++ ) {
                fingers.push_back( 
                    pixel( 
                        middlePix[ i ],
                        rgbwGetByAngle ( angle + fadeOffset ),
                        255 - ( i * 125 )
                    )
                );
                fadeOffset += 3;
            }
            
            for ( int i = 0; i < 3; i++ ) {
                fingers.push_back( 
                    pixel( 
                        ringPix[ i ],
                        rgbwGetByAngle ( angle + fadeOffset ),
                        255 - ( i * 125 )
                    )
                );
                fadeOffset += 3;
            }
            
            for ( int i = 0; i < 3; i++ ) {
                fingers.push_back( 
                    pixel( 
                        pinkyPix[ i ],
                        rgbwGetByAngle ( angle + fadeOffset ),
                        255 - ( i * 125 )
                    )
                );
                fadeOffset += 3;
            }
            
            for ( int i = 12; i < 21; i++ ) {
                fingers.push_back( 
                    pixel( 
                        coord2d_t { 0, i },
                        rgbwGetByAngle ( angle + fadeOffset ),
                        255
                    )
                );
                fadeOffset += 3;
            }
            
            angle += 3.0;
        }
        
        for ( int i = 1; i <= 10; ++i ) {
            avgVol += singleSidedFFT[ i  ];
        }

        avgVol /= avgSamples;              
        loAvgVol = avgVol;
        avgVol = -60;
        for ( int i = 11; i <= 20; ++i ) {
            avgVol += singleSidedFFT[ i  ];            
        }
        
        avgVol /= avgSamples;              
        midAvgVol = avgVol;
        avgVol = -60;    
        for ( int i = 21; i <= 30; ++i ) {
            avgVol += singleSidedFFT[ i  ];
            /*
            if ( singleSidedFFT[ i ] > peakVol ) {
                peakVol = singleSidedFFT[ i ];
            }
            */
        }
        
        /*
        avgVol /= avgSamples;              
        hiAvgVol = avgVol;
               
        int newFadeRate = 0;
        // numSplats = Remap ( peakVol, -60, 0, 0, 4 ) ;
        newFadeRate += Remap ( loAvgVol, -60, -25, 250, 30 ) ;
        newFadeRate += Remap ( midAvgVol, -60, -25, 250, 30 ) ;
        newFadeRate += Remap ( hiAvgVol, -60, -25, 250, 30 ) ;
        
        newFadeRate /= 3;
        
        
        if ( newFadeRate < 250 ) {
            ctrStrobe.SetDelay ( newFadeRate );
            ctrStrobe.Start();
        } else {
            ctrStrobe.Stop();
        }
        */
        
        // ctrStrobe.SetDelay ( newFadeRate );
        
        /* 
        if ( strobeRateLim.Done() ) {
            strobeRateLim.Reset();
            if ( fadeRate < newFadeRate ) {
                fadeRate += 10;
            } else if ( fadeRate > newFadeRate ) {
                fadeRate -= 10;
            }
            
            if ( fadeRate < 30 ) {
                fadeRate = 30;
            } else if ( fadeRate > 250 ) {
                fadeRate = 250;
            }
            
        }        
        */       
                
        if ( !strobe ) {
            Blit( fingers );
            // Blit( palm );
        }        
        
        if ( palmAngle > 360 ) {
            palmAngle -= 360;        
        }
        
        if ( angle > 360 ) {
            angle -= 360;
        }
    
        RefreshDisplay( FB_CLEAR );
            
    } // end main loop         
    
    return ( MODE_NEXT );
}