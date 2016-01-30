/*
* �����̸� : Ball.h
* �� �� �� : �Ӵټ�
* ������Ʈ ���� : [2015. 10. 23]
*/

#ifndef __BALL_H__
#define __BALL_H__

#pragma once

#include "global.h"

//���� ��ǥ, �����ӻ���, ȸ������ ���� �����͸� �����ϴ� Ŭ�����Դϴ�.
class Ball {
public:
	int bnum;
	bool isMoving;		//���� ���� �����̰� �ִ� �������� �����ϴ� �����Դϴ�.
	bool wherePooled;	//���� ���Ͽ� �������� ���θ� �����ϴ� �����Դϴ�.
	GLfloat speed;		//���� ���ǵ带 �����ϴ� �����Դϴ�.
	GLfloat posX, posY, posZ;	//���� ��ǥ���� �����ϴ� �����Դϴ�.
	GLfloat axisX, axisY, axisZ;//���� ������ �������� ȸ���ϰ� �Ǵµ� 
								//�������� ���� �̵��������� ���ϴ� ������ ȸ������ �ǰ� �� �ప�� ����ϱ� ���� �ʿ��� �����Դϴ�.
	GLfloat dAngle;		//dAngle�� ���� ����ȸ���Ҷ� �� ���� �����ϴ� �����Դϴ�. 
						//��, CUE�� ���� ���ؼ��� ť�� ������ �̷�� ���� �����մϴ�.
	GLfloat dirX, dirY, dirZ;	//���� ��������Դϴ�. dirX�� 0.5�̸� +�������� 0.5��ŭ �̵��ϴ� ����Դϴ�.
	GLfloat collisionCount[BALL_NUMBER];//b[CUE]������ �� ������ ������� �����Ƿ� �ٸ� ���� �����ϴµ� ����� ���̴�.
	int isCalced[BALL_NUMBER]; //x�� ���� �ε����� �迭�� x�ε����� 1�� �����Ͽ� üũ�ϴ� �����Դϴ�.

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