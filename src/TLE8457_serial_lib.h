#ifndef TLE8457_SERIAL_LIB
#define TLE8457_SERIAL_LIB

#include "common.h"

#ifdef CONTROLLER
  #define OUTPUT_QUEUE_SIZE 140
#else
  #define OUTPUT_QUEUE_SIZE 3
#endif

#define MAX_TRANSMIT_DELAY 100


#define NUMBER_OF_RETRANSMISSIONS 1
#define MAX_NUM_OF_RETRANSMISSIONS 5


/**
 * PROTOCOL
 *
 * 1) choose new seq number (increase)
 * 2) send a frame
 *      listen for a clear wire
 *       CSMA/CR - listen to the wire and in case of incorrect reading stop transmission. The lower ID device should win
 * 3) wait random time - random seed is a device ID
 * 4) repeat steps 2-4 given number of times
 *
 * recieve:
 * 1) frame witj incorrect CRC => reject
 * 2) check if pair SenderDevId/Seq was recently seen (keep a list of last pairs). If yes => reject
 * 3) execute
 */

#define FRAME_TRANSMISSION_TIME 20



#define COMMUNICATION_PAYLOAD_DATA_SIZE 8
/**
 * Communication frame
 */
typedef struct
{
  uint8_t SenderDevId;    // id of the sender
  uint8_t DstDevId;       // device ID the message is sent to or broadcast
  uint8_t Seq;            // seq number. Retransmitted frame should have the same seq
  uint8_t MessageType;    // MESSAGE_TYPE*

  uint8_t Data[COMMUNICATION_PAYLOAD_DATA_SIZE];  // data structure, to be mapped on tMessageType* structure

  uint16_t crc;   // CRC, frame will be rejected if CRC is incorrect
} tCommunicationFrame;

#include "TLE8457_serial_lib/CommSender.h"
#include "TLE8457_serial_lib/CommReciever.h"

#endif
