#include "./include/anSplatterSA.h" 

// Refer to fft.h/fft.cpp
extern bool analyzerRun;
extern volatile bool FFTBufferReady;
extern short singleSidedFFT[ N ];
// float testAvg, testPeak;

anSplatterSA::anSplatterSA( npDisplay* pDisplay, mode_t mode, int frames, opt_t opts, scale_t customScaling ) 
 : npAnimation( pDisplay, mode, frames, opts, customScaling ) {
    // Sync current animation runtime mode settings to LCD display
    
    analyzerRun = 1;    // used by T4 ISR
    InitFFT();
}

anSplatterSA::~anSplatterSA() {
    analyzerRun = 0;
}

int anSplatterSA::Draw() {    
    // Main animation loop
    counter fadeCtr ( 20, ON );
    const int avgSamples = 20;
    float avgVol = 0.0f;
    float peakVol = 0.f;
    
    while ( ( framesDrawn < frames ) || modeFlags.test( MODE_REPEAT ) ) {
        if ( CheckAnimSwitch() ) {
            return ( MODE_NEXT );
        }
                        
        if ( ret == MODE_PREV || ret == MODE_NEXT ) {
            break;  // break while loop and return to main signaling next/prev animation to be drawn
        }                            
           
        if ( !skip ) { 
            int numSplats;
            
            if ( FFTBufferReady ) {
                ComputeFFT();                
            }  
           
            if ( fadeCtr.Done() ) {
                fadeCtr.Reset();
                FadeOut();                      
            }            
            
            avgVol = 0.f;
            peakVol = -60.f;
            
            // Get average volume
            for ( int i = 3; i <= avgSamples; ++i ) {
                avgVol += singleSidedFFT[ i  ];
                if ( singleSidedFFT[ i ] > peakVol ) {
                    peakVol = singleSidedFFT[ i ];
                }
            }
            
            avgVol /= avgSamples;                    
            // testAvg = avgVol;
            // testPeak = peakVol;
            
            // numSplats = (int) Remap ( avgVol, -60, 0, 0, 5 ) ;
            numSplats = (int) Remap ( peakVol, -55, -5, 0, 6 ) ;

            if ( numSplats > 0 ) {
                // randomize where they go and draw them
                for ( int j = 0; j <= numSplats; j++ ) {
                    int rnd = rand() % 32;
                    Set( 0, rnd, rgbwGetByAngle( rand() % 360, rand() % 32 ) );
                }
            }
                        
            framesDrawn++;
        }      
        
        RefreshDisplay( );
        
    }   // end main loop
    
    return ( MODE_NEXT );
}
