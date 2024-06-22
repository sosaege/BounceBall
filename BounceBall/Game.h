#ifndef GAME_H
# define GAME_H

#include <string>
#include <vector>
#include "Console.h"
#include <functional>
#include "main.h"

class block {
public:
	float x;
	float y;

	void Draw();
};

class spike {
public:
	float x;
	float y;

	void Draw();
};

class clearPortal {
public:
	float x;
	float y;

	void Draw();
};

class portal {
public:
	float x;
	float y;

	void Draw();
};

class brick {
public:
	float x;
	float y;

	void Draw();
};


class spring {
public:
	float x;
	float y;

	void Draw();
};


class Ball {
public:
    float x, y;
    float vx, vy;
	const char	sprite = 'O';

    Ball() : x(0), y(0), vx(0), vy(0) {}
    Ball(float x, float y, float vx, float vy) : x(x), y(y), vx(vx), vy(vy) {}

    Ball(const Ball& other) : x(other.x), y(other.y), vx(other.vx), vy(other.vy) {}

    Ball& operator=(const Ball& other) {
        if (this != &other) {
            x = other.x;
            y = other.y;
            vx = other.vx;
            vy = other.vy;
        }
        return *this;
    }
};

//extern Ball ball;
//const float gravity = 0.15f;
//const float elasticity = 1.5f;
//const float friction = 0.9f;

class Game {
public:
    int g_scene;
    Ball ball;
    float gravity;
    float elasticity;
    float elasticity2;
    int tmp;
    bool brickHit[3];

    Game() : g_scene(TITLE), gravity(0.15f), elasticity(1.5f), elasticity2(-1.15f), tmp(0) {
        ball = { 5.0f, 12.0f, 0.0f, 0.0f };
        std::fill(std::begin(brickHit), std::end(brickHit), 0);
    }
    void UpdateTitle();
    void UpdateGame();
    void UpdateClear();
    void UpdateGame2();
    void UpdateGame3();
    void UpdateOver();

    void RenderMessage(const char* message);
    void PhysicsEngine();
};

extern Game game;

#endif
