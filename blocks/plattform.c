#include <windows.h>
#include <stdbool.h>
#include "plattform.h"
bool active = true;
bool msgcheck = false;

BITMAPINFO bitmap_info;
HWND window;
HDC hdc;

struct RENDER_STATE render_state;

LARGE_INTEGER frequency;
LARGE_INTEGER start_time;

void sleepforms(unsigned int _time_in_milliseconds) {
	Sleep(_time_in_milliseconds);
}

double get_time() {
	LARGE_INTEGER current_time;
	QueryPerformanceCounter(&current_time);
	return (double)(current_time.QuadPart - start_time.QuadPart) * 1000 / (double) frequency.QuadPart;
}

void drawWindow(unsigned int* buffer, int width, int height) {
	SetDIBitsToDevice(hdc, 0, 0, width, height, 0, 0, 0, height, buffer, &bitmap_info, DIB_RGB_COLORS);
}

void* create_thread(void* address, void* args, int* mainthreadID) {
	return CreateThread(NULL, 0, address, args, 0, mainthreadID);
}

void join_thread(void* thread_handle) {
	WaitForSingleObject(thread_handle, INFINITE);

	CloseHandle(thread_handle);
}



POINT GetMousePos() {
	POINT ret;
	GetCursorPos(&ret);
	RECT rect;
	GetClientRect(window, &rect);
	ret.y -= rect.top - 31;
	ret.x = ret.x - 100;
	return ret;
}

short keystate(int key) {
	return GetAsyncKeyState(key);
}

void WinControl() {
	while (active) {
		MSG message;
		while (PeekMessage(&message, window, 0, 0, PM_REMOVE)) {

			TranslateMessage(&message);
			DispatchMessage(&message);

		}
		sleepforms(10);
		
	}

	printf("Windows: recieved stop signal\n");

}

LRESULT CALLBACK WinProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	LRESULT result = 0;
	switch (uMsg) {
	case WM_CLOSE:
	case WM_DESTROY: {
		active = false;
	} break;

	case WM_SIZE: {
		msgcheck = true;
		RECT rect;
		GetClientRect(hwnd, &rect);
		render_state.buffer_width = rect.right - rect.left;
		render_state.buffer_height = rect.bottom - rect.top;

		int size = render_state.buffer_width * render_state.buffer_height * sizeof(unsigned int);

		if (render_state.buffer) VirtualFree(render_state.buffer, 0, MEM_RELEASE);
		render_state.buffer = (unsigned int*) VirtualAlloc(0, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

		bitmap_info.bmiHeader.biSize = sizeof(bitmap_info.bmiHeader);
		bitmap_info.bmiHeader.biWidth = render_state.buffer_width;
		bitmap_info.bmiHeader.biHeight = render_state.buffer_height;
		bitmap_info.bmiHeader.biPlanes = 1;
		bitmap_info.bmiHeader.biBitCount = 32;
		bitmap_info.bmiHeader.biCompression = BI_RGB;
		msgcheck = false;
	} break;

	default:
		result = DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

	return result;
}

int WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       nShowCmd
)

{
	AllocConsole();

	FILE* pConsole;
	freopen_s(&pConsole, "CONOUT$", "w", stdout);

	printf("\n");
	printf("Windows: starting\n");

	QueryPerformanceFrequency(&frequency);
	QueryPerformanceCounter(&start_time);

	WNDCLASS window_class = {
		CS_HREDRAW | CS_VREDRAW,
		WinProc,
		0,
		0,
		hInstance,
		NULL,
		NULL,
		NULL,
		NULL,
		"BasicWindowClass" };

	RegisterClass(&window_class);

	printf("Windows: Registered class\n");

	window = CreateWindow(
		window_class.lpszClassName,
		L"Blocks", // Use L prefix for wide character string
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		700,
		800,
		0,
		0,
		hInstance,
		0
	);

	hdc = GetDC(window);

	printf("Windows: entering main method\n");

	int mainthreadID;
	//HANDLE mainthread = CreateThread(NULL, 0, Entry, NULL, 0, &mainthreadID);

	void* mainthread = create_thread(Entry, NULL, &mainthreadID);

	WinControl();

	join_thread(mainthread);

	//WaitForSingleObject(mainthread, INFINITE);

	//CloseHandle(mainthread);
	
	FreeConsole();

	return 0;
}
