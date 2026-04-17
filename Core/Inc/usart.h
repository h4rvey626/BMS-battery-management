/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    usart.h
  * @brief   This file contains all the function prototypes for
  *          the usart.c file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USART_H__
#define __USART_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */
#define ACTIVATE_USART1
#define ACTIVATE_USART3

// Print text unit
#define ASCII_CODE  ( 0x00 )
#define DECIMALISM  ( 0x01 )
#define HEXADECIMAL ( 0x02 )
#define DUMMY_BYTES ( 0x0004 )
#define USART1_DATA_TAG  ( 0x53 )
#define USART2_DATA_TAG  ( 0x53 )
#define USART3_DATA_TAG  ( 0x53 )
#define LENGTH  ( 0x0400 )
// USART stop receive macro
#define USART1_STOP_TRUE  ( 0x01 )
#define USART1_STOP_FALSE ( 0x00 )
#define USART2_STOP_TRUE  ( 0x01 )
#define USART2_STOP_FALSE ( 0x00 )
#define USART3_STOP_TRUE  ( 0x01 )
#define USART3_STOP_FALSE ( 0x00 )

/* USER CODE END Includes */

extern UART_HandleTypeDef huart1;

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

void MX_USART1_UART_Init(void);

/* USER CODE BEGIN Prototypes */
void USART_SendData(USART_TypeDef* USARTx, uint8_t Data);
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __USART_H__ */

