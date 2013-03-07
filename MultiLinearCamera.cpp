/***************************************************************************************
 *
 * Title:       TSL3301 linear camera C library for the PRisme2
 * File:        linearCamera.cpp
 * Date:        07.12.2012
 * Author:      Karl Kangur
 * Comments:    Based on the assembly code by Christophe Winter, Fabrizio Lo Conte and
 *              Eric Seuret.
 * 
 * Datasheet:   http://www.ams.com/eng/content/download/250205/975793/142526
 *
 ***************************************************************************************/
#include <util/delay.h>
#include <robopoly.h>
#include "MultiLinearCamera.h"

unsigned char mlcam_buffer[102];

void mlcam_pulse()
{
  digital_write(LCAM2_PORT, LCAM2_SCLK, 1);
  digital_write(LCAM2_PORT, LCAM2_SCLK, 0);
}


void mlcam_pulse_clock(unsigned char times)
{
  for(unsigned char i = 0; i < times; i++)
  {
    mlcam_pulse();
  }
}

// Send a byte to the linear camera
void mlcam_send(unsigned char value)
{
  // Start bit
  digital_write(LCAM2_PORT, LCAM2_SDIN, 0);
  mlcam_pulse();
  // Send 8 bits to camera
  for(unsigned char i = 0; i < 8; i++)
  {
    digital_write(LCAM2_PORT, LCAM2_SDIN, ((value >> i) & 1));
    mlcam_pulse();
  }
  // Stop bit
  digital_write(LCAM2_PORT, LCAM2_SDIN, 1);
  mlcam_pulse();
}

// Initialization Sequence
void mlcam_reset(void)
{
  // allow a minimum of 1 millisecond for the internal analog circuitry to settle
  _delay_ms(1);
  
  // 10 clock impulsions with SDIN held high to clear the receiver logic
  digital_write(LCAM2_PORT, LCAM2_SDIN, 1);
  mlcam_pulse_clock(10);
  // 3 reset instructions to clear the control logic
  mlcam_send(0x1b);
  mlcam_send(0x1b);
  mlcam_send(0x1b);
  // 30 clock impulsions to assure the state of SDOUT
  mlcam_pulse_clock(30);
  
  // register write mode
  mlcam_send(0X5F);
  // Clear mode register (single chip, not sleep)
  mlcam_send(0x00);
}

void mlcam_setup(void)
{
  mlcam_reset();
  
  // set inputs and outputs
  pin_mode(LCAM2_PORT, LCAM2_SCLK, 1);
  pin_mode(LCAM2_PORT, LCAM2_SDIN, 1);
  pin_mode(LCAM2_PORT, LCAM2_SDOUT, 0);
  // make sure the clock is cleared
  digital_write(LCAM2_PORT, LCAM2_SCLK, 0);
  
  // Left offset
  mlcam_send(0x40);
  mlcam_send(0);
  // Left gain
  mlcam_send(0x41);
  mlcam_send(15);
  // Middle offset
  mlcam_send(0x42);
  mlcam_send(0);
  // Middle gain
  mlcam_send(0x43);
  mlcam_send(15);
  // Right offset
  mlcam_send(0x44);
  mlcam_send(0);
  // Right gain
  mlcam_send(0x45);
  mlcam_send(15);
}

void mlcam_startintegration(void)
{
  // Send start integration command
  mlcam_send(0x08);
  // delayed until the pixel reset cycle has been completed (22-clock delay)
  mlcam_pulse_clock(22);
}

void mlcam_endintegration(void)
{
  // Sample int command
  mlcam_send(0x10);
  // pixel reset sequence is initiated, requires 22 clocks
  mlcam_pulse_clock(22);
}

// shortcut for starting and ending integration
void mlcam_integrate(unsigned int microseconds)
{
  mlcam_startintegration();
  _delay_us(microseconds);
  mlcam_endintegration();
}

// Tell the camera to be ready to send data
void mlcam_read(void)
{
  unsigned char i, pixel_bit, pixel;
  // Read pixel command
  mlcam_send(0x02);
  // 44-clock cycle delay until the first pixel data is output
  mlcam_pulse_clock(44);
  // Read the 102 pixels from the camera
  for(i = 0; i < 102; i++)
  {
    pixel = 0;
    // pulse the pixel start bit (SDOUT = 0)
    mlcam_pulse();
    // read a byte, bit by bit
    for(pixel_bit = 0; pixel_bit < 8; pixel_bit++)
    {
      digital_write(LCAM2_PORT, LCAM2_SCLK, 1);
      // read pin while clock is at 1
      pixel |= (((LCAM2_PIN >> LCAM2_SDOUT) & 1) << pixel_bit);
      digital_write(LCAM2_PORT, LCAM2_SCLK, 0);
    }
    // store byte to buffer
    mlcam_buffer[i] = pixel;
    // pulse the pixel stop bit (SDOUT = 1)
    mlcam_pulse();
  }
}

unsigned char* mlcam_getdata()
{
  return mlcam_buffer;
}

// Divide the 100 first pixels into 25 4-byte averages and return the highest average index
unsigned char mlcam_getpic(void)
{
  unsigned char i, value, highest = 0, max_region = 0;
  for(i = 0; i < 25; i++)
  {
    // take 4-byte average and divide by 4 (shift to right by 2)
    if((value = ((mlcam_buffer[i*4] + mlcam_buffer[i*4+1] + mlcam_buffer[i*4+2] + mlcam_buffer[i*4+3]) >> 2)) > highest)
    {
      highest = value;
      max_region = i;
    }
  }
  return max_region;
}
