#include "Engine.h"
#include "framework.h"
#define _USE_MATH_DEFINES
#include <math.h>

enum ELetter_Type {
	ELT_None,
	ELT_O
};

enum EBrick_Type
{
	EBT_None,
	EBT_Pink,
	EBT_Blue
};



HPEN Brick_Pink_Pen, Brick_Blue_Pen, White_Pen, Letter_Pen, BG_Pen;
HBRUSH Brick_Pink_Brush, Brick_Blue_Brush, White_Brush, BG_Brush;

HPEN pen;
HBRUSH brush;
const int Global_Scale = 3;
const int Brick_Width = 15;
const int Brick_Height = 7;
const int Cell_Width = 16;
const int Cell_Height = 8;
const int Level_X_Offset = 8;
const int Level_Y_Offset = 6;
const int Level_Width = 14;
const int Level_Height = 12;
const int Brick_Round = 2 * Global_Scale;
const int Circle_Size = 7;
const int Platform_Y_Pos = 185;
const int Platform_Height = 7;

HWND Hwnd;
int Inner_Width = 21;
int Platform_X_Pos = 0;
int Platform_X_Step = Global_Scale * 2;
int Platform_Width = 28;


RECT Platform_Rect, Prev_Platform_Rect;
RECT Level_Rect;

char Level_01[Level_Width][Level_Height] =
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
void Redraw_Platform()
{
	Prev_Platform_Rect = Platform_Rect;
	Platform_Rect.left = (Level_X_Offset + Platform_X_Pos) * Global_Scale;
	Platform_Rect.top = Platform_Y_Pos * Global_Scale;
	Platform_Rect.right = Platform_Rect.left + Platform_Width * Global_Scale;
	Platform_Rect.bottom = Platform_Rect.top + Platform_Height * Global_Scale;
	InvalidateRect(Hwnd, &Prev_Platform_Rect, FALSE);
	InvalidateRect(Hwnd, &Platform_Rect, FALSE);
}
//-----------------------------------------------------------------------------------------------------------------------
void Init_Engine(HWND hwnd)
{
	Hwnd = hwnd;
	Letter_Pen = CreatePen(PS_SOLID, Global_Scale, RGB(255, 255, 255));
	Create_Pen_Brush(255, 85, 255, Brick_Pink_Pen, Brick_Pink_Brush);
	Create_Pen_Brush(85, 255, 255, Brick_Blue_Pen, Brick_Blue_Brush);
	Create_Pen_Brush(255, 255, 255, White_Pen, White_Brush);
	Create_Pen_Brush(0, 0, 0, BG_Pen, BG_Brush);

	Level_Rect.left = Level_X_Offset * Global_Scale;
	Level_Rect.top = Level_Y_Offset * Global_Scale;
	Level_Rect.right = Level_Rect.left + Cell_Width * Level_Width * Global_Scale;
	Level_Rect.bottom = Level_Rect.top + Cell_Height * Level_Height * Global_Scale;
	Redraw_Platform();
}



//-----------------------------------------------------------------------------------------------------------------------
//Draw bricks

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

void Set_Brick_Letter_Color(bool is_switch_color, HPEN& front_pen, HBRUSH& front_brush, HPEN& back_pen, HBRUSH& back_brush)
{
	if (is_switch_color) {
		front_pen = Brick_Pink_Pen;
		front_brush = Brick_Pink_Brush;
		back_pen = Brick_Blue_Pen;
		back_brush = Brick_Blue_Brush;
	}
	else {
		front_pen = Brick_Blue_Pen;
		front_brush = Brick_Blue_Brush;

		back_pen = Brick_Pink_Pen;
		back_brush = Brick_Pink_Brush;
	}
}
//-----------------------------------------------------------------------------------------------------------------------

void Draw_Brick_Letter(HDC hdc, int x, int y, EBrick_Type brick_type, ELetter_Type letter_type, int rotation_step)
{
	bool is_switch_color;
	double offset;
	double rotation_angle;
	int brick_half_height = (Brick_Height * Global_Scale / 2);
	int back_part_offset;
	HPEN front_pen, back_pen;
	HBRUSH front_brush, back_brush;
	if (!(brick_type == EBT_Blue || brick_type == EBT_Pink))
		return;

	rotation_step %= 16;

	if (rotation_step < 8)
		rotation_angle = 2.0 * M_PI / 16 * (double)rotation_step;
	else
		rotation_angle = 2.0 * M_PI / 16 * (double)(8 - rotation_step);

	if (rotation_step > 4 && rotation_step <= 12) {
		is_switch_color = brick_type == EBT_Blue;
	}
	else
	{
		is_switch_color = brick_type == EBT_Pink;
	}
	Set_Brick_Letter_Color(is_switch_color, front_pen, front_brush, back_pen, back_brush);

	if (rotation_step == 4 || rotation_step == 12)
	{
		//print background
		SelectObject(hdc, back_pen);
		SelectObject(hdc, back_brush);

		Rectangle(hdc, x, y + brick_half_height - Global_Scale, x + Brick_Width * Global_Scale, y + brick_half_height);
		//print foreground
		SelectObject(hdc, front_pen);
		SelectObject(hdc, front_brush);

		Rectangle(hdc, x, y + brick_half_height, x + Brick_Width * Global_Scale, y + brick_half_height + Global_Scale - 1);
	}
	else
	{
		SetGraphicsMode(hdc, GM_ADVANCED);
		//set up the letter rotation matrix
		XFORM xform, old_xform;
		xform.eM11 = 1.0f;
		xform.eM12 = 0.0f;
		xform.eM21 = 0.0f;
		xform.eM22 = (float)cos(rotation_angle);
		xform.eDx = (float)x;
		xform.eDy = (float)y + (float)brick_half_height;

		offset = 3.0 * (1.0 - fabs(xform.eM22)) * (double)Global_Scale;
		back_part_offset = (int)round(offset);

		GetWorldTransform(hdc, &old_xform);
		SetWorldTransform(hdc, &xform);
		//print background
		SelectObject(hdc, back_pen);
		SelectObject(hdc, back_brush);

		Rectangle(hdc, 0, -brick_half_height - back_part_offset, Brick_Width * Global_Scale, brick_half_height - back_part_offset);
		//print foreground
		SelectObject(hdc, front_pen);
		SelectObject(hdc, front_brush);
		Rectangle(hdc, 0, -brick_half_height, Brick_Width * Global_Scale, brick_half_height);

		if (rotation_step > 4 && rotation_step <= 12)
		{
			if (letter_type == ELT_O) {
				SelectObject(hdc, Letter_Pen);
				Ellipse(hdc, 0 + 5 * Global_Scale, (-5 * Global_Scale) / 2, 0 + 10 * Global_Scale, 5 * Global_Scale / 2);
			}
		}

		SetWorldTransform(hdc, &old_xform);
	}
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
	SelectObject(hdc, BG_Pen);
	SelectObject(hdc, BG_Brush);

	Rectangle(hdc, Prev_Platform_Rect.left, Prev_Platform_Rect.top,
		Prev_Platform_Rect.right, Prev_Platform_Rect.bottom);
	int circleDiameter = Circle_Size * Global_Scale;

	SelectObject(hdc, Brick_Blue_Pen);
	SelectObject(hdc, Brick_Blue_Brush);
	Ellipse(hdc,
		x * Global_Scale,
		y * Global_Scale,
		x * Global_Scale + circleDiameter,
		y * Global_Scale + circleDiameter);

	Ellipse(hdc,
		(x + Inner_Width) * Global_Scale,
		y * Global_Scale,
		(x + Inner_Width) * Global_Scale + circleDiameter,
		y * Global_Scale + circleDiameter);

	SelectObject(hdc, White_Pen);
	SelectObject(hdc, White_Brush);

	Arc(hdc,
		(x + 1) * Global_Scale,
		(y + 1) * Global_Scale,
		(x + Circle_Size - 1) * Global_Scale,
		(y + Circle_Size - 1) * Global_Scale,
		(x + 2) * Global_Scale,
		(y + 1) * Global_Scale,
		(x + 1) * Global_Scale,
		(y + 2) * Global_Scale);

	SelectObject(hdc, Brick_Pink_Pen);
	SelectObject(hdc, Brick_Pink_Brush);

	int left = (x + 4) * Global_Scale;
	int top = (y + 1) * Global_Scale;
	int right = (x + 4 + Inner_Width - 1) * Global_Scale;
	int bottom = (y + 1 + 5) * Global_Scale;

	RoundRect(hdc, left, top, right, bottom, 3 * Global_Scale, 3 * Global_Scale);
}


//-----------------------------------------------------------------------------------------------------------------------
//Game scene rendering

void Draw_Frame(HDC hdc, RECT& paint_area)
{
	RECT intersection_rect;
	if (IntersectRect(&intersection_rect, &paint_area, &Level_Rect))
		Draw_Level(hdc);
	if (IntersectRect(&intersection_rect, &paint_area, &Platform_Rect))
		Draw_Platform(hdc, Level_X_Offset + Platform_X_Pos, Platform_Y_Pos);

	/*int i;
	for (i = 0; i < 16; i++)
	{
		Draw_Brick_Letter(hdc, 20 + i * Cell_Width * Global_Scale, 100, EBT_Blue,ELT_O, i);
		Draw_Brick_Letter(hdc, 20 + i * Cell_Width * Global_Scale, 130, EBT_Pink, ELT_O, i);
	}*/

}


//-----------------------------------------------------------------------------------------------------------------------
int On_Key_Down(EKey_Type key_type) {
	switch (key_type) {
	case EKT_Left:
		Platform_X_Pos -= Platform_X_Step;
		Redraw_Platform();
		break;
	case EKT_Right:
		Platform_X_Pos += Platform_X_Step;
		Redraw_Platform();
		break;
	case EKT_Space:
		break;
	}
	return 0;
}
//-----------------------------------------------------------------------------------------------------------------------
