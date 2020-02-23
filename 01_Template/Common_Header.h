#include <windows.h>

#include <stdio.h> // for FILE I/O
#include <stdint.h> //for uint64_t

#include <C:\glew\include\GL\glew.h>//use it before other GL.h

#include <gl/GL.h>

#include "vmath.h"

// vmath namespace inclusion
using namespace vmath;

//#pragma comment(lib,"User32.lib")
//#pragma comment(lib,"GDI32.lib")
#pragma comment (lib, "opengl32.lib")
#pragma comment (lib, "C:\\glew\\lib\\Release\\x64\\glew32.lib")


#define WIN_WIDTH 800
#define WIN_HEIGHT 600

enum
{
	MATRIX_ATTRIBUTE_POSITION = 0,
	MATRIX_ATTRIBUTE_COLOR,
	MATRIX_ATTRIBUTE_NORMAL,
	MATRIX_ATTRIBUTE_TEXTURE0,
};

// Global Variable Declarations
FILE* gpFile		= NULL;
bool gbLight		= false;
mat4 gPerspectiveProjectionMatrix;



HDC		ghdc = NULL;
HGLRC	ghrc = NULL;
HWND	ghwnd = NULL;

bool bDone = false;
bool gbFullScreen = false;
bool gbActiveWindow = false;


DWORD dwStyle;
WINDOWPLACEMENT wpPrev = { sizeof(WINDOWPLACEMENT) };


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
void checkCompilationLog(char *shaderObjectName, GLuint object) {
	// function declaration
	void uninitialize(int);

	// variable declaration
	GLint iShaderCompileStatus = 0;
	GLint iInfoLogLength = 0;
	GLchar *szInfoLog = NULL;

	// code
	glGetShaderiv(object, GL_COMPILE_STATUS, &iShaderCompileStatus);

	if (iShaderCompileStatus == GL_FALSE) {
		glGetShaderiv(object, GL_INFO_LOG_LENGTH, &iInfoLogLength);

		if (iInfoLogLength > 0) {
			szInfoLog = (GLchar *)malloc(sizeof(GLchar) * iInfoLogLength);

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

void checkLinkLog(char *programObjectName, GLuint programObject) {
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
			szInfoLog = (GLchar *)malloc(sizeof(GLchar) * iInfoLogLength);

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
