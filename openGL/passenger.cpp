#include "passenger.h"
#define TEXT_X_OFFSET 200
#define TEXT_Y_OFFSET 40
passenger::passenger(Vec3 pos){
	count = 0;
	goOut = false;
	goIn = false;
	position =  pos;
	chromaKey = Vec3(255,255,255);
	static char passengerLocation[]  = "image/passenger.bmp";
	RGBpixmapController controller;
	nowPixmap = controller.getRGBpixmap(passengerLocation, chromaKey);
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
	if(count==0)
		return;
	nowPixmap -> blendTex(position.x, position.y, 1);
}
void passenger::updateP(){

}
