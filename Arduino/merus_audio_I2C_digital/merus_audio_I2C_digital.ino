
/*----------------------------------------------------------
 * Title:   I2C communication setup
 * Author:  Rien Oortgiesen
 * Company: Merus Audio
 * 
 * Description:
 * This code demonstrates initial I2C communication 
 * using Arduino Uno together with MA devices - P version
 * this means that the MA device will take a digital
 * audio as input stream - I2S
 * 
 * Use:
 * The code uses I2C lib from Wayne Truchsess
 * which allows repeated start and can be used 
 * in an interupt service routine
 * 
 * I2C.write(dev_addr, reg_addr, write_val)
 * 
 * I2C hardware config:
 * Uno breakout:       SCL=A5; SDA=A4 GND=GND; 
 * Snowflake CONN_COM: SCL=pin4; SDA=pin3; GND=pin2 
 * Device address: 0x20
 * 
 * Revisions: 
 * 20170814: intial version
 * 20171003: Update for Linkplay ref board 
 *  
 * This code is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 */


#include <I2C.h>                                // plug-in your MCU specific I2C library

 /*-------- Variable declaration ---------*/
  int pmp_sel;
  int curr_vol;

  int enable_n = 7;                             // /ENABLE connected to PD6
  int mute_n = 6;                               // /MUTE connected tp PD7
  // note schematic mute enable naming bug
  int msel0 = 5;                                // MSEL0 is connected to PD5
  
void setup ()
{
  pinMode(enable_n, OUTPUT);
  pinMode(mute_n, OUTPUT);
  pinMode(msel0, OUTPUT);
  
  /* -- MERUS AUDIO device intialization --
   * 1) Gain at 26dB  
   * 2) PMP selection
   * 3) Device error on interrupt
   */
 
  attachInterrupt (1, volume_up, FALLING);   // attach interrupt handler 
                                                // 1 is the internal interrupt attached to pin 3
                                                // interrupt triggered on falling edge

  attachInterrupt (0, volume_down, FALLING);   // attach interrupt handler 
                                                // 1 is the internal interrupt attached to pin 3
                                                // interrupt triggered on falling edge
                                                  
  /* ---- Set output configuration -----
   *  At start up keep device in reset 
   *  in order to set mode selection
   *  for SE, BTL, or PBTL
   *  After mode selction enable device
   * ----------------------------------- */
   digitalWrite(mute_n, LOW);
   digitalWrite(enable_n, HIGH);
   digitalWrite(msel0, HIGH);
   delay(100);
   digitalWrite(enable_n, LOW);
   delay(100); 
  

  I2c.begin();                                  // begin I2C communication
  
  /* ------- Clear Error Handler --------
   *  Error registers need to be cleared 
   *  at start-up. This is done by toggle
   *  eh_clear (0x2D) bit 2.
   *  ---------------------------------- */
  I2c.write(0x20,0x2D,0x34);                    // set eh_clear to 1
  I2c.write(0x20,0x2D,0x30);                    // set eh_clear to 0
  
  /* ------- Set I2S config--- --------
   *  
   *  ---------------------------------- */
  I2c.write(0x20,0x35,0x08);                    // set i2s dataformat and enable processor
  I2c.write(0x20,0x36,0x11);                    // set 32fs bitclock for 16bits data
  I2c.write(0x20,0x40,0x54);                    // -60dB master volume
  
  
 

  /* ---- Select Power Mode Profile ----
   *  Power Mode Profile can be set in 
   *  register PMprofile (0x1D) 
   *  ---------------------------------- */

  pmp_sel = 4;                                  // PMP select: 0=PMP0; 1=PMP1; 2=PMP2; 3=PMP3; 4=PMP4

  switch (pmp_sel) {
    case 0:                                     // PMP 0 case (default)
    /* set audio_in_mode (0x1D)
    * default:   0000 0000 (0x00)
    * new value: 0000 0000 (0x00) */
    I2c.write(0x20,0x1D,0x00);                  // PMprofile = 0
    break;

    case 1:                                     // PMP 1 case
    /* set audio_in_mode (0x1D)
    * default:   0000 0000 (0x00)
    * new value: 0000 0001 (0x01) */
    I2c.write(0x20,0x1D,0x01);                  // PMprofile = 1
    break;

    case 2:                                     // PMP 2 case  
    /* set audio_in_mode (0x1D)
    * default:   0000 0000 (0x00)
    * new value: 0000 0010 (0x02) */
    I2c.write(0x20,0x1D,0x02);                  // PMprofile = 2
    break;

    case 3:                                     // PMP 3 case
    /* set audio_in_mode (0x1D)
    * default:   0000 0000 (0x00)
    * new value: 0000 0011 (0x00) */
    I2c.write(0x20,0x1D,0x03);                  // PMprofile = 3
    break;

    case 4:                                     // PMP 4 case
    /* set audio_in_mode (0x1D)
    * default:   0000 0000 (0x00)
    * new value: 0000 0100 (0x00) */
    I2c.write(0x20,0x1D,0x04);                  // PMprofile = 4
    break;

    default:
    // no valid selection
    break;
  }
  
  I2c.end();                                    // stop I2C communication
  delay(100); 
  digitalWrite(mute_n, HIGH); 
      
}  // end of setup

 /* ---- Select Power Mode Profile ----
   *  Power Mode Profile can be set in 
   *  register PMprofile (0x1D) 
   *  ---------------------------------- */
void volume_up ()                           // Interrupt Service Routine (ISR)
{
  I2c.begin();                                 // begin I2C communication
  I2c.read(0x20,0x40,1);                       // read current volume
  
  curr_vol = I2c.receive();
  curr_vol = curr_vol - 1;
  
  I2c.write(0x20,0x40,curr_vol);               // write new volume

  I2c.end();                                   // stop I2C communication   
}  

void volume_down ()                           // Interrupt Service Routine (ISR)
{
  I2c.begin();                                 // begin I2C communication
  I2c.read(0x20,0x40,1);                       // read current volume
  
  curr_vol = I2c.receive();
  curr_vol = curr_vol + 1;
  
  I2c.write(0x20,0x40,curr_vol);               // write new volume

  I2c.end();                                   // stop I2C communication   
}  

void loop ()
{
  // wait for interrupt
  // MCU other instructions
} 


