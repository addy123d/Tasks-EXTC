#define ubbr 103
#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <lcd.h>
const char a[] PROGMEM = "apple";
const char b[] PROGMEM = "ball";
const char c[] PROGMEM = "cat";
const char d[] PROGMEM = "dog";
const char e[] PROGMEM = "elephant";
const char f[] PROGMEM = "fish";
const char g[] PROGMEM = "goat";
const char h[] PROGMEM = "hello";
const char i[] PROGMEM = "iron";
const char j[] PROGMEM = "jump";
const char k[] PROGMEM = "keep";
const char l[] PROGMEM = "lazy";
const char m[] PROGMEM = "mad";
const char n[] PROGMEM = "night";
const char o[] PROGMEM = "orange";
const char p[] PROGMEM = "parrot";
const char q[] PROGMEM = "queen";
const char r[] PROGMEM = "rest";
const char s[] PROGMEM = "sleep";
const char t[] PROGMEM = "time";
const char u[] PROGMEM = "umbrella";
const char v[] PROGMEM = "voilet";
const char w[] PROGMEM = "war";
const char x[] PROGMEM = "xmas";
const char y[] PROGMEM = "you";
const char z[] PROGMEM = "zebra";
const char *const charPointers [] PROGMEM = {a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x,y,z};

char buffer[30]; 



void UART_init(uint16_t ubrr)
{
    // UBRRn = (fosc/16*baud)-1

    // set baudrate in UBRR
    UBRR0L = (uint8_t)(ubrr & 0xFF);
    UBRR0H = (uint8_t)(ubrr >> 8);

    // enable the transmitter and receiver
    UCSR0B = (1 << RXEN0) | (1 << TXEN0);

    //Enable Global Interrupts
	sei();
}

// Interrupt Based Transmission
volatile uint8_t USART_TransmitBuffer; // Global Buffer

void USART_TransmitInterrupt(uint8_t Buffer)
{
	USART_TransmitBuffer = Buffer;
	UCSR0B |= DATA_REGISTER_EMPTY_INTERRUPT; // Enables the Interrupt
}

ISR(USART_UDRE_vect)
{
	UDR0 = USART_TransmitBuffer; // Store transmitted data into UDR0!

    for (int i = 0; i < strlen_P(charPointers); i++) { // Iterate over all values in the table !
        strcpy_P(buffer, (char *)pgm_read_word(&(charPointers[i])));  // Necessary casts and dereferencing, just copy.
        Serial.println(buffer); //Print all words for debugging purpose !

        if(UDR0 == buffer[0]){ //Match Found !
            Serial.println(buffer);
        }
    delay(500);
    }

}


// Interrupt Based Reception !
volatile uint8_t USART_ReceiveBuffer; // Global Buffer;

ISR(USART_RX_vect)
{
    USART_ReceiveBuffer = UDR0;
    // We have to print word whose first letter matches with char being transmitted !    
}

int main()
{
    // For Interrupt Based Transmission 
    uint8_t data = 'a';
	UART_init();
	USART_TransmitInterrupt(data);

    // For Interrupt Based Reception
	UCSR0B |= RX_COMPLETE_INTERRUPT;
	while (1)
	{
	}
	return 0;
}



// Steps to set up UART !
// 1. Setting Baud Rate (Register Involved : UBBRn): 
// Setting the baud rate is done by writing to the UBRR0H and UBRR0L registers.
// Formula : UBBRn = (fosc/16*baud) - 1
// We are using a 16Mhz oscillator, fosc = 16000000
// baud we will be going with 9600
// Value of UBBRn = 103.167, rounding off to 103

// Set value for UBBR0L(lower byte) and UBBROH(higher byte) registers
// UBBROL = (uint8_t)(103 & 0xFF)  (value for this is 0)
// UBBROH = (uint8_t)(103 >> 8)  (value for this is 103)

// Enable receiver and transmitter (Register Involved :  UCSR0B,RXEN0,TXEN0)
// What will happen if not enabled, RX,TX will act as standard IO pins.
// UCSR0B = (1 << RXEN0) | (1 << TXEN0);  


// Init function
// #define ubbr 103
//  void UART_init(uint16_t ubrr)
// {
//     // set baudrate in UBRR
//     UBRR0L = (uint8_t)(ubrr & 0xFF);
//     UBRR0H = (uint8_t)(ubrr >> 8);

//     // enable the transmitter and receiver
//     UCSR0B = (1 << RXEN0) | (1 << TXEN0);
// }







