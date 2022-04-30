
# Command line Interface

Code is located in the "console_cmd" folder

The following three commands were added:
* drawC - draws a Circle on the screen (Game Display) in one of 4 places depending on the integer passed (1,2,3,4)
* getGM - returns the direction (1,2,3,4) that gyro acceleration that passed the threshold, this number can be fed into a routine to draw on the display
* setGT - sets the tolerance value that is used in motion detection

**_NOTE:_** I had to update this header: .platformio/packages/framework-stm32cubef4/Drivers/STM32F4xx_HAL_Driver/Inc/stm32f4xx_hal_conf.h to get the clock to be set correctly for the UART and the LCD
```
#if !defined  (HSE_VALUE) 
  #define HSE_VALUE    ((uint32_t)8000000) /!< Default value of the External oscillator in Hz/
#endif /* HSE_VALUE /

#if !defined  (HSI_VALUE)
  #define HSI_VALUE    ((uint32_t)16000000) /!< Value of the Internal oscillator in Hz/
#endif / HSI_VALUE */
```