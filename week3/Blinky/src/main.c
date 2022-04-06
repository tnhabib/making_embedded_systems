#include "main.h"

#define GREEN_LED_PIN GPIO_PIN_13
#define RED_LED_PIN GPIO_PIN_14
#define LED_GPIO_PORT GPIOG
#define LED_GPIO_CLK_ENABLE() __HAL_RCC_GPIOG_CLK_ENABLE()

void LED_Init();

int main(void) {
    volatile int counter = 0;
    HAL_Init();
    LED_Init();
    while (1)
    {
        if (counter % 2 == 0) {
            HAL_GPIO_TogglePin(LED_GPIO_PORT, GREEN_LED_PIN);
        } else {
           HAL_GPIO_TogglePin(LED_GPIO_PORT, RED_LED_PIN); 
        }
        
        HAL_Delay(1000);
        counter++;
    }
    
    return 0;
}

void LED_Init() {
    LED_GPIO_CLK_ENABLE();
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.Pin = GREEN_LED_PIN | RED_LED_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_SPEED_HIGH;
    HAL_GPIO_Init(LED_GPIO_PORT, &GPIO_InitStruct);

}

void SysTick_Handler(void) {
  HAL_IncTick();
}