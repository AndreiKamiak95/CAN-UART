#ifndef CAN_PROCESSING_H
#define CAN_PROCESSING_H

#include <stdint.h>
#include "can_configurator.h"
#include "uart_parser.h"

#define CAN_MAIL_BOX 0
#define MAX_COUNT_ELEM 32

void SendMessageToCan(CAN_MsgTypeDef *msg);
void AddCanMessages(CAN_MsgTypeDef msg);
void ProcessingQueueCanMessages();

#endif //CAN_PROCESSING_H