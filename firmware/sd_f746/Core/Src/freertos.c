/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * File Name          : freertos.c
 * Description        : Code for freertos applications
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2025 STMicroelectronics.
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
#include <stdio.h>
#include <string.h>
#include "fatfs.h"
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
FATFS fs;
/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
		.name = "defaultTask",
		.stack_size = 1024 * 4,
		.priority = (osPriority_t) osPriorityNormal,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
FRESULT sd_ls(const char *path);
/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
 * @brief  FreeRTOS initialization
 * @param  None
 * @retval None
 */
void MX_FREERTOS_Init(void) {
	/* USER CODE BEGIN Init */

	/* USER CODE END Init */

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
	/* add queues, ... */
	/* USER CODE END RTOS_QUEUES */

	/* Create the thread(s) */
	/* creation of defaultTask */
	defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

	/* USER CODE BEGIN RTOS_THREADS */
	/* add threads, ... */
	/* USER CODE END RTOS_THREADS */

	/* USER CODE BEGIN RTOS_EVENTS */
	/* add events, ... */
	/* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
 * @brief  Function implementing the defaultTask thread.
 * @param  argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
	/* USER CODE BEGIN StartDefaultTask */
	printf("Entering Default Task\r\n");

	FRESULT res = f_mount(&fs, "", 1);
	if (res != FR_OK)
	{
		printf("Error mounting device %d\r\n", res);
		Error_Handler();
	}
	printf("fs mounted\r\n");

	res = sd_ls("/");
	if (res != FR_OK)
	{
		printf("Error sd_ls %d\r\n", res);
		Error_Handler();
	}
	printf("ls\r\n");

	FIL f;
	res = f_open(&f, "test.txt", FA_WRITE | FA_CREATE_ALWAYS);	// Or FA_CREATE_ALWAYS ?
	if (res != FR_OK)
	{
		printf("Error f_open %d\r\n", res);
		Error_Handler();
	}

	static char str[] = "Hello,world\r\n";

	UINT bw;
	res = f_write(&f, str, strlen(str), &bw);
	if (res != FR_OK)
	{
		printf("Error f_write %d\r\n", res);
		Error_Handler();
	}

	printf("written %u/%d\r\n", bw, strlen(str));

	res = f_close(&f);
	if (res != FR_OK)
	{
		printf("Error f_close %d\r\n", res);
		Error_Handler();
	}
	/* Infinite loop */
	for(;;)
	{
		osDelay(1);
	}
	/* USER CODE END StartDefaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
FRESULT sd_ls(const char *path)
{
	FRESULT res;
	DIR dir;
	FILINFO fno;

	res = f_opendir(&dir, path);  // Ouvre le répertoire
	if (res == FR_OK)
	{
		while (1)
		{
			res = f_readdir(&dir, &fno);
			if (res != FR_OK || fno.fname[0] == 0)
			{
				break; // Fin
			}
			printf("%s%s\r\n", fno.fname, (fno.fattrib & AM_DIR) ? "/" : "");
		}
		return f_closedir(&dir);
	}

	return res;
}
/* USER CODE END Application */

