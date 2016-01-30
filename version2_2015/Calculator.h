/*
* 파일이름 : Calculator.h
* 작 성 자 : 임다솜
* 업데이트 정보 : [2015. 10. 23]
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

//계산기 클래스입니다. 여러가지 계산 함수들을 가지고 있습니다.
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

//거리를 계산해서 리턴하는 메소드입니다.
//공 객체와 계산 타입을 매개변수로 받아서 계산타입만을 결과로 리턴합니다.
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

//좌표의 몇사분면인지 케이스를 나눠서 리턴하는 메소드
//사분면 판단은 n1의 위치에서 n2의 위치를 빼서 계산하는 기준입니다.
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

//radian, degree 각을 구해주는 메소드
//radian, degree 중 필요한 값을 type에 넘겨주어서 필요한 값을 리턴받을 수 있습니다.
GLfloat Calculator::Get_Angle(int type, int n1, int n2) {
	GLfloat disX = Get_Distance(GET_X_DIS, n1, n2);
	GLfloat disY = Get_Distance(GET_Y_DIS, n1, n2);

	if (type == RADIAN) { return atan2(fabs(disX), fabs(disY)); }
	else if (type == DEGREE) { return atan2(fabs(disX), fabs(disY)) * 180 / PI; }
}

//공이 앞으로 굴러갈 때 회전축을 계산해주는 함수
//이 axisX,Y 값으로 공이 어느각도로 회전할지를 결정하게 됩니다.
void Calculator::Get_RotateAxis(int n1) {
	//공의 x축 증가량/y축 증가량이 대각선 증가량이 되고 그 증가량이 회전축값이 됩니다.
	//먼저 공의 x,y축 진행방향이 축값을 결정하기 때문에 값을 대입해 줍니다.
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

//키보드로 조작하며, 큐가 공 주위를 회전하도록 하는 함수입니다. (x, y 증가량도 계산합니다)
//1stVersion에서 마우스로 조작하던 큐를 키보드로 조작하도록 변경하였는데
//Get_CuePoint()와 Get_CueAngle()을 제거하고 Get_CueDirection()을 구현하였습니다.
//큐와 관련된 함수이지만 큐클래스에 넣지 않았습니다.
//이유는 단순히 큐에만 해당하지 않고 다른 공에도 사용할 수 있도록 하기 위해서 입니다. 
void Calculator::Get_CueDirection(GLfloat f_inc, int n1, int n2) {
	int _case = 0, signX = 0, signY = 0;
	GLfloat rAngle = 0.0, dAngle = 0.0;

	if (n1 == CUE || n1 == LINE) { //큐와 수구의 위치관계를 _case로 구분하여 줍니다.
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

	switch (_case) { //케이스에 따라 x,y 증가량의 부호가 달라집니다.
	case 1:	signX = +1; signY = -1;	break;
	case 2:	signX = +1; signY = +1;	break;
	case 3:	signX = -1; signY = +1;	break;
	case 4:	signX = -1; signY = -1;	break;
	}
	//최종으로 부호와 값과 각도를 모두 곱해서 증가량을 도출해 줍니다.
	b[n1]->dirX = (f_inc)* signX * cos(rAngle);
	b[n1]->dirY = (f_inc)* signY * sin(rAngle);
	if (n1 == CUE) printf("Get_CueDirection:: 큐->dirX, Y = %f, %f\n", b[n1]->dirX, b[n1]->dirY);
}

//공의 이동방향(x축 y축의 이동량)을 계산하는 함수입니다.
//version1_2014에서 
//void Calc_TransAmount(GLfloat inc, GLfloat angle_radian, int BaseNum, int SubNum) 함수를
//불필요한 매개변수를 제거하여 변경하였습니다.
//또, TransAmount라는 이름이 명확하지 않다 생각하여 Get_BallDirection으로 변경해주었습니다.
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