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



GLfloat temp_counter_for_candleShow = 0.0f;
bool ShowCandleDone = false;

/*********************************************************/
GLuint gVertexShaderObject_pointLight_FirstScene;
GLuint gFragmentShaderObject_pointLight_FirstScene;
GLuint gShaderProgramObject_pointLight_FirstScene;


GLuint gModelMatrixUniform_pointLight_FirstScene;
GLuint gViewMatrixUniform_pointLight_FirstScene;
GLuint gProjectionMatrixUniform_pointLight_FirstScene;
GLuint gLdUniform_pointLight_FirstScene, gLaUniform_pointLight_FirstScene, gLsUniform_pointLight_FirstScene;
GLuint gLightPositionUniform_pointLight_FirstScene;
GLuint gKdUniform_pointLight_FirstScene, gKaUniform_pointLight_FirstScene, gKsUniform_pointLight_FirstScene;

GLuint gLKeyPressedUniform_pointLight_FirstScene, gTextureActiveUniform_pointLight_FirstScene;
GLuint gAlphaUniform_pointLight_FirstScene;
GLuint gViewPosUniform_pointLight_FirstScene, gNumPointLightsUniform_pointLight_FirstScene;
GLuint applyBloomUniform_pointLight_FirstScene, u_bloom_is_activeUniform_pointLight_FirstScene;
GLuint bloom_thresh_minUniform_pointLight_FirstScene, bloom_thresh_maxUniform_pointLight_FirstScene;
GLuint fadeinFactorUniform_pointLight_FirstScene, fadeoutFactorUniform_pointLight_FirstScene;
GLuint depthMapSamplerUniform_pointLight_FirstScene, farPlaneUniform_pointLight_FirstScene, shadowsUniform_pointLight_FirstScene;
GLuint isBumpTexturePresentUniform_pointLight_FirstScene;

//#define gNumPointLights_pointLight_FirstScene  20
struct PointLightUniform_FirstScene
{
	GLuint u_La;
	GLuint u_Ld;
	GLuint u_Ls;
	GLuint u_constant;
	GLuint u_linear;
	GLuint u_quadratic;
	GLuint position;
	GLuint DiffuseIntensity;
};
PointLightUniform_FirstScene m_pointLightsLocation_FirstScene[gNumPointLights_pointLight_FirstScene];

struct MaterialUniform_FirstScene
{
	GLuint diffuseSampler;
	GLuint normalSampler;
	GLuint specularSampler;
	GLuint shininess;
};
MaterialUniform_FirstScene m_material_FirstScene;
/**********************************************************/

float near_plane = 0.1f;
float far_plane = 150.0f;
vec3 lightPos;


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
	void initShadowDepthShader();

	initPointLightShader_FirstScene();
	initShadowDepthShader();
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

//vec3 positionLamp_FirstScene[] = { vec3(0.0f, 50.0f, 20.0f), vec3(0.0f, -0.5f, 0.0f) };
vec3 positionLamp_FirstScene[] = { vec3(-0.2f, -0.5f, -20.0f) };
//0.0f, -20.0f, -30.0f

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

	glUniformMatrix4fv(gModelMatrixUniform_pointLight_FirstScene, 1, GL_FALSE, modelMatrix);
	glUniform1i(applyBloomUniform_pointLight_FirstScene, isBloom);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_id);
	
	glBindVertexArray(vao_rectangle_book);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	glBindVertexArray(0);

	glBindTexture(GL_TEXTURE_2D, 0);
}

void renderModels_FirstScene(GLuint& NewModel)
{
	mat4 modelMatrix = mat4::identity();
	mat4 scaleMatrix = mat4::identity();
	mat4 rotateMatrix = mat4::identity();


	modelMatrix = mat4::identity();
	scaleMatrix = mat4::identity();

	//modelMatrix = vmath::translate(0.0f, -20.0f, -100.0f);
	modelMatrix = vmath::translate(0.0f, -20.0f, 0.0f);
	scaleMatrix = vmath::scale(100.0f, 2.0f, 40.0f);
	modelMatrix = modelMatrix * scaleMatrix;
	
	//glUniformMatrix4fv(NewModel, 1, GL_FALSE, modelMatrix);
	glUniformMatrix4fv(gModelMatrixUniform_pointLight_FirstScene, 1, GL_FALSE, modelMatrix);

	glUniform1i(isBumpTexturePresentUniform_pointLight_FirstScene, 0);

	drawCubeShape();


	draw_image(0, 10.0f, -17.6f, 10.0f, 0.0f, 1.0f, texture0_book);

	// image 1
	draw_image(0, -80.0f, -17.6f, -20.0f, 10.0f, 1.0f, texture1_book);

	// image 2
	draw_image(0, -30.0f, -17.6f, -5.0f, 5.0f, -1.0f, texture2_book);

	// image 3.1,3.2,3.3
	draw_image(0, -80.0f, -17.6f, 25.0f, 10.0f, 1.0f, texture33_book);
	draw_image(0, -50.0f, -17.55f, 15.0f, 5.0f, -1.0f, texture31_book);
	draw_image(0, -25.0f, -17.5f, 30.0f, 5.0f, 1.0f, texture32_book);

	// image 4
	draw_image(0, 70.0f, -17.6f, -25.0f, 10.0f, 1.0f, texture4_book);

	// image 5
	draw_image(0, 50.0f, -17.55f, 0.0f, 5.0f, -1.0f, texture5_book);

	// image 6
	draw_image(0, 80.0f, -17.6f, 25.0f, 10.0f, 1.0f, texture6_book);

	// image 7
	draw_image(0, 40.0f, -17.55f, 30.0f, 5.0f, 1.0f, texture7_book);


	modelMatrix = mat4::identity();
	scaleMatrix = mat4::identity();
	rotateMatrix = mat4::identity();

	//modelMatrix = vmath::translate(-20.0f, -16.0f, -90.0f);
	//modelMatrix = vmath::translate(-25.0f, -20.0f, -10.0f);
	modelMatrix = vmath::translate(0.0f, -20.0f, -30.0f);
	scaleMatrix = scale(3.0f, 3.0f, 3.0f);
	rotateMatrix = rotate(90.0f, 0.0f, 1.0f, 0.0f);

	modelMatrix = modelMatrix * scaleMatrix;
	modelMatrix = modelMatrix * rotateMatrix;

	glUniformMatrix4fv(NewModel, 1, GL_FALSE, modelMatrix);

	glUniform1i(applyBloomUniform_pointLight_FirstScene, 0);

	glUniform1i(isBumpTexturePresentUniform_pointLight_FirstScene, 0);

	glBindVertexArray(gModel_Candle.Vao);
	for (int i = 0; i < gModel_Candle.model_mesh_data.size(); i++)
	{
		if (gbLight == true)
		{
			glUniform3fv(gKaUniform_pointLight_FirstScene, 1, gModel_Candle.model_material[gModel_Candle.model_mesh_data[i].material_index].Ka);
			glUniform3fv(gKdUniform_pointLight_FirstScene, 1, gModel_Candle.model_material[gModel_Candle.model_mesh_data[i].material_index].Kd);
			glUniform3fv(gKsUniform_pointLight_FirstScene, 1, gModel_Candle.model_material[gModel_Candle.model_mesh_data[i].material_index].Ks);
			//glUniform1f(m_material_FirstScene.shininess, gModel_Candle.model_material[gModel_Candle.model_mesh_data[i].material_index].Ns);
			glUniform1f(gAlphaUniform_pointLight_FirstScene, gModel_Candle.model_material[gModel_Candle.model_mesh_data[i].material_index].d);
			if (gModel_Candle.model_material[gModel_Candle.model_mesh_data[i].material_index].ismap_Kd == true)
			{
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, gModel_Candle.model_material[gModel_Candle.model_mesh_data[i].material_index].gTexture_diffuse);
			
				glUniform1i(gTextureActiveUniform_pointLight_FirstScene, 1);
				
			}
			if (gModel_Candle.model_material[gModel_Candle.model_mesh_data[i].material_index].ismap_Bump == true)
			{
				glActiveTexture(GL_TEXTURE2);
				glBindTexture(GL_TEXTURE_2D, gModel_Candle.model_material[gModel_Candle.model_mesh_data[i].material_index].gTexture_bump);
			}
			else
				glUniform1i(gTextureActiveUniform_pointLight_FirstScene, 0);

			glBindTexture(GL_TEXTURE_2D, 0);
		}
		glDrawArrays(GL_TRIANGLES, gModel_Candle.model_mesh_data[i].vertex_Index, gModel_Candle.model_mesh_data[i].vertex_Count);
	}
	glBindVertexArray(0);

	glUniform1i(applyBloomUniform_pointLight_FirstScene, 0);
	glUniform1i(isBumpTexturePresentUniform_pointLight_FirstScene, 0);



	for (int i = 0; i < gNumPointLights_pointLight_FirstScene; i++)
	{
		modelMatrix = mat4::identity();
		scaleMatrix = mat4::identity();
		rotateMatrix = mat4::identity();

		modelMatrix = vmath::translate(positionLamp_FirstScene[i]);
	//	scaleMatrix = scale(0.4f, 0.4f, 0.4f);

		modelMatrix = modelMatrix * scaleMatrix;

		glUniformMatrix4fv(NewModel, 1, GL_FALSE, modelMatrix);

		drawCubeShape();					// small quad in code
	}

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

				pointLight[i].position = positionLamp_FirstScene[i];
		}
	
	
		glUseProgram(gShaderProgramObject_pointLight_FirstScene);

		if (isShowStartingScene == false)
		{
			if (FadeOutFactor_pointLightFirstScene >= 0.0f)
				FadeOutFactor_pointLightFirstScene -= 0.01f;
			else
				bDoneFadeOutFirstScene = true;
		}
		glUniform1f(fadeoutFactorUniform_pointLight_FirstScene, FadeOutFactor_pointLightFirstScene);
		glUniform1f(fadeinFactorUniform_pointLight_FirstScene, 1.0f);

		glUniform1i(u_bloom_is_activeUniform_pointLight_FirstScene, 1);
		glUniform1f(bloom_thresh_minUniform_pointLight_FirstScene, bloom_thresh_min);
		glUniform1f(bloom_thresh_maxUniform_pointLight_FirstScene, bloom_thresh_max);
		glUniform1i(applyBloomUniform_pointLight_FirstScene, 0);

		glUniform3fv(gViewPosUniform_pointLight_FirstScene, 1, first_scene_camera_eye_coord);
		if (gbLight == true)
		{
			glUniform1i(gLKeyPressedUniform_pointLight_FirstScene, 1);

			glUniform3fv(gLdUniform_pointLight_FirstScene, 1, lightDiffuse);//white
			glUniform3fv(gLaUniform_pointLight_FirstScene, 1, lightAmbient);
			glUniform3fv(gLsUniform_pointLight_FirstScene, 1, lightSpecular);
			glUniform4fv(gLightPositionUniform_pointLight_FirstScene, 1, lightPosition);

				//pointLight
			glUniform1i(gNumPointLightsUniform_pointLight_FirstScene, gNumPointLights_pointLight_FirstScene);
			for (int i = 0; i < gNumPointLights_pointLight_FirstScene; i++)
			{
				glUniform3fv(m_pointLightsLocation_FirstScene[i].u_La, 1, pointLight[i].u_La);
				glUniform3fv(m_pointLightsLocation_FirstScene[i].u_Ls, 1, pointLight[i].u_Ls);
				glUniform3fv(m_pointLightsLocation_FirstScene[i].u_Ld, 1, pointLight[i].u_Ld);
			

				glUniform3fv(m_pointLightsLocation_FirstScene[i].position, 1, pointLight[i].position);
				glUniform1f(m_pointLightsLocation_FirstScene[i].u_constant, pointLight[i].u_constant);
				glUniform1f(m_pointLightsLocation_FirstScene[i].u_linear, pointLight[i].u_linear);
				glUniform1f(m_pointLightsLocation_FirstScene[i].u_quadratic, pointLight[i].u_quadratic);
			}

			glUniform1i(m_material_FirstScene.diffuseSampler, 0);
			glUniform1i(m_material_FirstScene.normalSampler, 2);
			glUniform1f(m_material_FirstScene.shininess, 128.0f);
			glUniform3fv(gKdUniform_pointLight_FirstScene, 1, material_diffuse);
			glUniform3fv(gKaUniform_pointLight_FirstScene, 1, material_ambient);
			glUniform3fv(gKsUniform_pointLight_FirstScene, 1, material_specular);
			

		}
		else
		{
			glUniform1i(gLKeyPressedUniform_pointLight_FirstScene, 0);
		}

		
		glUniform1f(farPlaneUniform_pointLight_FirstScene, far_plane);
		glUniform1i(shadowsUniform_pointLight_FirstScene, 1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_CUBE_MAP, shadowMap_texCubeMapAttachment);
		glUniform1i(depthMapSamplerUniform_pointLight_FirstScene, 1);

		glUniform1i(applyBloomUniform_pointLight_FirstScene, 0);

	
		glUniformMatrix4fv(gProjectionMatrixUniform_pointLight_FirstScene, 1, GL_FALSE, gPerspectiveProjectionMatrix);
		glUniformMatrix4fv(gViewMatrixUniform_pointLight_FirstScene, 1, GL_FALSE, viewMatrix_for_firstScene);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, gWoodTexture_FirstScene);
		
		renderModels_FirstScene(gModelMatrixUniform_pointLight_FirstScene);

		
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
		modelMatrix_fire = vmath::translate(-0.1f, -5.8f, -20.0f);
		scaleMatrix_fire = vmath::scale(1.0f, 3.5f, 0.5f);
		//scaleMatrix_fire	= vmath::scale(50.0f, 50.0f, 100.0f);
		//rotateMatrix_fire = rotate(75.0f, 0.0f, 1.0f, 0.0f);

		modelMatrix_fire = modelMatrix_fire * rotateMatrix_fire * scaleMatrix_fire;

		glUniformMatrix4fv(gModelMatrixUniform_fire, 1, GL_FALSE, modelMatrix_fire);
		glUniformMatrix4fv(gViewMatrixUniform_fire, 1, GL_FALSE, viewMatrix_for_firstScene);
		glUniformMatrix4fv(gProjectionMatrixUniform_fire, 1, GL_FALSE, gPerspectiveProjectionMatrix);

		glUniform1f(timeUniform_fire, t_fire_FirstScene);

		//glPointSize(gWidth_fire / 10);
		//glPointSize(50);
		glPointSize(25);
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

		if (startAnimation == true)
		{
			updteForFirstScene();
		}
}


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
											first_scene_camera_eye_coord[0] > 0.0f && first_scene_camera_eye_coord[1] > -12.0f && first_scene_camera_eye_coord[2] > 20.0f &&
											first_scene_camera_center_coord[0] > 0.0f && first_scene_camera_center_coord[1] < -5.0f
											)
										{
											first_scene_camera_eye_coord[0] -= (46.0f - 0.0f) / 200.0f;
											first_scene_camera_eye_coord[1] -= (12.f - 3.0f) / 200.0f;
											first_scene_camera_eye_coord[2] -= (38.0f - 20.0f) / 200.0f;

											first_scene_camera_center_coord[0] -= (43.0f - 0.0f) / 200.0f;
											first_scene_camera_center_coord[1] += (54.0f - 5.0f) / 200.0f;
										}
										else
										{
											ShowCandleDone = true;
											temp_counter_for_candleShow = temp_counter_for_candleShow + 0.01f;
											if (temp_counter_for_candleShow > 19.8f && LastImageDone == false &&
												first_scene_camera_eye_coord[0] < 10.0f &&  first_scene_camera_eye_coord[2] < 24.0f &&
												first_scene_camera_center_coord[0] < 10.0f && first_scene_camera_center_coord[1] > -30.0f
												)
											{
												first_scene_camera_eye_coord[0] += (10.0f - 0.0f) / 200.0f;
												//first_scene_camera_eye_coord[1] -= (2.0f - 1.0f) / 200.0f;
												first_scene_camera_eye_coord[2] += (24.0f - 20.0f) / 200.0f;

												first_scene_camera_center_coord[0] += (10.0f - 0.0f) / 200.0f;
												first_scene_camera_center_coord[1] -= (30.0f - 5.0f) / 200.0f;
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



void initPointLightShader_FirstScene()
{
	void uninitialize(int);

	//VERTEX SHADER
	gVertexShaderObject_pointLight_FirstScene = glCreateShader(GL_VERTEX_SHADER);

	const GLchar* vertextShaderSourceCode_FirstScene =
		"#version 400" \
		"\n" \

		"struct PointLight" \
		"{" \
		"vec3 u_La;" \
		"vec3 u_Ld;" \
		"vec3 u_Ls;" \
		"float u_constant;" \
		"float u_linear;" \
		"float u_quadratic;" \
		"vec3 position;" \

		"};" \
		"uniform PointLight pointLight[1];" \
		"uniform vec3 viewPos;" \

		"in vec4 vPosition;" \
		"in vec3 vNormal;" \
		"in vec2 vTexcoord;" \

		"in vec3 vTangent;" \
		"in vec3 vBitangent;" \

		"uniform mat4 u_model_matrix;" \
		"uniform mat4 u_view_matrix;" \
		"uniform mat4 u_projection_matrix;" \

		"uniform int u_LKeyPressed;" \


		"out vec3 fragment_position;" \
		"out vec3 fragEyeSpace_position;" \
		"out vec2 out_texcord;" \
		
		//BumpMap
		"out vec3 eyeDir;" \
		"out vec3 lightDir;" \

		//Normal PointLight
		"out vec3 Normal;" \

		"uniform int isBumpTexturePresent;" \

		"void main(void)" \
		"{" \
		"if(isBumpTexturePresent == 1)" \
		"{" \
			//calculate ViewSpace coordinate
			"vec4 P = u_view_matrix * u_model_matrix * vPosition;" \
			//Normal in viewSpace
			"vec3 N = normalize(mat3(u_view_matrix * u_model_matrix) * vNormal);" \
			"vec3 T = normalize(mat3(u_view_matrix * u_model_matrix) * vTangent);" \
		
			"vec3 B = cross(N, T);" \

			//calculate light vector
			"vec3 L = pointLight[0].position - P.xyz;" \
			//calculate view vector
			"vec3 V = -P.xyz;" \
		
			"lightDir = normalize(vec3(dot(V, T), dot(V, B), dot(V, N)));" \

			"eyeDir = normalize(vec3(dot(V, T), dot(V, B), dot(V, N)));" \
		"}" \
		"else" \
		"{" \
			"Normal = transpose(inverse(mat3(u_model_matrix))) * vNormal;" \
			
		"}" \

		"out_texcord = vTexcoord;" \

		"gl_Position = u_projection_matrix * u_view_matrix * u_model_matrix * vPosition;" \
		"fragment_position = vec3(u_model_matrix * vPosition);" \
		"fragEyeSpace_position = vec3(u_view_matrix * u_model_matrix * vPosition);" \

		"}";

	glShaderSource(gVertexShaderObject_pointLight_FirstScene, 1, (const GLchar**)&vertextShaderSourceCode_FirstScene, NULL);

	//compile shader
	glCompileShader(gVertexShaderObject_pointLight_FirstScene);

	// Error checking
	checkCompilationLog((char*)"gVertexShaderObject_pointLight_FirstScene", gVertexShaderObject_pointLight_FirstScene);


	//FRAGMENT SHADER

	gFragmentShaderObject_pointLight_FirstScene = glCreateShader(GL_FRAGMENT_SHADER);

	const GLchar* fragmentShaderSourceCode_FirstScene =
		"#version 400" \
		"\n" \
		"struct Material" \
		"{" \
		"sampler2D diffuseMap;" \
		"sampler2D normalMap;" \

		/*"sampler2D specular;" \*/

		"float shininess;" \
		"};" \

		"struct PointLight" \
		"{" \
		"vec3 u_La;" \
		"vec3 u_Ld;" \
		"vec3 u_Ls;" \
		"float u_constant;" \
		"float u_linear;" \
		"float u_quadratic;" \
		"vec3 position;" \
		/*"float diffuseIntensity;" \*/
		"};" \

		"uniform samplerCube depthMap; " \
		"uniform float far_plane;" \
		"uniform bool shadows;" \

		"uniform int gNumPointLights;" \
		"uniform Material material;" \
		"uniform PointLight pointLight[1];" \
		"uniform vec3 viewPos;" \

		"uniform int isBumpTexturePresent;" \

		"uniform int u_is_texture;" \

		"in vec3 fragment_position;" \
		"in vec3 fragEyeSpace_position;" \
		"in vec2 out_texcord;" \
		//Bump
		"in vec3 eyeDir;" \
		"in vec3 lightDir;" \
		//normal
		"in vec3 Normal;" \

		"layout (location = 0) out vec4 FragColor;" \
		"layout (location = 1) out vec4 BloomColor;" \
		"layout (location = 2) out vec4 GodRaysColor;" \

		"uniform int u_LKeyPressed;" \
		
		"uniform float u_alpha;" \

		"uniform int applyBloom;" \
		"uniform float bloom_thresh_min = 0.8f;" \
		"uniform float bloom_thresh_max = 1.2f;" \
		"uniform int u_bloom_is_active;" \

		"uniform float fadeinFactor;" \
		"uniform float fadeoutFactor;" \

		"vec3 gridSamplingDisk[20] = vec3[]" \
		"(" \
		"vec3(1, 1, 1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1, 1,  1), " \
		"vec3(1, 1, -1), vec3(1, -1, -1), vec3(-1, -1, -1), vec3(-1, 1, -1), " \
		"vec3(1, 1, 0), vec3(1, -1, 0), vec3(-1, -1, 0), vec3(-1, 1, 0), " \
		"vec3(1, 0, 1), vec3(-1, 0, 1), vec3(1, 0, -1), vec3(-1, 0, -1), " \
		"vec3(0, 1, 1), vec3(0, -1, 1), vec3(0, -1, -1), vec3(0, 1, -1) " \
		");" \

		"float ShadowCalculation(vec3 fragPos)" \
		"{" \
		"vec3 fragToLight = fragPos - pointLight[0].position;" \
		"float currentDepth = length(fragToLight);" \
		"float shadow = 0.0;" \
		"float bias = 0.15;" \
		"int samples = 20;" \

		"float viewDistance = length(viewPos - fragPos);" \
		"float diskRadius = (1.0 + (viewDistance / far_plane)) / 25.0;" \

		"for(int i = 0; i < samples; ++i)" \
		"{" \
		"float closestDepth = texture(depthMap, fragToLight  + gridSamplingDisk[i] * diskRadius).r;" \
		"closestDepth *= far_plane;" \
		"if(currentDepth - bias > closestDepth)" \
		"shadow += 1.0;" \
		"}" \
		"shadow /= float(samples);" \


		/*"float closestDepth = texture(depthMap, fragToLight).r;" \
		"closestDepth *= far_plane;" \

		"shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;" \*/
		"return shadow;" \
		"}" \

		"vec3 calculatePointLight(int index)" \
		"{" \
		"vec3 pointLightColor;" \

		//ambient
		"vec3 ambient = pointLight[index].u_La * texture(material.diffuseMap, out_texcord).rgb;" \
		"vec3 diffuse = vec3(0.0);" \
		"vec3 specular = vec3(0.0);" \
		"if(isBumpTexturePresent == 1)" \
		"{" \
		"vec3 V = normalize(eyeDir);" \
		"vec3 L = normalize(lightDir);" \
		
		//normalMap
		"vec3 normMapVal = texture(material.normalMap, out_texcord).rgb;" \
		"normMapVal = normalize(normMapVal * 2.0 - vec3(1.0));" \

		//diffuse
		/*"vec3 lightDir = normalize(TangentLightPos - TangentFragPos);" \*/
	
		"vec3 R = reflect(-L, normMapVal);" \
	
		"float diff = max(dot(L, normMapVal), 0.0);" \
		
		"if(u_is_texture == 1)" \
		"{" \
			"diffuse = pointLight[index].u_Ld * diff * texture(material.diffuseMap, out_texcord).rgb;" \
		"}" \
		"else" \
		"{" \
			"diffuse = pointLight[index].u_Ld * diff;" \
		"}" \

		//specular
		/*"vec3 viewDir = normalize(TangentViewPos - TangentFragPos);" \*/
		
		/*"vec3 halfwayDir = normalize(lightDir + viewDir);" \*/
		"float spec = max(pow(dot(R, V), 5.0), 0.0f);" \
		"specular = pointLight[index].u_Ls * spec;" \

		
		"}" \
		"else{" \
			"vec3 normal = normalize(Normal);" \
			"vec3 lightDir = normalize(pointLight[index].position - fragment_position);" \
			"float diff = max(dot(lightDir , normal), 0.0);" \
			"if(u_is_texture == 1)" \
			"{" \
			"diffuse = pointLight[index].u_Ld * diff * texture(material.diffuseMap, out_texcord).rgb;" \
			"}" \
			"else" \
			"{" \
			"diffuse = pointLight[index].u_Ld * diff;" \
			"}" \

			//specular
			"vec3 viewDir = normalize(viewPos - fragment_position);" \
			"vec3 reflectDir = reflect(-lightDir, normal);" \
			"float spec = 0.0;" \
			"vec3 halfwayDir = normalize(lightDir + viewDir);" \
			/*"spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);" \*/
			"spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);" \

			"specular = vec3(0.0, 0.0, 0.0);" \
		"}" \

		//attenuation
		"float distance = length(pointLight[index].position - fragment_position);" \
		"float attenuation = 1.0 / (pointLight[index].u_constant + pointLight[index].u_linear * distance + " \
		"pointLight[index].u_quadratic * (distance * distance));" \

		"ambient *= attenuation;" \
		"diffuse *= attenuation;" \
		"specular *= attenuation;" \

		// calculate shadow
		"float shadow = shadows ? ShadowCalculation(fragment_position) : 0.0;" \

		"pointLightColor = ambient + (1 - shadow) * (diffuse + specular);" \

		"return pointLightColor;" \

		"}" \

		"void main(void)" \
		"{" \
		"vec3 phong_ads_color;" \
	
		"if(u_LKeyPressed==1)" \
		"{" \
	
		"vec3 pointLightColor = vec3(0.0, 0.0, 0.0);" \
		"for(int i = 0; i < gNumPointLights; i++)" \
		"{" \
		"pointLightColor += calculatePointLight(i);" \
		"}" \

		"phong_ads_color = pointLightColor;" \

		"}" \
		"else" \
		"{" \
		"phong_ads_color = vec3(1.0, 1.0, 1.0);" \
		"}" \

		"FragColor = vec4(phong_ads_color, u_alpha) * fadeinFactor * fadeoutFactor;" \


		"if(applyBloom == 1)" \
		"{" \
		"vec4 c = FragColor;" \
		"if (u_bloom_is_active == 1)" \
		"{" \
		"float Y = dot(c, vec4(0.299, 0.587, 0.144, 1.0));\n" \
		"c = c * 4.0 * smoothstep(bloom_thresh_min, bloom_thresh_max, Y);\n" \
		"BloomColor = c;\n" \
		"}" \
		"else" \
		"{" \
		"BloomColor = c;\n" \
		"}" \
		"}" \
		"else" \
		"{" \
		"BloomColor = vec4(0.0);" \
		"}" \
		"GodRaysColor = vec4(0.0, 0.0, 0.0, 0.0);" \
		"}";

	glShaderSource(gFragmentShaderObject_pointLight_FirstScene, 1, (const GLchar**)&fragmentShaderSourceCode_FirstScene, NULL);

	glCompileShader(gFragmentShaderObject_pointLight_FirstScene);

	// Error checking
	checkCompilationLog((char*)"gFragmentShaderObject_pointLight_FirstScene", gFragmentShaderObject_pointLight_FirstScene);


	//Shader Program

	gShaderProgramObject_pointLight_FirstScene = glCreateProgram();

	glAttachShader(gShaderProgramObject_pointLight_FirstScene, gVertexShaderObject_pointLight_FirstScene);

	glAttachShader(gShaderProgramObject_pointLight_FirstScene, gFragmentShaderObject_pointLight_FirstScene);

	glBindAttribLocation(gShaderProgramObject_pointLight_FirstScene, MATRIX_ATTRIBUTE_POSITION, "vPosition");
	glBindAttribLocation(gShaderProgramObject_pointLight_FirstScene, MATRIX_ATTRIBUTE_NORMAL, "vNormal");
	glBindAttribLocation(gShaderProgramObject_pointLight_FirstScene, MATRIX_ATTRIBUTE_TEXTURE0, "vTexcoord");
	glBindAttribLocation(gShaderProgramObject_pointLight_FirstScene, MATRIX_ATTRIBUTE_TANGENT, "vTangent");
	glBindAttribLocation(gShaderProgramObject_pointLight_FirstScene, MATRIX_ATTRIBUTE_BITANGENT, "vBitangent");

	glLinkProgram(gShaderProgramObject_pointLight_FirstScene);

	// Error checking
	checkLinkLog((char*)"gShaderProgramObject_pointLight_FirstScene", gShaderProgramObject_pointLight_FirstScene);


	gModelMatrixUniform_pointLight_FirstScene = glGetUniformLocation(gShaderProgramObject_pointLight_FirstScene, "u_model_matrix");
	gViewMatrixUniform_pointLight_FirstScene = glGetUniformLocation(gShaderProgramObject_pointLight_FirstScene, "u_view_matrix");
	gProjectionMatrixUniform_pointLight_FirstScene = glGetUniformLocation(gShaderProgramObject_pointLight_FirstScene, "u_projection_matrix");

	gLKeyPressedUniform_pointLight_FirstScene = glGetUniformLocation(gShaderProgramObject_pointLight_FirstScene, "u_LKeyPressed");

	gLdUniform_pointLight_FirstScene = glGetUniformLocation(gShaderProgramObject_pointLight_FirstScene, "u_Ld");
	gLaUniform_pointLight_FirstScene = glGetUniformLocation(gShaderProgramObject_pointLight_FirstScene, "u_La");
	gLsUniform_pointLight_FirstScene = glGetUniformLocation(gShaderProgramObject_pointLight_FirstScene, "u_Ls");

	
	gAlphaUniform_pointLight_FirstScene = glGetUniformLocation(gShaderProgramObject_pointLight_FirstScene, "u_alpha");

	gViewPosUniform_pointLight_FirstScene = glGetUniformLocation(gShaderProgramObject_pointLight_FirstScene, "viewPos");
	gNumPointLightsUniform_pointLight_FirstScene = glGetUniformLocation(gShaderProgramObject_pointLight_FirstScene, "gNumPointLights");

	gTextureActiveUniform_pointLight_FirstScene = glGetUniformLocation(gShaderProgramObject_pointLight_FirstScene, "u_is_texture");

	char Name[128];
	for (int i = 0; i < gNumPointLights_pointLight_FirstScene; i++)
	{
		memset(Name, 0, sizeof(Name));

		snprintf(Name, sizeof(Name), "pointLight[%d].u_La", i);
		m_pointLightsLocation_FirstScene[i].u_La = glGetUniformLocation(gShaderProgramObject_pointLight_FirstScene, Name);

		snprintf(Name, sizeof(Name), "pointLight[%d].u_Ld", i);
		m_pointLightsLocation_FirstScene[i].u_Ld = glGetUniformLocation(gShaderProgramObject_pointLight_FirstScene, Name);

		snprintf(Name, sizeof(Name), "pointLight[%d].u_Ls", i);
		m_pointLightsLocation_FirstScene[i].u_Ls = glGetUniformLocation(gShaderProgramObject_pointLight_FirstScene, Name);

		snprintf(Name, sizeof(Name), "pointLight[%d].u_constant", i);
		m_pointLightsLocation_FirstScene[i].u_constant = glGetUniformLocation(gShaderProgramObject_pointLight_FirstScene, Name);

		snprintf(Name, sizeof(Name), "pointLight[%d].u_linear", i);
		m_pointLightsLocation_FirstScene[i].u_linear = glGetUniformLocation(gShaderProgramObject_pointLight_FirstScene, Name);

		snprintf(Name, sizeof(Name), "pointLight[%d].u_quadratic", i);
		m_pointLightsLocation_FirstScene[i].u_quadratic = glGetUniformLocation(gShaderProgramObject_pointLight_FirstScene, Name);

		snprintf(Name, sizeof(Name), "pointLight[%d].position", i);
		m_pointLightsLocation_FirstScene[i].position = glGetUniformLocation(gShaderProgramObject_pointLight_FirstScene, Name);

	}

	memset(Name, 0, sizeof(Name));
	snprintf(Name, sizeof(Name), "material.diffuse");
	m_material_FirstScene.diffuseSampler = glGetUniformLocation(gShaderProgramObject_pointLight_FirstScene, Name);
	
	memset(Name, 0, sizeof(Name));
	snprintf(Name, sizeof(Name), "material.normalMap");
	m_material_FirstScene.normalSampler = glGetUniformLocation(gShaderProgramObject_pointLight_FirstScene, Name);

	memset(Name, 0, sizeof(Name));
	snprintf(Name, sizeof(Name), "material.specular");
	m_material_FirstScene.specularSampler = glGetUniformLocation(gShaderProgramObject_pointLight_FirstScene, Name);
	
	memset(Name, 0, sizeof(Name));
	snprintf(Name, sizeof(Name), "material.shininess");
	m_material_FirstScene.shininess = glGetUniformLocation(gShaderProgramObject_pointLight_FirstScene, Name);

	applyBloomUniform_pointLight_FirstScene = glGetUniformLocation(gShaderProgramObject_pointLight_FirstScene, "applyBloom");
	u_bloom_is_activeUniform_pointLight_FirstScene = glGetUniformLocation(gShaderProgramObject_pointLight_FirstScene, "u_bloom_is_active");
	bloom_thresh_minUniform_pointLight_FirstScene = glGetUniformLocation(gShaderProgramObject_pointLight_FirstScene, "bloom_thresh_min");
	bloom_thresh_maxUniform_pointLight_FirstScene = glGetUniformLocation(gShaderProgramObject_pointLight_FirstScene, "bloom_thresh_max");

	fadeinFactorUniform_pointLight_FirstScene = glGetUniformLocation(gShaderProgramObject_pointLight_FirstScene, "fadeinFactor");
	fadeoutFactorUniform_pointLight_FirstScene = glGetUniformLocation(gShaderProgramObject_pointLight_FirstScene, "fadeoutFactor");

	depthMapSamplerUniform_pointLight_FirstScene = glGetUniformLocation(gShaderProgramObject_pointLight_FirstScene, "depthMap");
	farPlaneUniform_pointLight_FirstScene = glGetUniformLocation(gShaderProgramObject_pointLight_FirstScene, "far_plane");
	shadowsUniform_pointLight_FirstScene = glGetUniformLocation(gShaderProgramObject_pointLight_FirstScene, "shadows");

	isBumpTexturePresentUniform_pointLight_FirstScene = glGetUniformLocation(gShaderProgramObject_pointLight_FirstScene, "isBumpTexturePresent");
}


std::vector<mat4> shadowTransforms;

void renderShadowDepthShader_FirstScene()
{
	mat4 shadowProj = perspective(90.0f, (GLfloat)(MAX_SCENE_WIDTH / 2) / (GLfloat)(MAX_SCENE_HEIGHT / 2), near_plane, far_plane);

	lightPos = positionLamp_FirstScene[0];
	shadowTransforms.push_back(shadowProj * lookat(lightPos, lightPos + vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, -1.0f, 0.0f)));
	shadowTransforms.push_back(shadowProj * lookat(lightPos, lightPos + vec3(-1.0f, 0.0f, 0.0f), vec3(0.0f, -1.0f, 0.0f)));
	shadowTransforms.push_back(shadowProj * lookat(lightPos, lightPos + vec3(0.0f, 1.0f, 0.0f), vec3(0.0f, 0.0f, 1.0f)));
	shadowTransforms.push_back(shadowProj * lookat(lightPos, lightPos + vec3(0.0f, -1.0f, 0.0f), vec3(0.0f, 0.0f, -1.0f)));
	shadowTransforms.push_back(shadowProj * lookat(lightPos, lightPos + vec3(0.0f, 0.0f, 1.0f), vec3(0.0f, -1.0f, 0.0f)));
	shadowTransforms.push_back(shadowProj * lookat(lightPos, lightPos + vec3(0.0f, 0.0f, -1.0f), vec3(0.0f, -1.0f, 0.0f)));

	glViewport(0, 0, MAX_SCENE_WIDTH / 2, MAX_SCENE_HEIGHT / 2);
	glBindFramebuffer(GL_FRAMEBUFFER, shaodowMap_fbo);
	glClear(GL_DEPTH_BUFFER_BIT);
	glUseProgram(gShaderProgramObject_ShadowDepthMap);
	for (unsigned int i = 0; i < 6; i++)
		glUniformMatrix4fv(gShadowMatricesUniform_ShadowDepthMap[i], 1, GL_FALSE, shadowTransforms[i]);

	glUniform1f(gFarPlaneUniform_ShadowDepthMap, far_plane);
	glUniform3fv(gLightPosUniform_ShadowDepthMap, 1, lightPos);

	renderModels_FirstScene(gModelMatrixUniform_ShadowDepthMap);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}