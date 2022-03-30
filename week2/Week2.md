
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
   * 3D digital magnetic sensor


## ADC Interfaces

#### Arduino MKZero
   * 8-, 10- or 12-bit resolution
   * Up to 350,000 samples per second (350ksps)
   * Differential and single-ended inputs
      * Up to 32 analog input
      * 25 positive and 10 negative, including internal and external
   * Five internal inputs
      * Bandgap
      * Temperature sensor
      * DAC
      * Scaled core supply
      * Scaled I/O supply
* 1/2x to 16x gain
* Single, continuous and pin-scan conversion options
* Windowing monitor with selectable channel
* Conversion range:
   *  Vref [1v to VDDANA - 0.6V] 
   * ADCx * GAIN [0V to -Vref ]
* Built-in internal reference and external reference options
   * Four bits for reference selection
* Event-triggered conversion for accurate timing (one event input)
* Optional DMA transfer of conversion result
* Hardware gain and offset compensation
* Averaging and oversampling with decimation to support, up to 16-bit result
* Selectable sampling time

#### STM32 F3 Discovery
* 2 ADC Interfaces (ADC1 - Master / ADC2)
   * 10 channels on ADC1
   * 12 channels on ADC2


## Application Notes

* STM32 F3 Discovery