#pragma once

#include "../../Include/BasicShapeLoader/Model/Matrix_Obj_Loader.h"

// SHADER 1
GLuint gVertexShaderObject_krishnaAnimate;
GLuint gGeometryShaderObject_krishnaAnimate;
GLuint gFragmentShaderObject_krishnaAnimate;
GLuint gShaderProgramObject_krishnaAnimate;

// SHADER 2
GLuint gGeometryShaderObject_2_krishnaAnimate;
GLuint gFragmentShaderObject_2_krishnaAnimate;
GLuint gShaderProgramObject_2_krishnaAnimate;

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

bool startExplode_krishnaAnimate	= false;
bool startJoin_krishnaAnimate		= false;
bool exploded_krishnaAnimate		= false;
bool Done_krishnaAnimate			= false;
int DoneFlag_krishnaAnimate			= 0;

float ftime_krishnaAnimate = 0.0f;

GLfloat lightAmbient_krishnaAnimate[] = { 0.0f,0.0f,0.0f,1.0f };
GLfloat lightDiffuse_krishnaAnimate[] = { 1.0f,1.0f,1.0f,1.0f };
GLfloat lightSpecular_krishnaAnimate[] = { 1.0f,1.0f,1.0f,1.0f };
GLfloat lightPosition_krishnaAnimate[] = { 100.0f,100.0f,100.0f,1.0f };


// animation variable
GLfloat gfKrishnaModelScale = 70.0f;


void initKrishnaAnimate()
{
	void uninitialize(int);

	// SHADER PROGRAM OBJECT 1
#pragma region SHADER1
	//VERTEX SHADER
	gVertexShaderObject_krishnaAnimate = glCreateShader(GL_VERTEX_SHADER);

	const GLchar* vertextShaderSourceCode =
		"#version 400" \
		"\n" \
		"in vec4 vPosition;" \
		"in vec3 vNormal;" \
		"in vec2 vTexcoord;" \

		"uniform mat4 u_model_matrix;" \
		"uniform mat4 u_view_matrix;" \
		"uniform mat4 u_projection_matrix;" \
		"uniform vec4 u_light_position;"\
		"uniform int u_LKeyPressed;" \

		"out vec3 transformed_normals;" \
		"out vec3 light_direction;" \
		"out vec3 viewer_vector;" \
		"out  vec2 out_texcord;" \

		"out VS_OUT "\
		"{" \
			"vec3 transformed_normals;" \
			"vec3 light_direction;" \
			"vec3 viewer_vector;" \
			"vec2 out_texcord;" \
		"}vs_out;" \

		"void main(void)" \
		"{" \
			"if(u_LKeyPressed == 1)" \
			"{" \
				"vec4 eye_coordinates			= u_view_matrix * u_model_matrix * vPosition;" \
				"vs_out.transformed_normals		= mat3(u_view_matrix * u_model_matrix) * vNormal;" \
				"vs_out.light_direction			= vec3(u_light_position) - eye_coordinates.xyz;" \
				"vs_out.viewer_vector			= -eye_coordinates.xyz;" \
			"}"\

			"vs_out.out_texcord = vTexcoord;" \
			"gl_Position		= u_projection_matrix * u_view_matrix * u_model_matrix * vPosition;" \
		"}";

	glShaderSource(gVertexShaderObject_krishnaAnimate, 1, (const GLchar**)&vertextShaderSourceCode, NULL);

	//compile shader
	glCompileShader(gVertexShaderObject_krishnaAnimate);

	// Error checking
	checkCompilationLog((char*)"gVertexShaderObject_krishnaAnimate", gVertexShaderObject_krishnaAnimate);

	// Geometry Shader

	gGeometryShaderObject_krishnaAnimate = glCreateShader(GL_GEOMETRY_SHADER);

	const GLchar* geometryShaderSourceCode =
		"#version 450 core" \
		"\n" \
		"layout(triangles)in;" \
		"layout(triangle_strip, max_vertices = 3)out;" \

		"in VS_OUT "\
		"{" \
			"vec3 transformed_normals;" \
			"vec3 light_direction;" \
			"vec3 viewer_vector;" \
			"vec2 out_texcord;" \
		"}gs_in[];" \

		"out vec3 transformed_normals;" \
		"out vec3 light_direction;" \
		"out vec3 viewer_vector;" \
		"out vec2 out_texcord;" \


		"uniform float time;" \

		"vec4 explode(vec4 position, vec3 normal)" \
		"{" \
			"normal = vec3(0.0,-1.0,0.0);" 
			"float magnitude	= 0.0;" \
			"float temp_time    = 1500.0f - time;" \
			"if(position.y > temp_time)" \
				"magnitude	= 50.0;" \
			/*"float magnitude	= position.y / 10.0;" \*/
			"vec3 direction		= vec3( normal * time * magnitude);" \
			"position			= position + vec4(direction, 0.0); " \
			"return position;" \
		"}" \

		"vec3 getNormal()" \
		"{" \
			"vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position) ;" \
			"vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position) ;" \
			"return normalize(cross(a, b));" \
		"}" \

		"void main(void)" \
		"{" \
			
			"vec3 normal = getNormal();" \

			"gl_Position			= explode(gl_in[0].gl_Position, normal);" \
			"out_texcord			= gs_in[0].out_texcord;" \
			"transformed_normals	= gs_in[0].transformed_normals;" \
			"light_direction		= gs_in[0].light_direction;" \
			"viewer_vector			= gs_in[0].viewer_vector;" \
			"EmitVertex();" \
		
			"gl_Position			= explode(gl_in[1].gl_Position , normal);" \
			"out_texcord			= gs_in[1].out_texcord;" \
			"transformed_normals	= gs_in[1].transformed_normals;" \
			"light_direction		= gs_in[1].light_direction;" \
			"viewer_vector			= gs_in[1].viewer_vector;" \
			"EmitVertex();" \
		
			"gl_Position			= explode(gl_in[2].gl_Position , normal);" \
			"out_texcord			= gs_in[2].out_texcord;" \
			"transformed_normals	= gs_in[2].transformed_normals;" \
			"light_direction		= gs_in[2].light_direction;" \
			"viewer_vector			= gs_in[2].viewer_vector;" \
			"EmitVertex();" \

			"EndPrimitive();" \
		"}";

	glShaderSource(gGeometryShaderObject_krishnaAnimate, 1, (const GLchar**)&geometryShaderSourceCode, NULL);
	glCompileShader(gGeometryShaderObject_krishnaAnimate);

	// Error checking
	checkCompilationLog((char*)"gGeometryShaderObject_krishnaAnimate", gGeometryShaderObject_krishnaAnimate);


	//FRAGMENT SHADER

	gFragmentShaderObject_krishnaAnimate = glCreateShader(GL_FRAGMENT_SHADER);

	const GLchar* fragmentShaderSourceCode =
		"#version 400" \
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

		"uniform float time;" \

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
				/*"FragColor		= vec4(phong_ads_color,u_alpha);" \*/
				"FragColor		= vec4(phong_ads_color,1.0f);" \
			"}" \
		"}";
	
	glShaderSource(gFragmentShaderObject_krishnaAnimate, 1, (const GLchar**)&fragmentShaderSourceCode, NULL);
	glCompileShader(gFragmentShaderObject_krishnaAnimate);

	// Error checking
	checkCompilationLog((char*)"gFragmentShaderObject_krishnaAnimate", gFragmentShaderObject_krishnaAnimate);


	//Shader Program

	gShaderProgramObject_krishnaAnimate = glCreateProgram();

	glAttachShader(gShaderProgramObject_krishnaAnimate, gVertexShaderObject_krishnaAnimate);

	glAttachShader(gShaderProgramObject_krishnaAnimate, gGeometryShaderObject_krishnaAnimate);

	glAttachShader(gShaderProgramObject_krishnaAnimate, gFragmentShaderObject_krishnaAnimate);

	glBindAttribLocation(gShaderProgramObject_krishnaAnimate, MATRIX_ATTRIBUTE_POSITION, "vPosition");
	glBindAttribLocation(gShaderProgramObject_krishnaAnimate, MATRIX_ATTRIBUTE_NORMAL, "vNormal");
	glBindAttribLocation(gShaderProgramObject_krishnaAnimate, MATRIX_ATTRIBUTE_TEXTURE0, "vTexcoord");

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

	gLightPositionUniform_krishnaAnimate		= glGetUniformLocation(gShaderProgramObject_krishnaAnimate, "u_light_position");
	gMaterialShininessUniform_krishnaAnimate	= glGetUniformLocation(gShaderProgramObject_krishnaAnimate, "u_material_shininess");

	gTextureSamplerUniform_krishnaAnimate = glGetUniformLocation(gShaderProgramObject_krishnaAnimate, "u_texture0_sampler");

	gTextureActiveUniform_krishnaAnimate	= glGetUniformLocation(gShaderProgramObject_krishnaAnimate, "u_is_texture");
	gAlphaUniform_krishnaAnimate			= glGetUniformLocation(gShaderProgramObject_krishnaAnimate, "u_alpha");

	gTimeUniform_krishnaAnimate = glGetUniformLocation(gShaderProgramObject_krishnaAnimate, "time");
#pragma endregion

	
	// SHADER PROGRAM OBJECT 2
#pragma region SHADER2

	gGeometryShaderObject_2_krishnaAnimate = glCreateShader(GL_GEOMETRY_SHADER);

	const GLchar* geometryShaderSourceCode_2 =
		"#version 450 core" \
		"\n" \
		"layout(triangles)in;" \
		"layout(points, max_vertices = 3)out;" \
	
		"in VS_OUT "\
		"{" \
		"vec3 transformed_normals;" \
		"vec3 light_direction;" \
		"vec3 viewer_vector;" \
		"vec2 out_texcord;" \
		"}gs_in[];" \

		"out vec3 transformed_normals;" \
		"out vec3 light_direction;" \
		"out vec3 viewer_vector;" \
		"out vec2 out_texcord;" \


		"uniform float time;" \

		"vec4 explode(vec4 position, vec3 normal)" \
		"{" \
			"normal = vec3(0.0,-1.0,0.0);"
			"float magnitude	= 0.0;" \
			"float temp_time    = 1250.0f - time;" \
			"if(position.y > temp_time)" \
				"magnitude	= 50.0;" \
			/*"float magnitude	= position.y / 10.0;" \*/
			"vec3 direction		= vec3( normal * time * magnitude);" \
			"position			= position + vec4(direction, 0.0); " \
			"return position;" \
		"}" \

		"vec3 getNormal()" \
		"{" \
			"vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position) ;" \
			"vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position) ;" \
			"return normalize(cross(a, b));" \
		"}" \

		"void main(void)" \
		"{" \

			"vec3 normal = getNormal();" \

			"gl_Position			= explode(gl_in[0].gl_Position, normal);" \
			"out_texcord			= gs_in[0].out_texcord;" \
			"transformed_normals	= gs_in[0].transformed_normals;" \
			"light_direction		= gs_in[0].light_direction;" \
			"viewer_vector			= gs_in[0].viewer_vector;" \
			"EmitVertex();" \

			"gl_Position			= explode(gl_in[1].gl_Position , normal);" \
			"out_texcord			= gs_in[1].out_texcord;" \
			"transformed_normals	= gs_in[1].transformed_normals;" \
			"light_direction		= gs_in[1].light_direction;" \
			"viewer_vector			= gs_in[1].viewer_vector;" \
			"EmitVertex();" \

			"gl_Position			= explode(gl_in[2].gl_Position , normal);" \
			"out_texcord			= gs_in[2].out_texcord;" \
			"transformed_normals	= gs_in[2].transformed_normals;" \
			"light_direction		= gs_in[2].light_direction;" \
			"viewer_vector			= gs_in[2].viewer_vector;" \
			"EmitVertex();" \

			"EndPrimitive();" \
		"}";

	glShaderSource(gGeometryShaderObject_2_krishnaAnimate, 1, (const GLchar**)&geometryShaderSourceCode_2, NULL);
	glCompileShader(gGeometryShaderObject_2_krishnaAnimate);

	// Error checking
	checkCompilationLog((char*)"gGeometryShaderObject_2_krishnaAnimate", gGeometryShaderObject_2_krishnaAnimate);

	//FRAGMENT SHADER

	gFragmentShaderObject_2_krishnaAnimate = glCreateShader(GL_FRAGMENT_SHADER);

	const GLchar* fragmentShaderSourceCode_2 =
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

		"uniform float time;" \

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
				"FragColor		= vec4(phong_ads_color,u_alpha);" \
			"}" \
			/*"FragColor.a = 1 - (time / 20.0f);" \*/
			"FragColor.a = 1;" \
		"}";

	glShaderSource(gFragmentShaderObject_2_krishnaAnimate, 1, (const GLchar**)&fragmentShaderSourceCode_2, NULL);
	glCompileShader(gFragmentShaderObject_2_krishnaAnimate);

	// Error checking
	checkCompilationLog((char*)"gFragmentShaderObject_krishnaAnimate", gFragmentShaderObject_2_krishnaAnimate);


	//Shader Program

	gShaderProgramObject_2_krishnaAnimate = glCreateProgram();

	glAttachShader(gShaderProgramObject_2_krishnaAnimate, gVertexShaderObject_krishnaAnimate);
	glAttachShader(gShaderProgramObject_2_krishnaAnimate, gGeometryShaderObject_2_krishnaAnimate);
	glAttachShader(gShaderProgramObject_2_krishnaAnimate, gFragmentShaderObject_2_krishnaAnimate);

	glBindAttribLocation(gShaderProgramObject_2_krishnaAnimate, MATRIX_ATTRIBUTE_POSITION, "vPosition");
	glBindAttribLocation(gShaderProgramObject_2_krishnaAnimate, MATRIX_ATTRIBUTE_NORMAL, "vNormal");
	glBindAttribLocation(gShaderProgramObject_2_krishnaAnimate, MATRIX_ATTRIBUTE_TEXTURE0, "vTexcoord");

	glLinkProgram(gShaderProgramObject_2_krishnaAnimate);

	// Error checking
	checkLinkLog((char*)"gShaderProgramObject_2_krishnaAnimate", gShaderProgramObject_2_krishnaAnimate);


	gModelMatrixUniform_krishnaAnimate		= glGetUniformLocation(gShaderProgramObject_2_krishnaAnimate, "u_model_matrix");
	gViewMatrixUniform_krishnaAnimate		= glGetUniformLocation(gShaderProgramObject_2_krishnaAnimate, "u_view_matrix");
	gProjectionMatrixUniform_krishnaAnimate = glGetUniformLocation(gShaderProgramObject_2_krishnaAnimate, "u_projection_matrix");

	gLKeyPressedUniform_krishnaAnimate = glGetUniformLocation(gShaderProgramObject_2_krishnaAnimate, "u_LKeyPressed");

	gLdUniform_krishnaAnimate = glGetUniformLocation(gShaderProgramObject_2_krishnaAnimate, "u_Ld");
	gLaUniform_krishnaAnimate = glGetUniformLocation(gShaderProgramObject_2_krishnaAnimate, "u_La");
	gLsUniform_krishnaAnimate = glGetUniformLocation(gShaderProgramObject_2_krishnaAnimate, "u_Ls");

	gKdUniform_krishnaAnimate = glGetUniformLocation(gShaderProgramObject_2_krishnaAnimate, "u_Kd");
	gKsUniform_krishnaAnimate = glGetUniformLocation(gShaderProgramObject_2_krishnaAnimate, "u_Ks");
	gKaUniform_krishnaAnimate = glGetUniformLocation(gShaderProgramObject_2_krishnaAnimate, "u_Ka");

	gLightPositionUniform_krishnaAnimate		= glGetUniformLocation(gShaderProgramObject_2_krishnaAnimate, "u_light_position");
	gMaterialShininessUniform_krishnaAnimate	= glGetUniformLocation(gShaderProgramObject_2_krishnaAnimate, "u_material_shininess");

	gTextureSamplerUniform_krishnaAnimate = glGetUniformLocation(gShaderProgramObject_2_krishnaAnimate, "u_texture0_sampler");

	gTextureActiveUniform_krishnaAnimate	= glGetUniformLocation(gShaderProgramObject_2_krishnaAnimate, "u_is_texture");
	gAlphaUniform_krishnaAnimate			= glGetUniformLocation(gShaderProgramObject_2_krishnaAnimate, "u_alpha");

	gTimeUniform_krishnaAnimate = glGetUniformLocation(gShaderProgramObject_2_krishnaAnimate, "time");
#pragma endregion

}



void initialize_krishnaAnimate()
{
	initKrishnaAnimate();

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

void display_krishnaAnimate()
{
	if (startJoin_krishnaAnimate)
	{
		if (ftime_krishnaAnimate < 0.1f)
		{
			glUseProgram(0);
			glUseProgram(gShaderProgramObject_krishnaAnimate);
		}
		else
		{
			glUseProgram(0);
			glUseProgram(gShaderProgramObject_2_krishnaAnimate);
		
			glEnable(GL_ALPHA_TEST);
			glAlphaFunc(GL_NOTEQUAL, 0);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
			glEnable(GL_POINT_SMOOTH);
			glPointSize(2.0);
		}
		gfKrishnaModelScale = 250.0f;
	}
	else
	{
		gfKrishnaModelScale = 70.0f;
		glUseProgram(0);
		glUseProgram(gShaderProgramObject_krishnaAnimate);
	}

	glUniform1i(gLKeyPressedUniform_krishnaAnimate, 1);

	glUniform3fv(gLdUniform_krishnaAnimate, 1, lightDiffuse_krishnaAnimate);
	glUniform3fv(gLaUniform_krishnaAnimate, 1, lightAmbient_krishnaAnimate);
	glUniform3fv(gLsUniform_krishnaAnimate, 1, lightSpecular_krishnaAnimate);
	glUniform4fv(gLightPositionUniform_krishnaAnimate, 1, lightPosition_krishnaAnimate);
	

	mat4 modelMatrix	 = mat4::identity();
	//mat4 viewMatrix	= mat4::identity();
	mat4 scaleMatrix	= mat4::identity();
	mat4 rotateMatrix	= mat4::identity();

	//gViewMatrix = lookat(vmath_camera_eye_coord, vmath_camera_center_coord, vmath_camera_up_coord);
	glUniformMatrix4fv(gViewMatrixUniform_krishnaAnimate, 1, GL_FALSE, gViewMatrix);
	glUniformMatrix4fv(gProjectionMatrixUniform_krishnaAnimate, 1, GL_FALSE, gPerspectiveProjectionMatrix);

	
	// KRISHNA MODEL
	modelMatrix		= mat4::identity();
	scaleMatrix		= mat4::identity();
	modelMatrix		= vmath::translate(-300.0f, 55.0f, 1.0f);
	//modelMatrix		= vmath::translate(-300.0f, 120.0f, 1.0f);
	
	scaleMatrix		= scale(gfKrishnaModelScale, gfKrishnaModelScale, gfKrishnaModelScale);
	rotateMatrix	= rotate(90.0f, 0.0f, 1.0f, 0.0f);
	
	modelMatrix = modelMatrix * rotateMatrix * scaleMatrix;
	
	glUniformMatrix4fv(gModelMatrixUniform_krishnaAnimate, 1, GL_FALSE, modelMatrix);
	glUniform1f(gTimeUniform_krishnaAnimate, ftime_krishnaAnimate);

	glBindVertexArray(gModel_Krishna.Vao);
	for (int i = 0; i < gModel_Krishna.model_mesh_data.size(); i++)
	{
		if (gbLight == true)
		{
			glUniform3fv(gKaUniform_krishnaAnimate, 1, gModel_Krishna.model_material[gModel_Krishna.model_mesh_data[i].material_index].Ka);
			glUniform3fv(gKdUniform_krishnaAnimate, 1, gModel_Krishna.model_material[gModel_Krishna.model_mesh_data[i].material_index].Kd);
			glUniform3fv(gKsUniform_krishnaAnimate, 1, gModel_Krishna.model_material[gModel_Krishna.model_mesh_data[i].material_index].Ks);
			glUniform1f(gMaterialShininessUniform_krishnaAnimate, material_shininess);
			glUniform1f(gAlphaUniform_krishnaAnimate, gModel_Krishna.model_material[gModel_Krishna.model_mesh_data[i].material_index].d);
			if (gModel_Krishna.model_material[gModel_Krishna.model_mesh_data[i].material_index].ismap_Kd == true)
			{
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, gModel_Krishna.model_material[gModel_Krishna.model_mesh_data[i].material_index].gTexture);
				glUniform1i(gTextureSamplerUniform_krishnaAnimate, 0);
				glUniform1i(gTextureActiveUniform_krishnaAnimate, 1);
			}
			else
				glUniform1i(gTextureActiveUniform_krishnaAnimate, 0);
		}
		glDrawArrays(GL_TRIANGLES, gModel_Krishna.model_mesh_data[i].vertex_Index, gModel_Krishna.model_mesh_data[i].vertex_Count);
	}
	glBindVertexArray(0);

	glUseProgram(0);

	glDisable(GL_ALPHA_TEST);
	glDisable(GL_BLEND);
	glDisable(GL_POINT_SMOOTH);
	
}

void update_krishnaAnimate()
{
	if (startJoin_krishnaAnimate)
	{
		if (ftime_krishnaAnimate > 0.0f)
		{
			ftime_krishnaAnimate = ftime_krishnaAnimate - 0.3f;
		}
	}
}

void uninitialize_krishnaAnimate()
{
	uninitializeAllModelData();

	uninitializeSphereShape();

	programObjectSafeRelease(gShaderProgramObject_krishnaAnimate);

}