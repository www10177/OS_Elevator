#include "coreThread.h"

Data::Data(){/*{{{*/
		isDone=false;
		isGoingUp=false;
		isIn=false;
		isOut=false;
}/*}}}*/

void t_passenger(int startFloor,int endFloor){/*{{{*/
	bool isGoingUp;
	int dataVectorPlace=0;
	if(startFloor<endFloor)
		isGoingUp=true;
	else
		isGoingUp=false;

	Data localData;
	localData.startFloor=startFloor;
	localData.endFloor=endFloor;
	localData.isGoingUp=isGoingUp;
	dataVectorPlace=dataVector.size();
	dataVector.push_back(localData);


	if(isGoingUp){
		mtx_havePeople.lock();
		havePeople_up[startFloor-1]=true;
		std::cout<<"["<<std::this_thread::get_id()<<"] at "<<startFloor<<" to "<<endFloor<<std::endl;  //when born
		mtx_havePeople.unlock();

		std::unique_lock <std::mutex> lck(mtx_floor);
		if(nowFloor==startFloor  && targetFloor<endFloor && isGoingUp!=dataVector[nowDataPlace].isGoingUp){
			cvFloorIn_up[startFloor-1].wait(lck);
		}
		else if(nowFloor!=startFloor){
			cvFloorIn_up[startFloor-1].wait(lck);
		}
		mtx_destination.lock();
		destination_up[endFloor-1]=true;
		std::cout<<"["<<std::this_thread::get_id()<<"] INNNNN!!!! from "<<startFloor<<" to "<<endFloor<<std::endl;  //when in
		dataVector[dataVectorPlace].isIn=true;
		mtx_destination.unlock();

		if(nowFloor!=endFloor){
			cvFloorOut_up[endFloor-1].wait(lck);
		}
		std::cout<<"["<<std::this_thread::get_id()<<"] out! from "<<startFloor<<" to "<<endFloor<<std::endl; //when out
		dataVector[dataVectorPlace].isOut=true;
	}
	else{
		mtx_havePeople.lock();
		havePeople_down[startFloor-1]=true;
		std::cout<<"["<<std::this_thread::get_id()<<"] at "<<startFloor<<" to "<<endFloor<<std::endl; //when born
		mtx_havePeople.unlock();

		std::unique_lock <std::mutex> lck(mtx_floor);
	
		if(nowFloor==startFloor && targetFloor>startFloor && isGoingUp != dataVector[nowDataPlace].isGoingUp){
			cvFloorIn_down[startFloor-1].wait(lck);
		}
		else if(nowFloor!=startFloor){
			cvFloorIn_down[startFloor-1].wait(lck);
		}

		mtx_destination.lock();
		destination_down[endFloor-1]=true;
		std::cout<<"["<<std::this_thread::get_id()<<"] INNNNN!!!! from "<<startFloor<<" to "<<endFloor<<std::endl; //when in
		dataVector[dataVectorPlace].isIn=true;
		mtx_destination.unlock();

		if(nowFloor!=endFloor){
			cvFloorOut_down[endFloor-1].wait(lck);
		}
		std::cout<<"["<<std::this_thread::get_id()<<"] out! from "<<startFloor<<" to "<<endFloor<<std::endl; //when out
		dataVector[dataVectorPlace].isOut=true;
	}
	dataVector[dataVectorPlace].isDone=true;
	peopleNum--;
	if(peopleNum==0){
		elevatorIsAwake=false;
	}
}/*}}}*/

void generator(){/*{{{*/
	unsigned seed;
	seed=(unsigned)time(NULL);
	std::default_random_engine generator(seed);
	std::poisson_distribution<int> distribution(5);

	while(1){	
		peopleNum++;
		int startFloor=0;
		int endFloor=0;
		int time;
		while(startFloor>NumberOfFloors || startFloor==0){	
			startFloor = distribution(generator);
		}
		while(endFloor>NumberOfFloors || endFloor==0 || endFloor==startFloor){
			endFloor = distribution(generator);
		}
		time = distribution(generator);
		usleep(time*Time_TIMES_NUM);
		passengerThreads.push_back(std::thread(passenger,startFloor,endFloor));
		elevatorIsAwake=true;
	}
}/*}}}*/

int stairSelect(){/*{{{*/
	for(int i = 0;i<dataVector.size()-1;i++){
		for(int j = 0;j<dataVector.size()-i-1;j++){
			if(!dataVector[i].isIn && dataVector[j].isIn){
				Data tempData;
				tempData = dataVector[i];
				dataVector[i] = dataVector[j];
				dataVector[j] = tempData;
			}
		}
	}
	for(int i = 0;i<dataVector.size();i++){
		if(dataVector[i].isDone){
			continue;
		}
		else{
			if(dataVector[i].isIn && !dataVector[i].isOut){
				dataVector[i].isOut=true;
				nowDataPlace=i;
				return dataVector[i].endFloor;
			}
			else if(!dataVector[i].isIn){
				dataVector[i].isIn=true;
				nowDataPlace=i;
				return dataVector[i].startFloor;
			}
		}
	}
	return 0;
}/*}}}*/

void t_elevator(){/*{{{*/
	while(!elevatorIsAwake){
		usleep(1*Time_TIMES_NUM);
		std::cout<<"Elevator Sleeping"<<std::endl;
	}
	std::cout<<"Elevator Awaken"<<std::endl;
	int i;
	while(peopleNum!=0){
		int targetFloor;
		targetFloor=stairSelect();
		std::cout<<"target = "<<targetFloor<<std::endl;
		if(targetFloor>nowFloor){
			upOrDown=true;
		}
		else if(targetFloor<nowFloor){
			upOrDown=false;
		}
		else if(targetFloor==nowFloor){
			cvFloorOut_up[nowFloor-1].notify_all();
			cvFloorIn_up[nowFloor-1].notify_all();
			havePeople_up[nowFloor-1]=false;
			destination_up[nowFloor-1]=false;
			std::cout<<"open door!"<<std::endl;
		}
		while(!dataVector[nowDataPlace].isDone){
			int initialFloor=nowFloor;
			if(upOrDown==true){
				for(i=0;i<targetFloor-initialFloor+1;i++){
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

					usleep(3*Time_TIMES_NUM);
					nowFloor++;
				}
				nowFloor--;
				if(!dataVector[nowDataPlace].isDone){
					targetFloor = dataVector[nowDataPlace].endFloor;
					std::cout<<"target = "<<targetFloor<<std::endl;
					if(nowFloor>dataVector[nowDataPlace].endFloor){
						upOrDown=false;
					}
				}
				continue;
			}
			else{
				for(i=0;i<initialFloor-targetFloor+1;i++){
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

					usleep(3*Time_TIMES_NUM);
					nowFloor--;
				}
				nowFloor++;
				if(!dataVector[nowDataPlace].isDone){
					targetFloor = dataVector[nowDataPlace].endFloor;
					std::cout<<"target = "<<targetFloor<<std::endl;
					if(nowFloor<dataVector[nowDataPlace].endFloor){
						upOrDown=true;
					}
				}
				continue;
			}
		}
	}
	std::cout<<"elevator end\n";
}/*}}}*/

void t_init(){/*{{{*/
	int i,k;
	for(i=0;i<10;i++){
		havePeople_up[i]=false;
		destination_up[i]=false;
		havePeople_down[i]=false;
		destination_down[i]=false;
	}

	std::thread mthread(t_elevator);
	generator();

	for(k = 0;k<(int)passengerThreads.size();k++){
		passengerThreads[k].join();
	}

	mthread.join();
}/*}}}*/
