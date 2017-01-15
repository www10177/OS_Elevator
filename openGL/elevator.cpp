#include "elevator.h"
elevator::elevator(Vec3 pos){
	nowFloor=1;
	position =  pos;
	speed = 100.0f;
	chromaKey = Vec3(255,255,255);
	static char elevatorLocation[]  = "image/elevator.bmp";
	RGBpixmapController controller;
	nowPixmap = controller.getRGBpixmap(elevatorLocation, chromaKey);
}
	void elevator::getPos(int nowFloor, bool upOrDown){
	this->nowFloor = nowFloor;
	this->upOrDown = upOrDown;
}
void elevator::update(){
	//if(abs(nowFloor*floorHeight - position.y) >= 100 )
		position.y = (nowFloor-1)*floorHeight;
	//if(upOrDown)
		//position.y += speed;
	//else
		//position.y -= speed;

}
void elevator::display(){
	nowPixmap -> blendTex(position.x, position.y, 1);
}
