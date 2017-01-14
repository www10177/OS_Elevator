#include <iostream>                // std::cout
#include <thread>                // std::thread
#include <mutex>                // std::mutex, std::unique_lock
#include <condition_variable>    // std::condition_variable
#include <string>
#include <chrono>
#include <vector>
#include <random>
#include <stdlib.h>
#include <time.h>
#define NumberOfFloors 10

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
int peopleNum=0;

std::vector<std::thread> passengerThreads;;
//std::thread th[3];

void passenger(int startFloor,int endFloor){
//	peopleNum++;
	if(startFloor<endFloor){

		mtx_havePeople.lock();
		havePeople_up[startFloor-1]=true;
		std::cout<<"["<<std::this_thread::get_id()<<"] at "<<startFloor<<" waiting for elevator and wanna gotta "<<endFloor<<std::endl;
		mtx_havePeople.unlock();

		std::unique_lock <std::mutex> lck(mtx_floor);
		cvFloorIn_up[startFloor-1].wait(lck);

		mtx_destination.lock();
		destination_up[endFloor-1]=true;
		std::cout<<"["<<std::this_thread::get_id()<<"] in and want go to "<<endFloor<<std::endl;
		mtx_destination.unlock();

		if(nowFloor!=endFloor){
			cvFloorOut_up[endFloor-1].wait(lck);
		}
		std::cout<<"["<<std::this_thread::get_id()<<"] out!"<<std::endl;
	}
	else{

		mtx_havePeople.lock();
		havePeople_down[startFloor-1]=true;
		std::cout<<"["<<std::this_thread::get_id()<<"] at "<<startFloor<<" and wait elevator"<<std::endl;
		mtx_havePeople.unlock();

		std::unique_lock <std::mutex> lck(mtx_floor);
		cvFloorIn_down[startFloor-1].wait(lck);

		mtx_destination.lock();
		destination_down[endFloor-1]=true;
		std::cout<<"["<<std::this_thread::get_id()<<"] in and want go to "<<endFloor<<std::endl;
		mtx_destination.unlock();

		if(nowFloor!=endFloor){
			cvFloorOut_down[endFloor-1].wait(lck);
		}
		std::cout<<"["<<std::this_thread::get_id()<<"] out!"<<std::endl;
	}
	peopleNum--;
}
void the(){
	unsigned seed;
	seed=(unsigned)time(NULL);
	std::default_random_engine generator(seed);
	std::poisson_distribution<int> distribution(5);

//	for(int i = 0;i<3;i++){
	while(1){	
		peopleNum++;
		int startFloor=0;
		int endFloor=0;
		int time;
		while(startFloor>10 || startFloor==0){	
			startFloor = distribution(generator);
		}
		while(endFloor>10 || endFloor==0 || endFloor==startFloor){
			endFloor = distribution(generator);
		}
		time = distribution(generator);
		std::this_thread::sleep_for(std::chrono::seconds(time));
		passengerThreads.push_back(std::thread(passenger,startFloor,endFloor));
	}
}
void elevator(){
	std::this_thread::sleep_for(std::chrono::seconds(1));
	int i;
	while(peopleNum!=0){
		if(upOrDown==true){
			for(i=0;i<NumberOfFloors;i++){
				std::cout<<"elevator: "<<nowFloor<<std::endl;

				mtx_havePeople.lock();mtx_destination.lock();
				if(havePeople_up[nowFloor-1]==true || destination_up[nowFloor-1]==true){
					cvFloorOut_up[nowFloor-1].notify_all();
					cvFloorIn_up[nowFloor-1].notify_all();
					havePeople_up[nowFloor-1]=false;
					destination_up[nowFloor-1]=false;
					std::cout<<"open door!"<<std::endl;
				}
				mtx_havePeople.unlock();mtx_destination.unlock();

				std::this_thread::sleep_for(std::chrono::seconds(3));
				nowFloor++;
			}
			upOrDown=false;
			nowFloor-=1;
		}

		if(upOrDown==false){
			for(i=0;i<NumberOfFloors;i++){
				std::cout<<"elevator: "<<nowFloor<<std::endl;

				mtx_havePeople.lock();mtx_destination.lock();
				if(havePeople_down[nowFloor-1]==true || destination_down[nowFloor-1]==true){
					cvFloorOut_down[nowFloor-1].notify_all();
					cvFloorIn_down[nowFloor-1].notify_all();
					havePeople_down[nowFloor-1]=false;
					destination_down[nowFloor-1]=false;
					std::cout<<"open door!"<<std::endl;
				}
				mtx_havePeople.unlock();mtx_destination.unlock();

				std::this_thread::sleep_for(std::chrono::seconds(3));
				nowFloor--;
			}
		}
	}
	std::cout<<"elevator end\n";
}

int main(){
	int i,k;
	for(i=0;i<10;i++){
		havePeople_up[i]=false;
		destination_up[i]=false;
		havePeople_down[i]=false;
		destination_down[i]=false;
	}

	std::thread mthread(elevator);
	the();

	for(k = 0;k<passengerThreads.size();k++){
		passengerThreads[k].join();
	}

	mthread.join();

	return 0;
}
