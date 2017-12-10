#include <Windows.h>
#include <stdio.h>
#include <conio.h>
#include <locale.h>
#include <time.h>
#include <thread>
HWND hwnd;
HDC hdc;
HANDLE hStdOut = FindWindowW(WC_DIALOG, TEXT("lab")); //�������� handle

//---------���������� ����������� ��� ����---------//
int PunctOf1Menu = 5, PunctOf2Menu = 4, PunctOf3Menu = 0, PunctOf4Menu = 0; // ���-�� ������� � ���� � �������, ��� ������������� ���������
int poz = 1; // ������� � ����
int NumOfMenu = 1; // ��������� �������� ����

//---------������������� ����---------//
enum direction { Up, Down, Left, Right, NAD }; // �����������, NAD(not a direction) - �� �����������
enum Status { Menu, Game, Pause }; //������ ���������
enum Music { Step, BackGroungMusicMenu, BackGroungMusicGame, ChoicePointOfMenu }; //������������ ��������� ������
Status status = Menu;

//---------������---------//
short int current_level = 0; // ���������� �������� ����� �������� ������
int SizeLevels[100] = {}; // ������� �������� ������� �������
const int SizeSmall = 14, SizeMedium = 26, SizeBig = 38; //����������� ������ (3*3*n)+2, ��� n - ����� �����, ���������� ���-�� ���������
//short int where_the_pattern_is_already_drawn[100][SizeBig / 9][SizeBig / 9];
short int Levels[100][SizeBig][SizeBig]; //���������� ������� �������� ��� ������
const int AmmounOfPattern = 11;
short int Pattern[AmmounOfPattern][3][3] = //����� ��������� ��� ��������� �������
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

//---------�������---------//
int Zume = 20; // ������
int rasmer = 9; // ������ ����
int razPL = 5; //������ ����� ������
HPEN hpen1; //��������� ������ ����
HGDIOBJ hpenOld, hbrushOld;
HBRUSH hbrush; //��������� �����

//---------����������---------//
COORD start, finish; //���������� ������, ������
COORD pozPL = {}; //���������� ������� ������
COORD tr; //���������� �� ������. ���������� ��� �������� ����� � ������

struct RGB
{
	int r, g, b;
};
struct RGB gan = { 255, 0, 0 }, zal = { 255,255,0 }; // ����������� gan - �������, zal - ������(�� ���� ������� � �������)

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
// ������������� �� ������+������
void rect(COORD rect, int rad, RGB gran, RGB zalivka)
{
	hpen1 = CreatePen(PS_SOLID, 2, RGB(gran.r, gran.g, gran.b)); //���������� ���� � �������� ������, ������� � ������
	hbrush = CreateSolidBrush(RGB(zalivka.r, zalivka.g, zalivka.b)); //���������� ����� � �������� ������, ������� � ������
	hbrushOld = (HBRUSH)SelectObject(hdc, hbrush);
	hpenOld = (HPEN)SelectObject(hdc, hpen1);
	Rectangle(hdc, rect.X + rad, rect.Y + rad, rect.X - rad, rect.Y - rad); //��������� ��������������
	DeleteObject(hpen1);       // ������� ��������� ����
	DeleteObject(hbrush);       // ������� ��������� �����
}
//��������� �������������� �� RECT
void rect(RECT h, RGB gran, RGB zalivka)
{
	hpen1 = CreatePen(PS_SOLID, 2, RGB(gran.r, gran.g, gran.b)); //���������� ���� � �������� ������, ������� � ������
	hbrush = CreateSolidBrush(RGB(zalivka.r, zalivka.g, zalivka.b)); //���������� ����� � �������� ������, ������� � ������
	hbrushOld = (HBRUSH)SelectObject(hdc, hbrush);
	hpenOld = (HPEN)SelectObject(hdc, hpen1);
	Rectangle(hdc, h.left, h.top, h.right, h.bottom); //��������� ��������������
	DeleteObject(hpen1);       // ������� ��������� ����
	DeleteObject(hbrush);       // ������� ��������� �����
}
//��������� �������������� �� ������
void rect(COORD xy1, COORD xy2, COORD xy3, COORD xy4, RGB gran)
{
	hpen1 = CreatePen(PS_SOLID, 2, RGB(gran.r, gran.g, gran.b)); //���������� ���� � �������� ������, ������� � ������
	MoveToEx(hdc, xy1.X, xy1.Y, NULL);
	LineTo(hdc, xy2.X, xy2.Y);
	LineTo(hdc, xy3.X, xy3.Y);
	LineTo(hdc, xy4.X, xy4.Y);
	LineTo(hdc, xy1.X, xy1.Y);
	DeleteObject(hpen1);       // ������� ��������� ����
}
// ���� �� ������+������
void elps(COORD rect, int rad, RGB gran, RGB zalivka)
{
	hpen1 = CreatePen(PS_SOLID, 2, RGB(gran.r, gran.g, gran.b)); //���������� ���� � �������� ������, ������� � ������
	hbrush = CreateSolidBrush(RGB(zalivka.r, zalivka.g, zalivka.b)); //���������� ����� � �������� ������, ������� � ������
	hbrushOld = (HBRUSH)SelectObject(hdc, hbrush);
	hpenOld = (HPEN)SelectObject(hdc, hpen1);
	Ellipse(hdc, rect.X + rad, rect.Y + rad, rect.X - rad, rect.Y - rad);
	DeleteObject(hpen1);       // ������� ��������� ����
	DeleteObject(hbrush);       // ������� ��������� �����
}
//����� ������ �� ��������
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
			if ((Text[i + 1] == 's')| (Text[i + 1] == 'c'))
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
//����� ������ �����������
void WriteTextSymbolBySymbol(RECT Rect,char Text[])
{
	int i = 0, time=0;
	//char About[] = { "������!*� ������ �� ��������������� �����?*�������)*� ���� � �� ����� ����� �� �������, ���� �� ��������� ����� ������ � WinApi*�� ��� �� ����� � ����� �� ���������� �������� 5 ������� � ������ ����� ���� �������� XD*� ����� ������ �� ������� ������� ������������ (bog5651)*�������� 10.11.2017*�����!" };
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
			Sleep(time*50);
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

//�������� ��� ����������, ������� true ���� ����������
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

bool findPattern(COORD potential�oordinates[], int amountCoordinates, int numOfPattern[], int *ammountFindPattetn, direction direct[])
{
	bool finded = false;
	COORD check = { 0,0 };
	direction PotentionalDirect;
	int num = -1;
	for (int i = 0; i < AmmounOfPattern; i++)
	{
		for (int j = 0; j < amountCoordinates; j++)
		{
			check = potential�oordinates[j];
			if (potential�oordinates[j].X == 2)
			{
				check.X = 0;
				PotentionalDirect = Right;
			}
			else
			{
				if (potential�oordinates[j].X == 0)
				{
					check.X = 2;
					PotentionalDirect = Left;
				}
			}
			if (potential�oordinates[j].Y == 2)
			{
				check.Y = 0;
				PotentionalDirect = Down;
			}
			else
			{
				if (potential�oordinates[j].Y == 0)
				{
					check.Y = 2;
					PotentionalDirect = Up;
				}
			}
			if (Pattern[i][check.Y][check.X] == 0)
			{
				numOfPattern[++num] = i; //�������� ����� ��������
				direct[num] = PotentionalDirect;
				finded = true;
			}
		}
	}
	*ammountFindPattetn = num + 1;
	return finded;
}

bool check_an_opportunity_to_draw_a_pattern(COORD position, COORD potential�oordinates[], int *amountCoordinates)
{
	bool finded = false;
	int num = -1;
	COORD check = position;
	check.X--;
	for (int i = 0; i < 8; i++)
	{
		if (i<3)
		{
			check.X ++;
			if (Levels[current_level][check.Y][check.X] == 0) //�������� ������� ����� ����
			{
				if ((check.Y - 1) > 0) //�������� ���� �����
				{
					potential�oordinates[++num].X = check.X - position.X;
					potential�oordinates[num].Y = check.Y - position.Y;
					finded = true;
				}
			}
		}
		if ((i > 2) && (i < 5))
		{
			check.Y++;
			if (Levels[current_level][check.Y][check.X] == 0)//�������� ������ ����� ����
			{
				if ((check.X + 1) < SizeLevels[current_level]) //�������� ���� �����
				{
					potential�oordinates[++num].X = check.X - position.X;
					potential�oordinates[num].Y = check.Y - position.Y;
					finded = true;
				}
			}
		}
		if ((i > 4) && (i < 7))
		{
			check.X--;
			if (Levels[current_level][check.Y][check.X] == 0)//�������� ������ ����� ����
			{
				if ((check.Y + 1) < SizeLevels[current_level]) //�������� ���� �����
				{
					potential�oordinates[++num].X = check.X - position.X;
					potential�oordinates[num].Y = check.Y - position.Y;
					finded = true;
				}
			}
		}
		if (i == 7)
		{
			check.Y--;
			if (Levels[current_level][check.Y][check.X] == 0)//������� ����� ����� ����
			{
				if ((check.X - 1) > 0) //�������� ���� �����
				{
					potential�oordinates[++num].X = check.X - position.X;
					potential�oordinates[num].Y = check.Y - position.Y;
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
	COORD potential�oordinates[8] = {};
	int amountCoordinates = 0;
	int amountFindPattern = 0;
	int numOfPattern[AmmounOfPattern * 8] = {};
	int numOfLastPattern = 0;

	int buf = rand() % 2; //����� ������� ����� (2 ����������� �� 3)TODO
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

	for (i = 0; i < SizeLevels[current_level]; i++) //���������� ������ ���������
	{
		for (j = 0; j < SizeLevels[current_level]; j++)
		{
			Levels[current_level][i][j] = 1;
		}
	}

	numOfLastPattern = 4 + rand() % 2;
	DrawPattern(positionOFDraw, numOfLastPattern);//��������� �������

	pozPL = finish = start = { 2,2 };

	srand(time(NULL));
	for(i = 0; i<SizeLevels[current_level];i++)
	{

		if (check_an_opportunity_to_draw_a_pattern(positionOFDraw, potential�oordinates, &amountCoordinates))
		{
				if (findPattern(potential�oordinates, amountCoordinates, numOfPattern, &amountFindPattern, direct))
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

//���������� � ���������� tr �� ������� � �������� ��� ��������� �� ������
void Set_Poz(COORD poz)
{
	int zume = 20; //������� 
	int smesh = 100; //�������� 
	tr = poz;
	tr.X = smesh + (tr.X*zume);
	tr.Y = smesh + (tr.Y*zume);
}
//true ���� � ��������� ����������� �� ������� ������(pozPL)
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
//���������� ��� ����� 
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
			if(Levels[current_level][j][i] == 1)
			{
				Set_Poz(ij);
				rect(tr, rasmer, gan, zal);
			}
		}
	}
}
//���������� 1 ���� ��������� �����
// � 0 ���� ��� �� �����
bool butgame(char key) 
{
	RGB emp = { 255,255,255 }; // ������ ���� ��� �������� "�����" �� ������
	direction direct = Up; // ��� ����������� 1-����� 2-���� 3-���� 4- ����� 
	Set_Poz(pozPL);
	elps(tr, razPL, gan, zal);
	switch (key)
	{
	case 38: //up
		direct = Up;
		if (proverka(direct))
		{
			Set_Poz(pozPL);
			tr.Y -= Zume;
			rect(tr, rasmer, gan, zal);
		}
		else
		{
			Set_Poz(pozPL);
			rect(tr, rasmer, emp, emp);
			pozPL.Y--;
			Set_Poz(pozPL);
			elps(tr, razPL, gan, zal);
		}
		break;
	case 40: //down
		direct = Down;
		if (proverka(direct))
		{
			Set_Poz(pozPL);
			tr.Y += Zume;
			rect(tr, rasmer, gan, zal);
		}
		else
		{
			Set_Poz(pozPL);
			rect(tr, rasmer, emp, emp);
			pozPL.Y++;
			Set_Poz(pozPL);
			elps(tr, razPL, gan, zal);
		}
		break;
	case 37: //left
		direct = Left;
		if (proverka(direct))
		{
			Set_Poz(pozPL);
			tr.X -= Zume;
			rect(tr, rasmer, gan, zal);
		}
		else
		{
			Set_Poz(pozPL);
			rect(tr, rasmer, emp, emp);
			pozPL.X--;
			Set_Poz(pozPL);
			elps(tr, razPL, gan, zal);
		}
		break;
	case 39: //right;
		direct = Right;
		if (proverka(direct))
		{
			Set_Poz(pozPL);
			tr.X += Zume;
			rect(tr, rasmer, gan, zal);
		}
		else
		{
			Set_Poz(pozPL);
			rect(tr, rasmer, emp, emp);
			pozPL.X++;
			Set_Poz(pozPL);
			elps(tr, razPL, gan, zal);
		}
		break;
	case 13: DrawAllCurrentLevel(); break; //enter
	}
	if (sravn(pozPL, finish)) // �������� �� �������������
	{
		if(MessageBoxW(hwnd, TEXT("YOU WIN\n�� ����� � ����, CANCEL - ��������"), TEXT("YOU WIN"), MB_OKCANCEL)==1) // ���� ������ �� ����� � ����, CANCEL - ��������
			return 1;
	}
	return 0;
}

void DrowGame()
{
	/*std::thread BackGround(PlayMusic, BackGroungMusicGame);
	BackGround.detach();*/
	RGB fin = { 139, 0, 255 }, strt = { 211, 31, 94 }; //����� ������� ��� ������ � ������
	Set_Poz(start);
	rect(tr, rasmer, gan, strt);
	Set_Poz(finish);
	rect(tr, rasmer, gan, fin);
	Set_Poz(pozPL);
	elps(tr, razPL, gan, zal);
}
/*������� ��������� ����, �� ���-�� punkt, enable - ���������� ����� ����, 
gran - ������� �������, zalivka - ������� ������, 
����� ������� char ��� ���������� ������� ����*/
void menu(int punkt, int enable, RGB gran, RGB zalivka, ...)
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
	va_start(argp, zalivka);
	for (i = 1; i <= punkt; i++)
	{
		if (i == enable) rect(Pbottun, EnableGran, EnableZalivka);
		else rect(Pbottun, gran, zalivka);
		p = va_arg(argp, char*);
		DrawTextA(hdc, p, -1, &text, DT_CENTER);
		Pbottun.top = text.top = text.bottom = Pbottun.bottom + 20;
		text.top += 15;
		Pbottun.bottom = text.bottom += 50;
		DrawTextA(hdc, "\0", -1, &text, DT_CENTER);
	}
	va_end(argp);
}
//����� ��������� ����
void DrawMenu(int poz)
{
	switch (NumOfMenu)
	{
	case 1: menu(PunctOf1Menu, poz, gan, zal, "������", "�� ����", "����� ���������", "�������", "�����"); break;
	case 2: menu(PunctOf2Menu, poz, gan, zal, "�����", "���������", "������", "�����"); break;
	default:
		break;
	}
}
//��������� ������� ����
void nummenu(int *poz)
{
	std::thread Music(PlayMusic, ChoicePointOfMenu);
	Music.detach();
	switch (NumOfMenu)
	{
	case 1: //������� ����
	{
		switch (*poz) //������� �� ����� ������� � ���� ����
		{
		case 1:*poz = 1; DrowGame(); ClearWindow(); status = Game; GenerateLocation(); break; //��������� �� ��� ���� �� ������ ������ � ��� �����
		case 2:*poz = 1; WriteTextSymbolBySymbol({ 0,65,350,0 }, "������!*� ������ �� ��������������� �����?*�������)*� ���� � �� ����� ����� �� �������, ���� �� ��������� ����� ������ � WinApi*�� ��� �� ����� � ����� �� ���������� �������� 5 ������� � ������ ����� ���� �������� XD*� ����� ������ �� ������� ������� ������������ (bog5651)*�������� 10.11.2017*�����!"); break;
		case 3:*poz = 1; NumOfMenu = 2; break;
		case 4:*poz = 1; break;
		case 5:*poz = 1; exit(NULL); break;//����� �� ���������(������)
		default: *poz = 1; break;
		}
	} break;
	case 2: //���� ������ ���������
	{
		switch (*poz) //������� �� ����� ������� � ���� ����
		{
		case 1:*poz = 1; current_level = 0; GenerateLocation(); break; //��������� �� ��� ���� �� ������ ������ � ��� �����
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
//��������� ������ � ���� � ��������� ������ ����
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
	case 37: break; //left wtf ����� � �� ���� �������? TODO ���������� �� ��� ���-����
	case 39: break; //right;
	case 13: nummenu(&*poz); break; //enter
	}
}

LRESULT WINAPI mainProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	//��������� ���������.
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