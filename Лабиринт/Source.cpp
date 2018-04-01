#include <Windows.h>
#include <stdio.h>
#include <conio.h>
#include <locale.h>
#include <time.h>
#include <thread>
HWND hwnd;
HDC hdc;
HANDLE hStdOut = FindWindowW(WC_DIALOG, TEXT("lab")); //получаем handle

//---------Переменные необходимые для меню---------//
int PunctOf1Menu = 5, PunctOf2Menu = 4, PunctOf3Menu = 0, PunctOf4Menu = 0; // кол-во пунктов в меню и подменю, при необходимости дополнить
int poz = 1; // позиция в меню
int NumOfMenu = 1; // начальная страница меню

//---------Перечисляемые типы---------//
enum direction { Up, Down, Left, Right, NAD }; // направления, NAD(not a direction) - не направление
enum Status { Menu, Game, Pause }; //статус программы
enum Music { Step, BackGroungMusicMenu, BackGroungMusicGame, ChoicePointOfMenu }; //перечисление возможной музыки
Status status = Menu;

//---------Уровни---------//
short int current_level = 0; // переменная хранящая номер текущего уровня
int SizeLevels[100] = {}; // матрица хранящая размеры уровней
const int SizeSmall = 14, SizeMedium = 26, SizeBig = 38; //размерности уровня (3*3*n)+2, где n - любое число, означающее кол-во паттернов
//short int where_the_pattern_is_already_drawn[100][SizeBig / 9][SizeBig / 9];
short int Levels[100][SizeBig][SizeBig]; //трехмерная матрица хранящая все уровни
const int AmmounOfPattern = 11;
short int Pattern[AmmounOfPattern][3][3] = //набор паттернов для постройки локации
{
	{1,0,1,
	 0,0,1,
	 1,1,1},
	{1,0,1,
	 1,0,0,
	 1,1,1,},
	{1,1,1,
	 1,0,0,
	 1,0,1,},
	{1,1,1,
	 0,0,1,
	 1,0,1,},
	{1,0,1,
	 1,0,1,
	 1,0,1,},
	{1,1,1,
	 0,0,0,
	 1,1,1,},
	{1,0,1,
	 1,0,0,
	 1,0,1,},
	{1,1,1,
	 0,0,0,
	 1,0,1,},
	{1,0,1,
	 0,0,1,
	 1,0,1,},
	{1,0,1,
	 0,0,0,
	 1,1,1,},
	{1,0,1,
	 0,0,0,
	 1,0,1,}
};

//-------Хар-ки Игрока------//
struct statics {
	int Health, Strength, Dexterity, Stamin;
};
statics CharacteristicsPlayer = { 100,1,1,100 };

//---------Графика---------//
int Zume = 20; // маштаб
int rasmer = 9; // размер стен
int razPL = 5; //размер круга игрока
HPEN hpen1; //объявляем объект перо
HGDIOBJ hpenOld, hbrushOld;
HBRUSH hbrush; //объявляем кисть

//---------Координаты---------//
COORD start, finish; //координата старта, финиша
COORD pozPL = {}; //координата позиции игрока
COORD tr; //координата на экране. переменная для отрисоки стены и игрока

struct RGB {
	int r, g, b;
};
const RGB CL_WHITE = { 255,255,255 };
const RGB CL_BLACK = { 0,0,0 };
const RGB CL_YELLOW = { 255,255,0 };
const RGB CL_RED = { 255,0,0 };
const RGB CL_GREEN = { 0,255,0 };
const RGB CL_BLUE = { 0,0,255 };
const RGB CL_LIGHTBLUE = { 102,102,255 };
const RGB CL_DARKGRAY = { 102,102,102 };
const RGB CL_GRAY = { 204,204,204 };
const RGB CL_SKIN = { 226,185,143 };

class II
{
public:
	COORD pozitionII;
	COORD LastPozitionPL;
public:
	void move()
	{

	};
	bool seePL()
	{
		return 0;
	};
	COORD GetLastPozitionPL()
	{
		return LastPozitionPL;
	};
	bool WriteLastPositionPL(COORD pozition)
	{
		LastPozitionPL = pozition;
	};
	bool gotoLastPozPL()
	{

	}
};

void PlayMusic(Music Music)
{
	switch (Music)
	{
	case Step:
	{
		PlaySound(L"wood-step.wav", 0, SND_ASYNC);
	}break;
	case BackGroungMusicGame:
	{
		//PlaySound(L"BackGroungMusicGame.wav", 0, SND_ASYNC);
	}break;
	case BackGroungMusicMenu:
	{
		//PlaySound(L"BackGroungMusicMenu.wav", 0, SND_ASYNC);
	}break;
	case ChoicePointOfMenu:
	{
		PlaySound(L"click-switch.wav", 0, SND_ASYNC);
	}break;
	}
}

void ClearWindow()
{
	RECT r;
	GetClientRect(hwnd, &r);
	FillRect(hdc, &r, (HBRUSH)(COLOR_WINDOW + 1));
}
// прямоугольник по центру, длинне и высоте
void RectangleOnCOODR(COORD rect, int length, int height, RGB foregrount, RGB background)
{
	hpen1 = CreatePen(PS_SOLID, 2, RGB(foregrount.r, foregrount.g, foregrount.b)); //логическое перо с заданным стилем, шириной и цветом
	hbrush = CreateSolidBrush(RGB(background.r, background.g, background.b)); //логическая кисть с заданным стилем, шириной и цветом
	hbrushOld = (HBRUSH)SelectObject(hdc, hbrush);
	hpenOld = (HPEN)SelectObject(hdc, hpen1);
	Rectangle(hdc, rect.X + length, rect.Y + height, rect.X - length, rect.Y - height); //рисование прямоугольника
	DeleteObject(hpen1); // Удаляем созданное перо
	DeleteObject(hbrush); // Удаляем созданную кисть
}
//рисование прямоугольника по RECT
void RectangleOnRECT(RECT h, RGB foregrount, RGB background)
{
	hpen1 = CreatePen(PS_SOLID, 2, RGB(foregrount.r, foregrount.g, foregrount.b)); //логическое перо с заданным стилем, шириной и цветом
	hbrush = CreateSolidBrush(RGB(background.r, background.g, background.b)); //логическая кисть с заданным стилем, шириной и цветом
	hbrushOld = (HBRUSH)SelectObject(hdc, hbrush);
	hpenOld = (HPEN)SelectObject(hdc, hpen1);
	Rectangle(hdc, h.left, h.top, h.right, h.bottom); //рисование прямоугольника
	DeleteObject(hpen1); // Удаляем созданное перо
	DeleteObject(hbrush); // Удаляем созданную кисть
}
//рисование объекта по точкам
void SimpleFigure(COORD xy[], int countPoint, RGB foregrount)
{
	hpen1 = CreatePen(PS_SOLID, 2, RGB(foregrount.r, foregrount.g, foregrount.b)); //логическое перо с заданным стилем, шириной и цветом
	hpenOld = (HPEN)SelectObject(hdc, hpen1);
	MoveToEx(hdc, xy[0].X, xy[0].Y, NULL);
	for (int i = 1; i < countPoint; i++)
	{
		LineTo(hdc, xy[i].X, xy[i].Y);
	}
	LineTo(hdc, xy[0].X, xy[0].Y);
	DeleteObject(hpen1); // Удаляем созданное перо
}
// круг по центру, ширине и длинне
void ElipseOnCOORD(COORD rect, int length, int height, RGB foregrount, RGB background)
{
	hpen1 = CreatePen(PS_SOLID, 2, RGB(foregrount.r, foregrount.g, foregrount.b)); //логическое перо с заданным стилем, шириной и цветом
	hbrush = CreateSolidBrush(RGB(background.r, background.g, background.b)); //логическая кисть с заданным стилем, шириной и цветом
	hbrushOld = (HBRUSH)SelectObject(hdc, hbrush);
	hpenOld = (HPEN)SelectObject(hdc, hpen1);
	Ellipse(hdc, rect.X + length, rect.Y + height, rect.X - length, rect.Y - height);
	DeleteObject(hpen1); // Удаляем созданное перо
	DeleteObject(hbrush); // Удаляем созданную кисть
}
//вывод текста (не работает)
int WriteText(RECT rect, char Text[], ...)
{
	va_list argp;
	int i;
	char *OutChar;
	int *OutInt;
	float *OutFloat;
	SetBkMode(hdc, TRANSPARENT);
	va_start(argp, Text);
	for (i = 0; i < strlen(Text); i++)
	{
		if (Text[i] == '%')
		{
			if (Text[i + 1] == 'd')
			{
				OutInt = va_arg(argp, int*);
				TextOutA(hdc, rect.top, rect.right, (LPCSTR)OutInt, strlen((char*)OutInt));
				//DrawTextA(hdc, OutInt, -1, &rect, DT_LEFT);
			}
			if (Text[i + 1] == 'f')
			{
				OutFloat = va_arg(argp, float*);
				TextOutA(hdc, rect.top, rect.right, (LPCSTR)OutFloat, strlen((char*)OutFloat));
				//DrawTextA(hdc, OutFloat, -1, &rect, DT_LEFT);
			}
			if ((Text[i + 1] == 's') | (Text[i + 1] == 'c'))
			{
				OutChar = va_arg(argp, char*);
				TextOutA(hdc, rect.top, rect.right, OutChar, strlen(OutChar));
				//DrawTextA(hdc, OutChar, -1, &rect, DT_LEFT);
			}
			i++;
		}
		else
		{
			TextOutA(hdc, rect.top, rect.right, &Text[i], 2);
			//DrawTextA(hdc, (LPCSTR)Text[i], -1, &rect, DT_LEFT);
		}
	}
	va_end(argp);
	return i;
}
//вывод текста посимвольно
void WriteTextSymbolBySymbol(RECT Rect, char Text[])
{
	int i = 0, time = 0;
	//char About[] = { "Привет!*я смотрю ты заинтересовался игрой?*забавно)*А ведь я ее пилил чисто по приколу, лишь бы набраться опыта работы с WinApi*Но как не крути я забил на разработку примерно 5 месяцев и напроч забыл чему научился XD*В общем создал ее Дивольд Евгений Владимирович (bog5651)*Примерно 10.11.2017*Удачи!" };
	for (i; i < strlen(Text); i++)
	{
		if (kbhit())
		{
			break;
		}
		if (Text[i] == ',')
		{
			Sleep(200);
		}
		if (Text[i] == '*')
		{
			Sleep(time * 50);
			ClearWindow();
			time = 0;
			i++;
		}
		time++;
		//WriteText(Rect,"fff%c", Text[i]);
		Sleep(50);
	}
	Sleep(200);
}

bool Fighting(int potentialDamage, int dexterityEnemy, int *healthEnemy)
{
	if (CharacteristicsPlayer.Dexterity > dexterityEnemy)//если ловкость игрока выше ловкости врага нанести обратный дамаг
	{
		*healthEnemy -= 10;
	}
	else CharacteristicsPlayer.Health -= potentialDamage;//иначе нанести игроку потенциальный дамаг
	return 0;
}

//сравнить две координаты, вернуть true если одинаковые
bool sravn(COORD cr1, COORD cr2)
{
	if (cr1.X == cr2.X)
	{
		if (cr1.Y == cr2.Y)
		{
			return true;
		}
		else return false;
	}
	else return false;
}

bool findPattern(COORD potentialСoordinates[], int amountCoordinates, int numOfPattern[], int *ammountFindPattetn, direction direct[])
{
	bool finded = false;
	COORD check = { 0,0 };
	direction PotentionalDirect;
	int num = -1;
	for (int i = 0; i < AmmounOfPattern; i++)
	{
		for (int j = 0; j < amountCoordinates; j++)
		{
			check = potentialСoordinates[j];
			if (potentialСoordinates[j].X == 2)
			{
				check.X = 0;
				PotentionalDirect = Right;
			}
			else
			{
				if (potentialСoordinates[j].X == 0)
				{
					check.X = 2;
					PotentionalDirect = Left;
				}
			}
			if (potentialСoordinates[j].Y == 2)
			{
				check.Y = 0;
				PotentionalDirect = Down;
			}
			else
			{
				if (potentialСoordinates[j].Y == 0)
				{
					check.Y = 2;
					PotentionalDirect = Up;
				}
			}
			if (Pattern[i][check.Y][check.X] == 0)
			{
				numOfPattern[++num] = i; //записать номер паттерна
				direct[num] = PotentionalDirect;
				finded = true;
			}
		}
	}
	*ammountFindPattetn = num + 1;
	return finded;
}

bool check_an_opportunity_to_draw_a_pattern(COORD position, COORD potentialСoordinates[], int *amountCoordinates)
{
	bool finded = false;
	int num = -1;
	COORD check = position;
	check.X--;
	for (int i = 0; i < 8; i++)
	{
		if (i < 3)
		{
			check.X++;
			if (Levels[current_level][check.Y][check.X] == 0) //проверка верхней части куба
			{
				if ((check.Y - 1) > 0) //проверка края карты
				{
					potentialСoordinates[++num].X = check.X - position.X;
					potentialСoordinates[num].Y = check.Y - position.Y;
					finded = true;
				}
			}
		}
		if ((i > 2) && (i < 5))
		{
			check.Y++;
			if (Levels[current_level][check.Y][check.X] == 0)//проверка правой части куба
			{
				if ((check.X + 1) < SizeLevels[current_level]) //проверка края карты
				{
					potentialСoordinates[++num].X = check.X - position.X;
					potentialСoordinates[num].Y = check.Y - position.Y;
					finded = true;
				}
			}
		}
		if ((i > 4) && (i < 7))
		{
			check.X--;
			if (Levels[current_level][check.Y][check.X] == 0)//проверка нижней части куба
			{
				if ((check.Y + 1) < SizeLevels[current_level]) //проверка края карты
				{
					potentialСoordinates[++num].X = check.X - position.X;
					potentialСoordinates[num].Y = check.Y - position.Y;
					finded = true;
				}
			}
		}
		if (i == 7)
		{
			check.Y--;
			if (Levels[current_level][check.Y][check.X] == 0)//провека левой чатси куба
			{
				if ((check.X - 1) > 0) //проверка края карты
				{
					potentialСoordinates[++num].X = check.X - position.X;
					potentialСoordinates[num].Y = check.Y - position.Y;
					finded = true;
				}
			}
		}
	}
	*amountCoordinates = num + 1;
	return finded;
}

void DrawPattern(COORD pozition, int NumberOfPattern)
{
	int i, j, PatternI, PatternJ;
	for (i = pozition.Y, PatternI = 0; i < pozition.Y + 3; i++, PatternI++)
	{
		for (j = pozition.X, PatternJ = 0; j < pozition.X + 3; j++, PatternJ++)
		{
			Levels[current_level][i][j] = Pattern[NumberOfPattern][PatternI][PatternJ];
		}
	}
}

void GenerateLocation()
{
	int i, j, k;
	direction direct[AmmounOfPattern * 8];
	COORD positionOFDraw = { 1,1 };
	COORD potentialСoordinates[8] = {};
	int amountCoordinates = 0;
	int amountFindPattern = 0;
	int numOfPattern[AmmounOfPattern * 8] = {};
	int numOfLastPattern = 0;

	int buf = rand() % 2; //выбор размера карты (2 переставить на 3)TODO
	switch (buf)
	{
	case 0:
		SizeLevels[current_level] = SizeSmall;
		break;
	case 1:
		SizeLevels[current_level] = SizeMedium;
		break;
	case 2:
		SizeLevels[current_level] = SizeBig;
		break;
	default:
		break;
	}

	for (i = 0; i < SizeLevels[current_level]; i++) //заполнение Уровня единицами
	{
		for (j = 0; j < SizeLevels[current_level]; j++)
		{
			Levels[current_level][i][j] = 1;
		}
	}

	numOfLastPattern = 4 + rand() % 2;
	DrawPattern(positionOFDraw, numOfLastPattern);//начальный паттерн

	pozPL = finish = start = { 2,2 };

	srand(time(NULL));
	for (i = 0; i < SizeLevels[current_level]; i++)
	{

		if (check_an_opportunity_to_draw_a_pattern(positionOFDraw, potentialСoordinates, &amountCoordinates))
		{
			if (findPattern(potentialСoordinates, amountCoordinates, numOfPattern, &amountFindPattern, direct))
			{
				buf = rand() % amountFindPattern;
				switch (direct[buf])
				{
				case Up:
					positionOFDraw.Y -= 3;
					break;
				case Right:
					positionOFDraw.X += 3;
					break;
				case Down:
					positionOFDraw.Y += 3;
					break;
				case Left:
					positionOFDraw.X -= 3;
					break;
				default:
					break;
				}
				DrawPattern(positionOFDraw, numOfPattern[buf]);
				numOfLastPattern = numOfPattern[buf];
			}
		}
		else
			break;
	}
}

//установить в переменную tr по маштабу и смещению для отрисовки на экране
void Set_Poz(COORD poz)
{
	int zume = 20; //масштаб 
	int smesh = 100; //смещение 
	tr = poz;
	tr.X = smesh + (tr.X*zume);
	tr.Y = smesh + (tr.Y*zume);
}
//true если в выбранном направлении от позиции игрока(pozPL)
bool proverka(direction direct)
{
	switch (direct)
	{
	case Up:
		if (Levels[current_level][pozPL.Y - 1][pozPL.X] == 1)
		{
			return true;
		}
		else return false;
		break;
	case Down:
		if ((Levels[current_level][pozPL.Y + 1][pozPL.X] == 1))
		{
			return true;
		}
		else return false;
		break;
	case Left:
		if (Levels[current_level][pozPL.Y][pozPL.X - 1] == 1)
		{
			return true;
		}
		else return false;
		break;
	case Right:
		if (Levels[current_level][pozPL.Y][pozPL.X + 1] == 1)
		{
			return true;
		}
		else return false;
		break;
	default:
		break;
	}
}
//нарисовать всю карту 
void DrawAllCurrentLevel()
{
	int i, j;
	COORD ij;
	for (i = 0; i < SizeLevels[current_level]; i++)
	{
		for (j = 0; j < SizeLevels[current_level]; j++)
		{
			ij.X = i;
			ij.Y = j;
			if (Levels[current_level][j][i] == 1)
			{
				Set_Poz(ij);
				RectangleOnCOODR(tr, rasmer, rasmer, CL_RED, CL_YELLOW);
			}
		}
	}
}
//возврящает 1 если достигнут финиш
// и 0 если еще не финиш
bool butgame(char key)
{
	RGB emp = { 255,255,255 }; // черный цвет для закраски "следа" за игрком
	direction direct = Up; // под направления 1-вверх 2-вниз 3-лево 4- право 
	Set_Poz(pozPL);
	ElipseOnCOORD(tr, razPL, razPL, CL_RED, CL_YELLOW);
	switch (key)
	{
	case 38: //up
		direct = Up;
		if (proverka(direct))
		{
			Set_Poz(pozPL);
			tr.Y -= Zume;
			RectangleOnCOODR(tr, rasmer, rasmer, CL_RED, CL_YELLOW);
		}
		else
		{
			Set_Poz(pozPL);
			RectangleOnCOODR(tr, rasmer, rasmer, CL_WHITE, CL_WHITE);
			pozPL.Y--;
			Set_Poz(pozPL);
			ElipseOnCOORD(tr, razPL, razPL, CL_RED, CL_YELLOW);
		}
		break;
	case 40: //down
		direct = Down;
		if (proverka(direct))
		{
			Set_Poz(pozPL);
			tr.Y += Zume;
			RectangleOnCOODR(tr, rasmer, rasmer, CL_RED, CL_YELLOW);
		}
		else
		{
			Set_Poz(pozPL);
			RectangleOnCOODR(tr, rasmer, rasmer, CL_WHITE, CL_WHITE);
			pozPL.Y++;
			Set_Poz(pozPL);
			ElipseOnCOORD(tr, razPL, razPL, CL_RED, CL_YELLOW);
		}
		break;
	case 37: //left
		direct = Left;
		if (proverka(direct))
		{
			Set_Poz(pozPL);
			tr.X -= Zume;
			RectangleOnCOODR(tr, rasmer, rasmer, CL_RED, CL_YELLOW);
		}
		else
		{
			Set_Poz(pozPL);
			RectangleOnCOODR(tr, rasmer, rasmer, CL_WHITE, CL_WHITE);
			pozPL.X--;
			Set_Poz(pozPL);
			ElipseOnCOORD(tr, razPL, razPL, CL_RED, CL_YELLOW);
		}
		break;
	case 39: //right;
		direct = Right;
		if (proverka(direct))
		{
			Set_Poz(pozPL);
			tr.X += Zume;
			RectangleOnCOODR(tr, rasmer, rasmer, CL_RED, CL_YELLOW);
		}
		else
		{
			Set_Poz(pozPL);
			RectangleOnCOODR(tr, rasmer, rasmer, CL_WHITE, CL_WHITE);
			pozPL.X++;
			Set_Poz(pozPL);
			ElipseOnCOORD(tr, razPL, razPL, CL_RED, CL_YELLOW);
		}
		break;
	case 13: DrawAllCurrentLevel(); break; //enter
	}
	if (sravn(pozPL, finish)) // проверка на финиширование
	{
		if (MessageBoxW(hwnd, TEXT("YOU WIN\nОК выйти в меню, CANCEL - остаться"), TEXT("YOU WIN"), MB_OKCANCEL) == 1) // если нажмет ОК выйти в меню, CANCEL - остаться
			return 1;
	}
	return 0;
}

void DrowGame()
{
	/*std::thread BackGround(PlayMusic, BackGroungMusicGame);
	BackGround.detach();*/
	RGB fin = { 139, 0, 255 }, strt = { 211, 31, 94 }; //цвета заливки для старта и финиша
	Set_Poz(start);
	RectangleOnCOODR(tr, rasmer, rasmer, CL_RED, CL_YELLOW);
	Set_Poz(finish);
	RectangleOnCOODR(tr, rasmer, rasmer, CL_BLUE, CL_BLUE);
	Set_Poz(pozPL);
	ElipseOnCOORD(tr, razPL, razPL, CL_BLACK, CL_YELLOW);
}
/*функция отрисовки меню, по кол-ву punkt, enable - выделенный пункт меню,
gran - граница пунктов, zalivka - заливка кнопки,
далее массивы char для заполнения пунктов меню*/
void menu(int punkt, int enable, RGB foreground, RGB background, ...)
{
	ClearWindow();
	va_list argp;
	char *p;
	int i;
	RGB EnableGran = { 0,255,0 }, EnableZalivka = { 0,0,255 };
	RECT Pbottun = { 0,50,350,0 }, text = { 0,65,350,0 };
	Pbottun.left = Pbottun.right + 215;
	Pbottun.bottom = Pbottun.top + 50;
	text.left = text.right + 215;
	text.bottom = text.top + 50;
	SetBkMode(hdc, TRANSPARENT);
	va_start(argp, background);
	for (i = 1; i <= punkt; i++)
	{
		if (i == enable) RectangleOnRECT(Pbottun, EnableGran, EnableZalivka);
		else RectangleOnRECT(Pbottun, foreground, background);
		p = va_arg(argp, char*);
		DrawTextA(hdc, p, -1, &text, DT_CENTER);
		Pbottun.top = text.top = text.bottom = Pbottun.bottom + 20;
		text.top += 15;
		Pbottun.bottom = text.bottom += 50;
		DrawTextA(hdc, "\0", -1, &text, DT_CENTER);
	}
	va_end(argp);
}
//Вызов Отрисовки меню
void DrawMenu(int poz)
{
	switch (NumOfMenu)
	{
	case 1: menu(PunctOf1Menu, poz, CL_RED, CL_YELLOW, "Играть", "Об игре", "Выбор сложности", "Рекорды", "Выход"); break;
	case 2: menu(PunctOf2Menu, poz, CL_RED, CL_YELLOW, "Легко", "Нормально", "Сложно", "Назад"); break;
	default:
		break;
	}
}
//обработка позиции меню
void nummenu(int *poz)
{
	std::thread Music(PlayMusic, ChoicePointOfMenu);
	Music.detach();
	switch (NumOfMenu)
	{
	case 1: //главное меню
	{
		switch (*poz) //смотрим на какой позиции в этом меню
		{
		case 1:*poz = 1; DrowGame(); ClearWindow(); status = Game; GenerateLocation(); break; //выполняем то что было на первом пункте и так далее
		case 2:*poz = 1; /*WriteTextSymbolBySymbol({ 0,65,350,0 }, "Привет!*я смотрю ты заинтересовался игрой?*забавно)*А ведь я ее пилил чисто по приколу, лишь бы набраться опыта работы с WinApi*Но как не крути я забил на разработку примерно 5 месяцев и напроч забыл чему научился XD*В общем создал ее Дивольд Евгений Владимирович (bog5651)*Примерно 10.11.2017*Удачи!");*/ break;
		case 3:*poz = 1; NumOfMenu = 2; break;
		case 4:*poz = 1; break;
		case 5:*poz = 1; exit(NULL); break;//выход из программы(совсем)
		default: *poz = 1; break;
		}
	} break;
	case 2: //меню выбора сложности
	{
		switch (*poz) //смотрим на какой позиции в этом меню
		{
		case 1:*poz = 1; current_level = 0; GenerateLocation(); break; //выполняем то что было на первом пункте и так далее
		case 2:*poz = 1; current_level = 1; GenerateLocation(); break;
		case 3:*poz = 1; current_level = 2; GenerateLocation(); break;
		case 4:*poz = 1; NumOfMenu = 1; break;
		default: *poz = 1; break;
		}
	} break;
	default:
		break;
	}
}
//обработка клавиш в меню и изменение пункта меню
void butMenu(char key, int punkts, int* poz)
{
	switch (key)
	{
	case 38: //up
		if (*poz == 1)
		{
			*poz = punkts;
		}
		else *poz = *poz - 1;
		break;
	case 40: //down
		if (*poz == punkts)
		{
			*poz = 1;
		}
		else *poz = *poz + 1;
		break;
	case 37: break; //left wtf зачем я их сюда дописал? TODO прикрутить на них что-либо
	case 39: break; //right;
	case 13: nummenu(&*poz); break; //enter
	}
}

LRESULT WINAPI mainProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	//обрабатка сообщений.
	switch (message)
	{
	case WM_PAINT:
	{
		switch (status)
		{
		case Menu:
		{
			DrawMenu(poz);
		}break;
		case Game:
		{
			DrowGame();
		}break;
		default:
			break;
		}
	}
	break;
	case WM_KEYUP:
	{
		char key[] = { (TCHAR)wParam,'\0' };
		switch (status)
		{
		case Menu:
		{
			switch (NumOfMenu)
			{
			case 1: butMenu(key[0], PunctOf1Menu, &poz); break;
			case 2: butMenu(key[0], PunctOf2Menu, &poz); break;
			case 3: butMenu(key[0], PunctOf3Menu, &poz); break;
			case 4: butMenu(key[0], PunctOf4Menu, &poz); break;
			}
		}break;
		case Game:
		{
			std::thread Step(PlayMusic, Step);
			Step.detach();
			if (butgame(key[0]))
				status = Menu;;
		}break;
		default:
			break;
		}
		SendMessageW(hwnd, WM_PAINT, NULL, NULL);
	}
	break;
	case WM_CLOSE:
	{
		PostQuitMessage(0);
	}
	break;
	}
	return 0;
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	setlocale(LC_ALL, "RUS");
	MSG msg;
	hwnd = CreateWindowExW(0, WC_DIALOG, TEXT("lab"), WS_VISIBLE | WS_SYSMENU, 500, 200, 900, 800, 0, 0, 0, 0);
	hdc = GetDC(hwnd);
	SetWindowLong(hwnd, DWL_DLGPROC, (long)mainProc);
	while (GetMessage(&msg, 0, 0, 0) != 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;
}