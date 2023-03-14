#define _CRT_SECURE_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN

#include "glad.h"
#include <windows.h>
#include <iostream>
#include "Application.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, PSTR, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

#if _DEBUG
#pragma comment( linker, "/subsystem:console" )
int main(int argc, const char** argv) {
	return WinMain(GetModuleHandle(NULL), NULL,
		GetCommandLineA(), SW_SHOWDEFAULT);
}
#else
#pragma comment( linker, "/subsystem:windows" )
#endif
#pragma comment(lib, "opengl32.lib")

#define WGL_CONTEXT_MAJOR_VERSION_ARB 0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB 0x2092
#define WGL_CONTEXT_FLAGS_ARB 0x2094
#define WGL_CONTEXT_CORE_PROFILE_BIT_ARB 0x00000001
#define WGL_CONTEXT_PROFILE_MASK_ARB 0x9126
typedef HGLRC(WINAPI* PFNWGLCREATECONTEXTATTRIBSARBPROC)
(HDC, HGLRC, const int*);

typedef const char*
(WINAPI* PFNWGLGETEXTENSIONSSTRINGEXTPROC) (void);
typedef BOOL(WINAPI* PFNWGLSWAPINTERVALEXTPROC) (int);
typedef int (WINAPI* PFNWGLGETSWAPINTERVALEXTPROC) (void);

Application* gApplication = 0;
GLuint gVertexArrayObject = 0;

wchar_t* wideString;


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow) {
	gApplication = new Application();
	
	// standard window definition
	WNDCLASSEX wndclass;
	wndclass.cbSize = sizeof(WNDCLASSEX);
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
	wndclass.lpszMenuName = 0;

	//------------------ // converting const char * to wide-character string before assigning to 'LPCWSTR'
	const char* myString = " Game Window!";
	int size = MultiByteToWideChar(CP_UTF8, 0, myString, -1, NULL, 0);
	wideString = new wchar_t[size];
	MultiByteToWideChar(CP_UTF8, 0, myString, -1, wideString, size);
	//-------------------

	wndclass.lpszClassName = wideString;
	RegisterClassEx(&wndclass);


	// configure window location and size
	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);
	int clientWidth = 800;
	int clientHeight = 600;
	RECT windowRect;
	SetRect(&windowRect,
		(screenWidth / 2) - (clientWidth / 2),
		(screenHeight / 2) - (clientHeight / 2),
		(screenWidth / 2) + (clientWidth / 2),
		(screenHeight / 2) + (clientHeight / 2));

	// change style of window 
	DWORD style = (WS_OVERLAPPED | WS_CAPTION |
		WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX);
	

	// adjust the client rectangle to include all window dressing in its size
	AdjustWindowRectEx(&windowRect, style, FALSE, 0);

	// create the actual window and store a reference to its device context
	HWND hwnd = CreateWindowEx(0, wndclass.lpszClassName,
		wideString, style, windowRect.left,
		windowRect.top, windowRect.right -
		windowRect.left, windowRect.bottom -
		windowRect.top, NULL, NULL,
		hInstance, szCmdLine);
	HDC hdc = GetDC(hwnd);

	// Create an OpenGL context
	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW
		| PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 24;
	pfd.cDepthBits = 32;
	pfd.cStencilBits = 8;
	pfd.iLayerType = PFD_MAIN_PLANE;
	int pixelFormat = ChoosePixelFormat(hdc, &pfd);
	SetPixelFormat(hdc, pixelFormat, &pfd);

	// create a temp OpenGL context using wglCreateContext. temp context needed to create modern context 
	HGLRC tempRC = wglCreateContext(hdc);
	wglMakeCurrent(hdc, tempRC);
	PFNWGLCREATECONTEXTATTRIBSARBPROC
		wglCreateContextAttribsARB = NULL;

	// temp context needed to get pointer wglCreateContextAttribsARB which is used  create modern context 
	wglCreateContextAttribsARB =
		(PFNWGLCREATECONTEXTATTRIBSARBPROC)
		wglGetProcAddress("wglCreateContextAttribsARB");
	
	// tmp context
	const int attribList[] = {
		WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
		WGL_CONTEXT_MINOR_VERSION_ARB, 6,
		WGL_CONTEXT_FLAGS_ARB, 0,
		WGL_CONTEXT_PROFILE_MASK_ARB,
		WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
		0, 
	};

	// function returns OpengGL 4.6 core context profile, bind it, and delete legacy context
	HGLRC hglrc = wglCreateContextAttribsARB(
		hdc, 0, attribList);
		wglMakeCurrent(NULL, NULL);
		wglDeleteContext(tempRC);
		wglMakeCurrent(hdc, hglrc);
	
	// glad library used to load OpenGL 4.6 core functions
	if (!gladLoadGL()) {
		std::cout << "Could not initialize GLAD\n";
	}
	else {
		std::cout << "OpenGL Version " <<
		GLVersion.major << "." << GLVersion.minor <<
		"\n";
	}

	// Enable vsynch on window
	PFNWGLGETEXTENSIONSSTRINGEXTPROC
		_wglGetExtensionsStringEXT =
		(PFNWGLGETEXTENSIONSSTRINGEXTPROC)
		wglGetProcAddress("wglGetExtensionsStringEXT");
	bool swapControlSupported = strstr(
		_wglGetExtensionsStringEXT(),
		"WGL_EXT_swap_control") != 0;

	// loading WGL_EXT_sawp_control if the extension is available
	int vsynch = 0;
	if (swapControlSupported) {
		PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT =
			(PFNWGLSWAPINTERVALEXTPROC)
			wglGetProcAddress("wglSwapIntervalEXT");
		PFNWGLGETSWAPINTERVALEXTPROC
			wglGetSwapIntervalEXT =
			(PFNWGLGETSWAPINTERVALEXTPROC)
			wglGetProcAddress("wglGetSwapIntervalEXT");
		if (wglSwapIntervalEXT(1)) {
			std::cout << "Enabled vsynch\n";
			vsynch = wglGetSwapIntervalEXT();
		}
		else {
			std::cout << "Could not enable vsynch\n";
		}
	}
	else { // !swapControlSupported
		std::cout << "WGL_EXT_swap_control not supported\n";
	}

	// create global VAO that is bound to WinMain and never unbounds until the window is destroyed
	glGenVertexArrays(1, &gVertexArrayObject);
	glBindVertexArray(gVertexArrayObject);

	// display current window
	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);
	gApplication->Initialize();

	// keep track of last frame time to calculate delta time between frames
	DWORD lastTick = GetTickCount();
	MSG msg;
	while (true) {
		// handle window event by peeking current message stack and dispatching messages accordingly
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT) {
				break;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// find delta time between last frame and this one, converting it into seconds. Update application
		DWORD thisTick = GetTickCount();
		float dt = float(thisTick - lastTick) * 0.001f;
		lastTick = thisTick;
		if (gApplication != 0) {
			gApplication->Update(dt);
		}

		// rendering application window 
		if (gApplication != 0) {
			RECT clientRect;
			GetClientRect(hwnd, &clientRect);
			clientWidth = clientRect.right -
				clientRect.left;
			clientHeight = clientRect.bottom -
				clientRect.top;
			glViewport(0, 0, clientWidth, clientHeight);
			glEnable(GL_DEPTH_TEST);
			glEnable(GL_CULL_FACE);
			glPointSize(5.0f);
			glBindVertexArray(gVertexArrayObject);
			glClearColor(0.5f, 0.6f, 0.7f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT |
				GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
			float aspect = (float)clientWidth /
				(float)clientHeight;
			gApplication->Render(aspect);
		}

		// swap buffers and if vysnch is enable glFinish is called after swapbuffers
		if (gApplication != 0) {
			SwapBuffers(hdc);
			if (vsynch != 0) {
				glFinish();
			}
		}
	} // End of game loop
	if (gApplication != 0) {
		std::cout << "Expected application to be null on exit\n";
			delete gApplication;
			delete[] wideString;
	}
	return (int)msg.wParam;
}

// creating event handler. defining WndPrco
LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg,
	WPARAM wParam, LPARAM lParam) {
	switch (iMsg) {

	case WM_CLOSE: // Upon receiving this msg it shutdowns the application class and emits a destroy window msg
		if (gApplication != 0) {
			gApplication->Shutdown();
			delete gApplication;
			delete[] wideString;
			gApplication = 0;
			DestroyWindow(hwnd);
		}
		else {
			std::cout << "Already shut down!\n";
		}
		break;

	case WM_DESTROY: // upon receiving msg, release window's OpenGL resouces i.e. delete VAO and context
		if (gVertexArrayObject != 0) {
			HDC hdc = GetDC(hwnd);
			HGLRC hglrc = wglGetCurrentContext();
			glBindVertexArray(0);
			glDeleteVertexArrays(1, &gVertexArrayObject);
			gVertexArrayObject = 0;
			wglMakeCurrent(NULL, NULL);
			wglDeleteContext(hglrc);
			ReleaseDC(hwnd, hdc);
			PostQuitMessage(0);
		}
		else {
			std::cout << "Multiple destroy messages\n";
		}
		break;

	case WM_PAINT: 
	case WM_ERASEBKGND:
		return 0;
	}
	// if msg received isn't one of the message already handled forward it to defualt window msg function
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}
