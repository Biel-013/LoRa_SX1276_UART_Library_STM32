/*
 *      Comunicaçao chip SX1276 LoRa via Uart - LoRa_SX1276.h
 *
 *      Data: 29 de agosto, 2023
 *      Autor: Gabriel Luiz
 *      Contato: (31) 97136-4334 || gabrielluiz.eletro@gmail.com
 */
/*
 * 		- Links Úteis -
 *
 */

#ifndef INC_LORA_SX1276_H_
#define INC_LORA_SX1276_H_

#include "stm32f1xx_hal.h"

#define AT_ENDTERM "\r\n"


typedef uint64_t LoRa_IdTypeDef;

typedef uint8_t LoRa_Data;

typedef uint16_t LoRa_Value;

typedef uint32_t LoRa_AdressTypeDef;

typedef struct {
	uint64_t *LoRa_HighKey;
	uint64_t *LoRa_LowKey;
} LoRa_KeyTypeDef;

typedef enum {
	LORA_FAILED             = 0x00U,
	LORA_OK                 = 0x01U,
	LORA_FAILED_CHARACTER   = 0x02U,
	LORA_FAILED_COMMAND     = 0x03U,
	LORA_BUSY               = 0x04U,
	LORA_TIMEOUT            = 0x05U
} LoRa_StatusTypeDef;

typedef enum {
	LORA_PUBLIC_NETWORK_OFF   = 0x00U,
	LORA_PUBLIC_NETWORK_ON    = 0x01U
} LoRa_PublicNetworkTypeDef;

typedef enum {
	LORA_NETWORK_JOIN_MODE_ABP     = 0x00U,
	LORA_NETWORK_JOIN_MODE_OTAA    = 0x01U,
	LORA_NETWORK_JOIN_MODE_P2P     = 0x02U
} LoRa_NetworkJoinModeTypeDef;

typedef enum {
	LORA_MAC_CLASS_A     = 'A',
	LORA_MAC_CLASS_B     = 'B',
	LORA_MAC_CLASS_C     = 'C'
} LoRa_MacClassTypeDef;

typedef enum {
	LORA_NETWORK_NOT_JOINED   = 0x00U,
	LORA_NETWORK_JOINED       = 0x01U
} LoRa_NetworkJoinTypeDef;

typedef enum {
	LORA_AUTO_NETWORK_JOIN_ON       = 0x00U,
	LORA_AUTO_NETWORK_JOIN_OFF      = 0x01U
} LoRa_AutoNetworkJoinTypeDef;
typedef enum {
	LORA_AUTO_DATA_RATE_OFF   = 0x00U,
	LORA_AUTO_DATA_RATE_ON    = 0x01U
} LoRa_AutoDataRateTypeDef;

typedef enum {
	LORA_BATERY_LEVEL_USB          = 0x00U,
	LORA_BATERY_LEVEL_MIN_LEVEL    = 0x01U,
	LORA_BATERY_LEVEL_MAX_LEVEL    = 0xFEU,
	LORA_BATERY_LEVEL_ERROR        = 0xFFU
} LoRa_BateryLevelTypeDef;

typedef enum {
	LORA_READOUT_ENCRYPTION_AES     = 0x00U,
	LORA_READOUT_ENCRYPTION_ARIA    = 0x01U
} LoRa_ReadoutEncryptionTypeDef;

typedef enum {
	LORA_UPLINK_PACKET_TYPE_NOT_CONFIRMED   = 0x00U,
	LORA_UPLINK_PACKET_TYPE_CONFIRMED       = 0x01U
} LoRa_UplinkTypePacketTypeDef;

typedef enum {
	LORA_ECHO_OFF   = 0x00U,
	LORA_ECHO_ON    = 0x01U
} LoRa_LoraEchoTypeDef;

typedef enum {
	LORA_DEBUG_MESSAG_OFF   = 0x00U,
	LORA_DEBUG_MESSAG_ON    = 0x01U
} LoRa_DebugMessageTypeDef;

typedef enum {
	LORA_RF_STRENGHT_TEST_BAUND_125KHZ    = 0x00U,
	LORA_RF_STRENGHT_TEST_BAUND_250KHZ    = 0x01U,
	LORA_RF_STRENGHT_TEST_BAUND_500KHZ    = 0x02U
} LoRa_StrenghtTestBaundTypeDef;

typedef enum {
	AT_OPERATION_READ             = 0x00U,
	AT_OPERATION_WRITE            = 0x01U,
	AT_OPERATION_ASYNCHRONOUS     = 0x02U
} LoRa_OperationTypeDef;

typedef enum {
	LORA_CURRENT_DATA_RATE_SF12_BW125    = 0x00U,
	LORA_CURRENT_DATA_RATE_SF11_BW125    = 0x01U,
	LORA_CURRENT_DATA_RATE_SF10_BW125    = 0x02U,
	LORA_CURRENT_DATA_RATE_SF9_BW125     = 0x03U,
	LORA_CURRENT_DATA_RATE_SF8_BW125     = 0x04U,
	LORA_CURRENT_DATA_RATE_SF7_BW125     = 0x05U,
	LORA_CURRENT_DATA_RATE_SF7_BW250     = 0x06U,
	LORA_CURRENT_DATA_RATE_FSK           = 0x07U
} LoRa_DataRateTypeDef;

typedef enum {
	LORAMAC_REGION_AS923      = 0x00U,
	LORAMAC_REGION_AU915      = 0x01U,
	LORAMAC_REGION_CN470      = 0x02U,
	LORAMAC_REGION_CN779      = 0x03U,
	LORAMAC_REGION_EU433      = 0x04U,
	LORAMAC_REGION_EU868      = 0x05U,
	LORAMAC_REGION_KR920      = 0x06U,
	LORAMAC_REGION_IN865      = 0x07U,
	LORAMAC_REGION_US915      = 0x07U
} LoRa_LoraMacRegionTypeDef;

typedef struct {
	LoRa_IdTypeDef LoRa_EndDeviceIdentifier;
	LoRa_IdTypeDef LoRa_AppEUIAdress;
	LoRa_KeyTypeDef LoRa_ApplicationKey;
	LoRa_PublicNetworkTypeDef LoRa_PublicNetwork;
	LoRa_NetworkJoinModeTypeDef LoRa_NetworkJoinMode;
	LoRa_MacClassTypeDef LoRa_MacClass;
	LoRa_NetworkJoinTypeDef LoRa_NetworkJoin;
	LoRa_IdTypeDef *LoRa_NetworkIdentifier;
	LoRa_AdressTypeDef *LoRa_DeviceAddress;
	LoRa_KeyTypeDef LoRa_NetworkSessionKey;
	LoRa_KeyTypeDef LoRa_ApplicationSessionKey;
	LoRa_AutoNetworkJoinTypeDef LoRa_AutoNetworkJoin;
} LoRa_ActivationSettingTypeDef;

/*---------------------------------------------------------------*/

/**
 * @brief
 * @param
 * @param
 * @param
 * @retval
 */

LoRa_StatusTypeDef AT_EndDeviceIdentifier (LoRa_OperationTypeDef _Operacao, LoRa_IdTypeDef *_Identifier);
LoRa_StatusTypeDef AT_AppEUIAdress (LoRa_OperationTypeDef _Operacao, LoRa_IdTypeDef *_Identifier);
LoRa_StatusTypeDef AT_ApplicationKey (LoRa_OperationTypeDef _Operacao, LoRa_KeyTypeDef *_Keyword);
LoRa_StatusTypeDef AT_PublicNetworkModeStatus (LoRa_OperationTypeDef _Operacao, LoRa_PublicNetworkTypeDef *_Status);
LoRa_StatusTypeDef AT_NetworkJoinMode (LoRa_OperationTypeDef _Operacao, LoRa_NetworkJoinTypeDef *_Mode);
LoRa_StatusTypeDef AT_LoRaMacClass (LoRa_OperationTypeDef _Operacao, LoRa_MacClassTypeDef *_Class);
LoRa_StatusTypeDef AT_JoinRequestNetworkServer (void);
LoRa_StatusTypeDef AT_JoinNetworkServerStatus (LoRa_NetworkJoinTypeDef *_Status);
LoRa_StatusTypeDef AT_AutoJoinNetworkServer (LoRa_OperationTypeDef _Operacao, LoRa_AutoNetworkJoinTypeDef *_Status);
LoRa_StatusTypeDef AT_NetworkSessionKey (LoRa_OperationTypeDef _Operacao, LoRa_KeyTypeDef *_Keyword);
LoRa_StatusTypeDef AT_ApplicationSessionKey (LoRa_OperationTypeDef _Operacao, LoRa_KeyTypeDef *_Keyword);
LoRa_StatusTypeDef AT_DeviceAddress (LoRa_OperationTypeDef _Operacao, LoRa_AdressTypeDef *_Adress);
LoRa_StatusTypeDef AT_NetworkIdentifier (LoRa_OperationTypeDef _Operacao, LoRa_IdTypeDef *_Identifier);
LoRa_StatusTypeDef AT_ActivationSettingValue (LoRa_ActivationSettingTypeDef _hSettings);

LoRa_StatusTypeDef AT_DataUplinkText (LoRa_Data _Data[5]);
LoRa_StatusTypeDef AT_DataUplinkHexadecimal (LoRa_Data _Data[5]);
LoRa_StatusTypeDef AT_ConfirmDownlinkDataText (LoRa_Data _Data[5]);
LoRa_StatusTypeDef AT_ConfirmDownlinkDataHexadecimal(LoRa_Data _Data[5]);
LoRa_StatusTypeDef AT_ReturnRSSI(LoRa_Value *_Value);
LoRa_StatusTypeDef AT_ReturnsSNR(LoRa_Value *_Value);

#endif /* INC_LORA_SX1276_H_ */