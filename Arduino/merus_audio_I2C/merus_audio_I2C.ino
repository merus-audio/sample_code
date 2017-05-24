/*----------------------------------------------------------
 * Title:   I2C communication setup
 * Author:  Rien Oortgiesen
 * Company: Merus Audio
 * 
 * Description:
 * This code demonstrates initial I2C communication 
 * using Arduino Uno together with MA devices
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
 * 20170524: intial sample version
 * 20170514: tested version + comments 
 *  
 * This code is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 */


#include <I2C.h>                                // plug-in your MCU specific I2C library

void setup ()
{
  /* -- MERUS AUDIO device intialization --
   * 1) Gain at 26dB  
   * 2) PMP selection
   */
  
  I2c.begin();                                  // begin I2C communication
  
  /* ------- Select Analog Gain --------
   *  Analog gain can be set by setting 
   *  register audio_in_mode_ext (0x27) 
   *  and register audio_in_mode (0x25)
   *  ---------------------------------- */
  
  /* set audio_in_mode_ext (0x27)
   * default:   0000 1000 (0x08)
   * new value: 0010 1000 (0x28) */
  I2c.write(0x20,0x27,0x28);                    // audio_in_mode_ext = 1
  
  int gain = 26;                                // select gain: 20=20dB; 26=26dB
  
  switch (gain) {
    case 20:                                    // 20dB gain case (default)
    /* set audio_in_mode (0x25)
    * default:   0001 0000 (0x16)
    * new value: 0001 0000 (0x16) */
    I2c.write(0x20,0x25,0x16);                  // audio_in_mode = 0
    break;
    
    case 26:                                    // 26dB gain case 
    /* set audio_in_mode (0x25)
    * default:   0001 0000 (0x16)
    * new value: 0011 0000 (0x30) */
    I2c.write(0x20,0x25,0x30);                  // audio_in_mode = 1
    break;

    default:
    // no valid selection
    break;
  }

  /* ---- Select Power Mode Profile ----
   *  Power Mode Profile can be set in 
   *  register PMprofile (0x1D) 
   *  ---------------------------------- */

  int pmp_sel = 1;                              // PMP select: 0 = PMP0; 1=PMP1; 2=PMP2; 3=PMP3; 4=PMP4

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
    // no valid selction
    break;
  }
  
  I2c.end();                                    // stop I2C communication

    
}  // end of setup

void loop ()
{
  // MCU other instructions
} 


