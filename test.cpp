#include "HAL/CanTalonSRX.h"
#include <iostream>
#include <vector>
#include "FRC_NetworkCommunication/CANSessionMux.h"
#include <memory>
#include <unistd.h>

CanTalonSRX* motor;

int main(int argc, char **argv) {
	if(argc > 1) {
		init_socketCAN(argv[1]);
		motor = new CanTalonSRX(2);
		double setgain = 0;
		while(true) {
			//motor->SetDemand(100);
			//sleep(1);
			//motor->SetDemand(0);
			//sleep(1);
			//double dparam;
			//motor->GetBatteryV(dparam);
			
			//std::cout << "volts: " <<std::dec << dparam << std::endl <<std::endl;
			motor->SetPgain(0, setgain);
			double gain;
			motor->GetPgain(0, gain);
			usleep(100000);
			setgain = setgain + .01;
			std::cout << "pgain: " <<std::dec << gain << std::endl;
			//int param;
			//motor->GetLimitSwitchClosedFor(param);
			//std::cout << "firmv: " << param<< std::endl <<std::endl;
			//usleep(20000);
			//sleep(1);
		}
	}
}
