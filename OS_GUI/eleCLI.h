#ifndef ELEVATORTHREAD_H
#define ELEVATORTHREAD_H
#include <iostream>                // std::cout
#include <thread>                // std::thread
#include <mutex>                // std::mutex, std::unique_lock
#include <condition_variable>    // std::condition_variable
#include <string>
#include <chrono>


const int NumberOfFloors=10;

std::mutex mtx_havePeople;
std::mutex mtx_destination;
std::mutex mtx_floor;

std::condition_variable cvFloorIn_up[NumberOfFloors];
std::condition_variable cvFloorOut_up[NumberOfFloors];
std::condition_variable cvFloorIn_down[NumberOfFloors];
std::condition_variable cvFloorOut_down[NumberOfFloors];

bool havePeople_up[NumberOfFloors];
bool destination_up[NumberOfFloors];
bool havePeople_down[NumberOfFloors];
bool destination_down[NumberOfFloors];
bool upOrDown=true;//Up:true Down:false
int nowFloor=1;

std::thread th[3];



void passenger(int startFloor,int endFloor,std::string name);
void the();
void elevator();
void init();





#endif // ELEVATORTHREAD_H
