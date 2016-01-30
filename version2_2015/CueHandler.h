/*
* �����̸� : CueHandler.h
* �� �� �� : �Ӵټ�
* ������Ʈ ���� : [2015. 10. 23]
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
	GLfloat cuepower; //cuepower = ť���Ŀ� = ť���� �����߽��� �Ÿ�.
	GLfloat ccp; //cuepower�� ť�� ������ ���ư��鼭 ���� ���ϹǷ� �� ������ �ʱⰪ�� �����Ѵ�.
	GLfloat inc; //���� ť ������ ������ �Ÿ��� �����ϵ��� �ϴ� ���� ���.

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
	cuepower = ccp = 0.05; //ť�Ŀ��� �����ϴ� ����. �� ���� ť�� ������ �Ÿ��� �ǹ��Ѵ�. ������ ť�� �Ÿ� = ť�Ŀ�.
	inc = 0.40; //���� ť ������ ������ �Ÿ��� �����ϵ��� �ϴ� ���� ���.
}

void CueHandler::setBall(Ball** _b) {
	b = _b;
}

void CueHandler::setCalculator(Calculator* _clt) {
	clt = _clt;
}

void CueHandler::MoveCue(int key) {
	if (key != 0) {   //���� key�� 0�� �ƴϸ� ����Ű�� �������̴�. �׷��� ���� ���� ���� ������ ť�� ��￩�� �ϹǷ� dir�� �ٽð���Ѵ�.
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
		b[CUE]->posX = -b[CUE]->dirX + b[0]->posX;  //������������ ���������� �׸��Ƿ� dir���� ������ �ʰ� ���ش�.
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

		//���ʺ��� �΋H���� ���
		if (b[LINE]->posX <= (c->leftPos + 0.05)) {
			isCnt = false;
		}
		//���ʺ��� �΋H���� ���
		else if (b[LINE]->posY >= (c->topPos - 0.05)) {
			isCnt = false;
		}
		//(�����ʺ��� �΋H���� ���
		else if (b[LINE]->posX >= (c->rightPos - 0.05)) {
			isCnt = false;
		}
		//�Ʒ��ʺ��� �΋H���� ���5
		else if (b[LINE]->posY <= (c->bottomPos + 0.05)) {
			isCnt = false;
		}
	}
}

void CueHandler::Shot() {
	ccp = cuepower;
	b[CUE]->speed = cuepower / 10;
	b[CUE]->isMoving = 1;
	b[CUE]->axisX = b[0]->posX; //������ �ʴ� ������ ���߿� ���ϰ� �� ���� b0�� pos�� �����ϴµ� ����Ͽ���.
	b[CUE]->axisY = b[0]->posY;
	clt->Get_CueDirection(b[CUE]->speed, CUE, 0);
}

void CueHandler::InitCueInfo() {
	cuepower = ccp = 0.05;
}

#endif