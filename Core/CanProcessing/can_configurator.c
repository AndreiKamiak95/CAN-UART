
#include "can_configurator.h"

extern CAN_HandleTypeDef hcan;
volatile uint8_t S = 5;
uint8_t isOpenCAN = 0;

void CAN_ReInit()
{
	if(isOpenCAN == 1)
		return;
	
	hcan.Instance = CAN1;
  //hcan.Init.Mode = CAN_MODE_NORMAL;
	hcan.Init.Mode = CAN_MODE_LOOPBACK;
  hcan.Init.TimeTriggeredMode = DISABLE;
  hcan.Init.AutoBusOff = DISABLE;
  hcan.Init.AutoWakeUp = DISABLE;
  hcan.Init.AutoRetransmission = DISABLE;
  hcan.Init.ReceiveFifoLocked = DISABLE;
  hcan.Init.TransmitFifoPriority = DISABLE;
	
	switch(S)
	{
		case S0:
			hcan.Init.Prescaler = 		S0_PRESCALLER;
			hcan.Init.TimeSeg1 = 			S0_TIMESEG1;
			hcan.Init.TimeSeg2 = 			S0_TIMESEG2;
		break;
		
		case S1:
			hcan.Init.Prescaler = 		S1_PRESCALLER;
			hcan.Init.TimeSeg1 = 			S1_TIMESEG1;
			hcan.Init.TimeSeg2 = 			S1_TIMESEG2;
		break;
		
		case S2:
			hcan.Init.Prescaler = 		S2_PRESCALLER;
			hcan.Init.TimeSeg1 = 			S2_TIMESEG1;
			hcan.Init.TimeSeg2 = 			S2_TIMESEG2;
		break;
		
		case S3:
			hcan.Init.Prescaler = 		S3_PRESCALLER;
			hcan.Init.TimeSeg1 = 			S3_TIMESEG1;
			hcan.Init.TimeSeg2 = 			S3_TIMESEG2;
		break;
		
		case S4:
			hcan.Init.Prescaler = 		S4_PRESCALLER;
			hcan.Init.TimeSeg1 = 			S4_TIMESEG1;
			hcan.Init.TimeSeg2 = 			S4_TIMESEG2;
		break;
		
		case S5:
			hcan.Init.Prescaler = 		S5_PRESCALLER;
			hcan.Init.TimeSeg1 = 			S5_TIMESEG1;
			hcan.Init.TimeSeg2 = 			S5_TIMESEG2;
		break;
		
		case S6:
			hcan.Init.Prescaler = 		S6_PRESCALLER;
			hcan.Init.TimeSeg1 = 			S6_TIMESEG1;
			hcan.Init.TimeSeg2 = 			S6_TIMESEG2;
		break;
		
		case S7:
			hcan.Init.Prescaler = 		S7_PRESCALLER;
			hcan.Init.TimeSeg1 = 			S7_TIMESEG1;
			hcan.Init.TimeSeg2 = 			S7_TIMESEG2;
		break;
		
		case S8:
			hcan.Init.Prescaler = 		S8_PRESCALLER;
			hcan.Init.TimeSeg1 = 			S8_TIMESEG1;
			hcan.Init.TimeSeg2 = 			S8_TIMESEG2;
		break;
	}
	hcan.Init.SyncJumpWidth = CAN_SJW_1TQ;
	
  if (HAL_CAN_Init(&hcan) != HAL_OK)
  {
    Error_Handler();
  }
	
	CAN_FilterTypeDef canFilterConfig;
  canFilterConfig.FilterBank = 0;
  canFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
  canFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
  canFilterConfig.FilterIdHigh = 0x0000;
  canFilterConfig.FilterIdLow = 0x0000;
  canFilterConfig.FilterMaskIdHigh = 0x0000;
  canFilterConfig.FilterMaskIdLow = 0x0000;
  canFilterConfig.FilterFIFOAssignment = CAN_RX_FIFO0;
  canFilterConfig.FilterActivation = ENABLE;
  canFilterConfig.SlaveStartFilterBank = 14;
  HAL_CAN_ConfigFilter(&hcan, &canFilterConfig);
	
	HAL_CAN_Start(&hcan);
	HAL_CAN_ActivateNotification(&hcan, CAN_IT_RX_FIFO0_MSG_PENDING);
	isOpenCAN = 1;
}

inline void CAN_DeInit()
{
	if(isOpenCAN == 0)
		return;
	
	HAL_CAN_DeactivateNotification(&hcan, CAN_IT_RX_FIFO0_MSG_PENDING);
	HAL_CAN_Stop(&hcan);
	
	HAL_CAN_DeInit(&hcan);

	isOpenCAN = 0;
}

inline void CAN_SetSpeed(uint8_t setSpeed)
{
	S = setSpeed;
}
