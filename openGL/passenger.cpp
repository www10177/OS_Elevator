#include "passenger.h"
passenger::passenger(){
	position = Vec3(200,0,1);
	chromaKey = Vec3(255,255,255);
	static char passengerLocation[]  = "image/passenger.bmp";
	RGBpixmapController controller;
	nowPixmap = controller.getRGBpixmap(passengerLocation, chromaKey);
}
passenger::passenger(Vec3 pos){
	position =  pos;
	chromaKey = Vec3(255,255,255);
	static char passengerLocation[]  = "image/passenger.bmp";
	RGBpixmapController controller;
	nowPixmap = controller.getRGBpixmap(passengerLocation, chromaKey);
}
void passenger::display(){
	nowPixmap -> blendTex(position.x, position.y, 1);
}
