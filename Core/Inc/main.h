/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define key_Pin GPIO_PIN_3
#define key_GPIO_Port GPIOA
#define key_EXTI_IRQn EXTI3_IRQn
#define rotaty_m2_Pin GPIO_PIN_4
#define rotaty_m2_GPIO_Port GPIOA
#define rotaty_m2_EXTI_IRQn EXTI4_IRQn
#define rotary_m1_Pin GPIO_PIN_5
#define rotary_m1_GPIO_Port GPIOA
#define rotary_m1_EXTI_IRQn EXTI9_5_IRQn
#define sound_Pin GPIO_PIN_6
#define sound_GPIO_Port GPIOA
#define ADC_sig_out_Pin GPIO_PIN_7
#define ADC_sig_out_GPIO_Port GPIOA
#define eeprom_wp_Pin GPIO_PIN_14
#define eeprom_wp_GPIO_Port GPIOB
#define reset_ESP_Pin GPIO_PIN_8
#define reset_ESP_GPIO_Port GPIOA
#define WiFi_TX_Pin GPIO_PIN_9
#define WiFi_TX_GPIO_Port GPIOA
#define WiFi_RX_Pin GPIO_PIN_10
#define WiFi_RX_GPIO_Port GPIOA
#define LCD_DC_Pin GPIO_PIN_15
#define LCD_DC_GPIO_Port GPIOA
#define LCD_SCK_Pin GPIO_PIN_3
#define LCD_SCK_GPIO_Port GPIOB
#define LCD_MISO_Pin GPIO_PIN_4
#define LCD_MISO_GPIO_Port GPIOB
#define LCD_MOSI_Pin GPIO_PIN_5
#define LCD_MOSI_GPIO_Port GPIOB
#define LCD_BACKLIGHT_Pin GPIO_PIN_6
#define LCD_BACKLIGHT_GPIO_Port GPIOB
#define LCD_CS_Pin GPIO_PIN_7
#define LCD_CS_GPIO_Port GPIOB
#define LCD_RESET_Pin GPIO_PIN_8
#define LCD_RESET_GPIO_Port GPIOB
#define loop_pwm_Pin GPIO_PIN_9
#define loop_pwm_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
