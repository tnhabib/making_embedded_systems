#include "main.h"
#include <stdio.h>
// #include <stdlib.h>

static void SystemClock_Config(void);
void Error_Handler(void);
void EXTI0_Callback(uint16_t GPIO_Pin);
void GyroInit();


int main(void) {
 
    uint8_t buf[1000];
    uint8_t buf_1[1000];
    uint8_t buf_2[1000];
 
    double xyzGyro[3];
    
  


    HAL_Init();
    SystemClock_Config();
    BSP_LCD_Init();
 
    BSP_LCD_LayerDefaultInit(0, LCD_FRAME_BUFFER);
    BSP_LCD_SelectLayer(0);
    BSP_LCD_DisplayOn();

    //BSP_LCD_SelectLayer(0);
    BSP_LCD_Clear(LCD_COLOR_BLUE);

     
  
    // BSP_LCD_DrawLine(20,20, 100,100);
    // BSP_LCD_SetTextColor(LCD_COLOR_LIGHTGREEN);
    // BSP_LCD_FillRect(20,20,100,100);

    GyroInit();

    while (1)
    {
  
      I3G4250D_ReadXYZAngRate(xyzGyro);
     
      sprintf(buf,  "X: %4.2f", xyzGyro[0]);
      sprintf(buf_1,  "Y: %4.2f", xyzGyro[1]);
      sprintf(buf_2,  "Z: %4.2f", xyzGyro[2]);
      
      BSP_LCD_DisplayStringAtLine(0, buf);
      BSP_LCD_DisplayStringAtLine(1, buf_1);
      BSP_LCD_DisplayStringAtLine(2, buf_2);
      HAL_Delay(1000);
     
    }
    
    return 0;
}


void GyroInit() {

  
    GYRO_InitTypeDef         Gyro_InitStructure;
    GYRO_FilterConfigTypeDef Gyro_FilterStructure = {0,0};
    uint16_t ctrl = 0x0000;


    Gyro_InitStructure.Power_Mode       = I3G4250D_MODE_ACTIVE;
    Gyro_InitStructure.Output_DataRate  = I3G4250D_OUTPUT_DATARATE_1;
    Gyro_InitStructure.Axes_Enable      = I3G4250D_AXES_ENABLE;
    Gyro_InitStructure.Band_Width       = I3G4250D_BANDWIDTH_4;
    Gyro_InitStructure.BlockData_Update = I3G4250D_BlockDataUpdate_Continous;
    Gyro_InitStructure.Endianness       = I3G4250D_BLE_LSB;
    Gyro_InitStructure.Full_Scale       = I3G4250D_FULLSCALE_245;

    /* Configure MEMS: data rate, power mode, full scale and axes */
    ctrl = (uint16_t) (Gyro_InitStructure.Power_Mode  | Gyro_InitStructure.Output_DataRate | \
                       Gyro_InitStructure.Axes_Enable | Gyro_InitStructure.Band_Width);

    ctrl |= (uint16_t) ((Gyro_InitStructure.BlockData_Update | Gyro_InitStructure.Endianness | \
                         Gyro_InitStructure.Full_Scale) << 8);
    I3G4250D_Init(ctrl);

    Gyro_FilterStructure.HighPassFilter_Mode_Selection   = I3G4250D_HPM_NORMAL_MODE_RES;
    Gyro_FilterStructure.HighPassFilter_CutOff_Frequency = I3G4250D_HPFCF_0;

    ctrl = (uint8_t) ((Gyro_FilterStructure.HighPassFilter_Mode_Selection |\
                       Gyro_FilterStructure.HighPassFilter_CutOff_Frequency));

    I3G4250D_FilterConfig(ctrl);

    I3G4250D_FilterCmd(I3G4250D_HIGHPASSFILTER_ENABLE);
    

}

void SysTick_Handler(void) {
  HAL_IncTick();
}

static void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;

  /* Enable Power Control clock */
  __PWR_CLK_ENABLE();
  
  /* The voltage scaling allows optimizing the power consumption when the device is 
     clocked below the maximum system frequency, to update the voltage scaling value 
     regarding system frequency refer to product datasheet.  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  
  /* Enable HSE Oscillator and activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 360;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
  
  if(HAL_PWREx_ActivateOverDrive() != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
  
  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 
     clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;  
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;  
  if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
}

// copied from Cube IDE Generated code
// void SystemClock_Config(void)
// {
//   RCC_ClkInitTypeDef RCC_ClkInitStruct;
//   RCC_OscInitTypeDef RCC_OscInitStruct;

//   /* Enable Power Control clock */
//   __HAL_RCC_PWR_CLK_ENABLE();
  
//   /* The voltage scaling allows optimizing the power consumption when the device is 
//      clocked below the maximum system frequency, to update the voltage scaling value 
//      regarding system frequency refer to product datasheet.  */
//   __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  
//   /* Enable HSE Oscillator and activate PLL with HSE as source */
//   RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
//   RCC_OscInitStruct.HSEState = RCC_HSE_ON;
//   RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
//   RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
//   RCC_OscInitStruct.PLL.PLLM = 8;
//   RCC_OscInitStruct.PLL.PLLN = 360;
//   RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
//   RCC_OscInitStruct.PLL.PLLQ = 7;
//   HAL_RCC_OscConfig(&RCC_OscInitStruct);

//   /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 
//      clocks dividers */
//   RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
//   RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
//   RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
//   RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;  
//   RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;  
//   HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);
// }


// copied from Cube IDE Generated code
void EXTI0_IRQHandler(void)
{
    
}


// programmer provided callback
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{

}

// copied from Cube IDE Generated code
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}


