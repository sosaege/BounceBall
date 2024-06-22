#include <Windows.h>
#include <time.h>
#include <stdio.h>
#include <string>
#include <vector>
#include "main.h"
#include "Console.h"
#include "Rendering.h"
#include "Game.h"
#include <cmath>


#define block1 20 //��ϵ��� ������
#define block2 49
#define block3 10

#define spikeNum 3 //������ ����
#define spike1 5 //���� �Ѽ�Ʈ�� ����

#define brickNum 3 //������ ����
#define brick1 5 //���� �� ��Ʈ�� ����

#define portalNum 4 //��Ż�� ����

#define hitbox(box) (ball.x <= box.x + 1 && ball.x >= box.x - 1) && (ball.y <= box.y + 2 && ball.y > box.y) //�� �� ��ֹ� ��ü�� �Է¹����� ��Ʈ�ڽ��� ���Դ����� �Ǻ��ϴ� ��ũ��

block b[block1];
block b2[block2];
spike s[spikeNum][spike1];
brick br[brickNum][brick1];
spring sp[4];
portal p2[portalNum];
portal p3[portalNum];
clearPortal p[3]; //���� ������ ��ü�� ����

void Game::PhysicsEngine()
{
	bool leftPressed = false;
	bool rightPressed = false;
	/*************************
				INPUT
	**************************/
	leftPressed = GetAsyncKeyState(VK_LEFT) & 0x8000;
	rightPressed = GetAsyncKeyState(VK_RIGHT) & 0x8000; //Ű���� �Է�
	/*************************
				LOGIC
	**************************/
	//Physics Engine
	// �߷� ����

	ball.vy += gravity; 

	// Ű �Է¿� ���� �ӵ� ����
	if (leftPressed)
		if (ball.vx >= -0.9f) //�⺻������ �¿츦 ������ ���� �������� �ӵ��� �����ϳ� �ִ� �ӵ��� ������
			ball.vx -= 0.2f;
	if (rightPressed)
		if (ball.vx <= 0.9f)
			ball.vx += 0.2f;
	if (!leftPressed && !rightPressed)
		ball.vx *= 0.9f; //�¿� ����Ű�� �Ѵ� �ȴ����� �ӵ��� ���� ����
	// ��ġ ������Ʈ
	ball.x += ball.vx; //�ӵ��� ���� ��ġ ������Ʈ
	ball.y += ball.vy;

	// ������ �浹 ���� �� ź�� ����
	if (ball.x < 1) {
		ball.x = 1;
		ball.vx = -ball.vx * elasticity; //���� �浹�� ź�� ����
	}
	if (ball.x > dfSCREEN_WIDTH - 3)
	{
		ball.x = dfSCREEN_WIDTH - 3;
		ball.vx = -ball.vx * elasticity;
	}

	// �ٴڰ��� �浹 ����
	if (ball.y > dfSCREEN_HEIGHT - 2) {
		ball.y = dfSCREEN_HEIGHT - 2;
		g_scene = GAMEOVER; //�ٴڰ� �浹�� ���ӿ���
	}

	// õ����� �浹 ����
	if (ball.y < 0) {
		ball.y = 0;
		ball.vy = -ball.vy * elasticity; //õ��� �浹�� ź�� ����
	}

	//��ϰ��� �浹����
	for (int i = 0; i < block1; i++) {  //��ϰ� �浹�� vy�� ź���� ������ ������Ŵ
		if (hitbox(b[i])) {
			ball.vy = elasticity2;
		}
	}
	if (g_scene == INGAME)
	{
		for (int i = 0; i < block2; i++) {
			if (hitbox(b2[i])) {
				ball.vy = elasticity2; //��ϰ� �浹�� vy�� ź���� ������ ������Ŵ
			}
		}
		
		//������ũ���� �浹����
		for (int i = 0; i < spikeNum; i++) {
			for (int j = 0; j < sizeof(s[i]) / sizeof(*s[i]); j++) {
				if (hitbox(s[i][j])) { //�浹�� ���ӿ���
					g_scene = GAMEOVER;
				}
			}
		}
	}
	else
	{
		for (int i = 0; i < block3; i++) {
			if (hitbox(b2[i])) { //��ϰ��� �浹����
				ball.vy = elasticity2;
			}
		}
	}

	//�������� �浹����
	for (int i = 0; i < brickNum; i++) {
		for (int j = 0; j < sizeof(br[i]) / sizeof(*br[i]); j++) {
			if (brickHit[i] == 0) { //������ ��Ʈ�� ���� ���� ���� �浹 ����
				if (hitbox(br[i][j])) {
					ball.vy = elasticity2; //ź�� ���� ��
					brickHit[i] = 1;  //i��° ������ ��Ʈ������ ǥ��
				}
			}
		}
	}


	//��Ż���� �浹����
	for (int i = 0; i < 3; i++) {
		if (hitbox(p[i])) {
			g_scene = CLEAR; // ��Ż�� ������ Ŭ����
		}
	}

	if (g_scene == INGAME3)
	{
		//��Ż2���� �浹����
		for (int i = 0; i < portalNum; i++) {
			if (hitbox(p2[i])) {
				ball.y = p3[i].y;
				ball.x = p3[i].x; //��Ż�� �浹�� �ٸ� ��Ż�� x, y��ġ�� �̵�
				ball.vy *= -1.05; //�߷¿� ���� �ӵ��� �������Ƿ� ������
			}
		}

		for (int i = 0; i < portalNum; i++) {
			if (hitbox(p3[i])) {
				ball.y = p2[i].y;
				ball.x = p2[i].x; //��Ż�� �浹�� �ٸ� ��Ż�� x��ġ�� �̵�, y��ǥ�� ��¦ ����
				ball.vy *= -1.05; //�߷¿� ���� �ӵ��� �������Ƿ� ������
			}
		}

		//���������� �浹����
		for (int i = 0; i < 4; i++) {
			if (hitbox(sp[i])) {
				if (ball.x > sp[i].x) {
					ball.vy = elasticity2 - 0.7; //���� ź������ ���ݴ� ���� ź�� ����
				}
			}
		}
	}
}

void	Game::UpdateTitle()
{
	RenderMessage("Bounce Ball!");

	// Ű���� �Է�
	for (int key = 8; key <= 255; key++)
	{
		//���� �����ִ� Ű Ȯ�� 
		if (GetAsyncKeyState(key) & 0x8000)
		{
			cs_ClearScreen();
			cs_MoveCursor(0, 0);
			g_scene = INGAME;
		}
	}
	// ���ӽ���! �޴� ���ý� ���Ӿ� ��ȯ
	cs_MoveCursor((dfSCREEN_WIDTH - strlen("======== = Game Start!==========")) / 2, dfSCREEN_HEIGHT / 2 + 1);
	printf("========= Game Start! ==========\n");
}


void	Game::UpdateGame()
{

	for (int i = 0; i < block1; i++) { //ù ���������� ������ ��ġ ����
		b[i].y = 19;
		b[i].x = i + 1;
	}

	for (int i = 0; i < block2; i++) {
		b2[i].y = 17;
		b2[i].x = i + 30;
	}

	for (int i = 0; i < spike1; i++) {
		s[0][i].y = 17;
		s[0][i].x = i + 40;
	}
	for (int i = 0; i < spike1; i++) {
		s[1][i].y = 17;
		s[1][i].x = i + 52;
	}
	for (int i = 0; i < spike1; i++) {
		s[2][i].y = 17;
		s[2][i].x = i + 64;
	}

	for (int i = 0; i < 3; i++) {
		p[i].y = 16;
		p[i].x = i + 73;
	}


	game.PhysicsEngine(); //���� ����

	/*************************
			RENDERING
	**************************/
	//������
	Buffer_Clear();
	Draw_Ball();
	Draw_Wall();
	for (int i = 0; i < block1; i++) {
		b[i].Draw();
	}
	for (int i = 0; i < block2; i++) {
		b2[i].Draw();
	}
	for (int i = 0; i < spikeNum; i++) {
		for (int j = 0; j < sizeof(s[i]) / sizeof(*s[i]); j++) {
			s[i][j].Draw();
		}
	}
	for (int i = 0; i < 3; i++) {
		p[i].Draw();
	}
	Buffer_Flip();
}



void Game::UpdateGame2() //�� ��° ���������� ������ ��ġ ����
{
	for (int i = 0; i < block1; i++) {
		b[i].y = 10;
		b[i].x = i + 1;
	}

	for (int i = 0; i < block3; i++) {
		b2[i].y = 10;
		b2[i].x = i + 70;
	}

	for (int i = 0; i < brick1; i++) {
		br[0][i].y = 10;
		br[0][i].x = i + 30;
	}
	for (int i = 0; i < brick1; i++) {
		br[1][i].y = 10;
		br[1][i].x = i + 42;
	}
	for (int i = 0; i < brick1; i++) {
		br[2][i].y = 10;
		br[2][i].x = i + 55;
	}

	for (int i = 0; i < 3; i++) {
		p[i].y = 9;
		p[i].x = i + 75;
	}

	game.PhysicsEngine(); 

	/*************************
			RENDERING
	**************************/

	Buffer_Clear();
	Draw_Ball();
	Draw_Wall();
	for (int i = 0; i < block1; i++) {
		b[i].Draw();
	}
	for (int i = 0; i < block3; i++) {
		b2[i].Draw();
	}
	for (int i = 0; i < 3; i++) {
		p[i].Draw();
	}

	for (int i = 0; i < brickNum; i++) { 
		if (brickHit[i] == 0) { //������ �������� ���� ���� �׸���
			for (int j = 0; j < sizeof(br[i]) / sizeof(*br[i]); j++) {
				br[i][j].Draw();
			}
		}
	}
	Buffer_Flip();
}

void Game::UpdateGame3()
{
	for (int i = 0; i < block1; i++) { //�� ���� ���������� ������ ��ġ ����
		b[i].y = 17; 
		b[i].x = i + 1;
	}

	for (int i = 0; i < block3; i++) {
		b2[i].y = 10;
		b2[i].x = i + 70;
	}


	for (int i = 0; i < 4; i++) {
		sp[i].y = 18;
		sp[i].x = i + 38;
	}

	for (int i = 0; i < 3; i++) {
		p[i].y = 9;
		p[i].x = i + 75;
	}

	for (int i = 0; i < portalNum; i++) {
		p2[i].y = 17;
		p2[i].x = i + 30;
	}
	for (int i = 0; i < portalNum; i++) {
		p3[i].y = 17;
		p3[i].x = i + 60;
	}

	game.PhysicsEngine();

	/*************************
			RENDERING
	**************************/

	Buffer_Clear();
	Draw_Ball();
	Draw_Wall();
	for (int i = 0; i < block1; i++) {
		b[i].Draw();
	}
	for (int i = 0; i < block3; i++) {
		b2[i].Draw();
	}
	for (int i = 0; i < 3; i++) {
		p[i].Draw();
	}

	for (int i = 0; i < portalNum; i++) {
		p2[i].Draw();
	}
	for (int i = 0; i < portalNum; i++) {
		p3[i].Draw();
	}
	for (int i = 0; i < 4; i++) {
		sp[i].Draw();
	}
	Buffer_Flip();
}


//�������� Ŭ���� ��
void Game::UpdateClear()
{
	RenderMessage("Stage Clear");

	// Ű���� �Է�
	for (int key = 8; key <= 255; key++)
	{
		//���� �����ִ� Ű Ȯ�� 
		if (GetAsyncKeyState(key) & 0x8000)
		{
			cs_ClearScreen();
			cs_MoveCursor(0, 0);
			if (tmp == 0)
			{
				g_scene = INGAME2;
			}
			else if (tmp == 1)
			{
				g_scene = INGAME3;
			}
			else if (tmp == 2)
			{
				g_scene = GAMEOVER;
			}
			tmp++;
		}
	}
}

//���� ����
void Game::UpdateOver()
{
	RenderMessage("GAME OVER");

	// Ű���� �Է�
	for (int key = 8; key <= 255; key++)
	{
		//���� �����ִ� Ű Ȯ�� 
		if (GetAsyncKeyState(key) & 0x8000)
		{
			cs_ClearScreen();
			cs_MoveCursor(0, 0);
			exit(1);
		}
	}
}

void Game::RenderMessage(const char* message)
{
	int idx = 0;
	std::string current_display = "";


	// �޴� ������
	while (idx < strlen(message))
	{
		clock_t	current_time = clock();
		if ((current_time / CLOCKS_PER_SEC) >= idx * 0.1)
		{
			current_display += message[idx];
			Buffer_Clear();
			int x = (dfSCREEN_WIDTH - current_display.length()) / 2;
			int y = dfSCREEN_HEIGHT / 2;
			for (int i = 0; i < current_display.length(); i++)
			{
				Sprite_Draw(x + i, y, current_display[i]);
			}
			Buffer_Flip();
			idx++;
		}
	}
	cs_MoveCursor((dfSCREEN_WIDTH - strlen("====== = Press any key.... ======")) / 2, dfSCREEN_HEIGHT / 2 + 2);
	printf("======= Press any key.... ======\n");
}
