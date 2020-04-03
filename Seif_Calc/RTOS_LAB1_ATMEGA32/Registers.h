#ifndef REGISTERS_H_
#define REGISTERS_H_

/** PORTS PINS DDRS **/
#define PORTA        (*(volatile u8*) (0x3B))
#define PORTB        (*(volatile u8*) (0x38))
#define PORTC        (*(volatile u8*) (0x35))
#define PORTD        (*(volatile u8*) (0x32))
#define DDRA         (*(volatile u8*) (0x3A))
#define DDRB         (*(volatile u8*) (0x37))
#define DDRC         (*(volatile u8*) (0x34))
#define DDRD         (*(volatile u8*) (0x31))
#define PINA         (*(volatile u8*) (0x39))
#define PINB         (*(volatile u8*) (0x36))
#define PINC         (*(volatile u8*) (0x33))
#define PIND         (*(volatile u8*) (0x30))


/***   Global Interrupt   ***/
#define SREG       (*(volatile u8*) (0x5F))
#define I				7

/** UART **/
#define UBRRH        (*(volatile u8*) (0x40))
#define UBRRL        (*(volatile u8*) (0x29))
#define UCSRA        (*(volatile u8*) (0x2B))
#define UCSRB        (*(volatile u8*) (0x2A))
#define UCSRC        (*(volatile u8*) (0x40))
#define UDR			(*(volatile u8*) (0x2C))
/** UCSRA **/
#define RXC          7   
#define TXC          6
#define UDRE         5
#define FE           4
#define DOR          3
#define PE           2
#define U2X          1
#define MPCM         0
/** UCSRB **/
#define RXCIE        7
#define TXCIE        6
#define UDRIE        5
#define RXEN         4
#define TXEN         3
#define UCSZ2        2
#define RXB8         1
#define TXB8         0
/** UCSRC **/
#define URSEL        7
#define UMSEL        6
#define UPM1         5
#define UPM0         4
#define USBS         3
#define UCSZ1        2
#define UCSZ0        1
#define UCPOL        0


#endif