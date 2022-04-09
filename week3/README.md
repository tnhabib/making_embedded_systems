
Blinky Code

Board Chosen - STH32F429 Discovery Board
Build Environment:
 - VSCode with PlatformIO (STMCUBE32 Framework)
 - STMCubeMX to generate supporting code for a interrupt routine

# Building the Code with Platformio
In order to get the interrupt working, I used STMCube IDE to configure the EXTI0 Interrupt then copied the following functions:
* SystemClock_Config();
* MX_NVIC_Init();
* EXTI0_IRQHandler(void)


The EXTI0_IRQHandler calls HAL_GPIO_EXTI_IRQHandler() which expects the function with the following signature to be implemented:
* void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)

# Observations during coding and running code on the board

1. What are the hardware registers that cause the LED to turn on and off?

The Port G Output Data Register (ODR) is able to cause the LED to turn and off. The GPIOG register start boundary is 0x40021800 and the ODR Register Offset from that address is 0x14

2. What are the registers that you read in order to find out the state of the button?

This board uses Port A Input Data Register(IDR) register to read the state of the button. The GPIOA register boundary is located at 0x4002000 and the IDR register offse from that address is 0x10

3. Can you read the register directly and see the button change in a debugger or by printing out the value of the memory at the register’s address?
Yes, I was able to see the button change in the debbuger. Also, for my own education, I was also able to see and change the bit in the GPIOGODR register to make the LED turn and off manually in the debugger as well.


I did not have time to implement the Debounce code.