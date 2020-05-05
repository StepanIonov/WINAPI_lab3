#include "windows.h"
#include <tchar.h>
#include "Resource.h"

#define Background RGB(255, 255, 0)//цвет фона
#define Bun RGB(150, 75, 0)//цвет булочек
#define ID_CLOSE 8001 //id кнопки закрытия внутри диалога
#define ID_INSTR1 8002//id 1-го статического поля внутри диалога
#define ID_INSTR2 8003//id 2-го статического поля внутри диалога

INT_PTR CALLBACK Dialog(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	HWND hDlg;//немодальное окно диалога
	TCHAR ClassName[] = _T("lab3");
	HWND hWnd;
	MSG msg;
	WNDCLASS wc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0; 
	wc.hbrBackground = (HBRUSH)CreateSolidBrush(Background);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hInstance = hInstance;
	wc.lpfnWndProc = WndProc;
	wc.lpszClassName = ClassName;
	wc.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	if (!RegisterClass(&wc))
		return 0;
	hWnd = CreateWindow(ClassName, _T("Лаб №3. \"Булочки\""), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, NULL, hInstance, NULL);
	hDlg = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, Dialog);
	ShowWindow(hDlg, nCmdShow);
	ShowWindow(hWnd, nCmdShow);
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!IsDialogMessage(hDlg, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static POINT centr;//центр клиентской области
	static int figure;//id пункта меню
	static RECT pish, bublik; //координаты пышки и круга внутри бублика соответственно
	HDC hdc;
	PAINTSTRUCT ps;
	static HPEN hOldPen;
	static HBRUSH hOldBrush;
	const int radPish = 100, radBub = 50;//радиус пышки и круга внутри бублика соответственно
	switch (message)
	{
	case WM_SIZE: //настройка фигур по центру клиентской области
		centr.x = LOWORD(lParam)/2;
		centr.y = HIWORD(lParam)/2;
		pish.left = centr.x - radPish; pish.top = centr.y - radPish; 
		pish.right = centr.x + radPish; pish.bottom = centr.y + radPish; //координаты пышки
		bublik.left = centr.x - radBub; bublik.top = centr.y - radBub; 
		bublik.right = centr.x + radBub; bublik.bottom = centr.y + radBub; //координаты круга внутри бублика
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_PISH:
			figure = ID_PISH;
			InvalidateRect(hWnd, &pish, true);
			break;
		case ID_BUBLIK:
			figure = ID_BUBLIK;
			InvalidateRect(hWnd, &pish, true);
			break;
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		switch (figure)
		{
		case ID_PISH:
			hOldPen = (HPEN)SelectObject(hdc, CreatePen(PS_NULL, 0, NULL));
			hOldBrush = (HBRUSH)SelectObject(hdc, CreateSolidBrush(Bun));
			Ellipse(hdc, pish.left, pish.top, pish.right, pish.bottom);
			SelectObject(hdc, hOldPen);
			SelectObject(hdc, hOldBrush);
			break;
		case ID_BUBLIK:
			hOldPen = (HPEN)SelectObject(hdc, CreatePen(PS_NULL, 0, NULL));
			hOldBrush = (HBRUSH)SelectObject(hdc, CreateSolidBrush(Bun));
			Ellipse(hdc, pish.left, pish.top, pish.right, pish.bottom);
			hOldBrush = (HBRUSH)SelectObject(hdc, CreateSolidBrush(Background));
			Ellipse(hdc, bublik.left, bublik.top, bublik.right, bublik.bottom);
			SelectObject(hdc, hOldPen);
			SelectObject(hdc, hOldBrush);
			break;
		default:
			break;
		}
		DeleteObject(hOldPen);
		DeleteObject(hOldBrush);
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

INT_PTR CALLBACK Dialog(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	HINSTANCE hInstance;
	switch (message)
	{
	case WM_INITDIALOG:
		hInstance = GetModuleHandle(NULL);
		CreateWindow(_T("Button"), _T("Закрыть"), WS_CHILD | WS_VISIBLE | WS_BORDER,
			10, 100, 70, 30, hDlg, (HMENU)ID_CLOSE, hInstance, NULL);
		CreateWindow(_T("Static"), _T("Для выбора изделия нажмите на пункт меню."), WS_CHILD | WS_VISIBLE,
			10, 10, 400, 20, hDlg, (HMENU)ID_INSTR1, hInstance, NULL);
		CreateWindow(_T("Static"), _T("Для выхода из диалога нажмите на кнопку Закрыть."), WS_CHILD | WS_VISIBLE,
			10, 50, 400, 20, hDlg, (HMENU)ID_INSTR2, hInstance, NULL); 
		return TRUE;
	case WM_CTLCOLORSTATIC:
		if ((HWND)lParam == GetDlgItem(hDlg, ID_INSTR1))
		{
			HDC hdc = (HDC)wParam; 
			SetBkColor(hdc, GetSysColor(CTLCOLOR_DLG));
			SetTextColor(hdc, RGB(255, 0, 0));
		} else
		if ((HWND)lParam == GetDlgItem(hDlg, ID_INSTR2))
		{
			HDC hdc = (HDC)wParam;
			SetBkColor(hdc, GetSysColor(CTLCOLOR_DLG));
			SetTextColor(hdc, RGB(48, 213, 200));
		}
		return (INT_PTR)GetStockObject(NULL_BRUSH);
	case WM_COMMAND:
		if (LOWORD(wParam) == ID_CLOSE)
		{
			DestroyWindow(hDlg);
			return TRUE;
		}
		return FALSE;
	default:	
		break;
	}
	return FALSE;
}