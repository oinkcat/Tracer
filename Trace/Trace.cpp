// Trace.cpp : Определяет точку входа для приложения.
//

#include "stdafx.h"
#include "Trace.h"

#define MAX_LOADSTRING 100

const DWORD WIN_WIDTH = 960;
const DWORD WIN_HEIGHT = 600;

const double VIEW_ANGLE = 1.5;

// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна

RayTracer *tracer;
BITMAPINFO bmi;

UINT_PTR watchTimer;
size_t startTick;

size_t numWorkers;
std::thread* workers;
std::atomic<size_t> numRunningThreads;

// Forward declarations
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

void StartRender()
{
	numWorkers = std::thread::hardware_concurrency();
	tracer->setNumberOfThreads(numWorkers);

	workers = new std::thread[numWorkers];
	startTick = GetTickCount();

	for (size_t i = 0; i < numWorkers; i++)
	{
		workers[i] = std::thread([&]() {
			numRunningThreads++;

			tracer->tracePart();
			numRunningThreads--;
		});
	}
}

void WatchTimerProc(HWND hWnd, UINT msg, UINT_PTR lPtr, DWORD wPtr)
{
	InvalidateRect(hWnd, NULL, false);

	if (numRunningThreads == 0)
	{
		KillTimer(hWnd, watchTimer);

		for (size_t i = 0; i < numWorkers; i++)
		{
			workers[i].join();
		}
	}
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // Инициализация глобальных строк
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_TRACE, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Выполнить инициализацию приложения:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TRACE));

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

    return (int) msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TRACE));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_TRACE);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

void PlaceWindowCentered(HWND hWnd)
{
	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);

	int winX = (screenWidth - WIN_WIDTH) / 2;
	int winY = (screenHeight - WIN_HEIGHT) / 2;

	SetWindowPos(hWnd, NULL, winX, winY, WIN_WIDTH, WIN_HEIGHT, NULL);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance;

   DWORD style = WS_OVERLAPPEDWINDOW & ~(WS_MAXIMIZEBOX | WS_THICKFRAME);
   HWND hWnd = CreateWindowW(szWindowClass, szTitle, style,
	   CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, 
	   nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   // Initialize raytracer
   RECT clientArea;
   GetClientRect(hWnd, &clientArea);
   tracer = new RayTracer(clientArea.right, clientArea.bottom, VIEW_ANGLE);

   StartRender();

   memset(&bmi, 0, sizeof(bmi));
   bmi.bmiHeader.biSize = sizeof(bmi.bmiHeader);
   bmi.bmiHeader.biWidth = tracer->viewportWidth();
   bmi.bmiHeader.biHeight = -tracer->viewportHeight();
   bmi.bmiHeader.biPlanes = 1;
   bmi.bmiHeader.biBitCount = 24;
   bmi.bmiHeader.biSizeImage = 0;

   // Start waiting timer
   watchTimer = SetTimer(hWnd, NULL, 200, (TIMERPROC)WatchTimerProc);

   // Finish window initialization
   PlaceWindowCentered(hWnd);
   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
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
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	}
    case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);

		SetDIBitsToDevice(hdc, 0, 0,
			tracer->viewportWidth(), tracer->viewportHeight(),
			0, 0, 0, tracer->viewportHeight(),
			tracer->frame(), &bmi, DIB_RGB_COLORS);

		EndPaint(hWnd, &ps);
		break;
	}
    case WM_DESTROY:
		if (numRunningThreads > 0)
		{
			for (size_t i = 0; i < numWorkers; i++)
			{
				workers[i].detach();
				TerminateThread((HANDLE)workers[i].native_handle(), 0);
			}
		}

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
