#include "passenger.h"
#define TEXT_Y_OFFSET 40
passenger::passenger(Vec3 pos,int offset,Vec3 c){
	count = 0;
	io = 0;
	position =  pos;
	movPosX = position.x;
	textOffsetX = offset;
	this->color = c;
	chromaKey = Vec3(255,255,255);
	static char passUpLoc[]  = "image/passengerG.bmp";
	static char passDownLoc[]  = "image/passengerR.bmp";
	RGBpixmapController controller[4];
	PixmapPic[0] = controller[0].getRGBpixmap(passUpLoc, chromaKey);
	PixmapPic[1] = controller[1].getRGBpixmap(passDownLoc, chromaKey);
	movePixmapPic[0] = controller[2].getRGBpixmap(passUpLoc, chromaKey);
	movePixmapPic[1] = controller[3].getRGBpixmap(passDownLoc, chromaKey);
	speed = 10.0f;
}

void passenger::display(){
	//Font
	char mss[3];                                                                                                         
	sprintf(mss, "%d", count);
	glColor3f(color.x,color.y,color.z);  //set font color
	glRasterPos2i(position.x+textOffsetX, position.y+TEXT_Y_OFFSET);    //set font start position
	for(int i=0; i<strlen(mss); i++) {
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, mss[i]);
	} 
	if(io == 1)
		movePixmapPic[isGoingup?0:1]->blendTex(movPosX+(isGoingup?-1*offset:offset),position.y,1);//print moving ball

	if(count==0)
		return;
		PixmapPic[isGoingup?0:1]->blendTex(movPosX+(isGoingup?-1*offset:offset),position.y,1);//print baseball
}
void passenger::updateP(){
	if(movPosX <=itemXLocation[0]+offset)
		io =0;
	if(io ==1){
		movPosX -= speed;
	}
	else 
		movPosX = position.x;

}
void passenger::setIO(int IO,bool isUp){
	io = IO;
	isGoingup = isUp;
	if(io == 0)
		count++;
	if (io == 1)
		count --;
}

