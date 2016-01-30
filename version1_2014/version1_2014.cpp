// version1_2014.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

#include "stdafx.h"
#include <GL/freeglut.h>
#include <GL/GLAUX.H>

#include <math.h>
#include <time.h>
#pragma comment (lib, "glaux.lib")
#include <iostream>	

#define GET_X_DIS 10
#define GET_Y_DIS 11
#define GET_Z_DIS 12
#define GET_SKEWED_DIS 13

#define RADIAN 20
#define DEGREE 30

#define center 8

static GLfloat LookAt_X = 0.0, LookAt_Y = -6.7800286e-008, LookAt_Z = 2.05;
static GLfloat orthoLeft = -1.4, orthoRight = 1.4, orthoBottom = -1.4, orthoTop = 1.4;
static GLfloat movingCount[8] = { 0.0 };
static int isPooled[8] = { 0 };

struct BallPoint {
	int bnum;
	GLfloat x;
	GLfloat y;
	GLfloat z;
};

static struct BallPoint st_BallPoint[9] = {
	{ 0, 0.0, 0.0, 0.0 },{ 1, 0.0, 0.0, 0.0 },{ 2, 0.0, 0.0, 0.0 },{ 3, 0.0, 0.0, 0.0 },
	{ 4, 0.0, 0.0, 0.0 },{ 5, 0.0, 0.0, 0.0 },{ 6, 0.0, 0.0, 0.0 },{ 7, 0.0, 0.0, 0.0 },{ 8, 0.0, 0.0, 0.0 }
};

struct Rotate {
	int isCalced;
	GLfloat angle;
	GLfloat axis_x;
	GLfloat axis_y;
	GLfloat axis_z;
};

static struct Rotate st_Rotate[8] = { 0.0 };

struct TransAmount {
	int isCalced[8];
	int count[8];
	GLfloat x;
	GLfloat y;
	GLfloat z;
};

static struct TransAmount * st_TransAmount[8] = { 0 };

struct BallDistance {
	int bnum;
	GLfloat x;
	GLfloat y;
	GLfloat z;
	GLfloat skewed;
};

static struct BallDistance st_BallDis[8] = {
	{ 0, 0.0, 0.0, 0.0, 0.0 },{ 1, 0.0, 0.0, 0.0, 0.0 },{ 2, 0.0, 0.0, 0.0, 0.0 },{ 3, 0.0, 0.0, 0.0, 0.0 },
	{ 4, 0.0, 0.0, 0.0, 0.0 },{ 5, 0.0, 0.0, 0.0, 0.0 },{ 6, 0.0, 0.0, 0.0, 0.0 },{ 7, 0.0, 0.0, 0.0, 0.0 }
};

static GLfloat f_inc = 0;
static int a = 0.0;
static int cuepower = 10;
static int shot = 0;
static GLfloat angle;
static GLfloat angle7 = 0.0, angle0 = 0.0;
static GLfloat RotateDisplay = 0.0;

static GLfloat glabal_ambient[] = { 0.1, 0.1, 0.1, 1.0 };
static GLfloat mat_diffuse[] = { 0.25, 0.25, 1.0, 0.0 };	//확산반사
static GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 0.0 };		//경면반사
static GLfloat light_position[] = { 0.0, 0.0, -5.0, 1.0 };//광원의 위치

GLuint Texture[7];
//텍스쳐 맵핑 함수

void Init() {
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialf(GL_FRONT, GL_SHININESS, 25.0);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glDisable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glShadeModel(GL_SMOOTH);
}

AUX_RGBImageRec *LoadBMP(char *Filename) { //BMP파일을 불러오는 함수
	FILE *File = NULL;

	if (!Filename) return NULL;
	fopen_s(&File, Filename, "r");
	if (File) {
		fclose(File);
		return auxDIBImageLoad(Filename);
	}
	return NULL;
}

void loadTexture(void) { //TEXTURE를 위한 함수
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
			if (texRec[i]->data) free(texRec[i]->data);
			free(texRec[i]);
		}
	}
	glEnable(GL_TEXTURE_2D);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
}

#define PI 3.14159265358979f;
void renderSphere(float cx, float cy, float cz, float r, int p) {	//구를 그리기 위한 함수 
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

//거리를 계산해서 리턴하는 코드
GLfloat Calc_Distance(int type, int BaseNum, int SubNum) {
	switch (type) {
	case GET_X_DIS:
		return st_BallPoint[SubNum].x - st_BallPoint[BaseNum].x;
	case GET_Y_DIS:
		return st_BallPoint[SubNum].y - st_BallPoint[BaseNum].y;
	case GET_Z_DIS:
		return st_BallPoint[SubNum].z - st_BallPoint[BaseNum].z;
	case GET_SKEWED_DIS:
		return sqrt(pow(Calc_Distance(GET_X_DIS, BaseNum, SubNum), 2) + pow(Calc_Distance(GET_Y_DIS, BaseNum, SubNum), 2));
	}
}

//좌표의 몇사분면인지 케이스를 나눠주는 함수
int Get_DistanceCase(int BaseNum, int SubNum) {
	struct BallDistance tmpDis;

	tmpDis.x = Calc_Distance(GET_X_DIS, BaseNum, SubNum);
	tmpDis.y = Calc_Distance(GET_Y_DIS, BaseNum, SubNum);

	if (tmpDis.x>0 && tmpDis.y<0) return 1;
	if (tmpDis.x<0 && tmpDis.y<0) return 2;
	if (tmpDis.x<0 && tmpDis.y>0) return 3;
	if (tmpDis.x>0 && tmpDis.y>0) return 4;
}

//각을 계산해서 리턴하는 코드
GLfloat Calc_Angle(int type, int BaseNum, int SubNum) {
	struct BallDistance tmpDis;
	GLfloat angle_radian = 0.0, angle_degree = 0.0;

	tmpDis.x = Calc_Distance(GET_X_DIS, BaseNum, SubNum);
	tmpDis.y = Calc_Distance(GET_Y_DIS, BaseNum, SubNum);

	angle_radian = atan2(fabs(tmpDis.x), fabs(tmpDis.y));

	if (type == DEGREE) {
		angle_degree = angle_radian * 180 / PI;
		return angle_degree;
	}
	return angle_radian;
}

//공의 좌표 이동 증감량 계산하는 함수
void Calc_TransAmount(GLfloat inc, GLfloat angle_radian, int BaseNum, int SubNum) {
	f_inc = inc;

	if (BaseNum == 7) {
		int _case = Get_DistanceCase(7, 0);
		if (_case == 1) { st_TransAmount[BaseNum]->x = f_inc*sin(angle_radian); st_TransAmount[BaseNum]->y = -f_inc*cos(angle_radian); }
		if (_case == 2) { st_TransAmount[BaseNum]->x = -f_inc*sin(angle_radian); st_TransAmount[BaseNum]->y = -f_inc*cos(angle_radian); }
		if (_case == 3) { st_TransAmount[BaseNum]->x = -f_inc*sin(angle_radian); st_TransAmount[BaseNum]->y = f_inc*cos(angle_radian); }
		if (_case == 4) { st_TransAmount[BaseNum]->x = f_inc*sin(angle_radian); st_TransAmount[BaseNum]->y = f_inc*cos(angle_radian); }
	}
	else {
		GLfloat b;
		if (angle7<0) b = 360 + angle7;
		else b = angle7;
		GLfloat tmpAngle = angle_radian * 180 / PI;
		int _case = Get_DistanceCase(BaseNum, SubNum);

		switch (_case) {
		case 1:
			if ((b>(0 + tmpAngle)) && (b<(90 + tmpAngle)))
			{
				st_TransAmount[BaseNum]->x = -f_inc*cos(angle_radian); st_TransAmount[BaseNum]->y = -f_inc*sin(angle_radian);
			}
			else if ((b>(90 + tmpAngle)) && (b<(180 + tmpAngle)))

			{
				st_TransAmount[BaseNum]->x = f_inc*cos(angle_radian); st_TransAmount[BaseNum]->y = f_inc*sin(angle_radian);
			}
			else if (b == 90 + tmpAngle)
			{
				st_TransAmount[BaseNum]->x = 0.0; st_TransAmount[SubNum]->y = 0.0;
			}
			else
			{
				st_TransAmount[BaseNum]->x = st_TransAmount[7]->x; st_TransAmount[BaseNum]->y = st_TransAmount[7]->y;
			}

			st_TransAmount[SubNum]->x = st_TransAmount[BaseNum]->y;
			st_TransAmount[SubNum]->y = st_TransAmount[BaseNum]->x;

			if (st_TransAmount[SubNum]->x<0) st_TransAmount[SubNum]->x *= (-1);
			if (st_TransAmount[SubNum]->y>0) st_TransAmount[SubNum]->y *= (-1);

			break;

		case 2:
			if ((b>(90 - tmpAngle)) && (b<(180 - tmpAngle)))
			{
				st_TransAmount[BaseNum]->x = f_inc*cos(angle_radian); st_TransAmount[BaseNum]->y = (-1)*f_inc*sin(angle_radian);
			}
			else if ((b>(360 - tmpAngle) && a <= 0) || (b<(90 - tmpAngle)) && a >= 0)
			{
				st_TransAmount[BaseNum]->x = (-1)*f_inc*cos(angle_radian); st_TransAmount[BaseNum]->y = f_inc*sin(angle_radian);
			}
			else if (b == 90 - tmpAngle)
			{
				st_TransAmount[0]->x = 0.0; st_TransAmount[0]->y = 0.0;
			}
			else
			{
				st_TransAmount[0]->x = st_TransAmount[7]->x; st_TransAmount[0]->y = st_TransAmount[7]->y;
			}

			st_TransAmount[SubNum]->x = st_TransAmount[BaseNum]->y;
			st_TransAmount[SubNum]->y = st_TransAmount[BaseNum]->x;

			if (st_TransAmount[SubNum]->x>0) st_TransAmount[SubNum]->x *= (-1);
			if (st_TransAmount[SubNum]->y>0) st_TransAmount[SubNum]->y *= (-1);

			break;

		case 3:
			if ((b>(180 + tmpAngle)) && (b<(270 + tmpAngle)))
			{
				st_TransAmount[BaseNum]->x = f_inc*cos(angle_radian); st_TransAmount[BaseNum]->y = f_inc*sin(angle_radian);
			}
			else if ((b>(270 + tmpAngle) && a <= 0) || (b<tmpAngle) && (a >= 0))
			{
				st_TransAmount[BaseNum]->x = -f_inc*cos(angle_radian); st_TransAmount[BaseNum]->y = -f_inc*sin(angle_radian);
			}
			else if (b == 270 + tmpAngle)
			{
				st_TransAmount[0]->x = 0.0; st_TransAmount[0]->y = 0.0;
			}
			else
			{
				st_TransAmount[0]->x = st_TransAmount[7]->x; st_TransAmount[0]->y = st_TransAmount[7]->y;
			}

			st_TransAmount[SubNum]->x = st_TransAmount[BaseNum]->y;
			st_TransAmount[SubNum]->y = st_TransAmount[BaseNum]->x;

			if (st_TransAmount[SubNum]->x>0) st_TransAmount[SubNum]->x *= (-1);
			if (st_TransAmount[SubNum]->y<0) st_TransAmount[SubNum]->y *= (-1);
			break;

		case 4:
			if ((b>(180 - tmpAngle)) && (b<(270 - tmpAngle)))
			{
				st_TransAmount[BaseNum]->x = f_inc*cos(angle_radian); st_TransAmount[BaseNum]->y = -f_inc*sin(angle_radian);
			}
			else if ((b>(270 - tmpAngle)) && (b<(360 - tmpAngle)))
			{
				st_TransAmount[BaseNum]->x = -f_inc*cos(angle_radian); st_TransAmount[BaseNum]->y = f_inc*sin(angle_radian);
			}
			else if (b == 270 - tmpAngle)
			{
				st_TransAmount[0]->x = 0.0; st_TransAmount[0]->y = 0.0;
			}
			else {
				st_TransAmount[0]->x = st_TransAmount[7]->x; st_TransAmount[0]->y = st_TransAmount[7]->y;
			}

			st_TransAmount[SubNum]->x = st_TransAmount[BaseNum]->y;
			st_TransAmount[SubNum]->y = st_TransAmount[BaseNum]->x;

			if (st_TransAmount[SubNum]->x<0) st_TransAmount[SubNum]->x *= (-1);
			if (st_TransAmount[SubNum]->y<0) st_TransAmount[SubNum]->y *= (-1);

			break;
		}
	}
}

//공의 회전각을 구하는 함수
void Calc_RotateAxis(int BaseNum) {
	st_Rotate[BaseNum].axis_x = st_TransAmount[BaseNum]->y;
	st_Rotate[BaseNum].axis_y = st_TransAmount[BaseNum]->x;

	if ((st_TransAmount[BaseNum]->x<0 && st_TransAmount[BaseNum]->y>0) || (st_TransAmount[BaseNum]->x>0 && st_TransAmount[BaseNum]->y<0)) {
		if (st_Rotate[BaseNum].axis_x<0) st_Rotate[BaseNum].axis_x *= (-1);
		if (st_Rotate[BaseNum].axis_y<0) st_Rotate[BaseNum].axis_y *= (-1);
	}
	if ((st_TransAmount[BaseNum]->x>0 && st_TransAmount[BaseNum]->y>0) || (st_TransAmount[BaseNum]->x<0 && st_TransAmount[BaseNum]->y<0)) {
		if (st_Rotate[BaseNum].axis_x<0) st_Rotate[BaseNum].axis_x *= (-1);
		if (st_Rotate[BaseNum].axis_y>0) st_Rotate[BaseNum].axis_y *= (-1);
	}
}

//좌표가 중복되지 않도록 비교하는 함수
void Compare(int target) {

	GLboolean isSuccess = TRUE;
	do {
		for (int k = target - 1; k >= 0; k--) {
			if (Calc_Distance(GET_SKEWED_DIS, k, target)<0.15) { isSuccess = FALSE; break; }
			else isSuccess = TRUE;
		}

		if (isSuccess == FALSE) {
			st_BallPoint[target].x = 1.7 * ((GLfloat)rand() / RAND_MAX) - 0.85;
			st_BallPoint[target].y = 1.7 * ((GLfloat)rand() / RAND_MAX) - 0.85;
		}
	} while (isSuccess != TRUE);
}

//공의 좌표를 랜덤하게 초기화하고 공 그리는 함수 호출
void Break() {

	srand((unsigned)time(NULL)); //좌표값 중복되지않게 그리되
	int bnum;
	for (bnum = 0; bnum<7; bnum++) {
		st_BallPoint[bnum].x = 1.7 * ((GLfloat)rand() / RAND_MAX) - 0.85;
		st_BallPoint[bnum].y = 1.7 * ((GLfloat)rand() / RAND_MAX) - 0.85;

		Compare(bnum);
	}
}

//공이 벽에 부딫혔을 때 케이스에 맞게 공의 진행방향을 바꿔주는 함수
void isBumpedWall(int bnum) {
	//왼쪽벽에 부딫혓을 경우
	if (st_BallPoint[bnum].x <= -0.95) {
		if ((st_TransAmount[bnum]->x<0 && st_TransAmount[bnum]->y>0) || (st_TransAmount[bnum]->x<0 && st_TransAmount[bnum]->y<0))
			st_TransAmount[bnum]->x *= (-1);
	}
	//위쪽벽에 부딫혔을 경우
	else if (st_BallPoint[bnum].y >= 0.95) {
		if ((st_TransAmount[bnum]->x<0 && st_TransAmount[bnum]->y>0) || (st_TransAmount[bnum]->x>0 && st_TransAmount[bnum]->y>0))
			st_TransAmount[bnum]->y *= (-1);
	}
	//(오른쪽벽에 부딫혓을 경우
	else if (st_BallPoint[bnum].x >= 0.95) {
		if ((st_TransAmount[bnum]->x>0 && st_TransAmount[bnum]->y<0) || (st_TransAmount[bnum]->x>0 && st_TransAmount[bnum]->y>0))
			st_TransAmount[bnum]->x *= (-1);
	}
	//아래쪽벽에 부딫혓을 경우5
	else if (st_BallPoint[bnum].y <= -0.95) {
		if ((st_TransAmount[bnum]->x>0 && st_TransAmount[bnum]->y<0) || (st_TransAmount[bnum]->x<0 && st_TransAmount[bnum]->y<0))
			st_TransAmount[bnum]->y *= (-1);
	}

	Calc_RotateAxis(bnum);
}

//공이 포켓에 빠졌는지 확인하고 처리하는 함수
void isPooledPocket(int bnum) {
	if (st_BallPoint[bnum].x<-0.9 && st_BallPoint[bnum].y>0.9) {
		isPooled[bnum] = 1;
		st_BallPoint[bnum].x = -1.0; st_BallPoint[bnum].y = 1.0;
	}
	else if (st_BallPoint[bnum].x>0.9 && st_BallPoint[bnum].y>0.9) {
		isPooled[bnum] = 2;
		st_BallPoint[bnum].x = 1.0; st_BallPoint[bnum].y = 1.0;
	}
	else if (st_BallPoint[bnum].y>-0.05 && st_BallPoint[bnum].y<0.05 && st_BallPoint[bnum].x<-0.9) {
		isPooled[bnum] = 3;
		st_BallPoint[bnum].x = -1.0; st_BallPoint[bnum].y = 0.0;
	}
	else if (st_BallPoint[bnum].y>-0.05 && st_BallPoint[bnum].y<0.05 && st_BallPoint[bnum].x>0.9) {
		isPooled[bnum] = 4;
		st_BallPoint[bnum].x = 1.0; st_BallPoint[bnum].y = 0.0;
	}
	else if (st_BallPoint[bnum].x<-0.9 && st_BallPoint[bnum].y<-0.9) {
		isPooled[bnum] = 5;
		st_BallPoint[bnum].x = 1.0; st_BallPoint[bnum].y = -1.0;
	}
	else if (st_BallPoint[bnum].x>0.9 && st_BallPoint[bnum].y<-0.9) {
		isPooled[bnum] = 6;
		st_BallPoint[bnum].x = 1.0; st_BallPoint[bnum].y = -1.0;
	}
}

//큐의 움직일 좌표 구하는 함수
void Get_CuePoint() {
	f_inc = 0.05*cuepower / 10 + 0.4;
	angle = Calc_Angle(RADIAN, 7, 0);
	int _case = Get_DistanceCase(7, 0);

	if (_case == 1) { st_BallPoint[7].x = -f_inc*sin(angle); st_BallPoint[7].y = f_inc*cos(angle); }
	if (_case == 2) { st_BallPoint[7].x = f_inc*sin(angle); st_BallPoint[7].y = f_inc*cos(angle); }
	if (_case == 3) { st_BallPoint[7].x = f_inc*sin(angle); st_BallPoint[7].y = -f_inc*cos(angle); }
	if (_case == 4) { st_BallPoint[7].x = -f_inc*sin(angle); st_BallPoint[7].y = -f_inc*cos(angle); }

	st_BallPoint[7].x = st_BallPoint[0].x + st_BallPoint[7].x; st_BallPoint[7].y = st_BallPoint[0].y + st_BallPoint[7].y;
}

//큐의 위치에 따라 회전각도을 구해주는 함수.
void Get_CueAngle() {
	angle7 = Calc_Angle(DEGREE, 7, 0);
	int _case = Get_DistanceCase(7, 0);

	if (_case == 1) angle7 = angle7 + 180;
	if (_case == 2) angle7 = 360 - angle7;
	if (_case == 3) angle7 = angle7;
	if (_case == 4) angle7 = 180 - angle7;

}

//큐가 얼마나 움직여야 하는지 정해주는 함수
void Get_CuemovingCount() {
	movingCount[7] = cuepower * 6;
	movingCount[0] = cuepower * 6;
}

//큐와 공의 거리에 따라 큐의 세기를 구해주는 함수
void Get_CuePower() {
	Calc_TransAmount(0.05, Calc_Angle(RADIAN, 7, 0), 7, 0);
	if (cuepower >= 60) {
		cuepower = 10;
		Get_CuePoint();
	}
	else if (cuepower != 0) {
		st_BallPoint[7].x += st_TransAmount[7]->x;
		st_BallPoint[7].y += st_TransAmount[7]->y;
	}
}

//공의 이동량을 0으로 초기화하는 함수
void InitPower() {
	for (int i = 0; i<8; i++) {
		movingCount[i] = 0;
		st_TransAmount[i]->x = 0.0; st_TransAmount[i]->y = 0.0; st_TransAmount[i]->z = 0.0;
	}
}

//수구를 초기화하는 함수
void Reset_WhiteBall() {

	srand((unsigned)time(NULL));

	st_BallPoint[0].x = 1.7 * ((GLfloat)rand() / RAND_MAX) - 0.85;
	st_BallPoint[0].y = 1.7 * ((GLfloat)rand() / RAND_MAX) - 0.85;

	GLboolean isSuccess = TRUE;

	do {
		for (int k = 1; k<7; k++) {
			if (Calc_Distance(GET_SKEWED_DIS, 0, k)<0.1) { isSuccess = FALSE; break; }
			else isSuccess = TRUE;
		}

		if (isSuccess == FALSE) {
			st_BallPoint[0].x = 1.7 * ((GLfloat)rand() / RAND_MAX) - 0.85;
			st_BallPoint[0].y = 1.7 * ((GLfloat)rand() / RAND_MAX) - 0.85;
		}
	} while (isSuccess != TRUE);

}

//당구대 그리기
void Draw_PoolTable() {
	glPushMatrix();
	glEnable(GL_COLOR_MATERIAL);
	glPushMatrix();

	glColor3f(0.0, 0.5, 0.0);  //초록색
	glBegin(GL_POLYGON);
	glVertex3f(-1.0, 1.0, 0.0);
	glVertex3f(1.0, 1.0, 0.0);
	glVertex3f(1.0, -1.0, 0.0);
	glVertex3f(-1.0, -1.0, 0.0);
	glEnd();

	glPopMatrix();
	glPushMatrix();

	glColor3f(0.3, 0.2, 0.0); //갈색
	glTranslatef(-1, 0, 0.05);
	glScalef(1, 20, 1);
	glutSolidCube(0.1);
	glColor3f(0, 0, 0);
	glutWireCube(0.1);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.3, 0.2, 0.0);
	glTranslatef(1, 0, 0.05);
	glScalef(1, 20, 1);
	glutSolidCube(0.1);
	glColor3f(0, 0, 0);
	glutWireCube(0.1);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.3, 0.2, 0.0);
	glTranslatef(0, -1, 0.05);
	glScalef(21, 1, 1);
	glutSolidCube(0.1);
	glColor3f(0, 0, 0);
	glutWireCube(0.1);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.3, 0.2, 0.0);
	glTranslatef(0, 1, 0.05);
	glScalef(21, 1, 1);
	glutSolidCube(0.1);
	glColor3f(0, 0, 0);
	glutWireCube(0.1);
	glPopMatrix();



	glColor3f(0.0, 0.0, 0.0); //검은색
	glPushMatrix();
	glTranslatef(-0.95, 0.95, 0);
	glutSolidSphere(0.07, 40, 40);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0.95, 0.95, 0);
	glutSolidSphere(0.07, 40, 40);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(-0.95, 0.0, 0);
	glutSolidSphere(0.07, 40, 40);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0.95, 0.0, 0);
	glutSolidSphere(0.07, 40, 40);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(-0.95, -0.95, 0);
	glutSolidSphere(0.07, 40, 40);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0.95, -0.95, 0);
	glutSolidSphere(0.07, 40, 40);
	glPopMatrix();

	glPopMatrix();
}

//공을 그리는 함수
static GLfloat MyPlane[] = { 0.0, 1.0, 0.0, 0.0 };
void Draw_Ball() {
	glPushMatrix();
	glEnable(GL_COLOR_MATERIAL);
	glColor3f(1.0, 1.0, 1.0);
	glEnable(GL_TEXTURE_2D);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	//원근에 따른 텍스쳐보정. P600

	glPushMatrix();

	glColor3f(0.5, 1.0, 1.0);
	glTranslatef((GLfloat)st_BallPoint[7].x, (GLfloat)st_BallPoint[7].y, 0.05);
	glRotatef((GLfloat)angle7, 0.0, 0.0, 1.0);
	glScalef(0.4, 14.0, 0.4);
	glutSolidCube(0.05);
	glBindTexture(GL_TEXTURE_2D, Texture[7]);
	//비트맵 이미지의 정점과 큐브의 정점을 연결하면됨

	glPopMatrix();
	glColor3f(1.0, 1.0, 1.0);
	glPushMatrix();

	glTranslatef((GLfloat)st_BallPoint[0].x, (GLfloat)st_BallPoint[0].y, 0.05);
	if (shot == 1) glRotatef((GLfloat)st_Rotate[0].angle, (GLfloat)st_Rotate[0].axis_x, (GLfloat)st_Rotate[0].axis_y, 0.0);
	if (isPooled[0] == 0) {
		glBindTexture(GL_TEXTURE_2D, Texture[0]);
		renderSphere(0, 0, 0, 0.05, 50);
	}

	glPopMatrix();
	glPushMatrix();

	glTranslatef((GLfloat)st_BallPoint[1].x, (GLfloat)st_BallPoint[1].y, 0.05);
	if (shot == 1) glRotatef((GLfloat)st_Rotate[1].angle, (GLfloat)st_Rotate[1].axis_x, (GLfloat)st_Rotate[1].axis_y, 0.0);
	if (isPooled[1] == 0) {
		glBindTexture(GL_TEXTURE_2D, Texture[1]);
		renderSphere(0, 0, 0, 0.05, 50);
	}

	glPopMatrix();
	glPushMatrix();

	glTranslatef((GLfloat)st_BallPoint[2].x, (GLfloat)st_BallPoint[2].y, 0.05);
	glRotatef((GLfloat)st_Rotate[2].angle, (GLfloat)st_Rotate[2].axis_x, (GLfloat)st_Rotate[2].axis_y, 0.0);
	if (isPooled[2] == 0) {
		glBindTexture(GL_TEXTURE_2D, Texture[2]);
		renderSphere(0, 0, 0, 0.05, 50);
	}

	glPopMatrix();
	glPushMatrix();

	glTranslatef((GLfloat)st_BallPoint[3].x, (GLfloat)st_BallPoint[3].y, 0.05);
	glRotatef((GLfloat)st_Rotate[3].angle, (GLfloat)st_Rotate[3].axis_x, (GLfloat)st_Rotate[3].axis_y, 0.0);
	if (isPooled[3] == 0) {
		glBindTexture(GL_TEXTURE_2D, Texture[3]);
		renderSphere(0, 0, 0, 0.05, 50);
	}

	glPopMatrix();
	glPushMatrix();

	glTranslatef((GLfloat)st_BallPoint[4].x, (GLfloat)st_BallPoint[4].y, 0.05);
	glRotatef((GLfloat)st_Rotate[4].angle, (GLfloat)st_Rotate[4].axis_x, (GLfloat)st_Rotate[4].axis_y, 0.0);
	if (isPooled[4] == 0) {
		glBindTexture(GL_TEXTURE_2D, Texture[4]);
		renderSphere(0, 0, 0, 0.05, 50);
	}

	glPopMatrix();
	glPushMatrix();

	glTranslatef((GLfloat)st_BallPoint[5].x, (GLfloat)st_BallPoint[5].y, 0.05);
	glRotatef((GLfloat)st_Rotate[5].angle, (GLfloat)st_Rotate[5].axis_x, (GLfloat)st_Rotate[5].axis_y, 0.0);
	if (isPooled[5] == 0) {
		glBindTexture(GL_TEXTURE_2D, Texture[5]);
		renderSphere(0, 0, 0, 0.05, 50);
	}

	glPopMatrix();
	glPushMatrix();

	glTranslatef((GLfloat)st_BallPoint[6].x, (GLfloat)st_BallPoint[6].y, 0.05);
	glRotatef((GLfloat)st_Rotate[6].angle, (GLfloat)st_Rotate[6].axis_x, (GLfloat)st_Rotate[6].axis_y, 0.0);
	if (isPooled[6] == 0) {
		glBindTexture(GL_TEXTURE_2D, Texture[6]);
		renderSphere(0, 0, 0, 0.05, 50);
	}
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	glPopMatrix();
}

//아래에 위치한 뷰에 포켓에 들어간 공을 그려주는 함수
void Draw_PooledBall() {
	glEnable(GL_TEXTURE_2D);
	glColor3f(0.0, 0.0, 0.0);
	glPushMatrix();

	glPushMatrix();
	glEnable(GL_LIGHT0);
	glColor3f(1.0, 1.0, 1.0);
	if (isPooled[1] != 0) {
		glPushMatrix();
		glTranslatef(-0.85, 0.0, 0.0);
		glBindTexture(GL_TEXTURE_2D, Texture[1]);
		renderSphere(0, 0, 0, 0.1, 50);
		glPopMatrix();
	}
	glPopMatrix();

	glPushMatrix();
	glEnable(GL_LIGHT0);
	if (isPooled[2] != 0) {
		glPushMatrix();
		glTranslatef(-0.51, 0.0, 0.0);
		glRotatef(40.0, 0.0, 1.0, 0.0);
		glBindTexture(GL_TEXTURE_2D, Texture[2]);
		renderSphere(0, 0, 0, 0.1, 50);
		glPopMatrix();
	}
	glPopMatrix();

	glPushMatrix();
	glEnable(GL_LIGHT0);
	if (isPooled[3] != 0) {
		glPushMatrix();
		glTranslatef(-0.17, 0.0, 0.0);
		glRotatef(40.0, 0.0, 1.0, 0.0);
		glBindTexture(GL_TEXTURE_2D, Texture[3]);
		renderSphere(0, 0, 0, 0.1, 50);
		glPopMatrix();
	}
	glPopMatrix();

	glPushMatrix();
	glEnable(GL_LIGHT0);
	if (isPooled[4] != 0) {
		glPushMatrix();
		glTranslatef(0.17, 0.0, 0.0);
		glRotatef(40.0, 0.0, 1.0, 0.0);
		glBindTexture(GL_TEXTURE_2D, Texture[4]);
		renderSphere(0, 0, 0, 0.1, 50);
		glPopMatrix();
	}
	glPopMatrix();

	glPushMatrix();
	glEnable(GL_LIGHT0);
	if (isPooled[5] != 0) {
		glPushMatrix();
		glTranslatef(0.51, 0.0, 0.0);
		glRotatef(40.0, 0.0, 1.0, 0.0);
		glBindTexture(GL_TEXTURE_2D, Texture[5]);
		renderSphere(0, 0, 0, 0.1, 50);
		glPopMatrix();
	}
	glPopMatrix();

	glPushMatrix();
	glEnable(GL_LIGHT0);
	if (isPooled[6] != 0) {
		glPushMatrix();
		glTranslatef(0.85, 0.0, 0.0);
		glRotatef(40.0, 0.0, 1.0, 0.0);
		glBindTexture(GL_TEXTURE_2D, Texture[6]);
		renderSphere(0, 0, 0, 0.1, 50);
		glPopMatrix();
	}
	glPopMatrix();

	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
}

//마우스를 클릭할때마다 큐가 뒤로 이동하며 큐의 세기가 세지도록 하는 함수
void MyMouseClick(GLint Button, GLint State, GLint X, GLint Y) {
	if (Button == GLUT_LEFT_BUTTON && State == GLUT_DOWN) {
		cuepower += 10;
	}

	if (Button == GLUT_RIGHT_BUTTON && State == GLUT_DOWN) {
		InitPower();

		st_TransAmount[7]->count[0] = (int)((0.05*cuepower / 10 + 0.4) / 0.01);
		Calc_TransAmount(0.01, Calc_Angle(RADIAN, 7, 0), 7, 0);

		angle0 = angle7;
		st_TransAmount[0]->x = st_TransAmount[7]->x;
		st_TransAmount[0]->y = st_TransAmount[7]->y;
		st_TransAmount[0]->z = st_TransAmount[7]->z;

		Get_CuemovingCount();
		cuepower = 10;
		shot = 1;
	}
}

//마우스를 따라 큐가 움직이도록 하는 함수
void MyMouseMove(GLint X, GLint Y) {
	st_BallPoint[7].x = (2.0*(GLfloat)X / 800) - 1.0;
	st_BallPoint[7].y = (-1)*((2.0*(GLfloat)Y / 800) - 1.0);
	Get_CueAngle();
	Get_CuePoint();
}

//키보드값에 따른 이벤트처리기
void MyKeyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 27:
		exit(1);
		break;
	case 'f':
		glEnable(GL_LIGHTING);//조명 켜기
		break;

	case 'o':
		glDisable(GL_LIGHTING); //조명 끄기
		break;

	case 'b': //테이블 위에 있는 공의 위치를 랜덤하게 다시 바꾸어준다.
		Break();
		for (int i = 0; i<8; i++)
			for (int j = 0; j<8; j++)
				st_TransAmount[i]->isCalced[j] = 0;
		glutPostRedisplay();
		break;

	case 'n': //새 게임을 시작한다
		InitPower();
		for (int i = 0; i<7; i++) isPooled[i] = 0;
		Break();
		glutPostRedisplay();
		break;

	case 'r': //흰공의 위치를 리셋
		Reset_WhiteBall();
		isPooled[0] = 0;
		st_TransAmount[0]->x = 0.0; st_TransAmount[0]->y = 0.0; st_TransAmount[0]->z = 0.0;
		glutPostRedisplay();
		break;

	case 'h': //화면 왼쪽으로 이동
		orthoLeft -= 0.2; orthoRight -= 0.2;
		glutPostRedisplay();
		break;
	case 'k': //화면을 오른쪽으로 이동
		orthoLeft += 0.2; orthoRight += 0.2;
		glutPostRedisplay();
		break;
	case 'u': //화면을 위로 이동
		orthoTop += 0.2; orthoBottom += 0.2;
		glutPostRedisplay();
		break;
	case 'j': //화면을 아래로 이동
		orthoTop -= 0.2; orthoBottom -= 0.2;
		glutPostRedisplay();
		break;
	case 'z': //누르면 시점변환(왼쪽으로 회전)
		RotateDisplay = RotateDisplay + 2.0;
		break;
	case 'x': //누르면 시점변환(오른쪽으로 회전)
		RotateDisplay = RotateDisplay - 2.0;
		break;

	}
}

//화살표 등의 키보드를 사용하기 위한 함수
void MyKeyboard2(int key, int x, int y) {
	switch (key) {

	case GLUT_KEY_RIGHT:
		LookAt_Z += 0.05; //더욱 위에서 보는 것처럼 보인다.
		glutPostRedisplay();
		break;

	case GLUT_KEY_LEFT:
		if (LookAt_Z>0.5) LookAt_Z -= 0.05; //더욱 가까이 보이게 된다. 
		glutPostRedisplay();
		break;

	case GLUT_KEY_UP:
		if (LookAt_Y<-0.05) {
			LookAt_Y += 0.05; //누운 당구대가 점점 서게 된다.
			LookAt_Z -= 0.05;
		}
		glutPostRedisplay();
		break;

	case GLUT_KEY_DOWN:
		LookAt_Y -= 0.05; //서있는 당구대가 점점 뒤로 눕는 모양이 된다.
		LookAt_Z += 0.05;
		glutPostRedisplay();
		break;
	}
}

//이 타이머에 의해 공의 움직임이 표현된다.
void MyTimer(int value) {
	//큐가 앞으로 나아갈때
	if (shot == 1 && movingCount[7] != 0 && st_TransAmount[7]->count[0]>8) {
		st_BallPoint[7].x += st_TransAmount[7]->x;
		st_BallPoint[7].y += st_TransAmount[7]->y;
		st_BallPoint[7].z -= st_TransAmount[7]->z;
		st_TransAmount[7]->count[0] -= 1;
		movingCount[7]--;
	}
	if (st_TransAmount[7]->count[0] == 8) { movingCount[7] = 0; shot = 0; }

	//i번공이 j번공에 부딫혔을 때 isCaled를 1로 마킹
	for (int i = 0; i<7; i++) {
		for (int j = 0; j<7; j++) {
			if (i != j) {
				st_TransAmount[i]->count[j] = (int)(Calc_Distance(GET_SKEWED_DIS, i, j) / 0.01);
				if (st_TransAmount[i]->count[j] == 10) st_TransAmount[i]->isCalced[j] = 1;
			}
		}
	}

	//i번 공이 j공과 부딫혔을때 i번공과 j번공의 이동량을 계산, 공의 회전 축도 계산
	for (int i = 0; i<7; i++) {
		for (int j = 1; j<7; j++) {
			if (st_TransAmount[i]->isCalced[j] == 1 && movingCount[i] != 0) {
				if (i != j) {
					Calc_TransAmount(0.01, Calc_Angle(RADIAN, i, j), i, j);
					movingCount[j] = movingCount[i];
					st_TransAmount[i]->isCalced[j] = 0;
					Calc_RotateAxis(i);
				}
			}
		}
	}

	//i번공을 계산된 이동량으로 이동시킴
	for (int b = 0; b<7; b++) {
		if (movingCount[b] != 0) {
			st_BallPoint[b].x += st_TransAmount[b]->x;
			st_BallPoint[b].y += st_TransAmount[b]->y;
			st_BallPoint[b].z += st_TransAmount[b]->z;
			movingCount[b]--;
			st_Rotate[b].angle += 5; if (st_Rotate[b].angle == 360) st_Rotate[b].angle = 0;
		}
	}

	//벽에 부딫혓는지 포켓에 들어갔는지 체크
	for (int i = 0; i<7; i++) {
		isBumpedWall(i);
		isPooledPocket(i);
		if (isPooled[i] != 0) movingCount[i] = 0;
	}

	glutPostRedisplay();
	glutTimerFunc(8, MyTimer, 1);
}

//모든 이미지를 그려내는 함수
void MyDisplay() {
	glPushMatrix();
	//1번뷰포트 - 당구대와 당구공 그리기
	glMatrixMode(GL_PROJECTION);
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();
	glOrtho((GLfloat)orthoLeft, (GLfloat)orthoRight, (GLfloat)orthoBottom, (GLfloat)orthoTop, -4.0, 4.0);
	gluPerspective(50, 1.0, 0.0, -50.0);
	glViewport(0, 100, 800, 800);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt((GLfloat)LookAt_X, (GLfloat)LookAt_Y, (GLfloat)LookAt_Z, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	glRotatef(RotateDisplay, 0.0, 0.0, 1.0);

	glPushMatrix();
	Draw_PoolTable();
	Draw_Ball();

	glPopMatrix();
	glPopMatrix();

	//2번뷰포트 - 들어간공 표시
	glPushMatrix();
	glViewport(0, 0, 800, 100);
	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_POLYGON);
	glVertex3f(-1.0, 1.0, 0.0);
	glVertex3f(1.0, 1.0, 0.0);
	glVertex3f(1.0, -1.0, 0.0);
	glVertex3f(-1.0, -1.0, 0.0);
	glEnd();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1.4, 1.4, -0.17, 0.17, -1.0, 1.0);
	Draw_PooledBall();

	glPopMatrix();
	glutSwapBuffers();
}

//메인함수
int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800, 900);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("컴퓨터 그래픽스 기말고사 프로젝트");
	glClearColor(0.0, 0.0, 0.0, 1.0);

	for (int i = 0; i<8; i++) {
		for (int j = 0; j<8; j++) {
			st_TransAmount[i] = (struct TransAmount*)malloc(sizeof(struct TransAmount));
			st_TransAmount[i]->count[j] = 0.0;
			st_TransAmount[i]->isCalced[j] = 0;
			st_TransAmount[i]->x = 0.0; st_TransAmount[i]->y = 0.0; st_TransAmount[i]->z = 0.0;
		}
	}
	loadTexture();
	Break();
	Init();
	glutIdleFunc(MyDisplay);
	glutDisplayFunc(MyDisplay);
	glutKeyboardFunc(MyKeyboard);
	glutSpecialFunc(MyKeyboard2);
	glutMouseFunc(MyMouseClick);
	glutPassiveMotionFunc(MyMouseMove);
	glutTimerFunc(8, MyTimer, 1);
	glutMainLoop();
	return 0;
}
