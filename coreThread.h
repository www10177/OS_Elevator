#ifndef CORETHREAD_H
#define CORETHREAD_H 
#include <iostream>                // std::cout
#include <thread>                // std::thread
#include <mutex>                // std::mutex, std::unique_lock
#include <condition_variable>    // std::condition_variable
#include <string>
#include <chrono>
#include <vector>
#include <random>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#define NumberOfFloors 8
#define Time_TIMES_NUM 100000

std::mutex mtx_havePeople;
std::mutex mtx_destination;
std::mutex mtx_floor;
std::mutex mtx_Elevator;

std::condition_variable cvFloorIn_up[NumberOfFloors];
std::condition_variable cvFloorOut_up[NumberOfFloors];
std::condition_variable cvFloorIn_down[NumberOfFloors];
std::condition_variable cvFloorOut_down[NumberOfFloors];
std::condition_variable cvElevatorIsAwake;

bool havePeople_up[NumberOfFloors];
bool destination_up[NumberOfFloors];
bool havePeople_down[NumberOfFloors];
bool destination_down[NumberOfFloors];
bool upOrDown=true;//Up:true Down:false
int nowFloor=1;
int peopleNum=0;
int nowDataPlace=0;
bool elevatorIsAwake=false;
std::vector<std::thread> passengerThreads;;
class Data;
std::vector<Data> dataVector;

class Data{
	public:
		int startFloor;
		int endFloor;
		bool isDone;
		bool upOrDown;
		bool isIn;
		bool isOut;
		Data();
};
void t_passenger(int start, int end);
void generator();
int stairSelect();
void t_elevator();
void t_init();


#endif
