#include "framework.h"
#include "Main.h"

#define MAX_LOADSTRING 100

HINSTANCE hInst;
WCHAR szTitle[MAX_LOADSTRING];
WCHAR szWindowClass[MAX_LOADSTRING];

const int Global_Scale = 3;
const int Brick_Width = 15;
const int Brick_Height = 7;
const int Cell_Width = 16;
const int Cell_Height = 8;
const int Level_X_Offset = 8;
const int Level_Y_Offset = 6;
const int Brick_Round = 5;

ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: Place code here.

	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_POPCORN, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_POPCORN));

	MSG msg;

	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}
//-----------------------------------------------------------------------------------------------------------------------

ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_POPCORN));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = CreateSolidBrush(RGB(0, 0, 0));
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_POPCORN);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

//-----------------------------------------------------------------------------------------------------------------------
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance;
	RECT windowRect;
	windowRect.left = 0;
	windowRect.top = 0;
	windowRect.right = 320 * 3;
	windowRect.bottom = 200 * 3;
	AdjustWindowRect(&windowRect, WS_EX_OVERLAPPEDWINDOW, TRUE);

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		0, 0, windowRect.right - windowRect.left, windowRect.bottom - windowRect.top, nullptr, nullptr, hInstance, nullptr);

	if (hWnd == 0)
		return FALSE;


	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}
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

void Draw_Brick(HDC hdc, int x, int y, bool is_blue)
{
	const int idx = is_blue ? 1 : 0;
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
//Game scene rendering

void Draw_Frame(HDC hdc)
{
	int i, j;

	for (i = 0; i < 14; i++)
	{
		for (j = 0; j < 12; j++)
		{
			Draw_Brick(hdc, Level_X_Offset + j * Cell_Width, Level_Y_Offset + i * Cell_Height, i % 4 >= 2);
		}
	}
}

//-----------------------------------------------------------------------------------------------------------------------
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		Draw_Frame(hdc);
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
//-----------------------------------------------------------------------------------------------------------------------
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
//-----------------------------------------------------------------------------------------------------------------------