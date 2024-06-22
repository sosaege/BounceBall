#include "Rendering.h"
#include "main.h"
#include "Console.h"
#include <Windows.h>
#include "Game.h"
#include <time.h>

Game game;

void main(void)
{
	cs_Initial();

	//--------------------------------------------------------------------
	// 게임의 메인 루프
	// 이 루프가  1번 돌면 1프레임이다.
	//--------------------------------------------------------------------
	while (1)
	{
		switch (game.g_scene) //g_scene값에 따라 스테이지나 화면을 교체함.
		{
		case TITLE:
			game.UpdateTitle();//타이틀
			break;
		case INGAME:
			game.UpdateGame(); //스테이지1
			break;
		case INGAME2:
			game.UpdateGame2();//스테이지2
			break;
		case INGAME3:
			game.UpdateGame3();//스테이지3
			break;
		case GAMEOVER:
			game.UpdateOver();//게임오버
			break;
		case CLEAR:
			game.UpdateClear(); //클리어 화면
			if (game.g_scene == INGAME2)
			{
				game.ball = { 5.0f, 5.0f, 0, 0 }; //공 위치 초기화,
			}
			if (game.g_scene == INGAME3)
			{
				game.ball = { 5.0f, 12.0f, 0, 0 };
			}
			break;
		}
		Sleep(50);
	}
}
