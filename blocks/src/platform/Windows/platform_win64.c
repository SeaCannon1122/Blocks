#ifdef _WIN64

#include "platform_win64.h"
#include "headers.h"

bool active = true;

struct window_state window_state;

BITMAPINFO bitmapInfo;
HWND window;
HDC hdc;

LARGE_INTEGER frequency;
LARGE_INTEGER startTime;

bool keyStates[256] = { 0 };

void sleep_for_ms(unsigned int _time_in_milliseconds) {
	Sleep(_time_in_milliseconds);
}

double get_time() {
	LARGE_INTEGER current_time;
	QueryPerformanceCounter(&current_time);
	return (double)(current_time.QuadPart - startTime.QuadPart) * 1000 / (double)frequency.QuadPart;
}

void* create_thread(void* address, void* args) {
	return CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)address, args, 0, NULL);
}

void join_thread(void* thread_handle) {
	WaitForSingleObject(thread_handle, INFINITE);
	CloseHandle(thread_handle);
}

void set_console_cursor_position(int x, int y) {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hConsole, (COORD) { (SHORT)x, (SHORT)y });
}

void draw_to_window(unsigned int* buffer, int width, int height) {
	SetDIBitsToDevice(hdc, 0, 0, width, height, 0, 0, 0, height, buffer, &bitmapInfo, DIB_RGB_COLORS);
}

char get_key_state(int key) {

	if (GetForegroundWindow() != window) return 0;

	char keyState = 0;

	SHORT currentKeyState = GetKeyState(key);

	if (currentKeyState & 0x8000) keyState |= 0b0001;

	if ((currentKeyState & 0x8000 ? 0x1 : 0x0) != keyStates[key]) keyState |= 0b0010;

	if (currentKeyState & 0x01) keyState |= 0b0100;

	keyStates[key] = (currentKeyState & 0x8000 ? 0x1 : 0x0);

	return keyState;
}

void WindowControl() {

	while (active) {
		MSG message;
		while (PeekMessageW(&message, window, 0, 0, PM_REMOVE)) {
			TranslateMessage(&message);
			DispatchMessageW(&message);
		}

		Sleep(10);	
	}

	printf("Windows: recieved stop signal\n");
	return;
}

LRESULT CALLBACK WinProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	LRESULT result = 0;
	switch (uMsg) {
	case WM_CLOSE:
	case WM_DESTROY: {
		active = false;
	} break;

	case WM_SIZE: {
		RECT rect;
		GetClientRect(hwnd, &rect);
		window_state.window_width = rect.right - rect.left;
		window_state.window_height = rect.bottom - rect.top;

		bitmapInfo.bmiHeader.biSize = sizeof(bitmapInfo.bmiHeader);
		bitmapInfo.bmiHeader.biWidth = window_state.window_width;
		bitmapInfo.bmiHeader.biHeight = window_state.window_height;
		bitmapInfo.bmiHeader.biPlanes = 1;
		bitmapInfo.bmiHeader.biBitCount = 32;
		bitmapInfo.bmiHeader.biCompression = BI_RGB;
	} break;

	default:
		result = DefWindowProcW(hwnd, uMsg, wParam, lParam);
	}

	return result;
}

int WINAPI WinMain(
	_In_	 HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_     LPSTR     lpCmdLine,
	_In_     int       nShowCmd
)

{
	(void)hInstance;
	(void)hPrevInstance;
	(void)lpCmdLine;
	(void)nShowCmd;

	HICON hIcon = LoadImageW( NULL, L"resources\\icon.ico", IMAGE_ICON, 0, 0, LR_LOADFROMFILE | LR_DEFAULTCOLOR );

	AllocConsole();

	FILE* pConsole;
	freopen_s(&pConsole, "CONOUT$", "w", stdout);

	WNDCLASS wc = {
		CS_HREDRAW | CS_VREDRAW,
		WinProc,
		0,
		0,
		hInstance,
		hIcon,
		LoadCursorW(NULL, IDC_ARROW),
		NULL,
		NULL,
		L"BasicWindowClass"
	};

	RegisterClassW(&wc);

	window = CreateWindowExW(
		0,
		wc.lpszClassName,
		L display_name,
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		initial_width,
		initial_height,
		NULL,
		NULL,
		hInstance,
		NULL
	);

	SendMessageW(window, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
	SendMessageW(window, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);

	hdc = GetDC(window);

	QueryPerformanceFrequency(&frequency);
	QueryPerformanceCounter(&startTime);

	void* mainthread = create_thread((void*)Entry, NULL);

	WindowControl();

	join_thread(mainthread);
	
	FreeConsole();

	return 0;
}

#endif
