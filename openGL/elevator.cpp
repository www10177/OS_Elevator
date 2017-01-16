#include "elevator.h"
elevator::elevator(Vec3 pos){
	nowFloor=1;
	position =  pos;
	speedPass = 10.0f;
	chromaKey = Vec3(255,255,255);
	static char elevatorLocation[]  = "image/elevator.bmp";
	RGBpixmapController controller;
	nowPixmap = controller.getRGBpixmap(elevatorLocation, chromaKey);
	static char passengerLocationG[]  = "image/passengerG.bmp";
	static char passengerLocationR[]  = "image/passengerR.bmp";
	movePixmap[0]= controller.getRGBpixmap(passengerLocationG, chromaKey);
	movePixmap[1]= controller.getRGBpixmap(passengerLocationR, chromaKey);
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
	if(passOut){
		if(isUp)
		movePixmap[0]->blendTex(movPosX,position.y,1);
		else
		movePixmap[1]->blendTex(movPosX,position.y,1);
	}
	nowPixmap-> blendTex(position.x, position.y, 1);
}
void elevator::passengerOut(bool Up){
	passOut = true;		
	isUp = Up;
	movPosX = position.x;
}
