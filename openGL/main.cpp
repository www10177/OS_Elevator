#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <windows.h>
//#include <GL/glew.h>
#include <GL/glut.h>
#endif

#include "Timer.h"
#include "elevator.h"
#include "Vec.h"
#include "passenger.h"

/*thread*/
#include <iostream>                // std::cout/*{{{*/
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
#define Time_TIMES_NUM 1000000/*}}}*/

/*THREAD*/
class Data{/*{{{*/
	public:
		int startFloor;
		int endFloor;
		bool isDone;
		bool isGoingUp;
		bool isIn;
		bool isOut;
		Data();
};
std::mutex mtx_havePeople;
std::mutex mtx_destination;
std::mutex mtx_floor;
std::mutex mtx_Elevator;

std::condition_variable cvFloorIn_up[NumberOfFloors];
std::condition_variable cvFloorOut_up[NumberOfFloors];
std::condition_variable cvFloorIn_down[NumberOfFloors];
std::condition_variable cvFloorOut_down[NumberOfFloors];
std::condition_variable cvElevatorIsAwake;

std::thread m_thread;

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
std::vector<Data> dataVector;/*}}}*/


/*CONFIGS*/
const int GAME_ONE_SHOT_TIME = 33;
int screenWidth = 800 , screenHeight = 800;
int floorHeight = screenHeight / NumberOfFloors;
const float floorColor1[3] = {153.0/255,101.0/255,70.0/255};
const float floorColor2[3] = {183.0/255,131.0/255,100.0/255};
float itemXLocation[2] = {200,400};

/*VARIABLES*/
elevator* mainElevator; 
passenger* passengers[NumberOfFloors];
thread mainThread;


/*FUNCTIONS*/
void myDisplay();
void updateTheGame(int value);
void myReshape(int w, int h);
void drawFloor();
void init();
void keys(unsigned char key, int x, int y);
/*Thread*/
void t_passenger(int start, int end);
void generator();
int stairSelect();
void t_elevator();
void t_init();


int main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(screenWidth, screenHeight);
	glutInitWindowPosition(50, 30);
	glutCreateWindow("OS Elevator Problem");
	init();
	updateTheGame(0);
	glutMainLoop();
	return 0;
}

void myDisplay(void) {/*{{{*/
	glClear(GL_COLOR_BUFFER_BIT);
	drawFloor();
	for (int i = 0; i < NumberOfFloors; ++i) {
	passengers[i]->display();
	}
	mainElevator->display();
	glutSwapBuffers();
}/*}}}*/

void updateTheGame(int value) { /*{{{*/
	glutPostRedisplay();
	mainElevator->update();
	for (int i = 0; i < NumberOfFloors; ++i) {
	passengers[i]->updateP();
	}
	glutTimerFunc(GAME_ONE_SHOT_TIME, updateTheGame, 0);
}/*}}}*/

void drawFloor(){/*{{{*/
	for (int i = 0; i <NumberOfFloors ; ++i) {
		if(i%2)
			glColor3f(floorColor1[0],floorColor1[1],floorColor1[2]);
		else
			glColor3f(floorColor2[0],floorColor2[1],floorColor2[2]);
		glRectf(0,floorHeight*i,screenWidth,floorHeight*(1+i));
	}
}/*}}}*/

void init() {/*{{{*/
	/*openGL init*/
	glutDisplayFunc(myDisplay);
	glutReshapeFunc(myReshape);
	glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
	glutKeyboardFunc(keys);


	/*Var Init*/
	mainElevator= new elevator(Vec3(itemXLocation[0],0,1));
	for (int i = 0; i < NumberOfFloors; ++i) {
	passengers[i]= new passenger(Vec3(itemXLocation[1],0+i *floorHeight,1));
	}
}/*}}}*/

void myReshape(int w, int h) {/*{{{*/
	screenWidth  = w;
	screenHeight = h;
	glViewport(0, 0, screenWidth, screenHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, (GLfloat)screenWidth, 0.0, (GLfloat)screenHeight, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}/*}}}*/

void keys(unsigned char key, int x, int y) {/*{{{*/
	switch(key)
	{
		case ' ':
			mainThread = thread(t_init);
			break;
	} //switch(key)
	glutPostRedisplay();
}
/*}}}*/



/*Thread Functions*/
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
		passengers[startFloor-1]->count++;
		std::cout<<"BORN : ["<<std::this_thread::get_id()<<"] at "<<startFloor<<" to "<<endFloor<<std::endl;  //when born
		mtx_havePeople.unlock();

		std::unique_lock <std::mutex> lck(mtx_floor);
		cvFloorIn_up[startFloor-1].wait(lck);

		mtx_destination.lock();
		destination_up[endFloor-1]=true;
		passengers[startFloor-1]->setIO(1);
		std::cout<<"["<<std::this_thread::get_id()<<"] INNNNN!!!! from "<<startFloor<<" to "<<endFloor<<std::endl;  //when in
		dataVector[dataVectorPlace].isIn=true;
		mtx_destination.unlock();

		if(nowFloor!=endFloor){
			cvFloorOut_up[endFloor-1].wait(lck);
		}
		mainElevator ->passengerOut();
		std::cout<<"["<<std::this_thread::get_id()<<"] out! from "<<startFloor<<" to "<<endFloor<<std::endl; //when out
		dataVector[dataVectorPlace].isOut=true;
	}
	else{
		mtx_havePeople.lock();
		havePeople_down[startFloor-1]=true;
		passengers[startFloor-1]->count++;
		std::cout<<"BORN : ["<<std::this_thread::get_id()<<"] at "<<startFloor<<" to "<<endFloor<<std::endl; //when born
		mtx_havePeople.unlock();

		std::unique_lock <std::mutex> lck(mtx_floor);
		cvFloorIn_down[startFloor-1].wait(lck);

		mtx_destination.lock();
		destination_down[endFloor-1]=true;
		passengers[startFloor-1]->setIO(1);
		std::cout<<"["<<std::this_thread::get_id()<<"] INNNNN!!!! from "<<startFloor<<" to "<<endFloor<<std::endl; //when in
		dataVector[dataVectorPlace].isIn=true;
		mtx_destination.unlock();

		if(nowFloor!=endFloor){
			cvFloorOut_down[endFloor-1].wait(lck);
		}
		mainElevator ->passengerOut();
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
	std::poisson_distribution<int> distribution(3);

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
		std::this_thread::sleep_for(std::chrono::microseconds(time*Time_TIMES_NUM));
		//usleep(time*Time_TIMES_NUM);
		passengerThreads.push_back(std::thread(t_passenger,startFloor,endFloor));
		elevatorIsAwake=true;
	}
}/*}}}*/
int stairSelect(){/*{{{*/
	for(int i = 0;i<dataVector.size();i++){
		if(dataVector[i].isDone){
			continue;
		}
		else{
			if(dataVector[i].isIn && !dataVector[i].isOut){
				dataVector[i].isOut=true;
				nowDataPlace=i;
				if(dataVector[i].endFloor<nowFloor){
					dataVector[i].isGoingUp=true;
				}
				else{
					dataVector[i].isGoingUp=false;
				}
				return dataVector[i].endFloor;
			}
			else if(!dataVector[i].isIn){
				dataVector[i].isIn=true;
				nowDataPlace=i;
				if(dataVector[i].startFloor<nowFloor){
					dataVector[i].isGoingUp=true;
				}               
				else{           
					dataVector[i].isGoingUp=false;
				}
				return dataVector[i].startFloor;
			}
		}
	}
	return 0;
}/*}}}*/
void t_elevator(){/*{{{*/
	while(!elevatorIsAwake){
		//usleep(1*Time_TIMES_NUM);
		std::this_thread::sleep_for(std::chrono::microseconds(Time_TIMES_NUM));
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
					mainElevator->getPos(nowFloor,upOrDown);
					std::cout << " elevator: "<<nowFloor<<std::endl; 
					mtx_havePeople.lock();mtx_destination.lock();
					if(havePeople_up[nowFloor-1]==true || destination_up[nowFloor-1]==true){
						cvFloorOut_up[nowFloor-1].notify_all();
						cvFloorIn_up[nowFloor-1].notify_all();
						havePeople_up[nowFloor-1]=false;
						destination_up[nowFloor-1]=false;
						std::cout<<"open door!"<<std::endl;
					}
					mtx_havePeople.unlock();mtx_destination.unlock();

					//usleep(3*Time_TIMES_NUM);
					std::this_thread::sleep_for(std::chrono::microseconds(1*Time_TIMES_NUM));
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
					mainElevator->getPos(nowFloor,upOrDown);
					std::cout<< " elevator: "<<nowFloor<<std::endl; 
					mtx_havePeople.lock();mtx_destination.lock();
					if(havePeople_down[nowFloor-1]==true || destination_down[nowFloor-1]==true){
						cvFloorOut_down[nowFloor-1].notify_all();
						cvFloorIn_down[nowFloor-1].notify_all();
						havePeople_down[nowFloor-1]=false;
						destination_down[nowFloor-1]=false;
						std::cout<<"open door!"<<std::endl;
					}
					mtx_havePeople.unlock();mtx_destination.unlock();

					//usleep(3*Time_TIMES_NUM);
					std::this_thread::sleep_for(std::chrono::microseconds(1*Time_TIMES_NUM));
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
void t_init(){/*{{{*//*{{{*/
	int i,k;/*}}}*/
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
