#include "gyro.h"
#include "i3g4250d.h"
#include "stm32f4xx_hal.h"
#include "gyro_util.h"
#include "main.h"
#include <time.h>

static int gyroPollDelay = 1000;
static float gyroMotionTolerance = 100000.0f;
#define ABS(x)         (x < 0) ? (-x) : x

void getGyroSample(float* xyzGyro) {
	I3G4250D_ReadXYZAngRate(xyzGyro);
}

int waitforGyroMotionDetection() {
    float xyzGyro[3];
	float Xval, Yval, Zval = 0x00;
	while (1) {
		I3G4250D_ReadXYZAngRate(xyzGyro);
		/* Update autoreload and capture compare registers value */
    	Xval = ABS((xyzGyro[0]));
    	Yval = ABS((xyzGyro[1])); 
    	Zval = ABS((xyzGyro[2])); 
	    if((Xval>Yval) && (Xval>Zval)) {
			if(xyzGyro[0] > gyroMotionTolerance)
			{ 
				// Down Detected
                return 4;
			}
			else if(xyzGyro[0] < -gyroMotionTolerance)
			{ 
				// Up Detected
                return 2;
			}      
        }
		else if ((Yval>Xval) && (Yval>Zval)) {
			if(xyzGyro[1] < -gyroMotionTolerance)
			{
				// Left Detected
                return 1;
			}
			else if(xyzGyro[1] > gyroMotionTolerance)
			{
				// Right Detected
				return 3;
			}     
    	} 
		HAL_Delay(gyroPollDelay);
	}

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


