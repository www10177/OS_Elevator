#include "elevator.h"
elevator::elevator(){
	position = Vec3(0,0,1);
	chromaKey = Vec3(255,255,255);
	static char elevatorLocation[]  = "image/elevator.bmp";
	RGBpixmapController controller;
	nowPixmap = controller.getRGBpixmap(elevatorLocation, chromaKey);
}
elevator::elevator(Vec3 pos){
	position =  pos;
	chromaKey = Vec3(255,255,255);
	static char elevatorLocation[]  = "image/elevator.bmp";
	RGBpixmapController controller;
	nowPixmap = controller.getRGBpixmap(elevatorLocation, chromaKey);
}
void elevator::display(){
	nowPixmap -> blendTex(position.x, position.y, 1);
}
