
#include "FRC_NetworkCommunication/CANSessionMux.h"

#include <iostream>
#include <vector>
#include <map>
#include <iterator>


#ifdef __cplusplus
extern "C"
{
#endif  
	//ros::NodeHandle *n;

	void init_CANSend() {
	}

	void FRC_NetworkCommunication_CANSessionMux_sendMessage(uint32_t messageID, const uint8_t *data, uint8_t dataSize, int32_t periodMs, int32_t *status) {
		std::cout << "sendCAN " << messageID << "\t";
		for(int i = 0; i < dataSize; i++) {
			std::cout << unsigned(data[i]) << "\t";
		}
		std::cout << "datsize:" << unsigned(dataSize) << "\tper:" << periodMs << std::endl;
	}

	void FRC_NetworkCommunication_CANSessionMux_receiveMessage(uint32_t *messageID, uint32_t messageIDMask, uint8_t *data, uint8_t *dataSize, uint32_t *timeStamp, int32_t *status) {
		std::cout << "recvCAN " << *messageID << std::endl;


	}

//sessionHandle: set this integer to identify a session
//messageID: arbID of messages to cache
//messageIDMask: CanTalonSRX sets this to 0xFFFFFFFF, probably can ignore
//maxMessages: number of messages to cache
//status: set to zero if everything worked
	uint32_t _messageID = 0;
	void FRC_NetworkCommunication_CANSessionMux_openStreamSession(uint32_t *sessionHandle, uint32_t messageID, uint32_t messageIDMask, uint32_t maxMessages, int32_t *status) {
		*sessionHandle = 1; //set the session handle to something or else CanTalonSRX won't continue
		_messageID = messageID;
	}

	void FRC_NetworkCommunication_CANSessionMux_closeStreamSession(uint32_t sessionHandle) {
	}

	void FRC_NetworkCommunication_CANSessionMux_readStreamSession(uint32_t sessionHandle, struct tCANStreamMessage *messages, uint32_t messagesToRead, uint32_t *messagesRead, int32_t *status) {
		uint8_t data[8];
		uint32_t timeStamp = 0;
		int32_t mystatus = 0;
		uint8_t tmpdata[8];
		uint8_t datasize;
		FRC_NetworkCommunication_CANSessionMux_receiveMessage(&_messageID, 0, &messages->data[0], &datasize, &timeStamp, &mystatus);
		messages->messageID = _messageID;
		*messagesRead = 1;
	}

	//void FRC_NetworkCommunication_CANSessionMux_getCANStatus(float *percentBusUtilization, uint32_t *busOffCount, uint32_t *txFullCount, uint32_t *receiveErrorCount, uint32_t *transmitErrorCount, int32_t *status);

#ifdef __cplusplus
}
#endif
