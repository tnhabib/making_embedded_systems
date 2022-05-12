
#include "main.h"
#include "console.h"
#include "gyro_util.h"
#include "LcdDisplay.h"
#include "game.h"

void UserButton_Init();
static void MX_NVIC_Init();
void EXTI0_Callback(uint16_t GPIO_Pin);

static void SystemClock_Config(void);

#define USER_BUTTON_PORT GPIOA
#define USER_BUTTON_PIN GPIO_PIN_0

/* Clocks */

#define USER_BUTTON_CLK_ENABLE() __HAL_RCC_GPIOA_CLK_ENABLE()

int main(void) {

  volatile enum GameState currentGameState = TITLE_SCREEN; 

  // __HAL_DBGMCU_FREEZE_TIM3();
  HAL_Init();

  SystemClock_Config();
    
  MX_NVIC_Init();
  
  UserButton_Init();

  /// Setup LCD 
  BSP_LCD_Init();
  BSP_LCD_LayerDefaultInit(0, LCD_FRAME_BUFFER);
  BSP_LCD_SelectLayer(0);
  BSP_LCD_DisplayOn();
  BSP_LCD_Clear(LCD_COLOR_WHITE);


  // Setup Gyro Sensor
  Gyro_Init();


  // Setup Command Line Console
  ConsoleInit();

  
  while(1) {
 
    nextGameState(currentGameState);
    ConsoleProcess();
    currentGameState = getGameState(); 
  }

}

void SysTick_Handler(void)
{
  HAL_IncTick();
}



static void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;

  /* Enable Power Control clock */
  __HAL_RCC_PWR_CLK_ENABLE();
  
  /* The voltage scaling allows optimizing the power consumption when the device is 
     clocked below the maximum system frequency, to update the voltage scaling value 
     regarding system frequency refer to product datasheet.  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  
  /* Activate the Over-Drive mode */
  HAL_PWREx_EnableOverDrive();

  /* Enable HSE Oscillator and activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 360;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);

  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 
     clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;  
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;  
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);
}






static void MX_NVIC_Init(void)
{
  /* EXTI0_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(EXTI0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI0_IRQn);
  
}




void UserButton_Init() {
    USER_BUTTON_CLK_ENABLE();
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.Pin = USER_BUTTON_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(USER_BUTTON_PORT, &GPIO_InitStruct);
    
}


void EXTI0_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(USER_BUTTON_PIN);
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  enum GameState curGameState = getGameState();
  // only start the same if in TITLE or GAME OVER state
  if (curGameState == TITLE_SCREEN || curGameState == GAME_OVER) {
    setGameState(START_GAME);
  }
  
}