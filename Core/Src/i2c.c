/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    i2c.c
  * @brief   This file provides code for the configuration
  *          of the I2C instances.
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
/* Includes ------------------------------------------------------------------*/
#include "i2c.h"

/* USER CODE BEGIN 0 */

static uint8_t CRC8(unsigned char *ptr, unsigned char len, unsigned char key)
{
    unsigned char i;
    unsigned char crc = 0;
    while (len-- != 0)
    {
        for (i = 0x80; i != 0; i /= 2)
        {
            if ((crc & 0x80) != 0) { crc *= 2; crc ^= key; }
            else                     crc *= 2;
            if ((*ptr & i) != 0)     crc ^= key;
        }
        ptr++;
    }
    return crc;
}

/* USER CODE END 0 */

I2C_HandleTypeDef hi2c1;

/* I2C1 init function */
void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

void HAL_I2C_MspInit(I2C_HandleTypeDef* i2cHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(i2cHandle->Instance==I2C1)
  {
  /* USER CODE BEGIN I2C1_MspInit 0 */

  /* USER CODE END I2C1_MspInit 0 */

    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**I2C1 GPIO Configuration
    PB8     ------> I2C1_SCL
    PB9     ------> I2C1_SDA
    */
    GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    __HAL_AFIO_REMAP_I2C1_ENABLE();

    /* I2C1 clock enable */
    __HAL_RCC_I2C1_CLK_ENABLE();
  /* USER CODE BEGIN I2C1_MspInit 1 */

  /* USER CODE END I2C1_MspInit 1 */
  }
}

void HAL_I2C_MspDeInit(I2C_HandleTypeDef* i2cHandle)
{

  if(i2cHandle->Instance==I2C1)
  {
  /* USER CODE BEGIN I2C1_MspDeInit 0 */

  /* USER CODE END I2C1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_I2C1_CLK_DISABLE();

    /**I2C1 GPIO Configuration
    PB8     ------> I2C1_SCL
    PB9     ------> I2C1_SDA
    */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_8);

    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_9);

  /* USER CODE BEGIN I2C1_MspDeInit 1 */

  /* USER CODE END I2C1_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */

/**
 * @brief  Write a single byte to a device register.
 * @param  devAddr  7-bit I2C device address (not shifted)
 * @param  regAddr  Register address
 * @param  data     Byte to write
 * @retval HAL_OK on success
 */
HAL_StatusTypeDef I2C_WriteByte(uint8_t devAddr, uint8_t regAddr, uint8_t data)
{
    return HAL_I2C_Mem_Write(&hi2c1, (uint16_t)(devAddr << 1), regAddr,
                             I2C_MEMADD_SIZE_8BIT, &data, 1, I2C_TIMEOUT);
}

/**
 * @brief  Read a single byte from a device register.
 * @param  devAddr  7-bit I2C device address (not shifted)
 * @param  regAddr  Register address
 * @param  data     Pointer to store the received byte
 * @retval HAL_OK on success
 */
HAL_StatusTypeDef I2C_ReadByte(uint8_t devAddr, uint8_t regAddr, uint8_t *data)
{
    return HAL_I2C_Mem_Read(&hi2c1, (uint16_t)(devAddr << 1), regAddr,
                            I2C_MEMADD_SIZE_8BIT, data, 1, I2C_TIMEOUT);
}

/**
 * @brief  Write multiple bytes starting from a device register.
 * @param  devAddr  7-bit I2C device address (not shifted)
 * @param  regAddr  Starting register address
 * @param  data     Pointer to data buffer
 * @param  len      Number of bytes to write
 * @retval HAL_OK on success
 */
HAL_StatusTypeDef I2C_WriteBytes(uint8_t devAddr, uint8_t regAddr, uint8_t *data, uint16_t len)
{
    return HAL_I2C_Mem_Write(&hi2c1, (uint16_t)(devAddr << 1), regAddr,
                             I2C_MEMADD_SIZE_8BIT, data, len, I2C_TIMEOUT);
}

/**
 * @brief  Read multiple bytes starting from a device register.
 * @param  devAddr  7-bit I2C device address (not shifted)
 * @param  regAddr  Starting register address
 * @param  data     Pointer to receive buffer
 * @param  len      Number of bytes to read
 * @retval HAL_OK on success
 */
HAL_StatusTypeDef I2C_ReadBytes(uint8_t devAddr, uint8_t regAddr, uint8_t *data, uint16_t len)
{
    return HAL_I2C_Mem_Read(&hi2c1, (uint16_t)(devAddr << 1), regAddr,
                            I2C_MEMADD_SIZE_8BIT, data, len, I2C_TIMEOUT);
}

/**
 * @brief  Write a single byte to a BQ76940 register with CRC.
 *         Protocol: START + [addr_w] + [regAddr] + [data] + [CRC8] + STOP
 *         CRC is computed over the three preceding bytes (including address).
 * @param  regAddr  Register address
 * @param  data     Byte to write
 * @retval HAL_OK on success
 */
HAL_StatusTypeDef BQ76940_WriteByte_CRC(uint8_t regAddr, uint8_t data)
{
    uint8_t crcBuf[3];
    crcBuf[0] = BQ76940_ADDR << 1;   /* write address byte, same as original */
    crcBuf[1] = regAddr;
    crcBuf[2] = data;

    uint8_t txBuf[3] = {regAddr, data, CRC8(crcBuf, 3, 7)};
    return HAL_I2C_Master_Transmit(&hi2c1, (uint16_t)(BQ76940_ADDR << 1),
                                   txBuf, 3, I2C_TIMEOUT);
}

/**
 * @brief  Read multiple consecutive registers and combine into a uint32_t.
 *         Matches original IIC1_read_len_byte: reads from (regAddr+len-1)
 *         down to regAddr, MSB first.
 * @param  regAddr  Base register address
 * @param  len      Number of bytes to read (1-4)
 * @retval Combined value
 */
uint32_t BQ76940_ReadBytes_u32(uint8_t regAddr, uint8_t len)
{
    uint8_t t, data;
    uint32_t temp = 0;
    for (t = 0; t < len; t++)
    {
        temp <<= 8;
        I2C_ReadByte(BQ76940_ADDR, regAddr + len - t - 1, &data);
        temp += data;
    }
    return temp;
}

/* USER CODE END 1 */
