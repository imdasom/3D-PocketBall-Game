/*
* 파일이름 : CueHandler.h
* 작 성 자 : 임다솜
* 업데이트 정보 : [2015. 10. 23]
*/

#ifndef __CUE_HANDLER_H__
#define __CUE_HANDLER_H__

#pragma once

#include "global.h"
class Ball;
class Constants;
class Calculator;
class CueHandler;
class BallHandler;
class DrawHandler;

class CueHandler {
private:
	Ball** b;
	Ball *line;
	Calculator* clt;
	Constants* c;

public:
	GLfloat cuepower; //cuepower = 큐의파워 = 큐끝과 수구중심의 거리.
	GLfloat ccp; //cuepower는 큐가 앞으로 나아가면서 값이 변하므로 이 변수에 초기값을 저장한다.
	GLfloat inc; //공과 큐 사이의 적당한 거리를 유지하도록 하는 고정 상수.

public:
	CueHandler(Constants* _c);
	void setBall(Ball** _b);
	void setCalculator(Calculator* _clt);
	void MoveCue(int key);
	void DrawCueLine();
	void Shot();
	void InitCueInfo();
};

CueHandler::CueHandler(Constants* _c) {
	c = _c;
	InitCueInfo();
	cuepower = ccp = 0.05; //큐파워를 저장하는 변수. 이 값은 큐와 수구의 거리를 의미한다. 수구와 큐의 거리 = 큐파워.
	inc = 0.40; //공과 큐 사이의 적당한 거리를 유지하도록 하는 고정 상수.
}

void CueHandler::setBall(Ball** _b) {
	b = _b;
}

void CueHandler::setCalculator(Calculator* _clt) {
	clt = _clt;
}

void CueHandler::MoveCue(int key) {
	if (key != 0) {   //만약 key가 0이 아니면 숫자키를 누른것이다. 그래서 누른 공에 대한 각도로 큐를 기울여야 하므로 dir을 다시계산한다.
		int	disCase = clt->Get_DistanceCase(0, key);
		GLfloat angle_D = clt->Get_Angle(DEGREE, 0, key);

		if (disCase == 1) {
			b[CUE]->dAngle = angle_D;
		}
		else if (disCase == 2) {
			b[CUE]->dAngle = 180 - angle_D;
		}
		else if (disCase == 3) {
			b[CUE]->dAngle = 180 + angle_D;
		}
		else if (disCase == 4) {
			b[CUE]->dAngle = 360 - angle_D;
		}

		clt->Get_CueDirection(inc + cuepower, CUE, 0);
		b[CUE]->posX = -b[CUE]->dirX + b[0]->posX;  //수구에서부터 움직여가서 그리므로 dir값을 더하지 않고 빼준다.
		b[CUE]->posY = -b[CUE]->dirY + b[0]->posY;
	}
}

void CueHandler::DrawCueLine() {
	bool isCnt = true;
	clt->Get_CueDirection(inc, LINE, 0);
	b[LINE]->posX = b[CUE]->posX + b[LINE]->dirX;
	b[LINE]->posY = b[CUE]->posY + b[LINE]->dirY;
	clt->Get_CueDirection(0.05, LINE, 0);

	while (isCnt) {

		b[LINE]->posX += b[LINE]->dirX;
		b[LINE]->posY += b[LINE]->dirY;

		glPushMatrix();
		glTranslatef((GLfloat)b[LINE]->posX, (GLfloat)b[LINE]->posY, 0.05);
		glColor3f(0.0, 0.0, 0.0);
		glutSolidCube(0.01);
		glPopMatrix();

		//왼쪽벽에 부딫혔을 경우
		if (b[LINE]->posX <= (c->leftPos + 0.05)) {
			isCnt = false;
		}
		//위쪽벽에 부딫혔을 경우
		else if (b[LINE]->posY >= (c->topPos - 0.05)) {
			isCnt = false;
		}
		//(오른쪽벽에 부딫혓을 경우
		else if (b[LINE]->posX >= (c->rightPos - 0.05)) {
			isCnt = false;
		}
		//아래쪽벽에 부딫혓을 경우5
		else if (b[LINE]->posY <= (c->bottomPos + 0.05)) {
			isCnt = false;
		}
	}
}

void CueHandler::Shot() {
	ccp = cuepower;
	b[CUE]->speed = cuepower / 10;
	b[CUE]->isMoving = 1;
	b[CUE]->axisX = b[0]->posX; //사용되지 않는 변수라서 나중에 변하게 될 원래 b0의 pos를 저장하는데 사용하였다.
	b[CUE]->axisY = b[0]->posY;
	clt->Get_CueDirection(b[CUE]->speed, CUE, 0);
}

void CueHandler::InitCueInfo() {
	cuepower = ccp = 0.05;
}

#endif