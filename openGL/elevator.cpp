#include "elevator.h"
elevator::elevator(Vec3 pos){
	nowFloor=1;
	position =  pos;
	speedPass = 10.0f;
	chromaKey = Vec3(255,255,255);
	static char elevatorLocation[]  = "image/elevator.bmp";
	RGBpixmapController controller;
	nowPixmap = controller.getRGBpixmap(elevatorLocation, chromaKey);
	static char passengerLocation[]  = "image/passenger.bmp";
	RGBpixmapController controller2;
	movePixmap= controller.getRGBpixmap(passengerLocation, chromaKey);
}
void elevator::getPos(int nowFloor, bool upOrDown){
	this->nowFloor = nowFloor;
	this->upOrDown = upOrDown;
}
void elevator::update(){
	if(passOut)
		movPosX -= speedPass;
	if(movPosX <= 0)
		passOut = false;
	position.y = (nowFloor-1)*floorHeight;
}
void elevator::display(){
	if(passOut)
		movePixmap->blendTex(movPosX,position.y,1);
	nowPixmap-> blendTex(position.x, position.y, 1);
}
void elevator::passengerOut(){
	passOut = true;		
	movPosX = position.x;
}
