
#include "FRC_NetworkCommunication/CANSessionMux.h"

#include <iostream>
#include <vector>
#include <map>
#include <iterator>

#include <sys/time.h>

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
#include <linux/can/bcm.h>

std::map<uint32_t, tCANStreamMessage> receivedMsgs; //I need to keep track of time and tCANStreamMessage does it

#ifdef __cplusplus
extern "C"
{
#endif  

	struct ifreq ifr;
	struct sockaddr_can addr;
	int s;
	int sbcm;
	struct can_filter rfilter[1];

	struct can_msg {
		struct bcm_msg_head msg_head;
		struct can_frame frame[1];
	} msg;

	void init_socketCAN(char *ifname) {
		s = socket(PF_CAN, SOCK_RAW | SOCK_NONBLOCK, CAN_RAW); //create nonblocking raw can socket
		strcpy(ifr.ifr_name, ifname);
		ioctl(s, SIOCGIFINDEX, &ifr); 
		addr.can_family = AF_CAN; 
		addr.can_ifindex = ifr.ifr_ifindex; 
		
		bind(s, (struct sockaddr *)&addr, sizeof(addr));

		sbcm = socket(PF_CAN, SOCK_DGRAM, CAN_BCM);
		connect(sbcm, (struct sockaddr *)&addr, sizeof(addr));

	}

	void FRC_NetworkCommunication_CANSessionMux_sendMessage(uint32_t messageID, const uint8_t *data, uint8_t dataSize, int32_t periodMs, int32_t *status) {
		std::cout << "sendCAN 0x" << std::hex << messageID << "\t";
		for(int i = 0; i < dataSize; i++) {
			std::cout << unsigned(data[i]) << "\t";
		}
		std::cout << "datsize:" << unsigned(dataSize) << "\tper:" << periodMs << std::endl;

		msg.msg_head.opcode  = TX_SETUP;
		msg.msg_head.can_id  = messageID | 0x80000000;
		msg.msg_head.flags   = SETTIMER|STARTTIMER|TX_CP_CAN_ID;
		msg.msg_head.nframes = 1;
		msg.msg_head.count = 0;
		msg.msg_head.ival1.tv_sec = 0;
		msg.msg_head.ival1.tv_usec = 0;
		msg.msg_head.ival2.tv_sec = 0;
		msg.msg_head.ival2.tv_usec = 1000*periodMs;
		//msg.frame[0].can_id    = 0x42; /* obsolete when using TX_CP_CAN_ID */
		msg.frame[0].can_dlc   = dataSize;
		memcpy(msg.frame[0].data, data, 8);
		write(sbcm, &msg, sizeof(msg));
	}

// this function does not block
// proper behavior is to return the last received message with requested messageID
// TODO: have old messages expire
	void FRC_NetworkCommunication_CANSessionMux_receiveMessage(uint32_t *messageID, uint32_t messageIDMask, uint8_t *data, uint8_t *dataSize, uint32_t *timeStamp, int32_t *status) {

		//rfilter[0].can_id = *messageID;
		//rfilter[0].can_mask = CAN_EFF_MASK;
		//setsockopt(s, SOL_CAN_RAW, CAN_RAW_FILTER, &rfilter, sizeof(rfilter));

		struct can_frame msg;
		while(1) {
			int a = read(s, &msg, sizeof(msg)); //TODO: better error handling
			if(a == -1) break; // exit loop if there are no more messages to read

			tCANStreamMessage tmsg;
			tmsg.messageID = msg.can_id & ~0x80000000; //remember to unset the MSB that indicates an extended ID
			tmsg.dataSize = msg.can_dlc;
			memcpy(tmsg.data, msg.data, 8);
			//std::cout << "tmsg id " << tmsg.messageID << std::endl;
			receivedMsgs[tmsg.messageID] = tmsg;

			struct timeval tval;
			gettimeofday(&tval, NULL);
			tmsg.timeStamp = tval.tv_usec;

			receivedMsgs[tmsg.messageID] = tmsg;

		}

		std::map<uint32_t, tCANStreamMessage>::iterator i = receivedMsgs.find(*messageID);
		if(i == receivedMsgs.end()) {
			*status = ERR_CANSessionMux_MessageNotFound;
			std::cout << "no message"<< std::endl;
		} else {
			*dataSize = receivedMsgs[*messageID].dataSize;
			*status = 0;
			memcpy(data, receivedMsgs[*messageID].data, 8);

			std::cout << "reqsCAN 0x" << std::hex << *messageID<< "\t";
			for(int b = 0; b < *dataSize; b++) {
				std::cout << unsigned(data[b]) << "\t";
			}
			std::cout <<std::endl;
		}
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
		uint8_t dataSize;
		FRC_NetworkCommunication_CANSessionMux_receiveMessage(&_messageID, 0, &messages->data[0], &dataSize, &timeStamp, &mystatus);
		messages->messageID = _messageID;
		*messagesRead = 1;
	}

	//void FRC_NetworkCommunication_CANSessionMux_getCANStatus(float *percentBusUtilization, uint32_t *busOffCount, uint32_t *txFullCount, uint32_t *receiveErrorCount, uint32_t *transmitErrorCount, int32_t *status);

#ifdef __cplusplus
}
#endif
