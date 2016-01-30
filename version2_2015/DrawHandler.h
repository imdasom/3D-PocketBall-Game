/*
* 파일이름 : DrawHandler.h
* 작 성 자 : 임다솜
* 업데이트 정보 : [2015. 10. 23]
*/

#ifndef __DRAW_HANDLER_H__
#define __DRAW_HANDLER_H__

#pragma once

#include "global.h"
class Ball;
class Constants;
class Calculator;
class CueHandler;
class BallHandler;
class DrawHandler;

class DrawHandler {
private:
	Ball** b;
	Calculator* clt;
	CueHandler* cHandler;
	Constants* c;
	GLuint Texture[7];
	GLUquadricObj* obj;

public:
	DrawHandler(Constants* _c);
	void setBall(Ball** _b);
	void setCalculator(Calculator* _clt);
	void setCueHandler(CueHandler* _cHandler);
	void Draw_PoolTable();
	void Draw_Ball();
	void Draw_PooledBall();
	void drawViewPort1();
	void drawViewPort2();
	AUX_RGBImageRec* LoadBMP(char *Filename);
	void loadTexture(void);
	void renderSphere(float cx, float cy, float cz, float r, int p);
};

DrawHandler::DrawHandler(Constants* _c) {
	c = _c;
	loadTexture();
	obj = gluNewQuadric();
	gluQuadricDrawStyle(obj, GLU_FILL);
}

void DrawHandler::setBall(Ball** _b) {
	b = _b;
}

void DrawHandler::setCalculator(Calculator* _clt) {
	clt = _clt;
}

void DrawHandler::setCueHandler(CueHandler* _cHandler) {
	cHandler = _cHandler;
}

//당구대 그리기
void DrawHandler::Draw_PoolTable() {
	glPushMatrix();
	glEnable(GL_COLOR_MATERIAL);
	glPushMatrix();

	glColor3f(0.1, 0.4, 0.7);  //파란색(당구대바닥)
	glBegin(GL_POLYGON);
	glVertex3f(c->leftPos, c->topPos, 0.0);		//좌상단 
	glVertex3f(c->rightPos, c->topPos, 0.0);		//우상단
	glVertex3f(c->rightPos, c->bottomPos, 0.0);	//우하단
	glVertex3f(c->leftPos, c->bottomPos, 0.0);	//좌하단
	glEnd();

	glPopMatrix();

	glPushMatrix();
	glColor3f(0.1, 0.4, 0.7);  //파란색다이 //왼쪽 다이
	glTranslatef(c->leftPos - 0.05, 0, 0.05);
	glScalef(1, 42, 1);
	glutSolidCube(0.1);
	glColor3f(0, 0, 0);
	glutWireCube(0.1);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.1, 0.4, 0.7);  //오른쪽 다이
	glTranslatef(c->rightPos + 0.05, 0, 0.05);
	glScalef(1, 42, 1);
	glutSolidCube(0.1);
	glColor3f(0, 0, 0);
	glutWireCube(0.1);
	glPopMatrix();

	glColor3f(0.1, 0.4, 0.7);  //아래다이
	glTranslatef(0, c->bottomPos - 0.05, 0.05);
	glScalef(80, 1, 1);
	glutSolidCube(0.1);
	glColor3f(0, 0, 0);
	glutWireCube(0.1);
	glPopMatrix();

	glColor3f(0.1, 0.4, 0.7); //위쪽다이
	glTranslatef(0.0, c->topPos + 0.05, 0.05);
	glScalef(80, 1, 1);
	glutSolidCube(0.1);
	glColor3f(0, 0, 0);
	glutWireCube(0.1);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.3, 0.2, 0.0); //갈색(다이) //왼쪽다이
	glTranslatef(c->leftPos - 0.2, 0, 0.05);
	glScalef(2, 46, 1);
	glutSolidCube(0.1);
	glColor3f(0, 0, 0);
	glutWireCube(0.1);
	glPopMatrix();

	glPushMatrix();  //아래다이
	glColor3f(0.3, 0.2, 0.0);
	glTranslatef(0, c->bottomPos - 0.2, 0.05);
	glScalef(82, 2, 1);
	glutSolidCube(0.1);
	glColor3f(0, 0, 0);
	glutWireCube(0.1);
	glPopMatrix();

	glPushMatrix(); //위쪽다이
	glColor3f(0.3, 0.2, 0.0);
	glTranslatef(0, c->topPos + 0.2, 0.05);
	glScalef(82, 2, 1);
	glutSolidCube(0.1);
	glColor3f(0, 0, 0);
	glutWireCube(0.1);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.3, 0.2, 0.0); //오른쪽 다이
	glTranslatef(c->rightPos + 0.2, 0, 0.05);
	glScalef(2, 46, 1);
	glutSolidCube(0.1);
	glColor3f(0, 0, 0);
	glutWireCube(0.1);
	glPopMatrix();



	glColor3f(0.5, 0.5, 0.5);
	glPushMatrix(); //위다이 흰점찍기
	glTranslatef(0.0, c->topPos + 0.25, 0.0);
	gluDisk(obj, 0.0, 0.015, 10, 10);

	glTranslatef(1.0, 0.0, 0.0);
	gluDisk(obj, 0.0, 0.015, 10, 10);

	glTranslatef(1.0, 0.0, 0.0);
	gluDisk(obj, 0.0, 0.015, 10, 10);

	glTranslatef(1.0, 0.0, 0.0);
	gluDisk(obj, 0.0, 0.015, 10, 10);

	glTranslatef(1.0, 0.0, 0.0);
	gluDisk(obj, 0.0, 0.015, 10, 10);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.0, c->topPos + 0.25, 0.0);
	gluDisk(obj, 0.0, 0.015, 10, 10);

	glTranslatef(-1.0, 0.0, 0.0);
	gluDisk(obj, 0.0, 0.015, 10, 10);

	glTranslatef(-1.0, 0.0, 0.0);
	gluDisk(obj, 0.0, 0.015, 10, 10);

	glTranslatef(-1.0, 0.0, 0.0);
	gluDisk(obj, 0.0, 0.015, 10, 10);
	glPopMatrix();

	glPushMatrix(); //아래다이 흰점찍기
	glTranslatef(0.0, c->bottomPos - 0.25, 0.0);
	gluDisk(obj, 0.0, 0.015, 10, 10);

	glTranslatef(1.0, 0.0, 0.0);
	gluDisk(obj, 0.0, 0.015, 10, 10);

	glTranslatef(1.0, 0.0, 0.0);
	gluDisk(obj, 0.0, 0.015, 10, 10);

	glTranslatef(1.0, 0.0, 0.0);
	gluDisk(obj, 0.0, 0.015, 10, 10);

	glTranslatef(1.0, 0.0, 0.0);
	gluDisk(obj, 0.0, 0.015, 10, 10);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.0, c->bottomPos - 0.25, 0.0);
	gluDisk(obj, 0.0, 0.015, 10, 10);

	glTranslatef(-1.0, 0.0, 0.0);
	gluDisk(obj, 0.0, 0.015, 10, 10);

	glTranslatef(-1.0, 0.0, 0.0);
	gluDisk(obj, 0.0, 0.015, 10, 10);

	glTranslatef(-1.0, 0.0, 0.0);
	gluDisk(obj, 0.0, 0.015, 10, 10);
	glPopMatrix();

	glPushMatrix(); //왼쪽다이 흰점찍기
	glTranslatef(c->leftPos - 0.25, 0.0, 0.0);
	gluDisk(obj, 0.0, 0.015, 10, 10);

	glTranslatef(0.0, 1.0, 0.0);
	gluDisk(obj, 0.0, 0.015, 10, 10);

	glTranslatef(0.0, 1.0, 0.0);
	gluDisk(obj, 0.0, 0.015, 10, 10);

	glPopMatrix();

	glPushMatrix();
	glTranslatef(c->leftPos - 0.25, -1.0, 0.0);
	gluDisk(obj, 0.0, 0.015, 10, 10);

	glTranslatef(0.0, -1.0, 0.0);
	gluDisk(obj, 0.0, 0.015, 10, 10);
	glPopMatrix();

	glPushMatrix(); //오른쪽다이 흰점찍기
	glTranslatef(c->rightPos + 0.25, 0.0, 0.0);
	gluDisk(obj, 0.0, 0.015, 10, 10);

	glTranslatef(0.0, 1.0, 0.0);
	gluDisk(obj, 0.0, 0.015, 10, 10);

	glTranslatef(0.0, 1.0, 0.0);
	gluDisk(obj, 0.0, 0.015, 10, 10);

	glPopMatrix();

	glPushMatrix();
	glTranslatef(c->rightPos + 0.25, -1.0, 0.0);
	gluDisk(obj, 0.0, 0.015, 10, 10);

	glTranslatef(0.0, -1.0, 0.0);
	gluDisk(obj, 0.0, 0.015, 10, 10);
	glPopMatrix();


	glColor3f(0.0, 0.0, 0.0); //검은색(포켓)
	glPushMatrix();
	glTranslatef(c->leftPos - 0.05, c->topPos + 0.05, 0); //좌상단부터 시계방향
	glutSolidSphere(0.13, 40, 40);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0.0, c->topPos + 0.05, 0);
	glutSolidSphere(0.13, 40, 40);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(c->rightPos + 0.05, c->topPos + 0.05, 0);
	glutSolidSphere(0.13, 40, 40);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(c->rightPos + 0.05, c->bottomPos - 0.05, 0);
	glutSolidSphere(0.13, 40, 40);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0.0, c->bottomPos - 0.05, 0);
	glutSolidSphere(0.13, 40, 40);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(c->leftPos - 0.05, c->bottomPos - 0.05, 0);
	glutSolidSphere(0.13, 40, 40);
	glPopMatrix();

}

//1번 뷰포트의 공그리기
void DrawHandler::Draw_Ball() {
	glPushMatrix();
	glEnable(GL_COLOR_MATERIAL);
	glColor3f(1.0, 1.0, 1.0);
	glEnable(GL_TEXTURE_2D);

	switch (c->perspective) {
	case ALL:
		for (int i = 0; i<LINE; i++) {
			if (i == CUE) {
				glPushMatrix();
				glTranslatef((GLfloat)b[i]->posX, (GLfloat)b[i]->posY, 0.05);
				glRotatef(b[i]->dAngle, 0.0, 0.0, 1.0);
				if (b[i]->wherePooled == 0) {
					if (cHandler->cuepower >= -(cHandler->ccp)) {
						glColor3f(0.5, 1.0, 1.0);
						glScalef(0.4, 14.0, 0.4);
						glutSolidCube(0.05);
					}
				}
				glPopMatrix();
			}
			else if (i != CUE) {
				glPushMatrix();
				glTranslatef((GLfloat)b[i]->posX, (GLfloat)b[i]->posY, 0.05);
				if (c->isShooted == 1)
					glRotatef(b[i]->dAngle, b[i]->axisX, b[i]->axisY, 0.0); //공이 굴러가는 회전축이다.
				if (b[i]->wherePooled == 0) {
					glBindTexture(GL_TEXTURE_2D, Texture[i]);
					renderSphere(0, 0, 0, 0.05, 8);
				}
				glPopMatrix();
			}
		}
		break;

	case ZOOM:
		for (int k = 0; k<LINE; k++) b[CUE]->collisionCount[k] = 0;
		//그릴떄마다 순서가 달라지므로 항상 0으로초기화하고시작한다.
		int maxBall = 1; //일단 1번 공으로 초기화해놓는다.
		GLfloat maxDis = clt->Get_Distance(GET_SKEWED_DIS, 0, 1);
		for (int i = 1; i<CUE; i++) {  //큐는 맨 나중에 그린다.
			glPushMatrix();
			for (int j = 1; j<CUE; j++) { //0번부터 6번공까지의 거리를 검사해서 가장 가까운 공을 찾는다.
				GLfloat dis = clt->Get_Distance(GET_SKEWED_DIS, 0, j);
				if (dis >= maxDis) {
					if (b[CUE]->collisionCount[j] != 1) {
						maxBall = j;
						maxDis = dis;
					}
				}
			}
			b[CUE]->collisionCount[maxBall] = 1;
			maxDis = 0.0;
			glTranslatef((GLfloat)b[maxBall]->posX, (GLfloat)b[maxBall]->posY, 0.05);
			if (c->isShooted == 1)
				glRotatef(b[maxBall]->dAngle, b[maxBall]->axisX, b[maxBall]->axisY, 0.0); //공이 굴러가는 회전축이다.
			if (b[maxBall]->wherePooled == 0) { //wherePooled가 0이면 포켓에 들어가지 않은 것이므로 공을 그린다.
				glBindTexture(GL_TEXTURE_2D, Texture[maxBall]);
				renderSphere(0, 0, 0, 0.05, 8);
			}
			glPopMatrix();
		}
		//수구를 그린다.
		glPushMatrix();
		b[CUE]->collisionCount[0] = 1;
		glTranslatef((GLfloat)b[0]->posX, (GLfloat)b[0]->posY, 0.05);
		if (c->isShooted == 1)
			glRotatef(b[0]->dAngle, b[0]->axisX, b[0]->axisY, 0.0); //공이 굴러가는 회전축이다.
		if (b[maxBall]->wherePooled == 0) { //wherePooled가 0이면 포켓에 들어가지 않은 것이므로 공을 그린다.
			glBindTexture(GL_TEXTURE_2D, Texture[0]);
			renderSphere(0, 0, 0, 0.05, 8);
		}
		glPopMatrix();
		//큐를 그린다.
		glPushMatrix();
		glTranslatef((GLfloat)b[CUE]->posX, (GLfloat)b[CUE]->posY, 0.05);
		glRotatef(b[CUE]->dAngle, 0.0, 0.0, 1.0);
		if (b[CUE]->wherePooled == 0) { //wherePooled가 0이면 포켓에 들어가지 않은 것이므로 공을 그린다.
			if (cHandler->cuepower >= -(cHandler->ccp)) {
				glColor3f(0.5, 1.0, 1.0);
				glScalef(0.4, 14.0, 0.4);
				glutSolidCube(0.05);
			}
		}
		glPopMatrix();
		break;
	}
	if (c->isShooted != 1) cHandler->DrawCueLine();
}

//2번 뷰포트에 공그리기
void DrawHandler::Draw_PooledBall() {
	glEnable(GL_TEXTURE_2D);
	glColor3f(0.0, 0.0, 0.0);
	glPushMatrix();

	int posx = -1.0;
	for (int i = 1; i<CUE; i++) {
		glPushMatrix();
		glEnable(GL_LIGHT0);
		glColor3f(1.0, 1.0, 1.0);
		if (b[i]->wherePooled != 0) {
			glPushMatrix();
			glTranslatef(posx, 0.0, 0.0);
			glBindTexture(GL_TEXTURE_2D, Texture[i]);
			renderSphere(0, 0, 0, 0.1, 20);
			glPopMatrix();
		}
		glPopMatrix();
		posx += 0.4;
	}

	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
}

void DrawHandler::drawViewPort1() {

	glMatrixMode(GL_PROJECTION);
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();
	glOrtho((GLfloat)c->orthoLeft, (GLfloat)c->orthoRight, (GLfloat)c->orthoBottom, (GLfloat)c->orthoTop, -4.0, 4.0);
	gluPerspective(50, 1.0, 0.0, -50.0);
	glViewport(0, 100, 1800, 900);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	if (c->perspective == ALL) {
		glPushMatrix();
		c->eyeX = 0.0;
		c->eyeY = -6.7800286e-008;
		c->eyeZ = 3.5;
		gluLookAt(c->eyeX, c->eyeY, c->eyeZ, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0);
		glRotatef(c->RotateDisplay, 0.0, 0.0, 1.0);
	}
	else if (c->perspective == ZOOM) {
		glPushMatrix();
		c->eyeX = b[CUE]->posX;
		c->eyeY = b[CUE]->posY;
		c->eyeZ = 0.2;
		c->centerX = b[0]->posX;
		c->centerY = b[0]->posY;
		c->centerZ = 0.2;
		//glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	//원근에 따른 텍스쳐보정. P600
		gluLookAt(c->eyeX, c->eyeY, c->eyeZ, c->centerX, c->centerY, c->centerZ, 0.0, 0.0, 1.0);
	}
	glPushMatrix();

	//당구대와 공을 그립니다.
	Draw_PoolTable();
	Draw_Ball();

	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
}

void DrawHandler::drawViewPort2() {
	glPushMatrix();
	glViewport(0, 0, 1800, 100);
	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_POLYGON);
	glVertex3f(-1.0, 1.0, 0.0);
	glVertex3f(1.0, 1.0, 0.0);
	glVertex3f(1.0, -1.0, 0.0);
	glVertex3f(-1.0, -1.0, 0.0);
	glEnd();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-2.4, 2.4, -0.17, 0.17, -1.0, 1.0);
	Draw_PooledBall();
}

AUX_RGBImageRec* DrawHandler::LoadBMP(char *Filename)								//BMP파일을 불러오는 함수 
{
	FILE *File = NULL;

	if (!Filename) return NULL;
	fopen_s(&File, Filename, "r");
	if (File) {
		fclose(File);
		return auxDIBImageLoad(Filename);
	}
	return NULL;
}

void DrawHandler::loadTexture(void)													//TEXTURE를 위한 함수
{
	AUX_RGBImageRec *texRec[10];
	memset(texRec, 0, sizeof(void *) * 1);

	if ((texRec[0] = LoadBMP("bnum0.bmp")) &&
		(texRec[1] = LoadBMP("bnum1.bmp")) &&
		(texRec[2] = LoadBMP("bnum2.bmp")) &&
		(texRec[3] = LoadBMP("bnum3.bmp")) &&
		(texRec[4] = LoadBMP("bnum4.bmp")) &&
		(texRec[5] = LoadBMP("bnum5.bmp")) &&
		(texRec[6] = LoadBMP("bnum6.bmp")))
	{
		for (int i = 0; i<7; i++)
		{
			glGenTextures(1, &Texture[i]);
			glBindTexture(GL_TEXTURE_2D, Texture[i]);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glTexImage2D(GL_TEXTURE_2D, 0, 3, texRec[i]->sizeX, texRec[i]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, texRec[i]->data);
		}
	}

	for (int i = 0; i<7; i++)
	{
		if (texRec[i])
		{
			if (texRec[i]->data != NULL) free(texRec[i]->data);
			free(texRec[i]);
		}
	}
	glEnable(GL_TEXTURE_2D);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
}

//렌더링 함수는 인터넷에서 코드를 가져와 사용하였습니다.
void DrawHandler::renderSphere(float cx, float cy, float cz, float r, int p) {				//구를 그리기 위한 함수 
	int i, j;
	const float TWOPI = 6.28318530717958f;
	const float PIDIV2 = 1.57079632679489f;

	float theta1 = 0.0;
	float theta2 = 0.0;
	float theta3 = 0.0;

	float ex = 0.0f;
	float ey = 0.0f;
	float ez = 0.0f;

	float px = 0.0f;
	float py = 0.0f;
	float pz = 0.0f;

	if (r < 0)
		r = -r;

	if (p < 0)
		p = -p;

	if (p < 4 || r <= 0) {
		glBegin(GL_POINTS);
		glVertex3f(cx, cy, cz);
		glEnd();
		return;
	}

	for (i = 0; i < p / 2; ++i) {
		theta1 = i * TWOPI / p - PIDIV2;
		theta2 = (i + 1) * TWOPI / p - PIDIV2;

		glBegin(GL_TRIANGLE_STRIP);
		{
			for (j = 0; j <= p; ++j)
			{
				theta3 = j * TWOPI / p;

				ex = cos(theta2) * cos(theta3);
				ey = sin(theta2);
				ez = cos(theta2) * sin(theta3);
				px = cx + r * ex;
				py = cy + r * ey;
				pz = cz + r * ez;

				glNormal3f(ex, ey, ez);
				glTexCoord2f(-(j / (float)p), 2 * (i + 1) / (float)p);
				glVertex3f(px, py, pz);

				ex = cos(theta1) * cos(theta3);
				ey = sin(theta1);
				ez = cos(theta1) * sin(theta3);
				px = cx + r * ex;
				py = cy + r * ey;
				pz = cz + r * ez;

				glNormal3f(ex, ey, ez);
				glTexCoord2f(-(j / (float)p), 2 * i / (float)p);
				glVertex3f(px, py, pz);
			}
		}
		glEnd();
	}
}

#endif