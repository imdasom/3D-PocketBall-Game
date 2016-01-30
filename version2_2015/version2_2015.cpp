// version2_2015.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//
/*
* 작성자 : 임다솜
* blog.naver.com/dasom7978 제 블로그에 가시면 영상과 함께 구현 원리 등의 자세한 설명이 있습니다.

큐대 이동 : LEFT, RIGHT 방향키
큐 스피드(거리) 조절 : UP, DOWN 방향키
샷 : 스페이스바
'esc': 게임 종료
'f'	 : -
'o'	 : -
'b'	 : 테이블 위에 있는 공의 위치를 랜덤하게 다시 바꾸기
'n'	 : 새 게임 시작
'r'	 : 흰공의 위치를 리셋

--'시점' 자신의 위치 이동하기--
'h'  : 화면 왼쪽으로 이동
'k'  : 화면을 오른쪽으로 이동
'u'  : 화면을 위로 이동
'j'  : 화면을 아래로 이동
'z'  : 시점을 왼쪽으로 회전
'x'  : 시점을 오른쪽으로 회전

--시점이 바라보는 '대상' 이동하기--
GLUT_KEY_RIGHT : 더욱 위에서 보는 것처럼 보인다.
GLUT_KEY_LEFT  : 더욱 가까이 보이게 된다.
GLUT_KEY_UP    : 누운 당구대가 점점 서게 된다.
GLUT_KEY_DOWN  : 서있는 당구대가 점점 뒤로 눕는 모양이 된다.

큐대로 공을 조준 : 1, 2, 3, 4, 5, 6 번 숫자키
수구 시점/3자 시점 변환 : 'z'키
*/

#include "stdafx.h"
#include "global.h"
#include "Ball.h"
#include "Constants.h"
#include "Calculator.h"
#include "CueHandler.h"
#include "BallHandler.h"
#include "DrawHandler.h"

static Constants* c;
static Ball* b[BALL_NUMBER];				//공객체입니다. 공의 기본 정보를 가지고 있습니다.
static Calculator* clt;						//계산기 객체입니다. 계산을 위한 여러 메소드를 가지고 있습니다.
static CueHandler* cHandler;				//큐 핸들러 객체입니다. 큐를 움직이는데 필요한 메소드를 가지고 있습니다.
static BallHandler* bHandler;				//공 핸들러 객체입니다. 공을 한 세트로 묶어서 관리할 수 있는 객체입니다.
static DrawHandler* drawHandler;			//그리기 핸들러 객체입니다. 화면에 공과 당구대를 그리는 등의 메소드를 가지고 있습니다.

void Init() {

	//공, 공핸들러, 계산기, 큐핸들러, 그리기핸들러, 디바이스핸들러 객체를 생성합니다.
	c = new Constants();

	clt = new Calculator(c);
	clt->setBall(b);

	cHandler = new CueHandler(c);
	cHandler->setCalculator(clt);
	cHandler->setBall(b);

	bHandler = new BallHandler(c);
	bHandler->setBall(b);
	bHandler->setCalculator(clt);
	bHandler->setCueHandler(cHandler);
	bHandler->CreateBall(BALL_NUMBER);

	drawHandler = new DrawHandler(c);
	drawHandler->setCalculator(clt);
	drawHandler->setBall(b);
	drawHandler->setCueHandler(cHandler);

	glMaterialf(GL_FRONT, GL_SHININESS, 25.0);
	glShadeModel(GL_SMOOTH);
}

void MyTimer(int value) {  ///////최대한 간결하게, 전체적인 알고리즘을 이해할 수 있도록 구현하였습니다.

	//스페이스바를 눌러 큐가 공을 치게 되면 -> isShooted=1이 되어 실행됩니다.
	//모든 공이 멈추게 되면 -> isShooted=0 이 되어 실행되지 않습니다.
	if (c->isShooted == 1) {
		bHandler->DetectCollision();			//1. 공과 공 사이의 충돌을 조사합니다.
		bHandler->isBumpedWall();				//2. 공과 벽 사이의 충돌을 조사합니다.
		bHandler->isPooledPocket();				//3. 공이 포켓에 빠졌는지 조사합니다.
		bHandler->MoveBall();					//4. 조사한 값을 가지고 공이 이동해야 하는 만큼 공의 좌표값을 변경합니다.
		if (bHandler->isAllBallStop() == true) {//5. 모든 공이 멈추면 사라진 큐를 다시 그리고 큐정보를 초기화합니다.
			c->isShooted = 0;
			cHandler->InitCueInfo();
			bHandler->InitBallInfo();
		}
	}

	//만약 흰 공이 포켓으로 3번 들어가면 게임 오버가 되고 게임을 처음부터 다시 시작합니다.
	if (c->gameoverCount == 0) {
		c->gameoverCount = 3;
		bHandler->Breaking();
		cHandler->InitCueInfo();
		bHandler->InitBallInfo();
	}

	glutPostRedisplay();
	glutTimerFunc(8, MyTimer, 1); //glutTimerFunc()은 1회성 함수이기 때문에 내부에서 다시 호출해줍니다.
}

void MyDisplay() {
	glPushMatrix();
	drawHandler->drawViewPort1();	//1번뷰포트 - 당구대와 당구공 그리기
	drawHandler->drawViewPort2();	//2번뷰포트 - 들어간공 표시
	glPopMatrix();
	glutSwapBuffers();
}

void MyKeyboard(unsigned char key, int x, int y) {
	switch (key) {
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
		cHandler->MoveCue((int)(key - 48));
		glutPostRedisplay();
		break;
	case 'q':
	case 'Q':
		exit(1);
		break;
	case 'b':
	case 'B':
		bHandler->Breaking();
		glutPostRedisplay();
		break;
	case 'z':
	case 'Z':
		if (c->perspective == ALL)	c->perspective = ZOOM;
		else if (c->perspective == ZOOM) c->perspective = ALL;
		break;
	case ' ':
		if (c->isShooted != 1) {
			c->isShooted = 1;
			cHandler->Shot();
		}
		glutPostRedisplay();
		break;
	case 27:
		exit(0); break;
	}
}

void MyKeyboard2(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_RIGHT: //시계방향으로 큐가 돌아가도록.
		b[CUE]->dAngle -= 0.2;
		if (b[CUE]->dAngle < 0) b[CUE]->dAngle += 360;
		c->RotateDisplayZOOM = b[CUE]->dAngle + 180;
		glutPostRedisplay();
		break;
	case GLUT_KEY_LEFT: //반시계방향으로 큐가 돌아가도록.
		b[CUE]->dAngle += 0.2;
		if (b[CUE]->dAngle >= 360) b[CUE]->dAngle -= 360;
		c->RotateDisplayZOOM = b[CUE]->dAngle + 180;
		glutPostRedisplay();
		break;
	case GLUT_KEY_UP:
		if (clt->Get_Distance(GET_SKEWED_DIS, CUE, 0) >= (0.42)) { //큐가 공보다 앞으로 나가지 않도록 한다.
			cHandler->cuepower -= 0.01; //큐파워에도 값을 저장해준다. 위에서 사용한다. //큐파워는 감소한다. 앞으로 갈수록 살살친다는 의미이므로.
			glutPostRedisplay();
		}
		break;
	case GLUT_KEY_DOWN:
		if (clt->Get_Distance(GET_SKEWED_DIS, CUE, 0) <= 0.80) { //큐가 너무많이 뒤로 나가지 않도록 한다.
			cHandler->cuepower += 0.01;
			glutPostRedisplay();
		}
		break;
	}
	clt->Get_CueDirection(cHandler->inc + cHandler->cuepower, CUE, 0);
	b[CUE]->posX = -b[CUE]->dirX + b[0]->posX;  //수구에서부터 움직여가서 그리므로 dir값을 더하지 않고 빼준다.
	b[CUE]->posY = -b[CUE]->dirY + b[0]->posY;
}

//glut함수를 이용하여 관련 이벤트리스너를 세팅합니다.
void Setting() {
	glutIdleFunc(MyDisplay);						//glut가 아무일도 하지 않는 동안에 계산을 도와줄 메소드를 지정해줍니다.
	glutDisplayFunc(MyDisplay);						//MyDisplay() 메소드를 실행하여 화면을 그리게 됩니다.
	glutKeyboardFunc(MyKeyboard);					//일반 문자에 대한 이벤트 리스너를 세팅합니다.
	glutSpecialFunc(MyKeyboard2);					//특수문자에 대한 이벤트 리스너를 세팅합니다.(방향키 등)
	glutTimerFunc(8, MyTimer, 1);					//첫번째 인자로 몇초에 한번 실행할 것인지 밀리초단위로 정해줍니다. 
													//두번째 인자로 실행할 내용을 MyTimer()란 함수로 넘겨줍니다.
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(1800, 1000);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("임다솜_3D당구게임_Ver2_C++로 구현");
	glClearColor(0.0, 0.0, 0.0, 1.0);

	Init();				//객체를 생성하고 초기 설정을 진행합니다.
	Setting();			//glut 메소드를 이용하여 Input에 대한 이벤트리스너를 세팅하고, 화면을 그리기 위한 메소드와 타이머를 세팅합니다.
	glutMainLoop();		//glut가 메인루프를 진행하면 화면을 그림과 동시에 게임이 시작됩니다.

	return 0;
}
