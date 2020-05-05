#include "Include/Common_Header.h"


#include "Include/BasicShapeLoader/Shapes/Matrix_BasicShapes.h"

#include "Scene/Framebuffer/Framebuffer.h"

//Add your header files here
#include "Scene/PerFragment.h"
#include "Scene/Light/PointLight.h"
#include "Scene/Bloom/Bloom.h"
#include"Scene/Fire/Fire.h"
#include"Scene/KrishnaAnimate/KrishnaAnimationUsingAssimp.h"
#include"Scene/KrishnaAnimate/KrishnaAnimate5.h"
#include "Scene/GodRays/GodRays.h"

#include "Scene/DepthOfField/DepthOfField.h"

#include"Scene/EndNames/FluidTextRendering.h"

#include "Scene/XAudio/Audio.h"
#include "Scene/FirstScene/FirstScene.h"

// Callback
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);


extern int Clothintialize();
extern void Clothdisplay();
extern void Clothunintialize(void);

//FPS calc in milliseconds
UINT64 iSysFrequency = 0;
UINT64 iCurrentTime = 0;
UINT64 iPreviousTime = 0;


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

	//start Counter
	QueryPerformanceFrequency((LARGE_INTEGER*)&iSysFrequency);

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
			QueryPerformanceCounter((LARGE_INTEGER*)&iCurrentTime);
			/*if ((((double)iCurrentTime - (double)iPreviousTime) / (double)iSysFrequency) >= (1.0 / 60.0))
			{*/
				if (gbActiveWindow == true)
				{
					update();
				}
				display();
				
				iPreviousTime = iCurrentTime;
			//}
			
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
	// added new case just to set default variable values
	case WM_CREATE:
		gbLight = true;
		bIsLKeyPressed = true;
		bShowBloom_bloom = true;
		break;

	case WM_SETFOCUS:
		gbActiveWindow = true;
		break;

	case WM_KILLFOCUS:
		gbActiveWindow = false;
		break;

	case WM_SIZE:
		currentWidth = LOWORD(lParam);
		currentHeight = HIWORD(lParam);
	
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

		case VK_UP:
			if (isShowStartingScene)
				first_scene_camera_eye_coord[1] += 0.1f;
			else
				vmath_camera_eye_coord[1] = vmath_camera_eye_coord[1] + 5.0f;
			break;

		case VK_DOWN:
			if (isShowStartingScene)
				first_scene_camera_eye_coord[1] -= 0.1f;
			else
				vmath_camera_eye_coord[1] = vmath_camera_eye_coord[1] - 5.0f;
			break;

		default:
			break;
		}
		break;
	case WM_MOUSEMOVE:
		break;
	case WM_CHAR:
		switch (LOWORD(wParam))
		{
		case '0':
			gbGoToFullViewKrishna = !gbGoToFullViewKrishna;
			X_Pos_mor_pis = -260.0f;
			/*
			vmath_camera_eye_coord[0] = 1055.0f;
			vmath_camera_eye_coord[1] = 890.0f;
			vmath_camera_eye_coord[2] = -20.0f;

			vmath_camera_center_coord[0] = -270.0f;
			vmath_camera_center_coord[1] = 630.0f;
			vmath_camera_center_coord[2] = 0.0f;
			*/
			break;

		case '1':
			isFirstScene = true;
			break;
		case '2':
			isFirstScene = false;
			isAssimpAnimatedModelShow = true;
			break;
		case '3':
			isShowStartingScene = !isShowStartingScene;
			break;

		case'9':
			gbStartAnimationOfSudarshan = !gbStartAnimationOfSudarshan;
			break;

		case '7':
			gpIXAudio2_SceneFirstSourceVoice->Start(0);
			startAnimation = true;
			break;

		//Reset Logic
		case 'B':
		case 'b':
			//bShowBloom_bloom = !bShowBloom_bloom;

			//first_scene_camera_eye_coord = { 0.0f,0.0f,5.0f };
			first_scene_camera_eye_coord	= { 0.0f,-50.0f,100.0f };
			first_scene_camera_center_coord = { 0.0f,-70.0f,0.0f };
			first_scene_camera_up_coord		= { 0.0f,1.0f,0.0f };

			vmath_camera_eye_coord = { 2910.0f, 75.0f, 560.0f };
			vmath_camera_center_coord = { 0.0f,-255.0f,0.0f };
			vmath_camera_up_coord = { 0.0f,1.0f,0.0f };

			isAssimpAnimatedModelShow = false;
			iShowEndScene = false;
			isShowStartingScene = true;

			//CommonHeader
			isFirstScene = true;
			isHandsUpDone = false;

			//PerFragment
			gbStartAnimationOfSudarshan = false;
			gbStartCamera = false;
			gbZoomOutForFullView = false;
			gbGoToFullViewKrishna = false;

			XForSudarshan = -250.0f;
			YForSudarshan = 790.0f;
			ZForSudarshan = 480.0f;

			//KrishnaAnimatedUsingAssimp
			isModelAnimationStart = false;

			//KrishnaAnimate
			startJoin_krishnaAnimate = false;
			ftime_krishnaAnimate = 0.0f;

			t_for_attractor = 0.03f;
			b_for_attractor = 0.037f;

			X_Pos_attractor = -250.0f;
			Y_Pos_attractor = 150.0f;
			Z_Pos_attractor = -10.0f;

			X_Pos_mor_pis = -250.0f;
			resetAttractorVao();

			//fire
			t_fire = 0.0f;

			//firstScene
			t_fire_FirstScene = 0.0f;

			//End Credits
			NameCount_fluidText = 1;
			ZTransitionForName_fluidText = -35.0f;
			global_temp_count_fluidText = 2.0;
			
			// first scene
			firstSceneBottomUpDone = false;
			endOfFirstScene		= false;

			break;
			
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
			
			if(isShowStartingScene)
				first_scene_camera_eye_coord[2] -= 1.0f;
			else
				vmath_camera_eye_coord[2] = vmath_camera_eye_coord[2] - 10.0f;
			break;

			// zoom out
		case 'o':
		case 'O':
			if (isShowStartingScene)
				first_scene_camera_eye_coord[2] += 1.0f;
			else
				vmath_camera_eye_coord[2] = vmath_camera_eye_coord[2] + 10.0f;
			break;

		case 'j':
		case 'J':
			if (isShowStartingScene)
				first_scene_camera_eye_coord[0] -= 1.0f;
			else
				vmath_camera_eye_coord[0] = vmath_camera_eye_coord[0] - 10.0f;
			break;

		case 'k':
		case 'K':
			if (isShowStartingScene)
				first_scene_camera_eye_coord[0] += 1.0f;
			else
				vmath_camera_eye_coord[0] = vmath_camera_eye_coord[0] + 10.0f;
			break;

			// camera center

		case 'w':
		case 'W':
			if (isShowStartingScene)
				first_scene_camera_center_coord[1] += 1.0f;
			else
				vmath_camera_center_coord[1] = vmath_camera_center_coord[1] + 10.0f;
			break;

		case 's':
		case 'S':
			if (isShowStartingScene)
				first_scene_camera_center_coord[1] -= 1.0f;
			else
				vmath_camera_center_coord[1] = vmath_camera_center_coord[1] - 10.0f;
			break;


		case 'A':
		case 'a':
			if (isShowStartingScene)
				first_scene_camera_center_coord[0] -= 1.0f;
			else
				vmath_camera_center_coord[0] = vmath_camera_center_coord[0] - 10.0f;
			break;

		case 'D':
		case 'd':
			if (isShowStartingScene)
				first_scene_camera_center_coord[0] += 1.0f;
			else
				vmath_camera_center_coord[0] = vmath_camera_center_coord[0] + 10.0f;
			break;

		case 'p':
			/*
			fprintf(gpFile, "Current Positins  : \n");
			fprintf(gpFile, "___________________________________________________ \n");
			fprintf(gpFile, "eye coodinates    :  %f     , %f     , %f  \n", vmath_camera_eye_coord[0], vmath_camera_eye_coord[1], vmath_camera_eye_coord[2]);
			fprintf(gpFile, "center coodinates :  %f     , %f     , %f  \n", vmath_camera_center_coord[0], vmath_camera_center_coord[1], vmath_camera_center_coord[2]);
			fprintf(gpFile, "up coodinates     :  %f     , %f     , %f  \n", vmath_camera_up_coord[0], vmath_camera_up_coord[1], vmath_camera_up_coord[2]);
			fprintf(gpFile, "___________________________________________________ \n");
			fprintf(gpFile, "ftime_krishnaAnimate : %f \n", ftime_krishnaAnimate);
			fprintf(gpFile, "___________________________________________________ \n");
			/*


			/*
			fprintf(gpFile, "Depth Of Field Variables: \n");
			fprintf(gpFile, "____________________________________________________\n");
			fprintf(gpFile, "focal_distance: %f\n", focal_distance);
			fprintf(gpFile, "focal_depth: %f\n", focal_depth);
			fprintf(gpFile, "____________________________________________________\n");
			*/

			/*
			fprintf(gpFile, "Sudarshan Variables: \n");
			fprintf(gpFile, "____________________________________________________\n");
			fprintf(gpFile, "XForSudarshan : %f ,YForSudarshan  : %f ,ZForSudarshan  : %f \n ",XForSudarshan,YForSudarshan,ZForSudarshan);
			fprintf(gpFile, "____________________________________________________\n");
			*/

			/*fprintf(gpFile, "\n\n First Scene Current Positins Camera  : \n");
			fflush(gpFile);
			fprintf(gpFile, "___________________________________________________ \n");
			fflush(gpFile);
			fprintf(gpFile, "eye coodinates    :  %f     , %f     , %f  \n", first_scene_camera_eye_coord[0],		first_scene_camera_eye_coord[1],	first_scene_camera_eye_coord[2]);
			fflush(gpFile);
			fprintf(gpFile, "center coodinates :  %f     , %f     , %f  \n", first_scene_camera_center_coord[0],	first_scene_camera_center_coord[1], first_scene_camera_center_coord[2]);
			fflush(gpFile);
			fprintf(gpFile, "up coodinates     :  %f     , %f     , %f  \n", first_scene_camera_up_coord[0],		first_scene_camera_up_coord[1],		first_scene_camera_up_coord[2]);
			fflush(gpFile);
			fprintf(gpFile, "___________________________________________________ \n");
			fflush(gpFile);*/
			fprintf(gpFile, "fps_counter_second_scene = %f\n", fps_counter_second_scene);
			fflush(gpFile);
			break;


		case 'c':		// start camera animation
		case 'C':
			
			isModelAnimationStart = false;
			isAssimpAnimatedModelShow = false;
			gpIXAudio2_SceneFirstSourceVoice->Start(0);
			
			vmath_camera_eye_coord[0] = -35.0f;
			vmath_camera_eye_coord[1] = 105.0f;
			vmath_camera_eye_coord[2] = 10.0f;

			vmath_camera_center_coord[0] = -350.0f;
			vmath_camera_center_coord[1] = 100.0f;
			vmath_camera_center_coord[2] = 0.0f;
			
			gbStartCamera	= !gbStartCamera;
			startJoin_krishnaAnimate = true;
			
			//ftime_krishnaAnimate = 1250.0f;
			break;

		case 'q':
		case 'Q':
			vmath_camera_eye_coord[0] = 2910.0f;
			vmath_camera_eye_coord[1] = 75.0f;
			vmath_camera_eye_coord[2] = 560.0f;
			
			vmath_camera_center_coord[0] = 0.0f;	
			vmath_camera_center_coord[1] = -255.00f;
			vmath_camera_center_coord[2] = 0.0f;
			break;
		
		case 'E':
		case 'e':
			vmath_camera_eye_coord[0] = -60.0f;
			vmath_camera_eye_coord[1] = 180.0f;
			vmath_camera_eye_coord[2] = 330.0f;

			vmath_camera_center_coord[0] = -250.0f;
			vmath_camera_center_coord[1] = 195.00f;
			vmath_camera_center_coord[2] = 0.0f;
			break;


		case 'z':
		case 'Z':
			isModelAnimationStart = true;
			break;

		// for end scene names
		case 'h':
		case 'H':
			iShowEndScene = !iShowEndScene;
			break;

		case 'g':
		case 'G':
			gpIXAudio2_SceneFirstSourceVoice->Start(0);
			break;

		case 'm':
		case 'M':
			gpIXAudio2_SceneFirstSourceVoice->Stop(0);
			break;

		// sudarshan move
		case 'R':
		case 'r':
			XForSudarshan = XForSudarshan + 10.0f;
			break;

		case 'T':
		case 't':
			XForSudarshan = XForSudarshan - 10.0f;
			break;

		case 'Y':
		case 'y':
			YForSudarshan = YForSudarshan + 10.0f;
			break;

		case 'U':
		case 'u':
			YForSudarshan = YForSudarshan - 10.0f;
			break;

		case '4':
			ZForSudarshan = ZForSudarshan + 10.0f;
			break;

		case '5':
			ZForSudarshan = ZForSudarshan - 10.0f;
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

	initAudio();
	
	//Load All Shapes and Model
	initCubeShape();
	
	initSphereShape();
	initQuadShape();
	LoadAllModels();
	
	//Load FrameBuffer
	initAllFrameBuffer();
	
	//Load All Scenes

	initialize_perFragmentLight();
	initialize_pointLight();
	
	initialize_krishnaAnimate();
	initialize_AssimpModelLoader();
	
	Clothintialize();

	initialize_fire();

	initializeBloom();
	
	initializeDepthOfField();

	initialize_FluidText();

	InitColorProgramShaders();
	initBrightChakraSource();
	initGodRaysPostProcessing();
	
	
	initFirstScene();

	renderShadowDepthShader_FirstScene();
	// ................................................................................................
	//
	// Initialize your specific scene here above
	//
	// ................................................................................................

	gPerspectiveProjectionMatrix = mat4::identity();

	timer.start();
	resize(WIN_WIDTH, WIN_HEIGHT);

	gWidth = WIN_WIDTH;
	gHeight = WIN_HEIGHT;

	glShadeModel(GL_SMOOTH);

	glClearDepth(1.0f);
	// enable depth testing
	glEnable(GL_DEPTH_TEST);
	// depth test to do
	glDepthFunc(GL_LEQUAL);

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glEnable(GL_CULL_FACE);

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // blue

	return(0);	// for successfull return
}

void display(void)
{
	
	if (iShowEndScene)
	{
		display_FluidText();
	}
	else
	{
		static const GLfloat one = { 1.0f };
		static const GLfloat black[] = { 0.0f, 0.0f, 0.0, 1.0f };

		//if (bDoneFadeOutFirstScene == false)
		//{
			//renderShadowDepthShader_FirstScene();
		//}
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glViewport(0, 0, gWidth, gHeight);
		//call your scene Display here

		glBindFramebuffer(GL_FRAMEBUFFER, render_fbo_bloom);
		glClearBufferfv(GL_COLOR, 0, black);	// GL_COLOR_ATTACHMENT0
		glClearBufferfv(GL_COLOR, 1, black);	// GL_COLOR_ATTACHMENT1
		glClearBufferfv(GL_COLOR, 2, black);	// GL_COLOR_ATTACHMENT2
		glClearBufferfv(GL_DEPTH, 0, &one);

	
		if(isShowStartingScene == false)
		{ 
		
			//applyDOF();
			if (gbGoToFullViewKrishna)
			{
				//renderBrightSource();
				renderBrightChakraSource();
			}

			display_perFragmentLight();

			display_pointLight();

			Clothdisplay();
			if (isFireInYadnya == true)
			{
				display_fire();
			}
			if (isFirstScene == false)
			{
				if (isAssimpAnimatedModelShow == true)
					display_AssimpModelLoader();
				else
					display_krishnaAnimate();
			}

			//stopApplyingDOF();
		}
		if (bDoneFadeOutFirstScene == false)
		{
			renderLampWithPointLight();
		}
		

		glBindFramebuffer(GL_FRAMEBUFFER, 0);


		glBindFramebuffer(GL_READ_FRAMEBUFFER, render_fbo_bloom);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, intermediate_fbo);
	
		glReadBuffer(GL_COLOR_ATTACHMENT0);
		glDrawBuffer(GL_COLOR_ATTACHMENT0);
		glBlitFramebuffer(0, 0, MAX_SCENE_WIDTH, MAX_SCENE_HEIGHT, 0, 0, MAX_SCENE_WIDTH, MAX_SCENE_HEIGHT, GL_COLOR_BUFFER_BIT, GL_NEAREST);

		glReadBuffer(GL_COLOR_ATTACHMENT1);
		glDrawBuffer(GL_COLOR_ATTACHMENT1);
		glBlitFramebuffer(0, 0, MAX_SCENE_WIDTH, MAX_SCENE_HEIGHT, 0, 0, MAX_SCENE_WIDTH, MAX_SCENE_HEIGHT, GL_COLOR_BUFFER_BIT, GL_NEAREST);

		glReadBuffer(GL_COLOR_ATTACHMENT2);
		glDrawBuffer(GL_COLOR_ATTACHMENT2);
		glBlitFramebuffer(0, 0, MAX_SCENE_WIDTH, MAX_SCENE_HEIGHT, 0, 0, MAX_SCENE_WIDTH, MAX_SCENE_HEIGHT, GL_COLOR_BUFFER_BIT, GL_NEAREST);

		glBlitFramebuffer(0, 0, MAX_SCENE_WIDTH, MAX_SCENE_HEIGHT, 0, 0, MAX_SCENE_WIDTH, MAX_SCENE_HEIGHT, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
	
		display_GodRaysPostProcessing();

		glViewport(0, 0, gWidth, gHeight);
		ApplyingBloom();
	}

	SwapBuffers(ghdc);
}

void update(void)
{
	//call your scene Update here
	update_perFragmentLight();
	//update_pointLight();
	update_krishnaAnimate();

	/*static float count = 0.0f;
	if (bFadeOutSecondSceneDone == true)
	{
		count += 0.1f;
		if(count >= 7.5f)
		iShowEndScene = true;
	}*/
	if (isShowStartingScene == false)
	{
		fps_counter_second_scene += 1;
		if (fps_counter_second_scene == 885.0f)
		{
			//yadny kunat fir ali ani raje basle
			isFireInYadnya = true;
			isRajeInSabha = true;
		}
		//if (fps_counter_second_scene == 1500)
		//{
		//	//Krishna Baslay, shishupal alay
		//	isFirstScene = false;
		//	isAssimpAnimatedModelShow = true;
		//}
		//if (fps_counter_second_scene == 2100)
		//{
		//	//krishna cha angle side vala
		//	//e
		//	vmath_camera_eye_coord[0] = -60.0f;
		//	vmath_camera_eye_coord[1] = 180.0f;
		//	vmath_camera_eye_coord[2] = 330.0f;

		//	vmath_camera_center_coord[0] = -250.0f;
		//	vmath_camera_center_coord[1] = 195.00f;
		//	vmath_camera_center_coord[2] = 0.0f;
		//}
		if (fps_counter_second_scene == 2700)
		{
			isModelAnimationStart = true;
		}
		//if (fps_counter_second_scene == 2860)
		//{
		//	//shishupal angle
		//	//q
		//	vmath_camera_eye_coord[0] = 2910.0f;
		//	vmath_camera_eye_coord[1] = 75.0f;
		//	vmath_camera_eye_coord[2] = 560.0f;

		//	vmath_camera_center_coord[0] = 0.0f;
		//	vmath_camera_center_coord[1] = -255.00f;
		//	vmath_camera_center_coord[2] = 0.0f;

		//}
		if (fps_counter_second_scene == 3150)
		{
			//c
			isModelAnimationStart = false;
			isAssimpAnimatedModelShow = false;
			//gpIXAudio2_SceneFirstSourceVoice->Start(0);

			vmath_camera_eye_coord[0] = -35.0f;
			vmath_camera_eye_coord[1] = 105.0f;
			vmath_camera_eye_coord[2] = 10.0f;

			vmath_camera_center_coord[0] = -350.0f;
			vmath_camera_center_coord[1] = 100.0f;
			vmath_camera_center_coord[2] = 0.0f;

			gbStartCamera = !gbStartCamera;
			startJoin_krishnaAnimate = true;

		}
		if (fps_counter_second_scene == 6700)
		{
			//0
			gbGoToFullViewKrishna = !gbGoToFullViewKrishna;
			X_Pos_mor_pis = -260.0f;
		}
		if (fps_counter_second_scene == 7750)
		{
			isModelAnimationStart = true;
		}
		if (fps_counter_second_scene == 8450)
		{
			gbStartAnimationOfSudarshan = true;
		}
	}
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

	gWidth = width;
	gHeight = height;

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
	Uninitialize_Audio();

	uninitialize_perFragmentLight();
	uninitialize_pointLight();
	uninitialize_krishnaAnimate();

	Clothunintialize();

	uninitialize_fire();
	
	uninitializeBloom();
	
	uninitializeDOF();
	
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
