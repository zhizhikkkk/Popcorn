#pragma once

#include<Windows.h>
enum EKey_Type {
	EKT_Left,
	EKT_Right,
	EKT_Space
};
void Init_Engine(HWND hwnd);
void Draw_Frame(HDC hdc, RECT& paint_area);
int On_Key_Down(EKey_Type key_type);


