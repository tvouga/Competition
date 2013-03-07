/***************************************************************************************
 *
 * Title:       TSL3301 linear camera C library for the PRisme2
 * File:        linearCamera.h
 * Date:        07.12.2012
 * Author:      Karl Kangur
 * Comments:    Based on the assembly code by Christophe Winter, Fabrizio Lo Conte and
 *              Eric Seuret.
 * 
 * Datasheet:   http://www.ams.com/eng/content/download/250205/975793/142526
 * http://www.eureca.de/datasheets/01.xx.xxxx/01.06.xxxx/01.06.0015/TSL3301.pdf
 *
 ***************************************************************************************/
#include <avr/io.h>

#define LCAM1_PORT  PORTC // lcamera port
#define LCAM1_DDR   DDRC // lcamera DDR
#define LCAM1_PIN   PINC // lcamera pin
#define LCAM2_PORT  PORTD // lcamera port
#define LCAM2_DDR   DDRD // lcamera DDR
#define LCAM2_PIN   PIND // lcamera pin

#define LCAM1_SDIN  3 // SDIN: master out, camera in
#define LCAM1_SDOUT 4 // SDOUT: camera out, master in
#define LCAM1_SCLK  5 // camera clock

#define LCAM2_SDIN  3 // SDIN: master out, camera in
#define LCAM2_SDOUT 4 // SDOUT: camera out, master in
#define LCAM2_SCLK  5 // camera clock

void lcam_pulse(void);
void lcam_pulse_clock(uint8_t times);
void lcam_send(uint8_t value);
void lcam_reset(void);
void lcam_setup(void);
void lcam_startintegration(void);
void lcam_endintegration(void);
void lcam_integrate(unsigned int microseconds);
unsigned char* lcam_getdata(void);
void lcam_read(void);
unsigned char lcam_getpic(void);
