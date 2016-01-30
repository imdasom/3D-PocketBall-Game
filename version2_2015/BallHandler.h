/*
* 파일이름 : BallHandler.h
* 작 성 자 : 임다솜
* 업데이트 정보 : [2015. 10. 23]
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

void BallHandler::CreateBall(int BallNumber) { //BallHandler객체가 함부로 만들어지는 것을 방지하고자 공의 갯수를 입력해야 공 객체가 생성되도록 만들었다.
	c->perspective = ALL;

	for (int i = 0; i<BallNumber; i++) {
		b[i] = new Ball(i);
	}
	Breaking(); //공의 좌표를 새롭게 생성한다.
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

void BallHandler::Breaking() {	//맨처음 공의 절대좌표를 생성한다.

	srand((unsigned)time(NULL)); //좌표값 중복되지않게 그리되
	bool isSuccess = false;

	for (int i = 0; i<CUE; i++) { //좌표생성
		while (isSuccess == false) {
			b[i]->posX = 3.4 * ((GLfloat)rand() / RAND_MAX) - 1.7;
			b[i]->posY = 1.7 * ((GLfloat)rand() / RAND_MAX) - 0.85;

			for (int j = 0; j<CUE; j++) {
				if (i != j) {
					if (sqrt(pow(b[i]->posX - b[j]->posX, 2) + pow(b[i]->posY - b[j]->posY, 2)) <= 0.1)
						// 원점간의 거리가 0.05보다 작다면 중복된것이다. 여유롭게 0.1로 값을 주었다.
						isSuccess = false; //중복된다면 i번 공의 좌표를 다시 생성한다.
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
	//맨 처음에는 큐를 수구의 위치에서 +45도 기울어진 곳에 위치시킨다.
	b[CUE]->posX = 0.45 * -sin(rAngle) + b[0]->posX;
	b[CUE]->posY = 0.45 * cos(rAngle) + b[0]->posY;
}

void BallHandler::isBumpedWall() {
	for (int n = 0; n<CUE; n++) {
		//왼쪽벽에 부딫혓을 경우
		if (b[n]->posX <= (c->leftPos + 0.05)) {
			if ((b[n]->dirX<0 && b[n]->dirY>0) || (b[n]->dirX<0 && b[n]->dirY<0))
				b[n]->dirX *= (-1);
		}
		//위쪽벽에 부딫혔을 경우
		else if (b[n]->posY >= (c->topPos - 0.05)) {
			if ((b[n]->dirX<0 && b[n]->dirY>0) || (b[n]->dirX>0 && b[n]->dirY>0))
				b[n]->dirY *= (-1);
		}
		//(오른쪽벽에 부딫혓을 경우
		else if (b[n]->posX >= (c->rightPos - 0.05)) {
			if ((b[n]->dirX>0 && b[n]->dirY<0) || (b[n]->dirX>0 && b[n]->dirY>0))
				b[n]->dirX *= (-1);
		}
		//아래쪽벽에 부딫혓을 경우
		else if (b[n]->posY <= (c->bottomPos + 0.05)) {
			if ((b[n]->dirX>0 && b[n]->dirY<0) || (b[n]->dirX<0 && b[n]->dirY<0))
				b[n]->dirY *= (-1);
		}
		clt->Get_RotateAxis(n);
	}
}

//공이 포켓에 빠졋나 확인하는 함수
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

//i번공이 j번공에 부딫혔을 때 isCaled를 1로 마킹
void BallHandler::DetectCollision() {
	//큐와 수구의 충돌 판단
	if (cHandler->cuepower <= 0.0 && b[CUE]->isMoving == 1) {
		printf("DetectCollision() 안에 첫번쨰if문\n");
		b[0]->isMoving = 1;
		b[0]->speed = b[CUE]->speed*1.2;
		printf("b[0]->speed = %f\n", b[0]->speed);
		b[0]->dirX = b[CUE]->dirX;
		b[0]->dirY = b[CUE]->dirY;
		clt->Get_RotateAxis(0);
	}
	//일반 다른 공들의 충돌 판단
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
						if (i == 0) printf("DetectCollision() 안에 두번째에있는 for문안\nb[0]->speed = %f\n", b[0]->speed);
						b[j]->speed = b[i]->speed * 0.8; //공의 스피드도 함께 전달한다.
														 //b[i]->speed *= 0.8;
														 //printf("b[%d]->speed *= 0.8\n", i);
														 //b[j]->isMoving=1; //충돌하였으므로 현재(남은) isMoving값이 충돌한 공에 전달된다.
														 //b[i]->isCalced[j]=0; //한번계산되면 또 계산하지 않도록  0으로 막아준다. 다시 충돌이 일어나면 1이 되어 다시 계산한다.
						clt->Get_RotateAxis(i); //충돌이 일어나면 공의 진행방향도 바뀌고 공의 전진회전축도 바뀐다.
												//충돌하면 충돌한 공에 대해서만 각을 계산하여 dir값을 구한다.
						clt->Get_BallDirection(b[i]->speed, i, j);
					}
				}
			}
		}
	}
}

//i번공을 계산된 이동량으로 이동시킴 
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
	b[CUE]->posX = -b[CUE]->dirX + b[0]->posX;  //수구에서부터 움직여가서 그리므로 dir값을 더하지 않고 빼준다.
	b[CUE]->posY = -b[CUE]->dirY + b[0]->posY;

	if (b[0]->wherePooled != 0) {
		b[0]->posX = b[CUE]->axisX; //사용되지않는메모리공간인 큐의 axis를 이용해서 샷하는 순간의 수구의 위치를 저장해놓았다가 다시 대입해준다.
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