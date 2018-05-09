#include "./include/pixel.h"

using namespace std;

pixel::pixel( void ) { }

pixel::pixel( coord2d_t coord, rgbw_t color, uint16_t brt ) :  
    coord( coord ), 
    color( color ), 
    brt( brt ) { }

pixel::~pixel( void ) { }

void pixel::Shift( int x, int y ) {
    coord.x += x;
    coord.y += y;   
    
    /* else if ( x > 0 ) { // shifting right
        if ( ( coord.x + x ) > displayTopRight.x ) {  // check if this shift will overflow the right extents
            color = { 0, 0, 0, 0 }; // if so, just turn the pixel off
        } else {
            coord.x += x;
        }
    }
     */
    
}