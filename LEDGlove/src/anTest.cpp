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
    
    while ( ( framesDrawn < frames ) || modeFlags.test( MODE_REPEAT ) ) {        
        if ( !skip ) {                
            
        } // not skipped 
    } // end main loop         
    
    return ( MODE_NEXT );
}