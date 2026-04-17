/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    i2c.h
  * @brief   This file contains all the function prototypes for
  *          the i2c.c file
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
#ifndef __I2C_H__
#define __I2C_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

extern I2C_HandleTypeDef hi2c1;

/* USER CODE BEGIN Private defines */

#define BQ76940_ADDR  0x08U   /* 7-bit I2C address, ADDR pin = GND */
#define I2C_TIMEOUT   100U    /* I2C timeout in ms */

/* USER CODE END Private defines */

void MX_I2C1_Init(void);

/* USER CODE BEGIN Prototypes */

HAL_StatusTypeDef I2C_WriteByte(uint8_t devAddr, uint8_t regAddr, uint8_t data);
HAL_StatusTypeDef I2C_ReadByte(uint8_t devAddr, uint8_t regAddr, uint8_t *data);
HAL_StatusTypeDef I2C_WriteBytes(uint8_t devAddr, uint8_t regAddr, uint8_t *data, uint16_t len);
HAL_StatusTypeDef I2C_ReadBytes(uint8_t devAddr, uint8_t regAddr, uint8_t *data, uint16_t len);

HAL_StatusTypeDef BQ76940_WriteByte_CRC(uint8_t regAddr, uint8_t data);
uint32_t          BQ76940_ReadBytes_u32(uint8_t regAddr, uint8_t len);

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __I2C_H__ */

