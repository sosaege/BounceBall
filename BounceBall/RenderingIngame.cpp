#include "Game.h"
#include "Rendering.h"

void block::Draw() {

	Sprite_Draw(x, y, '#');
} //±âº»ºí·° °³Ã¼


void spike::Draw() {
	Sprite_Draw(x, y, '^');
} //°¡½Ã °´Ã¼

void clearPortal::Draw() {
	Sprite_Draw(x, y, '@');
} //Å¬¸®¾î Æ÷Å» °´Ã¼

void portal::Draw() {
	Sprite_Draw(x, y, '=');
} //Æ÷Å» °´Ã¼

void brick::Draw() {
	Sprite_Draw(x, y, 'X');
} //º®µ¹ °´Ã¼


void spring::Draw() {
	Sprite_Draw(x, y, 'Z');
} //½ºÇÁ¸µ °´Ã¼

void Draw_Ball()
{
	Sprite_Draw(game.ball.x, game.ball.y, game.ball.sprite);
} //°ø °´Ã¼

void Draw_Wall()
{
	for (int y = 0; y < dfSCREEN_HEIGHT; y++)
	{
		for (int x = 0; x < dfSCREEN_WIDTH; x++)
		{
			if (y == dfSCREEN_HEIGHT - 1)
			{
				Sprite_Draw(x, y, '_'); //¹Ù´Ú ±×¸®±â
			}

			if (x == dfSCREEN_WIDTH - 2 || x == 0)
			{
				Sprite_Draw(x, y, '|'); //º® ±×¸®±â
			}
		}

	} 
}

