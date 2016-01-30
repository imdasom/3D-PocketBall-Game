/*
* �����̸� : Calculator.h
* �� �� �� : �Ӵټ�
* ������Ʈ ���� : [2015. 10. 23]
*/

#ifndef __CALCULATOR_H__
#define __CALCULATOR_H__

#pragma once

#include "global.h"
class Ball;
class Constants;
class Calculator;
class CueHandler;
class BallHandler;
class DrawHandler;

//���� Ŭ�����Դϴ�. �������� ��� �Լ����� ������ �ֽ��ϴ�.
class Calculator {
private:
	Ball** b;
	Constants* c;

public:
	Calculator();
	Calculator(Constants* _c);
	void setBall(Ball** _b);
	void setBallHandler();
	GLfloat Get_Distance(int type, int n1, int n2);
	int Get_DistanceCase(int n1, int n2);
	GLfloat Get_Angle(int type, int n1, int n2);
	void Get_RotateAxis(int n1);
	void Get_CueDirection(GLfloat f_inc, int n1, int n2);
	void Get_BallDirection(GLfloat f_inc, int n1, int n2);

};

Calculator::Calculator() {

}

Calculator::Calculator(Constants* _c) {

}

void Calculator::setBall(Ball** _b) {
	b = _b;
}

void Calculator::setBallHandler() {

}

//�Ÿ��� ����ؼ� �����ϴ� �޼ҵ��Դϴ�.
//�� ��ü�� ��� Ÿ���� �Ű������� �޾Ƽ� ���Ÿ�Ը��� ����� �����մϴ�.
GLfloat Calculator::Get_Distance(int type, int n1, int n2) {
	switch (type) {
	case GET_X_DIS:
		return b[n1]->posX - b[n2]->posX;
	case GET_Y_DIS:
		return b[n1]->posY - b[n2]->posY;
	case GET_Z_DIS:
		return b[n1]->posZ - b[n2]->posZ;
	case GET_SKEWED_DIS:
		return sqrt(pow(Get_Distance(GET_X_DIS, n1, n2), 2) + pow(Get_Distance(GET_Y_DIS, n1, n2), 2));
	}
}

//��ǥ�� ���и����� ���̽��� ������ �����ϴ� �޼ҵ�
//��и� �Ǵ��� n1�� ��ġ���� n2�� ��ġ�� ���� ����ϴ� �����Դϴ�.
int Calculator::Get_DistanceCase(int n1, int n2) {
	GLfloat disX = Get_Distance(GET_X_DIS, n1, n2);
	GLfloat disY = Get_Distance(GET_Y_DIS, n1, n2);
	printf("----Get_DistanceCase called!!----\n");
	printf("%d-%d dis : ( %f, %f )\n", n1, n2, disX, disY);
	if (disX<0 && disY>0) return 1;
	else if (disX<0 && disY<0) return 2;
	else if (disX>0 && disY<0) return 3;
	else if (disX>0 && disY>0) return 4;
	else return 0;
}

//radian, degree ���� �����ִ� �޼ҵ�
//radian, degree �� �ʿ��� ���� type�� �Ѱ��־ �ʿ��� ���� ���Ϲ��� �� �ֽ��ϴ�.
GLfloat Calculator::Get_Angle(int type, int n1, int n2) {
	GLfloat disX = Get_Distance(GET_X_DIS, n1, n2);
	GLfloat disY = Get_Distance(GET_Y_DIS, n1, n2);

	if (type == RADIAN) { return atan2(fabs(disX), fabs(disY)); }
	else if (type == DEGREE) { return atan2(fabs(disX), fabs(disY)) * 180 / PI; }
}

//���� ������ ������ �� ȸ������ ������ִ� �Լ�
//�� axisX,Y ������ ���� ��������� ȸ�������� �����ϰ� �˴ϴ�.
void Calculator::Get_RotateAxis(int n1) {
	//���� x�� ������/y�� �������� �밢�� �������� �ǰ� �� �������� ȸ���ప�� �˴ϴ�.
	//���� ���� x,y�� ��������� �ప�� �����ϱ� ������ ���� ������ �ݴϴ�.
	b[n1]->axisX = b[n1]->dirY;
	b[n1]->axisY = b[n1]->dirX;

	if ((b[n1]->dirX<0 && b[n1]->dirY>0) ||
		(b[n1]->dirX>0 && b[n1]->dirY<0)) {
		if (b[n1]->axisX<0) b[n1]->axisX *= (-1);
		if (b[n1]->axisY<0) b[n1]->axisY *= (-1);
	}
	if ((b[n1]->dirX>0 && b[n1]->dirY>0) ||
		(b[n1]->dirX<0 && b[n1]->dirY<0)) {
		if (b[n1]->axisX<0) b[n1]->axisX *= (-1);
		if (b[n1]->axisY>0) b[n1]->axisY *= (-1);
	}
}

//Ű����� �����ϸ�, ť�� �� ������ ȸ���ϵ��� �ϴ� �Լ��Դϴ�. (x, y �������� ����մϴ�)
//1stVersion���� ���콺�� �����ϴ� ť�� Ű����� �����ϵ��� �����Ͽ��µ�
//Get_CuePoint()�� Get_CueAngle()�� �����ϰ� Get_CueDirection()�� �����Ͽ����ϴ�.
//ť�� ���õ� �Լ������� ťŬ������ ���� �ʾҽ��ϴ�.
//������ �ܼ��� ť���� �ش����� �ʰ� �ٸ� ������ ����� �� �ֵ��� �ϱ� ���ؼ� �Դϴ�. 
void Calculator::Get_CueDirection(GLfloat f_inc, int n1, int n2) {
	int _case = 0, signX = 0, signY = 0;
	GLfloat rAngle = 0.0, dAngle = 0.0;

	if (n1 == CUE || n1 == LINE) { //ť�� ������ ��ġ���踦 _case�� �����Ͽ� �ݴϴ�.
		dAngle = b[CUE]->dAngle;
		if (dAngle >= 0 && dAngle<90) {
			_case = 1;
			dAngle = 90 - dAngle;
		}
		else if (dAngle >= 90 && dAngle<180) {
			_case = 2;
			dAngle = dAngle - 90;
		}
		else if (dAngle >= 180 && dAngle<270) {
			_case = 3;
			dAngle = 270 - dAngle;
		}
		else if (dAngle >= 270 && dAngle<360) {
			_case = 4;
			dAngle = dAngle - 270;
		}
		rAngle = dAngle / 180 * PI;

	}
	else if (n1 != CUE && n1 != LINE) {
		dAngle = Get_Angle(DEGREE, n1, n2);
		rAngle = Get_Angle(RADIAN, n1, n2);
		_case = Get_DistanceCase(n1, n2);
	}

	switch (_case) { //���̽��� ���� x,y �������� ��ȣ�� �޶����ϴ�.
	case 1:	signX = +1; signY = -1;	break;
	case 2:	signX = +1; signY = +1;	break;
	case 3:	signX = -1; signY = +1;	break;
	case 4:	signX = -1; signY = -1;	break;
	}
	//�������� ��ȣ�� ���� ������ ��� ���ؼ� �������� ������ �ݴϴ�.
	b[n1]->dirX = (f_inc)* signX * cos(rAngle);
	b[n1]->dirY = (f_inc)* signY * sin(rAngle);
	if (n1 == CUE) printf("Get_CueDirection:: ť->dirX, Y = %f, %f\n", b[n1]->dirX, b[n1]->dirY);
}

//���� �̵�����(x�� y���� �̵���)�� ����ϴ� �Լ��Դϴ�.
//version1_2014���� 
//void Calc_TransAmount(GLfloat inc, GLfloat angle_radian, int BaseNum, int SubNum) �Լ���
//���ʿ��� �Ű������� �����Ͽ� �����Ͽ����ϴ�.
//��, TransAmount��� �̸��� ��Ȯ���� �ʴ� �����Ͽ� Get_BallDirection���� �������־����ϴ�.
void Calculator::Get_BallDirection(GLfloat f_inc, int n1, int n2) {

	int disCase = Get_DistanceCase(n1, n2);
	int dirCase = 0;
	GLfloat clsAngle_d = Get_Angle(DEGREE, n1, n2);
	GLfloat clsAngle_r = Get_Angle(RADIAN, n1, n2);

	GLfloat dirX = b[n1]->dirX;
	GLfloat dirY = b[n1]->dirY;
	GLfloat dirAngle_d = atan2(dirX, dirY) * 180 / PI;

	if (dirX>0 && dirY<0) dirCase = 1;
	else if (dirX>0 && dirY>0) dirCase = 2;
	else if (dirX<0 && dirY>0) dirCase = 3;
	else if (dirX<0 && dirY<0) dirCase = 4;

	switch (disCase) {
	case 1:
		if ((dirCase == 1 && dirAngle_d > clsAngle_d) ||
			(dirCase == 4 && dirAngle_d > 90 - clsAngle_d)) {
			b[n1]->dirX = -f_inc*cos(clsAngle_r);
			b[n1]->dirY = -f_inc*sin(clsAngle_r);
			b[n2]->dirX = f_inc*sin(clsAngle_r)*0.8;
			b[n2]->dirY = -f_inc*cos(clsAngle_r)*0.8;
		}
		else if ((dirCase == 1 && dirAngle_d < clsAngle_d) ||
			(dirCase == 2 && dirAngle_d < 90 - clsAngle_d)) {
			b[n1]->dirX = f_inc*cos(clsAngle_r);
			b[n1]->dirY = f_inc*sin(clsAngle_r);
			b[n2]->dirX = f_inc*sin(clsAngle_r)*0.8;
			b[n2]->dirY = -f_inc*cos(clsAngle_r)*0.8;
		}
		else if (dirCase == 1 && (int)dirAngle_d == (int)clsAngle_d) {
			b[n2]->dirX = b[n1]->dirX*0.8;
			b[n2]->dirY = b[n1]->dirY*0.8;
			b[n1]->dirX = 0.0;
			b[n1]->dirY = 0.0;
			b[n2]->speed = b[n1]->speed;
			b[n2]->isMoving = 1;
			b[n1]->speed = 0.0;
		}
		break;

	case 2:
		if ((dirCase == 1 && dirAngle_d < clsAngle_d) ||
			(dirCase == 2 && dirAngle_d < 90 - clsAngle_d)) {
			b[n1]->dirX = -f_inc*cos(clsAngle_r);
			b[n1]->dirY = f_inc*sin(clsAngle_r);
			b[n2]->dirX = f_inc*sin(clsAngle_r)*0.8;
			b[n2]->dirY = f_inc*cos(clsAngle_r)*0.8;
		}
		else if ((dirCase == 2 && dirAngle_d > clsAngle_d) ||
			(dirCase == 3 && dirAngle_d > 90 - clsAngle_d)) {
			b[n1]->dirX = f_inc*cos(clsAngle_r);
			b[n1]->dirY = -f_inc*sin(clsAngle_r);
			b[n2]->dirX = f_inc*sin(clsAngle_r)*0.8;
			b[n2]->dirY = f_inc*cos(clsAngle_r)*0.8;
		}
		else if (dirCase == 2 && (int)dirAngle_d == (int)clsAngle_d) {
			b[n2]->dirX = b[n1]->dirX*0.8;
			b[n2]->dirY = b[n1]->dirY*0.8;
			b[n1]->dirX = 0.0;
			b[n1]->dirY = 0.0;
			b[n2]->speed = b[n1]->speed;
			b[n2]->isMoving = 1;
			b[n1]->speed = 0.0;
		}
		break;

	case 3:
		if ((dirCase == 2 && dirAngle_d > 90 - clsAngle_d) ||
			(dirCase == 3 && dirAngle_d > clsAngle_d)) {
			b[n1]->dirX = f_inc*cos(clsAngle_r);
			b[n1]->dirY = f_inc*sin(clsAngle_r);
			b[n2]->dirX = -f_inc*sin(clsAngle_r)*0.8;
			b[n2]->dirY = f_inc*cos(clsAngle_r)*0.8;
		}
		else if ((dirCase == 3 && dirAngle_d < clsAngle_d) ||
			(dirCase == 4 && dirAngle_d > clsAngle_d)) {
			b[n1]->dirX = -f_inc*cos(clsAngle_r);
			b[n1]->dirY = -f_inc*sin(clsAngle_r);
			b[n2]->dirX = -f_inc*sin(clsAngle_r)*0.8;
			b[n2]->dirY = f_inc*cos(clsAngle_r)*0.8;
		}
		else if (dirCase == 3 && (int)dirAngle_d == (int)clsAngle_d) {
			printf("case %d - 90angle!!\n", disCase);
			b[n2]->dirX = b[n1]->dirX*0.8;
			b[n2]->dirY = b[n1]->dirY*0.8;
			b[n1]->dirX = 0.0;
			b[n1]->dirY = 0.0;
			b[n2]->speed = b[n1]->speed;
			b[n2]->isMoving = 1;
			b[n1]->speed = 0.0;
		}
		break;

	case 4:
		if ((dirCase == 1 && dirAngle_d > 90 - clsAngle_d) ||
			(dirCase == 4 && dirAngle_d > clsAngle_d)) {
			b[n1]->dirX = f_inc*cos(clsAngle_r);
			b[n1]->dirY = -f_inc*sin(clsAngle_r);
			b[n2]->dirX = f_inc*sin(clsAngle_r)*0.8;
			b[n2]->dirY = -f_inc*cos(clsAngle_r)*0.8;
		}
		else if ((dirCase == 3 && dirAngle_d < 90 - clsAngle_d) ||
			(dirCase == 4 && dirAngle_d < clsAngle_d)) {
			b[n1]->dirX = -f_inc*cos(clsAngle_r);
			b[n1]->dirY = f_inc*sin(clsAngle_r);
			b[n2]->dirX = f_inc*sin(clsAngle_r)*0.8;
			b[n2]->dirY = -f_inc*cos(clsAngle_r)*0.8;
		}
		else if (dirCase == 4 && (int)dirAngle_d == (int)clsAngle_d) {
			printf("case %d - 90angle!!\n", disCase);
			b[n2]->dirX = b[n1]->dirX*0.8;
			b[n2]->dirY = b[n1]->dirY*0.8;
			b[n1]->dirX = 0.0;
			b[n1]->dirY = 0.0;
			b[n2]->speed = b[n1]->speed;
			b[n2]->isMoving = 1;
			b[n1]->speed = 0.0;
		}
		break;
	}
}

#endif