#define gNumPointLights_pointLight_FirstScene  1

GLuint gWoodTexture_FirstScene;

mat4 viewMatrix_for_firstScene			= mat4::identity();							// we are going to use this, only here. so no neet to declare anywhere else

//vec3 first_scene_camera_eye_coord		= { 0.0f,0.0f,5.0f };
vec3 first_scene_camera_eye_coord		= { 0.0f,-50.0f,100.0f };
vec3 first_scene_camera_center_coord	= { 0.0f,-70.0f,0.0f };
vec3 first_scene_camera_up_coord		= { 0.0f,1.0f,0.0f }; 

bool firstSceneBottomUpDone = false;
bool FirstImageDone			= false;
bool SecondImageDone		= false;
bool ThirdImageDone_1			= false;
bool ThirdImageDone_2			= false;
bool ForthImageDone			= false;
bool FifthImageDone			= false;
bool SixthImageDone			= false;
bool SevenImageDone			= false;
bool LastImageDone			= false;


bool endOfFirstScene = false;

// ... BOOK
GLuint gShaderProgramObject_book;
GLuint gVertexShaderObject_book;
GLuint gFragmentShaderObject_book;

GLuint vao_rectangle_book;
GLuint vbo_position_rectangle_book;
GLuint vbo_texture_rectangle_book;

GLuint gModelMatrixUniform_book;
GLuint gViewMatrixUniform_book;
GLuint gProjectionMatrixUniform_book;

GLuint samplerUniform1_book;

GLuint texture0_book;
GLuint texture1_book;
GLuint texture2_book;
GLuint texture31_book;
GLuint texture32_book;
GLuint texture33_book;
GLuint texture4_book;
GLuint texture5_book;
GLuint texture6_book;
GLuint texture7_book;


bool startAnimation = false;
GLfloat temp_counter_for_candleShow = 0.0f;
bool ShowCandleDone = false;
BOOL loadTexture_firstScene(GLuint* texture, TCHAR imageResourceID[])
{
	HBITMAP hBitmap = NULL;
	BITMAP bmp;
	BOOL bStatus = FALSE;

	// code
	hBitmap = (HBITMAP)LoadImage(GetModuleHandle(NULL), imageResourceID, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION);

	if (hBitmap)
	{
		bStatus = TRUE;

		GetObject(hBitmap, sizeof(BITMAP), &bmp);

		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

		glGenTextures(1, texture);

		glBindTexture(GL_TEXTURE_2D, *texture);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		//gluBuild2DMipmaps(GL_TEXTURE_2D, 3, bmp.bmWidth, bmp.bmHeight, GL_BGR_EXT, GL_UNSIGNED_BYTE, bmp.bmBits);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, bmp.bmWidth, bmp.bmHeight, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, bmp.bmBits);
		glGenerateMipmap(GL_TEXTURE_2D);

		DeleteObject(hBitmap);

		glBindTexture(GL_TEXTURE_2D, 0);

	}

	return(bStatus);
}

void initFirstScene()
{
	void initPointLightShader_FirstScene();

	/*//VERTEX SHADER
	gVertexShaderObject_book = glCreateShader(GL_VERTEX_SHADER);
	const GLchar* vertextShaderSourceCode_book =
		"#version 450 core" \
		"\n" \
		"in vec4 vPosition;" \
		"in vec2 vTexcoord;" \
		"out vec2 out_texcoord;" \
		"uniform mat4 u_model_matrix;" \
		"uniform mat4 u_view_matrix;" \
		"uniform mat4 u_projection_matrix;" \
		"void main(void)" \
		"{" \
		"gl_Position		= u_projection_matrix * u_view_matrix * u_model_matrix * vPosition;" \
		"out_texcoord = vTexcoord;" \
		"}";

	glShaderSource(gVertexShaderObject_book, 1, (const GLchar**)&vertextShaderSourceCode_book, NULL);
	glCompileShader(gVertexShaderObject_book);
	checkCompilationLog((char*)"gVertexShaderObject_book", gVertexShaderObject_book);


	//FRAGMENT SHADER

	gFragmentShaderObject_book = glCreateShader(GL_FRAGMENT_SHADER);
	const GLchar* fragmentShaderSourceCode_book =
		"#version 450 core" \
		"\n" \
		"in vec2 out_texcoord;" \
		"out vec4 FragColor;" \
		"uniform sampler2D u_sampler1;" \

		"void main(void)" \
		"{" \
		"FragColor = texture(u_sampler1,out_texcoord);" \
		"}";

	glShaderSource(gFragmentShaderObject_book, 1, (const GLchar**)&fragmentShaderSourceCode_book, NULL);
	glCompileShader(gFragmentShaderObject_book);
	checkCompilationLog((char*)"gFragmentShaderObject_book", gFragmentShaderObject_book);


	//Shader Program
	gShaderProgramObject_book = glCreateProgram();

	glAttachShader(gShaderProgramObject_book, gVertexShaderObject_book);
	glAttachShader(gShaderProgramObject_book, gFragmentShaderObject_book);

	glBindAttribLocation(gShaderProgramObject_book, MATRIX_ATTRIBUTE_POSITION, "vPosition");
	glBindAttribLocation(gShaderProgramObject_book, MATRIX_ATTRIBUTE_TEXTURE0, "vTexcoord");

	glLinkProgram(gShaderProgramObject_book);

	// Error checking
	checkLinkLog((char*)"gShaderProgramObject_book", gShaderProgramObject_book);


	gModelMatrixUniform_book = glGetUniformLocation(gShaderProgramObject_book, "u_model_matrix");
	gViewMatrixUniform_book = glGetUniformLocation(gShaderProgramObject_book, "u_view_matrix");
	gProjectionMatrixUniform_book = glGetUniformLocation(gShaderProgramObject_book, "u_projection_matrix");

	samplerUniform1_book = glGetUniformLocation(gShaderProgramObject_book, "u_sampler1");
	
	*/

	const GLfloat rectangleVertices_book[] =
	{
		1.0f, 1.0f,0.0f,
		-1.0f, 1.0f,0.0f,
		-1.0f, -1.0f,0.0f,
		1.0f, -1.0f,0.0f
	};

	const GLfloat rectangleTexcoords_book[] =
	{
		1.0f, 1.0f,
		0.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 0.0f
	};

	// create vao
	glGenVertexArrays(1, &vao_rectangle_book);
	glBindVertexArray(vao_rectangle_book);

	// create vbo for position
	glGenBuffers(1, &vbo_position_rectangle_book);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_position_rectangle_book);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rectangleVertices_book), rectangleVertices_book, GL_STATIC_DRAW);
	glVertexAttribPointer(MATRIX_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(MATRIX_ATTRIBUTE_POSITION);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// create vbo for texture
	glGenBuffers(1, &vbo_texture_rectangle_book);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_texture_rectangle_book);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rectangleTexcoords_book), rectangleTexcoords_book, GL_STATIC_DRAW);
	glVertexAttribPointer(MATRIX_ATTRIBUTE_TEXTURE0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(MATRIX_ATTRIBUTE_TEXTURE0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);


	glBindVertexArray(0);

	
	glEnable(GL_TEXTURE_2D);

	bool status = loadTexture_firstScene(&texture1_book, MAKEINTRESOURCE(106));
	if (status == FALSE)
	{
		fprintf(gpFile, "LoadGLTextures_fire Failed in FirstScene\n");
	}

	status = loadTexture_firstScene(&texture2_book, MAKEINTRESOURCE(107));
	if (status == FALSE)
	{
		fprintf(gpFile, "LoadGLTextures_fire Failed in FirstScene\n");
	}

	status = loadTexture_firstScene(&texture31_book, MAKEINTRESOURCE(1081));
	status = loadTexture_firstScene(&texture32_book, MAKEINTRESOURCE(1082));
	status = loadTexture_firstScene(&texture33_book, MAKEINTRESOURCE(1083));
	if (status == FALSE)
	{
		fprintf(gpFile, "LoadGLTextures_fire Failed in FirstScene\n");
	}

	status = loadTexture_firstScene(&texture4_book, MAKEINTRESOURCE(109));
	if (status == FALSE)
	{
		fprintf(gpFile, "LoadGLTextures_fire Failed in FirstScene\n");
	}

	status = loadTexture_firstScene(&texture5_book, MAKEINTRESOURCE(110));
	if (status == FALSE)
	{
		fprintf(gpFile, "LoadGLTextures_fire Failed in FirstScene\n");
	}

	status = loadTexture_firstScene(&texture6_book, MAKEINTRESOURCE(111));
	if (status == FALSE)
	{
		fprintf(gpFile, "LoadGLTextures_fire Failed in FirstScene\n");
	}

	status = loadTexture_firstScene(&texture7_book, MAKEINTRESOURCE(112));
	if (status == FALSE)
	{
		fprintf(gpFile, "LoadGLTextures_fire Failed in FirstScene\n");
	}

	status = loadTexture_firstScene(&texture0_book, MAKEINTRESOURCE(113));
	if (status == FALSE)
	{
		fprintf(gpFile, "LoadGLTextures_fire Failed in FirstScene\n");
	}

	status = loadTexture_firstScene(&gWoodTexture_FirstScene, MAKEINTRESOURCE(104));
	if (status == FALSE)
	{
		fprintf(gpFile, "loadTexture_firstScene Failed in FirstScene\n");
	}

}

float t_fire_FirstScene = 0.0f;
//vec3 positionLamp_FirstScene[] = { vec3(0.0f, 0.0f, 0.0f), vec3(0.5f, -0.5f, 0.0f) };

//vec3 positionLamp_FirstScene[] = { vec3(-20.0f, 30.0f, 0.0f), vec3(0.5f, -0.5f, 0.0f) };
vec3 positionLamp_FirstScene[] = { vec3(0.0f, 50.0f, 20.0f), vec3(0.0f, -0.5f, 0.0f) };
//vec3 positionLamp_FirstScene[] = { vec3(-25.0f, -3.0f, -10.0f), vec3(0.0f, 10.0f, -10.0f) };

void draw_image(int isBloom, GLfloat TranslateX, GLfloat TranslateY, GLfloat TranslateZ, GLfloat RotateAngleZ, GLfloat RotateZDir, GLuint texture_id)
{
	mat4 modelMatrix	= mat4::identity();
	mat4 scaleMatrix	= mat4::identity();
	mat4 rotateMatrix	= mat4::identity();
	mat4 rotateMatrix2	= mat4::identity();

	modelMatrix		= vmath::translate(TranslateX, TranslateY, TranslateZ);
	scaleMatrix		= vmath::scale(20.0f, 10.0f, 1.0f);
	rotateMatrix	= rotate(90.0f, -1.0f, 0.0f, 0.0f);
	rotateMatrix2	= rotate(RotateAngleZ, 0.0f, 0.0f, RotateZDir);
	modelMatrix		= modelMatrix * rotateMatrix * rotateMatrix2 *scaleMatrix;

	glUniformMatrix4fv(gModelMatrixUniform_pointLight, 1, GL_FALSE, modelMatrix);
	glUniform1i(applyBloomUniform_pointLight, isBloom);

	glUniform1i(gTextureActiveUniform_pointLight, 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_id);
	glUniform1i(gTextureSamplerUniform_pointLight, 0);
	//drawMyQuadShape();
	glBindVertexArray(vao_rectangle_book);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	glBindVertexArray(0);
}

float FadeOutFactor_pointLightFirstScene = 1.0f;
void renderLampWithPointLight()
{
	void updteForFirstScene(void);

		mat4 modelMatrix				= mat4::identity();
		mat4 scaleMatrix				= mat4::identity();
		mat4 rotateMatrix				= mat4::identity();
		
		PointLight pointLight[gNumPointLights_pointLight_FirstScene];

		// set the camera matrix at first
		viewMatrix_for_firstScene = lookat(first_scene_camera_eye_coord, first_scene_camera_center_coord, first_scene_camera_up_coord);					// updating view matrix
	
		for (int i = 0; i < gNumPointLights_pointLight_FirstScene; i++)
		{
				pointLight[i].u_La				= vec3(0.0f, 0.0f, 0.0f);
				pointLight[i].u_Ld				= vec3(1.0f, 0.749f, 0.0f);
				pointLight[i].u_Ls				= vec3(1.0f, 0.749f, 0.0f);
				pointLight[i].u_linear			= 0.01;
				pointLight[i].u_constant		= 0.0;
				pointLight[i].u_quadratic		= 0.0;
				pointLight[i].DiffuseIntensity	= 1.0f;

				pointLight[i].position = positionLamp_FirstScene[i];
		}
	
	
		glUseProgram(gShaderProgramObject_pointLight);

		if (isShowStartingScene == false)
		{
			if (FadeOutFactor_pointLightFirstScene >= 0.0f)
				FadeOutFactor_pointLightFirstScene -= 0.01f;
			else
				bDoneFadeOutFirstScene = true;
		}
		glUniform1f(fadeoutFactorUniform_pointLight, FadeOutFactor_pointLightFirstScene);
		glUniform1f(fadeinFactorUniform_pointLight, 1.0f);

		glUniform1i(u_bloom_is_activeUniform_pointLight, 1);
		glUniform1f(bloom_thresh_minUniform_pointLight, bloom_thresh_min);
		glUniform1f(bloom_thresh_maxUniform_pointLight, bloom_thresh_max);
		glUniform1i(applyBloomUniform_pointLight, 0);
		if (gbLight == true)
		{
			glUniform1i(gLKeyPressedUniform_pointLight, 1);

			glUniform3fv(gLdUniform_pointLight, 1, lightDiffuse);//white
			glUniform3fv(gLaUniform_pointLight, 1, lightAmbient);
			glUniform3fv(gLsUniform_pointLight, 1, lightSpecular);
			glUniform4fv(gLightPositionUniform_pointLight, 1, lightPosition);

				//pointLight
			glUniform1i(gNumPointLightsUniform_pointLight, gNumPointLights_pointLight_FirstScene);
			for (int i = 0; i < gNumPointLights_pointLight_FirstScene; i++)
			{
				glUniform3fv(m_pointLightsLocation[i].u_La, 1, pointLight[i].u_La);
				glUniform3fv(m_pointLightsLocation[i].u_Ls, 1, pointLight[i].u_Ls);
				glUniform3fv(m_pointLightsLocation[i].u_Ld, 1, pointLight[i].u_Ld);
				glUniform1f(m_pointLightsLocation[i].DiffuseIntensity, pointLight[i].DiffuseIntensity);

				glUniform3fv(m_pointLightsLocation[i].position, 1, pointLight[i].position);
				glUniform1f(m_pointLightsLocation[i].u_constant, pointLight[i].u_constant);
				glUniform1f(m_pointLightsLocation[i].u_linear, pointLight[i].u_linear);
				glUniform1f(m_pointLightsLocation[i].u_quadratic, pointLight[i].u_quadratic);
			}

			glUniform3fv(gKdUniform_pointLight, 1, material_diffuse);
			glUniform3fv(gKaUniform_pointLight, 1, material_ambient);
			glUniform3fv(gKsUniform_pointLight, 1, material_specular);
			glUniform1f(gMaterialShininessUniform_pointLight, material_shininess);

		}
		else
		{
			glUniform1i(gLKeyPressedUniform_pointLight, 0);
		}


		glUniform1i(applyBloomUniform_pointLight, 0);

		modelMatrix = mat4::identity();
		scaleMatrix = mat4::identity();

		//modelMatrix = vmath::translate(0.0f, -20.0f, -100.0f);
		modelMatrix = vmath::translate(0.0f, -20.0f, 0.0f);
		scaleMatrix = vmath::scale(100.0f, 2.0f, 40.0f);
		modelMatrix = modelMatrix * scaleMatrix;

		glUniformMatrix4fv(gModelMatrixUniform_pointLight, 1, GL_FALSE, modelMatrix);
		glUniformMatrix4fv(gProjectionMatrixUniform_pointLight, 1, GL_FALSE, gPerspectiveProjectionMatrix);
		glUniformMatrix4fv(gViewMatrixUniform_pointLight, 1, GL_FALSE, viewMatrix_for_firstScene);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, gWoodTexture_FirstScene);
		glUniform1i(gTextureSamplerUniform_pointLight, 0);
		glUniform1i(gTextureActiveUniform_pointLight, 1);
		//glUniform1i(samplerUniform1_book, 0);

		drawCubeShape();																	// for khalcha motha paat

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//////				FOR TEXTURES																															//////////////////
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		glUniformMatrix4fv(gViewMatrixUniform_pointLight, 1, GL_FALSE, viewMatrix_for_firstScene);					// globally camera set in perFrag file
		glUniformMatrix4fv(gProjectionMatrixUniform_pointLight, 1, GL_FALSE, gPerspectiveProjectionMatrix);			// globally pojection set

		// void draw_image(int isBloom, GLfloat TranslateX, GLfloat TranslateY, GLfloat TranslateZ, GLfloat RotateAngleZ, GLfloat RotateZDir, GLuint texture_id)
		// image 0 
		draw_image(0, 10.0f, -17.6f, 10.0f, 0.0f, 1.0f,texture0_book);

		// image 1
		draw_image(0, -80.0f, -17.6f, -20.0f, 10.0f, 1.0f,texture1_book);

		// image 2
		draw_image(0, -30.0f, -17.6f, -5.0f, 5.0f, -1.0f,texture2_book);

		// image 3.1,3.2,3.3
		draw_image(0, -80.0f, -17.6f, 25.0f, 10.0f, 1.0f,texture33_book);
		draw_image(0, -50.0f, -17.55f, 15.0f, 5.0f, -1.0f,texture31_book);
		draw_image(0, -25.0f, -17.5f, 30.0f, 5.0f, 1.0f,texture32_book);

		// image 4
		draw_image(0, 70.0f, -17.6f, -25.0f, 10.0f, 1.0f, texture4_book);

		// image 5
		draw_image(0, 50.0f, -17.55f, 0.0f, 5.0f, -1.0f, texture5_book);

		// image 6
		draw_image(0, 80.0f, -17.6f, 25.0f, 10.0f, 1.0f, texture6_book);

		// image 7
		draw_image(0, 40.0f, -17.55f, 30.0f, 5.0f, 1.0f, texture7_book);

		/*
		// bind with vao
		glBindVertexArray(vao_rectangle_book);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
		glBindVertexArray(0);
		*/
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//////				FOR TEXTURES END																														//////////////////
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		glUniform1i(applyBloomUniform_pointLight, 0);
		//candle model
		modelMatrix		= mat4::identity();
		scaleMatrix		= mat4::identity();
		rotateMatrix	= mat4::identity();

		//modelMatrix = vmath::translate(-20.0f, -16.0f, -90.0f);
		//modelMatrix = vmath::translate(-25.0f, -20.0f, -10.0f);
		modelMatrix = vmath::translate(0.0f, -20.0f, -30.0f);
		scaleMatrix = scale(0.07f, 0.07f, 0.07f);
		//rotateMatrix = rotate(-90.0f, 0.0f, 1.0f, 0.0f);

		modelMatrix = modelMatrix * scaleMatrix;

		glUniformMatrix4fv(gModelMatrixUniform_pointLight, 1, GL_FALSE, modelMatrix);
		glUniformMatrix4fv(gViewMatrixUniform_pointLight, 1, GL_FALSE, viewMatrix_for_firstScene);
		glUniformMatrix4fv(gProjectionMatrixUniform_pointLight, 1, GL_FALSE, gPerspectiveProjectionMatrix);
		

		glBindVertexArray(gModel_Candle.Vao);
		for (int i = 0; i < gModel_Candle.model_mesh_data.size(); i++)
		{
			if (gbLight == true)
			{
				glUniform3fv(gKaUniform_pointLight, 1, gModel_Candle.model_material[gModel_Candle.model_mesh_data[i].material_index].Ka);
				glUniform3fv(gKdUniform_pointLight, 1, gModel_Candle.model_material[gModel_Candle.model_mesh_data[i].material_index].Kd);
				glUniform3fv(gKsUniform_pointLight, 1, gModel_Candle.model_material[gModel_Candle.model_mesh_data[i].material_index].Ks);
				glUniform1f(gMaterialShininessUniform_pointLight, material_shininess);
				glUniform1f(gAlphaUniform_pointLight, gModel_Candle.model_material[gModel_Candle.model_mesh_data[i].material_index].d);
				if (gModel_Candle.model_material[gModel_Candle.model_mesh_data[i].material_index].ismap_Kd == true)
				{
					glActiveTexture(GL_TEXTURE0);
					glBindTexture(GL_TEXTURE_2D, gModel_Candle.model_material[gModel_Candle.model_mesh_data[i].material_index].gTexture);
					glUniform1i(gTextureSamplerUniform_pointLight, 0);
					glUniform1i(gTextureActiveUniform_pointLight, 1);
					/*fprintf(gpFile, "perFragmentLight Krishna ismapKd true \n");
					fflush(gpFile);*/
				}
				else
					glUniform1i(gTextureActiveUniform_pointLight, 0);
			}
			glDrawArrays(GL_TRIANGLES, gModel_Candle.model_mesh_data[i].vertex_Index, gModel_Candle.model_mesh_data[i].vertex_Count);
		}
		glBindVertexArray(0);

		for (int i = 0; i < gNumPointLights_pointLight_FirstScene; i++)
		{
			modelMatrix = mat4::identity();
			scaleMatrix = mat4::identity();
			rotateMatrix = mat4::identity();

			modelMatrix = vmath::translate(positionLamp_FirstScene[i]);
			scaleMatrix = scale(0.1f, 0.1f, 0.1f);

			modelMatrix = modelMatrix * scaleMatrix;

			glUniformMatrix4fv(gModelMatrixUniform_pointLight, 1, GL_FALSE, modelMatrix);
			glUniformMatrix4fv(gProjectionMatrixUniform_pointLight, 1, GL_FALSE, gPerspectiveProjectionMatrix);
			glUniformMatrix4fv(gViewMatrixUniform_pointLight, 1, GL_FALSE, viewMatrix_for_firstScene);

			drawCubeShape();					// small quad in code
		}



		
		glUseProgram(0);

#pragma region FIRE
		
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		glUseProgram(gShaderProgramObject_fire);

		if (isShowStartingScene == false)
		{
			if (FadeOutFactor_pointLightFirstScene >= 0.0f)
				FadeOutFactor_pointLightFirstScene -= 0.0001f;
		}

		glUniform1f(fadeoutFactorUniform_pointLight, FadeOutFactor_pointLightFirstScene);
		glUniform1f(fadeinFactorUniform_pointLight, 1.0f);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		glClearDepth(1.0f);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		glEnable(GL_POINT_SPRITE);

		//t_fire_FirstScene += 0.01f;
		t_fire_FirstScene += 0.1f;
		if (t_fire_FirstScene > 360.0f)
			t_fire_FirstScene = 0.0f;

		glUseProgram(gShaderProgramObject_fire);

		glUniform1i(u_bloom_is_activeUniform_fire, 1);
		glUniform1f(bloom_thresh_minUniform_fire, bloom_thresh_min);
		glUniform1f(bloom_thresh_maxUniform_fire, bloom_thresh_max);
		glUniform1i(applyBloomUniform_fire, 1);

		mat4 modelMatrix_fire = mat4::identity();
		mat4 scaleMatrix_fire = mat4::identity();
		mat4 rotateMatrix_fire = mat4::identity();

		//modelMatrix_fire = vmath::translate(-20.0f, 1.0f, -90.0f);
		//modelMatrix_fire = vmath::translate(-25.5f, -3.0f, -10.0f);
		modelMatrix_fire = vmath::translate(-0.3f, -3.5f, -30.0f);
		scaleMatrix_fire = vmath::scale(0.4f, 2.0f, 0.05f);
		//scaleMatrix_fire	= vmath::scale(50.0f, 50.0f, 100.0f);
		//rotateMatrix_fire = rotate(75.0f, 0.0f, 1.0f, 0.0f);

		modelMatrix_fire = modelMatrix_fire * rotateMatrix_fire * scaleMatrix_fire;

		glUniformMatrix4fv(gModelMatrixUniform_fire, 1, GL_FALSE, modelMatrix_fire);
		glUniformMatrix4fv(gViewMatrixUniform_fire, 1, GL_FALSE, viewMatrix_for_firstScene);
		glUniformMatrix4fv(gProjectionMatrixUniform_fire, 1, GL_FALSE, gPerspectiveProjectionMatrix);

		glUniform1f(timeUniform_fire, t_fire_FirstScene);

		//glPointSize(gWidth_fire / 10);
		//glPointSize(50);
		glPointSize(5);
		glActiveTexture(GL_TEXTURE0);

		glBindTexture(GL_TEXTURE_2D, gParticleTexture_fire);
		glUniform1i(sTextureUniform_fire, 0);


		glBindVertexArray(vao_fire);
		glDrawArrays(GL_POINTS, 0, numParticles_fire);
		glBindVertexArray(0);

		glBindTexture(GL_TEXTURE_2D, 0);

		glDisable(GL_BLEND);
		glUseProgram(0);

#pragma endregion

		// books ....................................................

		/*glUseProgram(gShaderProgramObject_book);

		modelMatrix = mat4::identity();
		scaleMatrix = mat4::identity();
		rotateMatrix = mat4::identity();

		modelMatrix = vmath::translate(0.0f, -15.0f, 10.0f);
		scaleMatrix = vmath::scale(20.0f, 10.0f, 1.0f);
		rotateMatrix = rotate(90.0f, -1.0f, 0.0f, 0.0f);
		modelMatrix = modelMatrix * rotateMatrix * scaleMatrix;

		glUniformMatrix4fv(gModelMatrixUniform_book, 1, GL_FALSE, modelMatrix);
		glUniformMatrix4fv(gViewMatrixUniform_book, 1, GL_FALSE, viewMatrix_for_firstScene);								    // globally camera set in perFrag file
		glUniformMatrix4fv(gProjectionMatrixUniform_book, 1, GL_FALSE, gPerspectiveProjectionMatrix);			// globally pojection set

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1_book);
		glUniform1i(samplerUniform1_book, 0);

		// bind with vao
		glBindVertexArray(vao_rectangle_book);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
		glBindVertexArray(0);

		glUseProgram(0);*/

		if (startAnimation == true)
		{
			updteForFirstScene();
		}
}

// vec3 first_scene_camera_eye_coord =		{ 0.0f,-50.0f,100.0f };
// vec3 first_scene_camera_center_coord =	{ 0.0f,-70.0f,0.0f };
// vec3 first_scene_camera_up_coord =		{ 0.0f,1.0f,0.0f };
void updteForFirstScene()
{
	// here we should write 1st scene camera movement
	if (first_scene_camera_eye_coord[1] < 50.0f && first_scene_camera_eye_coord[2] < 165.0f && first_scene_camera_center_coord[1] < 30.0f && firstSceneBottomUpDone == false)
	{
		first_scene_camera_eye_coord[1] += (50.0f + 50.0f) / 1500.0f;
		first_scene_camera_eye_coord[2] += (165.0f - 100.0f) / 1500.0f;
		first_scene_camera_center_coord[1] += (70.0f + 30.0f) / 1500.0f;
	}
	else				// BOTTOM TO TOP DONE
	{
		firstSceneBottomUpDone = true;


		if (FirstImageDone == false &&
			first_scene_camera_eye_coord[0] > -80.0f && first_scene_camera_eye_coord[1] > 10.0f && first_scene_camera_eye_coord[2] > 5.0f &&
			first_scene_camera_center_coord[0] > -80.0f && first_scene_camera_center_coord[1] > 5.0f
			)
		{
			first_scene_camera_eye_coord[0] -= (80.0f - 0.0f) / 400.0f;
			first_scene_camera_eye_coord[1] -= (50.0f - 10.0f) / 400.0f;
			first_scene_camera_eye_coord[2] -= (165.0f - 5.0f) / 400.0f;

			first_scene_camera_center_coord[0] -= (80.0f - 0.0f) / 400.0f;
			first_scene_camera_center_coord[1] -= (30.0f - 5.0f) / 400.0f;


		}
		else				// FIRST IMAGE DONE
		{
			FirstImageDone = true;

			if (SecondImageDone == false &&
				first_scene_camera_eye_coord[0] < -45.0f &&
				first_scene_camera_center_coord[0] < -45.0f && first_scene_camera_center_coord[1] > 2.0f
				)
			{
				first_scene_camera_eye_coord[0] += (80.0f - 45.0f) / 700.0f;

				first_scene_camera_center_coord[0] += (80.0f - 45.0f) / 700.0f;
				first_scene_camera_center_coord[1] -= (5.0f - 2.0f) / 700.0f;
			}
			else               // SECOND IMAGE DONE
			{
				SecondImageDone = true;

				if (ThirdImageDone_1 == false &&
					first_scene_camera_eye_coord[0] > -55.0f && first_scene_camera_eye_coord[1] > 1.0f && first_scene_camera_eye_coord[2] < 32.0f &&
					first_scene_camera_center_coord[0] > -70.0f && first_scene_camera_center_coord[1] > -80.0f
					)
				{
					first_scene_camera_eye_coord[0] -= (55.0f - 45.0f) / 500.0f;
					first_scene_camera_eye_coord[1] -= (10.0f - 1.0f) / 500.0f;
					first_scene_camera_eye_coord[2] += (32.0f - 5.0f) / 500.0f;


					first_scene_camera_center_coord[0] -= (70.0f - 45.0f) / 500.0f;
					first_scene_camera_center_coord[1] -= (80.0f + 2.0f) / 500.0f;
				}
				else           // THIRD IMAGE DONE
				{
					ThirdImageDone_1 = true;

					if (ThirdImageDone_2 == false &&
						first_scene_camera_center_coord[0] < -34.0f
						)
					{
						first_scene_camera_center_coord[0] += (70.0f - 34.0f) / 700.0f;
					}
					else     // THIRD V2 IMAGE DONE
					{
						ThirdImageDone_2 = true;

						if (ForthImageDone == false &&
							first_scene_camera_eye_coord[0] < 74.0f && first_scene_camera_eye_coord[1] < 6.0f && first_scene_camera_eye_coord[2] > 12.0f &&
							first_scene_camera_center_coord[0] < 73.0f && first_scene_camera_center_coord[1] < -1.0f
							)
						{
							first_scene_camera_eye_coord[0] += (74.0f + 55.0f) / 100.0f;
							first_scene_camera_eye_coord[1] += (6.0f - 1.0f) / 100.0f;
							first_scene_camera_eye_coord[2] -= (32.0f - 12.0f) / 100.0f;

							first_scene_camera_center_coord[0] += (73.0f + 34.0f) / 100.0f;
							first_scene_camera_center_coord[1] += (80.0f - 1.0f) / 100.0f;
						}
						else   // FORTH IMAGE DONE
						{
							ForthImageDone = true;

							if (FifthImageDone == false &&
								first_scene_camera_eye_coord[0] > 51.0f && first_scene_camera_eye_coord[2] < 15.0f &&
								first_scene_camera_center_coord[0] > 55.0f && first_scene_camera_center_coord[1] > -10.0f
								)
							{
								first_scene_camera_eye_coord[0] -= (74.0f - 51.0f) / 2000.0f;
								first_scene_camera_eye_coord[2] += (15.0f - 12.0f) / 2000.0f;

								first_scene_camera_center_coord[0] -= (73.0f - 55.0f) / 2000.0f;
								first_scene_camera_center_coord[1] -= (10.0f - 1.0f) / 2000.0f;
							}
							else
							{
								FifthImageDone = true;  // FIFTH IMAGE DONE


								if (SixthImageDone == false &&
									first_scene_camera_eye_coord[0] < 84.0f && first_scene_camera_eye_coord[1] > 3.0f && first_scene_camera_eye_coord[2] < 38.0f &&
									first_scene_camera_center_coord[0] < 85.0f && first_scene_camera_center_coord[1] > -38.0f
									)
								{
									first_scene_camera_eye_coord[0] += (84.0f - 51.0f) / 500.0f;
									first_scene_camera_eye_coord[1] -= (6.0f - 3.0f) / 500.0f;
									first_scene_camera_eye_coord[2] += (38.0f - 15.0f) / 500.0f;

									first_scene_camera_center_coord[0] += (85.0f - 55.0f) / 500.0f;
									first_scene_camera_center_coord[1] -= (38.0f - 10.0f) / 500.0f;
								}
								else
								{
									SixthImageDone = true; // SIXTH IMAGE DONE

									if (SevenImageDone == false &&
										first_scene_camera_eye_coord[0] > 46.0f &&
										first_scene_camera_center_coord[0] > 43.0f && first_scene_camera_center_coord[1] > -54.0f
										)
									{
										first_scene_camera_eye_coord[0] -= (84.0f - 46.0f) / 700.0f;

										first_scene_camera_center_coord[0] -= (85.0f - 43.0f) / 700.0f;
										first_scene_camera_center_coord[1] -= (54.0f - 38.0f) / 700.0f;
									}
									else
									{
										SevenImageDone = true;  // SEVEN IMAGE DONE

										if (ShowCandleDone == false &&
											first_scene_camera_eye_coord[0] > 0.0f && first_scene_camera_eye_coord[1] > -1.0f && first_scene_camera_eye_coord[2] > 7.0f &&
											first_scene_camera_center_coord[0] > 0.0f && first_scene_camera_center_coord[1] < -2.0f
											)
										{
											first_scene_camera_eye_coord[0] -= (46.0f - 0.0f) / 200.0f;
											first_scene_camera_eye_coord[1] -= (3.f - 1.0f) / 200.0f;
											first_scene_camera_eye_coord[2] -= (38.0f - 7.0f) / 200.0f;

											first_scene_camera_center_coord[0] -= (43.0f - 0.0f) / 200.0f;
											first_scene_camera_center_coord[1] += (54.0f - 2.0f) / 200.0f;
										}
										else
										{
											ShowCandleDone = true;
											temp_counter_for_candleShow = temp_counter_for_candleShow + 0.01f;
											if (temp_counter_for_candleShow > 19.8f && LastImageDone == false &&
												first_scene_camera_eye_coord[0] < 10.0f && first_scene_camera_eye_coord[1] > -2.0f && first_scene_camera_eye_coord[2] < 24.0f &&
												first_scene_camera_center_coord[0] < 10.0f && first_scene_camera_center_coord[1] > -28.0f
												)
											{
												first_scene_camera_eye_coord[0] += (10.0f - 0.0f) / 200.0f;
												first_scene_camera_eye_coord[1] -= (2.0f - 1.0f) / 200.0f;
												first_scene_camera_eye_coord[2] += (24.0f - 7.0f) / 200.0f;

												first_scene_camera_center_coord[0] += (10.0f - 0.0f) / 200.0f;
												first_scene_camera_center_coord[1] -= (28.0f - 2.0f) / 200.0f;
											}
											else
											{
												if (temp_counter_for_candleShow > 20.0f)
												{
													LastImageDone = true;
													isShowStartingScene = false;
												}
											}

										}
									}

								}

							}

						}
					}
				}
			}

		}

	}
}
