#include "Game.h"
#include "Rendering.h"

void block::Draw() {

	Sprite_Draw(x, y, '#');
} //�⺻�� ��ü


void spike::Draw() {
	Sprite_Draw(x, y, '^');
} //���� ��ü

void clearPortal::Draw() {
	Sprite_Draw(x, y, '@');
} //Ŭ���� ��Ż ��ü

void portal::Draw() {
	Sprite_Draw(x, y, '=');
} //��Ż ��ü

void brick::Draw() {
	Sprite_Draw(x, y, 'X');
} //���� ��ü


void spring::Draw() {
	Sprite_Draw(x, y, 'Z');
} //������ ��ü

void Draw_Ball()
{
	Sprite_Draw(game.ball.x, game.ball.y, game.ball.sprite);
} //�� ��ü

void Draw_Wall()
{
	for (int y = 0; y < dfSCREEN_HEIGHT; y++)
	{
		for (int x = 0; x < dfSCREEN_WIDTH; x++)
		{
			if (y == dfSCREEN_HEIGHT - 1)
			{
				Sprite_Draw(x, y, '_'); //�ٴ� �׸���
			}

			if (x == dfSCREEN_WIDTH - 2 || x == 0)
			{
				Sprite_Draw(x, y, '|'); //�� �׸���
			}
		}

	} 
}

