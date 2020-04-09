#define gNumPointLights_pointLight_FirstScene  1

GLuint gWoodTexture_FirstScene;

mat4 viewMatrix_for_firstScene			= mat4::identity();							// we are going to use this, only here. so no neet to declare anywhere else

//vec3 first_scene_camera_eye_coord		= { 0.0f,0.0f,5.0f };
vec3 first_scene_camera_eye_coord		= { 0.0f,-50.0f,100.0f };
vec3 first_scene_camera_center_coord	= { 0.0f,-70.0f,0.0f };
vec3 first_scene_camera_up_coord		= { 0.0f,1.0f,0.0f }; 

bool firstSceneBottomUpDone = false;
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

GLuint texture1_book;



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

	glEnable(GL_TEXTURE_2D);

	bool status = LoadGLTextures_fire(&texture1_book, MAKEINTRESOURCE(105));
	if (status == FALSE)
	{
		fprintf(gpFile, "LoadGLTextures_fire Failed in FirstScene\n");
	}

	status = LoadGLTextures_fire(&gWoodTexture_FirstScene, MAKEINTRESOURCE(104));
	if (status == FALSE)
	{
		fprintf(gpFile, "LoadGLTextures_fire Failed in FirstScene\n");
	}

}

float t_fire_FirstScene = 0.0f;
//vec3 positionLamp_FirstScene[] = { vec3(0.0f, 0.0f, 0.0f), vec3(0.5f, -0.5f, 0.0f) };

vec3 positionLamp_FirstScene[] = { vec3(-20.0f, 30.0f, 0.0f), vec3(0.5f, -0.5f, 0.0f) };
//vec3 positionLamp_FirstScene[] = { vec3(-25.0f, -3.0f, -10.0f), vec3(0.0f, 10.0f, -10.0f) };


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
		scaleMatrix = vmath::scale(50.0f, 2.0f, 30.0f);
		modelMatrix = modelMatrix * scaleMatrix;

		glUniformMatrix4fv(gModelMatrixUniform_pointLight, 1, GL_FALSE, modelMatrix);
		glUniformMatrix4fv(gProjectionMatrixUniform_pointLight, 1, GL_FALSE, gPerspectiveProjectionMatrix);
		glUniformMatrix4fv(gViewMatrixUniform_pointLight, 1, GL_FALSE, viewMatrix_for_firstScene);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, gWoodTexture_FirstScene);
		glUniform1i(gTextureSamplerUniform_pointLight, 0);
		glUniform1i(gTextureActiveUniform_pointLight, 1);
		//glUniform1i(samplerUniform1_book, 0);

		drawCubeShape();

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		glUniform1i(applyBloomUniform_pointLight, 0);
		modelMatrix = mat4::identity();
		scaleMatrix = mat4::identity();
		rotateMatrix = mat4::identity();

		modelMatrix = vmath::translate(10.0f, -17.6f, 10.0f);
		scaleMatrix = vmath::scale(25.0f, 15.0f, 1.0f);
		rotateMatrix = rotate(90.0f, -1.0f, 0.0f, 0.0f);
		modelMatrix = modelMatrix * rotateMatrix * scaleMatrix;

		glUniformMatrix4fv(gModelMatrixUniform_pointLight, 1, GL_FALSE, modelMatrix);
		glUniformMatrix4fv(gViewMatrixUniform_pointLight, 1, GL_FALSE, viewMatrix_for_firstScene);								    // globally camera set in perFrag file
		glUniformMatrix4fv(gProjectionMatrixUniform_pointLight, 1, GL_FALSE, gPerspectiveProjectionMatrix);			// globally pojection set

		glUniform1i(gTextureActiveUniform_pointLight, 1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1_book);
		glUniform1i(gTextureSamplerUniform_pointLight, 0);

		drawQuadShape();
		// bind with vao
		/*glBindVertexArray(vao_rectangle_book);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
		glBindVertexArray(0);*/

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		glUniform1i(applyBloomUniform_pointLight, 0);
		//candle model
		modelMatrix		= mat4::identity();
		scaleMatrix		= mat4::identity();
		rotateMatrix	= mat4::identity();

		//modelMatrix = vmath::translate(-20.0f, -16.0f, -90.0f);
		modelMatrix = vmath::translate(-25.0f, -20.0f, -10.0f);
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


	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		glUseProgram(gShaderProgramObject_fire);

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
		modelMatrix_fire = vmath::translate(-25.5f, -3.0f, -10.0f);
		scaleMatrix_fire = vmath::scale(0.1f, 2.0f, 0.1f);
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

		updteForFirstScene();
}

void updteForFirstScene()
{
	// here we should write 1st scene camera movement
	if (first_scene_camera_eye_coord[1] < 40.0f && first_scene_camera_center_coord[1] < 0.0f && firstSceneBottomUpDone == false)
	{
		first_scene_camera_eye_coord[1] += (40.0f + 50.0f) / 500.0f;
		first_scene_camera_center_coord[1] += 70.0f / 500.0f;
	}
	else
	{
		firstSceneBottomUpDone = true;
	}
	if (firstSceneBottomUpDone == true && first_scene_camera_eye_coord[2] > 60.0f && first_scene_camera_center_coord[1] > -5.0f && endOfFirstScene == false)
	{
		first_scene_camera_eye_coord[2] -= (100.0f - 60.0f) / 500.0f;
		first_scene_camera_center_coord[1] -= 5.0f / 500.0f;
	}
	else
	{
		endOfFirstScene = true;
	}
	//if (endOfFirstScene == true && firstSceneBottomUpDone == true && 1
	//	//first_scene_camera_eye_coord[1]		>  10.0f	&& first_scene_camera_eye_coord[2]		>  9.5f &&
	//	//first_scene_camera_center_coord[1]	> -15.0f	&& first_scene_camera_center_coord[2]	<  9.7f &&
	//	//first_scene_camera_up_coord[1]		>  0.0f		&& first_scene_camera_up_coord[2]		> -1.0f
	//	)
	//{
	//	
	//	first_scene_camera_eye_coord[0]		=	0.0f;//{ 0.0f,-50.0f,100.0f };
	//	first_scene_camera_eye_coord[1]		=	10.0f;//{ 0.0f,-50.0f,100.0f };
	//	first_scene_camera_eye_coord[2]		=	9.5f;//{ 0.0f,-50.0f,100.0f };
	//	first_scene_camera_center_coord[0]	=	0.0f;//{ 0.0f,-70.0f,0.0f };
	//	first_scene_camera_center_coord[1]	=	-15.0f;//{ 0.0f,-70.0f,0.0f };
	//	first_scene_camera_center_coord[2]	=	9.7f;//{ 0.0f,-70.0f,0.0f };
	//	first_scene_camera_up_coord[0]		=	0.0f;//{ 0.0f,1.0f,0.0f };
	//	first_scene_camera_up_coord[1]		=	0.0f;//{ 0.0f,1.0f,0.0f };
	//	first_scene_camera_up_coord[2]		=	-1.0f;//{ 0.0f,1.0f,0.0f };
	//	/*
	//	first_scene_camera_eye_coord[1] -= (40.0f - 10.0f) / 100.0f;
	//	first_scene_camera_eye_coord[2] -= (60.0f - 9.5f) / 100.0f;

	//	first_scene_camera_center_coord[1] -= (15.0f - 5.0f) / 100.0f;
	//	first_scene_camera_center_coord[2] += (9.7f - 0.0f) / 100.0f;

	//	first_scene_camera_up_coord[1] -= 1.0f / 100.0f;
	//	first_scene_camera_up_coord[2] -= 1.0f / 100.0f;
	//	*/
	//}

}
