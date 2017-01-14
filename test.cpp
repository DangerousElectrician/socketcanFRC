#include "HAL/CanTalonSRX.h"
#include <iostream>
#include <vector>
#include "FRC_NetworkCommunication/CANSessionMux.h"
#include <memory>
#include <unistd.h>

CanTalonSRX* motor;
int main(int argc, char **argv) {
	init_socketCAN();
	motor = new CanTalonSRX(2);
	while(true) {
		//motor->SetDemand(100);
		//sleep(1);
		//motor->SetDemand(0);
		//sleep(1);
		double dparam;
		motor->GetBatteryV(dparam);
		std::cout << "volts: " << dparam << std::endl <<std::endl;
		usleep(20000);
	}
}
