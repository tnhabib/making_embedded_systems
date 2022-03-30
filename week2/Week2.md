
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
* SD Card interface with dedicated SPI

## STM32 F3 Discovery
* MEMs motion sensor with 3-axis digital output gyroscope
* MEMs package with 2 sensors
   * 3D digital linear accleration sensor 
   * 3D digital magnetic sensor
