#pragma once

#include"..\..\Include\Fire\FireHeader.h"


GLuint gVertexShaderObject_fire;
GLuint gFragmentShaderObject_fire;
GLuint gShaderProgramObject_fire;


GLuint gModelMatrixUniform_fire, gViewMatrixUniform_fire, gProjectionMatrixUniform_fire;

mat4 gPerspectiveProjectionMatrix_fire;

GLuint timeUniform_fire, PointSizeUniform_fire, sTextureUniform_fire;
GLuint vao_fire, vbo_lifeTime_fire, vbo_xPos_fire, vbo_YSpeed_fire, vbo_color_fire;

float numParticles_fire;

GLuint gParticleTexture_fire;

float t_fire = 0.0f;

float gWidth_fire;

int LoadGLTextures_fire(GLuint* texture, TCHAR imageResourceId[])
{
	HBITMAP hBitmap;
	BITMAP bmp;
	int iStatus = FALSE;
	glGenTextures(1, texture);
	hBitmap = (HBITMAP)LoadImage(GetModuleHandle(NULL), imageResourceId, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION);
	if (hBitmap)
	{
		iStatus = TRUE;
		GetObject(hBitmap, sizeof(bmp), &bmp);

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glBindTexture(GL_TEXTURE_2D, *texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		//		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, bmp.bmWidth, bmp.bmHeight, GL_BGR_EXT, GL_UNSIGNED_BYTE, bmp.bmBits);
		glTexImage2D(GL_TEXTURE_2D,
			0,
			GL_RGB,
			bmp.bmWidth,
			bmp.bmHeight,
			0,
			GL_BGR,
			GL_UNSIGNED_BYTE,
			bmp.bmBits);

		glGenerateMipmap(GL_TEXTURE_2D);
		DeleteObject(hBitmap);
	}
	return (iStatus);
}

void initfire(void)
{

	//VERTEX SHADER
	gVertexShaderObject_fire = glCreateShader(GL_VERTEX_SHADER);

	const GLchar* vertextShaderSourceCode_fire =
		"#version 460" \
		"\n" \

		"in float aLifetime;" \
		"in float aXPos;" \
		"in float aYSpeed;" \
		"in vec2 aColor;" \

		"uniform float uTime;" \
		"uniform float uPointSize;" \

		"uniform mat4 u_model_matrix;" \
		"uniform mat4 u_view_matrix;" \
		"uniform mat4 u_projection_matrix;" \

		"out float vLifetime;" \
		"out vec2 color;" \

		"void main(void)" \
		"{" \
		"vLifetime		= mod(uTime, aLifetime);" \
		"float ti		= 1.0 - vLifetime/aLifetime;" \
		"gl_Position	= u_projection_matrix * u_view_matrix * u_model_matrix * vec4(aXPos*ti, aYSpeed*vLifetime - 1.0, 0.0, 1.0);" \
		"vLifetime		= 4.0 * ti * (1.0 - ti);" \
		"color			= aColor;" \
		"}";

	glShaderSource(gVertexShaderObject_fire, 1, (const GLchar**)&vertextShaderSourceCode_fire, NULL);

	//compile shader
	glCompileShader(gVertexShaderObject_fire);

	checkCompilationLog((char*)"gVertexShaderObject_fire", gVertexShaderObject_fire);

	//FRAGMENT SHADER

	gFragmentShaderObject_fire = glCreateShader(GL_FRAGMENT_SHADER);

	const GLchar* fragmentShaderSourceCode_fire =
		"#version 460" \
		"\n" \
		"uniform sampler2D sTexture;" \

		"in float vLifetime;" \
		"in vec2 color;" \

		"out vec4 FragColor;" \

		"void main(void)" \
		"{" \
		/*"gl_PointSize = 5;" \*/
		"vec4 ColorTemp;" \
		"vec4 texColor	= texture2D(sTexture, gl_PointCoord);" \
		"ColorTemp		= vec4(color, 0., 1.) * texColor ;" \
		"ColorTemp.a	= vLifetime;" \
		"if(ColorTemp.r < 0.1 && ColorTemp.g < 0.1 && ColorTemp.b < 0.1)" \
			"discard;" \
		"FragColor		= ColorTemp;" \
		"}";

	glShaderSource(gFragmentShaderObject_fire, 1, (const GLchar**)&fragmentShaderSourceCode_fire, NULL);

	glCompileShader(gFragmentShaderObject_fire);
	checkCompilationLog((char*)"gFragmentShaderObject_fire", gFragmentShaderObject_fire);


	//Shader Program

	gShaderProgramObject_fire = glCreateProgram();

	glAttachShader(gShaderProgramObject_fire, gVertexShaderObject_fire);
	glAttachShader(gShaderProgramObject_fire, gFragmentShaderObject_fire);

	glBindAttribLocation(gShaderProgramObject_fire, 0, "aLifetime");
	glBindAttribLocation(gShaderProgramObject_fire, 1, "aXPos");
	glBindAttribLocation(gShaderProgramObject_fire, 2, "aYSpeed");
	glBindAttribLocation(gShaderProgramObject_fire, 3, "aColor");

	glLinkProgram(gShaderProgramObject_fire);
	checkLinkLog((char*)"gShaderProgramObject_fire", gShaderProgramObject_fire);


	timeUniform_fire = glGetUniformLocation(gShaderProgramObject_fire, "uTime");
	PointSizeUniform_fire = glGetUniformLocation(gShaderProgramObject_fire, "uPointSize");
	sTextureUniform_fire = glGetUniformLocation(gShaderProgramObject_fire, "sTexture");

	gModelMatrixUniform_fire = glGetUniformLocation(gShaderProgramObject_fire, "u_model_matrix");
	gViewMatrixUniform_fire = glGetUniformLocation(gShaderProgramObject_fire, "u_view_matrix");
	gProjectionMatrixUniform_fire = glGetUniformLocation(gShaderProgramObject_fire, "u_projection_matrix");
}

void initialize_fire(void)
{
	initfire();
	numParticles_fire = 1000;
	
	std::vector<float> lifetimes_fire, xPos_fire, ySpeed_fire, colors_fire;
	

	for (int i = 0; i < numParticles_fire; i++)
	{
		
		lifetimes_fire.push_back(2 * ((GLfloat)(rand() % 1000) / 1000.0f) + 1);
		xPos_fire.push_back(1.5 * (((GLfloat)(rand() % 1000) / 1000.0f) - 0.5));
		ySpeed_fire.push_back(0.7 * ((GLfloat)(rand() % 1000) / 1000.0f));
		colors_fire.push_back((GLfloat)(rand() % 1000) / 1000.0f);
		colors_fire.push_back(0.2 * ((GLfloat)(rand() % 1000) / 1000.0f));

	}

	glGenVertexArrays(1, &vao_fire);
	glBindVertexArray(vao_fire);

	//lifetimes_fire
	glGenBuffers(1, &vbo_lifeTime_fire);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_lifeTime_fire);
	glBufferData(GL_ARRAY_BUFFER, lifetimes_fire.size() * sizeof(float), &lifetimes_fire[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 1, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//xPos_fire
	glGenBuffers(1, &vbo_xPos_fire);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_xPos_fire);
	glBufferData(GL_ARRAY_BUFFER, xPos_fire.size() * sizeof(float), &xPos_fire[0], GL_STATIC_DRAW);
	glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//aYSpeed
	glGenBuffers(1, &vbo_YSpeed_fire);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_YSpeed_fire);
	glBufferData(GL_ARRAY_BUFFER, ySpeed_fire.size() * sizeof(float), &ySpeed_fire[0], GL_STATIC_DRAW);
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, 0);


	//vbo_color_fire
	glGenBuffers(1, &vbo_color_fire);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_color_fire);
	glBufferData(GL_ARRAY_BUFFER, colors_fire.size() * sizeof(float), &colors_fire[0], GL_STATIC_DRAW);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(3);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	glBindVertexArray(0);

	srand(time(0));
	

	bool status = LoadGLTextures_fire(&gParticleTexture_fire, MAKEINTRESOURCE(IDBITMAP_PARTICLE));
	if (status == FALSE)
	{
		fprintf(gpFile, "LoadGLTextures_fire Failed \n");
	}

	/*
	glShadeModel(GL_SMOOTH);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Right now of no use	- for this code
	glEnable(GL_CULL_FACE);								// Right now of no use	- for this code

	glEnable(GL_POINT_SPRITE);							// this is required to use texture on point
	*/
	//glClearColor(0.0f, 0.0f, 0.2f, 1.0f);

}

void display_fire(void)
{
	//glShadeModel(GL_SMOOTH);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);


	glEnable(GL_POINT_SPRITE);							// this is required to use texture on point


	t_fire += 0.01f;
	if (t_fire > 360.0f)
		t_fire = 0.0f;
		
	glUseProgram(gShaderProgramObject_fire);

	mat4 modelMatrix_fire	= mat4::identity();
	mat4 scaleMatrix_fire	= mat4::identity();
	mat4 rotateMatrix_fire	= mat4::identity();

	modelMatrix_fire	= vmath::translate(1200.0f, 100.0f, 0.0f);
	scaleMatrix_fire	= vmath::scale(100.0f, 100.0f, 100.0f);
	rotateMatrix_fire	= rotate(45.0f, 0.0f, 1.0f, 0.0f);

	modelMatrix_fire = modelMatrix_fire * rotateMatrix_fire * scaleMatrix_fire;

	glUniformMatrix4fv(gModelMatrixUniform_fire, 1, GL_FALSE, modelMatrix_fire);
	glUniformMatrix4fv(gViewMatrixUniform_fire, 1, GL_FALSE, gViewMatrix);

	glUniformMatrix4fv(gProjectionMatrixUniform_fire, 1, GL_FALSE, gPerspectiveProjectionMatrix);
	glUniform1f(timeUniform_fire, t_fire);

	//glPointSize(gWidth_fire / 10);
	glPointSize(50);
	glActiveTexture(GL_TEXTURE0);

	glBindTexture(GL_TEXTURE_2D, gParticleTexture_fire);
	glUniform1i(sTextureUniform_fire, 0);

	
	glBindVertexArray(vao_fire);
	glDrawArrays(GL_POINTS, 0, numParticles_fire);
	glBindVertexArray(0);

	glBindTexture(GL_TEXTURE_2D, 0);
	
	glDisable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ZERO);
	//glClearDepth(1.0f);
	//glDisable(GL_DEPTH_TEST);
	//glDepthFunc(GL_LESS);
	
	//glDisable(GL_POINT_SPRITE);							// this is required to use texture on point

	glUseProgram(0);

}



void uninitialize_fire(void)
{
	// will release vao later
	programObjectSafeRelease(gShaderProgramObject_fire);
}

