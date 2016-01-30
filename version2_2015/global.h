/*
* 파일이름 : global.h
* 작 성 자 : 임다솜
* 업데이트 정보 : [2015. 10. 23]
*/

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <iostream>	
#include <math.h>
#include <time.h>
using namespace std;

#include <gl/freeglut.h>
#include <glaux.h>
#pragma comment (lib, "glaux.lib")

#define BALL_NUMBER		9 //0은 수구, 1~6은 적구, 7은 큐, 8은 cue line을 그릴 번호 = 총9개
#define CUE				BALL_NUMBER-2
#define LINE			BALL_NUMBER-1

#define GET_X_DIS		10	//가독성을 위해 define 정의 해주었습니다.
#define GET_Y_DIS		11
#define GET_Z_DIS		12
#define GET_SKEWED_DIS	13

#define ALL				15
#define ZOOM			16

#define RADIAN			20
#define DEGREE			21

#define PI 3.14159265358979f;