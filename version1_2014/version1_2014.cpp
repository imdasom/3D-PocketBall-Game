// version1_2014.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
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
static GLfloat mat_diffuse[] = { 0.25, 0.25, 1.0, 0.0 };	//Ȯ��ݻ�
static GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 0.0 };		//���ݻ�
static GLfloat light_position[] = { 0.0, 0.0, -5.0, 1.0 };//������ ��ġ

GLuint Texture[7];
//�ؽ��� ���� �Լ�

void Init() {
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialf(GL_FRONT, GL_SHININESS, 25.0);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glDisable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glShadeModel(GL_SMOOTH);
}

AUX_RGBImageRec *LoadBMP(char *Filename) { //BMP������ �ҷ����� �Լ�
	FILE *File = NULL;

	if (!Filename) return NULL;
	fopen_s(&File, Filename, "r");
	if (File) {
		fclose(File);
		return auxDIBImageLoad(Filename);
	}
	return NULL;
}

void loadTexture(void) { //TEXTURE�� ���� �Լ�
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
void renderSphere(float cx, float cy, float cz, float r, int p) {	//���� �׸��� ���� �Լ� 
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

//�Ÿ��� ����ؼ� �����ϴ� �ڵ�
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

//��ǥ�� ���и����� ���̽��� �����ִ� �Լ�
int Get_DistanceCase(int BaseNum, int SubNum) {
	struct BallDistance tmpDis;

	tmpDis.x = Calc_Distance(GET_X_DIS, BaseNum, SubNum);
	tmpDis.y = Calc_Distance(GET_Y_DIS, BaseNum, SubNum);

	if (tmpDis.x>0 && tmpDis.y<0) return 1;
	if (tmpDis.x<0 && tmpDis.y<0) return 2;
	if (tmpDis.x<0 && tmpDis.y>0) return 3;
	if (tmpDis.x>0 && tmpDis.y>0) return 4;
}

//���� ����ؼ� �����ϴ� �ڵ�
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

//���� ��ǥ �̵� ������ ����ϴ� �Լ�
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

//���� ȸ������ ���ϴ� �Լ�
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

//��ǥ�� �ߺ����� �ʵ��� ���ϴ� �Լ�
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

//���� ��ǥ�� �����ϰ� �ʱ�ȭ�ϰ� �� �׸��� �Լ� ȣ��
void Break() {

	srand((unsigned)time(NULL)); //��ǥ�� �ߺ������ʰ� �׸���
	int bnum;
	for (bnum = 0; bnum<7; bnum++) {
		st_BallPoint[bnum].x = 1.7 * ((GLfloat)rand() / RAND_MAX) - 0.85;
		st_BallPoint[bnum].y = 1.7 * ((GLfloat)rand() / RAND_MAX) - 0.85;

		Compare(bnum);
	}
}

//���� ���� �΋H���� �� ���̽��� �°� ���� ��������� �ٲ��ִ� �Լ�
void isBumpedWall(int bnum) {
	//���ʺ��� �΋H���� ���
	if (st_BallPoint[bnum].x <= -0.95) {
		if ((st_TransAmount[bnum]->x<0 && st_TransAmount[bnum]->y>0) || (st_TransAmount[bnum]->x<0 && st_TransAmount[bnum]->y<0))
			st_TransAmount[bnum]->x *= (-1);
	}
	//���ʺ��� �΋H���� ���
	else if (st_BallPoint[bnum].y >= 0.95) {
		if ((st_TransAmount[bnum]->x<0 && st_TransAmount[bnum]->y>0) || (st_TransAmount[bnum]->x>0 && st_TransAmount[bnum]->y>0))
			st_TransAmount[bnum]->y *= (-1);
	}
	//(�����ʺ��� �΋H���� ���
	else if (st_BallPoint[bnum].x >= 0.95) {
		if ((st_TransAmount[bnum]->x>0 && st_TransAmount[bnum]->y<0) || (st_TransAmount[bnum]->x>0 && st_TransAmount[bnum]->y>0))
			st_TransAmount[bnum]->x *= (-1);
	}
	//�Ʒ��ʺ��� �΋H���� ���5
	else if (st_BallPoint[bnum].y <= -0.95) {
		if ((st_TransAmount[bnum]->x>0 && st_TransAmount[bnum]->y<0) || (st_TransAmount[bnum]->x<0 && st_TransAmount[bnum]->y<0))
			st_TransAmount[bnum]->y *= (-1);
	}

	Calc_RotateAxis(bnum);
}

//���� ���Ͽ� �������� Ȯ���ϰ� ó���ϴ� �Լ�
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

//ť�� ������ ��ǥ ���ϴ� �Լ�
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

//ť�� ��ġ�� ���� ȸ�������� �����ִ� �Լ�.
void Get_CueAngle() {
	angle7 = Calc_Angle(DEGREE, 7, 0);
	int _case = Get_DistanceCase(7, 0);

	if (_case == 1) angle7 = angle7 + 180;
	if (_case == 2) angle7 = 360 - angle7;
	if (_case == 3) angle7 = angle7;
	if (_case == 4) angle7 = 180 - angle7;

}

//ť�� �󸶳� �������� �ϴ��� �����ִ� �Լ�
void Get_CuemovingCount() {
	movingCount[7] = cuepower * 6;
	movingCount[0] = cuepower * 6;
}

//ť�� ���� �Ÿ��� ���� ť�� ���⸦ �����ִ� �Լ�
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

//���� �̵����� 0���� �ʱ�ȭ�ϴ� �Լ�
void InitPower() {
	for (int i = 0; i<8; i++) {
		movingCount[i] = 0;
		st_TransAmount[i]->x = 0.0; st_TransAmount[i]->y = 0.0; st_TransAmount[i]->z = 0.0;
	}
}

//������ �ʱ�ȭ�ϴ� �Լ�
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

//�籸�� �׸���
void Draw_PoolTable() {
	glPushMatrix();
	glEnable(GL_COLOR_MATERIAL);
	glPushMatrix();

	glColor3f(0.0, 0.5, 0.0);  //�ʷϻ�
	glBegin(GL_POLYGON);
	glVertex3f(-1.0, 1.0, 0.0);
	glVertex3f(1.0, 1.0, 0.0);
	glVertex3f(1.0, -1.0, 0.0);
	glVertex3f(-1.0, -1.0, 0.0);
	glEnd();

	glPopMatrix();
	glPushMatrix();

	glColor3f(0.3, 0.2, 0.0); //����
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



	glColor3f(0.0, 0.0, 0.0); //������
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

//���� �׸��� �Լ�
static GLfloat MyPlane[] = { 0.0, 1.0, 0.0, 0.0 };
void Draw_Ball() {
	glPushMatrix();
	glEnable(GL_COLOR_MATERIAL);
	glColor3f(1.0, 1.0, 1.0);
	glEnable(GL_TEXTURE_2D);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	//���ٿ� ���� �ؽ��ĺ���. P600

	glPushMatrix();

	glColor3f(0.5, 1.0, 1.0);
	glTranslatef((GLfloat)st_BallPoint[7].x, (GLfloat)st_BallPoint[7].y, 0.05);
	glRotatef((GLfloat)angle7, 0.0, 0.0, 1.0);
	glScalef(0.4, 14.0, 0.4);
	glutSolidCube(0.05);
	glBindTexture(GL_TEXTURE_2D, Texture[7]);
	//��Ʈ�� �̹����� ������ ť���� ������ �����ϸ��

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

//�Ʒ��� ��ġ�� �信 ���Ͽ� �� ���� �׷��ִ� �Լ�
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

//���콺�� Ŭ���Ҷ����� ť�� �ڷ� �̵��ϸ� ť�� ���Ⱑ �������� �ϴ� �Լ�
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

//���콺�� ���� ť�� �����̵��� �ϴ� �Լ�
void MyMouseMove(GLint X, GLint Y) {
	st_BallPoint[7].x = (2.0*(GLfloat)X / 800) - 1.0;
	st_BallPoint[7].y = (-1)*((2.0*(GLfloat)Y / 800) - 1.0);
	Get_CueAngle();
	Get_CuePoint();
}

//Ű���尪�� ���� �̺�Ʈó����
void MyKeyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 27:
		exit(1);
		break;
	case 'f':
		glEnable(GL_LIGHTING);//���� �ѱ�
		break;

	case 'o':
		glDisable(GL_LIGHTING); //���� ����
		break;

	case 'b': //���̺� ���� �ִ� ���� ��ġ�� �����ϰ� �ٽ� �ٲپ��ش�.
		Break();
		for (int i = 0; i<8; i++)
			for (int j = 0; j<8; j++)
				st_TransAmount[i]->isCalced[j] = 0;
		glutPostRedisplay();
		break;

	case 'n': //�� ������ �����Ѵ�
		InitPower();
		for (int i = 0; i<7; i++) isPooled[i] = 0;
		Break();
		glutPostRedisplay();
		break;

	case 'r': //����� ��ġ�� ����
		Reset_WhiteBall();
		isPooled[0] = 0;
		st_TransAmount[0]->x = 0.0; st_TransAmount[0]->y = 0.0; st_TransAmount[0]->z = 0.0;
		glutPostRedisplay();
		break;

	case 'h': //ȭ�� �������� �̵�
		orthoLeft -= 0.2; orthoRight -= 0.2;
		glutPostRedisplay();
		break;
	case 'k': //ȭ���� ���������� �̵�
		orthoLeft += 0.2; orthoRight += 0.2;
		glutPostRedisplay();
		break;
	case 'u': //ȭ���� ���� �̵�
		orthoTop += 0.2; orthoBottom += 0.2;
		glutPostRedisplay();
		break;
	case 'j': //ȭ���� �Ʒ��� �̵�
		orthoTop -= 0.2; orthoBottom -= 0.2;
		glutPostRedisplay();
		break;
	case 'z': //������ ������ȯ(�������� ȸ��)
		RotateDisplay = RotateDisplay + 2.0;
		break;
	case 'x': //������ ������ȯ(���������� ȸ��)
		RotateDisplay = RotateDisplay - 2.0;
		break;

	}
}

//ȭ��ǥ ���� Ű���带 ����ϱ� ���� �Լ�
void MyKeyboard2(int key, int x, int y) {
	switch (key) {

	case GLUT_KEY_RIGHT:
		LookAt_Z += 0.05; //���� ������ ���� ��ó�� ���δ�.
		glutPostRedisplay();
		break;

	case GLUT_KEY_LEFT:
		if (LookAt_Z>0.5) LookAt_Z -= 0.05; //���� ������ ���̰� �ȴ�. 
		glutPostRedisplay();
		break;

	case GLUT_KEY_UP:
		if (LookAt_Y<-0.05) {
			LookAt_Y += 0.05; //���� �籸�밡 ���� ���� �ȴ�.
			LookAt_Z -= 0.05;
		}
		glutPostRedisplay();
		break;

	case GLUT_KEY_DOWN:
		LookAt_Y -= 0.05; //���ִ� �籸�밡 ���� �ڷ� ���� ����� �ȴ�.
		LookAt_Z += 0.05;
		glutPostRedisplay();
		break;
	}
}

//�� Ÿ�̸ӿ� ���� ���� �������� ǥ���ȴ�.
void MyTimer(int value) {
	//ť�� ������ ���ư���
	if (shot == 1 && movingCount[7] != 0 && st_TransAmount[7]->count[0]>8) {
		st_BallPoint[7].x += st_TransAmount[7]->x;
		st_BallPoint[7].y += st_TransAmount[7]->y;
		st_BallPoint[7].z -= st_TransAmount[7]->z;
		st_TransAmount[7]->count[0] -= 1;
		movingCount[7]--;
	}
	if (st_TransAmount[7]->count[0] == 8) { movingCount[7] = 0; shot = 0; }

	//i������ j������ �΋H���� �� isCaled�� 1�� ��ŷ
	for (int i = 0; i<7; i++) {
		for (int j = 0; j<7; j++) {
			if (i != j) {
				st_TransAmount[i]->count[j] = (int)(Calc_Distance(GET_SKEWED_DIS, i, j) / 0.01);
				if (st_TransAmount[i]->count[j] == 10) st_TransAmount[i]->isCalced[j] = 1;
			}
		}
	}

	//i�� ���� j���� �΋H������ i������ j������ �̵����� ���, ���� ȸ�� �൵ ���
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

	//i������ ���� �̵������� �̵���Ŵ
	for (int b = 0; b<7; b++) {
		if (movingCount[b] != 0) {
			st_BallPoint[b].x += st_TransAmount[b]->x;
			st_BallPoint[b].y += st_TransAmount[b]->y;
			st_BallPoint[b].z += st_TransAmount[b]->z;
			movingCount[b]--;
			st_Rotate[b].angle += 5; if (st_Rotate[b].angle == 360) st_Rotate[b].angle = 0;
		}
	}

	//���� �΋H������ ���Ͽ� ������ üũ
	for (int i = 0; i<7; i++) {
		isBumpedWall(i);
		isPooledPocket(i);
		if (isPooled[i] != 0) movingCount[i] = 0;
	}

	glutPostRedisplay();
	glutTimerFunc(8, MyTimer, 1);
}

//��� �̹����� �׷����� �Լ�
void MyDisplay() {
	glPushMatrix();
	//1������Ʈ - �籸��� �籸�� �׸���
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

	//2������Ʈ - ���� ǥ��
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

//�����Լ�
int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800, 900);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("��ǻ�� �׷��Ƚ� �⸻��� ������Ʈ");
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
