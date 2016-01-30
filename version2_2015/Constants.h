/*
* �����̸� : Constants.h
* �� �� �� : �Ӵټ�
* ������Ʈ ���� : [2015. 10. 23]
*/

#ifndef __CONSTANTS_H__
#define __CONSTANTS_H__

#pragma once

#include "global.h"

class Constants {
public:
	//������ ���õ� ������ �Դϴ�.
	int perspective;
	GLfloat leftPos, rightPos, topPos, bottomPos;
	GLfloat eyeX, eyeY, eyeZ;
	GLfloat centerX, centerY, centerZ;
	GLfloat orthoLeft, orthoRight, orthoBottom, orthoTop;

	int isShooted;
	int gameoverCount; //������ ���Ͽ� 3�� ������ ���ӿ� �����ϰ� �ǰ� �������� �ٽ� �����մϴ�.
	GLfloat RotateDisplay;
	GLfloat RotateDisplayZOOM;

public:
	Constants();
};

Constants::Constants() {
	//������ ���õ� ������ �Դϴ�.
	perspective = 0;
	leftPos = -4.0, rightPos = 4.0, topPos = 2.0, bottomPos = -2.0;
	eyeX = 0.0, eyeY = -6.7800286e-008, eyeZ = 3.5;
	centerX = 0.0, centerY = 0.0, centerZ = 0.0;
	orthoLeft = -4.0, orthoRight = 4.0, orthoBottom = -2.0, orthoTop = 2.0;

	isShooted = 0;
	gameoverCount = 3; //������ ���Ͽ� 3�� ������ ���ӿ� �����ϰ� �ǰ� �������� �ٽ� �����մϴ�.
	RotateDisplay = 0.0;
	RotateDisplayZOOM = 0.0;
}

#endif