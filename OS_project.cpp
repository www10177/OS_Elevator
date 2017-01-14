#include <iostream>                // std::cout
#include <thread>                // std::thread
#include <mutex>                // std::mutex, std::unique_lock
#include <condition_variable>    // std::condition_variable
#include <string>
#include <chrono>
#define NumberOfFloors 10

std::mutex mtx_havePeople;
std::mutex mtx_destination;
std::mutex mtx_floor;

std::condition_variable cvFloorIn[NumberOfFloors];
std::condition_variable cvFloorOut[NumberOfFloors];

bool havePeople[NumberOfFloors];
bool destination[NumberOfFloors];
int nowFloor=1;
std::thread th[3];

void passenger(int startFloor,int endFloor,std::string name){

	mtx_havePeople.lock();
	havePeople[startFloor-1]=true;
	std::cout<<name<<" at "<<startFloor<<" and wait elevator"<<std::endl;
	mtx_havePeople.unlock();

	std::unique_lock <std::mutex> lck(mtx_floor);
	//if(nowFloor!=startFloor){
		cvFloorIn[startFloor-1].wait(lck);
	//}

	mtx_destination.lock();
	destination[endFloor-1]=true;
	std::cout<<name<<" in and want go to "<<endFloor<<std::endl;
	mtx_destination.unlock();

	if(nowFloor!=endFloor){
		cvFloorOut[endFloor-1].wait(lck);
	}
	std::cout<<name<<" out!"<<std::endl;
}
void the(){
	th[0]=std::thread(passenger,1,2,"kkk");
	th[1]=std::thread(passenger,3,4,"hhh");
	th[2]=std::thread(passenger,4,5,"sss");
}
void elevator(){
	std::this_thread::sleep_for(std::chrono::seconds(1));
	int i;
	for(i=0;i<NumberOfFloors;i++){
		std::cout<<"elevator: "<<nowFloor<<std::endl;

		mtx_havePeople.lock();mtx_destination.lock();
		if(havePeople[nowFloor-1]==true || destination[nowFloor-1]==true){
			cvFloorOut[nowFloor-1].notify_all();
			cvFloorIn[nowFloor-1].notify_all();
			havePeople[nowFloor-1]=false;
			destination[nowFloor-1]=false;
			std::cout<<"open door!"<<std::endl;
		}
		mtx_havePeople.unlock();mtx_destination.unlock();

		std::this_thread::sleep_for(std::chrono::seconds(2));
		nowFloor++;
	}
	std::cout<<"elevator end\n";
}

int main(){
	int i,k;
	for(i=0;i<10;i++){
		havePeople[i]=false;
		destination[i]=false;
	}

	std::thread mthread(elevator);
	the();
	th[0].join();
	th[1].join();
	th[2].join();
	mthread.join();

	return 0;
}
