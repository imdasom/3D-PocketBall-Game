/*
* �����̸� : BallHandler.h
* �� �� �� : �Ӵټ�
* ������Ʈ ���� : [2015. 10. 23]
*/

#ifndef __BALL_HANDLER_H__
#define __BALL_HANDLER_H__

#pragma once

#include "global.h"
class Ball;
class Constants;
class Calculator;
class CueHandler;
class BallHandler;
class DrawHandler;

class BallHandler {
private:
	Ball** b;
	Calculator* clt;
	Constants* c;
	CueHandler* cHandler;

public:
	BallHandler(Constants* _c);
	void CreateBall(int BallNumber);
	void setBall(Ball** _b);
	void setCalculator(Calculator* _clt);
	void setCueHandler(CueHandler* _cHandler);
	void Breaking();
	void isBumpedWall();
	void isPooledPocket();
	void DetectCollision();
	void MoveBall();
	void InitBallInfo();
	bool isAllBallStop();
};

BallHandler::BallHandler(Constants* _c) {
	c = _c;
}

void BallHandler::CreateBall(int BallNumber) { //BallHandler��ü�� �Ժη� ��������� ���� �����ϰ��� ���� ������ �Է��ؾ� �� ��ü�� �����ǵ��� �������.
	c->perspective = ALL;

	for (int i = 0; i<BallNumber; i++) {
		b[i] = new Ball(i);
	}
	Breaking(); //���� ��ǥ�� ���Ӱ� �����Ѵ�.
	InitBallInfo();
}

void BallHandler::setBall(Ball** _b) {
	b = _b;
}

void BallHandler::setCalculator(Calculator* _clt) {
	clt = _clt;
}

void BallHandler::setCueHandler(CueHandler* _cHandler) {
	cHandler = _cHandler;
}

void BallHandler::Breaking() {	//��ó�� ���� ������ǥ�� �����Ѵ�.

	srand((unsigned)time(NULL)); //��ǥ�� �ߺ������ʰ� �׸���
	bool isSuccess = false;

	for (int i = 0; i<CUE; i++) { //��ǥ����
		while (isSuccess == false) {
			b[i]->posX = 3.4 * ((GLfloat)rand() / RAND_MAX) - 1.7;
			b[i]->posY = 1.7 * ((GLfloat)rand() / RAND_MAX) - 0.85;

			for (int j = 0; j<CUE; j++) {
				if (i != j) {
					if (sqrt(pow(b[i]->posX - b[j]->posX, 2) + pow(b[i]->posY - b[j]->posY, 2)) <= 0.1)
						// �������� �Ÿ��� 0.05���� �۴ٸ� �ߺ��Ȱ��̴�. �����Ӱ� 0.1�� ���� �־���.
						isSuccess = false; //�ߺ��ȴٸ� i�� ���� ��ǥ�� �ٽ� �����Ѵ�.
					else {
						isSuccess = true;
					}
				}
			}
		}
		isSuccess = false;
	}

	b[CUE]->dAngle = 45.0;
	GLfloat rAngle = b[CUE]->dAngle / 180 * PI;
	//�� ó������ ť�� ������ ��ġ���� +45�� ������ ���� ��ġ��Ų��.
	b[CUE]->posX = 0.45 * -sin(rAngle) + b[0]->posX;
	b[CUE]->posY = 0.45 * cos(rAngle) + b[0]->posY;
}

void BallHandler::isBumpedWall() {
	for (int n = 0; n<CUE; n++) {
		//���ʺ��� �΋H���� ���
		if (b[n]->posX <= (c->leftPos + 0.05)) {
			if ((b[n]->dirX<0 && b[n]->dirY>0) || (b[n]->dirX<0 && b[n]->dirY<0))
				b[n]->dirX *= (-1);
		}
		//���ʺ��� �΋H���� ���
		else if (b[n]->posY >= (c->topPos - 0.05)) {
			if ((b[n]->dirX<0 && b[n]->dirY>0) || (b[n]->dirX>0 && b[n]->dirY>0))
				b[n]->dirY *= (-1);
		}
		//(�����ʺ��� �΋H���� ���
		else if (b[n]->posX >= (c->rightPos - 0.05)) {
			if ((b[n]->dirX>0 && b[n]->dirY<0) || (b[n]->dirX>0 && b[n]->dirY>0))
				b[n]->dirX *= (-1);
		}
		//�Ʒ��ʺ��� �΋H���� ���
		else if (b[n]->posY <= (c->bottomPos + 0.05)) {
			if ((b[n]->dirX>0 && b[n]->dirY<0) || (b[n]->dirX<0 && b[n]->dirY<0))
				b[n]->dirY *= (-1);
		}
		clt->Get_RotateAxis(n);
	}
}

//���� ���Ͽ� ������ Ȯ���ϴ� �Լ�
void BallHandler::isPooledPocket() {
	for (int n = 0; n<BALL_NUMBER; n++) {
		if (b[n]->posX<(c->leftPos + 0.2) && b[n]->posY>(c->topPos - 0.2)) {
			b[n]->wherePooled = 1;
			b[n]->posX = c->leftPos; b[n]->posY = c->topPos;
		}
		else if (b[n]->posY<(c->topPos + 0.2) && b[n]->posY>(c->topPos - 0.2) && b[n]->posX<(0.2) && b[n]->posX>(-0.2)) {
			b[n]->wherePooled = 2;
			b[n]->posX = c->rightPos; b[n]->posY = 0.0;
		}
		else if (b[n]->posX>(c->rightPos - 0.2) && b[n]->posY>(c->topPos - 0.2)) {
			b[n]->wherePooled = 3;
			b[n]->posX = c->rightPos; b[n]->posY = c->topPos;
		}
		else if (b[n]->posX>(c->rightPos - 0.2) && b[n]->posY<(c->bottomPos + 0.2)) {
			b[n]->wherePooled = 4;
			b[n]->posX = c->rightPos; b[n]->posY = c->bottomPos;
		}
		else if (b[n]->posY>(c->bottomPos - 0.2) && b[n]->posY<(c->bottomPos + 0.2) && b[n]->posX<(0.2) && b[n]->posX>-0.2) {
			b[n]->wherePooled = 5;
			b[n]->posX = c->leftPos; b[n]->posY = 0.0;
		}
		else if (b[n]->posX<(c->leftPos + 0.2) && b[n]->posY<(c->bottomPos + 0.2)) {
			b[n]->wherePooled = 6;
			b[n]->posX = c->leftPos; b[n]->posY = c->bottomPos;
		}
		if (b[n]->wherePooled != 0) b[n]->isMoving = 0;
	}
}

//i������ j������ �΋H���� �� isCaled�� 1�� ��ŷ
void BallHandler::DetectCollision() {
	//ť�� ������ �浹 �Ǵ�
	if (cHandler->cuepower <= 0.0 && b[CUE]->isMoving == 1) {
		printf("DetectCollision() �ȿ� ù����if��\n");
		b[0]->isMoving = 1;
		b[0]->speed = b[CUE]->speed*1.2;
		printf("b[0]->speed = %f\n", b[0]->speed);
		b[0]->dirX = b[CUE]->dirX;
		b[0]->dirY = b[CUE]->dirY;
		clt->Get_RotateAxis(0);
	}
	//�Ϲ� �ٸ� ������ �浹 �Ǵ�
	for (int i = 0; i<CUE; i++) {
		for (int j = 0; j<CUE; j++) {
			if (i != j) {
				GLfloat distance = clt->Get_Distance(GET_SKEWED_DIS, i, j);
				if (distance <= 0.131 && distance >= 0.101) {
					if (b[i]->isCalced[j] != 1 && b[i]->isMoving == 1) {
						b[i]->isCalced[j] = 1;
						//b[i]->isMoving = 1;
						b[j]->isMoving = 1;
						b[i]->speed *= 0.8;
						if (i == 0) printf("DetectCollision() �ȿ� �ι�°���ִ� for����\nb[0]->speed = %f\n", b[0]->speed);
						b[j]->speed = b[i]->speed * 0.8; //���� ���ǵ嵵 �Բ� �����Ѵ�.
														 //b[i]->speed *= 0.8;
														 //printf("b[%d]->speed *= 0.8\n", i);
														 //b[j]->isMoving=1; //�浹�Ͽ����Ƿ� ����(����) isMoving���� �浹�� ���� ���޵ȴ�.
														 //b[i]->isCalced[j]=0; //�ѹ����Ǹ� �� ������� �ʵ���  0���� �����ش�. �ٽ� �浹�� �Ͼ�� 1�� �Ǿ� �ٽ� ����Ѵ�.
						clt->Get_RotateAxis(i); //�浹�� �Ͼ�� ���� ������⵵ �ٲ�� ���� ����ȸ���൵ �ٲ��.
												//�浹�ϸ� �浹�� ���� ���ؼ��� ���� ����Ͽ� dir���� ���Ѵ�.
						clt->Get_BallDirection(b[i]->speed, i, j);
					}
				}
			}
		}
	}
}

//i������ ���� �̵������� �̵���Ŵ 
void BallHandler::MoveBall() {
	for (int i = 0; i<BALL_NUMBER; i++) {
		if (i == 0 || i == 7)
			printf("b[%d]->isMoving = %d\n", i, b[i]->isMoving);
		if (b[i]->isMoving == 1) {
			b[i]->dirX *= 0.997;
			b[i]->dirY *= 0.997;
			b[i]->speed *= 0.997;
			if (i == CUE) {
				if (cHandler->cuepower >= -cHandler->ccp) {
					cHandler->cuepower -= b[CUE]->speed;
				}
				else b[CUE]->speed = 0.0;
			}
			b[i]->posX += b[i]->dirX;
			b[i]->posY += b[i]->dirY;
			if (i == 0 || i == 7)
				printf("b[%d]->speed : ( %f )\n", i, b[i]->speed);
			if (b[i]->speed <= 0.002) {
				b[i]->isMoving = 0;
				if (i == 0 || i == 7)
					printf("b[%d]->isMoving = 0\n", i);
			}
			if (i != CUE) {
				b[i]->dAngle += 5;
				if (b[i]->dAngle == 360) b[i]->dAngle = 0;
			}
		}
	}
	printf("\n");
}

void BallHandler::InitBallInfo() {
	clt->Get_CueDirection(cHandler->inc + cHandler->cuepower, CUE, 0);
	b[CUE]->posX = -b[CUE]->dirX + b[0]->posX;  //������������ ���������� �׸��Ƿ� dir���� ������ �ʰ� ���ش�.
	b[CUE]->posY = -b[CUE]->dirY + b[0]->posY;

	if (b[0]->wherePooled != 0) {
		b[0]->posX = b[CUE]->axisX; //�������ʴ¸޸𸮰����� ť�� axis�� �̿��ؼ� ���ϴ� ������ ������ ��ġ�� �����س��Ҵٰ� �ٽ� �������ش�.
		b[0]->posY = b[CUE]->axisY;
		c->gameoverCount--;
		b[0]->wherePooled = 0;
	}
	for (int i = 0; i<BALL_NUMBER; i++) {
		b[i]->speed = 0.0;
		b[i]->isMoving = 0;
		for (int j = 0; j<BALL_NUMBER; j++) {
			b[i]->isCalced[j] = 0;
			b[i]->collisionCount[j] = 0;
		}
		b[i]->dirX = 0.0; b[i]->dirY = 0.0;
	}
}

bool BallHandler::isAllBallStop() {
	int result = 0;
	for (int i = 0; i<LINE; i++) {
		result += b[i]->isMoving;
	}
	if (result == 0) return true;
	else return false;
}

#endif