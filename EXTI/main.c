
#include "stm32f10x.h"
#include "stm32f10x_conf.h"
#include <stdio.h>
#include <stdlib.h>

void EXTI0_Config(void);
void EXTI0_IRQHandler(void);
void gpio_toggle(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);

void gpio_toggle(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
  GPIOx->ODR ^= GPIO_Pin;
}

static inline void Delay_1us(uint32_t nCnt_1us)
{
  volatile uint32_t nCnt;

  for (; nCnt_1us != 0; nCnt_1us--)
    for (nCnt = 13; nCnt != 0; nCnt--);
}

void EXTI0_IRQHandler()
{
        Delay_1us(500);
        if (EXTI_GetITStatus(EXTI_Line0) != RESET){ // clear flag (TIM_IT_Update)

                gpio_toggle(GPIOC, GPIO_Pin_13);

                EXTI_ClearITPendingBit(EXTI_Line0);
        }
}

void EXTI0_Config(void)
{
  /* Enable GPIOA clock */
  GPIO_InitTypeDef   GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  /* Configure PA.00 pin as input floating */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* Enable AFIO clock(EXTI) */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
  /* Connect EXTI0 Line to PA.00 pin */
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);
  /* Configure EXTI0 line */
  EXTI_InitTypeDef   EXTI_InitStructure;
  EXTI_InitStructure.EXTI_Line = EXTI_Line0;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;  
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);

  NVIC_SetPriorityGrouping(NVIC_PriorityGroup_4);
  /* Enable and set EXTI0 Interrupt to the lowest priority */
  NVIC_InitTypeDef   NVIC_InitStructure;
  NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  NVIC_SetPriority(EXTI0_IRQn, NVIC_EncodePriority(4,1,0));
}


int main(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
  //Configure LED Pin
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Pin =   GPIO_Pin_13;  
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  EXTI0_Config();

          
  while (1)
  {
  }
}



