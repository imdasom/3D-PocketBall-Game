// version2_2015.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//
/*
* �ۼ��� : �Ӵټ�
* blog.naver.com/dasom7978 �� ��α׿� ���ø� ����� �Բ� ���� ���� ���� �ڼ��� ������ �ֽ��ϴ�.

ť�� �̵� : LEFT, RIGHT ����Ű
ť ���ǵ�(�Ÿ�) ���� : UP, DOWN ����Ű
�� : �����̽���
'esc': ���� ����
'f'	 : -
'o'	 : -
'b'	 : ���̺� ���� �ִ� ���� ��ġ�� �����ϰ� �ٽ� �ٲٱ�
'n'	 : �� ���� ����
'r'	 : ����� ��ġ�� ����

--'����' �ڽ��� ��ġ �̵��ϱ�--
'h'  : ȭ�� �������� �̵�
'k'  : ȭ���� ���������� �̵�
'u'  : ȭ���� ���� �̵�
'j'  : ȭ���� �Ʒ��� �̵�
'z'  : ������ �������� ȸ��
'x'  : ������ ���������� ȸ��

--������ �ٶ󺸴� '���' �̵��ϱ�--
GLUT_KEY_RIGHT : ���� ������ ���� ��ó�� ���δ�.
GLUT_KEY_LEFT  : ���� ������ ���̰� �ȴ�.
GLUT_KEY_UP    : ���� �籸�밡 ���� ���� �ȴ�.
GLUT_KEY_DOWN  : ���ִ� �籸�밡 ���� �ڷ� ���� ����� �ȴ�.

ť��� ���� ���� : 1, 2, 3, 4, 5, 6 �� ����Ű
���� ����/3�� ���� ��ȯ : 'z'Ű
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
static Ball* b[BALL_NUMBER];				//����ü�Դϴ�. ���� �⺻ ������ ������ �ֽ��ϴ�.
static Calculator* clt;						//���� ��ü�Դϴ�. ����� ���� ���� �޼ҵ带 ������ �ֽ��ϴ�.
static CueHandler* cHandler;				//ť �ڵ鷯 ��ü�Դϴ�. ť�� �����̴µ� �ʿ��� �޼ҵ带 ������ �ֽ��ϴ�.
static BallHandler* bHandler;				//�� �ڵ鷯 ��ü�Դϴ�. ���� �� ��Ʈ�� ��� ������ �� �ִ� ��ü�Դϴ�.
static DrawHandler* drawHandler;			//�׸��� �ڵ鷯 ��ü�Դϴ�. ȭ�鿡 ���� �籸�븦 �׸��� ���� �޼ҵ带 ������ �ֽ��ϴ�.

void Init() {

	//��, ���ڵ鷯, ����, ť�ڵ鷯, �׸����ڵ鷯, ����̽��ڵ鷯 ��ü�� �����մϴ�.
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

void MyTimer(int value) {  ///////�ִ��� �����ϰ�, ��ü���� �˰����� ������ �� �ֵ��� �����Ͽ����ϴ�.

	//�����̽��ٸ� ���� ť�� ���� ġ�� �Ǹ� -> isShooted=1�� �Ǿ� ����˴ϴ�.
	//��� ���� ���߰� �Ǹ� -> isShooted=0 �� �Ǿ� ������� �ʽ��ϴ�.
	if (c->isShooted == 1) {
		bHandler->DetectCollision();			//1. ���� �� ������ �浹�� �����մϴ�.
		bHandler->isBumpedWall();				//2. ���� �� ������ �浹�� �����մϴ�.
		bHandler->isPooledPocket();				//3. ���� ���Ͽ� �������� �����մϴ�.
		bHandler->MoveBall();					//4. ������ ���� ������ ���� �̵��ؾ� �ϴ� ��ŭ ���� ��ǥ���� �����մϴ�.
		if (bHandler->isAllBallStop() == true) {//5. ��� ���� ���߸� ����� ť�� �ٽ� �׸��� ť������ �ʱ�ȭ�մϴ�.
			c->isShooted = 0;
			cHandler->InitCueInfo();
			bHandler->InitBallInfo();
		}
	}

	//���� �� ���� �������� 3�� ���� ���� ������ �ǰ� ������ ó������ �ٽ� �����մϴ�.
	if (c->gameoverCount == 0) {
		c->gameoverCount = 3;
		bHandler->Breaking();
		cHandler->InitCueInfo();
		bHandler->InitBallInfo();
	}

	glutPostRedisplay();
	glutTimerFunc(8, MyTimer, 1); //glutTimerFunc()�� 1ȸ�� �Լ��̱� ������ ���ο��� �ٽ� ȣ�����ݴϴ�.
}

void MyDisplay() {
	glPushMatrix();
	drawHandler->drawViewPort1();	//1������Ʈ - �籸��� �籸�� �׸���
	drawHandler->drawViewPort2();	//2������Ʈ - ���� ǥ��
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
	case GLUT_KEY_RIGHT: //�ð�������� ť�� ���ư�����.
		b[CUE]->dAngle -= 0.2;
		if (b[CUE]->dAngle < 0) b[CUE]->dAngle += 360;
		c->RotateDisplayZOOM = b[CUE]->dAngle + 180;
		glutPostRedisplay();
		break;
	case GLUT_KEY_LEFT: //�ݽð�������� ť�� ���ư�����.
		b[CUE]->dAngle += 0.2;
		if (b[CUE]->dAngle >= 360) b[CUE]->dAngle -= 360;
		c->RotateDisplayZOOM = b[CUE]->dAngle + 180;
		glutPostRedisplay();
		break;
	case GLUT_KEY_UP:
		if (clt->Get_Distance(GET_SKEWED_DIS, CUE, 0) >= (0.42)) { //ť�� ������ ������ ������ �ʵ��� �Ѵ�.
			cHandler->cuepower -= 0.01; //ť�Ŀ����� ���� �������ش�. ������ ����Ѵ�. //ť�Ŀ��� �����Ѵ�. ������ ������ ���ģ�ٴ� �ǹ��̹Ƿ�.
			glutPostRedisplay();
		}
		break;
	case GLUT_KEY_DOWN:
		if (clt->Get_Distance(GET_SKEWED_DIS, CUE, 0) <= 0.80) { //ť�� �ʹ����� �ڷ� ������ �ʵ��� �Ѵ�.
			cHandler->cuepower += 0.01;
			glutPostRedisplay();
		}
		break;
	}
	clt->Get_CueDirection(cHandler->inc + cHandler->cuepower, CUE, 0);
	b[CUE]->posX = -b[CUE]->dirX + b[0]->posX;  //������������ ���������� �׸��Ƿ� dir���� ������ �ʰ� ���ش�.
	b[CUE]->posY = -b[CUE]->dirY + b[0]->posY;
}

//glut�Լ��� �̿��Ͽ� ���� �̺�Ʈ�����ʸ� �����մϴ�.
void Setting() {
	glutIdleFunc(MyDisplay);						//glut�� �ƹ��ϵ� ���� �ʴ� ���ȿ� ����� ������ �޼ҵ带 �������ݴϴ�.
	glutDisplayFunc(MyDisplay);						//MyDisplay() �޼ҵ带 �����Ͽ� ȭ���� �׸��� �˴ϴ�.
	glutKeyboardFunc(MyKeyboard);					//�Ϲ� ���ڿ� ���� �̺�Ʈ �����ʸ� �����մϴ�.
	glutSpecialFunc(MyKeyboard2);					//Ư�����ڿ� ���� �̺�Ʈ �����ʸ� �����մϴ�.(����Ű ��)
	glutTimerFunc(8, MyTimer, 1);					//ù��° ���ڷ� ���ʿ� �ѹ� ������ ������ �и��ʴ����� �����ݴϴ�. 
													//�ι�° ���ڷ� ������ ������ MyTimer()�� �Լ��� �Ѱ��ݴϴ�.
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(1800, 1000);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("�Ӵټ�_3D�籸����_Ver2_C++�� ����");
	glClearColor(0.0, 0.0, 0.0, 1.0);

	Init();				//��ü�� �����ϰ� �ʱ� ������ �����մϴ�.
	Setting();			//glut �޼ҵ带 �̿��Ͽ� Input�� ���� �̺�Ʈ�����ʸ� �����ϰ�, ȭ���� �׸��� ���� �޼ҵ�� Ÿ�̸Ӹ� �����մϴ�.
	glutMainLoop();		//glut�� ���η����� �����ϸ� ȭ���� �׸��� ���ÿ� ������ ���۵˴ϴ�.

	return 0;
}
