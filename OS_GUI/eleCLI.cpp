#include"eleCLI.h"
void passenger(int startFloor,int endFloor,std::string name){

    if(startFloor<endFloor){

        mtx_havePeople.lock();
        havePeople_up[startFloor-1]=true;
        std::cout<<name<<" at "<<startFloor<<" and wait elevator"<<std::endl;
        mtx_havePeople.unlock();

        std::unique_lock <std::mutex> lck(mtx_floor);
        cvFloorIn_up[startFloor-1].wait(lck);

        mtx_destination.lock();
        destination_up[endFloor-1]=true;
        std::cout<<name<<" in and want go to "<<endFloor<<std::endl;
        mtx_destination.unlock();

        if(nowFloor!=endFloor){
            cvFloorOut_up[endFloor-1].wait(lck);
        }
        std::cout<<name<<" out!"<<std::endl;
    }
    else{

        mtx_havePeople.lock();
        havePeople_down[startFloor-1]=true;
        std::cout<<name<<" at "<<startFloor<<" and wait elevator"<<std::endl;
        mtx_havePeople.unlock();

        std::unique_lock <std::mutex> lck(mtx_floor);
        cvFloorIn_down[startFloor-1].wait(lck);

        mtx_destination.lock();
        destination_down[endFloor-1]=true;
        std::cout<<name<<" in and want go to "<<endFloor<<std::endl;
        mtx_destination.unlock();

        if(nowFloor!=endFloor){
            cvFloorOut_down[endFloor-1].wait(lck);
        }
        std::cout<<name<<" out!"<<std::endl;
    }
}
void the(){
    th[0]=std::thread(passenger,1,2,"kkk");
    th[1]=std::thread(passenger,3,4,"hhh");
    th[2]=std::thread(passenger,6,3,"sss");
}
void elevator(){
    std::this_thread::sleep_for(std::chrono::seconds(1));
    int i;
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

            std::this_thread::sleep_for(std::chrono::seconds(2));
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

            std::this_thread::sleep_for(std::chrono::seconds(2));
            nowFloor--;
        }
    }
    std::cout<<"elevator end\n";
}
void init(){
    int i;
    for(i=0;i<10;i++){
        havePeople_up[i]=false;
        destination_up[i]=false;
        havePeople_down[i]=false;
        destination_down[i]=false;
    }
    std::thread mthread(elevator);
    the();
    th[0].join();
    th[1].join();
    th[2].join();
    mthread.join();
}
