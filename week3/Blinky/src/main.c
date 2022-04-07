#include "main.h"

/* PIN Definitions */
#define GREEN_LED_PIN GPIO_PIN_13
#define RED_LED_PIN GPIO_PIN_14
#define LED_GPIO_PORT GPIOG

#define USER_BUTTON_PORT GPIOA
#define USER_BUTTON_PIN GPIO_PIN_0

/* Clocks */
#define LED_GPIO_CLK_ENABLE() __HAL_RCC_GPIOG_CLK_ENABLE()
#define USER_BUTTON_CLK_ENABLE() __HAL_RCC_GPIOA_CLK_ENABLE()

void LED_Init();
void UserButton_Init();


int main(void) {
    volatile int counter = 0;
    HAL_Init();
    LED_Init();
    UserButton_Init();
    while (1)
    {
        if (HAL_GPIO_ReadPin(USER_BUTTON_PORT, USER_BUTTON_PIN)) {
            HAL_GPIO_WritePin(LED_GPIO_PORT,GREEN_LED_PIN,GPIO_PIN_RESET);
            HAL_GPIO_WritePin(LED_GPIO_PORT,RED_LED_PIN,GPIO_PIN_RESET);
            HAL_Delay(200);
        } else {
            if (counter % 2 == 0) {
                HAL_GPIO_TogglePin(LED_GPIO_PORT, GREEN_LED_PIN);
            } else {
                HAL_GPIO_TogglePin(LED_GPIO_PORT, RED_LED_PIN); 
            }
            
            HAL_Delay(1000);
            counter++;
        }
       
    }
    
    return 0;
}

void UserButton_Init() {
    USER_BUTTON_CLK_ENABLE();
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.Pin = USER_BUTTON_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
    HAL_GPIO_Init(USER_BUTTON_PORT, &GPIO_InitStruct);
}

void LED_Init() {
    LED_GPIO_CLK_ENABLE();
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.Pin = GREEN_LED_PIN | RED_LED_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
    HAL_GPIO_Init(LED_GPIO_PORT, &GPIO_InitStruct);

}

void SysTick_Handler(void) {
  HAL_IncTick();
}