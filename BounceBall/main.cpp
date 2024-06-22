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
	// ������ ���� ����
	// �� ������  1�� ���� 1�������̴�.
	//--------------------------------------------------------------------
	while (1)
	{
		switch (game.g_scene) //g_scene���� ���� ���������� ȭ���� ��ü��.
		{
		case TITLE:
			game.UpdateTitle();//Ÿ��Ʋ
			break;
		case INGAME:
			game.UpdateGame(); //��������1
			break;
		case INGAME2:
			game.UpdateGame2();//��������2
			break;
		case INGAME3:
			game.UpdateGame3();//��������3
			break;
		case GAMEOVER:
			game.UpdateOver();//���ӿ���
			break;
		case CLEAR:
			game.UpdateClear(); //Ŭ���� ȭ��
			if (game.g_scene == INGAME2)
			{
				game.ball = { 5.0f, 5.0f, 0, 0 }; //�� ��ġ �ʱ�ȭ,
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
