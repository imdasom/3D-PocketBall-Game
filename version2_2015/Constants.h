/*
* 파일이름 : Constants.h
* 작 성 자 : 임다솜
* 업데이트 정보 : [2015. 10. 23]
*/

#ifndef __CONSTANTS_H__
#define __CONSTANTS_H__

#pragma once

#include "global.h"

class Constants {
public:
	//시점과 관련된 변수들 입니다.
	int perspective;
	GLfloat leftPos, rightPos, topPos, bottomPos;
	GLfloat eyeX, eyeY, eyeZ;
	GLfloat centerX, centerY, centerZ;
	GLfloat orthoLeft, orthoRight, orthoBottom, orthoTop;

	int isShooted;
	int gameoverCount; //수구가 포켓에 3번 빠지면 게임에 실패하게 되고 새게임을 다시 시작합니다.
	GLfloat RotateDisplay;
	GLfloat RotateDisplayZOOM;

public:
	Constants();
};

Constants::Constants() {
	//시점과 관련된 변수들 입니다.
	perspective = 0;
	leftPos = -4.0, rightPos = 4.0, topPos = 2.0, bottomPos = -2.0;
	eyeX = 0.0, eyeY = -6.7800286e-008, eyeZ = 3.5;
	centerX = 0.0, centerY = 0.0, centerZ = 0.0;
	orthoLeft = -4.0, orthoRight = 4.0, orthoBottom = -2.0, orthoTop = 2.0;

	isShooted = 0;
	gameoverCount = 3; //수구가 포켓에 3번 빠지면 게임에 실패하게 되고 새게임을 다시 시작합니다.
	RotateDisplay = 0.0;
	RotateDisplayZOOM = 0.0;
}

#endif