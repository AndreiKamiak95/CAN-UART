
#include "can_processing.h"

CAN_TxHeaderTypeDef msgHeader;

CAN_MsgTypeDef recvCanMeassages[MAX_COUNT_ELEM];
uint8_t countQueueMess = 0;

void SendMessageToCan(CAN_MsgTypeDef *msg)
{
	msgHeader.StdId = msg->StdId;
	msgHeader.ExtId = msg->ExtId;
	
	msgHeader.DLC = msg->DLC;
	msgHeader.TransmitGlobalTime = DISABLE;
	msgHeader.RTR = msg->RTR;
	msgHeader.IDE = msg->IDE;
	
	uint32_t mailBoxNum = 0;
	
	HAL_CAN_AddTxMessage(&hcan, &msgHeader, msg->Data, &mailBoxNum);
}

void AddCanMessages(CAN_MsgTypeDef msg)
{
	recvCanMeassages[countQueueMess] = msg;
	countQueueMess++;
	if (countQueueMess >= MAX_COUNT_ELEM)
		countQueueMess = MAX_COUNT_ELEM - 1;
}

void ProcessingQueueCanMessages()
{
	uint8_t strSend[27];
	
	if(countQueueMess != 0)
	{
		uint8_t poz = countQueueMess - 1;
		
		if(recvCanMeassages[poz].IDE == CAN_ID_EXT)
		{
			strSend[0] = recvCanMeassages[poz].RTR != 0 ? 'T' : 'R';
			strSend[1] = HexToAscii((uint8_t)((recvCanMeassages[poz].ExtId & 0xF0000000) >> 28));
			strSend[2] = HexToAscii((uint8_t)((recvCanMeassages[poz].ExtId & 0x0F000000) >> 24));
			strSend[3] = HexToAscii((uint8_t)((recvCanMeassages[poz].ExtId & 0x00F00000) >> 20));
			strSend[4] = HexToAscii((uint8_t)((recvCanMeassages[poz].ExtId & 0x000F0000) >> 16));
			strSend[5] = HexToAscii((uint8_t)((recvCanMeassages[poz].ExtId & 0x0000F000) >> 12));
			strSend[6] = HexToAscii((uint8_t)((recvCanMeassages[poz].ExtId & 0x00000F00) >> 8));
			strSend[7] = HexToAscii((uint8_t)((recvCanMeassages[poz].ExtId & 0x000000F0) >> 4));
			strSend[8] = HexToAscii((uint8_t)( recvCanMeassages[poz].ExtId & 0x0000000F));
			strSend[9] = HexToAscii((uint8_t)( recvCanMeassages[poz].DLC));
			for(uint8_t i = 0; i < recvCanMeassages[poz].DLC; i++)
			{
				strSend[10 + 2*i] = HexToAscii((recvCanMeassages[poz].Data[i] & 0xF0) >> 4);
				strSend[10 + 2*i + 1] = HexToAscii(recvCanMeassages[poz].Data[i] & 0x0F);
			}
			strSend[11 + recvCanMeassages[poz].DLC * 2] = 0x0D;
			USART_Send_Str(strSend, 11 + recvCanMeassages[poz].DLC * 2);
		}
		else if(recvCanMeassages[poz].IDE == CAN_ID_STD)
		{
			strSend[0] = recvCanMeassages[poz].RTR != 0 ? 't' : 'r';
			strSend[1] = HexToAscii((uint8_t)((recvCanMeassages[poz].StdId & 0x00000F00) >> 8));
			strSend[2] = HexToAscii((uint8_t)((recvCanMeassages[poz].StdId & 0x000000F0) >> 4));
			strSend[3] = HexToAscii((uint8_t)( recvCanMeassages[poz].StdId & 0x0000000F));
			strSend[4] = HexToAscii((uint8_t)( recvCanMeassages[poz].DLC));
			for(uint8_t i = 0; i < recvCanMeassages[poz].DLC; i++)
			{
				strSend[5 + 2*i] = HexToAscii((recvCanMeassages[poz].Data[i] & 0xF0) >> 4);
				strSend[5 + 2*i + 1] = HexToAscii(recvCanMeassages[poz].Data[i] & 0x0F);
			}
			strSend[5 + recvCanMeassages[poz].DLC * 2] = 0x0D;
			USART_Send_Str(strSend, 6 + recvCanMeassages[poz].DLC * 2);
		}
		countQueueMess--;
	}
}
