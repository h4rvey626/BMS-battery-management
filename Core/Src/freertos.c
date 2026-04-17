/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
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
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "BQ76930.h"
#include "usart.h"
#include "i2c.h"
#include "string.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
/* Definitions for SampleTask */
osThreadId_t SampleTaskHandle;
const osThreadAttr_t SampleTask_attributes = {
  .name = "SampleTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityAboveNormal,
};
/* Definitions for CommTask */
osThreadId_t CommTaskHandle;
const osThreadAttr_t CommTask_attributes = {
  .name = "CommTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for balanceTask */
osThreadId_t balanceTaskHandle;
const osThreadAttr_t balanceTask_attributes = {
  .name = "balanceTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityBelowNormal7,
};
/* Definitions for protectTask */
osThreadId_t protectTaskHandle;
const osThreadAttr_t protectTask_attributes = {
  .name = "protectTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for bmsDataMutex */
osMutexId_t bmsDataMutexHandle;
const osMutexAttr_t bmsDataMutex_attributes = {
  .name = "bmsDataMutex"
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartSampleTask(void *argument);
void StartCommTask(void *argument);
void StartbalanceTask(void *argument);
void StartprotectTask(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
  /* BQ76940 硬件初始化 (必须在调度器启动前，此时 HAL_Delay 安全) */
  HAL_UART_Transmit(&huart1, (uint8_t *)"BQ76940 HW Init...\r\n", 20, HAL_MAX_DELAY);
  WAKE_ALL_DEVICE();
  HAL_UART_Transmit(&huart1, (uint8_t *)"WAKE done\r\n", 11, HAL_MAX_DELAY);
  BQ_1_config();
  HAL_UART_Transmit(&huart1, (uint8_t *)"BQ_1_config done\r\n", 18, HAL_MAX_DELAY);
  Get_offset();
  HAL_UART_Transmit(&huart1, (uint8_t *)"Get_offset done\r\n", 17, HAL_MAX_DELAY);
  OV_UV_1_PROTECT();
  HAL_UART_Transmit(&huart1, (uint8_t *)"OV_UV_1_PROTECT done\r\n", 22, HAL_MAX_DELAY);
  OCD_SCD_PROTECT();
  HAL_UART_Transmit(&huart1, (uint8_t *)"OCD_SCD_PROTECT done\r\n", 22, HAL_MAX_DELAY);
  BQ76940_WriteByte_CRC(SYS_STAT, 0xFF);
  HAL_UART_Transmit(&huart1, (uint8_t *)"HW Init complete\r\n", 18, HAL_MAX_DELAY);
  /* USER CODE END Init */
  /* Create the mutex(es) */
  /* creation of bmsDataMutex */
  bmsDataMutexHandle = osMutexNew(&bmsDataMutex_attributes);

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* queues removed - using task notifications instead */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of SampleTask */
  SampleTaskHandle = osThreadNew(StartSampleTask, NULL, &SampleTask_attributes);

  /* creation of CommTask */
  CommTaskHandle = osThreadNew(StartCommTask, NULL, &CommTask_attributes);

  /* creation of balanceTask */
  balanceTaskHandle = osThreadNew(StartbalanceTask, NULL, &balanceTask_attributes);

  /* creation of protectTask */
  protectTaskHandle = osThreadNew(StartprotectTask, NULL, &protectTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartSampleTask */
/**
  * @brief  Function implementing the SampleTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartSampleTask */
void StartSampleTask(void *argument)
{
  /* USER CODE BEGIN StartSampleTask */
  HAL_UART_Transmit(&huart1, (uint8_t *)"Sample Task Running\r\n", 21, HAL_MAX_DELAY);

  for(;;)
  {
    // 获取互斥量，保护数据更新
    if(osMutexAcquire(bmsDataMutexHandle, 100) == osOK)
    {
      Get_Battery1();
      Get_Battery2();
      Get_Battery5();
      Get_Battery6();
      Get_Battery7();
      Get_Battery10();
      Get_Battery11();
      Get_Battery12();
      Get_Battery15();
      Get_Update_ALL_Data();
      Get_SOC();
      Get_BQ1_2_Temp();
      Get_BQ_Current();
      BMS_STA();
      
      // 释放互斥量
      osMutexRelease(bmsDataMutexHandle);

      // 通知消费者任务数据已更新
      xTaskNotifyGive(CommTaskHandle);
      xTaskNotifyGive(protectTaskHandle);
    }

    osDelay(1000);
  }
  /* USER CODE END StartSampleTask */
}

/* USER CODE BEGIN Header_StartCommTask */
/**
* @brief Function implementing the CommTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartCommTask */
void StartCommTask(void *argument)
{
  /* USER CODE BEGIN StartCommTask */
  HAL_UART_Transmit(&huart1, (uint8_t *)"BQ76940 Comm Task Starting...\r\n", 32, HAL_MAX_DELAY);


  for(;;)
  {
    ulTaskNotifyTake(pdTRUE, osWaitForever);

    // 获取互斥量，安全访问共享数据
    if(osMutexAcquire(bmsDataMutexHandle, 100) == osOK)
    {
      Update_val_usart();
      osMutexRelease(bmsDataMutexHandle);
    }
  }
  /* USER CODE END StartCommTask */
}

/* USER CODE BEGIN Header_StartbalanceTask */
/**
* @brief Function implementing the balanceTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartbalanceTask */
void StartbalanceTask(void *argument)
{
  /* USER CODE BEGIN StartbalanceTask */
  int cell_voltages[9];
  static const int cell_idx[] = {0, 1, 4, 5, 6, 9, 10, 11, 14};

  /* 平衡函数指针数组，与 cell_idx 一一对应 */
  typedef void (*BalanceFunc)(void);
  static const BalanceFunc balance_funcs[9] = {
    Battery1_Balance,  Battery2_Balance,  Battery5_Balance,
    Battery6_Balance,  Battery7_Balance,  Battery10_Balance,
    Battery11_Balance, Battery12_Balance, Battery15_Balance
  };

  HAL_UART_Transmit(&huart1, (uint8_t *)"Balance Task Starting...\r\n", 26, HAL_MAX_DELAY);

  for(;;)
  {
    osDelay(5000); // 每5秒执行一次平衡检查

    // 获取互斥量，读取电池电压数据
    if(osMutexAcquire(bmsDataMutexHandle, 100) != osOK)
      continue;

    for(int i = 0; i < 9; i++)
    {
      cell_voltages[i] = Batteryval[cell_idx[i]];
    }
    osMutexRelease(bmsDataMutexHandle);

    // 任何保护标志激活时，禁止平衡
    if(OV_FLAG || UV_FLAG || OC_FLAG || OT_Alarm_flag || UT_Alarm_flag)
    {
      if(osMutexAcquire(bmsDataMutexHandle, 100) == osOK)
      {
        Stop_Balance();
        osMutexRelease(bmsDataMutexHandle);
      }
      continue;
    }

    // 查找最小电压和最大电压(及其索引)
    int min_voltage = cell_voltages[0];
    int max_voltage = cell_voltages[0];
    int max_idx = 0;

    for(int i = 1; i < 9; i++)
    {
      if(cell_voltages[i] < min_voltage)
        min_voltage = cell_voltages[i];
      if(cell_voltages[i] > max_voltage)
      {
        max_voltage = cell_voltages[i];
        max_idx = i;
      }
    }

    // 获取互斥量，执行平衡操作
    if(osMutexAcquire(bmsDataMutexHandle, 100) != osOK)
      continue;

    if((max_voltage - min_voltage) > BALANCE_THRESHOLD_MV)
    {
      // 对最高电压电芯开启被动均衡放电
      balance_funcs[max_idx]();
    }
    else
    {
      // 电芯电压已均衡，停止平衡
      Stop_Balance();
    }

    osMutexRelease(bmsDataMutexHandle);
  }
  /* USER CODE END StartbalanceTask */
}

/* USER CODE BEGIN Header_StartprotectTask */
/**
* @brief Function implementing the protectTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartprotectTask */
void StartprotectTask(void *argument)
{
  /* USER CODE BEGIN StartprotectTask */
  int cell_voltages[9];
  int current;
  float temperature;
  static const int cell_idx[] = {0, 1, 4, 5, 6, 9, 10, 11, 14};

  HAL_UART_Transmit(&huart1, (uint8_t *)"Protect Task Starting...\r\n", 26, HAL_MAX_DELAY);

  for(;;)
  {
    // 等待采样完成通知，超时2秒作为故障安全
    BaseType_t notifyResult = ulTaskNotifyTake(pdTRUE, 2000);

    if(notifyResult == 0)
    {
      // 采样任务超过2秒未更新，可能异常，执行安全动作
      if(osMutexAcquire(bmsDataMutexHandle, 100) == osOK)
      {
        Close_DSG_CHG();
        BQ76940_WriteByte_CRC(SYS_STAT, 0xFF);
        osMutexRelease(bmsDataMutexHandle);
      }
      continue;
    }

    // ==================== 阶段1: 读取数据 ====================
    if(osMutexAcquire(bmsDataMutexHandle, 100) != osOK)
      continue;

    for(int i = 0; i < 9; i++)
    {
      cell_voltages[i] = Batteryval[cell_idx[i]];
    }
    current = Batteryval[17];
    temperature = Tempval_2;

    // 读取BQ76940硬件报警标志 (SYS_STAT寄存器)
    ALERT_1_Recognition();

    osMutexRelease(bmsDataMutexHandle);

    // ==================== 阶段2: 评估保护条件 ====================
    int state_changed = 0;

    // --- 过压保护 (OV): 任一电芯 > 4200mV 关闭充电 ---
    int ov_detected = 0;
    int ov_all_recovered = 1;
    for(int i = 0; i < 9; i++)
    {
      if(cell_voltages[i] > OV_SOFT_THRESHOLD)
        ov_detected = 1;
      if(cell_voltages[i] >= OV_SOFT_RECOVERY)
        ov_all_recovered = 0;
    }

    if(ov_detected && !OV_FLAG)
    {
      OV_FLAG = 1;
      state_changed = 1;
    }
    else if(OV_FLAG && ov_all_recovered)
    {
      OV_FLAG = 0;
      state_changed = 1;
    }

    // --- 欠压保护 (UV): 任一电芯 < 2800mV 关闭放电 ---
    int uv_detected = 0;
    int uv_all_recovered = 1;
    for(int i = 0; i < 9; i++)
    {
      if(cell_voltages[i] < UV_SOFT_THRESHOLD)
        uv_detected = 1;
      if(cell_voltages[i] <= UV_SOFT_RECOVERY)
        uv_all_recovered = 0;
    }

    if(uv_detected && !UV_FLAG)
    {
      UV_FLAG = 1;
      state_changed = 1;
    }
    else if(UV_FLAG && uv_all_recovered)
    {
      UV_FLAG = 0;
      state_changed = 1;
    }

    // --- 过流保护 (OC): 电流 > 2000mA 关闭充放电 ---
    if((current > OC_THRESHOLD_MA) && !OC_FLAG)
    {
      OC_FLAG = 1;
      state_changed = 1;
    }
    else if(OC_FLAG && (current <= OC_THRESHOLD_MA))
    {
      OC_FLAG = 0;
      state_changed = 1;
    }

    // --- 过温保护 (OT): 温度 > OTPThreshold 关闭充放电 ---
    if((temperature > OTPThreshold) && !OT_Alarm_flag)
    {
      OT_Alarm_flag = 1;
      state_changed = 1;
    }
    else if(OT_Alarm_flag && (temperature <= OTPThreshold))
    {
      OT_Alarm_flag = 0;
      state_changed = 1;
    }

    // --- 低温保护 (UT): 温度 < UTPThreshold 关闭充放电 ---
    if((temperature < UTPThreshold) && !UT_Alarm_flag)
    {
      UT_Alarm_flag = 1;
      state_changed = 1;
    }
    else if(UT_Alarm_flag && (temperature >= UTPThreshold))
    {
      UT_Alarm_flag = 0;
      state_changed = 1;
    }

    // ==================== 阶段3: 根据所有标志综合控制MOS ====================
    if(osMutexAcquire(bmsDataMutexHandle, 100) != osOK)
      continue;

    // 判断当前是否有任何保护标志激活
    int any_fault = OV_FLAG || UV_FLAG || OC_FLAG || OT_Alarm_flag || UT_Alarm_flag;

    if(state_changed)
    {
      // 优先级: 关闭双MOS > 关闭单MOS > 全开
      if(OC_FLAG || OT_Alarm_flag || UT_Alarm_flag)
      {
        // 过流/过温/低温 → 关闭充电和放电MOS
        Close_DSG_CHG();
      }
      else if(OV_FLAG && UV_FLAG)
      {
        // 同时过压+欠压 → 关闭充电和放电MOS
        Close_DSG_CHG();
      }
      else if(OV_FLAG)
      {
        // 仅过压 → 关闭充电MOS，保持放电
        Only_Close_CHG();
      }
      else if(UV_FLAG)
      {
        // 仅欠压 → 关闭放电MOS，保持充电
        Only_Close_DSG();
      }
      else
      {
        // 所有保护已恢复 → 打开充电和放电MOS
        Open_DSG_CHG();
      }
    }
    else if(!any_fault)
    {
      // 兜底恢复: 所有保护标志已清零但state_changed未触发时
      // (例如: failsafe超时关闭MOS后恢复，或上电后标志本就为0)
      // 检查MOS实际状态，若关闭则重新打开
      BMS_STA();
      if(DSG_STA == 0 || CHG_STA == 0)
      {
        Open_DSG_CHG();
      }
    }

    // 每次循环都清除BQ76940状态寄存器，防止硬件报警锁死
    BQ76940_WriteByte_CRC(SYS_STAT, 0xFF);
    osMutexRelease(bmsDataMutexHandle);
  }
  /* USER CODE END StartprotectTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

