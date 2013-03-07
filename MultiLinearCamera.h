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

#define LCAM_PORT(X)  LCAMX_PORT // lcamera port
#define LCAM_PIN(X)   LCAMX_PIN // lcamera pin
#define LCAM_DDR(X)  LCAMX_DDR // lcamera DDR

#define LCAM1_DDR   DDRC // lcamera DDR
#define LCAM1_PIN   PINC // lcamera pin
#define LCAM1_PORT  PORTC // lcamera port

#define LCAM2_DDR   DDRC // lcamera DDR
#define LCAM2_PIN   PINC // lcamera pin
#define LCAM2_PORT  PORTC // lcamera port

#define LCAM3_DDR   DDRB // lcamera DDR
#define LCAM3_PIN   PINB // lcamera pin
#define LCAM3_PORT  PORTB // lcamera port

#define LCAM4_DDR   DDRB // lcamera DDR
#define LCAM4_PIN   PINB // lcamera pin
#define LCAM4_PORT  PORTB // lcamera port

#define LCAM_SDIN(X)  LCAMX_SDIN // SDIN: master out, camera in
#define LCAM_SDOUT(X) LCAMX_SDOUT // SDOUT: camera out, master in
#define LCAM_SCLK(X)  LCAMX_SCLK // camera clock

#define LCAM1_SDIN  0 // SDIN: master out, camera in
#define LCAM1_SDOUT 1 // SDOUT: camera out, master in
#define LCAM1_SCLK  2 // camera clock

#define LCAM2_SDIN  3 // SDIN: master out, camera in
#define LCAM2_SDOUT 4 // SDOUT: camera out, master in
#define LCAM2_SCLK  5 // camera clock

#define LCAM3_SDIN  0 // SDIN: master out, camera in
#define LCAM3_SDOUT 1 // SDOUT: camera out, master in
#define LCAM3_SCLK  2 // camera clock

#define LCAM4_SDIN  3 // SDIN: master out, camera in
#define LCAM4_SDOUT 4 // SDOUT: camera out, master in
#define LCAM4_SCLK  5 // camera clock

void mlcam_pulse(void);
void mlcam_pulse_clock(uint8_t times);
void mlcam_send(uint8_t value);
void mlcam_reset(void);
void mlcam_setup(void);
void mlcam_startintegration(void);
void mlcam_endintegration(void);
void mlcam_integrate(unsigned int microseconds);
unsigned char* mlcam_getdata(void);
void mlcam_read(void);
unsigned char mlcam_getpic(void);
