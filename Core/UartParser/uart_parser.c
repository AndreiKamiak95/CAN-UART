
#include "uart_parser.h"

#define LEN_UART_BUFFER 30

uint8_t OKSymbol = 0x0D;
uint8_t stopSymbol = 0x0D; //[CR]
const uint8_t version1 = 'v';
const uint8_t version2 = 'V';
const uint8_t close = 'C'; //Close the CAN channel
const uint8_t setBitRates = 'S'; //Setup with standard CAN bit-rates where n is 0-8.
const uint8_t open = 'O'; //Open the CAN channel
const uint8_t timeStamp = 'Z'; // Sets Time Stamp ON/OFF for received frames only
const uint8_t extMess = 'T'; //Extension Message
const uint8_t mess = 't'; // Message
const uint8_t rtrMess = 'r'; // RTR message
const uint8_t extRtrMess = 'R'; //Extention RTR message 
const uint8_t statusFlag = 'F'; // Read Status Flags
const uint8_t ACR = 'M';
const uint8_t AMR = 'm';

uint8_t uartBuffer[LEN_UART_BUFFER];
uint8_t uartSymbolIndex = 0;
uint8_t flagReceiveMessage = 0;

void UartParser(uint8_t symbol)
{
	if(symbol != stopSymbol)
	{
		uartBuffer[uartSymbolIndex] = symbol;
		uartSymbolIndex++;
	}
	else
	{
		flagReceiveMessage = 1;
	}
}

void UART_Message_Processing()
{
	if(flagReceiveMessage != 1)
		return;
	
	switch (uartBuffer[0])
	{
		case version1:
			TransmitVersion();
		break;
			
		case version2:
			TransmitVersion();
		break;
		
		case open:
			OpenCAN();
		break;
		
		case close:
			CloseCAN();
		break;
		
		case setBitRates:
			SetBitRates(uartBuffer);
		break;
		
		case timeStamp:
			SetTimeStap();
		break;
		
		case mess:
			ParsMessage(uartBuffer);
		break;
		
		case extMess:
			ParsExtMessage(uartBuffer);
		break;
		
		case statusFlag:
			Flag();
		break;
		
		case rtrMess:
			ParsRtr(uartBuffer);
		break;
		
		case extRtrMess:
			ParsExtRtr(uartBuffer);
		break;
		
		case ACR:
		break;
		
		case AMR:
		break;
		
		default:
			TransmitVersion();
		break;
	}
	uartSymbolIndex = 0;
	flagReceiveMessage = 0;
}

uint8_t AsciiToHex(uint8_t symbol)
{
	uint8_t buf = 0x00;
	if((symbol >= '0') && (symbol <= '9'))
		buf = symbol-'0';
	else if((symbol >= 'A') && (symbol <= 'F'))
		buf = symbol-55;
	
	return buf;
}

uint8_t HexToAscii(uint8_t num)
{
	uint8_t buf = 0x00;
	if((num >= 0) && (num <= 9))
		buf = num + '0';
	else if((num >= 10) && (num <= 15))
		buf = num + 55;

	return buf;
}

void TransmitVersion()
{
	uint8_t ver[] = "1010";
	USART_Send_Str(ver, 4);
	USART_Send_Str(&OKSymbol, 1);
}

void CloseCAN()
{
	CAN_DeInit();
	USART_Send_Str(&OKSymbol, 1);
}

void SetBitRates(uint8_t *mess)
{
	uint8_t recvS = AsciiToHex(mess[1]);
	CAN_SetSpeed(recvS);
	USART_Send_Str(&OKSymbol, 1);
}

void SetTimeStap()
{
	USART_Send_Str(&OKSymbol, 1);
}

void OpenCAN()
{
	CAN_ReInit();
	USART_Send_Str(&OKSymbol, 1);
}

void ParsMessage(uint8_t *mess)
{
	CAN_MsgTypeDef msg;

	msg.StdId =  (((uint32_t)AsciiToHex(mess[1])) << 8) | (((uint32_t)AsciiToHex(mess[2])) << 4) | ((uint32_t)AsciiToHex(mess[3]));
	msg.DLC = AsciiToHex(mess[4]);
	msg.IDE = CAN_ID_STD;
	msg.RTR = 0;
	for(uint8_t i = 0; i < msg.DLC; i++)
	{
		msg.Data[i] = (((uint32_t)AsciiToHex(mess[2 * i  + 5])) << 4) | ((uint32_t)AsciiToHex(mess[2 * i + 6]));
	}
	SendMessageToCan(&msg);
	USART_Send_Str(&OKSymbol, 1);
}

void ParsExtMessage(uint8_t *mess)
{
	CAN_MsgTypeDef msg;

	msg.ExtId	= 0;
	msg.ExtId |= (((uint32_t)AsciiToHex(mess[1])) << 28) | (((uint32_t)AsciiToHex(mess[2])) << 24);
	msg.ExtId |= (((uint32_t)AsciiToHex(mess[3])) << 20) | (((uint32_t)AsciiToHex(mess[4])) << 16);
	msg.ExtId |= (((uint32_t)AsciiToHex(mess[5])) << 12) | (((uint32_t)AsciiToHex(mess[6])) << 8);
	msg.ExtId |= (((uint32_t)AsciiToHex(mess[7])) << 4)  |  ((uint32_t)AsciiToHex(mess[8]));
	msg.DLC = AsciiToHex(mess[9]);
	msg.IDE = CAN_ID_EXT;
	msg.RTR = 0;
	for(uint8_t i = 0; i < msg.DLC; i++)
	{
		msg.Data[i] = (((uint32_t)AsciiToHex(mess[2 * i  + 10])) << 4) | ((uint32_t)AsciiToHex(mess[2 * i + 11]));
	}
	SendMessageToCan(&msg);
	USART_Send_Str(&OKSymbol, 1);
}

void ParsRtr(uint8_t *mess)
{
	CAN_MsgTypeDef msg;

	msg.StdId =  (((uint32_t)AsciiToHex(mess[1])) << 8) | (((uint32_t)AsciiToHex(mess[2])) << 4) | ((uint32_t)AsciiToHex(mess[3]));
	msg.ExtId = 0;
	msg.DLC = AsciiToHex(mess[4]);
	msg.RTR = 1;
	for(uint8_t i = 0; i < msg.DLC; i++)
	{
		msg.Data[i] = (((uint32_t)AsciiToHex(mess[2 * i  + 5])) << 4) | ((uint32_t)AsciiToHex(mess[2 * i + 6]));
	}
	SendMessageToCan(&msg);
	USART_Send_Str(&OKSymbol, 1);
}

void ParsExtRtr(uint8_t *mess)
{
	CAN_MsgTypeDef msg;

	msg.ExtId	= 0;
	msg.ExtId |= (((uint32_t)AsciiToHex(mess[1])) << 28) | (((uint32_t)AsciiToHex(mess[2])) << 24);
	msg.ExtId |= (((uint32_t)AsciiToHex(mess[3])) << 20) | (((uint32_t)AsciiToHex(mess[4])) << 16);
	msg.ExtId |= (((uint32_t)AsciiToHex(mess[5])) << 12) | (((uint32_t)AsciiToHex(mess[6])) << 8);
	msg.ExtId |= (((uint32_t)AsciiToHex(mess[7])) << 4)  |  ((uint32_t)AsciiToHex(mess[8]));
	msg.StdId = 0;
	msg.DLC = AsciiToHex(mess[9]);
	msg.RTR = 1;
	for(uint8_t i = 0; i < msg.DLC; i++)
	{
		msg.Data[i] = (((uint32_t)AsciiToHex(mess[2 * i  + 10])) << 4) | ((uint32_t)AsciiToHex(mess[2 * i + 11]));
	}
	SendMessageToCan(&msg);
	USART_Send_Str(&OKSymbol, 1);
}

void Flag()
{
	USART_Send_Str(&OKSymbol, 1);
}
