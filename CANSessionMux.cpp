
#include "FRC_NetworkCommunication/CANSessionMux.h"

#include <iostream>
#include <vector>
#include <map>
#include <iterator>

//socketcan includes
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <net/if.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>

#include <linux/can.h>
#include <linux/can/raw.h>


#ifdef __cplusplus
extern "C"
{
#endif  

	struct ifreq ifr;
	struct sockaddr_can addr;
	int s;
	const char *ifname = "slcan0";

	void init_socketCAN() {
		//s = socket(PF_CAN, SOCK_DGRAM, CAN_BCM);
		s = socket(PF_CAN, SOCK_RAW, CAN_RAW);
		strcpy(ifr.ifr_name, ifname);
		ioctl(s, SIOCGIFINDEX, &ifr); 
		addr.can_family = AF_CAN; 
		addr.can_ifindex = ifr.ifr_ifindex; 
		//connect(s, (struct sockaddr *)&addr, sizeof(addr));
		
		bind(s, (struct sockaddr *)&addr, sizeof(addr));
	}

	void FRC_NetworkCommunication_CANSessionMux_sendMessage(uint32_t messageID, const uint8_t *data, uint8_t dataSize, int32_t periodMs, int32_t *status) {
		std::cout << "sendCAN " << messageID << "\t";
		for(int i = 0; i < dataSize; i++) {
			std::cout << unsigned(data[i]) << "\t";
		}
		std::cout << "datsize:" << unsigned(dataSize) << "\tper:" << periodMs << std::endl;
	}

	void FRC_NetworkCommunication_CANSessionMux_receiveMessage(uint32_t *messageID, uint32_t messageIDMask, uint8_t *data, uint8_t *dataSize, uint32_t *timeStamp, int32_t *status) {
		std::cout << "recvCAN " << std::hex<<*messageID << std::endl;
		struct can_frame msg;
		read(s, &msg, sizeof(msg));
		std::cout << std::hex<<msg.can_id << std::endl;
		for(int i=0; i<8; i++) data[i] = msg.data[i];



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
