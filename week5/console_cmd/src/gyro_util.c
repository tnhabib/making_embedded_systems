#include "gyro.h"
#include "i3g4250d.h"
#include "stm32f4xx_hal.h"
#include "gyro_util.h"
#include "main.h"


#define TIMx                           TIM3
#define TIMx_CLK_ENABLE                __HAL_RCC_TIM3_CLK_ENABLE
#define TIMx_IRQn                      TIM3_IRQn
#define TIMx_IRQHandler                TIM3_IRQHandler
static int gyroPollDelay = 50;
static float gyroMotionTolerance = 100000.0f;


#define ABS(x)         (x < 0) ? (-x) : x
static void Error_Handler(void);

volatile int timerExpired = 0;
TIM_HandleTypeDef    TimHandle;
uint16_t uwPreScalerValue;

void getGyroSample(float* xyzGyro) {
	I3G4250D_ReadXYZAngRate(xyzGyro);
}




int waitforGyroMotionDetection(float* xyzGyro) {
      BSP_LED_Init(LED3);
  BSP_LED_Init(LED4);
    int motionResult = -1;
    int motionDetected = 0;
      TimHandle.Instance = TIMx;
     /* Compute the prescaler value to have TIM3 counter clock equal to 10 KHz */
  uwPrescalerValue = (uint32_t) ((SystemCoreClock /2) / 10000) - 1;
  timerExpired = 0;
  TimHandle.Init.Period = 20000 - 1;
  TimHandle.Init.Prescaler = uwPrescalerValue;
  TimHandle.Init.ClockDivision = 0;
  TimHandle.Init.CounterMode = TIM_COUNTERMODE_UP;
  TimHandle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

   

  if(HAL_TIM_Base_Init(&TimHandle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
    timerExpired = 0;

    while (!timerExpired && !motionDetected) {
        //   TimHandle.Instance->SR = 0;
        //  __HAL_TIM_CLEAR_FLAG(&TimHandle, TIM_SR_UIF);
        if(HAL_TIM_Base_Start_IT(&TimHandle) != HAL_OK)
        {
            /* Starting Error */
            Error_Handler();
        }
     
		I3G4250D_ReadXYZAngRate(xyzGyro);
		if(xyzGyro[0] > gyroMotionTolerance)
		{ 
			// Down Detected
            motionResult = 4;
            motionDetected = 1;
		}
		else if(xyzGyro[0] < -gyroMotionTolerance)
		{ 
			// Up Detected
            motionResult =  2;
            motionDetected = 1;
		}      
        
		if(xyzGyro[1] < -gyroMotionTolerance)
		{
			// Left Detected
            motionResult =  1;
            motionDetected = 1;
		}
		else if(xyzGyro[1] > gyroMotionTolerance)
		{
			// Right Detected
			motionResult = 3;
            motionDetected = 1;
		}      	
		HAL_Delay(gyroPollDelay);
	}
    // timer Expired
    if (timerExpired) {
        motionResult = -1;
    }
    // HAL_TIM_Base_Stop_IT(&TimHandle);
    return motionResult;
}
void setGyroMotionTolerance(float t) {
    gyroMotionTolerance = t;
}


void Gyro_Init() {

  
    GYRO_InitTypeDef         Gyro_InitStructure;
    GYRO_FilterConfigTypeDef Gyro_FilterStructure = {0,0};
    uint16_t ctrl = 0x0000;


    Gyro_InitStructure.Power_Mode       = I3G4250D_MODE_ACTIVE;
    Gyro_InitStructure.Output_DataRate  = I3G4250D_OUTPUT_DATARATE_1;
    Gyro_InitStructure.Axes_Enable      = I3G4250D_AXES_ENABLE;
    Gyro_InitStructure.Band_Width       = I3G4250D_BANDWIDTH_4;
    Gyro_InitStructure.BlockData_Update = I3G4250D_BlockDataUpdate_Continous;
    Gyro_InitStructure.Endianness       = I3G4250D_BLE_MSB;
    Gyro_InitStructure.Full_Scale       = I3G4250D_FULLSCALE_500;

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




static void Error_Handler(void)
{
  /* Turn LED4 on */
  BSP_LED_On(LED4);
  while(1)
  {
  }
}


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    
     if (htim->Instance == TIM3) {
        BSP_LED_Toggle(LED3);
        timerExpired = 1;
        // HAL_TIM_Base_Stop_IT(&TimHandle);
     }
   
}



void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
  /*##-1- Enable peripherals and GPIO Clocks #################################*/
  /* TIMx Peripheral clock enable */
  TIMx_CLK_ENABLE();

  /*##-2- Configure the NVIC for TIMx ########################################*/
  /* Set the TIMx priority */
  HAL_NVIC_SetPriority(TIMx_IRQn, 0, 1);
  
  /* Enable the TIMx global Interrupt */
  HAL_NVIC_EnableIRQ(TIMx_IRQn);

}

void TIMx_IRQHandler(void)
{
  HAL_TIM_IRQHandler(&TimHandle);
  
}
