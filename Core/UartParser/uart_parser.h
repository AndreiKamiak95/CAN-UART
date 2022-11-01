#ifndef UART_PARSER_H
#define UART_PARSER_H

#include <stdint.h>
#include "usart.h"
#include "can_configurator.h"
#include "can_processing.h"

#define SendSymbolToUart(a) LL_USART_TransmitData8(USART1, a)

//void SendSymbolToUart(uint8_t symbol);

void UartParser(uint8_t symbol);
void UART_Message_Processing();
void TransmitVersion();
void OpenCAN();
void CloseCAN();
void SetBitRates();
void SetTimeStap();
void ParsMessage(uint8_t *mess);
void ParsExtMessage(uint8_t *mess);
void ParsRtr(uint8_t *mess);
void ParsExtRtr(uint8_t *mess);
void Flag();

uint8_t AsciiToHex(uint8_t);
uint8_t HexToAscii(uint8_t);

#endif //UART_PARSER_H
