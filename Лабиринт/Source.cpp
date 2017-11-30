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
enum direction { Up, Down, Left, Right }; // �����������
enum Status { Menu, Game, Pause }; //������ ���������
enum Music { Step, BackGroungMusicMenu, BackGroungMusicGame, ChoicePointOfMenu }; //������������ ��������� ������
Status status = Menu;

//---------������---------//
short int current_level = 0; // ���������� �������� ����� �������� ������
int SizeLevels[100] = {}; // ������� �������� ������� �������
const int SizeSmall = 14, SizeMedium = 26, SizeBig = 38; //����������� ������ (3*3*n)+2, ��� n - ����� �����, ���������� ���-�� ���������
short int Levels[100][SizeBig][SizeBig]; //���������� ������� �������� ��� ������
short int Pattern[11][3][3] = //����� ��������� ��� ��������� �������
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

//���������� ����������� ������� �����
int SizeOfMap()
{
	return SizeLevels[current_level];
}

void writeInMap(COORD r, int Level, int value)
{
	Levels[Level][r.Y][r.X] = value;
}
//���������� ���������� ������ ��������� � x,y
int requestFromMap(int x, int y)
{
	return Levels[current_level][y][x];
}
//���������� ���������� ������ ��������� � x,y
int requestFromMap(COORD Pozition)
{
	return Levels[current_level][Pozition.Y][Pozition.X];
	/*switch (mode)
	{
	case 1: return labA[zapros.Y][zapros.X]; break;
	case 2: return labB[zapros.Y][zapros.X]; break;
	case 3: return labC[zapros.Y][zapros.X]; break;
	}*/
}
//���������� 1 ���� ���� poz ������� ����� � ������ �����, �� ����������� kyda
//� 0 ���� ���� ���� ���-�� ������
bool nearbyFeel(COORD poz, direction direct)
{
	switch (direct)
	{
	case Up:
	{
		if (requestFromMap(poz.X + 1, poz.Y) == 1)
		{
			if (requestFromMap(poz.X - 1, poz.Y) == 1)
			{
				if (requestFromMap(poz.X, poz.Y - 1) == 1)
				{
					return true;
				}
				else return false;
			}
			else return false;
		}
		else return false;
	}
	break;
	case Down:
	{
		if (requestFromMap(poz.X + 1, poz.Y) == 1)
		{
			if (requestFromMap(poz.X, poz.Y - 1) == 1)
			{
				if (requestFromMap(poz.X, poz.Y + 1) == 1)
				{
					return true;
				}
				else return false;
			}
			else return false;
		}
		else return false;
	} break;
	case Left:
	{
		if (requestFromMap(poz.X + 1, poz.Y) == 1)
		{
			if (requestFromMap(poz.X - 1, poz.Y) == 1)
			{
				if (requestFromMap(poz.X, poz.Y + 1) == 1)
				{
					return true;
				}
				else return false;
			}
			else return false;
		}
		else return false;
	}break;
	case Right:
	{
		if (requestFromMap(poz.X - 1, poz.Y) == 1)
		{
			if (requestFromMap(poz.X, poz.Y - 1) == 1)
			{
				if (requestFromMap(poz.X, poz.Y + 1) == 1)
				{
					return true;
				}
				else return false;
			}
			else return false;
		}
		else return false;
	}break;
	}
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
//��������� ������� ����� � ����� �� "���� �����"
bool krayKarty(COORD check)
{
	if ((check.X == SizeOfMap() - 1) | (check.X == 1))
		return 1;
	if ((check.Y == SizeOfMap() - 1) | (check.Y == 1))
		return 1;
	return 0;
}

//permitCollizionCoridors �������� 1 ���� ����� ���� �������� ������������ ��������, �� �����?
COORD drawCoridor(COORD At, bool permitCollizionCoridors)
{
	int i = 0, j = 0;
	int maxtry = permitCollizionCoridors ? 50 : 100; //������������ ���-�� ������� ���������� ��������
	int tru = 0; //������� ������� ���� ����������
	direction direct; // ������ ����������t
	int buf;
	bool brk = false, ex = false;
	COORD put, cbuf, End;//put ����. cbuf ����� ��� �������� ����. End ������������ ���������� ����� ��������
	End.X = 0;
	End.Y = 0;
	put = At;
	brk = false;
	writeInMap(At, current_level, 0);
	do
	{
		buf = rand() % 4;
		switch (buf)
		{
		case 0:
			direct = Up;
			if (put.Y - 2 == 1)
			{
				tru++;
				break;
			}
			else
			{
				cbuf = put;
				cbuf.Y--;
				/*if (krayKarty(cbuf))
				{
					tru++;
					break;
				}*/
				if ((nearbyFeel(cbuf, direct))&(!permitCollizionCoridors))
				{
					put.Y--;
					writeInMap(put, current_level, 0);
					tru = 0;
				}
				else tru++;
				if (permitCollizionCoridors)
				{
					put.Y--;
					writeInMap(put, current_level, 0);
					tru++;
				}
			}
			break;
		case 1:
			{
				direct = Down;
				if (put.Y + 1 == SizeOfMap() - 2)
				{
					tru++;
					break;
				}
				else
				{
					cbuf = put;
					cbuf.Y++;
					/*if (krayKarty(cbuf))
					{
						tru++;
						break;
					}*/
					if ((nearbyFeel(cbuf, direct))&(!permitCollizionCoridors))
					{
						put.Y++;
						writeInMap(put, current_level, 0);
						tru = 0;
					}
					else tru++;
					if (permitCollizionCoridors)
					{
						put.Y++;
						writeInMap(put, current_level, 0);
						tru++;
					}
				}
			}break;
		case 2:
			{
				direct = Left;
				if (put.X - 2 == 1)
				{
					tru++;
					break;
				}
				else
				{
					cbuf = put;
					cbuf.X--;
					/*if (krayKarty(cbuf))
					{
						tru++;
						break;
					}*/
					if ((nearbyFeel(cbuf, direct))&(!permitCollizionCoridors))
					{
						put.X--;
						writeInMap(put, current_level, 0);
						tru = 0;
					}
					else tru++;
					if (permitCollizionCoridors)
					{
						put.X--;
						writeInMap(put, current_level, 0);
						tru++;
					}
				}
			}break;
		case 3:
			{
				direct = Right;
				if (put.X + 1 == SizeOfMap() - 2)
				{
					tru++;
					break;
				}
				else
				{
					cbuf = put;
					cbuf.X++;
					/*if (krayKarty(cbuf))
					{
						tru++;
						break;
					}*/
					if ((nearbyFeel(cbuf, direct))&(!permitCollizionCoridors))
					{
						put.X++;
						writeInMap(put, current_level, 0);
						tru = 0;
					}
					else tru++;
					if (permitCollizionCoridors)
					{
						put.X++;
						writeInMap(put, current_level, 0);
						tru++;
					}
				}
			}break;
		default:
			break;
		}
		if (maxtry <= tru)
		{
			End = put;
		}
	} while (!sravn(put, End));
	return End;
}

void DrawPattern(COORD pozition, int NumberOfPattern)
{
	int i, j, PatternI, PatternJ;
	for (i = pozition.Y, PatternI = 0; i < pozition.Y + 3; i++, PatternI++)
	{
		for (j = pozition.X, PatternJ = 0; j < pozition.Y + 3; j++, PatternJ++)
		{
			Levels[current_level][i][j] = Pattern[NumberOfPattern][PatternI][PatternJ];
		}
	}
}

void GenerateLocation()
{
	int i, j, k;
	direction direct;
	COORD positionOFDraw = { 1,1 };
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
	for (i = 0; i < SizeOfMap(); i++) //���������� ������ ���������
	{
		for (j = 0; j < SizeOfMap(); j++)
		{
			Levels[current_level][i][j] = 1;
		}
	}

	int PotentialPattern = 4 + rand() % 2;
	DrawPattern(positionOFDraw, PotentialPattern);//��������� �������
	start = { 2,2 };
	pozPL = start;
	do
	{
		if (requestFromMap(positionOFDraw.X + 2, positionOFDraw.Y + 1) == 0) //�������� ����������� �������� ������ �� ��������
		{

		}
		if (requestFromMap(positionOFDraw.X + 2, positionOFDraw.Y + 1) == 0) //�������� ����������� �������� ������ �� ��������
		{

		}
		if (requestFromMap(positionOFDraw.X + 2, positionOFDraw.Y + 1) == 0) //�������� ����������� �������� ������ �� ��������
		{

		}
		if (requestFromMap(positionOFDraw.X + 2, positionOFDraw.Y + 1) == 0) //�������� ����������� �������� ������ �� ��������
		{

		}
	} while (false);
}

//���������� ����� �� ��������
void GenerateMap()
{
	int i, j;
	int buf;
	bool brk = false, ex = false;
	COORD tempForCoridor;
	srand(time(NULL));
	buf = rand() % 2; //����� ������� ����� (����������� �� 3)TODO
	switch (buf)
	{
	case 0:
		{
			SizeLevels[current_level] = SizeSmall;
		}break;
	case 1:
		{
			SizeLevels[current_level] = SizeMedium;
		}break;
	case 2:
		{
			SizeLevels[current_level] = SizeBig;
		}break;
	default:
		break;
	}
	for (i = 0; i < SizeOfMap(); i++)
		{
			for (j = 0; j < SizeOfMap(); j++)
			{
				Levels[current_level][i][j] = 1;
			}
		}
	brk = true;
	//����� ����� ������ � ������
	buf = rand() % 4; //����� ����� ������
	switch (buf)
	{
	case 0: start.Y = 1; start.X = 2 + rand() % (SizeOfMap() - 2); break;
	case 1: start.X = (SizeOfMap() - 2); start.Y = 1 + rand() % (SizeOfMap() - 2); break;
	case 2: start.Y = (SizeOfMap() - 2); start.X = 1 + rand() % (SizeOfMap() - 2); break;
	case 3: start.X = 1; start.Y = 2 + rand() % (SizeOfMap() - 2); break;
	}
	pozPL = start;
	finish = drawCoridor(start,0);
	for (i = 0; i < SizeOfMap() / 1.5; i++)
	{
		buf = rand() % 4; //����� ����� ������ ��� ��������� ��������� ���������
		switch (buf)
		{
		case 0: tempForCoridor.Y = 2; tempForCoridor.X = 2 + rand() % (SizeOfMap() - 2); break;
		case 1: tempForCoridor.X = (SizeOfMap() - 2); tempForCoridor.Y = 2 + rand() % (SizeOfMap() - 2); break;
		case 2: tempForCoridor.Y = (SizeOfMap() - 2); tempForCoridor.X = 2 + rand() % (SizeOfMap() - 2); break;
		case 3: tempForCoridor.X = 2; tempForCoridor.Y = 2 + rand() % (SizeOfMap() - 2); break;
		}
		if (requestFromMap(tempForCoridor) != 0)
		{
			drawCoridor(tempForCoridor, 0);
		}
	}
	/*for (i = 0; i<SizeOfMap()/3 ; i++)
		for (j = 0; j< SizeOfMap()/3; j++)
		{
			put.X = 2 + rand() % (SizeOfMap() - 2);
			put.Y = 2 + rand() % (SizeOfMap() - 2);
			writeInMap(put, 0);
		}*/
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
	for (i = 0; i < SizeOfMap(); i++)
	{
		for (j = 0; j < SizeOfMap(); j++)
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
		case 1:*poz = 1; current_level = 0; GenerateMap(); break; //��������� �� ��� ���� �� ������ ������ � ��� �����
		case 2:*poz = 1; current_level = 1; GenerateMap(); break;
		case 3:*poz = 1; current_level = 2; GenerateMap(); break;
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