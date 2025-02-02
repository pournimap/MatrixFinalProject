#include "Common_Header.h"
#include "Matrix_Object.h"

//Add your header files here
#include "PerFragment.h"

// Callback
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

uint64_t initTime, initFrequency, changingTime;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow) 
{
	// Function Declaration
	void update(void);
	void display(void);
	int initialize(void);
	void uninitialize(int);
	void ToggleFullScreen(void);

	// Variable Declaration
	WNDCLASSEX wndclass;
	HWND hwnd = NULL;
	MSG msg;
	int iRet = 0;
	//TCHAR szAppName[] = L"Matrix Final Project";
	TCHAR szAppName[] = TEXT("Matrix Final Project");

	fopen_s(&gpFile, "Log.txt", "w");
	fprintf(gpFile, "Log File Successfully Created.\n");
	fflush(gpFile);

	wndclass.cbSize = sizeof(WNDCLASSEX);
	wndclass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.lpfnWndProc = WndProc;
	wndclass.lpszClassName = szAppName;
	wndclass.lpszMenuName = NULL;
	wndclass.hInstance = hInstance;
	wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	if (!RegisterClassEx(&wndclass)) {
		MessageBox(NULL, TEXT("RegisterClassEx() Failed.\nProgram Will Now Exit."), TEXT("Error"), MB_OK);
		fprintf(gpFile, "RegisterClassEx() Failed.\n");
		fflush(gpFile);
		uninitialize(EXIT_FAILURE);
		//exit(0);
	}

	hwnd = CreateWindowEx(WS_EX_APPWINDOW,
		szAppName,
		TEXT("Matrix Final Project"),
		WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		WIN_WIDTH,
		WIN_HEIGHT,
		HWND_DESKTOP,
		NULL,
		hInstance,
		NULL);

	if (hwnd == NULL) {
		MessageBox(NULL, TEXT("CreateWindowEx() Failed.\nProgram Will Now Exit."), TEXT("Error"), MB_OK);
		fprintf(gpFile, "CreateWindowEx() Failed.\n");
		fflush(gpFile);
		//exit(0);
		uninitialize(EXIT_FAILURE);
	}

	ghwnd = hwnd;

	iRet = initialize();

	if (iRet == -1) {
		fprintf(gpFile, "ChoosePixelFormat() Failed.\n");
		fflush(gpFile);
		DestroyWindow(ghwnd);
	}
	else if (iRet == -2) {
		fprintf(gpFile, "SetPixelFormat() Failed.\n");
		fflush(gpFile);
		DestroyWindow(ghwnd);
	}
	else if (iRet == -3) {
		fprintf(gpFile, "wglCreateContext() Failed.\n");
		fflush(gpFile);
		DestroyWindow(ghwnd);
	}
	else if (iRet == -4) {
		fprintf(gpFile, "wglMakeCurrent() Failed.\n");
		fflush(gpFile);
		DestroyWindow(ghwnd);
	}
	else {
		fprintf(gpFile, "Initialization Succeeded.\n");
		fflush(gpFile);
	}

	ShowWindow(hwnd, iCmdShow);
	SetForegroundWindow(hwnd);
	SetFocus(hwnd);
	ToggleFullScreen();						// default full screen mode

	while (bDone == false)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				bDone = true;
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			if (gbActiveWindow == true)
			{
				update();
			}
			display();
		}
	}

	//uninitialize(0);
	return((int)(msg.wParam));
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	// Function Declaration
	void resize(int, int);
	void uninitialize(int);
	void ToggleFullScreen(void);

	static bool bIsLKeyPressed = false;

	// Code
	switch (iMsg)
	{
	case WM_SETFOCUS:
		gbActiveWindow = true;
		break;

	case WM_KILLFOCUS:
		gbActiveWindow = false;
		break;

	case WM_SIZE:
		resize(LOWORD(lParam), HIWORD(lParam));
		break;

	case WM_ERASEBKGND:
		return(0);

	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;

	case WM_DESTROY:
		uninitialize(EXIT_SUCCESS);
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		switch (LOWORD(wParam))
		{
		case VK_ESCAPE:
			DestroyWindow(hwnd);
			break;

		/*
		case 0x46: //f or F
			ToggleFullScreen();
			break;
		case 0x4C:
			if (bIsLKeyPressed == false)
			{
				gbLight = true;
				bIsLKeyPressed = true;
			}
			else
			{
				gbLight = false;
				bIsLKeyPressed = false;
			}
			break;
		*/
		default:
			break;
		}
		break;

	case WM_CHAR:
		switch (LOWORD(wParam))
		{
		case 'F':
		case 'f':
			ToggleFullScreen();
			break;

		case 'L':
		case 'l':
			if (bIsLKeyPressed == false)
			{
				gbLight = true;
				bIsLKeyPressed = true;
			}
			else
			{
				gbLight = false;
				bIsLKeyPressed = false;
			}
			break;

		default:
			break;
		}
		break;
	}

	return ((DefWindowProc(hwnd, iMsg, wParam, lParam)));
}

int initialize(void)
{
	// Function Declaration
	void resize(int, int);
	void uninitialize(int);

	// Variable Declaration
	PIXELFORMATDESCRIPTOR pfd;
	int iPixelFormatIndex = 0;

	GLenum result;

#pragma region DEFAULT_INITIALIZE_PART
	// Code
	ZeroMemory(&pfd, sizeof(PIXELFORMATDESCRIPTOR));

	pfd.nSize		= sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion	= 1;
	pfd.dwFlags		= PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER ;
	pfd.cColorBits	= 32;
	pfd.cRedBits	= 8;
	pfd.cGreenBits	= 8;
	pfd.cBlueBits	= 8;
	pfd.cAlphaBits	= 8;
	pfd.cDepthBits	= 32;

	ghdc = GetDC(ghwnd);

	iPixelFormatIndex = ChoosePixelFormat(ghdc, &pfd);
	if (iPixelFormatIndex == 0) {
		return (-1);
	}

	if ((SetPixelFormat(ghdc, iPixelFormatIndex, &pfd)) == FALSE) {
		return (-2);
	}

	ghrc = wglCreateContext(ghdc);
	if (ghrc == NULL) {
		return (-3);
	}

	if ((wglMakeCurrent(ghdc, ghrc)) == FALSE) {
		return (-4);
	}

	result = glewInit(); //The Extensions Are Enabled And Initialized Here.
	if (result != GLEW_OK) {
		fprintf(gpFile, "glewInit() Failed\n");
		fflush(gpFile);
		uninitialize(EXIT_FAILURE);
	}

	fprintf(gpFile, "%s\n", glGetString(GL_VERSION));
	fprintf(gpFile, "Shading Lang Version : %s \n", glGetString(GL_SHADING_LANGUAGE_VERSION));
	fflush(gpFile);

#pragma endregion


	// ................................................................................................
	//
	// Initialize your specific scene here below
	//
	// ................................................................................................
	
	initialize_perFragmentLight();

	
	
	
	
	
	// ................................................................................................
	//
	// Initialize your specific scene here above
	//
	// ................................................................................................
	
	QueryPerformanceFrequency((LARGE_INTEGER*)&initFrequency);

	QueryPerformanceCounter((LARGE_INTEGER*)&initTime);

	gPerspectiveProjectionMatrix = mat4::identity();

	resize(WIN_WIDTH, WIN_HEIGHT);

	return(0);	// for successfull return
}

void display(void)
{
	/*
	float getTime(void);

	currentFrame = getTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
	*/

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//call your scene Display here
	display_perFragmentLight();

	SwapBuffers(ghdc);
}

void update(void)
{
	//call your scene Update here
	update_perFragmentLight();
}

float getTime(void)
{
	float time = (float)(QueryPerformanceCounter((LARGE_INTEGER*)&changingTime) - initTime) / initFrequency;
	return(time);
}

void resize(int width, int height)
{
	// Code
	if (height == 0)
	{
		height = 1;
	}

	glViewport(0, 0, (GLsizei)width, (GLsizei)height);

	gPerspectiveProjectionMatrix = perspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 1000.0f);
}

void ToggleFullScreen(void)
{
	// Variable Declaration
	MONITORINFO mi;
	// Code
	if (gbFullScreen == false)
	{
		dwStyle = GetWindowLong(ghwnd, GWL_STYLE);
		if (dwStyle & WS_OVERLAPPEDWINDOW)
		{
			mi = { sizeof(MONITORINFO) };
			if ((GetWindowPlacement(ghwnd, &wpPrev)) && GetMonitorInfo((MonitorFromWindow(ghwnd, MONITORINFOF_PRIMARY)), &mi))
			{
				SetWindowLong(ghwnd, GWL_STYLE, dwStyle & ~WS_OVERLAPPEDWINDOW);
				SetWindowPos(ghwnd, HWND_TOP, mi.rcMonitor.left, mi.rcMonitor.top, (mi.rcMonitor.right - mi.rcMonitor.left), (mi.rcMonitor.bottom - mi.rcMonitor.top), SWP_FRAMECHANGED | SWP_NOZORDER);
			}
			ShowCursor(FALSE);
			gbFullScreen = true;
		}	
	}
	else
	{
		SetWindowLong(ghwnd, GWL_STYLE, dwStyle | WS_OVERLAPPEDWINDOW);
		SetWindowPlacement(ghwnd, &wpPrev);
		SetWindowPos(ghwnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOOWNERZORDER | SWP_FRAMECHANGED | SWP_NOZORDER);
		ShowCursor(TRUE);
		gbFullScreen = false;
	}
}

void uninitialize(int i_Exit_Flag)
{
	if (gbFullScreen == false)
	{
		SetWindowPlacement(ghwnd, &wpPrev);
		SetWindowPos(ghwnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOOWNERZORDER | SWP_FRAMECHANGED | SWP_NOZORDER);
		ShowCursor(TRUE);
	}

	// ....................................................................................
	//
	// call your scene uninitialize here below
	//
	// ....................................................................................
	
	uninitialize_perFragmentLight();

	
	
	
	// ....................................................................................
	//
	// call your scene uninitialize here above
	//
	// ....................................................................................
	
	//Stray call to glUseProgram(0)
	glUseProgram(0);

	if (wglGetCurrentContext() == ghrc) {
		wglMakeCurrent(NULL, NULL);
	}

	if (ghrc) {
		wglDeleteContext(ghrc);
		ghrc = NULL;
	}

	if (ghdc) {
		ReleaseDC(ghwnd, ghdc);
		ghdc = NULL;
	}

	if (i_Exit_Flag == EXIT_FAILURE)
	{
		fprintf(gpFile, "Application Closed Erroneously");
		fflush(gpFile);
	}

	if (gpFile)
	{
		fprintf(gpFile, "Log File Closed Successfully");
		fflush(gpFile);
		fclose(gpFile);
		gpFile = NULL;
	}
	
}
