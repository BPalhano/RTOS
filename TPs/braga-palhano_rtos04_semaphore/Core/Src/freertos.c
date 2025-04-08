/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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

/* USER CODE BEGIN */
#include "ensi_uart.h"
#include "semphr.h"
#include "queue.h"

/* Declaring Queue */
QueueHandle_t SimpleQueue;

/* Declaring Semaphore (Mutex) */
SemaphoreHandle_t xSemaphore;

/**
* @brief Creation of software system architecture
*/
void app_init(void);

/**
* @brief Task 1 for test. Rename task with useful name for a real application
*/
static void task1(void);

/**
* @brief Task 2 for test. Rename task with useful name for a real application
*/
static void task2(void);

/**
* @brief Task 3 for test. Rename task with useful name for a real application
*/
static void task3(void);


void app_init(void) {
    /* application tasks creation */
	//xSemaphore = xSemaphoreCreateBinary();

	/* Defining Mutex */
	xSemaphore = xSemaphoreCreateMutex();

	/* Defining Queue */
	SimpleQueue = xQueueCreate(5, sizeof (char *));

	if ( SimpleQueue == 0 ) {
		ENSI_UART_PutString((uint8_t*)"\r\n Queue creation error");
	}

	/* Defining tasks */
	xTaskCreate(task1,
				"task1",
				configMINIMAL_STACK_SIZE,
				(void *) 1,
				2,
				NULL);

	 xTaskCreate(task2,
	 				"task2",
	 				configMINIMAL_STACK_SIZE,
	 				(void *) 1,
	 				1,
	 				NULL);

	 xTaskCreate(task3,
	 				"task3",
	 				configMINIMAL_STACK_SIZE,
	 				(void *) 1,
	 				1,
	 				NULL);


	 xSemaphoreGive ( xSemaphore );
}



static void task1(void) {
    // Task init
	char * queue_message = "\r\nAAAAAAAA";
	TickType_t tick = xTaskGetTickCount();
	// Task infinite loop
while(1) {
		HAL_GPIO_WritePin(Task1_GPIO_Port, Task1_Pin, GPIO_PIN_SET);
		//xSemaphoreGive( xSemaphore );
		xQueueSend(SimpleQueue, &queue_message, portMAX_DELAY );
		HAL_GPIO_WritePin(Task1_GPIO_Port, Task1_Pin, GPIO_PIN_RESET);
			vTaskDelayUntil(&tick, 300);

	}
}


static void task2(void) {
    // Task init
	char * received_message;
	// Task infinite loop
	while(1) {

			//xSemaphoreTake( xSemaphore, 3000 );
			xQueueReceive(SimpleQueue, &received_message, portMAX_DELAY);
			HAL_GPIO_WritePin(Task2_GPIO_Port, Task2_Pin, GPIO_PIN_SET);

			/* Critical section begin : shared resource -> UART channel */
			xSemaphoreTake ( xSemaphore, ( TickType_t ) 300 );
			//taskENTER_CRITICAL();


			ENSI_UART_PutString(received_message);
			ENSI_UART_PutString((uint8_t*)"\r\n22222222");

			/* Critical section end : shared resource -> UART channel */
			//taskEXIT_CRITICAL();

			xSemaphoreGive ( xSemaphore );

			HAL_GPIO_WritePin(Task2_GPIO_Port, Task2_Pin, GPIO_PIN_RESET);


	}
}


static void task3(void) {
    // Task init

	// Task infinite loop
	while(1) {

		HAL_GPIO_WritePin(Task3_GPIO_Port, Task3_Pin, GPIO_PIN_SET);

		/* Critical section begin : shared resource -> UART channel */
		xSemaphoreTake ( xSemaphore, ( TickType_t ) 300 );
		//taskENTER_CRITICAL();

		ENSI_UART_PutString((uint8_t*)"\r\n33333333");

		/* Critical section end : shared resource -> UART channel */
		//taskEXIT_CRITICAL();

		xSemaphoreGive ( xSemaphore );

		HAL_GPIO_WritePin(Task3_GPIO_Port, Task3_Pin, GPIO_PIN_RESET);
		vTaskDelay(1000);


	}
}


/* USER CODE END */

