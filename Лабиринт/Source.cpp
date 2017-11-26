#include <Windows.h>
#include <stdio.h>
#include <conio.h>
#include <locale.h>
#include <time.h>
HWND hwnd;
HDC hdc;
HANDLE hStdOut = FindWindowW(WC_DIALOG, TEXT("lab")); //получаем handle
HPEN hpen1; //объявляем объект перо
HGDIOBJ hpenOld, hbrushOld;
HBRUSH hbrush; //объявляем кисть
int PunctOf1Menu = 5, PunctOf2Menu = 4, PunctOf3Menu = 0, PunctOf4Menu = 0; // кол-во пунктов в меню и подменю, при необходимости дополнить
int poz = 1; // позиция в меню
int NumOfMenu = 1; // начальная страница меню
enum Status {Menu, Game, Pause};
Status status = Menu;
const int izi = 10, normal = 25, hard = 40;
int labA[izi][izi] = { 1,0,0,0,1,
					   1,0,1,0,1,
					   1,1,1,0,0,
					   1,0,0,0,1,
					   1,0,1,1,1
					}; // заполнен в качесвте примера
int labB[normal][normal] = {};
int labC[hard][hard] = {};
int mode = 2; // сложность
int Zume = 20; // маштаб
int rasmer = 9; // размер стен
int razPL = 5; //размер круга игрока
COORD start, finish; //координата старта, финиша
COORD pozPL = {}; //координата позиции игрока
COORD tr; //координата на экране. переменная для отрисоки стены и игрока

struct RGB
{
	int r, g, b;
};
struct RGB gan = { 255, 0, 0 }, zal = { 255,255,0 }; // стандартные gan - красный, zal - желтый(от слов граница и заливка)


void ClearWindow()
{
	RECT r;
	GetClientRect(hwnd, &r);
	FillRect(hdc, &r, (HBRUSH)(COLOR_WINDOW + 1));
}
// прямоугольник по центру+радиус
void rect(COORD rect, int rad, RGB gran, RGB zalivka)
{
	hpen1 = CreatePen(PS_SOLID, 2, RGB(gran.r, gran.g, gran.b)); //логическое перо с заданным стилем, шириной и цветом
	hbrush = CreateSolidBrush(RGB(zalivka.r, zalivka.g, zalivka.b)); //логическая кисть с заданным стилем, шириной и цветом
	hbrushOld = (HBRUSH)SelectObject(hdc, hbrush);
	hpenOld = (HPEN)SelectObject(hdc, hpen1);
	Rectangle(hdc, rect.X + rad, rect.Y + rad, rect.X - rad, rect.Y - rad); //рисование прямоугольника
	DeleteObject(hpen1);       // Удаляем созданное перо
	DeleteObject(hbrush);       // Удаляем созданную кисть
}
//рисование прямоугольника по RECT
void rect(RECT h, RGB gran, RGB zalivka)
{
	hpen1 = CreatePen(PS_SOLID, 2, RGB(gran.r, gran.g, gran.b)); //логическое перо с заданным стилем, шириной и цветом
	hbrush = CreateSolidBrush(RGB(zalivka.r, zalivka.g, zalivka.b)); //логическая кисть с заданным стилем, шириной и цветом
	hbrushOld = (HBRUSH)SelectObject(hdc, hbrush);
	hpenOld = (HPEN)SelectObject(hdc, hpen1);
	Rectangle(hdc, h.left, h.top, h.right, h.bottom); //рисование прямоугольника
	DeleteObject(hpen1);       // Удаляем созданное перо
	DeleteObject(hbrush);       // Удаляем созданную кисть
}
//рисование многоугольника по точкам
void rect(COORD xy1, COORD xy2, COORD xy3, COORD xy4, RGB gran)
{
	hpen1 = CreatePen(PS_SOLID, 2, RGB(gran.r, gran.g, gran.b)); //логическое перо с заданным стилем, шириной и цветом
	MoveToEx(hdc, xy1.X, xy1.Y, NULL);
	LineTo(hdc, xy2.X, xy2.Y);
	LineTo(hdc, xy3.X, xy3.Y);
	LineTo(hdc, xy4.X, xy4.Y);
	LineTo(hdc, xy1.X, xy1.Y);
	DeleteObject(hpen1);       // Удаляем созданное перо
}
// круг по центру+радиус
void elps(COORD rect, int rad, RGB gran, RGB zalivka)
{
	hpen1 = CreatePen(PS_SOLID, 2, RGB(gran.r, gran.g, gran.b)); //логическое перо с заданным стилем, шириной и цветом
	hbrush = CreateSolidBrush(RGB(zalivka.r, zalivka.g, zalivka.b)); //логическая кисть с заданным стилем, шириной и цветом
	hbrushOld = (HBRUSH)SelectObject(hdc, hbrush);
	hpenOld = (HPEN)SelectObject(hdc, hpen1);
	Ellipse(hdc, rect.X + rad, rect.Y + rad, rect.X - rad, rect.Y - rad);
	DeleteObject(hpen1);       // Удаляем созданное перо
	DeleteObject(hbrush);       // Удаляем созданную кисть
}
//вывод текста не работает
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

//вывод текста посимвольно
void WriteTextSymbolBySymbol(RECT Rect,char Text[])
{
	int i = 0, time=0;
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
//возвращает размерность текущей карты
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
//возвращает содержимое ячейки лабиринта в x,y
int zaprosFromMap(int x, int y)
{
	switch (mode)
	{
	case 1: return labA[y][x]; break;
	case 2: return labB[y][x]; break;
	case 3: return labC[y][x]; break;
	}
}
//возвращает содержимое ячейки лабиринта в x,y
int zaprosFromMap(COORD zapros)
{
	switch (mode)
	{
	case 1: return labA[zapros.Y][zapros.X]; break;
	case 2: return labB[zapros.Y][zapros.X]; break;
	case 3: return labC[zapros.Y][zapros.X]; break;
	}
}
//возвращает 1 если перд poz клеткой слева и справа стены, по направлению kyda
//1-вверх 2-вниз 3-лево 4- право 
//и 0 если есть хоть где-то проход
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
//проверить текущую точку в карте на "край карты"
bool krayKarty(COORD check)
{
	if ((check.X == SizeOfMap() - 1) | (check.X == 1))
		return 1;
	if ((check.Y == SizeOfMap() - 1) | (check.Y == 1))
		return 1;
	return 0;
}

//permitCollizionCoridors передать 1 если хотим чтоб коридоры пересекались работает, но криво?
COORD drawCoridor(COORD At, bool permitCollizionCoridors)
{
	int i = 0, j = 0;
	int maxtry = permitCollizionCoridors ? 50 : 100; //максимальное кол-во попыток написовать лабиринт
	int tru = 0; //сколько попыток было примененно
	int kyda = 0; // храним код направления 0-3
	int buf;
	bool brk = false, ex = false;
	COORD put, cbuf, End;//put путь. cbuf буфер для пробного шага. End возвращаемая координата конца коридора
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
		case 0: // рисовать в верх
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
		case 1: //вправо
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
		case 2: //вниз
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
		case 3: //влево
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
//заполнение карты
void feelMap()
{
	int i, j;
	int buf;
	bool brk = false, ex = false;
	COORD tempForCoridor;
	srand(time(NULL));
	// заполнение массива стенами (единицами)
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
	//выбор места старта и финиша
	buf = rand() % 4; //выбор места старта
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
		buf = rand() % 4; //выбор места старта
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
//1-вверх 2-вниз 3-лево 4- право 
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
//нарисовать всю карту 
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
//возврящает 1 если достигнут финиш
// и 0 если еще не финиш
bool butgame(char key) 
{
	RGB emp = { 255,255,255 }; // черный цвет для закраски "следа" за игрком
	int kyda = 0; // под направления 1-вверх 2-вниз 3-лево 4- право 
	Set_Poz(pozPL);
	elps(tr, razPL, gan, zal);
	switch (key)
	{
	case 38: //up
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
	case 40: //down
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
	case 37: //left
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
	case 39: //right;
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
	if (sravn(pozPL, finish)) // проверка на финиширование
	{
		if(MessageBoxW(hwnd, TEXT("YOU WIN\nОК выйти в меню, CANCEL - остаться"), TEXT("YOU WIN"), MB_OKCANCEL)==1) // если нажмет ОК выйти в меню, CANCEL - остаться
			return 1;
	}
	return 0;
}

void DrowGame()
{
	RGB fin = { 139, 0, 255 }, strt = { 211, 31, 94 }; //цвета заливки для старта и финиша
	char key = 0;
	Set_Poz(pozPL);
	elps(tr, razPL, gan, zal);
	Set_Poz(start);
	rect(tr, rasmer, gan, strt);
	Set_Poz(finish);
	rect(tr, rasmer, gan, fin);
}
/*функция отрисовки меню, по кол-ву punkt, enable - выделенный пункт меню, 
gran - граница пунктов, zalivka - заливка кнопки, 
далее массивы char для заполнения пунктов меню*/
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
//Отрисовка меню
void DrawMenu(int poz)
{
	switch (NumOfMenu)
	{
	case 1: menu(PunctOf1Menu, poz, gan, zal, "Играть", "Об игре", "Выбор сложности", "Рекорды", "Выход"); break;
	case 2: menu(PunctOf2Menu, poz, gan, zal, "Легко", "Нормально", "Сложно", "Назад"); break;
	default:
		break;
	}
}
//обработка позиции меню
void nummenu(int *poz)
{
	switch (NumOfMenu)
	{
	case 1: //главное меню
	{
		switch (*poz) //смотрим на какой позиции в этом меню
		{
		case 1:*poz = 1; DrowGame(); ClearWindow(); status = Game; feelMap(); break; //выполняем то что было на первом пункте и так далее
		case 2:*poz = 1; WriteTextSymbolBySymbol({ 0,65,350,0 }, "Привет!*я смотрю ты заинтересовался игрой?*забавно)*А ведь я ее пилил чисто по приколу, лишь бы набраться опыта работы с WinApi*Но как не крути я забил на разработку примерно 5 месяцев и напроч забыл чему научился XD*В общем создал ее Дивольд Евгений Владимирович (bog5651)*Примерно 10.11.2017*Удачи!"); break;
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
		case 1:*poz = 1; mode = 1; feelMap(); break; //выполняем то что было на первом пункте и так далее
		case 2:*poz = 1; mode = 2; feelMap(); break;
		case 3:*poz = 1; mode = 3; feelMap(); break;
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