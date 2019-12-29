#include <stdio.h>
#include <time.h>
#include <Windows.h>
#include <math.h>

#define TRUE  1
#define FALSE 0

#define FPS 30

#define MAP_WIDTH  30
#define MAP_HEIGHT 30

#define ENEMY_MOVE_TERM 100
#define SPEED_CORRECTION_VALUE 0.01f

int playing = TRUE;
int score = 0;

int enemyCreateTerm;
int enemy[MAP_WIDTH][MAP_HEIGHT];

float playerX = MAP_WIDTH / 2;
float playerY = MAP_HEIGHT - 3;

float playerSpeed = 5;

void gotoxy(int x, int y);
int hu(float input);

void Init()
{
	enemyCreateTerm = (MAP_HEIGHT - 2) * ENEMY_MOVE_TERM;

	// initialize enemy array
	for (int y = 0; y < MAP_HEIGHT; y++)
	{
		for (int x = 0; x < MAP_WIDTH; x++)
		{
			enemy[x][y] = 0;
		}
	}
}

int lastCreateTime  = 0;
int lastMoveTime    = 0;

void Update(int term)
{
	float tempX = playerX;
	int xDirection = 0;

	if (GetAsyncKeyState(VK_LEFT) & 0x8001) // ¿ÞÂÊ
		xDirection = -1;
	else if (GetAsyncKeyState(VK_RIGHT) & 0x8001) // ¿À¸¥ÂÊ
		xDirection = 1;

	tempX += term * playerSpeed * SPEED_CORRECTION_VALUE * xDirection;
	if (!(tempX < 1 || tempX > MAP_WIDTH - 1))
		playerX = tempX;


	lastCreateTime += term;
	lastMoveTime += term;

	if (lastCreateTime > enemyCreateTerm) // create enemy
	{
		for (int x = 1; x < MAP_WIDTH - 1; x++) // 1 ~ 18
		{
			enemy[x][0] = rand() % 2;
		}

		score += 100;
		lastCreateTime = 0;
	}
	else if (lastMoveTime > ENEMY_MOVE_TERM) // move enemy
	{
		int newEnemy[MAP_WIDTH][MAP_HEIGHT];

		for (int y = 0; y < MAP_HEIGHT - 2; y++)
		{
			for (int x = 0; x < MAP_WIDTH - 1; x++)
			{
				if (enemy[x][y] == 1)
				{
					newEnemy[x][y + 1] = 1;

					if (hu(playerX) == x && hu(playerY) == y) // collapse
						playing = FALSE;
				}
				else
					newEnemy[x][y + 1] = 0;
			}
		}

		memcpy(enemy, newEnemy, sizeof(enemy)); // copy newEnemy to enemy

		lastMoveTime = 0;
	}
}

void Render()
{
	gotoxy(0, 0);
	int px = hu(playerX);
	int py = hu(playerY);

	for (int y = 0; y < MAP_HEIGHT; y++)
	{
		for (int x = 0; x < MAP_WIDTH; x++)
		{
			if (x == 0 || x == MAP_WIDTH - 1) // wall
			{
				printf("0");
			}
			else if (y == 0 || y == MAP_HEIGHT - 1) // wall
			{
				printf("0");
			}
			else if (enemy[x][y]) // enemy
			{
				printf("V");
			}
			else if (px == x && py == y) // player
			{
				printf("#");
			}
			else // air
			{
				printf(" ");
			}
		}
		
		printf("\n");
	}

	printf("%d, %d              \n", px, py);
	printf("score : %d          \n", score);

	if (!playing)
	{
		printf("\n\n ================= GAME OVER ================ \n\n");
	}
}

void Release()
{
	printf("program end\n");
	system("pause");
}

int main()
{
	Init();

	int previousTime = clock();
	int frameTerm = 1000 / FPS;

	while (playing)
	{
		int term = clock() - previousTime;
		if (term < frameTerm)
			continue;

		previousTime = clock();

		Update(term);
		Render();
	}

	Release();
}

void gotoxy(int x, int y)
{
	COORD pos = { x,y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

int hu(float input)
{
	return floor(input + 0.5);
}

