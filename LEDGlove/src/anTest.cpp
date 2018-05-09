#include "include/anTest.h"
#include "include/matrix.h"
#include "include/types.h"

using namespace std;

anTest::anTest( npDisplay* pDisplay, mode_t mode, int frames, opt_t opts, scale_t customScaling ) 
: npAnimation( pDisplay, mode, frames, opts, customScaling ) { }

anTest::~anTest() { }

// STRICTLY FOR TESTING //
int anTest::Draw() {
    // Main animation loop   
    const int palmPixels = 20;
    counter ctrFingers ( 500, ON );
    counter ctrFingerFade ( 500, ON );
    counter ctrPalmFade ( 200, ON );
    counter ctrPalmBlink ( 100, ON );
    float angle1 = 30;
    float angle2 = 60;
    int shiftCount = 0;
    int palmBlink = 1;
    
    // Init display
    pixel pinky( coord2d_t { 0, 0 }, rgbwGetByAngle( angle1 ) );
    pixel ring( coord2d_t { 0, 5 }, rgbwGetByAngle( angle1 ) );
    pixel middle( coord2d_t { 0, 6 }, rgbwGetByAngle( angle1 ) );
    pixel index( coord2d_t { 0, 11 }, rgbwGetByAngle( angle1 ) );
    
    pixel palm[palmPixels];
    
    for ( int i = 0; i < palmPixels; i++ ) {            
        palm[i].color = rgbwGetByAngle( angle2 );
        palm[i].coord = {0, i+12};
    }
    
    
    // RefreshDisplay( FB_CLEAR );
    
    
    while ( ( framesDrawn < frames ) || modeFlags.test( MODE_REPEAT ) ) {        
        if ( !skip ) {                

            if ( ctrFingers.Done() ) {        
                ctrFingers.Reset();
                
                shiftCount++;
                
                if ( shiftCount > 3 ) {                       
                    pinky.coord = { 0, 0 };
                    ring.coord =  { 0, 5 };
                    middle.coord = { 0, 6 };
                    index.coord = { 0, 11 };
                    
                    // Fingers
                    Set( pinky.coord.x, pinky.coord.y, rgbwGetByAngle( angle1 ));
                    Set( ring.coord.x, ring.coord.y, rgbwGetByAngle( angle1 + 30 ) );
                    Set( middle.coord.x, middle.coord.y, rgbwGetByAngle( angle1 + 60 ) );
                    Set( index.coord.x, index.coord.y, rgbwGetByAngle( angle1 + 90 ) );                    
                    shiftCount = 0;
                } 
                
                if ( (shiftCount > 0) && (shiftCount < 3) ) {
                    pinky.Shift( 0, 1 );
                    ring.Shift( 0, -1 );
                    middle.Shift( 0, 1 );
                    index.Shift( 0, -1 );                                
                    // Fingers
                    Set( pinky.coord.x, pinky.coord.y, rgbwGetByAngle( angle1 ));
                    Set( ring.coord.x, ring.coord.y, rgbwGetByAngle( angle1 + 30 ) );
                    Set( middle.coord.x, middle.coord.y, rgbwGetByAngle( angle1 + 60 ) );
                    Set( index.coord.x, index.coord.y, rgbwGetByAngle( angle1 + 90 ) );
                }
                
                if ( shiftCount == 3 ) {
                    Set( pinky.coord.x, pinky.coord.y, rgbw_t {0, 0, 0, 0} );
                    Set( ring.coord.x, ring.coord.y, rgbw_t {0, 0, 0, 0} );
                    Set( middle.coord.x, middle.coord.y, rgbw_t {0, 0, 0, 0} );
                    Set( index.coord.x, index.coord.y, rgbw_t {0, 0, 0, 0} );                    
                }
                                                                
                RefreshDisplay( FB_CLEAR );                
            }
            

            if ( ctrFingerFade.Done() ) {
                ctrFingerFade.Reset();
                if ( ( angle1 += 5.0f )  > 360 ) { 
                    angle1 -= 360;
                    
                    // RefreshDisplay( FB_CLEAR );
                }
            }
            
            if ( ctrPalmFade.Done() ) {
                ctrPalmFade.Reset();
                if ( ( angle2 += 1.0f ) > 360 ) { 
                    angle2 -= 360;
                    
                   // RefreshDisplay( FB_CLEAR );
                }
            }
            
            if ( ctrPalmBlink.Done() ) {
                ctrPalmBlink.Reset();
                if ( palmBlink ) {
                    for ( int i = 0; i < palmPixels; i++ ) {
                        Set( palm[i].coord.x, palm[i].coord.y, rgbw_t { 0, 0, 0, 0 } );
                    }
                    palmBlink = 0;
                } 
                
                else {
                    for ( int i = 0; i < palmPixels; i++ ) {
                        Set( palm[i].coord.x, palm[i].coord.y, rgbwGetByAngle( angle2 + (i*4) ) );
                    }
                    palmBlink = 1;
                }
                
                RefreshDisplay( FB_BLEND );                                
            } 
            
            
        } // not skipped 
    } // end main loop         
    
    return ( MODE_NEXT );
}