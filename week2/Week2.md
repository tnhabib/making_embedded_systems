
# High Level Comparision
|                          | STM32F3 Discovery Board                 | Arduino MKRZero                  | 
|--------------------------|-----------------------------------------|----------------------------------|
| **Processor**            |  STM32F303VCT6 (Arm Cortex M4)          | ATSAMD21GA18A-48 (Arm Cortex M0+) |  
| **Flash Memory**         |  256 KB                                 | 256KB                            |  
| **RAM**                  |  48 KB                                  | 32 KB                                          |  
| **Clock Speed**          |  32 kHz(RTC), 72 Mhz                    | 32.768 kHz (RTC), 48Mhz                         |
| **Board Price**          |  \$15.75 (Digikey)                       | \$25.20 (Arduino Store)                          |
| **Processor Price**      |  \$9.97 (Digikey), \$9.96 (Mouser)        | \$4.06 (Digikey, ATSAMD21G18A-MU), \$4.12 (Mouser) | 

# Peripherals
Some interesting peripherals for each board
## Arduino MKZero
* SD Card interface with dedicated SPI (SPI1)
* Sleepwalking Peripheral modes
  * allows peripherals to wake up from standby based on specificed conditions
* [I2S (Inter-IC-Sound)](https://www.sparkfun.com/datasheets/BreakoutBoards/I2SBUS.pdf) interface to connect/communicate PCM audio data
  * Phillips standard with at least 3 wires:
    1. SCK (Serial Clock)
    2. FS (Frame Select)
    3. SD (Serial Data)

## STM32 F3 Discovery
* ST-LINK 
  * Progamming / Debugging tool with Virtual COM and Mass Storage over USB
* MEMs motion sensor with 3-axis digital output gyroscope (I2C / SPI Interface)
* MEMs package with 2 sensors (I2C Interface)
   * 3D digital linear accleration sensor 
   * 3D digital magnetic sensor (eCompass)


## ADC Interfaces

#### Arduino MKZero
   * 8-, 10- or 12-bit resolution
   * Up to 350,000 samples per second (350ksps)
   * Optional DMA transfer of conversion result
   * Averaging and oversampling with decimation to support, up to 16-bit result
   * Selectable sampling time

#### STM32 F3 Discovery
* 2 ADC Interfaces (ADC1 - Master / ADC2)
   * 10 channels on ADC1
   * 12 channels on ADC2
* Self-calibration

## Application Notes

* [STM32 F3 Discovery eCompass Application Notes](https://www.st.com/resource/en/application_note/an4825-ultracompact-highperformance-ecompass-module-based-on-the-lsm303agr-stmicroelectronics.pdf)
   * This document details on how to operate the accelerometer and magnetomer

* Arduio MKRZero
 * Since this is more of a consumer end product I found the following libraries with supported sample applications:
   * [Arduino Sound Library](https://www.arduino.cc/en/Reference/ArduinoSound)
      * Clap Detector
      * Whistle Detector
      * Amplitude Serial Plotter

   * [Arduino Low Power Library](https://www.arduino.cc/en/Reference/ArduinoLowPower) - Demonstrate how to wake up board from an external source
 