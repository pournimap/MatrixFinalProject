#pragma once

#include "../Include/BasicShapeLoader/Model/Matrix_Obj_Loader.h"

GLuint gVertexShaderObject_perFragmentLight;
GLuint gFragmentShaderObject_perFragmentLight;
GLuint gShaderProgramObject_perFragmentLight;


GLuint gModelMatrixUniform_perFragmentLight;
GLuint gViewMatrixUniform_perFragmentLight;
GLuint gProjectionMatrixUniform_perFragmentLight;
GLuint gLdUniform_perFragmentLight, gLaUniform_perFragmentLight, gLsUniform_perFragmentLight;
GLuint gLightPositionUniform_perFragmentLight;
GLuint gKdUniform_perFragmentLight, gKaUniform_perFragmentLight, gKsUniform_perFragmentLight;
GLuint gMaterialShininessUniform_perFragmentLight;
GLuint gLKeyPressedUniform_perFragmentLight;
GLuint gTextureSamplerUniform_perFragmentLight, gTextureActiveUniform_perFragmentLight, gAlphaUniform_perFragmentLight;


GLfloat lightAmbient[] = { 0.0f,0.0f,0.0f,1.0f };
GLfloat lightDiffuse[] = { 1.0f,1.0f,1.0f,1.0f };
GLfloat lightSpecular[] = { 1.0f,1.0f,1.0f,1.0f };
GLfloat lightPosition[] = { 100.0f,100.0f,100.0f,1.0f };

GLfloat material_ambient[] = { 0.0f,0.0f,0.0f,1.0f };
GLfloat material_diffuse[] = { 1.0f,1.0f,1.0f,1.0f };
GLfloat material_specular[] = { 1.0f,1.0f,1.0f,1.0f };
GLfloat material_shininess = 50.0f;

// animation variable
bool gbIsAnimate = false;
GLfloat gfKrishnaModelScale = 20.0f;





// FOR CAMERA
vec3 vmath_camera_eye_coord		= { -500.0f, 0.0f,1.0f };
vec3 vmath_camera_center_coord	= { -1000.0f,0.0f,0.0f };
vec3 vmath_camera_up_coord		= { 0.0f,1.0f,0.0f };

bool gbStartCamera			= false;
bool gbZoomOutForFullView	= false;



void initPerFragmentShader()
{
	void uninitialize(int);

	//VERTEX SHADER
	gVertexShaderObject_perFragmentLight = glCreateShader(GL_VERTEX_SHADER);

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

		"void main(void)" \
		"{" \
		"if(u_LKeyPressed == 1)" \
		"{" \
		"vec4 eye_coordinates=u_view_matrix * u_model_matrix * vPosition;" \
		"transformed_normals=mat3(u_view_matrix * u_model_matrix) * vNormal;" \
		"light_direction = vec3(u_light_position) - eye_coordinates.xyz;" \
		"viewer_vector = -eye_coordinates.xyz;" \
		"}"\

		"out_texcord = vTexcoord;" \
		"gl_Position = u_projection_matrix * u_view_matrix * u_model_matrix * vPosition;" \
		"}";

	glShaderSource(gVertexShaderObject_perFragmentLight, 1, (const GLchar**)&vertextShaderSourceCode, NULL);

	//compile shader
	glCompileShader(gVertexShaderObject_perFragmentLight);

	// Error checking
	checkCompilationLog((char*)"gVertexShaderObject_perFragmentLight", gVertexShaderObject_perFragmentLight);


	//FRAGMENT SHADER

	gFragmentShaderObject_perFragmentLight = glCreateShader(GL_FRAGMENT_SHADER);

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

		"void main(void)" \
		"{" \
		"vec3 phong_ads_color;" \
		"vec4 Final_Texture;" \
		"if(u_LKeyPressed==1)" \
		"{" \
		"vec3 normalized_transformed_normals=normalize(transformed_normals);" \
		"vec3 normalized_light_direction=normalize(light_direction);" \
		"vec3 normalized_viewer_vector=normalize(viewer_vector);" \
		"vec3 ambient = u_La * u_Ka;" \
		"float tn_dot_ld = max(dot(normalized_transformed_normals, normalized_light_direction),0.0);" \
		"vec3 diffuse = u_Ld * u_Kd * tn_dot_ld;" \
		"vec3 reflection_vector = reflect(-normalized_light_direction, normalized_transformed_normals);" \
		"vec3 specular = u_Ls * u_Ks * pow(max(dot(reflection_vector, normalized_viewer_vector), 0.0), u_material_shininess);" \
		"phong_ads_color=ambient + diffuse + specular;" \
		"}" \
		"else" \
		"{" \
		"phong_ads_color = vec3(1.0, 1.0, 1.0);" \
		"}" \
		"if(u_is_texture == 1)" \
		"{" \
		"Final_Texture = texture(u_texture0_sampler, out_texcord); " \
		"FragColor = vec4(phong_ads_color, u_alpha) * Final_Texture;" \
		"}" \
		"else" \
		"{" \
		"FragColor = vec4(phong_ads_color, u_alpha);" \
		"}" \
		"}";

	glShaderSource(gFragmentShaderObject_perFragmentLight, 1, (const GLchar**)&fragmentShaderSourceCode, NULL);

	glCompileShader(gFragmentShaderObject_perFragmentLight);

	// Error checking
	checkCompilationLog((char*)"gFragmentShaderObject_perFragmentLight", gFragmentShaderObject_perFragmentLight);


	//Shader Program

	gShaderProgramObject_perFragmentLight = glCreateProgram();

	glAttachShader(gShaderProgramObject_perFragmentLight, gVertexShaderObject_perFragmentLight);

	glAttachShader(gShaderProgramObject_perFragmentLight, gFragmentShaderObject_perFragmentLight);

	glBindAttribLocation(gShaderProgramObject_perFragmentLight, MATRIX_ATTRIBUTE_POSITION, "vPosition");
	glBindAttribLocation(gShaderProgramObject_perFragmentLight, MATRIX_ATTRIBUTE_NORMAL, "vNormal");
	glBindAttribLocation(gShaderProgramObject_perFragmentLight, MATRIX_ATTRIBUTE_TEXTURE0, "vTexcoord");

	glLinkProgram(gShaderProgramObject_perFragmentLight);

	// Error checking
	checkLinkLog((char*)"gShaderProgramObject_perFragmentLight", gShaderProgramObject_perFragmentLight);


	gModelMatrixUniform_perFragmentLight = glGetUniformLocation(gShaderProgramObject_perFragmentLight, "u_model_matrix");
	gViewMatrixUniform_perFragmentLight = glGetUniformLocation(gShaderProgramObject_perFragmentLight, "u_view_matrix");
	gProjectionMatrixUniform_perFragmentLight = glGetUniformLocation(gShaderProgramObject_perFragmentLight, "u_projection_matrix");

	gLKeyPressedUniform_perFragmentLight = glGetUniformLocation(gShaderProgramObject_perFragmentLight, "u_LKeyPressed");

	gLdUniform_perFragmentLight = glGetUniformLocation(gShaderProgramObject_perFragmentLight, "u_Ld");
	gLaUniform_perFragmentLight = glGetUniformLocation(gShaderProgramObject_perFragmentLight, "u_La");
	gLsUniform_perFragmentLight = glGetUniformLocation(gShaderProgramObject_perFragmentLight, "u_Ls");

	gKdUniform_perFragmentLight = glGetUniformLocation(gShaderProgramObject_perFragmentLight, "u_Kd");
	gKsUniform_perFragmentLight = glGetUniformLocation(gShaderProgramObject_perFragmentLight, "u_Ks");
	gKaUniform_perFragmentLight = glGetUniformLocation(gShaderProgramObject_perFragmentLight, "u_Ka");

	gLightPositionUniform_perFragmentLight = glGetUniformLocation(gShaderProgramObject_perFragmentLight, "u_light_position");
	gMaterialShininessUniform_perFragmentLight = glGetUniformLocation(gShaderProgramObject_perFragmentLight, "u_material_shininess");

	gTextureSamplerUniform_perFragmentLight = glGetUniformLocation(gShaderProgramObject_perFragmentLight, "u_texture0_sampler");

	gTextureActiveUniform_perFragmentLight = glGetUniformLocation(gShaderProgramObject_perFragmentLight, "u_is_texture");
	gAlphaUniform_perFragmentLight = glGetUniformLocation(gShaderProgramObject_perFragmentLight, "u_alpha");
}


void initialize_perFragmentLight()
{
	initPerFragmentShader();

	initSphereShape();

	LoadAllModels();

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

void display_perFragmentLight()
{
	glUseProgram(gShaderProgramObject_perFragmentLight);

	if (gbLight == true)
	{
		glUniform1i(gLKeyPressedUniform_perFragmentLight, 1);

		glUniform3fv(gLdUniform_perFragmentLight, 1, lightDiffuse);//white
		glUniform3fv(gLaUniform_perFragmentLight, 1, lightAmbient);
		glUniform3fv(gLsUniform_perFragmentLight, 1, lightSpecular);
		glUniform4fv(gLightPositionUniform_perFragmentLight, 1, lightPosition);

		/*glUniform3fv(gKdUniform_perFragmentLight, 1, material_diffuse);
		glUniform3fv(gKaUniform_perFragmentLight, 1, material_ambient);
		glUniform3fv(gKsUniform_perFragmentLight, 1, material_specular);
		glUniform1f(gMaterialShininessUniform_perFragmentLight, material_shininess);*/

	}
	else
	{
		glUniform1i(gLKeyPressedUniform_perFragmentLight, 0);
	}

	mat4 modelMatrix = mat4::identity();
	//glm::mat4 viewMatrix = glm::mat4();
	mat4 viewMatrix = mat4::identity();
	mat4 scaleMatrix = mat4::identity();
	mat4 rotateMatrix = mat4::identity();

	//viewMatrix = Scene_camera.GetViewMatrix();
	//glUniformMatrix4fv(gViewMatrixUniform_perFragmentLight, 1, GL_FALSE, &viewMatrix[0][0]);
	
	viewMatrix = lookat(vmath_camera_eye_coord, vmath_camera_center_coord, vmath_camera_up_coord);
	glUniformMatrix4fv(gViewMatrixUniform_perFragmentLight, 1, GL_FALSE, viewMatrix);

	modelMatrix = vmath::translate(0.0f, 0.0f, -2.0f);

	glUniformMatrix4fv(gProjectionMatrixUniform_perFragmentLight, 1, GL_FALSE, gPerspectiveProjectionMatrix);


	
	//object
	//Draw Mahal Model
	modelMatrix = mat4::identity();
	modelMatrix = vmath::translate(0.0f, -2.0f, -6.0f);
	scaleMatrix = scale(10.0f, 10.0f, 10.0f);
	modelMatrix = modelMatrix * scaleMatrix;
	glUniformMatrix4fv(gModelMatrixUniform_perFragmentLight, 1, GL_FALSE, modelMatrix);
	glBindVertexArray(gModel_Mahal.Vao);

	for (int i = 0; i < gModel_Mahal.model_mesh_data.size(); i++)
	{
		if (gbLight == true)
		{
			glUniform3fv(gKaUniform_perFragmentLight, 1, gModel_Mahal.model_material[gModel_Mahal.model_mesh_data[i].material_index].Ka);
			glUniform3fv(gKdUniform_perFragmentLight, 1, gModel_Mahal.model_material[gModel_Mahal.model_mesh_data[i].material_index].Kd);
			glUniform3fv(gKsUniform_perFragmentLight, 1, gModel_Mahal.model_material[gModel_Mahal.model_mesh_data[i].material_index].Ks);
			glUniform1f(gMaterialShininessUniform_perFragmentLight, material_shininess);
			glUniform1f(gAlphaUniform_perFragmentLight, gModel_Mahal.model_material[gModel_Mahal.model_mesh_data[i].material_index].d);

			if (gModel_Mahal.model_material[gModel_Mahal.model_mesh_data[i].material_index].ismap_Kd == true)
			{
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, gModel_Mahal.model_material[gModel_Mahal.model_mesh_data[i].material_index].gTexture);
				glUniform1i(gTextureSamplerUniform_perFragmentLight, 0);
				glUniform1i(gTextureActiveUniform_perFragmentLight, 1);
				/*fprintf(gpFile, "perFragmentLight Mahal ismapKd true \n");
				fflush(gpFile);*/
			}
			else
				glUniform1i(gTextureActiveUniform_perFragmentLight, 0);
		}
		glDrawArrays(GL_TRIANGLES, gModel_Mahal.model_mesh_data[i].vertex_Index, gModel_Mahal.model_mesh_data[i].vertex_Count);
	}
	glBindVertexArray(0);
	
	
	modelMatrix = mat4::identity();
	modelMatrix = vmath::translate(-700.0f, 0.0f, 1.0f);
	scaleMatrix = scale(gfKrishnaModelScale, gfKrishnaModelScale, gfKrishnaModelScale);
	rotateMatrix = rotate(90.0f, 0.0f, 1.0f, 0.0f);

	modelMatrix = modelMatrix * rotateMatrix * scaleMatrix;
	glUniformMatrix4fv(gModelMatrixUniform_perFragmentLight, 1, GL_FALSE, modelMatrix);
	//Draw Krishna Model
	glBindVertexArray(gModel_Krishna.Vao);
	for (int i = 0; i < gModel_Krishna.model_mesh_data.size(); i++)
	{
		if (gbLight == true)
		{
			glUniform3fv(gKaUniform_perFragmentLight, 1, gModel_Krishna.model_material[gModel_Krishna.model_mesh_data[i].material_index].Ka);
			glUniform3fv(gKdUniform_perFragmentLight, 1, gModel_Krishna.model_material[gModel_Krishna.model_mesh_data[i].material_index].Kd);
			glUniform3fv(gKsUniform_perFragmentLight, 1, gModel_Krishna.model_material[gModel_Krishna.model_mesh_data[i].material_index].Ks);
			glUniform1f(gMaterialShininessUniform_perFragmentLight, material_shininess);
			glUniform1f(gAlphaUniform_perFragmentLight, gModel_Krishna.model_material[gModel_Krishna.model_mesh_data[i].material_index].d);
			//glUniform1f(gAlphaUniform_perFragmentLight, 0.5f);
			if (gModel_Krishna.model_material[gModel_Krishna.model_mesh_data[i].material_index].ismap_Kd == true)
			{
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, gModel_Krishna.model_material[gModel_Krishna.model_mesh_data[i].material_index].gTexture);
				glUniform1i(gTextureSamplerUniform_perFragmentLight, 0);
				glUniform1i(gTextureActiveUniform_perFragmentLight, 1);
				/*fprintf(gpFile, "perFragmentLight Krishna ismapKd true \n");
				fflush(gpFile);*/
			}
			else
				glUniform1i(gTextureActiveUniform_perFragmentLight, 0);
		}
		glDrawArrays(GL_TRIANGLES, gModel_Krishna.model_mesh_data[i].vertex_Index, gModel_Krishna.model_mesh_data[i].vertex_Count);
		/*
		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_NOTEQUAL, 0);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
		glEnable(GL_POINT_SMOOTH);
		glPointSize(5.0f);
		glDrawArrays(GL_POINTS, gModel_Krishna.model_mesh_data[i].vertex_Index, gModel_Krishna.model_mesh_data[i].vertex_Count);
		*/
	}
	glBindVertexArray(0);


	glUseProgram(0);
}

void update_perFragmentLight()
{
	if (gbIsAnimate)
	{
		if (gfKrishnaModelScale < 220.0f)
		{
			gfKrishnaModelScale = gfKrishnaModelScale + 0.1f;
		}
	}
	if (gbStartCamera)
	{
		
		if (vmath_camera_eye_coord[1] < 660.0f && vmath_camera_center_coord[1] < 900.0f &&  gbZoomOutForFullView == false)
		{
			vmath_camera_eye_coord[1] = vmath_camera_eye_coord[1] + (660.0f / 1000.0f);
			vmath_camera_center_coord[1] = vmath_camera_center_coord[1] + (900.0f / 1000.0f);
		}
		else
		{
			gbZoomOutForFullView = true;
		}
		
		if (gbZoomOutForFullView == true)
		{
			if (vmath_camera_eye_coord[0] < 340.0f)
			{
				vmath_camera_eye_coord[0] = vmath_camera_eye_coord[0] + ((500.0f + 340.0f) / 1000.0f);			// -500.0f is old position , so old position to final position in 1000 steps
			}
			if (vmath_camera_eye_coord[2] < 11.0f)
			{
				vmath_camera_eye_coord[2] = vmath_camera_eye_coord[2] + (10.0f / 1000.0f);			// 11 final pos, 1 old pos so 10 / 1000 steps
			}
			if (vmath_camera_center_coord[1] > 370.0f)
			{
				vmath_camera_center_coord[1] = vmath_camera_center_coord[1] - ((900.0f - 370.0f) / 1000.0f);
			}
		}
		
	}
}

void uninitialize_perFragmentLight()
{
	uninitializeAllModelData();

	uninitializeSphereShape();

	programObjectSafeRelease(gShaderProgramObject_perFragmentLight);

}