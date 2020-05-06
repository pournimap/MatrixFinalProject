#include <windows.h>
#include <windowsx.h>

#include <stdio.h> // for FILE I/O
#include <stdint.h> //for uint64_t
#include <string>

#include <C:\glew\include\GL\glew.h>//use it before other GL.h

#include <gl/GL.h>

#include<vector>		// to use vectors
#include<time.h>

#include "math_lib_headers/vmath.h"
#include "Camera/Camera_2.h"
#include "Include/my_helper_timer.h"

// vmath namespace inclusion
using namespace vmath;

//#pragma comment(lib,"User32.lib")
//#pragma comment(lib,"GDI32.lib")
#pragma comment (lib, "opengl32.lib")
#pragma comment (lib, "C:\\glew\\lib\\Release\\x64\\glew32.lib")
#pragma comment(lib,"assimp-vc140-mt.lib")


#define WIN_WIDTH 800
#define WIN_HEIGHT 600

enum
{
	MATRIX_ATTRIBUTE_POSITION = 0,
	MATRIX_ATTRIBUTE_COLOR,
	MATRIX_ATTRIBUTE_NORMAL,
	MATRIX_ATTRIBUTE_TEXTURE0,
	MATRIX_ATTRIBUTE_TANGENT,
	MATRIX_ATTRIBUTE_BITANGENT,
	MATRIX_ATTRIBUTE_ID,
	MATRIX_ATTRIBUTE_WEIGHTS
};

enum {
	MAX_SCENE_WIDTH = 2048,
	MAX_SCENE_HEIGHT = 2048,
	FBO_SIZE = 2048
};


// Global Variable Declarations
FILE* gpFile = NULL;
bool gbLight = false;
mat4 gPerspectiveProjectionMatrix;
mat4 gViewMatrix;



HDC		ghdc = NULL;
HGLRC	ghrc = NULL;
HWND	ghwnd = NULL;

bool bDone = false;
bool gbFullScreen = false;
bool gbActiveWindow = false;


DWORD dwStyle;
WINDOWPLACEMENT wpPrev = { sizeof(WINDOWPLACEMENT) };

//camera
Camera Scene_camera;
bool FirstMouse = true;
GLfloat lastX = (float)WIN_WIDTH / 2.0;
GLfloat lastY = (float)WIN_HEIGHT / 2.0;
GLfloat midX = WIN_WIDTH / 2.0f;
GLfloat midY = WIN_HEIGHT / 2.0f;
GLfloat deltaTime = 0.0f;
GLfloat currentFrame = 0.0f, lastFrame = 0.0f;
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
float _yaw = -90.0f;
float _pitch = 0.0f;

StopWatchWin timer;

float gWidth, gHeight;

float bloom_thresh_min = 0.8f;
float bloom_thresh_max = 1.2f;

bool isFirstScene = true;
bool isHandsUpDone = false;
bool isShowStartingScene = true;
bool bDoneFadeOutFirstScene = false;
bool bStartFadeOutSecondScene = false;
bool bFadeOutSecondSceneDone = false;
bool iShowEndScene = false;

float fps_counter_second_scene = 0.0f;
bool isFireInYadnya = false;
bool isRajeInSabha = false;
bool isAssimpAnimatedModelShow = false;
bool isModelAnimationStart = false;
bool isStartTitle = true;
bool isEndTitle = false;
bool startAnimation = false;
void programObjectSafeRelease(GLuint shaderProgramObject) {
	// code
	if (shaderProgramObject) {
		GLsizei shaderCount;
		GLsizei shaderNumber;

		glUseProgram(shaderProgramObject);

		glGetProgramiv(shaderProgramObject, GL_ATTACHED_SHADERS, &shaderCount);

		GLuint* pShaders = (GLuint*)malloc(sizeof(GLuint) * shaderCount);

		if (pShaders) {
			glGetAttachedShaders(shaderProgramObject, shaderCount, &shaderCount, pShaders);

			for (shaderNumber = 0; shaderNumber < shaderCount; shaderNumber++) {
				glDetachShader(shaderProgramObject, pShaders[shaderNumber]);

				glDeleteShader(pShaders[shaderNumber]);
				pShaders[shaderNumber] = 0;
			}

			free(pShaders);
		}

		glDeleteProgram(shaderProgramObject);
		shaderProgramObject = 0;

		glUseProgram(0);
	}
}

#pragma region Log
void checkCompilationLog(char* shaderObjectName, GLuint object) {
	// function declaration
	void uninitialize(int);

	// variable declaration
	GLint iShaderCompileStatus = 0;
	GLint iInfoLogLength = 0;
	GLchar* szInfoLog = NULL;

	// code
	glGetShaderiv(object, GL_COMPILE_STATUS, &iShaderCompileStatus);

	if (iShaderCompileStatus == GL_FALSE) {
		glGetShaderiv(object, GL_INFO_LOG_LENGTH, &iInfoLogLength);

		if (iInfoLogLength > 0) {
			szInfoLog = (GLchar*)malloc(sizeof(GLchar) * iInfoLogLength);

			if (szInfoLog != NULL) {
				GLsizei written;

				glGetShaderInfoLog(object, iInfoLogLength, &written, szInfoLog);

				fprintf(gpFile, "\n\n%s Compilation Error: \n%s\n\n", shaderObjectName, szInfoLog);

				glDeleteShader(object);
				free(szInfoLog);
				uninitialize(EXIT_FAILURE);
				DestroyWindow(ghwnd);
				exit(0);
			}
		}
	}
}

void checkLinkLog(char* programObjectName, GLuint programObject) {
	// function declaration
	void uninitialize(int);

	// variable declarations
	GLint iProgramLinkStatus = 0;
	GLint iInfoLogLength = 0;
	GLchar* szInfoLog = NULL;

	// code
	glGetProgramiv(programObject, GL_LINK_STATUS, &iProgramLinkStatus);

	if (iProgramLinkStatus == GL_FALSE) {
		glGetProgramiv(programObject, GL_INFO_LOG_LENGTH, &iInfoLogLength);

		if (iInfoLogLength > 0) {
			szInfoLog = (GLchar*)malloc(sizeof(GLchar) * iInfoLogLength);

			if (szInfoLog != NULL) {
				GLsizei written;

				glGetProgramInfoLog(programObject, iInfoLogLength, &written, szInfoLog);

				fprintf(gpFile, "\n\n%s Linking Error: \n%s\n\n", programObjectName, szInfoLog);

				free(szInfoLog);
				uninitialize(EXIT_FAILURE);
				DestroyWindow(ghwnd);
				exit(0);
			}
		}
	}
}
#pragma endregion
