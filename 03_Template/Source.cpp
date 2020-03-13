#include "Include/Common_Header.h"
#include "Include/BasicShapeLoader/Shapes/Matrix_BasicShapes.h"

//Add your header files here
#include "Scene/PerFragment.h"
#include "Scene/Light/PointLight.h"
#include"Scene/Fire/Fire.h"
#include"Scene/KrishnaAnimate/KrishnaAnimate2.h"


// Callback
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

uint64_t initTime, initFrequency, changingTime;

extern int Clothintialize();
extern void Clothdisplay();
extern void Clothunintialize(void);

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
	
	//camera
	void MouseMovement(double xpos, double ypos);
	int xpos = GET_X_LPARAM(lParam);
	int ypos = GET_Y_LPARAM(lParam);

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
		if (GetAsyncKeyState(VK_DOWN))
		{
			Scene_camera.ProcessKeyboard(BACKWARD, 40.0f);
		}
		if (GetAsyncKeyState(VK_UP))
		{
			Scene_camera.ProcessKeyboard(FORWARD, 40.0f);
		}
		if (GetAsyncKeyState(VK_RIGHT))
		{
			Scene_camera.ProcessKeyboard(RIGHT, 40.0f);
		}
		if (GetAsyncKeyState(VK_LEFT))
		{
			Scene_camera.ProcessKeyboard(LEFT, 40.0f);
		}
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

		case VK_UP:
			vmath_camera_eye_coord[1] = vmath_camera_eye_coord[1] + 1.0f;
			break;

		case VK_DOWN:
			vmath_camera_eye_coord[1] = vmath_camera_eye_coord[1] - 1.0f;
			break;

		default:
			break;
		}
		break;
	case WM_MOUSEMOVE:
		//xpos = GET_X_LPARAM(lParam);
		//ypos = GET_Y_LPARAM(lParam);
		//MouseMovement((double)xpos, (double)ypos);
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

			// eye coordinate
			// zoom in
		case 'i':
		case 'I':
			vmath_camera_eye_coord[2] = vmath_camera_eye_coord[2] - 10.0f;
			break;

			// zoom out
		case 'o':
		case 'O':
			vmath_camera_eye_coord[2] = vmath_camera_eye_coord[2] + 10.0f;
			break;

		case 'j':
		case 'J':
			vmath_camera_eye_coord[0] = vmath_camera_eye_coord[0] - 10.0f;
			break;

		case 'k':
		case 'K':
			vmath_camera_eye_coord[0] = vmath_camera_eye_coord[0] + 10.0f;
			break;

			// camera center

		case 'w':
		case 'W':
			vmath_camera_center_coord[1] = vmath_camera_center_coord[1] + 10.0f;
			break;

		case 's':
		case 'S':
			vmath_camera_center_coord[1] = vmath_camera_center_coord[1] - 10.0f;
			break;


		case 'A':
		case 'a':
			vmath_camera_center_coord[0] = vmath_camera_center_coord[0] - 10.0f;
			break;

		case 'D':
		case 'd':
			vmath_camera_center_coord[0] = vmath_camera_center_coord[0] + 10.0f;
			break;

		case 'p':
			fprintf(gpFile, "Current Positins  : \n");
			fprintf(gpFile, "___________________________________________________ \n");
			fprintf(gpFile, "eye coodinates    :  %f     , %f     , %f  \n", vmath_camera_eye_coord[0], vmath_camera_eye_coord[1], vmath_camera_eye_coord[2]);
			fprintf(gpFile, "center coodinates :  %f     , %f     , %f  \n", vmath_camera_center_coord[0], vmath_camera_center_coord[1], vmath_camera_center_coord[2]);
			fprintf(gpFile, "up coodinates     :  %f     , %f     , %f  \n", vmath_camera_up_coord[0], vmath_camera_up_coord[1], vmath_camera_up_coord[2]);
			fprintf(gpFile, "___________________________________________________ \n");
			fprintf(gpFile, "ftime_krishnaAnimate : %f \n", ftime_krishnaAnimate);
			fprintf(gpFile, "___________________________________________________ \n");
			fflush(gpFile);
			break;


		case 'c':		// start camera animation
		case 'C':
			/*
			vmath_camera_eye_coord[0] = -40.0f;
			vmath_camera_eye_coord[1] = 180.0f;
			vmath_camera_eye_coord[2] = 1.0f;

			vmath_camera_center_coord[0] = -1000.0f;
			vmath_camera_center_coord[1] = 200.00f;
			vmath_camera_center_coord[2] = 0.0f;
		

			vmath_camera_eye_coord[0] = 144.0f;
			vmath_camera_eye_coord[1] = 39.0f;
			vmath_camera_eye_coord[2] = -2.0f;

			vmath_camera_center_coord[0] = 0.0f;
			vmath_camera_center_coord[1] = 91.00f;
			vmath_camera_center_coord[2] = 0.0f;

			

			vmath_camera_eye_coord[0] = 245.0f;
			vmath_camera_eye_coord[1] = 95.0f;
			vmath_camera_eye_coord[2] = -240.0f;

			vmath_camera_center_coord[0] = -260.0f;
			vmath_camera_center_coord[1] = 260.0f;
			vmath_camera_center_coord[2] = 0.0f;
			*/
			
			vmath_camera_eye_coord[0] = -35.0f;
			vmath_camera_eye_coord[1] = 105.0f;
			vmath_camera_eye_coord[2] = 10.0f;

			vmath_camera_center_coord[0] = -350.0f;
			vmath_camera_center_coord[1] = 100.0f;
			vmath_camera_center_coord[2] = 0.0f;
			
			gbStartCamera	= !gbStartCamera;
			startJoin_krishnaAnimate = true;
			ftime_krishnaAnimate = 1250.0f;
			break;

		case 'q':
		case 'Q':
			vmath_camera_eye_coord[0] = 3190.0f;	
			vmath_camera_eye_coord[1] = 80.0f;	
			vmath_camera_eye_coord[2] = 610.0f;
			
			vmath_camera_center_coord[0] = 0.0f;	
			vmath_camera_center_coord[1] = -235.00f;
			vmath_camera_center_coord[2] = 0.0f;
			break;
		
		case 'E':
		case 'e':
			Done_krishnaAnimate			= false;
			startExplode_krishnaAnimate = true;
			startJoin_krishnaAnimate	= false;
			break;

		case 'R':
		case 'r':
			
			startJoin_krishnaAnimate = true;
			DoneFlag_krishnaAnimate  = !DoneFlag_krishnaAnimate;
			break;


		default:
			break;
		}
		break;
	}

	return ((DefWindowProc(hwnd, iMsg, wParam, lParam)));
}

void MouseMovement(double xpos, double ypos)
{
	if (FirstMouse)
	{
		lastX = (GLfloat)xpos;
		lastY = (GLfloat)ypos;
		FirstMouse = false;
	}
	GLfloat xoffset = (GLfloat)xpos - lastX;
	GLfloat yoffset = lastY - (GLfloat)ypos;
	lastX = (GLfloat)xpos;
	lastY = (GLfloat)ypos;
	Scene_camera.ProcessMouseMovement(xoffset, yoffset);
	if (lastX != (float)midX || lastY != (float)midY)
		SetCursorPos(midX, midY);
	lastX = (float)midX;
	lastY = (float)midY;
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

	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.cColorBits = 32;
	pfd.cRedBits = 8;
	pfd.cGreenBits = 8;
	pfd.cBlueBits = 8;
	pfd.cAlphaBits = 8;
	pfd.cDepthBits = 32;

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

	//camera
	Scene_camera = Camera(cameraPos, cameraUp, _yaw, _pitch);

	// ................................................................................................
	//
	// Initialize your specific scene here below
	//
	// ................................................................................................

	initialize_perFragmentLight();
	initialize_pointLight();
	
	initialize_krishnaAnimate();

	Clothintialize();

	initialize_fire();

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
	display_pointLight();
	
	display_krishnaAnimate();
	
	Clothdisplay();

	display_fire();
	
	SwapBuffers(ghdc);
}

void update(void)
{
	//call your scene Update here
	update_perFragmentLight();
	//update_pointLight();
	update_krishnaAnimate();
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

	gPerspectiveProjectionMatrix = perspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 10000.0f);
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
	uninitialize_pointLight();
	uninitialize_krishnaAnimate();

	//Clothunintialize();

	uninitialize_fire();

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
