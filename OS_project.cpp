#include <iostream>                // std::cout
#include <thread>                // std::thread
#include <mutex>                // std::mutex, std::unique_lock
#include <condition_variable>    // std::condition_variable
#include <string>
#include <chrono>
#define NumberOfFloors 10
using namespace std;
mutex mtx_havePeople;
mutex mtx_destination;
mutex mtx_floor;

condition_variable cvFloorIn[NumberOfFloors];
condition_variable cvFloorOut[NumberOfFloors];

bool havePeople[NumberOfFloors];
bool destination[NumberOfFloors];
int nowFloor=1;
thread th[3];

void passenger(int startFloor,int endFloor,string name){

	mtx_havePeople.lock();
	havePeople[startFloor-1]=true;
	cout<<name<<" at "<<startFloor<<" and wait elevator"<<endl;
	mtx_havePeople.unlock();

	unique_lock <mutex> lck(mtx_floor);
	//if(nowFloor!=startFloor){
		cvFloorIn[startFloor-1].wait(lck);
	//}

	mtx_destination.lock();
	destination[endFloor-1]=true;
	cout<<name<<" in and want go to "<<endFloor<<endl;
	mtx_destination.unlock();

	if(nowFloor!=endFloor){
		cvFloorOut[endFloor-1].wait(lck);
	}
	cout<<name<<" out!"<<endl;
}
void the(){
	th[0]= thread(passenger,1,2,"kkk");
	th[1]=thread(passenger,3,4,"hhh");
	th[2]=thread(passenger,4,5,"sss");
}
void elevator(){
	this_thread::sleep_for(chrono::seconds(1));
	int i;
	for(i=0;i<NumberOfFloors;i++){
		cout<<"elevator: "<<nowFloor<<endl;

		mtx_havePeople.lock();mtx_destination.lock();
		if(havePeople[nowFloor-1]==true || destination[nowFloor-1]==true){
			cvFloorOut[nowFloor-1].notify_all();
			cvFloorIn[nowFloor-1].notify_all();
			havePeople[nowFloor-1]=false;
			destination[nowFloor-1]=false;
			cout<<"open door!"<<endl;
		}
		mtx_havePeople.unlock();mtx_destination.unlock();

		this_thread::sleep_for(chrono::seconds(2));
		nowFloor++;
	}
	cout<<"elevator end\n";
}

int main(){
	int i,k;
	for(i=0;i<10;i++){
		havePeople[i]=false;
		destination[i]=false;
	}

	thread mthread(elevator);
	the();
	th[0].join();
	th[1].join();
	th[2].join();
	mthread.join();

	return 0;
}
