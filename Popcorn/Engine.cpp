#include "Engine.h"
#include "framework.h"
enum EBrick_Type {
	EBT_None,
	EBT_Pink,
	EBT_Blue
};

const int Global_Scale = 3;
const int Brick_Width = 15;
const int Brick_Height = 7;
const int Cell_Width = 16;
const int Cell_Height = 8;
const int Level_X_Offset = 8;
const int Level_Y_Offset = 6;
const int Brick_Round = 2 * Global_Scale;

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
struct BrickBrushes {
	HPEN   pen[2];
	HBRUSH brush[2];
	BrickBrushes() {
		pen[0] = CreatePen(PS_SOLID, 0, RGB(255, 85, 255)); // pink
		brush[0] = CreateSolidBrush(RGB(255, 85, 255));
		pen[1] = CreatePen(PS_SOLID, 0, RGB(85, 255, 255));  // blue
		brush[1] = CreateSolidBrush(RGB(85, 255, 255));
	}
	~BrickBrushes() {
		for (int i = 0;i < 2;++i) { DeleteObject(pen[i]); DeleteObject(brush[i]); }
	}
};
static BrickBrushes g_bricks;

//-----------------------------------------------------------------------------------------------------------------------
//Draw brick

void Draw_Brick(HDC hdc, int x, int y, EBrick_Type brick_type)
{
	int idx = 0;
	switch (brick_type) {
	case EBT_None:
		return;
	case EBT_Pink:
		idx = 0;
		break;
	case EBT_Blue:
		idx = 1;
		break;
	default:
		return;
	}
	HGDIOBJ oldPen = SelectObject(hdc, g_bricks.pen[idx]);
	HGDIOBJ oldBrush = SelectObject(hdc, g_bricks.brush[idx]);

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
//Game scene rendering

void Draw_Frame(HDC hdc)
{
	Draw_Level(hdc);
}