#include <Windows.h>
#include <stdio.h>
#include <conio.h>
#include <locale.h>
#include <time.h>
HWND hwnd;
HDC hdc;
HANDLE hStdOut = FindWindowW(WC_DIALOG, TEXT("lab")); //�������� handle
HPEN hpen1; //��������� ������ ����
HGDIOBJ hpenOld, hbrushOld;
HBRUSH hbrush; //��������� �����
int PunctOf1Menu = 5, PunctOf2Menu = 4, PunctOf3Menu = 0, PunctOf4Menu = 0; // ���-�� ������� � ���� � �������, ��� ������������� ���������
int poz = 1; // ������� � ����
int NumOfMenu = 1; // ��������� �������� ����
const int izi = 10, normal = 25, hard = 40;
int labA[izi][izi] = { 1,0,0,0,1,
					   1,0,1,0,1,
					   1,1,1,0,0,
					   1,0,0,0,1,
					   1,0,1,1,1
					}; // �������� � �������� �������
int labB[normal][normal] = {};
int labC[hard][hard] = {};
int mode = 2; // ���������
int Zume = 20; // ������
int rasmer = 9; // ������ ����
int razPL = 5; //������ ����� ������
COORD start, finish; //���������� ������, ������
COORD pozPL = {}; //���������� ������� ������
COORD tr; //���������� �� ������. ���������� ��� �������� ����� � ������

struct RGB
{
	int r, g, b;
};
struct RGB gan = { 255, 0, 0 }, zal = { 255,255,0 }; // ����������� gan - �������, zal - ������(�� ���� ������� � �������)

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
//��
void about()
{
	int i = 0, time=0;
	char About[] = { "������!*� ������ �� ��������������� �����?*�������)*� ���� � �� ����� ����� �� �������, ���� �� ��������� ����� ������ � WinApi*�� ��� �� ����� � ����� �� ���������� �������� 5 ������� � ������ ����� ���� �������� XD*� ����� ������ �� ������� ������� ������������ (bog5651)*�������� 10.11.2017*�����!" };
	for (i; i < sizeof(About); i++)
	{
		if (kbhit())
		{
			break;
		}
		if (About[i] == ',')
		{
			Sleep(200);
		}
		if (About[i] == '*')
		{
			Sleep(time*50);
			system("cls");
			time = 0;
			i++;
		}
		time++;
		printf("%c", About[i]);
		Sleep(50);
	}
	Sleep(200);
}
//���������� ����������� ������� �����
int SizeOfMap()
{
	switch (mode)
	{
	case 1: return izi; break;
	case 2: return normal; break;
	case 3: return hard; break;
	}
}

void writeInMap(COORD r, int i)
{
	switch (mode)
	{
	case 1: labA[r.Y][r.X] = i; break;
	case 2: labB[r.Y][r.X] = i; break;
	case 3: labC[r.Y][r.X] = i; break;
	}
}
//���������� ���������� ������ ��������� � x,y
int zaprosFromMap(int x, int y)
{
	switch (mode)
	{
	case 1: return labA[y][x]; break;
	case 2: return labB[y][x]; break;
	case 3: return labC[y][x]; break;
	}
}
//���������� ���������� ������ ��������� � x,y
int zaprosFromMap(COORD zapros)
{
	switch (mode)
	{
	case 1: return labA[zapros.Y][zapros.X]; break;
	case 2: return labB[zapros.Y][zapros.X]; break;
	case 3: return labC[zapros.Y][zapros.X]; break;
	}
}
//���������� 1 ���� ���� poz ������� ����� � ������ �����, �� ����������� kyda
//1-����� 2-���� 3-���� 4- ����� 
//� 0 ���� ���� ���� ���-�� ������
bool nearbyFeel(COORD poz, int kyda)
{
	switch (kyda)
	{
	case 1:
	{
		if (zaprosFromMap(poz.X + 1, poz.Y) == 1)
		{
			if (zaprosFromMap(poz.X - 1, poz.Y) == 1)
			{
				if (zaprosFromMap(poz.X, poz.Y - 1) == 1)
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
	case 2:
	{
		if (zaprosFromMap(poz.X + 1, poz.Y) == 1)
		{
			if (zaprosFromMap(poz.X, poz.Y - 1) == 1)
			{
				if (zaprosFromMap(poz.X, poz.Y + 1) == 1)
				{
					return true;
				}
				else return false;
			}
			else return false;
		}
		else return false;
	} break;
	case 3:
	{
		if (zaprosFromMap(poz.X + 1, poz.Y) == 1)
		{
			if (zaprosFromMap(poz.X - 1, poz.Y) == 1)
			{
				if (zaprosFromMap(poz.X, poz.Y + 1) == 1)
				{
					return true;
				}
				else return false;
			}
			else return false;
		}
		else return false;
	}break;
	case 4:
	{
		if (zaprosFromMap(poz.X - 1, poz.Y) == 1)
		{
			if (zaprosFromMap(poz.X, poz.Y - 1) == 1)
			{
				if (zaprosFromMap(poz.X, poz.Y + 1) == 1)
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
	int kyda = 0; // ������ ��� ����������� 0-3
	int buf;
	bool brk = false, ex = false;
	COORD put, cbuf, End;//put ����. cbuf ����� ��� �������� ����. End ������������ ���������� ����� ��������
	End.X = 0;
	End.Y = 0;
	pozPL = start;
	put = At;
	brk = false;
	writeInMap(At, 0);
	do
	{
		buf = rand() % 4;
		switch (buf)
		{
		case 0: // �������� � ����
		{
			kyda = 1;
			if (put.Y - 1 == 1)
			{
				tru++;
				break;
			}
			else
			{
				cbuf = put;
				cbuf.Y--;
				if (krayKarty(cbuf))
				{
					tru++;
					break;
				}
				if ((nearbyFeel(cbuf, kyda))&(!permitCollizionCoridors))
				{
					put.Y--;
					writeInMap(put, 0);
					tru = 0;
				}
				else tru++;
				if (permitCollizionCoridors)
				{
					put.Y--;
					writeInMap(put, 0);
					tru++;
				}
			}
		}break;
		case 1: //������
		{
			kyda = 2;
			if (put.X + 1 == SizeOfMap() - 2)
			{
				tru++;
				break;
			}
			else
			{
				cbuf = put;
				cbuf.X++;
				if (krayKarty(cbuf))
				{
					tru++;
					break;
				}
				if ((nearbyFeel(cbuf, kyda))&(!permitCollizionCoridors))
				{
					put.X++;
					writeInMap(put, 0);
					tru = 0;
				}
				else tru++;
				if (permitCollizionCoridors)
				{
					put.X++;
					writeInMap(put, 0);
					tru ++;
				}
			}
		}break;
		case 2: //����
		{
			kyda = 3;
			if (put.Y + 1 == SizeOfMap() - 2)
			{
				tru++;
				break;
			}
			else
			{
				cbuf = put;
				cbuf.Y++;
				if (krayKarty(cbuf))
				{
					tru++;
					break;
				}
				if ((nearbyFeel(cbuf, kyda))&(!permitCollizionCoridors))
				{
					put.Y++;
					writeInMap(put, 0);
					tru = 0;
				}
				else tru++;
				if (permitCollizionCoridors)
				{
					put.Y++;
					writeInMap(put, 0);
					tru ++;
				}
			}
		}break;
		case 3: //�����
		{
			kyda = 4;
			if (put.X - 1 == 1)
			{
				tru++;
				break;
			}
			else
			{
				cbuf = put;
				cbuf.X--;
				if (krayKarty(cbuf))
				{
					tru++;
					break;
				}
				if ((nearbyFeel(cbuf, kyda))&(!permitCollizionCoridors))
				{
					put.X--;
					writeInMap(put, 0);
					tru = 0;
				}
				else tru++;
				if (permitCollizionCoridors)
				{
					put.X--;
					writeInMap(put, 0);
					tru ++;
				}
			}
		}break;
		}
		if (maxtry <= tru)
		{
			End = put;
		}
	} while (!sravn(put, End));
	return End;
}
//���������� ����� ���������
void feelMap()
{
	int i, j;
	int buf;
	bool brk = false, ex = false;
	COORD tempForCoridor;
	srand(time(NULL));
	// ���������� ������� ������� (���������)
	for (i = 0; i < hard; i++)
	{
		for (j = 0; !brk; j++)
		{
			switch (mode)
			{
			case 1:
			{
				if ((i >= izi) || (j >= izi)) brk = true;
				else labA[i][j] = 1;
			} break;
			case 2:
			{
				if ((i >= normal) || (j >= normal)) brk = true;
				else labB[i][j] = 1;
			} break;
			case 3:
			{
				if ((i >= hard) || (j >= hard)) brk = true;
				else labC[i][j] = 1;
			} break;
			}
		}
		brk = false;
	}
	brk = true;
	//����� ����� ������ � ������
	buf = rand() % 4; //����� ����� ������
	switch (buf)
	{
	case 0: start.Y = 1; start.X = 1 + rand() % (SizeOfMap() - 2); break;
	case 1: start.X = (SizeOfMap() - 2); start.Y = 1 + rand() % (SizeOfMap() - 2); break;
	case 2: start.Y = (SizeOfMap() - 2); start.X = 1 + rand() % (SizeOfMap() - 2); break;
	case 3: start.X = 1; start.Y = 1 + rand() % (SizeOfMap() - 2); break;
	}
	finish = drawCoridor(start,0);
	for (i = 0; i < SizeOfMap() / 1.5; i++)
	{
		buf = rand() % 4; //����� ����� ������
		switch (buf)
		{
		case 0: tempForCoridor.Y = 2; tempForCoridor.X = 2 + rand() % (SizeOfMap() - 2); break;
		case 1: tempForCoridor.X = (SizeOfMap() - 2); tempForCoridor.Y = 2 + rand() % (SizeOfMap() - 2); break;
		case 2: tempForCoridor.Y = (SizeOfMap() - 2); tempForCoridor.X = 2 + rand() % (SizeOfMap() - 2); break;
		case 3: tempForCoridor.X = 2; tempForCoridor.Y = 2 + rand() % (SizeOfMap() - 2); break;
		}
		if (zaprosFromMap(tempForCoridor) != 0)
		{
			drawCoridor(tempForCoridor, 0);
		}
	}
	printf("%d", i);
	/*for (i = 0; i<SizeOfMap()/3 ; i++)
		for (j = 0; j< SizeOfMap()/3; j++)
		{
			put.X = 2 + rand() % (SizeOfMap() - 2);
			put.Y = 2 + rand() % (SizeOfMap() - 2);
			writeInMap(put, 0);
		}*/

	labA[start.Y][start.X];
	labA[finish.Y][finish.X];
	labB[start.Y][start.X];
	labB[finish.Y][finish.X];
	labC[start.Y][start.X];
	labC[finish.Y][finish.X];
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
//1-����� 2-���� 3-���� 4- ����� 
bool proverka(int kyda)
{
	switch (mode)
	{
	case 1:
	{
		switch (kyda)
		{
		case 1:
			if (labA[pozPL.Y - 1][pozPL.X] == 1)
			{
				return true;
			}
			else return false;
			break;
		case 2:
			if ((labA[pozPL.Y + 1][pozPL.X] == 1))
			{
				return true;
			}
			else return false;
			break;
		case 3:
			if (labA[pozPL.Y][pozPL.X - 1] == 1)
			{
				return true;
			}
			else return false;
			break;
		case 4:
			if (labA[pozPL.Y][pozPL.X + 1] == 1)
			{
				return true;
			}
			else return false;
			break;
		}
	}
	break;
	case 2: 
	{
		switch (kyda)
		{
		case 1:
			if (labB[pozPL.Y - 1][pozPL.X] == 1)
			{
				return true;
			}
			else return false;
			break;
		case 2:
			if ((labB[pozPL.Y + 1][pozPL.X] == 1))
			{
				return true;
			}
			else return false;
			break;
		case 3:
			if (labB[pozPL.Y][pozPL.X - 1] == 1)
			{
				return true;
			}
			else return false;
			break;
		case 4:
			if (labB[pozPL.Y][pozPL.X + 1] == 1)
			{
				return true;
			}
			else return false;
			break;
		}
	} break;
	case 3: 
	{
		switch (kyda)
		{
		case 1:
			if (labC[pozPL.Y - 1][pozPL.X] == 1)
			{
				return true;
			}
			else return false;
			break;
		case 2:
			if ((labC[pozPL.Y + 1][pozPL.X] == 1))
			{
				return true;
			}
			else return false;
			break;
		case 3:
			if (labC[pozPL.Y][pozPL.X - 1] == 1)
			{
				return true;
			}
			else return false;
			break;
		case 4:
			if (labC[pozPL.Y][pozPL.X + 1] == 1)
			{
				return true;
			}
			else return false;
			break;
		}
	} break;
	}
}
//���������� ��� ����� 
void drawmap()
{
	int i, j;
	COORD ij;
	for (i = 0; i < SizeOfMap(); i++)
	{
		for (j = 0; j < SizeOfMap(); j++)
		{
			ij.X = i;
			ij.Y = j;
			switch (mode)
			{
			case 1:
			{
				if(labA[j][i] == 1)
				{
					Set_Poz(ij);
					rect(tr, rasmer, gan, zal);
				}
			}break;
			case 2: 
			{
				if (labB[j][i] == 1)
				{
					Set_Poz(ij);
					rect(tr, rasmer, gan, zal);
				}
			}break;
			case 3: 
			{
				if (labC[j][i] == 1)
				{
					Set_Poz(ij);
					rect(tr, rasmer, gan, zal);
				}
			}break;
			}
		}
	}
}
//���������� 1 ���� ��������� �����
// � 0 ���� ��� �� �����
bool butgame(char key) 
{
	RGB emp = { 0,0,0 }; // ������ ���� ��� �������� "�����" �� ������
	int kyda = 0; // ��� ����������� 1-����� 2-���� 3-���� 4- ����� 
	Set_Poz(pozPL);
	elps(tr, razPL, gan, zal);
	switch (key)
	{
	case 72: //up
		kyda = 1;
		if (proverka(kyda))
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
	case 80: //down
		kyda = 2;
		if (proverka(kyda))
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
	case 75: //left
		kyda = 3;
		if (proverka(kyda))
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
	case 77: //right;
		kyda = 4;
		if (proverka(kyda))
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
	case 13: drawmap(); break; //enter
	}
	if (sravn(pozPL, finish))
	{
		printf("yw");
		return 1;
	}
	return 0;
}

void DrowGame()
{
	RGB fin = { 139, 0, 255 }, strt = { 211, 31, 94 }; //����� ������� ��� ������ � ������
	char key = 0;
	feelMap();
	Set_Poz(pozPL);
	elps(tr, razPL, gan, zal);
	while (true)
	{
		if (kbhit())
		{
			key = getch();
			if (key == -32) // ��������� ������������� �������, ���. ������� ���� 2 ���� �����, ������ - ��� ��� �������, ������ - ��� ������� �������
			{
				key = getch();
			}
			Set_Poz(start);
			rect(tr, rasmer, gan, strt);
			Set_Poz(finish);
			rect(tr, rasmer, gan, fin);
			if (butgame(key))
				break;
		}
		Sleep(50);
	}
}
/*������� ��������� ����, �� ���-�� punkt, enable - ���������� ����� ����, 
gran - ������� �������, zalivka - ������� ������, 
����� ������� char ��� ���������� ������� ����*/
void menu(int punkt, int enable, RGB gran, RGB zalivka, ...)
{
	RECT r;
	GetClientRect(hwnd, &r);
	FillRect(hdc, &r, (HBRUSH)(COLOR_WINDOW + 1));
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
//��������� ����
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
	switch (NumOfMenu)
	{
	case 1: //������� ����
	{
		switch (*poz) //������� �� ����� ������� � ���� ����
		{
		case 1:*poz = 1; DrowGame(); break; //��������� �� ��� ���� �� ������ ������ � ��� �����
		case 2:*poz = 1; about(); break;
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
		case 1:*poz = 1; mode = 1; break; //��������� �� ��� ���� �� ������ ������ � ��� �����
		case 2:*poz = 1; mode = 2; break;
		case 3:*poz = 1; mode = 3; break;
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
		DrawMenu(poz);
	} 
	break;
	case WM_KEYUP:
	{
		char key[] = { (TCHAR)wParam,'\0'};
		switch (NumOfMenu)
		{
		case 1: butMenu(key[0], PunctOf1Menu, &poz); break;
		case 2: butMenu(key[0], PunctOf2Menu, &poz); break;
		case 3: butMenu(key[0], PunctOf3Menu, &poz); break;
		case 4: butMenu(key[0], PunctOf4Menu, &poz); break;
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
	hwnd = CreateWindowExW(0, WC_DIALOG, TEXT("lab"), WS_VISIBLE | WS_SYSMENU, 500, 500, 900, 500, 0, 0, 0, 0);
	hdc = GetDC(hwnd);
	SetWindowLong(hwnd, DWL_DLGPROC, (long)mainProc);
	while (GetMessage(&msg, 0, 0, 0) != 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;
}