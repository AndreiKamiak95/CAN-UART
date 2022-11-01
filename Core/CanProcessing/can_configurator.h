#ifndef CAN_CONFIGURATOR_H
#define CAN_CONFIGURATOR_H

#include <stdint.h>
#include "can.h"

//Fclk = 72MHz

//S0 = 10kbit/s
#define S0							0
#define S0_PRESCALLER		200
#define S0_TIMESEG1 		CAN_BS1_15TQ
#define S0_TIMESEG2			CAN_BS2_2TQ

//S1 = 20kbit/s
#define S1							1
#define S1_PRESCALLER		100
#define S1_TIMESEG1			CAN_BS1_15TQ
#define S1_TIMESEG2			CAN_BS2_2TQ

//S2 = 50kbit/s
#define S2							2
#define S2_PRESCALLER		40
#define S2_TIMESEG1 		CAN_BS1_15TQ
#define S2_TIMESEG2			CAN_BS2_2TQ

//S3 = 100kbit/s
#define S3							3
#define S3_PRESCALLER		20
#define S3_TIMESEG1 		CAN_BS1_15TQ
#define S3_TIMESEG2			CAN_BS2_2TQ

//S4 = 125kbit/s
#define S4							4
#define S4_PRESCALLER		16
#define S4_TIMESEG1 		CAN_BS1_15TQ
#define S4_TIMESEG2			CAN_BS2_2TQ

//S5 = 250kbit/s
#define S5							5
#define S5_PRESCALLER		9
#define S5_TIMESEG1 		CAN_BS1_16TQ
#define S5_TIMESEG2			CAN_BS2_2TQ

//S6 = 500kbit/s
#define S6							6
#define S6_PRESCALLER		4
#define S6_TIMESEG1 		CAN_BS1_15TQ
#define S6_TIMESEG2			CAN_BS2_2TQ

//S7 = 800kbit/s
#define S7							7
#define S7_PRESCALLER		3
#define S7_TIMESEG1 		CAN_BS1_12TQ
#define S7_TIMESEG2			CAN_BS2_2TQ

//S8 = 1000kbit/s
#define S8							8
#define S8_PRESCALLER		2
#define S8_TIMESEG1 		CAN_BS1_15TQ
#define S8_TIMESEG2			CAN_BS2_2TQ

typedef struct 
{
	uint32_t StdId;  /*!< Specifies the standard identifier.
                        This parameter can be a value between 0 to 0x7FF. */

  uint32_t ExtId;  /*!< Specifies the extended identifier.
                        This parameter can be a value between 0 to 0x1FFFFFFF. */

  uint8_t IDE;     /*!< Specifies the type of identifier for the message that 
                        will be transmitted. This parameter can be a value 
                        of @ref CAN_identifier_type */

  uint8_t RTR;     /*!< Specifies the type of frame for the message that will 
                        be transmitted. This parameter can be a value of 
                        @ref CAN_remote_transmission_request */

  uint8_t DLC;     /*!< Specifies the length of the frame that will be 
                        transmitted. This parameter can be a value between 
                        0 to 8 */

  uint8_t Data[8]; /*!< Contains the data to be received. It ranges from 0 to 
                        0xFF. */
} CAN_MsgTypeDef;	

void CAN_ReInit();
void CAN_DeInit();
void CAN_SetSpeed(uint8_t setSpeed);

#endif //CAN_CONFIGURATOR_H