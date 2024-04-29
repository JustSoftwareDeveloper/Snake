#include <iostream>
#include <Windows.h>
using namespace std;

//прототипы функций
void snakeController(); //тут считывается пользовательский ввод, и меняется направление змейки
void snakeUpdater(); //тут происходят все изменения над змейкой
void displayBoard(char* map); //выводит игровое поле 
void appleSpawner(); //создаёт яблоки
void gotoxy(int x, int y); //переводит курсор в нужное положение
bool gameOver(); //окно проигрыша

//глобальные константы
const int WIDTH = 35; //ширина карты
const int HEIGHT = 18; //высота карты
const int MAX_LEN_SNAKE = (WIDTH - 3) * (HEIGHT - 2); //максимальная длина змейки
const int UP = 1; //движение вверх
const int DOWN = 2; //движение вниз
const int LEFT = 3; //движение влево
const int RIGHT = 4; //движение вправо

//глобальные переменные
char snake = 'O'; //символ для змейки
char apple = '+'; //символ для аблок
bool isRunning = true; //идёт ли игра
int snakeDir = UP; //текущее направление
int snakeX[MAX_LEN_SNAKE] = { 0 }; //масив для координатов всех частей змейки по X
int snakeY[MAX_LEN_SNAKE] = { 0 }; //масив для координатов всех частей змейки по Y
int snakeLength = 1; //текущая длина змейки
int appleX; //координата яблока по Х
int appleY; //координата яблока по Y
int bestScore = 1; //лучший рещультат
float stopwatch = clock(); //секундомер. Он нужен для задержки между кадрами


int main()
{
	do
	{
		//ставим голову в середину
		snakeX[0] = WIDTH / 2; 
		snakeY[0] = HEIGHT / 2;

		appleSpawner();

		while (isRunning)
		{
			//выйти, если нажата ESC
			if (GetKeyState(VK_ESCAPE) & 0x100)
			{
				cout << "Closing the game.";
				return 0;
			}

			//задержка между кадрами
			if ((clock() - stopwatch) / CLOCKS_PER_SEC >= 0.1)
			{
				//карта
				char map[] =
					"##################################\n"
					"#                                #\n"
					"#                                #\n"
					"#                                #\n"
					"#                                #\n"
					"#                                #\n"
					"#                                #\n"
					"#                                #\n"
					"#                                #\n"
					"#                                #\n"
					"#                                #\n"
					"#                                #\n"
					"#                                #\n"
					"#                                #\n"
					"#                                #\n"
					"#                                #\n"
					"#                                #\n"
					"##################################\n";

				snakeController();
				snakeUpdater();
				displayBoard(map);
				stopwatch = clock(); //обновляем секундомер

				//если змейка столкнулась со своим хвостом
				for (int i = 1; i < snakeLength; i++)
				{
					if (snakeX[0] == snakeX[i] && snakeY[0] == snakeY[i])
					{
						isRunning = false;
						i = snakeLength;
					}
				}
			}
		}
	} while (gameOver());
}

void snakeController()
{
	if (GetKeyState(VK_UP) & 0x100 && snakeDir != DOWN)
		snakeDir = UP;

	else if (GetKeyState(VK_DOWN) & 0x100 && snakeDir != UP)
		snakeDir = DOWN;

	else if (GetKeyState(VK_LEFT) & 0x100 && snakeDir != RIGHT)
		snakeDir = LEFT;

	else if (GetKeyState(VK_RIGHT) & 0x100 && snakeDir != LEFT)
		snakeDir = RIGHT;
}

void snakeUpdater()
{
	//сдвигаем каждый элемент змейки на позицию предыдущего элемента
	for (int i = snakeLength - 2; i >= 0; i--)
	{
		snakeX[i + 1] = snakeX[i];
		snakeY[i + 1] = snakeY[i];
	}

	//движение змейки
	if (snakeDir == UP) { --snakeY[0]; }
	if (snakeDir == DOWN) { ++snakeY[0]; }
	if (snakeDir == LEFT) { --snakeX[0]; }
	if (snakeDir == RIGHT) { ++snakeX[0]; }

	//появление змейки с другой части стены
	if (snakeX[0] == 0)
		snakeX[0] = WIDTH - 3;
	if (snakeX[0] == WIDTH - 2)
		snakeX[0] = 1;
	if (snakeY[0] == 0)
		snakeY[0] = HEIGHT - 2;
	if (snakeY[0] == HEIGHT - 1)
		snakeY[0] = 1;

	//если голова змейки находится на яблоке, спавним новое яблоко
	if ( (snakeX[0] == appleX || snakeX[1] == appleX) && (snakeY[0] == appleY || snakeY[1] == appleY) )
	{
		++snakeLength;
		appleSpawner();
	}
}

void displayBoard(char* map)
{
	gotoxy(0, 0); //переносим курсор в начало

	//обновляем рекорд
	if (snakeLength > bestScore)
		bestScore = snakeLength;

	//инструкция
	cout << "Use the arrows to change the direction" << endl;
	cout << "To exit the game, press ESC" << endl;
	cout << "The best score - " << bestScore << endl;
	cout << "The snake length - " << snakeLength << endl;

	//прорисовываем каждый элемент змейки на карту
	for (int i = 0; i < snakeLength; i++)
	{
		map[snakeY[i] * WIDTH + snakeX[i]] = snake;
	}

	map[appleY * WIDTH + appleX] = apple; //прорисовываем яблоко

	cout << map; // выводим карту на экран

	//удаляем элементы змейки с карты
	for (int i = 0; i < snakeLength; i++)
	{
		map[snakeY[i] * WIDTH + snakeX[i]] = ' ';
	}
}

void appleSpawner()
{
	bool isOccupied;
	do
	{
		isOccupied = false;
		
		//ищем случаной место поля
		appleX = rand() % (WIDTH - 3) + 1; 
		appleY = rand() % (HEIGHT - 2) + 1;

		//проверяем, зането ли это поле змейкой
		for (int i = 0; i < snakeLength; i++)
		{
			if (appleX == snakeX[i] && appleY == snakeY[i])
			{
				isOccupied = true;
				break;
			}
		}
	} while (isOccupied);
}

bool gameOver()
{	
	//если победил
	if (snakeLength == MAX_LEN_SNAKE)
	{
		gotoxy(12, HEIGHT / 2);
		cout << "You won!";
	}
	//если проиграл
	else
	{
		gotoxy(12, HEIGHT / 2);
		cout << "Game Over!";
	}
	gotoxy(7, HEIGHT / 2 + 1);
	cout << "press ENTER to repeat";

	cin.get(); //ожидаем нажатия ENTER

	isRunning = true;
	snakeLength = 1;
	system("cls"); //очищаем игровое поле
	return true;
}

void gotoxy(int x, int y) {
	COORD pos = { x, y };
	HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(output, pos);
}