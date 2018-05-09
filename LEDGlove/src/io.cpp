#include "./include/io.h"

int uartBytesRead, uartPacketSize;
rgbw_t colorPicker;
bool isPacket, isCmdPacket, isColorPacket;
uint64_t readBuffer;
gModes_t globalMode;

extern "C"
void UART_ReadByte() {
    // asm volatile( "nop" );
    // get most recent byte out of UART buffer
    uint64_t rxByte = ReadUART1();
    
    // Packet header ID is 0x21
    if ( rxByte == 0x21 ) {
        isCmdPacket = isColorPacket = 0;
        isPacket = 1;
        uartBytesRead = 0;
        readBuffer = 0;
    }
        
    if ( isPacket ) {
        readBuffer = ( readBuffer << 8 ) | rxByte;        
        
        switch ( readBuffer ) {
            case ( 0x2142 ) :
                isCmdPacket = 1;
                uartPacketSize = 5;
                break;

            case ( 0x2143 ) :
                isColorPacket = 0;
                uartPacketSize = 6;
                break;         

            default:
                break;
        }               
         
        ++uartBytesRead;
        
        // Color Packet: 0x21 0x43 0xRR 0xGG 0xBB 0xCRC 
        // Clean this shit up...
        if ( isColorPacket ) {
            switch ( uartBytesRead ) {
                case 3:
                    colorPicker.r = (uint8_t)rxByte;
                    break;
                case 4:
                    colorPicker.g = (uint8_t)rxByte;
                    break;
                case 5:
                    colorPicker.b = (uint8_t)rxByte;
                    break;
                default:
                    break;
            }
        }
    }
       
    // Drop packets larger than 6 bytes in length
    if ( uartPacketSize && ( uartBytesRead > uartPacketSize ) ) {
        isPacket = isCmdPacket = isColorPacket = 0;
        uartBytesRead = 0;
        readBuffer = 0;
    }
    
}

void UART_SendMessage( const uint64_t msg, int len ) {
    // sends out packet bytes on UART depending on
    // packet buffer length (between 1 and 6 bytes)
    if ( len >= 6 ) {
        putcUART1( (char)( ( msg >> 40 ) & 0xFF ) );
    }
    
    if ( len >= 5 ) {
        putcUART1( (char)( ( msg >> 32 ) & 0xFF ) );
    }
    
    if ( len >= 4 ) {
        putcUART1( (char)( ( msg >> 24 ) & 0xFF ) );
    }
    
    if ( len >= 3 ) {
        putcUART1( (char)( ( msg >> 16 ) & 0xFF ) );
    }
    
    if ( len >= 2 ) {
        putcUART1( (char)( ( msg >> 8 ) & 0xFF ) );
    }
    
    if ( len >= 1 ) {
        putcUART1( (char)( ( msg  ) & 0xFF ) );
    }
}
