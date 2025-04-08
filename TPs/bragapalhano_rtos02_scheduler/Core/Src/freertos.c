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
    /* TODO: CREATE YOUR TASKS HERE */


}


static void task1(void *pvParameters) {
	// Task initialization
    uint8_t* str = (uint8_t*)"\n\r11111111";

	// Task infinite loop
	while(1) {
		HAL_GPIO_WritePin(Task1_GPIO_Port, Task1_Pin, GPIO_PIN_SET);
		ENSI_UART_PutString(str);
		for(uint32_t i = 0 ; i <  160000 ; i++); // Waits for approx 1.0s (L073RZ)
//		for(uint32_t i = 0 ; i < 8000000 ; i++); // Waits for approx 1.0s (L476RG, F411RE)
		HAL_GPIO_WritePin(Task1_GPIO_Port, Task1_Pin, GPIO_PIN_RESET);
	}
}


static void task2(void *pvParameters) {
	// Task initialization
	uint8_t* str = (uint8_t*)"\n\r22222222";

	// Task infinite loop
	while(1) {
		HAL_GPIO_WritePin(Task2_GPIO_Port, Task2_Pin, GPIO_PIN_SET);
		ENSI_UART_PutString(str);
		for(uint32_t i = 0 ; i <  160000 ; i++); // Waits for approx 1.0s (L073RZ)
//		for(uint32_t i = 0 ; i < 8000000 ; i++); // Waits for approx 1.0s (L476RG, F411RE)
		HAL_GPIO_WritePin(Task2_GPIO_Port, Task2_Pin, GPIO_PIN_RESET);
	}
}


static void task3(void *pvParameters) {
	// Task initialization
	uint8_t* str = (uint8_t*)"\n\r33333333";

	// Task infinite loop
	while(1) {
		HAL_GPIO_WritePin(Task3_GPIO_Port, Task3_Pin, GPIO_PIN_SET);
		ENSI_UART_PutString(str);
		for(uint32_t i = 0 ; i <  160000 ; i++); // Waits for approx 1.0s (L073RZ)
//		for(uint32_t i = 0 ; i < 8000000 ; i++); // Waits for approx 1.0s (L476RG, F411RE)
		HAL_GPIO_WritePin(Task3_GPIO_Port, Task3_Pin, GPIO_PIN_RESET);
	}
}

/* TODO: Idle task callback function */


/* USER CODE END */

