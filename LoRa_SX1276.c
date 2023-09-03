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
#include <string.h>
#include <stdio.h>

/* USER CODE END PI */

/* External variables --------------------------------------------------------*/
/* USER CODE BEGIN EV */

extern UART_HandleTypeDef huart3; /* Variável externa de configuração do UART */

/* USER CODE END EV */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

#define LORA_HANDLER_UART &huart3 /* Ponteiro para o handle de configuração UART */

/* USER CODE END PD */

/* External functions ------------------------------------------------------------*/
/* USER CODE BEGIN EF */
/* USER CODE END EF */

/* Private variables --------------------------------------------------------*/
/* USER CODE BEGIN PV */
char AT_command[50];
char hexade[50];
char LORA_UART_BUFFER[100];
LoRa_StatusTypeDef LORA_STATUS_RECEIVE = LORA_FAILED;
LoRa_Id Identifier = 0;
/* USER CODE END PV */

/* Private functions ------------------------------------------------------------*/
/* USER CODE BEGIN PF */
void LORA_Receive(uint8_t buffer[50]) {
	int posição = 0;
	for (int i = 0; i < 50; i++) {
		if (!memcmp(buffer + i, "<OK>", 4)) {
			posição = i + 4;
			break;
		}
	}
	if (posição != 0) {
		for (int i = 0; i < 100; i++) {
			if (i <= posição + 1)
				LORA_UART_BUFFER[i] = buffer[i];
			else
				LORA_UART_BUFFER[i] = '\000';
		}
		LORA_STATUS_RECEIVE = LORA_OK;
	}
	LORA_STATUS_RECEIVE = LORA_FAILED;
}

void LORA_WaitReceive(uint16_t _TimerWait) {
	uint32_t Timer_start = HAL_GetTick();
	while (LORA_STATUS_RECEIVE != LORA_OK)
		if (HAL_GetTick() - Timer_start > _TimerWait)
			break;
}

void LORA_TransmitCommand(char *_Command, uint8_t _Size) {
	HAL_UART_Transmit(LORA_HANDLER_UART, (uint8_t*) _Command, _Size, 100);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*---- IDENTIDICADOR DO DISPOSITIVO FINAL --------------------------------------------------------------*/

/**
 * @brief Identificador de dispositivo final no espaço de endereço IEEE EUI64
 * @param _Operacao: Modo de operação do comando
 * @param _Identifier: Identificador de dispositivo final
 * @retval Status de execução do comando
 */

LoRa_StatusTypeDef AT_EndDeviceIdentifier(LoRa_OperationTypeDef _Operacao,
		LoRa_Id *_Identifier) {
	switch (_Operacao) {

	case AT_OPERATION_READ:
		sprintf(AT_command, "AT+DEVEUI\r\n");
		LORA_TransmitCommand(AT_command, strlen(AT_command));
		LORA_WaitReceive(100);
		sscanf(LORA_UART_BUFFER, "%s\r%8lx%8lx\r", AT_command,
				&(((uint32_t*) _Identifier)[1]), &(((uint32_t*) _Identifier)[0]));
		break;
	case AT_OPERATION_WRITE:
		sprintf(AT_command, "AT+DEVEUI %04X%04X%04X%04X\r\n",
				((uint16_t*) _Identifier)[3], ((uint16_t*) _Identifier)[2],
				((uint16_t*) _Identifier)[1], ((uint16_t*) _Identifier)[0]);
		LORA_TransmitCommand(AT_command, strlen(AT_command));
		break;
	default:
		break;
	}
	return LORA_OK;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*---- ID DE APLICATIVO GLOBAL -----------------------------------------------------------*/

/**
 * @brief ID de aplicativo global no espaço de endereço IEEE EUI64
 * @param _Operacao: Modo de operação do comando
 * @param _Identifier: ID de aplicativo global
 * @retval Status de execução do comando
 */

LoRa_StatusTypeDef AT_AppEUIAdress(LoRa_OperationTypeDef _Operacao,
		LoRa_Id *_Identifier);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*---- CHAVE DO DISPOSITIVO FINAL -----------------------------------------------------------*/

/**
 * @brief Chave de raiz AES-128 específica para o dispositivo final
 * @param _Operacao: Modo de operação do comando
 * @param _Keyword: Chave do dispositivo final
 * @retval Status de execução do comando
 */

LoRa_StatusTypeDef AT_ApplicationKey(LoRa_OperationTypeDef _Operacao,
		LoRa_KeyTypeDef *_Keyword);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*---- STATUS DA REDE PÚBLICA -----------------------------------------------------------*/

/**
 * @brief Status do modo de rede pública
 * @param _Operacao: Modo de operação do comando
 * @param _Status: Status da rede pública
 * @retval Status de execução do comando
 */

LoRa_StatusTypeDef AT_PublicNetworkModeStatus(LoRa_OperationTypeDef _Operacao,
		LoRa_PublicNetworkTypeDef *_Status);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*---- CONFIGURAÇÃO DO MODO DE INGRESSO NA REDE -----------------------------------------------------------*/

/**
 * @brief Comando para configuração do modo de ingresso na rede, reinicie após a configuração ser atualizada
 * @param _Operacao: Modo de operação do comando
 * @param _Mode: Modo de ingresso na rede
 * @retval Status de execução do comando
 */

LoRa_StatusTypeDef AT_NetworkJoinMode(LoRa_OperationTypeDef _Operacao,
		LoRa_NetworkJoinTypeDef *_Mode);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*---- DEFINIÇÃO DE LORA MAC -----------------------------------------------------------*/

/**
 * @brief Comando para definir a classe LoRa MAC
 * @param _Operacao: Modo de operação do comando
 * @param _Class: Classe de MAC
 * @retval Status de execução do comando
 */

LoRa_StatusTypeDef AT_LoRaMacClass(LoRa_OperationTypeDef _Operacao,
		LoRa_MacClassTypeDef *_Class);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*---- INGRESSO NA REDE LORA -----------------------------------------------------------*/

/**
 * @brief Comando para ingressar no servidor de rede LoRa
 * @param ***NONE***
 * @retval Status de execução do comando
 */

LoRa_StatusTypeDef AT_JoinRequestNetworkServer(void);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*---- STATUS DE INGRESSO NA REDE LORA -----------------------------------------------------------*/

/**
 * @brief Comando para verificar o status de ingresso na rede LoRa
 * @param _Status: Status de ingresso
 * @retval Status de execução do comando
 */

LoRa_StatusTypeDef AT_JoinNetworkServerStatus(LoRa_NetworkJoinTypeDef *_Status);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*---- SOLICITAÇÃO DE INGRESSO NA REDE AUTOMÁTICO -----------------------------------------------------------*/

/**
 * @brief Comando para definir a solicitação de ingresso na rede automática quando o dispositivo é inicializado
 * @param _Operacao: Modo de operação do comando
 * @param _Status: Status de solicitação de ingresso automático
 * @retval Status de execução do comando
 */

LoRa_StatusTypeDef AT_AutoJoinNetworkServer(LoRa_OperationTypeDef _Operacao,
		LoRa_AutoNetworkJoinTypeDef *_Status);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*---- CHAVE DA SESSÃO DE REDE -----------------------------------------------------------*/

/**
 * @brief Chave da sessão de rede
 * @param _Operacao: Modo de operação do comando
 * @param _Keyword: Chave da sessão
 * @retval Status de execução do comando
 */

LoRa_StatusTypeDef AT_NetworkSessionKey(LoRa_OperationTypeDef _Operacao,
		LoRa_KeyTypeDef *_Keyword);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*---- CHAVE DA SESSÃO DO APLICATIVO -----------------------------------------------------------*/

/**
 * @brief Chave de sessão do aplicativo
 * @param _Operacao: Modo de operação do comando
 * @param _Keyword: Chave da sessão
 * @retval Status de execução do comando
 */

LoRa_StatusTypeDef AT_ApplicationSessionKey(LoRa_OperationTypeDef _Operacao,
		LoRa_KeyTypeDef *_Keyword);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*---- ENDEREÇO DO DISPOSITIVO -----------------------------------------------------------*/

/**
 * @brief Endereço do dispositivo
 * @param _Operacao: Modo de operação do comando
 * @param  _Adress: Endereço
 * @retval Status de execução do comando
 */

LoRa_StatusTypeDef AT_DeviceAddress(LoRa_OperationTypeDef _Operacao,
		LoRa_Adress *_Adress);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*---- IDENTIFICADOR DA REDE -----------------------------------------------------------*/

/**
 * @brief Valor de exibição do identificador da rede
 * @param _Operacao: Modo de operação do comando
 * @param _Identifier: Identificador da rede
 * @retval Status de execução do comando
 */

LoRa_StatusTypeDef AT_NetworkIdentifier(LoRa_OperationTypeDef _Operacao,
		LoRa_Id *_Identifier);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*---- CONFIGURAÇOES ATIVAS -----------------------------------------------------------*/

/**
 * @brief Retorna as configurações ativas
 * @param _hSettings: Configurações ativas
 * @retval Status de execução do comando
 */

LoRa_StatusTypeDef AT_ActivationSettingValue(
		LoRa_ActivationSettingTypeDef _hSettings);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*---- ENVIO DE DADOS (STRING) -----------------------------------------------------------*/

/**
 * @brief Comando LoRa para envio de dados do tipo string
 * @param _Data: Dados para envio
 * @retval Status de execução do comando
 */

LoRa_StatusTypeDef AT_DataUplinkText(LoRa_Data _Data[5]);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*---- ENVIO DE DADOS (HEXADECIMAL) -----------------------------------------------------------*/

/**
 * @brief Comando LoRa para envio de dados do tipo hexacimal
 * @param _Data: Dados para envio
 * @retval Status de execução do comando
 */

LoRa_StatusTypeDef AT_DataUplinkHexadecimal(LoRa_Data _Data[5]);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*---- RECEBIMENTO DE DADOS (STRING) -----------------------------------------------------------*/

/**
 * @brief Comando para leitura de dados de recebidos
 * @param _Data: Dados recebidos
 * @retval Status de execução do comando
 */

LoRa_StatusTypeDef AT_ConfirmDownlinkDataText(LoRa_Data _Data[5]);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*---- RECEBIMENTO DE DADOS (HEXADECIMAL) -----------------------------------------------------------*/

/**
 * @brief Comando para leitura de dados de recebidos
 * @param _Data: Dados recebidos
 * @retval Status de execução do comando
 */

LoRa_StatusTypeDef AT_ConfirmDownlinkDataHexadecimal(LoRa_Data _Data[5]);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*---- VALOR DO RSSI DE LEITURA -----------------------------------------------------------*/

/**
 * @brief Valor RSSI de leitura dos últimos dados recebidos
 * @param  _Value: Valor do RSSI
 * @retval Status de execução do comando
 */

LoRa_StatusTypeDef AT_ReturnRSSI(LoRa_RSSI *_Value);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*---- VALOR DO SNR DE LEITURA -----------------------------------------------------------*/

/**
 * @brief Leitura do valor SNR (relação sinal-ruído) dos últimos dados recebidos
 * @param _Value: Valor do SNR
 * @retval Status de execução do comando
 */

LoRa_StatusTypeDef AT_ReturnsSNR(LoRa_Value *_Value);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*---- CONFIGURAÇÃO DA REGIÃO LORAMAC -----------------------------------------------------------*/

/**
 * @brief Retorna a configuração da região LoRaMAC. Reinicie após a atualização da configuração
 * @param _Operacao: Modo de operação do comando
 * @param _Region: Região de LoRaMAC
 * @retval Status de execução do comando
 */

LoRa_StatusTypeDef AT_LoRaMacRegion(LoRa_OperationTypeDef _Operacao,
		LoRa_LoraMacRegionTypeDef *_Region);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*---- TAXA DE DADOS AUTOMÁTICO -----------------------------------------------------------*/

/**
 * @brief Configuração de Auto Data Rate (ADR)
 * @param _Operacao: Modo de operação do comando
 * @param _Status: Status do ADR
 * @retval Status de execução do comando
 */

LoRa_StatusTypeDef AT_AutoDateRate(LoRa_OperationTypeDef _Operacao,
		LoRa_AutoDataRateTypeDef *_Status);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*---- CONFIGURAÇÃO DE TAXA DE DADOS -----------------------------------------------------------*/

/**
 * @brief Status da configuração da taxa de dados. O fator de espalhamento (SF) pode variar de acordo com a região
 * @param _Operacao: Modo de operação do comando
 * @param _DateRate: Taxa de dados
 * @retval Status de execução do comando
 */

LoRa_StatusTypeDef AT_DataRateCommand(LoRa_OperationTypeDef _Operacao,
		LoRa_DataRateTypeDef *_DateRate);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*---- FREQUÊNCIA RX DA JANELA 2 -----------------------------------------------------------*/

/**
 * @brief Comando da frequência Rx da janela 2
 * @param _Operacao: Modo de operação do comando
 * @param _Rate: Frequência
 * @retval Status de execução do comando
 */

LoRa_StatusTypeDef AT_RxWindow2Frequency(LoRa_OperationTypeDef _Operacao,
		LoRa_Rate *_Rate);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*---- TAXA DE DADOS RX DA JANELA 2 -----------------------------------------------------------*/

/**
 * @brief Comando da taxa de dados da janela Rx 2 (0-7 correspondente a DR_X)
 * @param _Operacao: Modo de operação do comando
 * @param _Value: Taxa de dados
 * @retval Status de execução do comando
 */

LoRa_StatusTypeDef AT_RxWindow2DataRate(LoRa_OperationTypeDef _Operacao,
		LoRa_Value *_Value);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*---- DELAY ENTRE JANELAS TX E RX1 -----------------------------------------------------------*/

/**
 * @brief Define o atraso entre o final da janela TX e a janela Rx 1 em ms
 * @param _Operacao: Modo de operação do comando
 * @param _Value: Delay em ms
 * @retval Status de execução do comando
 */

LoRa_StatusTypeDef AT_TxRxWindow1Delay(LoRa_OperationTypeDef _Operacao,
		LoRa_Value *_Value);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*---- DELAY ENTRE JANELAS TX E RX2 -----------------------------------------------------------*/

/**
 * @brief Define o atraso entre o final do TX e a janela Rx 2 em ms
 * @param _Operacao: Modo de operação do comando
 * @param _Value: Delay em ms
 * @retval Status de execução do comando
 */

LoRa_StatusTypeDef AT_TxRxWindow2Delay(LoRa_OperationTypeDef _Operacao,
		LoRa_Value *_Value);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*---- DELAY DE JUNÇÃO DA JANELA RX1 -----------------------------------------------------------*/

/**
 * @brief Comando para acessar o atraso de junção na janela RX 1 em ms
 * @param _Operacao: Modo de operação do comando
 * @param _Value: Delay em ms
 * @retval Status de execução do comando
 */

LoRa_StatusTypeDef AT_TxRxWindow1JoinDelay(LoRa_OperationTypeDef _Operacao,
		LoRa_Value *_Value);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*---- DELAY DE ACEITAÇÃO DE JUNÇÃO DAS JANELAS TX E RX2 ------------------------------------------------*/

/**
 * @brief Defina o atraso de aceitação de junção entre o final do TX e a junção da janela Rx 2 em ms
 * @param _Operacao: Modo de operação do comando
 * @param _Value: Delay em ms
 * @retval Status de execução do comando
 */

LoRa_StatusTypeDef AT_TxRxWindow2JoinDelay(LoRa_OperationTypeDef _Operacao,
		LoRa_Value *_Value);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*---- REPETIÇÃO DE UPLINKS NÃO CONFIRMADOS -----------------------------------------------------------*/

/**
 * @brief Comando para repetir o uplink não confirmado sem aguardar o reconhecimento do servidor (1 - 15)
 * @param _Operacao: Modo de operação do comando
 * @param _Value: Número de repetiçoes
 * @retval Status de execução do comando
 */

LoRa_StatusTypeDef AT_RepeatUnconfirmedUplink(LoRa_OperationTypeDef _Operacao,
		LoRa_Value *_Value);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*----  -----------------------------------------------------------*/

/**
 * @brief Comando para reenvio de uplink confirmado. O envio se repete até que uma confirmação servidor chegue (1 - 8)
 * @param _Operacao: Modo de operação do comando
 * @param _Value: Número de repetiçoes
 * @retval Status de execução do comando
 */

LoRa_StatusTypeDef AT_ResendConfirmedUplink(LoRa_OperationTypeDef _Operacao,
		LoRa_Value *_Value);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*---- ÍNDICE DE POTÊNCIA TX -----------------------------------------------------------*/

/**
 * @brief Comando de Índice de Potência Tx
 * @param _Operacao: Modo de operação do comando
 * @param _Value: Índice de Potência
 * @retval Status de execução do comando
 */

LoRa_StatusTypeDef AT_TxPowerIndex(LoRa_OperationTypeDef _Operacao,
		LoRa_Value *_Value);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*---- CONTADOR DE UPLINKS -----------------------------------------------------------*/

/**
 * @brief Comando para acesso ao contador de uplinks (0 - 65535)
 * @param _Operacao: Modo de operação do comando
 * @param _Value: Número de uplinks
 * @retval Status de execução do comando
 */

LoRa_StatusTypeDef AT_UplinkCounter(LoRa_OperationTypeDef _Operacao,
		LoRa_Value *_Value);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*---- CONTADOR DE DOWNLINKS -----------------------------------------------------------*/

/**
 * @brief Comando para acesso ao contador de downlinks (0 - 65535)
 * @param _Operacao: Modo de operação do comando
 * @param _Value: Número de downlinks
 * @retval Status de execução do comando
 */

LoRa_StatusTypeDef AT_DownlinkCounter(LoRa_OperationTypeDef _Operacao,
		LoRa_Value *_Value);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*---- NÍVEL DE BATERIA DO DISPOSITIVO FINAL -----------------------------------------------------------*/

/**
 * @brief Este comando permite ao usuário acessar o nível da bateria do dispositivo final
 * @param _Operacao: Modo de operação do comando
 * @param _Level: Nível de bateria
 * @retval Status de execução do comando
 */

LoRa_StatusTypeDef AT_BatteryLevel(LoRa_OperationTypeDef _Operacao,
		LoRa_BateryLevelTypeDef *_Level);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*---- VERIFICAÇÃO DE LINK -----------------------------------------------------------*/

/**
 * @brief Comando usado para verificar se o link está funcionando corretamente
 * @retval Status de execução do comando
 */

LoRa_StatusTypeDef AT_MacLineCheckRequest(void);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*---- CRIPTOGRAFIA DE LEITURA -----------------------------------------------------------*/

/**
 * @brief Configuração de criptografia de leitura
 * @param _Operacao: Modo de operação do comando
 * @param _Encryption: Modo de encriptação
 * @retval Status de execução do comando
 */

LoRa_StatusTypeDef AT_EncryptionConfiguration(LoRa_OperationTypeDef _Operacao,
		LoRa_ReadoutEncryptionTypeDef *_Encryption);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*---- LEITURA E ATUALIZAÇÃO DE CONFIGURAÇÃO DE CANAL -----------------------------------------------------------*/

/**
 * @brief Comando para ler as configurações atuais do canal, e atualizar-las
 * @param _Operacao: Modo de operação do comando
 * @param _ChOperation: Configuração a ser lida ou modificada
 * @param _hConfiguration: Handler de configurações do canal
 * @retval Status de execução do comando
 */

LoRa_StatusTypeDef AT_ChannelConfiguration(LoRa_OperationTypeDef _Operacao,
		LoRa_ChannelOperationTypeDef *_ChOperation,
		LoRa_ChannelConfigurationTypeDef *_hConfiguration);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*---- REINICIALIZAÇÃO DO SISTEMA -----------------------------------------------------------*/

/**
 * @brief Comando de reinicialização do canal ou do sistema
 * @param _Mode: Modo de reinicialização
 * @param _Channel: Canal de reinicialização
 * @retval Status de execução do comando
 */

LoRa_StatusTypeDef AT_SystemReboot(LoRa_SystemRebootModeTypeDef _Mode,
		LoRa_Value *_Channel);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*---- INFORMAÇÕES DO SISTEMA -----------------------------------------------------------*/

/**
 * @brief Comando para ler informações do sistema
 * @param _hInfo: Handler de informações do sistema
 * @retval Status de execução do comando
 */

LoRa_StatusTypeDef AT_SystemInformation(LoRa_SystemInfoTypeDef *_hInfo);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*---- VERSÃO DO FIRMWARE -----------------------------------------------------------*/

/**
 * @brief Informações sobre a versão do firmware
 * @param _Version: Versão do firmaware
 * @retval Status de execução do comando
 */

LoRa_StatusTypeDef AT_FirmwareVersion(LoRa_Float *_Version);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*---- GANHO DE ANTENA -----------------------------------------------------------*/

/**
 * @brief O comando para acesso ao ganho da antena (-4 e 6)
 * @param _Gain: Ganho de antena
 * @retval Status de execução do comando
 */

LoRa_StatusTypeDef AT_AntennaGain(LoRa_Float *_Gain);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*---- TIPO DE PACOTE DE UPLINK -----------------------------------------------------------*/

/**
 * @brief Comando de leitura e definição de tipo de pacote uplink (0 ou 1)
 * @param _Operacao: Modo de operação do comando
 * @param _Type: Tipo de pacote
 * @retval Status de execução do comando
 */

LoRa_StatusTypeDef AT_UplinkPacketType(LoRa_OperationTypeDef _Operacao,
		LoRa_UplinkTypePacketTypeDef *_Type);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*---- MODO DE BAIXO CONSUMO -----------------------------------------------------------*/

/**
 * @brief Comando para entrar no modo de baixo consumo (usar reset para voltar ao normal)
 * @retval Status de execução do comando
 */

LoRa_StatusTypeDef AT_EntersLowPowerMode(void);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*---- ALARME DO RTC -----------------------------------------------------------*/

/**
 * @brief Hora de despertar do "real time clock" (RTC)
 * @param _Operacao: Modo de operação do comando
 * @param _Time: Tempo para despertar (em segundos)
 * @retval Status de execução do comando
 */

LoRa_StatusTypeDef AT_RTCWakeupTime(LoRa_OperationTypeDef _Operacao,
		LoRa_Value *_Time);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*---- RELÓGIO DO RTC -----------------------------------------------------------*/

/**
 * @brief Comando de acessor ao relógio do "real time clock" (RTC)
 * @param _Operacao: Modo de operação do comando
 * @param _Time: Tempo do RTC
 * @retval Status de execução do comando
 */

LoRa_StatusTypeDef AT_RTCTime(LoRa_OperationTypeDef _Operacao,
		LoRa_TimeTypeDef *_Time);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*---- DATA DO RTC -----------------------------------------------------------*/

/**
 * @brief Comando de acessor a data do "real time clock" (RTC)
 * @param _Operacao: Modo de operação do comando
 * @param _Date: Data do RTC
 * @retval Status de execução do comando
 */

LoRa_StatusTypeDef AT_RTCDate(LoRa_OperationTypeDef _Operacao,
		LoRa_DateTypeDef *_Date);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*---- RETORNO DE ECO -----------------------------------------------------------*/

/**
 * @brief Comando de retorno de eco
 * @param _Operacao: Modo de operação do comando
 * @param _Echo: Retorno do eco
 * @retval Status de execução do comando
 */

LoRa_StatusTypeDef AT_ECHO(LoRa_OperationTypeDef _Operacao,
		LoRa_LoraEchoTypeDef *_Echo);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*---- REDEFINIÇÃO DE CONFIGURAÇÕES -----------------------------------------------------------*/

/**
 * @brief Comando para redefinir as configuração
 * @retval Status de execução do comando
 */

LoRa_StatusTypeDef AT_ResetConfiguration(void);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*---- MODO DE DEBUG -----------------------------------------------------------*/

/**
 * @brief Comando para acesso ao modo de dubug
 * @param _Operacao: Modo de operação do comando
 * @param _Status: Status do modo debug
 * @retval Status de execução do comando
 */

LoRa_StatusTypeDef AT_DebugMessageStatus(LoRa_OperationTypeDef _Operacao,
		LoRa_DebugMessageTypeDef *_Status);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*---- TESTE DE FORÇA TX -----------------------------------------------------------*/

/**
 * @brief Modo de onda contínua FSK Tx (teste de força Tx)
 * @param _Operacao: Modo de operação do comando
 * @param _Frequency: Frequência de operação do teste
 * @param _Power: Potência de transmissão na operação do teste
 * @param _Timeout: Tempo total de teste (em segundos)
 * @retval Status de execução do comando
 */

LoRa_StatusTypeDef AT_FSKTxContinuousWaveMode(LoRa_OperationTypeDef _Operacao,
		LoRa_Rate _Frequency, LoRa_Value _Power, LoRa_Value *_Timeout);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*---- TESTE DE FORÇA DA ANTENA PARA RECEBIMENTO -----------------------------------------------------------*/

/**
 * @brief LoRa Rx (teste de força de RF)
 * @param _Operacao: Modo de operação do comando
 * @param _Frequency: Frequência de operação do teste
 * @param _DataRate: Configuração de taxa de dados do teste (0 - 7)
 * @param _TBaund: Largura de banda do teste (0 a 3)
 * @param _hInfo: Handler de informação sobre o teste
 * @retval Status de execução do comando
 */

LoRa_StatusTypeDef AT_LoRaRxSignalStrengthTest(LoRa_OperationTypeDef _Operacao,
		LoRa_Rate _Frequency, LoRa_Value _DataRate,
		LoRa_StrenghtTestBaundTypeDef _TBaund,
		LoRa_RxStrengthTestTypeDef *_hInfo);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*---- TESTE DE FORÇA DA ANTENA PARA TRANSMISSÃO -----------------------------------------------------------*/

/**
 * @brief LoRa Tx (teste de força de RF)
 * @param _Operacao: Modo de operação do comando
 * @param _Frequency: Frequência de operação do teste
 * @param _Power: Potência de transmissão na operação do teste
 * @param _DataRate: Configuração de taxa de dados do teste (0 - 7)
 * @param _TBaund: Largura de banda do teste (0 a 3)
 * @param _NumberBytes: Número de bytes para transmissão
 * @param _Period: Periodo de transmissão dos bytes
 * @param _Status: Status de final do teste
 * @retval Status de execução do comando
 */

LoRa_StatusTypeDef AT_LoRaTxSignalStrengthTest(LoRa_OperationTypeDef _Operacao,
		LoRa_Rate _Frequency, LoRa_Value _Power, LoRa_Value _DataRate,
		LoRa_StrenghtTestBaundTypeDef _TBaund, LoRa_Value _NumberBytes,
		LoRa_Value _Period, LoRa_StrenghtTestStatusTypeDef *_Status);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*---- FINALIZAÇÃO DE TESTE DE RF -----------------------------------------------------------*/

/**
 * @brief Comando para finalização de teste de RF
 * @retval Status de execução do comando
 */

LoRa_StatusTypeDef AT_StopRFTest(void);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*---- CONFIGURAÇÃO DE PINO GPIO -----------------------------------------------------------*/

/**
 * @brief Comando para acesso a configuração de pinos GPIO MS500
 * @param _Operacao: Modo de operação do comando
 * @param _GPIO: Pino GPIO para acesso
 * @param _Config: Configuração do pino GPIO
 * @retval Status de execução do comando
 */

LoRa_StatusTypeDef AT_GPIOPinInformation(LoRa_OperationTypeDef _Operacao,
		LoRa_PinTypeDef _GPIO, LoRa_PinConfigurationTypeDef *_Config);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*---- CANAL DE COMUNICAÇÃO P2P -----------------------------------------------------------*/

/**
 * @brief Comando para listar canais de comunicação diponíveis, e seleção de canal para comunicação P2P
 * @param _Operacao: Modo de operação do comando
 * @param _Channel: Canal de comunicação P2P
 * @param _ChannelsList: Lista de canais disponíveis
 * @retval Status de execução do comando
 */

LoRa_StatusTypeDef AT_RegionalChannelListP2P(LoRa_OperationTypeDef _Operacao,
		LoRa_Value _Channel, LoRa_ChannelsTypeDef *_ChannelsList);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*---- ENDEREÇO DE COMUNICAÇÃO P2P -----------------------------------------------------------*/

/**
 * @brief  Defina o endereço do dispositivo P2P (4 bytes) para comunicação
 * @param _Operacao: Modo de operação do comando
 * @param _Adress: Endereço de comunicação P2P
 * @retval Status de execução do comando
 */

LoRa_StatusTypeDef AT_DeviceAdressP2P(LoRa_OperationTypeDef _Operacao,
		LoRa_Adress *_Adress);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*---- CHAVE DE SINCRONIZAÇÃO -----------------------------------------------------------*/

/**
 * @brief Comando para definição de chave de sincronização
 * @param _Operacao: Modo de operação do comando
 * @param _Word: Chave de sincronização (1 a 255)
 * @retval Status de execução do comando
 */

LoRa_StatusTypeDef AT_SyncWordP2P(LoRa_OperationTypeDef _Operacao,
		LoRa_Value *_Word);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* USER CODE END PF */
