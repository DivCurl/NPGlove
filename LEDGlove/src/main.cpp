#include "./include/main.h"

extern int16c sampleBuffer[];  
extern bool analyzerRun;
extern volatile bool FFTBufferReady;
extern int sampleIndex;
extern int uartBytesRead;
extern bool isCmdPacket;
extern int uartPacketSize;
extern uint64_t readBuffer;
 

#pragma config POSCMOD      = HS            // Primary oscillator using high speed crystal mode
#pragma config FNOSC        = PRIPLL        // Internal Fast RC oscillator (4 MHz) w/ PLL
#pragma config FPLLIDIV     = DIV_1         // Divide FRC before PLL (now 4 MHz)
#pragma config FPLLMUL      = MUL_20        // PLL Multiply (now 80 MHz)
#pragma config FPLLODIV     = DIV_2         // Divide After PLL (now 40 MHz)
#pragma config FPBDIV       = DIV_1         // Divide core clock by 1 for peripheral bus (=40MHz Fpb)
#pragma config FWDTEN       = OFF           // Watchdog Timer Disabled
#pragma config ICESEL       = ICS_PGx1      // ICE/ICD Comm Channel Select
#pragma config JTAGEN       = OFF           // Disable JTAG
#pragma config FSOSCEN      = OFF           // Disable Secondary Oscillator

using namespace std;

int T4Period = PB_FREQ / ADC_SAMPLE_FREQ;
volatile uint32_t t2Ticks = 0;
volatile uint32_t ticks = 0;
uint16_t _1_ms_tick;
npDisplay display( RGBW );   
int refreshMode = FB_CLEAR;
   
int main() {                    
    SYSTEMConfigPerformance( SYS_FREQ );
    mJTAGPortEnable( DEBUG_JTAGPORT_OFF );
    INTEnableSystemMultiVectoredInt();
    InitADC();          
    _1_ms_tick = ( uint16_t )round( ( SYS_FREQ / T4Period ) * 0.001f - 1 ) ;
    // CloseTimer4();
    OpenTimer4( T4_ON | T4_SOURCE_INT | T4_PS_1_1, T4Period );  
    ConfigIntTimer4( T4_INT_ON | T4_INT_PRIOR_7 ); 
    ANSELA = 0;
    ANSELB = 0;
    // Set PORTB tristate pin modes
    TRISBbits.TRISB2 = 0;       // Neopixel data pin 
    TRISBbits.TRISB3 = 1;       // Input pin (animation switch PB)
    TRISBbits.TRISB15 = 1;      // Analog Input pin    
            
    npAnimation* pAnim;
    npDisplay display( RGBW );   
    display.SetBrightness( 230 );
    display.AddNeopixel( 32, &LATBSET, &LATBCLR, &TRISB, portPin[ 2 ] );   
    
    srand( ReadADC10( 0 ) ); 

    int currAnim = 1;
    
    currAnim = ID_AN_PALM_STROBE;        
    // currAnim = ID_AN_SPLATTER_SA; // works
    // currAnim = ID_AN_PALM_PULSE_SA;  // Works
    
    while ( 1 ) { 
        if ( currAnim == ID_AN_NULL ) {
            currAnim = ID_AN_MAX - 1;
        } 
        else if ( currAnim >= ID_AN_MAX ) {
            currAnim = 1;
        }
        
        switch ( currAnim ) {  
            
            case ( ID_AN_TEST ):
                pAnim = new anTest( &display, MODE_REPEAT );
                
                if ( pAnim->Draw() == MODE_PREV ) {
                    currAnim--;
                } 
                else if ( pAnim->Draw() == MODE_NEXT ) {
                    currAnim++;
                }
                
                delete pAnim;
                break;
                
            case ( ID_AN_PALM_STROBE ):
                pAnim = new anPalmStrobe( &display, MODE_REPEAT );
                
                if ( pAnim->Draw() == MODE_PREV ) {
                    currAnim--;
                } 
                else if ( pAnim->Draw() == MODE_NEXT ) {
                    currAnim++;
                }
                
                delete pAnim;
                break;                          
                
            case ( ID_AN_PALM_PULSE_SA ):
                pAnim = new anPalmPulseSA( &display, MODE_REPEAT );
                
                if ( pAnim->Draw() == MODE_PREV ) {
                    currAnim--;
                } 
                else if ( pAnim->Draw() == MODE_NEXT ) {
                    currAnim++;
                }
                
                delete pAnim;
                break;
                
            case ( ID_AN_PALM_PULSE_NO_STROBE_SA ):
                pAnim = new anPalmPulseNoStrobeSA( &display, MODE_REPEAT );
                
                if ( pAnim->Draw() == MODE_PREV ) {
                    currAnim--;
                } 
                else if ( pAnim->Draw() == MODE_NEXT ) {
                    currAnim++;
                }
                
                delete pAnim;
                break;
                
            case ( ID_AN_SPLATTER_SA ):
                pAnim = new anSplatterSA( &display, MODE_REPEAT );
                
                if ( pAnim->Draw() == MODE_PREV ) {
                    currAnim--;
                } 
                else if ( pAnim->Draw() == MODE_NEXT ) {
                    currAnim++;
                }
                
                delete pAnim;
                break;
                
            case ( ID_AN_VAR_PULSE_SA ):
                pAnim = new anVariablePulseSA( &display, MODE_REPEAT );
                
                if ( pAnim->Draw() == MODE_PREV ) {
                    currAnim--;
                } 
                else if ( pAnim->Draw() == MODE_NEXT ) {
                    currAnim++;
                }
                
                delete pAnim;
                break;
                            
            
            default : 
                currAnim++;
                delete pAnim;
                break;                        
        }
    }
    
    return ( EXIT_SUCCESS );
}

// Note - electret mic wired to AI4
void InitADC( void ) {   
    CloseADC10();
    SetChanADC10( ADC_CH0_NEG_SAMPLEA_NVREF );
    OpenADC10( ADC_CONFIG1, ADC_CONFIG2, ADC_CONFIG3, ADC_CONFIGPORT, ADC_CONFIGSCAN );
    EnableADC10(); 
    while ( !mAD1GetIntFlag() ) {}
}

// Timer4 interrupt for accurate timing
extern "C"
void __ISR ( _TIMER_4_VECTOR, IPL7AUTO ) TMR4IntHandler( void ) {    
    mT4ClearIntFlag(); 
    
    if ( ticks++ == _1_ms_tick ) {
        t2Ticks++;  
        ticks = 0;
    } 
    
    if ( analyzerRun ) {               
        int16_t s = ReadADC10( 0 );
        // some very basic noise rejection before storing sample   
        // sampleBuffer[ sampleIndex ].re = ( s > ( ADC_COUNT_BIAS - ADC_COUNT_NOISE ) ) && ( s < ( ADC_COUNT_BIAS + ADC_COUNT_NOISE ) ) ? 0 : s - ADC_COUNT_ZERO ;
        sampleBuffer[ sampleIndex ].re = s;
        sampleBuffer[ sampleIndex ].im = 0;
        
        if ( sampleIndex == ( N - 1 ) ) {
            sampleIndex = 0;
            FFTBufferReady = 1;
        } 
        else {
            sampleIndex++;
        }        
    }
    
    // mT4ClearIntFlag(); 
} 