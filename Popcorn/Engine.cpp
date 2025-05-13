#include "Engine.h"
#include "framework.h"
enum EBrick_Type
{
	EBT_None,
	EBT_Pink,
	EBT_Blue
};

HPEN Brick_Pink_Pen, Brick_Blue_Pen, White_Pen;
HBRUSH Brick_Pink_Brush, Brick_Blue_Brush, White_Brush;

HPEN pen;
HBRUSH brush;
const int Global_Scale = 3;
const int Brick_Width = 15;
const int Brick_Height = 7;
const int Cell_Width = 16;
const int Cell_Height = 8;
const int Level_X_Offset = 8;
const int Level_Y_Offset = 6;
const int Brick_Round = 2 * Global_Scale;
const int Circle_Size = 7;


int Inner_Width = 21;
char Level_01[14][12] =
{
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};
//-----------------------------------------------------------------------------------------------------------------------
void Create_Pen_Brush(unsigned char r, unsigned char g, unsigned char b, HPEN& pen, HBRUSH& brush)
{
	pen = CreatePen(PS_SOLID, 0, RGB(r, g, b));
	brush = CreateSolidBrush(RGB(r, g, b));
}
//-----------------------------------------------------------------------------------------------------------------------
void Init()
{
	Create_Pen_Brush(255, 85, 255, Brick_Pink_Pen, Brick_Pink_Brush);
	Create_Pen_Brush(85, 255, 255, Brick_Blue_Pen, Brick_Blue_Brush);
	Create_Pen_Brush(255, 255, 255, White_Pen, White_Brush);
}

//-----------------------------------------------------------------------------------------------------------------------
//Draw brick

void Draw_Brick(HDC hdc, int x, int y, EBrick_Type brick_type)
{
	switch (brick_type)
	{
	case EBT_None:
		return;
	case EBT_Pink:
		pen = Brick_Pink_Pen;
		brush = Brick_Pink_Brush;
		break;
	case EBT_Blue:
		pen = Brick_Blue_Pen;
		brush = Brick_Blue_Brush;
		break;
	default:
		return;
	}
	HGDIOBJ oldPen = SelectObject(hdc, pen);
	HGDIOBJ oldBrush = SelectObject(hdc, brush);

	RoundRect(hdc, x * Global_Scale, y * Global_Scale,
		(x + Brick_Width) * Global_Scale,
		(y + Brick_Height) * Global_Scale,
		Brick_Round, Brick_Round);

	SelectObject(hdc, oldPen);
	SelectObject(hdc, oldBrush);
}

//-----------------------------------------------------------------------------------------------------------------------
//Bricks rendering
void Draw_Level(HDC hdc)
{
	int i, j;

	for (i = 0; i < 14; i++)
	{
		for (j = 0; j < 12; j++)
		{
			Draw_Brick(hdc, Level_X_Offset + j * Cell_Width, Level_Y_Offset + i * Cell_Height, (EBrick_Type)Level_01[i][j]);
		}
	}
}
//-----------------------------------------------------------------------------------------------------------------------

void Draw_Platform(HDC hdc, int x, int y)
{
	//Drawing side balls
	SelectObject(hdc, Brick_Blue_Pen);
	SelectObject(hdc, Brick_Blue_Brush);
	Ellipse(hdc, x * Global_Scale, y * Global_Scale, (x + Circle_Size) * Global_Scale, (y + Circle_Size) * Global_Scale);
	Ellipse(hdc, (x + Inner_Width) * Global_Scale, 100 * Global_Scale, (x + Circle_Size + Inner_Width) * Global_Scale, (y + Circle_Size) * Global_Scale);
	
	//Drawing highlight
	SelectObject(hdc, White_Pen);
	SelectObject(hdc, White_Brush);
	Arc(hdc, (x + 1) * Global_Scale, (y + 1) * Global_Scale, (x + Circle_Size - 1) * Global_Scale, (y + Circle_Size - 1) * Global_Scale,
		(x + 1 + 1) * Global_Scale, (y + 1) * Global_Scale,
		(x + 1) * Global_Scale, (y + 1 + 1) * Global_Scale);

	//Drawing inner platform
	SelectObject(hdc, Brick_Pink_Pen);
	SelectObject(hdc, Brick_Pink_Brush);
	RoundRect(hdc, (x + 4) * Global_Scale, (y + 1) * Global_Scale, (x + 4 + (Inner_Width - 1)) * Global_Scale, (y + 1 + 5) * Global_Scale, 3 * Global_Scale, 3 * Global_Scale);
}

//-----------------------------------------------------------------------------------------------------------------------
//Game scene rendering

void Draw_Frame(HDC hdc)
{/*
	Draw_Level(hdc);
	Draw_Platform(hdc, 50, 100);*/
	SelectObject(hdc, Brick_Blue_Pen);
	SelectObject(hdc, Brick_Blue_Brush);
	Rectangle(hdc, 20 * Global_Scale, 100 * Global_Scale, (20 + 15) * Global_Scale, (100 + 7) * Global_Scale);
	
}

