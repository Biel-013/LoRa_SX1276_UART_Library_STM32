/*
 *      Comunicaçao chip SX1276 LoRa via Uart - LoRa_SX1276.c
 *
 *      Data: 29 de agosto, 2023
 *      Autor: Gabriel Luiz
 *      Contato: (31) 97136-4334 || gabrielluiz.eletro@gmail.com
 */
/*
 * 		- Links Úteis -
 *
 */

/* Private Includes ----------------------------------------------------------*/
/* USER CODE BEGIN PI */

#include "LoRa_SX1276.h"

/* USER CODE END PI */

/* External variables --------------------------------------------------------*/
/* USER CODE BEGIN EV */

extern UART_HandleTypeDef huart3; /* Variável externa de configuração do I2C */

/* USER CODE END EV */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

#define EEPROM_HANDLE_I2C &huart3 /* Ponteiro para o handle de configuração I2C */

/* USER CODE END PD */

/* External functions ------------------------------------------------------------*/
/* USER CODE BEGIN EF */
/* USER CODE END EF */

/* Private variables --------------------------------------------------------*/
/* USER CODE BEGIN PV */
/* USER CODE END PV */

/* Private functions ------------------------------------------------------------*/
/* USER CODE BEGIN PF */

/*-------------------------------------------------------------------------*/

/**
 * @brief
 * @param
 * @param
 * @retval
 */

