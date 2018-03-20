/*  Copyright (c) 2009 by Alex Leone <acleone ~AT~ gmail.com>

    This file is part of the Arduino TLC5940 Library.

    The Arduino TLC5940 Library is free software: you can redistribute it
    and/or modify it under the terms of the GNU General Public License as
    published by the Free Software Foundation, either version 3 of the
    License, or (at your option) any later version.

    The Arduino TLC5940 Library is distributed in the hope that it will be
    useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with The Arduino TLC5940 Library.  If not, see
    <http://www.gnu.org/licenses/>. */

#ifndef TLC_CHIP_INCLUDES_H
#define TLC_CHIP_INCLUDES_H
/** \file
    Includes the chip-specfic defaults and pin definitions. */
#include <avr/io.h>
#ifndef PB0
#define PB0     PORTB0
#define PB1     PORTB1
#define PB2     PORTB2
#define PB3     PORTB3
#define PB4     PORTB4
#define PB5     PORTB5
#define PB6     PORTB6
#define PB7     PORTB7
#endif
/* Chip Specific Pinouts */
#if defined (__AVR_ATmega32U4__)
/* Leonardo */
#include "Arduino_Leonardo.h"
#else
#error "Unknown Chip!"
#endif
#endif
