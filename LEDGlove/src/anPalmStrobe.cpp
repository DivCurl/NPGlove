#include "include/anPalmStrobe.h"
#include "include/matrix.h"
#include "include/types.h"

using namespace std;

anPalmStrobe::anPalmStrobe( npDisplay* pDisplay, mode_t mode, int frames, opt_t opts, scale_t customScaling ) 
: npAnimation( pDisplay, mode, frames, opts, customScaling ) { }

anPalmStrobe::~anPalmStrobe() { }

int anPalmStrobe::Draw() {
    counter ctrFingerShift ( 35, ON );
    counter ctrStrobe ( 20, ON );
    counter ctrColorFade ( 100, ON );
    counter ctrPalmFade( 25, ON );    
    int fingerShiftCount = 0;  
    int strobe = 0;
    vector<pixel> fingers;
    vector<pixel> palm;
    
    fingers.push_back( 
        pixel( 
            indexPix[ 0 ],
            rgbwGetByAngle ( fingerAngle ),
            255 
        )
    );     
    
    fingers.push_back( 
        pixel( 
            middlePix[ 0 ],
            rgbwGetByAngle ( fingerAngle + 5 ),
            255 
        )
    );
    
    fingers.push_back( 
        pixel( 
            ringPix[ 0 ],
            rgbwGetByAngle ( fingerAngle + 10 ),
            255 
        )
    );
    
    fingers.push_back( 
        pixel( 
            pinkyPix[ 0 ],
            rgbwGetByAngle ( fingerAngle + 15 ),
            255 
        )
    );
    
    while ( ( framesDrawn < frames ) || modeFlags.test( MODE_REPEAT ) ) {                      
        if ( CheckAnimSwitch() ) {
            return ( MODE_NEXT );
        }
        
        if ( ctrStrobe.Done() ) {
                ctrStrobe.Reset();
                strobe ^= 1;
        }
        
        if ( ctrColorFade.Done() ) {
            ctrColorFade.Reset();            
            fingerAngle += 5.0;
        }
        
        if ( ctrPalmFade.Done () ) {
            palm.clear();
            ctrPalmFade.Reset();
            palmAngle += 4.0;
            
            for ( int i = 12; i < 31; ++i ) {
                palm.push_back( 
                    pixel( 
                        coord2d_t { 0, i },
                        rgbwGetByAngle ( palmAngle + (i - 12) * 2.0 ),
                        200 
                    )
                );     
            }
        }
                    
        if ( ctrFingerShift.Done() ) {
            ctrFingerShift.Reset();
            fingers.clear();            
            
            if ( ++fingerShiftCount > 3 ) {
                fingerShiftCount = 0;                
            } 
            
            if ( fingerShiftCount < 3 ) { 
                fingers.push_back( 
                    pixel( 
                        indexPix[ fingerShiftCount ],
                        rgbwGetByAngle ( fingerAngle ),
                        255 
                    )
                    );     
    
                fingers.push_back( 
                    pixel( 
                        middlePix[ fingerShiftCount ],
                        rgbwGetByAngle ( fingerAngle + 20 ),
                        255 
                    )
                );

                fingers.push_back( 
                    pixel( 
                        ringPix[ fingerShiftCount ],
                        rgbwGetByAngle ( fingerAngle + 40 ),
                        255 
                    )
                );

                fingers.push_back( 
                    pixel( 
                        pinkyPix[ fingerShiftCount ],
                        rgbwGetByAngle ( fingerAngle + 60 ),
                        255 
                    )
                );
            }
            
            
        }
        if ( !strobe ) {
            Blit( fingers );
            Blit( palm );
        }        
        
        if ( palmAngle > 360 ) {
            palmAngle -= 360;        
        }
        
        if ( fingerAngle > 360 ) {
            fingerAngle -= 360;
        }
    
        RefreshDisplay( FB_CLEAR );
            
    } // end main loop         
    
    return ( MODE_NEXT );
}