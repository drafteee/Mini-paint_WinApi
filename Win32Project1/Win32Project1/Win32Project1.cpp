// Win32Project1.cpp: определяет точку входа для приложения.
//

#include "stdafx.h"
#include "Win32Project1.h"
#include <stack>
#include "Line.h"
#include <map>
#define MAX_LOADSTRING 100

// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна

// Отправить объявления функций, включенных в этот модуль кода:
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

	// TODO: разместите код здесь.

	// Инициализация глобальных строк
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_WIN32PROJECT1, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Выполнить инициализацию приложения:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WIN32PROJECT1));

	MSG msg;

	// Цикл основного сообщения:
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



//
//  ФУНКЦИЯ: MyRegisterClass()
//
//  НАЗНАЧЕНИЕ: регистрирует класс окна.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WIN32PROJECT1));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_WIN32PROJECT1);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
	
	return RegisterClassExW(&wcex);
}

//
//   ФУНКЦИЯ: InitInstance(HINSTANCE, int)
//
//   НАЗНАЧЕНИЕ: сохраняет обработку экземпляра и создает главное окно.
//
//   КОММЕНТАРИИ:
//
//        В данной функции дескриптор экземпляра сохраняется в глобальной переменной, а также
//        создается и выводится на экран главное окно программы.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // Сохранить дескриптор экземпляра в глобальной переменной

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW ,
		0, 0, 700, 700, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  ФУНКЦИЯ: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  НАЗНАЧЕНИЕ:  обрабатывает сообщения в главном окне.
//
//  WM_COMMAND — обработать меню приложения
//  WM_PAINT — отрисовать главное окно
//  WM_DESTROY — отправить сообщение о выходе и вернуться
//
//
POINT dotsLine;
BOOL line = false;
POINT first,second;
int status;
std::deque<Line> listLines;


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	RECT rect;
	const RECT re;
	
	HDC hdc;
	switch (message)
	{
	case WM_CREATE: {
		break;
	}
	case WM_SIZE:
		GetClientRect(hWnd, &rect);

		break;
	case WM_KEYDOWN: {
		status++;
		//SendMessage(hWnd, WM_PAINT, wParam, 0);
		break;
	}
	case WM_LBUTTONDOWN: {

		//first.x = LOWORD(lParam);
		//first.y = HIWORD(lParam);
		if (status != 1) {
			first.x = LOWORD(lParam);
			first.y = HIWORD(lParam);
		}
		if (status == 0) {
			second = first;
		}
		status++;
		break;
	}
	case WM_MOUSEMOVE: {

		if (status==1) {
			second.x = LOWORD(lParam);
			second.y = HIWORD(lParam);
			InvalidateRect(hWnd, NULL, FALSE);
		}
		else if (status == 2)
		{
			Line li;
			li.firstDot = first;
			li.secondDot = second;
			listLines.push_front(li);
			status = 0;
			
		}
		break;
	}
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// Разобрать выбор в меню:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		case IDM_LINE: {
			line = true;
			break;
		}
		case IDM_PEN: {
			line = true;
			break;
		}
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		//HDC hdc = BeginPaint(hWnd, &ps);
		if (line) {
			HDC hdc = GetDC(hWnd);
			HDC hdc2 = CreateCompatibleDC(hdc);
			HBITMAP hbm = CreateCompatibleBitmap(hdc, 700, 700);
			HANDLE hold = SelectObject(hdc2, hbm);

			COLORREF Scolor = RGB(255, 255, 255);
			HBRUSH brush = CreateSolidBrush(Scolor);
			HPEN pen = CreatePen(PS_SOLID, 1, RGB(100, 100, 100));

			SelectObject(hdc2, brush);
			SelectObject(hdc2, pen);
			
			Rectangle(hdc2, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
			DeleteObject(brush);
			DeleteObject(pen);

			if (!listLines.empty()) {
				for (int i = 0;i < listLines.size();i++) {
					MoveToEx(hdc2, listLines[i].firstDot.x, listLines[i].firstDot.y, NULL);
					LineTo(hdc2, listLines[i].secondDot.x, listLines[i].secondDot.y);
				}
			}
			/*if (status > 0) {
			MessageBoxW(hWnd, _T("dsadas"), _T("fdsafsdfs00"), MB_OK);
			MoveToEx(hdc, 100, 100, NULL);
			LineTo(hdc, 200, 200);
			status--;
			}*/
			if (line) {
				MoveToEx(hdc2, first.x, first.y, NULL);
				LineTo(hdc2, second.x, second.y);
				//first.x = second.x;
				//first.y = second.y;
			}
			BitBlt(hdc, 0, 0, 700, 700, hdc2, 0, 0, SRCCOPY);
			SelectObject(hdc2, hold);
			DeleteObject(hbm);
			DeleteDC(hdc2);
			ReleaseDC(hWnd, hdc);
		}
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


// Обработчик сообщений для окна "О программе".
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
