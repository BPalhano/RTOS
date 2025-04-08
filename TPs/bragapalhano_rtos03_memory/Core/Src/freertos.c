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


/**
* @brief Creation of software system architecture
*/
void app_init(void);

/**
* @brief Iterative function that only calls itself
*/
static void growth(void);

/**
* @brief Task 1 for test. Rename task with useful name for a real application
*/
static void task1(void *pvParameters);

/**
* @brief Task 2 for test. Rename task with useful name for a real application
*/
static void task2(void *pvParameters);

/**
* @brief Task 3 for test. Rename task with useful name for a real application
*/
static void task3(void *pvParameters);


void app_init(void) {
    /* application tasks creation */

	xTaskCreate(task1,       				/* Function that implements the task. */
				"Tache 1",          		/* Text name for the task. */
				configMINIMAL_STACK_SIZE,  	/* Stack size in words, not bytes. */
				NULL,    					/* Parameter passed into the task. */
				tskIDLE_PRIORITY+2,			/* Priority at which the task is created. */
				NULL);      				/* Used to pass out the created task's handle. */

	xTaskCreate(task2,       				/* Function that implements the task. */
				"Tache 2",          		/* Text name for the task. */
				configMINIMAL_STACK_SIZE,  	/* Stack size in words, not bytes. */
				NULL,    					/* Parameter passed into the task. */
				tskIDLE_PRIORITY+1,			/* Priority at which the task is created. */
				NULL);      				/* Used to pass out the created task's handle. */

	xTaskCreate(task3,       				/* Function that implements the task. */
				"Tache 3",          		/* Text name for the task. */
				configMINIMAL_STACK_SIZE,  	/* Stack size in words, not bytes. */
				NULL,    					/* Parameter passed into the task. */
				tskIDLE_PRIORITY+1,			/* Priority at which the task is created. */
				NULL);      				/* Used to pass out the created task's handle. */
}


static void growth(void) {
	vTaskDelay(1);
	growth();
}


static void task1(void *pvParameters) {
	// Task infinite loop
	while(1) {
		ENSI_UART_PutChar('1');
		growth();
	}
}


static void task2(void *pvParameters) {
	// Task infinite loop
	while(1) {
		ENSI_UART_PutChar('2');
	}
}


static void task3(void *pvParameters) {
	// Task infinite loop
	while(1) {
		ENSI_UART_PutChar('3');
	}
}


/*
* FreeRTOS callback function for stack overflow detection
*/
void vApplicationStackOverflowHook( TaskHandle_t xTask, signed char *pcTaskName )
{
    ENSI_UART_PutString((uint8_t*)"\r\n*** Application error - ");
    ENSI_UART_PutString((uint8_t*)pcTaskName);
    ENSI_UART_PutString((uint8_t*)" stackoverflow ***");

    /* it's a trap. Comment while(1) to force application software reset */
    while(1);
    NVIC_SystemReset();
}


/*
* FreeRTOS callback function for heap overflow detection
*/
void vApplicationMallocFailedHook( void )
{
	ENSI_UART_PutString((uint8_t*)"\r\n*** Application error - heap overflow ***");

    /* it's a trap. Comment while(1) to force application software reset */
    while(1);
    NVIC_SystemReset();
}

/* USER CODE END */

