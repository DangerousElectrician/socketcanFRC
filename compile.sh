export CPATH=./include
g++ -Wall -g include/FRC_NetworkCommunication/CANSessionMux.h CANSessionMux.cpp include/HAL/CanTalonSRX.h CanTalonSRX.cpp include/ctre/CtreCanNode.h CtreCanNode.cpp include/ctre/ctre.h test.cpp -std=c++0x
