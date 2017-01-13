#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#include <mutex>
#include <condition_variable>
#define MAX_FLOOR 5 
using namespace std;
/*shared variable*/
volatile bool waitingFloors[MAX_FLOOR];
volatile bool destinationFloor[MAX_FLOOR];
/*Mutex Lock*/
mutex waitingMux; // lock waitingFloors
mutex desMux;  // lock destination Floor
mutex floorMux[MAX_FLOOR];
condition_variable floorCv[MAX_FLOOR];


void init(){
	for (int i = 0; i < MAX_FLOOR; ++i) {
		waitingFloors[i]=false;
		destinationFloor[i]=false;
	}
}
void printShard(){
	for (int i = 0; i < MAX_FLOOR; ++i) {
		cout << waitingFloors[i] << "     des : " << destinationFloor[i]<<endl;
	}
}

void testFunction(int i){
	unique_lock<mutex> lck(waitingMux);
	std::cout << "p2 locked" << std::endl;
	this_thread::sleep_for(chrono::seconds(3));
	std::cout << "p2 end sleep" << std::endl;

}
void ArrivingGoingFromTo(int from,int to){
	unique_lock<mutex> waitingLock(waitingMux,defer_lock);
	cout << "Create waiting Lock " << endl;
	unique_lock<mutex> floorLck(floorMux[from],defer_lock); // lock floor[from]
	cout << "Create FloorLock " << endl;
	lock(waitingLock,floorLck);
	std::cout << "Enter Section"  << std::endl;
	/*critical section*/
	waitingFloors[from] = true;
	waitingLock.unlock();
	cout << this_thread::get_id() << " is waiting at Floor : " << from << endl;
	floorCv[from].wait(floorLck);
	std::cout << this_thread::get_id() << " end wait. "  << std::endl;
	
}


int main(void){
	init();
	thread p1(ArrivingGoingFromTo,1,2);
	thread p2(testFunction,2);
	p1.join();
	p2.join();
		
	return 0;
}
