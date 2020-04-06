#pragma once

#include "../../Include/BasicShapeLoader/Model/BasicOBJLoader/Matrix_Obj_Loader.h"

// SHADER 1
GLuint gVertexShaderObject_krishnaAnimate;
GLuint gGeometryShaderObject_krishnaAnimate;
GLuint gFragmentShaderObject_krishnaAnimate;
GLuint gShaderProgramObject_krishnaAnimate;

// SHADER 2
GLuint gGeometryShaderObject_2_krishnaAnimate;
GLuint gFragmentShaderObject_2_krishnaAnimate;
GLuint gShaderProgramObject_2_krishnaAnimate;

// SHADER 3 
// attractor
GLuint gShaderProgramObject_attractor_krishna;
GLuint gVertexShaderObject_attractor_krishna;
GLuint gFragmentShaderObject_attractor_krishna;

// SHADER 4
// morpis
GLuint gShaderProgramObject_mor_pis_krishna;
GLuint gVertexShaderObject_mor_pis_krishna;
GLuint gFragmentShaderObject_mor_pis_krishna;


GLuint gModelMatrixUniform_krishnaAnimate;
GLuint gViewMatrixUniform_krishnaAnimate;
GLuint gProjectionMatrixUniform_krishnaAnimate;
GLuint gLdUniform_krishnaAnimate, gLaUniform_krishnaAnimate, gLsUniform_krishnaAnimate;
GLuint gLightPositionUniform_krishnaAnimate;
GLuint gKdUniform_krishnaAnimate, gKaUniform_krishnaAnimate, gKsUniform_krishnaAnimate;
GLuint gMaterialShininessUniform_krishnaAnimate;
GLuint gLKeyPressedUniform_krishnaAnimate;
GLuint gTextureSamplerUniform_krishnaAnimate, gTextureActiveUniform_krishnaAnimate, gAlphaUniform_krishnaAnimate;
GLuint gTimeUniform_krishnaAnimate;
GLuint gViratRoopStartUniform_krishnaAnimate, gIs_animatedUniform_krishnaAnimate;

GLuint applyBloomUniform_krishnaAnimate, u_bloom_is_activeUniform_krishnaAnimate;
GLuint bloom_thresh_minUniform_krishnaAnimate, bloom_thresh_maxUniform_krishnaAnimate;

bool startJoin_krishnaAnimate = false;

float ftime_krishnaAnimate = 0.0f;

GLfloat lightAmbient_krishnaAnimate[] = { 0.0f,0.0f,0.0f,1.0f };
GLfloat lightDiffuse_krishnaAnimate[] = { 1.0f,1.0f,1.0f,1.0f };
GLfloat lightSpecular_krishnaAnimate[] = { 1.0f,1.0f,1.0f,1.0f };
GLfloat lightPosition_krishnaAnimate[] = { 100.0f,100.0f,100.0f,1.0f };


// animation variable
GLfloat gfKrishnaModelScale = 70.0f;

//
GLuint VaoForAttractor;
GLuint VboForAttractor;


std::vector<float> vertices_attractor;

// for different attaractor
//GLfloat t_for_attractor = 0.02f;
GLfloat t_for_attractor = 0.03f;
//GLfloat Rotate = 0.0f;
GLfloat b_for_attractor = 0.037f;

GLfloat X_Pos_attractor = -250.0f;
GLfloat Y_Pos_attractor = 150.0f;
GLfloat Z_Pos_attractor = -10.0f;

GLuint gModelMatrixUniform_krishnaAttractor;
GLuint gViewMatrixUniform_krishnaAttractor;
GLuint gProjectionMatrixUniform_krishnaAttractor;
GLuint applyBloomUniform_krishnaAttractor, u_bloom_is_activeUniform_krishnaAttractor;
GLuint bloom_thresh_minUniform_krishnaAttractor, bloom_thresh_maxUniform_krishnaAttractor;

Model krishna_Animated_StandUpHand;

bool isKrishnaRenderInPoints = true;


// for mor pis
GLuint vao_rectangle_mor_pis;
GLuint vbo_position_rectangle_mor_pis;
GLuint vbo_texture_rectangle_mor_pis;

GLuint gModelMatrixUniform_mor_pis;
GLuint gViewMatrixUniform_mor_pis;
GLuint gProjectionMatrixUniform_mor_pis;

GLuint samplerUniform1_mor_pis;
GLuint samplerUniform2_mor_pis;

GLuint textureFeather1_mor_pis;
GLuint textureFeather2_mor_pis;

GLfloat X_Pos_mor_pis = -250.0f;



void initKrishnaAnimate()
{
	void uninitialize(int);

	// SHADER PROGRAM OBJECT 1
#pragma region SHADER1
	//VERTEX SHADER
	gVertexShaderObject_krishnaAnimate = glCreateShader(GL_VERTEX_SHADER);

	const GLchar* vertextShaderSourceCode =
		"#version 440" \
		"\n" \
		"in vec4 vPosition;" \
		"in vec3 vNormal;" \
		"in vec2 vTexcoord;" \
		"in vec3 aTangent;" \
		"in vec3 aBitangent;" \
		"in vec4 s_vWeights;" \
		"in ivec4 s_vIDs;" \

		"const int MAX_BONES = 100;" \
		"uniform mat4 gBones[MAX_BONES];" \

		"uniform mat4 u_model_matrix;" \
		"uniform mat4 u_view_matrix;" \
		"uniform mat4 u_projection_matrix;" \
		"uniform vec4 u_light_position;"\
		"uniform int u_LKeyPressed;" \

		"uniform int is_animated;" \

		"out vec3 transformed_normals;" \
		"out vec3 light_direction;" \
		"out vec3 viewer_vector;" \
		"out vec2 out_texcord;" \

		"out vec4 out_gl_pos;" \

		"void main(void)" \
		"{" \
		"mat4 BMatrix;" \
		"BMatrix = gBones[s_vIDs[0]] * s_vWeights[0];" \
		" BMatrix += gBones[s_vIDs[1]] * s_vWeights[1];" \
		" BMatrix += gBones[s_vIDs[2]] * s_vWeights[2];" \
		" BMatrix += gBones[s_vIDs[3]] * s_vWeights[3];" \

		"if(u_LKeyPressed == 1)" \
		"{" \
		"vec4 eye_coordinates	= u_view_matrix * u_model_matrix * vPosition;" \
		"transformed_normals	= mat3(u_view_matrix * u_model_matrix) * vNormal;" \
		"light_direction		= vec3(u_light_position) - eye_coordinates.xyz;" \
		"viewer_vector			= -eye_coordinates.xyz;" \
		"}"\

		"out_texcord	= vTexcoord;" \
		"if(is_animated == 1)" \
		"gl_Position	= u_projection_matrix * u_view_matrix * u_model_matrix * BMatrix * vPosition;" \
		"else" \
		"{" \
		"gl_Position = u_projection_matrix * u_view_matrix * u_model_matrix * vPosition;" \
		"}" \
		"out_gl_pos		= gl_Position;" \
		"}";

	glShaderSource(gVertexShaderObject_krishnaAnimate, 1, (const GLchar**)&vertextShaderSourceCode, NULL);

	//compile shader
	glCompileShader(gVertexShaderObject_krishnaAnimate);

	// Error checking
	checkCompilationLog((char*)"gVertexShaderObject_krishnaAnimate", gVertexShaderObject_krishnaAnimate);


	//FRAGMENT SHADER

	gFragmentShaderObject_krishnaAnimate = glCreateShader(GL_FRAGMENT_SHADER);

	const GLchar* fragmentShaderSourceCode =
		"#version 450" \
		"\n" \
		"in vec3 transformed_normals;" \
		"in vec3 light_direction;" \
		"in vec3 viewer_vector;" \
		"in vec2 out_texcord;" \

		"in vec4 out_gl_pos;" \

		"layout (location = 0) out vec4 FragColor;" \
		"layout (location = 1) out vec4 BloomColor;" \
		"layout (location = 2) out vec4 GodRaysColor;" \

		"uniform vec3 u_La;" \
		"uniform vec3 u_Ld;" \
		"uniform vec3 u_Ls;" \
		"uniform vec3 u_Ka;" \
		"uniform vec3 u_Kd;" \
		"uniform vec3 u_Ks;" \
		"uniform float u_material_shininess;" \
		"uniform int u_LKeyPressed;" \
		"uniform sampler2D u_texture0_sampler;"\
		"uniform int u_is_texture;" \
		"uniform float u_alpha;" \
		"uniform vec3 material_diffuse;" \
		"uniform vec3 material_ambient;" \
		"uniform vec3 material_specular;" \

		"uniform float time;" \
		"uniform int virat_roop_start;" \

		"uniform int applyBloom;" \
		"uniform float bloom_thresh_min = 0.8f;" \
		"uniform float bloom_thresh_max = 1.2f;" \
		"uniform int u_bloom_is_active;" \

		"void main(void)" \
		"{" \
		"vec3 phong_ads_color;" \
		"vec4 Final_Texture;" \
		"if(u_LKeyPressed==1)" \
		"{" \
		"vec3 normalized_transformed_normals	= normalize(transformed_normals);" \
		"vec3 normalized_light_direction		= normalize(light_direction);" \
		"vec3 normalized_viewer_vector			= normalize(viewer_vector);" \
		
		
		"float tn_dot_ld						= max(dot(normalized_transformed_normals, normalized_light_direction),0.0);" \
		
		"vec3 reflection_vector					= reflect(-normalized_light_direction, normalized_transformed_normals);" \
		"vec3 ambient;" \
		"vec3 diffuse;" \
		"vec3 specular;" \

		"ambient							= u_La * u_Ka * material_ambient;" \
		"diffuse							= u_Ld * u_Kd * tn_dot_ld * material_diffuse;" \
		"specular							= u_Ls * u_Ks * pow(max(dot(reflection_vector, normalized_viewer_vector), 0.0), u_material_shininess) * material_specular;" \
		
		"phong_ads_color						= ambient + diffuse + specular;" \
		"}" \
		"else" \
		"{" \
		"phong_ads_color = vec3(1.0, 1.0, 1.0);" \
		"}" \
		"if(u_is_texture == 1)" \
		"{" \
		"Final_Texture	= texture(u_texture0_sampler, out_texcord); " \
		"FragColor		= vec4(phong_ads_color, 1.0f) * Final_Texture;" \
		"}" \
		"else" \
		"{" \
		"FragColor		= vec4(phong_ads_color,1.0f);"\
		"}" \

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
		//"if(virat_roop_start == 1)" \
				"FragColor		= vec4(0.2,0.1,1.0,1.0f);" 

		"if(virat_roop_start == 1 && out_gl_pos.y > time)" \
		"{" \
		"discard;" \
		"}" \


		"}";

	glShaderSource(gFragmentShaderObject_krishnaAnimate, 1, (const GLchar**)&fragmentShaderSourceCode, NULL);
	glCompileShader(gFragmentShaderObject_krishnaAnimate);

	// Error checking
	checkCompilationLog((char*)"gFragmentShaderObject_krishnaAnimate", gFragmentShaderObject_krishnaAnimate);


	//Shader Program

	gShaderProgramObject_krishnaAnimate = glCreateProgram();

	glAttachShader(gShaderProgramObject_krishnaAnimate, gVertexShaderObject_krishnaAnimate);
	glAttachShader(gShaderProgramObject_krishnaAnimate, gFragmentShaderObject_krishnaAnimate);

	glBindAttribLocation(gShaderProgramObject_krishnaAnimate, MATRIX_ATTRIBUTE_POSITION, "vPosition");
	glBindAttribLocation(gShaderProgramObject_krishnaAnimate, MATRIX_ATTRIBUTE_NORMAL, "vNormal");
	glBindAttribLocation(gShaderProgramObject_krishnaAnimate, MATRIX_ATTRIBUTE_TEXTURE0, "vTexcoord");
	glBindAttribLocation(gShaderProgramObject_krishnaAnimate, MATRIX_ATTRIBUTE_TANGENT, "aTangent");
	glBindAttribLocation(gShaderProgramObject_krishnaAnimate, MATRIX_ATTRIBUTE_BITANGENT, "aBitangent");
	glBindAttribLocation(gShaderProgramObject_krishnaAnimate, MATRIX_ATTRIBUTE_ID, "s_vIDs");
	glBindAttribLocation(gShaderProgramObject_krishnaAnimate, MATRIX_ATTRIBUTE_WEIGHTS, "s_vWeights");

	glLinkProgram(gShaderProgramObject_krishnaAnimate);

	// Error checking
	checkLinkLog((char*)"gShaderProgramObject_krishnaAnimate", gShaderProgramObject_krishnaAnimate);


	gModelMatrixUniform_krishnaAnimate = glGetUniformLocation(gShaderProgramObject_krishnaAnimate, "u_model_matrix");
	gViewMatrixUniform_krishnaAnimate = glGetUniformLocation(gShaderProgramObject_krishnaAnimate, "u_view_matrix");
	gProjectionMatrixUniform_krishnaAnimate = glGetUniformLocation(gShaderProgramObject_krishnaAnimate, "u_projection_matrix");

	gLKeyPressedUniform_krishnaAnimate = glGetUniformLocation(gShaderProgramObject_krishnaAnimate, "u_LKeyPressed");

	gLdUniform_krishnaAnimate = glGetUniformLocation(gShaderProgramObject_krishnaAnimate, "u_Ld");
	gLaUniform_krishnaAnimate = glGetUniformLocation(gShaderProgramObject_krishnaAnimate, "u_La");
	gLsUniform_krishnaAnimate = glGetUniformLocation(gShaderProgramObject_krishnaAnimate, "u_Ls");

	gKdUniform_krishnaAnimate = glGetUniformLocation(gShaderProgramObject_krishnaAnimate, "u_Kd");
	gKsUniform_krishnaAnimate = glGetUniformLocation(gShaderProgramObject_krishnaAnimate, "u_Ks");
	gKaUniform_krishnaAnimate = glGetUniformLocation(gShaderProgramObject_krishnaAnimate, "u_Ka");

	gLightPositionUniform_krishnaAnimate = glGetUniformLocation(gShaderProgramObject_krishnaAnimate, "u_light_position");
	gMaterialShininessUniform_krishnaAnimate = glGetUniformLocation(gShaderProgramObject_krishnaAnimate, "u_material_shininess");

	gTextureSamplerUniform_krishnaAnimate = glGetUniformLocation(gShaderProgramObject_krishnaAnimate, "u_texture0_sampler");

	gTextureActiveUniform_krishnaAnimate = glGetUniformLocation(gShaderProgramObject_krishnaAnimate, "u_is_texture");
	gAlphaUniform_krishnaAnimate = glGetUniformLocation(gShaderProgramObject_krishnaAnimate, "u_alpha");

	gTimeUniform_krishnaAnimate = glGetUniformLocation(gShaderProgramObject_krishnaAnimate, "time");
	gViratRoopStartUniform_krishnaAnimate = glGetUniformLocation(gShaderProgramObject_krishnaAnimate, "virat_roop_start");

	applyBloomUniform_krishnaAnimate = glGetUniformLocation(gShaderProgramObject_krishnaAnimate, "applyBloom");
	u_bloom_is_activeUniform_krishnaAnimate = glGetUniformLocation(gShaderProgramObject_krishnaAnimate, "u_bloom_is_active");
	bloom_thresh_minUniform_krishnaAnimate = glGetUniformLocation(gShaderProgramObject_krishnaAnimate, "bloom_thresh_min");
	bloom_thresh_maxUniform_krishnaAnimate = glGetUniformLocation(gShaderProgramObject_krishnaAnimate, "bloom_thresh_max");

	gIs_animatedUniform_krishnaAnimate = glGetUniformLocation(gShaderProgramObject_krishnaAnimate, "is_animated");
#pragma endregion

	// SHADER PROGRAM OBJECT 3
	// for attractors
#pragma region ATTRACTOR_SHADER

	//VERTEX SHADER
	gVertexShaderObject_attractor_krishna = glCreateShader(GL_VERTEX_SHADER);

	const GLchar* vertextShaderSourceCode_attractor_krishna =
		"#version 440 core" \
		"\n" \
		"in vec4 vPosition;" \

		"uniform mat4 u_model_matrix;" \
		"uniform mat4 u_view_matrix;" \
		"uniform mat4 u_projection_matrix;" \

		"void main(void)" \
		"{" \
		"gl_Position		= u_projection_matrix * u_view_matrix * u_model_matrix * vPosition;" \
		"}";

	glShaderSource(gVertexShaderObject_attractor_krishna, 1, (const GLchar**)&vertextShaderSourceCode_attractor_krishna, NULL);

	//compile shader
	glCompileShader(gVertexShaderObject_attractor_krishna);

	// Error checking
	checkCompilationLog((char*)"gVertexShaderObject_attractor_krishna", gVertexShaderObject_attractor_krishna);


	//FRAGMENT SHADER

	gFragmentShaderObject_attractor_krishna = glCreateShader(GL_FRAGMENT_SHADER);

	const GLchar* fragmentShaderSourceCode_attractor_krishna =
		"#version 440 core" \
		"\n" \
		"layout (location = 0) out vec4 FragColor;" \
		"layout (location = 1) out vec4 BloomColor;" \
		"layout (location = 2) out vec4 GodRaysColor;" \

		"uniform int applyBloom;" \
		"uniform float bloom_thresh_min = 0.8f;" \
		"uniform float bloom_thresh_max = 1.2f;" \
		"uniform int u_bloom_is_active;" \

		"void main(void)" \
		"{" \
		"FragColor = vec4(255.0/255.0,115.0/255.0,0.0,0.5);" \
		//"FragColor = vec4(255.0/255.0,215.0/255.0,0.0,0.5);" 
		
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
		//"FragColor = vec4(0.2,0.1,1.0,0.5);" 
		"}";

	glShaderSource(gFragmentShaderObject_attractor_krishna, 1, (const GLchar**)&fragmentShaderSourceCode_attractor_krishna, NULL);
	glCompileShader(gFragmentShaderObject_attractor_krishna);

	// Error checking
	checkCompilationLog((char*)"gFragmentShaderObject_attractor_krishna", gFragmentShaderObject_attractor_krishna);


	//Shader Program

	gShaderProgramObject_attractor_krishna = glCreateProgram();

	glAttachShader(gShaderProgramObject_attractor_krishna, gVertexShaderObject_attractor_krishna);
	glAttachShader(gShaderProgramObject_attractor_krishna, gFragmentShaderObject_attractor_krishna);

	glBindAttribLocation(gShaderProgramObject_attractor_krishna, MATRIX_ATTRIBUTE_POSITION, "vPosition");
	//glBindAttribLocation(gShaderProgramObject_attractor_krishna, MATRIX_ATTRIBUTE_TEXTURE0, "vTexcoord");

	glLinkProgram(gShaderProgramObject_attractor_krishna);

	// Error checking
	checkLinkLog((char*)"gShaderProgramObject_attractor_krishna", gShaderProgramObject_attractor_krishna);


	gModelMatrixUniform_krishnaAttractor = glGetUniformLocation(gShaderProgramObject_attractor_krishna, "u_model_matrix");
	gViewMatrixUniform_krishnaAttractor = glGetUniformLocation(gShaderProgramObject_attractor_krishna, "u_view_matrix");
	gProjectionMatrixUniform_krishnaAttractor = glGetUniformLocation(gShaderProgramObject_attractor_krishna, "u_projection_matrix");

	applyBloomUniform_krishnaAttractor = glGetUniformLocation(gShaderProgramObject_attractor_krishna, "applyBloom");
	u_bloom_is_activeUniform_krishnaAttractor = glGetUniformLocation(gShaderProgramObject_attractor_krishna, "u_bloom_is_active");
	bloom_thresh_minUniform_krishnaAttractor = glGetUniformLocation(gShaderProgramObject_attractor_krishna, "bloom_thresh_min");
	bloom_thresh_maxUniform_krishnaAttractor = glGetUniformLocation(gShaderProgramObject_attractor_krishna, "bloom_thresh_max");

#pragma endregion

#pragma region MORPIS
	//VERTEX SHADER
	gVertexShaderObject_mor_pis_krishna = glCreateShader(GL_VERTEX_SHADER);
	const GLchar* vertextShaderSourceCode_mor_pis_krishna =
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

	glShaderSource(gVertexShaderObject_mor_pis_krishna, 1, (const GLchar**)&vertextShaderSourceCode_mor_pis_krishna, NULL);
	glCompileShader(gVertexShaderObject_mor_pis_krishna);
	checkCompilationLog((char*)"gVertexShaderObject_mor_pis_krishna", gVertexShaderObject_mor_pis_krishna);


	//FRAGMENT SHADER

	gFragmentShaderObject_mor_pis_krishna = glCreateShader(GL_FRAGMENT_SHADER);
	const GLchar* fragmentShaderSourceCode_mor_pis_krishna =
		"#version 450 core" \
		"\n" \
		"in vec2 out_texcoord;" \
		"out vec4 FragColor;" \
		"uniform sampler2D u_sampler1;" \
		"uniform sampler2D u_sampler2;" \

		"void main(void)" \
		"{" \
		"vec4 FragColor1 = texture(u_sampler1,out_texcoord);" \
		"vec4 FragColor2 = texture(u_sampler2,out_texcoord);" \
		"FragColor = FragColor1 * FragColor2;"
		"if(FragColor.r < 0.1 && FragColor.g < 0.1 && FragColor.b < 0.1)" \
		"discard;" \
		"}";

	glShaderSource(gFragmentShaderObject_mor_pis_krishna, 1, (const GLchar**)&fragmentShaderSourceCode_mor_pis_krishna, NULL);
	glCompileShader(gFragmentShaderObject_mor_pis_krishna);
	checkCompilationLog((char*)"gFragmentShaderObject_mor_pis_krishna", gFragmentShaderObject_mor_pis_krishna);


	//Shader Program
	gShaderProgramObject_mor_pis_krishna = glCreateProgram();

	glAttachShader(gShaderProgramObject_mor_pis_krishna, gVertexShaderObject_mor_pis_krishna);
	glAttachShader(gShaderProgramObject_mor_pis_krishna, gFragmentShaderObject_mor_pis_krishna);

	glBindAttribLocation(gShaderProgramObject_mor_pis_krishna, MATRIX_ATTRIBUTE_POSITION, "vPosition");
	glBindAttribLocation(gShaderProgramObject_mor_pis_krishna, MATRIX_ATTRIBUTE_TEXTURE0, "vTexcoord");

	glLinkProgram(gShaderProgramObject_mor_pis_krishna);

	// Error checking
	checkLinkLog((char*)"gShaderProgramObject_mor_pis_krishna", gShaderProgramObject_mor_pis_krishna);


	gModelMatrixUniform_mor_pis			= glGetUniformLocation(gShaderProgramObject_mor_pis_krishna, "u_model_matrix");
	gViewMatrixUniform_mor_pis			= glGetUniformLocation(gShaderProgramObject_mor_pis_krishna, "u_view_matrix");
	gProjectionMatrixUniform_mor_pis	= glGetUniformLocation(gShaderProgramObject_mor_pis_krishna, "u_projection_matrix");

	samplerUniform1_mor_pis = glGetUniformLocation(gShaderProgramObject_mor_pis_krishna, "u_sampler1");
	samplerUniform2_mor_pis = glGetUniformLocation(gShaderProgramObject_mor_pis_krishna, "u_sampler2");
#pragma endregion
}


BOOL loadTexture_mor_pis(GLuint* texture, TCHAR imageResourceID[])
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

void initialize_krishnaAnimate()
{
	initKrishnaAnimate();

	krishna_Animated_StandUpHand.loadModel("Resources/11MarchKrishnaModel/pointFinger.fbx");

	for (GLfloat i = -2.0f; i <= 2.0f; i = i + 0.15f)
	{
		for (GLfloat j = -2.0f; j <= 2.0f; j = j + 0.15f)
		{
			for (GLfloat k = -2.0f; k <= 2.0f; k = k + 0.10f)
			{
				vertices_attractor.push_back(i);
				vertices_attractor.push_back(j);
				vertices_attractor.push_back(k);
			}
		}
	}

	// create VaoForAttractor
	glGenVertexArrays(1, &VaoForAttractor);
	glBindVertexArray(VaoForAttractor);

	// create VboForAttractorForAttractor
	glGenBuffers(1, &VboForAttractor);
	glBindBuffer(GL_ARRAY_BUFFER, VboForAttractor);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVertices), triangleVertices, GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, vertices_attractor.size() * sizeof(float), NULL, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(MATRIX_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(MATRIX_ATTRIBUTE_POSITION);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	const GLfloat rectangleVertices_mor_pis[] =
	{
		1.0f, 1.0f,0.0f,
		-1.0f, 1.0f,0.0f,
		-1.0f, -1.0f,0.0f,
		1.0f, -1.0f,0.0f
	};

	const GLfloat rectangleTexcoords_mor_pis[] =
	{
		1.0f, 1.0f,
		0.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 0.0f
	};

	// create vao
	glGenVertexArrays(1, &vao_rectangle_mor_pis);
	glBindVertexArray(vao_rectangle_mor_pis);

	// create vbo for position
	glGenBuffers(1, &vbo_position_rectangle_mor_pis);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_position_rectangle_mor_pis);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rectangleVertices_mor_pis), rectangleVertices_mor_pis, GL_STATIC_DRAW);
	glVertexAttribPointer(MATRIX_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(MATRIX_ATTRIBUTE_POSITION);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// create vbo for texture
	glGenBuffers(1, &vbo_texture_rectangle_mor_pis);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_texture_rectangle_mor_pis);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rectangleTexcoords_mor_pis), rectangleTexcoords_mor_pis, GL_STATIC_DRAW);
	glVertexAttribPointer(MATRIX_ATTRIBUTE_TEXTURE0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(MATRIX_ATTRIBUTE_TEXTURE0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);


	glBindVertexArray(0);

	/* defined in Text.rc
	#define IDBITMAP_FEATHER1 102
	#define IDBITMAP_FEATHER2 103
	*/
	glEnable(GL_TEXTURE_2D);

	loadTexture_mor_pis(&textureFeather1_mor_pis, MAKEINTRESOURCE(102));
	loadTexture_mor_pis(&textureFeather2_mor_pis, MAKEINTRESOURCE(103));
	
}

void display_krishnaAnimate()
{
	/*glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_NOTEQUAL, 0);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
	glEnable(GL_POINT_SMOOTH);
	glPointSize(1.5);
	*/

	if (startJoin_krishnaAnimate)
	{
		gfKrishnaModelScale = 12.0f;
	}
	else
	{
		gfKrishnaModelScale = 4.0f;
	}
	
	glUseProgram(0);
	glUseProgram(gShaderProgramObject_krishnaAnimate);
	
	krishna_Animated_StandUpHand.initShaders(gShaderProgramObject_krishnaAnimate);

	glUniform1i(u_bloom_is_activeUniform_krishnaAnimate, 1);
	glUniform1f(bloom_thresh_minUniform_krishnaAnimate, bloom_thresh_min);
	glUniform1f(bloom_thresh_maxUniform_krishnaAnimate, bloom_thresh_max);
	glUniform1i(applyBloomUniform_krishnaAnimate, 1);

	glUniform1i(gLKeyPressedUniform_krishnaAnimate, 1);

	glUniform3fv(gLdUniform_krishnaAnimate, 1, lightDiffuse_krishnaAnimate);
	glUniform3fv(gLaUniform_krishnaAnimate, 1, lightAmbient_krishnaAnimate);
	glUniform3fv(gLsUniform_krishnaAnimate, 1, lightSpecular_krishnaAnimate);
	glUniform4fv(gLightPositionUniform_krishnaAnimate, 1, lightPosition_krishnaAnimate);
	glUniform3fv(gKdUniform_krishnaAnimate, 1, material_diffuse);
	glUniform3fv(gKaUniform_krishnaAnimate, 1, material_ambient);
	glUniform3fv(gKsUniform_krishnaAnimate, 1, material_specular);
	glUniform1f(gMaterialShininessUniform_krishnaAnimate, material_shininess);

	mat4 modelMatrix = mat4::identity();
	mat4 scaleMatrix = mat4::identity();
	mat4 rotateMatrix = mat4::identity();

	// KRISHNA MODEL
	modelMatrix = vmath::translate(-250.0f, 55.0f, 1.0f);
	//modelMatrix		= vmath::translate(-300.0f, 120.0f, 1.0f);
	scaleMatrix = scale(gfKrishnaModelScale, gfKrishnaModelScale, gfKrishnaModelScale);
	rotateMatrix = rotate(90.0f, 0.0f, 1.0f, 0.0f);

	modelMatrix = modelMatrix * rotateMatrix * scaleMatrix;

	glUniformMatrix4fv(gModelMatrixUniform_krishnaAnimate, 1, GL_FALSE, modelMatrix);
	glUniformMatrix4fv(gViewMatrixUniform_krishnaAnimate, 1, GL_FALSE, gViewMatrix);								// globally camera set in perFrag file
	glUniformMatrix4fv(gProjectionMatrixUniform_krishnaAnimate, 1, GL_FALSE, gPerspectiveProjectionMatrix);			// globally pojection set

	glUniform1f(gTimeUniform_krishnaAnimate, ftime_krishnaAnimate);

	if (startJoin_krishnaAnimate)
	{
		glUniform1i(gViratRoopStartUniform_krishnaAnimate, 1);
	}
	else
	{
		glUniform1i(gViratRoopStartUniform_krishnaAnimate, 0);
	}

	
	glUniform1i(gIs_animatedUniform_krishnaAnimate, isModelAnimationStart);
	glUniform1i(gTextureActiveUniform_krishnaAnimate, 1);
	glUniform1i(gAlphaUniform_krishnaAnimate, 1.0);
	
	
	krishna_Animated_StandUpHand.draw(gShaderProgramObject_krishnaAnimate, isModelAnimationStart, 2);
	
	glUseProgram(0);

	if (startJoin_krishnaAnimate)
	{
		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_NOTEQUAL, 0);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
		glEnable(GL_POINT_SMOOTH);
		//glPointSize(1.5);
		glPointSize(3.0);
		glUseProgram(gShaderProgramObject_attractor_krishna);

		glUniform1i(u_bloom_is_activeUniform_krishnaAttractor, 1);
		glUniform1f(bloom_thresh_minUniform_krishnaAttractor, bloom_thresh_min);
		glUniform1f(bloom_thresh_maxUniform_krishnaAttractor, bloom_thresh_max);
		glUniform1i(applyBloomUniform_krishnaAttractor, 1);

		modelMatrix = mat4::identity();
		scaleMatrix = mat4::identity();
		rotateMatrix = mat4::identity();

		modelMatrix = vmath::translate(X_Pos_attractor, Y_Pos_attractor, Z_Pos_attractor);
		scaleMatrix = scale(30.0f, 30.0f, 30.0f);
		modelMatrix = modelMatrix * rotateMatrix * scaleMatrix;

		glUniformMatrix4fv(gModelMatrixUniform_krishnaAttractor, 1, GL_FALSE, modelMatrix);
		glUniformMatrix4fv(gViewMatrixUniform_krishnaAttractor, 1, GL_FALSE, gViewMatrix);								    // globally camera set in perFrag file
		glUniformMatrix4fv(gProjectionMatrixUniform_krishnaAttractor, 1, GL_FALSE, gPerspectiveProjectionMatrix);			// globally pojection set

		glBindVertexArray(VaoForAttractor);
		glBindBuffer(GL_ARRAY_BUFFER, VboForAttractor);
		glBufferData(GL_ARRAY_BUFFER, vertices_attractor.size() * sizeof(float), &vertices_attractor[0], GL_DYNAMIC_DRAW);
		glDrawArrays(GL_POINTS, 0, vertices_attractor.size());
		glBindVertexArray(0);

		glUseProgram(0);

		glDisable(GL_ALPHA_TEST);
		glDisable(GL_BLEND);
		//glDisable(GL_POINT_SMOOTH);

	}

	/*if (isKrishnaRenderInPoints == true)
	{
		glDisable(GL_ALPHA_TEST);
		glDisable(GL_BLEND);
		glDisable(GL_POINT_SMOOTH);
	}*/

	
	glUseProgram(gShaderProgramObject_mor_pis_krishna);

	modelMatrix = mat4::identity();
	scaleMatrix = mat4::identity();
	rotateMatrix = mat4::identity();

	modelMatrix = translate(X_Pos_mor_pis, 1050.0f, -40.0f);
	scaleMatrix = scale(50.0f, 50.0f, 50.0f);
	rotateMatrix = rotate(90.0f, 0.0f, 1.0f, 0.0f);
	modelMatrix = modelMatrix * rotateMatrix * scaleMatrix;

	glUniformMatrix4fv(gModelMatrixUniform_mor_pis, 1, GL_FALSE, modelMatrix);
	glUniformMatrix4fv(gViewMatrixUniform_mor_pis, 1, GL_FALSE, gViewMatrix);								    // globally camera set in perFrag file
	glUniformMatrix4fv(gProjectionMatrixUniform_mor_pis, 1, GL_FALSE, gPerspectiveProjectionMatrix);			// globally pojection set

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureFeather1_mor_pis);
	glUniform1i(samplerUniform1_mor_pis, 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, textureFeather2_mor_pis);
	glUniform1i(samplerUniform2_mor_pis, 1);

	// bind with vao
	glBindVertexArray(vao_rectangle_mor_pis);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	glBindVertexArray(0);

	glUseProgram(0);
	
}

void update_krishnaAnimate()
{
	if (startJoin_krishnaAnimate)
	{
		if (ftime_krishnaAnimate < 3000.0f)
		{
			ftime_krishnaAnimate = ftime_krishnaAnimate + 0.4f;
		}

		// attractor

		for (int i = 0; i < vertices_attractor.size(); i = i + 3)
		{
			GLfloat X1 = (GLfloat)(vertices_attractor[i + 0] + t_for_attractor * ((GLfloat)sin(vertices_attractor[i + 1]) - (b_for_attractor * vertices_attractor[i + 0])));
			GLfloat Y1 = (GLfloat)(vertices_attractor[i + 1] + t_for_attractor * ((GLfloat)sin(vertices_attractor[i + 2]) - (b_for_attractor * vertices_attractor[i + 1])));
			GLfloat Z1 = (GLfloat)(vertices_attractor[i + 2] + t_for_attractor * ((GLfloat)sin(vertices_attractor[i + 0]) - (b_for_attractor * vertices_attractor[i + 2])));

			vertices_attractor[i + 0] = X1;
			vertices_attractor[i + 1] = Y1;
			vertices_attractor[i + 2] = Z1;
		}

		// to move attractor along with camera
		//if (X_Pos_attractor < 100.0f)
		//{
		//	X_Pos_attractor = X_Pos_attractor + 0.1f;
		//}
		
		if (ftime_krishnaAnimate > 1200.0f && Y_Pos_attractor > 170.0f)
		{
			Y_Pos_attractor = Y_Pos_attractor - 2.5f;
			//t_for_attractor = 0.05f;
			t_for_attractor = 0.1f;
			//b_for_attractor = 0.03f;
			b_for_attractor = 0.045f;
		}
		else if (Y_Pos_attractor < 1350.0f)
		{
			Y_Pos_attractor = Y_Pos_attractor + 0.4f;
		}
		
		/*
		if (Z_Pos_attractor > -100.0f)
		{
			Z_Pos_attractor = Z_Pos_attractor - 0.1f;
		}
		*/

	}
}

void uninitialize_krishnaAnimate()
{
	uninitializeAllModelData();
	uninitializeSphereShape();
	programObjectSafeRelease(gShaderProgramObject_krishnaAnimate);

}