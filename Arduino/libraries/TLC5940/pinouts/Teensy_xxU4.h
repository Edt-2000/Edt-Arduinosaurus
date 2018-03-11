#ifndef TLC_Teensy_xxU4_h
#define TLC_Teensy_xxU4_h

// #if DATA_TRANSFER_MODE == TLC_BITBANG
// /** SIN (Arduino digital pin 7) -> SIN (TLC pin 26) */
// #define DEFAULT_BB_SIN_PIN      PD7
// #define DEFAULT_BB_SIN_PORT     PORTD
// #define DEFAULT_BB_SIN_DDR      DDRD
// /** SCLK (Arduino digital pin 4) -> SCLK (TLC pin 25) */
// #define DEFAULT_BB_SCLK_PIN     PD4
// #define DEFAULT_BB_SCLK_PORT    PORTD
// #define DEFAULT_BB_SCLK_DDR     DDRD
// #endif

// MOSI (Teensy pin 2) -> SIN (TLC pin 26)
// #define TLC_MOSI_PIN	2
// #define TLC_MOSI_PORT	PORTB
// #define TLC_MOSI_DDR	DDRB

// // SCK (Teensy pin 1) -> SCLK (TLC pin 25)
// #define TLC_SCK_PIN	1
// #define TLC_SCK_PORT	PORTB
// #define TLC_SCK_DDR	DDRB

// // SS (Teensy pin 0)
// #define TLC_SS_PIN	0
// #define TLC_SS_DDR	DDRB

// // OC1A (Teensy pin 14) -> XLAT (TLC pin 24)
// #define XLAT_PIN	5
// #define XLAT_PORT	PORTB
// #define XLAT_DDR	DDRB

// // OC1B (Teensy pin 15) -> BLANK (TLC pin 23)
// #define BLANK_PIN	6
// #define BLANK_PORT	PORTB
// #define BLANK_DDR	DDRB

// // OC3A (Teensy pin 9) -> GSCLK (TLC pin 18)
// #define GSCLK_PIN	6
// #define GSCLK_PORT	PORTC
// #define GSCLK_DDR	DDRC
// #define TLC_TIMER3_GSCLK 1

#define DEFAULT_VPRG_PIN    PB0
#define DEFAULT_VPRG_PORT   PORTB
#define DEFAULT_VPRG_DDR    DDRB

/** XERR (Arduino digital pin 12) -> XERR (TLC pin 16) */
#define DEFAULT_XERR_PIN    PB4
#define DEFAULT_XERR_PORT   PORTB
#define DEFAULT_XERR_DDR    DDRB
#define DEFAULT_XERR_PINS   PINB

/** SIN (Arduino digital pin 7) -> SIN (TLC pin 26) */
#define DEFAULT_BB_SIN_PIN      PD7
#define DEFAULT_BB_SIN_PORT     PORTD
#define DEFAULT_BB_SIN_DDR      DDRD
/** SCLK (Arduino digital pin 4) -> SCLK (TLC pin 25) */
#define DEFAULT_BB_SCLK_PIN     PD4
#define DEFAULT_BB_SCLK_PORT    PORTD
#define DEFAULT_BB_SCLK_DDR     DDRD

/** MOSI (Arduino digital pin 11) -> SIN (TLC pin 26) */
#define TLC_MOSI_PIN     PB3
#define TLC_MOSI_PORT    PORTB
#define TLC_MOSI_DDR     DDRB

/** SCK (Arduino digital pin 13) -> SCLK (TLC pin 25) */
#define TLC_SCK_PIN      PB5
#define TLC_SCK_PORT     PORTB
#define TLC_SCK_DDR      DDRB

/** SS will be set to output as to not interfere with SPI master operation.
    If you have changed the pin-outs and the library doesn't seem to work
    or works intermittently, make sure this pin is set correctly.  This pin
    will not be used by the library other than setting its direction to
    output. */
#define TLC_SS_PIN       PB4
#define TLC_SS_DDR       DDRB

/** OC1A (Arduino digital pin 9) -> XLAT (TLC pin 24) */
#define XLAT_PIN     PB1
#define XLAT_PORT    PORTB
#define XLAT_DDR     DDRB

/** OC1B (Arduino digital pin 10) -> BLANK (TLC pin 23) */
#define BLANK_PIN    PB4
#define BLANK_PORT   PORTB
#define BLANK_DDR    DDRB

/** OC2B (Arduino digital pin 3) -> GSCLK (TLC pin 18) */
#define GSCLK_PIN    PD3
#define GSCLK_PORT   PORTD
#define GSCLK_DDR    DDRD

#endif

