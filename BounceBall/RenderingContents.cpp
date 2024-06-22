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


#define block1 20 //블록들의 사이즈
#define block2 49
#define block3 10

#define spikeNum 3 //가시의 개수
#define spike1 5 //가시 한세트의 길이

#define brickNum 3 //벽돌의 개수
#define brick1 5 //벽돌 한 세트의 길이

#define portalNum 4 //포탈의 길이

#define hitbox(box) (ball.x <= box.x + 1 && ball.x >= box.x - 1) && (ball.y <= box.y + 2 && ball.y > box.y) //블럭 등 장애물 객체를 입력받으면 히트박스에 들어왔는지를 판별하는 매크로

block b[block1];
block b2[block2];
spike s[spikeNum][spike1];
brick br[brickNum][brick1];
spring sp[4];
portal p2[portalNum];
portal p3[portalNum];
clearPortal p[3]; //여러 블럭들의 객체를 선언

void Game::PhysicsEngine()
{
	bool leftPressed = false;
	bool rightPressed = false;
	/*************************
				INPUT
	**************************/
	leftPressed = GetAsyncKeyState(VK_LEFT) & 0x8000;
	rightPressed = GetAsyncKeyState(VK_RIGHT) & 0x8000; //키보드 입력
	/*************************
				LOGIC
	**************************/
	//Physics Engine
	// 중력 적용

	ball.vy += gravity; 

	// 키 입력에 따른 속도 변경
	if (leftPressed)
		if (ball.vx >= -0.9f) //기본적으로 좌우를 누르면 그쪽 방향으로 속도가 증가하나 최대 속도가 정해짐
			ball.vx -= 0.2f;
	if (rightPressed)
		if (ball.vx <= 0.9f)
			ball.vx += 0.2f;
	if (!leftPressed && !rightPressed)
		ball.vx *= 0.9f; //좌우 방향키를 둘다 안누르면 속도가 점점 감소
	// 위치 업데이트
	ball.x += ball.vx; //속도에 따른 위치 업데이트
	ball.y += ball.vy;

	// 벽과의 충돌 판정 및 탄성 적용
	if (ball.x < 1) {
		ball.x = 1;
		ball.vx = -ball.vx * elasticity; //벽에 충돌시 탄성 적용
	}
	if (ball.x > dfSCREEN_WIDTH - 3)
	{
		ball.x = dfSCREEN_WIDTH - 3;
		ball.vx = -ball.vx * elasticity;
	}

	// 바닥과의 충돌 판정
	if (ball.y > dfSCREEN_HEIGHT - 2) {
		ball.y = dfSCREEN_HEIGHT - 2;
		g_scene = GAMEOVER; //바닥과 충돌시 게임오버
	}

	// 천장과의 충돌 판정
	if (ball.y < 0) {
		ball.y = 0;
		ball.vy = -ball.vy * elasticity; //천장과 충돌시 탄성 적용
	}

	//블록과의 충돌판정
	for (int i = 0; i < block1; i++) {  //블록과 충돌시 vy를 탄성을 적용해 점프시킴
		if (hitbox(b[i])) {
			ball.vy = elasticity2;
		}
	}
	if (g_scene == INGAME)
	{
		for (int i = 0; i < block2; i++) {
			if (hitbox(b2[i])) {
				ball.vy = elasticity2; //블록과 충돌시 vy를 탄성을 적용해 점프시킴
			}
		}
		
		//스파이크와의 충돌판정
		for (int i = 0; i < spikeNum; i++) {
			for (int j = 0; j < sizeof(s[i]) / sizeof(*s[i]); j++) {
				if (hitbox(s[i][j])) { //충돌시 게임오버
					g_scene = GAMEOVER;
				}
			}
		}
	}
	else
	{
		for (int i = 0; i < block3; i++) {
			if (hitbox(b2[i])) { //블록과의 충돌판정
				ball.vy = elasticity2;
			}
		}
	}

	//벽돌과의 충돌판정
	for (int i = 0; i < brickNum; i++) {
		for (int j = 0; j < sizeof(br[i]) / sizeof(*br[i]); j++) {
			if (brickHit[i] == 0) { //벽돌이 히트해 있지 않을 떄만 충돌 적용
				if (hitbox(br[i][j])) {
					ball.vy = elasticity2; //탄성 적용 후
					brickHit[i] = 1;  //i번째 벽돌이 히트했음을 표시
				}
			}
		}
	}


	//포탈과의 충돌판정
	for (int i = 0; i < 3; i++) {
		if (hitbox(p[i])) {
			g_scene = CLEAR; // 포탈과 닿으면 클리어
		}
	}

	if (g_scene == INGAME3)
	{
		//포탈2과의 충돌판정
		for (int i = 0; i < portalNum; i++) {
			if (hitbox(p2[i])) {
				ball.y = p3[i].y;
				ball.x = p3[i].x; //포탈과 충돌시 다른 포탈의 x, y위치로 이동
				ball.vy *= -1.05; //중력에 의해 속도가 느려지므로 보정값
			}
		}

		for (int i = 0; i < portalNum; i++) {
			if (hitbox(p3[i])) {
				ball.y = p2[i].y;
				ball.x = p2[i].x; //포탈과 충돌시 다른 포탈의 x위치로 이동, y좌표는 살짝 위로
				ball.vy *= -1.05; //중력에 의해 속도가 느려지므로 보정값
			}
		}

		//스프링과의 충돌판정
		for (int i = 0; i < 4; i++) {
			if (hitbox(sp[i])) {
				if (ball.x > sp[i].x) {
					ball.vy = elasticity2 - 0.7; //기존 탄성보다 조금더 강한 탄성 적용
				}
			}
		}
	}
}

void	Game::UpdateTitle()
{
	RenderMessage("Bounce Ball!");

	// 키보드 입력
	for (int key = 8; key <= 255; key++)
	{
		//현재 눌려있는 키 확인 
		if (GetAsyncKeyState(key) & 0x8000)
		{
			cs_ClearScreen();
			cs_MoveCursor(0, 0);
			g_scene = INGAME;
		}
	}
	// 게임시작! 메뉴 선택시 게임씬 전환
	cs_MoveCursor((dfSCREEN_WIDTH - strlen("======== = Game Start!==========")) / 2, dfSCREEN_HEIGHT / 2 + 1);
	printf("========= Game Start! ==========\n");
}


void	Game::UpdateGame()
{

	for (int i = 0; i < block1; i++) { //첫 스테이지의 블럭들의 위치 적용
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


	game.PhysicsEngine(); //물리 적용

	/*************************
			RENDERING
	**************************/
	//렌더링
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



void Game::UpdateGame2() //두 번째 스테이지의 블럭들의 위치 적용
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
		if (brickHit[i] == 0) { //벽돌이 깨져있지 않을 떄만 그리기
			for (int j = 0; j < sizeof(br[i]) / sizeof(*br[i]); j++) {
				br[i][j].Draw();
			}
		}
	}
	Buffer_Flip();
}

void Game::UpdateGame3()
{
	for (int i = 0; i < block1; i++) { //세 번쨰 스테이지의 블럭들의 위치 적용
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


//스테이지 클리어 씬
void Game::UpdateClear()
{
	RenderMessage("Stage Clear");

	// 키보드 입력
	for (int key = 8; key <= 255; key++)
	{
		//현재 눌려있는 키 확인 
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

//게임 끝남
void Game::UpdateOver()
{
	RenderMessage("GAME OVER");

	// 키보드 입력
	for (int key = 8; key <= 255; key++)
	{
		//현재 눌려있는 키 확인 
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


	// 메뉴 랜더링
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
