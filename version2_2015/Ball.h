/*
* 파일이름 : Ball.h
* 작 성 자 : 임다솜
* 업데이트 정보 : [2015. 10. 23]
*/

#ifndef __BALL_H__
#define __BALL_H__

#pragma once

#include "global.h"

//공의 좌표, 움직임상태, 회전각도 등의 데이터를 저장하는 클래스입니다.
class Ball {
public:
	int bnum;
	bool isMoving;		//공이 현재 움직이고 있는 상태인지 저장하는 변수입니다.
	bool wherePooled;	//공이 포켓에 빠졌는지 여부를 저장하는 변수입니다.
	GLfloat speed;		//공의 스피드를 저장하는 변수입니다.
	GLfloat posX, posY, posZ;	//공의 좌표값을 저장하는 변수입니다.
	GLfloat axisX, axisY, axisZ;//공이 앞으로 굴러가며 회전하게 되는데 
								//원점에서 공의 이동방향으로 향하는 각도가 회전각이 되고 그 축값을 계산하기 위해 필요한 변수입니다.
	GLfloat dAngle;		//dAngle은 공이 전진회전할때 그 각을 저장하는 변수입니다. 
						//단, CUE번 공에 대해서는 큐와 수구가 이루는 각을 저장합니다.
	GLfloat dirX, dirY, dirZ;	//공의 진행방향입니다. dirX가 0.5이면 +방향으로 0.5만큼 이동하는 방식입니다.
	GLfloat collisionCount[BALL_NUMBER];//b[CUE]에서는 이 변수를 사용하지 않으므로 다른 값을 저장하는데 사용할 것이다.
	int isCalced[BALL_NUMBER]; //x번 공과 부딪히면 배열의 x인덱스에 1을 저장하여 체크하는 변수입니다.

public:
	Ball();
	Ball(int number);

};

Ball::Ball() {
	bnum = 0;
	isMoving = 0;
	wherePooled = 0;
	speed = 1.0;
	posX = posY = posZ = 0.0;
	axisX = axisY = axisZ = 0.0;
	dAngle = 0.0;
	dirX = dirY = dirZ = 0.0;
	for (int i = 0; i<BALL_NUMBER; i++) {
		collisionCount[i] = 0.0;
		isCalced[i] = 0;
	}
}

Ball::Ball(int number) {
	bnum = number;
	isMoving = 0;
	wherePooled = 0;
	speed = 1.0;
	posX = posY = posZ = 0.0;
	axisX = axisY = axisZ = 0.0;
	dAngle = 0.0;
	dirX = dirY = dirZ = 0.0;
	for (int i = 0; i<BALL_NUMBER; i++) {
		collisionCount[i] = 0.0;
		isCalced[i] = 0;
	}
}

#endif