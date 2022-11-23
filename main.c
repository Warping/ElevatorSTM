/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2021 STMicroelectronics.
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
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

typedef enum
{
	Floor5_State, //Stopped on Floor 5
	Floor4_State, //Stopped on Floor 4
	Floor3_State, //Stopped on Floor 3
	Floor2_State, //Stopped on Floor 2
	Floor1_State, //Stopped on Floor 1
	TransitionUp_State, //Sets Elevator to go UP
	TransitionDown_State, //Sets Elevator to go DOWN
	Waiting_State //Elevator Waits for input
} eSystemState;

int currentFloor = 1; //Current Floor Tracker
int nextFloor = 1; //Next Floor to Travel to Tracker

eSystemState Floor5(void)
{
	HAL_GPIO_WritePin(GPIOE, LED5_Pin, GPIO_PIN_SET); //Sets LED for Floor
	currentFloor = 5;
	return Waiting_State;
}

eSystemState Floor4(void)
{
	HAL_GPIO_WritePin(GPIOE, LED4_Pin, GPIO_PIN_SET); //Sets LED for Floor
	currentFloor = 4;
	return Waiting_State;
}

eSystemState Floor3(void)
{
	HAL_GPIO_WritePin(GPIOE, LED3_Pin, GPIO_PIN_SET); //Sets LED for Floor
	currentFloor = 3;
	return Waiting_State;
}

eSystemState Floor2(void)
{
	HAL_GPIO_WritePin(GPIOE, LED2_Pin, GPIO_PIN_SET); //Sets LED for Floor
	currentFloor = 2;
	return Waiting_State;
}

eSystemState Floor1(void)
{
	HAL_GPIO_WritePin(GPIOE, LED1_Pin, GPIO_PIN_SET); //Sets LED for Floor
	currentFloor = 1;
	return Waiting_State;
}

eSystemState Waiting(void) //Waiting State Compares currentFloor to Pressed Floor Button and Moves UP or DOWN Accordingly
{
	HAL_GPIO_WritePin(GPIOB, MOTOR_DOWN_Pin, GPIO_PIN_RESET); //Sets Motor to idle
	HAL_GPIO_WritePin(GPIOB, MOTOR_UP_Pin, GPIO_PIN_RESET); //Sets Motor to idle
	HAL_GPIO_WritePin(GPIOE, LED6_Pin, GPIO_PIN_SET); //Sets Input Indicator Light (Waits for input)
	if (HAL_GPIO_ReadPin(GPIOC, BT1_Pin)) {
		if (currentFloor > 1) {
			nextFloor = 1;
			return TransitionDown_State;
		}
	} else if (HAL_GPIO_ReadPin(GPIOC, BT2_Pin)) {
		if (currentFloor > 2) {
			nextFloor = 2;
			return TransitionDown_State;
		} else if (currentFloor < 2) {
			nextFloor = 2;
			return TransitionUp_State;
		}
	} else if (HAL_GPIO_ReadPin(GPIOC, BT3_Pin)) {
		if (currentFloor > 3) {
			nextFloor = 3;
			return TransitionDown_State;
		} else if (currentFloor < 3) {
			nextFloor = 3;
			return TransitionUp_State;
		}
	} else if (HAL_GPIO_ReadPin(GPIOC, BT4_Pin)) {
		if (currentFloor > 4) {
			nextFloor = 4;
			return TransitionDown_State;
		} else if (currentFloor < 4) {
			nextFloor = 4;
			return TransitionUp_State;
		}
	} else if (HAL_GPIO_ReadPin(GPIOC, BT5_Pin)) {
		if (currentFloor < 5) {
			nextFloor = 5;
			return TransitionUp_State;
		}
	}
	HAL_Delay(100);
	return Waiting_State;
}

eSystemState TransitionUp() //Moves Elevator Up by spinning motor in proper direction and Flashing LED of currentFloor of Elevator as it moves
{
	HAL_GPIO_WritePin(GPIOE, LED6_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, MOTOR_DOWN_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, MOTOR_UP_Pin, GPIO_PIN_SET);
	HAL_Delay(100);
	switch(currentFloor)
	{
		case 1:
			for(int i = 0; i < 3; i++) {
				HAL_GPIO_WritePin(GPIOE, LED1_Pin, GPIO_PIN_SET);
				HAL_Delay(500);
				HAL_GPIO_WritePin(GPIOE, LED1_Pin, GPIO_PIN_RESET);
				HAL_Delay(500);
			}
			currentFloor++;
			break;
		case 2:
			for(int i = 0; i < 3; i++) {
				HAL_GPIO_WritePin(GPIOE, LED2_Pin, GPIO_PIN_SET);
				HAL_Delay(500);
				HAL_GPIO_WritePin(GPIOE, LED2_Pin, GPIO_PIN_RESET);
				HAL_Delay(500);
			}
			currentFloor++;
			break;
		case 3:
			for(int i = 0; i < 3; i++) {
				HAL_GPIO_WritePin(GPIOE, LED3_Pin, GPIO_PIN_SET);
				HAL_Delay(500);
				HAL_GPIO_WritePin(GPIOE, LED3_Pin, GPIO_PIN_RESET);
				HAL_Delay(500);
			}
			currentFloor++;
			break;
		case 4:
			for(int i = 0; i < 3; i++) {
				HAL_GPIO_WritePin(GPIOE, LED4_Pin, GPIO_PIN_SET);
				HAL_Delay(500);
				HAL_GPIO_WritePin(GPIOE, LED4_Pin, GPIO_PIN_RESET);
				HAL_Delay(500);
			}
			currentFloor++;
			break;
		default:
			currentFloor = 1;
			nextFloor = 1;
			return Floor1_State;
			break;
	}
	if (currentFloor == nextFloor)
	{
		switch(currentFloor)
		{
			case 2:
				return Floor2_State;
				break;
			case 3:
				return Floor3_State;
				break;
			case 4:
				return Floor4_State;
				break;
			case 5:
				return Floor5_State;
				break;
			default:
				return Floor1_State;
				break;
		}
	} else {
		return TransitionUp_State;
	}
}

eSystemState TransitionDown() //Moves Elevator Down by spinning motor in proper direction and Flashing LED of currentFloor of Elevator as it moves
{
	HAL_GPIO_WritePin(GPIOE, LED6_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, MOTOR_DOWN_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOB, MOTOR_UP_Pin, GPIO_PIN_RESET);
	HAL_Delay(100);
	switch(currentFloor)
	{
		case 2:
			for(int i = 0; i < 3; i++) {
				HAL_GPIO_WritePin(GPIOE, LED2_Pin, GPIO_PIN_SET);
				HAL_Delay(500);
				HAL_GPIO_WritePin(GPIOE, LED2_Pin, GPIO_PIN_RESET);
				HAL_Delay(500);
			}
			currentFloor--;
			break;
		case 3:
			for(int i = 0; i < 3; i++) {
				HAL_GPIO_WritePin(GPIOE, LED3_Pin, GPIO_PIN_SET);
				HAL_Delay(500);
				HAL_GPIO_WritePin(GPIOE, LED3_Pin, GPIO_PIN_RESET);
				HAL_Delay(500);
			}
			currentFloor--;
			break;
		case 4:
			for(int i = 0; i < 3; i++) {
				HAL_GPIO_WritePin(GPIOE, LED4_Pin, GPIO_PIN_SET);
				HAL_Delay(500);
				HAL_GPIO_WritePin(GPIOE, LED4_Pin, GPIO_PIN_RESET);
				HAL_Delay(500);
			}
			currentFloor--;
			break;
		case 5:
			for(int i = 0; i < 3; i++) {
				HAL_GPIO_WritePin(GPIOE, LED5_Pin, GPIO_PIN_SET);
				HAL_Delay(500);
				HAL_GPIO_WritePin(GPIOE, LED5_Pin, GPIO_PIN_RESET);
				HAL_Delay(500);
			}
			currentFloor--;
			break;
		default:
			currentFloor = 1;
			nextFloor = 1;
			return Floor1_State;
			break;
	}
	if (currentFloor == nextFloor)
	{
		switch(currentFloor)
		{
			case 1:
				return Floor1_State;
				break;
			case 2:
				return Floor2_State;
				break;
			case 3:
				return Floor3_State;
				break;
			case 4:
				return Floor4_State;
				break;
			default:
				return Floor1_State;
				break;
		}
	} else {
		return TransitionDown_State;
	}
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  /* USER CODE BEGIN 2 */

  eSystemState eNextState = Floor1_State; //Sets Elevator to 1st Floor

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  switch(eNextState)
	  {
	  	  case Floor1_State:
	  		  eNextState = Floor1();
	  		  break;
	  	  case Floor2_State:
	  		  eNextState = Floor2();
	  		  break;
	  	  case Floor3_State:
	  		  eNextState = Floor3();
	  		  break;
	  	  case Floor4_State:
	  		  eNextState = Floor4();
	  		  break;
	  	  case Floor5_State:
	  		  eNextState = Floor5();
	  		  break;
	  	  case Waiting_State:
	  		  eNextState = Waiting();
	  		  break;
	  	  case TransitionUp_State:
	  		  eNextState = TransitionUp();
	  		  break;
	  	  case TransitionDown_State:
	  		  eNextState = TransitionDown();
	  		  break;
	  	  default:
	  		  eNextState = Floor1();
	  }
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, LD1_Pin|LD3_Pin|LD2_Pin|MOTOR_DOWN_Pin
                          |MOTOR_UP_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, LED1_Pin|LED2_Pin|LED3_Pin|LED4_Pin
                          |LED5_Pin|LED6_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(USB_PowerSwitchOn_GPIO_Port, USB_PowerSwitchOn_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : USER_Btn_Pin */
  GPIO_InitStruct.Pin = USER_Btn_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(USER_Btn_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : RMII_MDC_Pin RMII_RXD0_Pin RMII_RXD1_Pin */
  GPIO_InitStruct.Pin = RMII_MDC_Pin|RMII_RXD0_Pin|RMII_RXD1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF11_ETH;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : RMII_REF_CLK_Pin RMII_MDIO_Pin RMII_CRS_DV_Pin */
  GPIO_InitStruct.Pin = RMII_REF_CLK_Pin|RMII_MDIO_Pin|RMII_CRS_DV_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF11_ETH;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : LD1_Pin LD3_Pin LD2_Pin MOTOR_DOWN_Pin
                           MOTOR_UP_Pin */
  GPIO_InitStruct.Pin = LD1_Pin|LD3_Pin|LD2_Pin|MOTOR_DOWN_Pin
                          |MOTOR_UP_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : LED1_Pin LED2_Pin LED3_Pin LED4_Pin
                           LED5_Pin LED6_Pin */
  GPIO_InitStruct.Pin = LED1_Pin|LED2_Pin|LED3_Pin|LED4_Pin
                          |LED5_Pin|LED6_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pin : RMII_TXD1_Pin */
  GPIO_InitStruct.Pin = RMII_TXD1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF11_ETH;
  HAL_GPIO_Init(RMII_TXD1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : STLK_RX_Pin STLK_TX_Pin */
  GPIO_InitStruct.Pin = STLK_RX_Pin|STLK_TX_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF7_USART3;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pin : USB_PowerSwitchOn_Pin */
  GPIO_InitStruct.Pin = USB_PowerSwitchOn_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(USB_PowerSwitchOn_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : USB_OverCurrent_Pin */
  GPIO_InitStruct.Pin = USB_OverCurrent_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(USB_OverCurrent_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : BT1_Pin BT2_Pin BT3_Pin BT4_Pin
                           BT5_Pin */
  GPIO_InitStruct.Pin = BT1_Pin|BT2_Pin|BT3_Pin|BT4_Pin
                          |BT5_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : USB_SOF_Pin USB_ID_Pin USB_DM_Pin USB_DP_Pin */
  GPIO_InitStruct.Pin = USB_SOF_Pin|USB_ID_Pin|USB_DM_Pin|USB_DP_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF10_OTG_FS;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : USB_VBUS_Pin */
  GPIO_InitStruct.Pin = USB_VBUS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(USB_VBUS_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : RMII_TX_EN_Pin RMII_TXD0_Pin */
  GPIO_InitStruct.Pin = RMII_TX_EN_Pin|RMII_TXD0_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF11_ETH;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

