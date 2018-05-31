#include "./include/anSplatterSA.h" 
#include "include/types.h"

// Refer to fft.h/fft.cpp
extern bool analyzerRun;
extern volatile bool FFTBufferReady;
extern short singleSidedFFT[ N ];
// float testAvg, testPeak;

anSplatterSA::anSplatterSA( npDisplay* pDisplay, mode_t mode, int frames, opt_t opts, scale_t customScaling ) 
 : npAnimation( pDisplay, mode, frames, opts, customScaling ) {
    // Sync current animation runtime mode settings to LCD display
        
    analyzerRun = 1;   
    InitFFT();
}

anSplatterSA::~anSplatterSA() {
    analyzerRun = 0;
}

int anSplatterSA::Draw() {    
    // Main animation loop
    counter fadeCtr ( 20, ON );
    const int avgSamples = 10;
    int numSplats;
    
    while ( ( framesDrawn < frames ) || modeFlags.test( MODE_REPEAT ) ) {
        if ( CheckAnimSwitch() ) {
            return ( MODE_NEXT );
        }
               
        

        if ( FFTBufferReady ) {
            ComputeFFT();                
        }  

        if ( fadeCtr.Done() ) {
            fadeCtr.Reset();
            FadeOut();                      
        }            

        
        // peakVol = -60.f;
        float avgVol, loAvgVol, midAvgVol, hiAvgVol = -60;
        // Get average volume - low freq
        for ( int i = 1; i <= 10; ++i ) {
            avgVol += singleSidedFFT[ i  ];
            /*
            if ( singleSidedFFT[ i ] > peakVol ) {
                peakVol = singleSidedFFT[ i ];
            }
            */
        }

        avgVol /= avgSamples;              
        loAvgVol = avgVol;
        avgVol = -60;
        for ( int i = 11; i <= 20; ++i ) {
            avgVol += singleSidedFFT[ i  ];
            /*
            if ( singleSidedFFT[ i ] > peakVol ) {
                peakVol = singleSidedFFT[ i ];
            }
            */
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
        
        avgVol /= avgSamples;              
        hiAvgVol = avgVol;
        
        numSplats = 0;
        // numSplats = Remap ( peakVol, -60, 0, 0, 4 ) ;
        numSplats += Remap ( loAvgVol, -60, -25, 0, 8 ) ;
        numSplats += Remap ( midAvgVol, -60, -25, 0, 8 ) ;
        numSplats += Remap ( hiAvgVol, -60, -25, 0, 8 ) ;

        if ( numSplats > 0 ) {
            // randomize where they go and draw them
            for ( int j = 0; j <= numSplats; j++ ) {
                int rnd = rand() % 21;
                Set( 0, rnd, rgbwGetByAngle( rand() % 360, rand() % 32 ) );
            }
        }

        framesDrawn++;
        
        RefreshDisplay( );
    }      

    return ( MODE_NEXT );
}
