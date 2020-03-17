#include "../../Include/BasicShapeLoader/Model/AssimpModelLoader/Model.h"

// SHADER 1
GLuint gVertexShaderObject_krishnaAnimateAssimp;
GLuint gFragmentShaderObject_krishnaAnimateAssimp;
GLuint gShaderProgramObject_krishnaAnimateAssimp;

GLuint gModelMatrixUniform_krishnaAnimateAssimp;
GLuint gViewMatrixUniform_krishnaAnimateAssimp;
GLuint gProjectionMatrixUniform_krishnaAnimateAssimp;
GLuint gLdUniform_krishnaAnimateAssimp, gLaUniform_krishnaAnimateAssimp, gLsUniform_krishnaAnimateAssimp;
GLuint gLightPositionUniform_krishnaAnimateAssimp;
GLuint gKdUniform_krishnaAnimateAssimp, gKaUniform_krishnaAnimateAssimp, gKsUniform_krishnaAnimateAssimp;
GLuint gMaterialShininessUniform_krishnaAnimateAssimp;
GLuint gLKeyPressedUniform_krishnaAnimateAssimp;
GLuint gTextureSamplerUniform_krishnaAnimateAssimp, gTextureActiveUniform_krishnaAnimateAssimp, gAlphaUniform_krishnaAnimateAssimp;
GLuint gViewPosUniform_krishnaAnimateAssimp, gNumPointLightsUniform_krishnaAnimateAssimp, gIs_animatedUniform_krishnaAnimateAssimp;

Model krishna_Animated;

bool isModelAnimationStart = false;
void initAssimpModelLoaderShader()
{
	void uninitialize(int);

	//VERTEX SHADER
	gVertexShaderObject_krishnaAnimateAssimp = glCreateShader(GL_VERTEX_SHADER);

	const GLchar* vertextShaderSourceCode =
		"#version 450" \
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
			
		"}";

	glShaderSource(gVertexShaderObject_krishnaAnimateAssimp, 1, (const GLchar**)&vertextShaderSourceCode, NULL);

	//compile shader
	glCompileShader(gVertexShaderObject_krishnaAnimateAssimp);

	// Error checking
	checkCompilationLog((char*)"gVertexShaderObject_krishnaAnimateAssimp", gVertexShaderObject_krishnaAnimateAssimp);


	//FRAGMENT SHADER

	gFragmentShaderObject_krishnaAnimateAssimp = glCreateShader(GL_FRAGMENT_SHADER);

	const GLchar* fragmentShaderSourceCode =
		"#version 450" \
		"\n" \
		"in vec3 transformed_normals;" \
		"in vec3 light_direction;" \
		"in vec3 viewer_vector;" \
		"in vec2 out_texcord;" \

		"out vec4 FragColor;" \

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


		"void main(void)" \
		"{" \
			"vec3 phong_ads_color;" \
			"vec4 Final_Texture;" \
			"if(u_LKeyPressed==1)" \
			"{" \
				"vec3 normalized_transformed_normals	= normalize(transformed_normals);" \
				"vec3 normalized_light_direction		= normalize(light_direction);" \
				"vec3 normalized_viewer_vector			= normalize(viewer_vector);" \
				"vec3 ambient							= u_La * u_Ka;" \
				"float tn_dot_ld						= max(dot(normalized_transformed_normals, normalized_light_direction),0.0);" \
				"vec3 diffuse							= u_Ld * u_Kd * tn_dot_ld;" \
				"vec3 reflection_vector					= reflect(-normalized_light_direction, normalized_transformed_normals);" \
				"vec3 specular							= u_Ls * u_Ks * pow(max(dot(reflection_vector, normalized_viewer_vector), 0.0), u_material_shininess);" \
				"phong_ads_color						= ambient + diffuse + specular;" \
			"}" \
			"else" \
			"{" \
				"phong_ads_color = vec3(1.0, 1.0, 1.0);" \
			"}" \
			"if(u_is_texture == 1)" \
			"{" \
				"Final_Texture	= texture(u_texture0_sampler, out_texcord); " \
				"FragColor		= vec4(phong_ads_color, u_alpha) * Final_Texture;" \
			"}" \
			"else" \
			"{" \
				"FragColor		= vec4(phong_ads_color,1.0f);"\
			"}" \
		"}";
		
	glShaderSource(gFragmentShaderObject_krishnaAnimateAssimp, 1, (const GLchar**)&fragmentShaderSourceCode, NULL);

	glCompileShader(gFragmentShaderObject_krishnaAnimateAssimp);

	// Error checking
	checkCompilationLog((char*)"gFragmentShaderObject_krishnaAnimateAssimp", gFragmentShaderObject_krishnaAnimateAssimp);


	//Shader Program

	gShaderProgramObject_krishnaAnimateAssimp = glCreateProgram();

	glAttachShader(gShaderProgramObject_krishnaAnimateAssimp, gVertexShaderObject_krishnaAnimateAssimp);

	glAttachShader(gShaderProgramObject_krishnaAnimateAssimp, gFragmentShaderObject_krishnaAnimateAssimp);

	glBindAttribLocation(gShaderProgramObject_krishnaAnimateAssimp, MATRIX_ATTRIBUTE_POSITION, "vPosition");
	glBindAttribLocation(gShaderProgramObject_krishnaAnimateAssimp, MATRIX_ATTRIBUTE_NORMAL, "vNormal");
	glBindAttribLocation(gShaderProgramObject_krishnaAnimateAssimp, MATRIX_ATTRIBUTE_TEXTURE0, "vTexcoord");
	glBindAttribLocation(gShaderProgramObject_krishnaAnimateAssimp, MATRIX_ATTRIBUTE_TANGENT, "aTangent");
	glBindAttribLocation(gShaderProgramObject_krishnaAnimateAssimp, MATRIX_ATTRIBUTE_BITANGENT, "aBitangent");
	glBindAttribLocation(gShaderProgramObject_krishnaAnimateAssimp, MATRIX_ATTRIBUTE_ID, "s_vIDs");
	glBindAttribLocation(gShaderProgramObject_krishnaAnimateAssimp, MATRIX_ATTRIBUTE_WEIGHTS, "s_vWeights");
	
	glLinkProgram(gShaderProgramObject_krishnaAnimateAssimp);

	// Error checking
	checkLinkLog((char*)"gShaderProgramObject_krishnaAnimateAssimp", gShaderProgramObject_krishnaAnimateAssimp);


	gModelMatrixUniform_krishnaAnimateAssimp = glGetUniformLocation(gShaderProgramObject_krishnaAnimateAssimp, "u_model_matrix");
	gViewMatrixUniform_krishnaAnimateAssimp = glGetUniformLocation(gShaderProgramObject_krishnaAnimateAssimp, "u_view_matrix");
	gProjectionMatrixUniform_krishnaAnimateAssimp = glGetUniformLocation(gShaderProgramObject_krishnaAnimateAssimp, "u_projection_matrix");

	gLKeyPressedUniform_krishnaAnimateAssimp = glGetUniformLocation(gShaderProgramObject_krishnaAnimateAssimp, "u_LKeyPressed");

	gLdUniform_krishnaAnimateAssimp = glGetUniformLocation(gShaderProgramObject_krishnaAnimateAssimp, "u_Ld");
	gLaUniform_krishnaAnimateAssimp = glGetUniformLocation(gShaderProgramObject_krishnaAnimateAssimp, "u_La");
	gLsUniform_krishnaAnimateAssimp = glGetUniformLocation(gShaderProgramObject_krishnaAnimateAssimp, "u_Ls");

	gKdUniform_krishnaAnimateAssimp = glGetUniformLocation(gShaderProgramObject_krishnaAnimateAssimp, "u_Kd");
	gKsUniform_krishnaAnimateAssimp = glGetUniformLocation(gShaderProgramObject_krishnaAnimateAssimp, "u_Ks");
	gKaUniform_krishnaAnimateAssimp = glGetUniformLocation(gShaderProgramObject_krishnaAnimateAssimp, "u_Ka");

	gLightPositionUniform_krishnaAnimateAssimp		= glGetUniformLocation(gShaderProgramObject_krishnaAnimateAssimp, "u_light_position");
	gMaterialShininessUniform_krishnaAnimateAssimp	= glGetUniformLocation(gShaderProgramObject_krishnaAnimateAssimp, "u_material_shininess");

	gTextureSamplerUniform_krishnaAnimateAssimp = glGetUniformLocation(gShaderProgramObject_krishnaAnimateAssimp, "u_texture0_sampler");

	gTextureActiveUniform_krishnaAnimateAssimp	= glGetUniformLocation(gShaderProgramObject_krishnaAnimateAssimp, "u_is_texture");
	gAlphaUniform_krishnaAnimateAssimp			= glGetUniformLocation(gShaderProgramObject_krishnaAnimateAssimp, "u_alpha");
	
	
	
	gIs_animatedUniform_krishnaAnimateAssimp = glGetUniformLocation(gShaderProgramObject_krishnaAnimateAssimp, "is_animated");
	
}


void initialize_AssimpModelLoader()
{
	initAssimpModelLoaderShader();

	krishna_Animated.initShaders(gShaderProgramObject_krishnaAnimateAssimp);
	
	krishna_Animated.loadModel("Resources/11MarchKrishnaModel/SeatToStandWithOrnament.fbx");
	//initCubeShape();

	//initSphereShape();

	//LoadAllModels();

	glShadeModel(GL_SMOOTH);

	glClearDepth(1.0f);
	// enable depth testing
	glEnable(GL_DEPTH_TEST);
	// depth test to do
	glDepthFunc(GL_LEQUAL);

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glEnable(GL_CULL_FACE);

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // blue

}

void display_AssimpModelLoader()
{
	
	glUseProgram(gShaderProgramObject_krishnaAnimateAssimp);
	
	if (gbLight == true)
	{
		glUniform1i(gLKeyPressedUniform_krishnaAnimateAssimp, 1);

		glUniform3fv(gLdUniform_krishnaAnimateAssimp, 1, lightDiffuse);//white
		glUniform3fv(gLaUniform_krishnaAnimateAssimp, 1, lightAmbient);
		glUniform3fv(gLsUniform_krishnaAnimateAssimp, 1, lightSpecular);
		glUniform4fv(gLightPositionUniform_krishnaAnimateAssimp, 1, lightPosition);
		
		glUniform3fv(gKdUniform_krishnaAnimateAssimp, 1, material_diffuse);
		glUniform3fv(gKaUniform_krishnaAnimateAssimp, 1, material_ambient);
		glUniform3fv(gKsUniform_krishnaAnimateAssimp, 1, material_specular);
		glUniform1f(gMaterialShininessUniform_krishnaAnimateAssimp, material_shininess);
	}
	else
	{
		glUniform1i(gLKeyPressedUniform_krishnaAnimateAssimp, 0);
	}

	mat4 modelMatrix = mat4::identity();
	mat4 viewMatrix = mat4::identity();
	mat4 scaleMatrix = mat4::identity();
	mat4 rotateMatrix = mat4::identity();
	
	gViewMatrix = lookat(vmath_camera_eye_coord, vmath_camera_center_coord, vmath_camera_up_coord);
	glUniformMatrix4fv(gViewMatrixUniform_krishnaAnimateAssimp, 1, GL_FALSE, gViewMatrix);
	
	glUniform3fv(gViewPosUniform_krishnaAnimateAssimp, 1, vmath_camera_eye_coord);
	modelMatrix = vmath::translate(0.0f, 0.0f, -2.0f);

	glUniformMatrix4fv(gProjectionMatrixUniform_krishnaAnimateAssimp, 1, GL_FALSE, gPerspectiveProjectionMatrix);

	//object

	modelMatrix = mat4::identity();
	//modelMatrix = vmath::translate(0.0f, 0.0f, -6.0f);
	modelMatrix = vmath::translate(-300.0f, 55.0f, 1.0f);
	rotateMatrix = vmath::rotate(90.0f, vec3(0.0f, 1.0f, 0.0f));
	
	scaleMatrix = scale(3.0f, 3.0f, 3.0f);
	modelMatrix = modelMatrix * rotateMatrix;
	modelMatrix = modelMatrix * scaleMatrix;

	glUniformMatrix4fv(gModelMatrixUniform_krishnaAnimateAssimp, 1, GL_FALSE, modelMatrix);
	
	glUniform1i(gIs_animatedUniform_krishnaAnimateAssimp, isModelAnimationStart);
	glUniform1i(gTextureActiveUniform_krishnaAnimateAssimp, 1);
	glUniform1i(gAlphaUniform_krishnaAnimate, 1.0);
	
	krishna_Animated.draw(gShaderProgramObject_krishnaAnimateAssimp, isModelAnimationStart);

	glUseProgram(0);
}