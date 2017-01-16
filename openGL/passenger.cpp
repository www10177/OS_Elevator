#include "passenger.h"
#define TEXT_X_OFFSET 200
#define TEXT_Y_OFFSET 40
passenger::passenger(Vec3 pos){
	count = 0;
	io = 0;
	position =  pos;
	movPosX = position.x;
	chromaKey = Vec3(255,255,255);
	static char passengerLocation[]  = "image/passenger.bmp";
	RGBpixmapController controller;
	nowPixmap = controller.getRGBpixmap(passengerLocation, chromaKey);
	RGBpixmapController controller2;
	movePixmap = controller.getRGBpixmap(passengerLocation, chromaKey);
	speed = 10.0f;
}

void passenger::display(){
	//Font
	char mss[3];                                                                                                         
	sprintf(mss, "%d", count);
	glColor3f(0.8, 1.0, 0.0);  //set font color
	glRasterPos2i(position.x+TEXT_X_OFFSET, position.y+TEXT_Y_OFFSET);    //set font start position
	for(int i=0; i<strlen(mss); i++) {
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, mss[i]);
	} 
	if(io == 1)
		movePixmap ->blendTex(movPosX,position.y,1);
		
	if(count==0)
		return;
	nowPixmap -> blendTex(position.x, position.y, 1);
}
void passenger::updateP(){
	if(movPosX <= itemXLocation[0])
		io =0;
	if(io ==1){
		movPosX -= speed;
	}
	else 
		movPosX = position.x;

}
void passenger::setIO(int IO){
	io = IO;
	if (io == 1)
		count --;
}

