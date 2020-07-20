#pragma once

#include "../Include/BasicShapeLoader/Model/BasicOBJLoader/Matrix_Obj_Loader.h"

//function declaration
void initialize_perFragmentLight(void);
void initShadowDepthShader(void);

void display_Matrix_X(void);
void display_perFragmentLight(void);


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
GLuint applyBloomUniform_perFragmentLight, u_bloom_is_activeUniform_perFragmentLight;
GLuint bloom_thresh_minUniform_perFragmentLight, bloom_thresh_maxUniform_perFragmentLight;
GLuint fadeinFactor_perFragmentLight, fadeoutFactor_perFragmentLight;

GLfloat lightAmbient[] = { 0.0f,0.0f,0.0f,1.0f };
GLfloat lightDiffuse[] = { 1.0f,1.0f,1.0f,1.0f };
GLfloat lightSpecular[] = { 1.0f,1.0f,1.0f,1.0f };
GLfloat lightPosition[] = { 100.0f,100.0f,100.0f,1.0f };

GLfloat material_ambient[] = { 0.0f,0.0f,0.0f,1.0f };
GLfloat material_diffuse[] = { 1.0f,1.0f,1.0f,1.0f };
GLfloat material_specular[] = { 1.0f,1.0f,1.0f,1.0f };
GLfloat material_shininess = 50.0f;

vec3 vmath_camera_eye_coord = { 3000.0f, 600.0f, 0.0f };
vec3 vmath_camera_center_coord = { 0.0f,-415.0f,0.0f };
vec3 vmath_camera_up_coord = { 0.0f,1.0f,0.0f };

bool gbStartCamera			= false;
bool gbZoomOutForFullView	= false;
bool gbGoToFullViewKrishna	= false;

// -250.0f, 790.0f, 480.0f
GLfloat XForSudarshan = -250.0f;
GLfloat YForSudarshan = 790.0f;
GLfloat ZForSudarshan = 480.0f;

bool gbStartAnimationOfSudarshan = false;

float FadeOutFactor_perFragmentLight = 1.0f;
float FadeInFactor_perFragmentLight = 0.0f;

bool showEmptyMahal = false;
bool showEmptyMahalGoInDone = false;
bool showEmptyMahalGoRtToLeftDone = false;
bool showEmptyMahalGoOutDone = false;
bool showEmptyMahalGoInVer2Done = false;
bool showEmptyMahalGoOutVer2Done = false;
bool GoKrishnaCloserLook = false;


void initialize_perFragmentLight(void)
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

		"layout (location = 0) out vec4 FragColor;" \
		"layout (location = 1) out vec4 BloomColor;" \
		"layout (location = 2) out vec4 GodRaysColor; " \

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
		"uniform int applyBloom;" \
		"uniform float bloom_thresh_min = 0.8f;" \
		"uniform float bloom_thresh_max = 1.2f;" \
		"uniform int u_bloom_is_active;" \

		"uniform float fadeinFactor;" \
		"uniform float fadeoutFactor;" \

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
				"FragColor = vec4(phong_ads_color, u_alpha) * Final_Texture * fadeinFactor * fadeoutFactor;" \
			"}" \
			"else" \
			"{" \
				"FragColor = vec4(phong_ads_color, u_alpha) * fadeinFactor * fadeoutFactor;" \
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

	gLightPositionUniform_perFragmentLight		= glGetUniformLocation(gShaderProgramObject_perFragmentLight, "u_light_position");
	gMaterialShininessUniform_perFragmentLight	= glGetUniformLocation(gShaderProgramObject_perFragmentLight, "u_material_shininess");

	gTextureSamplerUniform_perFragmentLight = glGetUniformLocation(gShaderProgramObject_perFragmentLight, "u_texture0_sampler");

	gTextureActiveUniform_perFragmentLight	= glGetUniformLocation(gShaderProgramObject_perFragmentLight, "u_is_texture");
	gAlphaUniform_perFragmentLight			= glGetUniformLocation(gShaderProgramObject_perFragmentLight, "u_alpha");
	applyBloomUniform_perFragmentLight = glGetUniformLocation(gShaderProgramObject_perFragmentLight, "applyBloom");
	u_bloom_is_activeUniform_perFragmentLight = glGetUniformLocation(gShaderProgramObject_perFragmentLight, "u_bloom_is_active");
	bloom_thresh_minUniform_perFragmentLight = glGetUniformLocation(gShaderProgramObject_perFragmentLight, "bloom_thresh_min");
	bloom_thresh_maxUniform_perFragmentLight = glGetUniformLocation(gShaderProgramObject_perFragmentLight, "bloom_thresh_max");

	fadeinFactor_perFragmentLight = glGetUniformLocation(gShaderProgramObject_perFragmentLight, "fadeinFactor");
	fadeoutFactor_perFragmentLight = glGetUniformLocation(gShaderProgramObject_perFragmentLight, "fadeoutFactor");

}

void display_Matrix_X(void)
{
	glUseProgram(gShaderProgramObject_perFragmentLight);
	glUniform1f(fadeoutFactor_perFragmentLight, 1.0f);
	glUniform1f(fadeinFactor_perFragmentLight, 1.0f);

	glUniform1i(u_bloom_is_activeUniform_perFragmentLight, (GLint)1);
	glUniform1f(bloom_thresh_minUniform_perFragmentLight, bloom_thresh_min);
	glUniform1f(bloom_thresh_maxUniform_perFragmentLight, bloom_thresh_max);

	if (gbLight == true)
	{
		glUniform1i(gLKeyPressedUniform_perFragmentLight, 1);

		glUniform3fv(gLdUniform_perFragmentLight, 1, lightDiffuse);//white
		glUniform3fv(gLaUniform_perFragmentLight, 1, lightAmbient);
		glUniform3fv(gLsUniform_perFragmentLight, 1, lightSpecular);
		glUniform4fv(gLightPositionUniform_perFragmentLight, 1, lightPosition);

		glUniform3fv(gKaUniform_perFragmentLight, 1, material_ambient);
		glUniform3fv(gKdUniform_perFragmentLight, 1, vec3(0.117647, 0.772549, 0.011764));
		glUniform3fv(gKsUniform_perFragmentLight, 1, material_specular);
		glUniform1f(gMaterialShininessUniform_perFragmentLight, material_shininess);
		glUniform1f(gAlphaUniform_perFragmentLight, 1.0f);

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

	gViewMatrix = lookat(vmath_camera_eye_coord, vmath_camera_center_coord, vmath_camera_up_coord);
	glUniformMatrix4fv(gViewMatrixUniform_perFragmentLight, 1, GL_FALSE, viewMatrix);
	glUniformMatrix4fv(gProjectionMatrixUniform_perFragmentLight, 1, GL_FALSE, gPerspectiveProjectionMatrix);

	glUniform1i(applyBloomUniform_perFragmentLight, 1);

	modelMatrix = vmath::translate(-13.0f, 5.0f, -100.0f);
	scaleMatrix = scale(1.0f, 5.0f, 1.0f);
	rotateMatrix = rotate(30.0f, 0.0f, 0.0f, 1.0f);
	//rotateMatrix = rotate(gAngle_rotateY, 0.0f, 1.0f, 0.0f);
	modelMatrix = modelMatrix * rotateMatrix * scaleMatrix;

	glUniformMatrix4fv(gModelMatrixUniform_perFragmentLight, 1, GL_FALSE, modelMatrix);

	drawCubeShape();

	modelMatrix = vmath::translate(-13.0f, 5.0f, -100.0f);
	scaleMatrix = scale(1.0f, 5.0f, 1.0f);
	rotateMatrix = rotate(-30.0f, 0.0f, 0.0f, 1.0f);
	//rotateMatrix = rotate(gAngle_rotateY, 0.0f, 1.0f, 0.0f);
	modelMatrix = modelMatrix * rotateMatrix * scaleMatrix;

	glUniformMatrix4fv(gModelMatrixUniform_perFragmentLight, 1, GL_FALSE, modelMatrix);

	drawCubeShape();

	glUseProgram(0);
}

void display_sudarshanChakra(void)
{
	static const GLfloat one = { 1.0f };

	glUseProgram(gShaderProgramObject_perFragmentLight);
	glUniform1f(fadeoutFactor_perFragmentLight, 1.0f);
	glUniform1f(fadeinFactor_perFragmentLight, 1.0f);

	glUniform1i(u_bloom_is_activeUniform_perFragmentLight, (GLint)one);
	glUniform1f(bloom_thresh_minUniform_perFragmentLight, bloom_thresh_min);
	glUniform1f(bloom_thresh_maxUniform_perFragmentLight, bloom_thresh_max);
	if (gbLight == true)
	{
		glUniform1i(gLKeyPressedUniform_perFragmentLight, 1);

		glUniform3fv(gLdUniform_perFragmentLight, 1, lightDiffuse);//white
		glUniform3fv(gLaUniform_perFragmentLight, 1, lightAmbient);
		glUniform3fv(gLsUniform_perFragmentLight, 1, lightSpecular);
		glUniform4fv(gLightPositionUniform_perFragmentLight, 1, lightPosition);
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

	gViewMatrix = lookat(vmath_camera_eye_coord, vmath_camera_center_coord, vmath_camera_up_coord);
	glUniformMatrix4fv(gViewMatrixUniform_perFragmentLight, 1, GL_FALSE, viewMatrix);
	glUniformMatrix4fv(gProjectionMatrixUniform_perFragmentLight, 1, GL_FALSE, gPerspectiveProjectionMatrix);

	glUniform1i(applyBloomUniform_perFragmentLight, 0);

	glUniform1i(applyBloomUniform_perFragmentLight, 1);
	modelMatrix = mat4::identity();
	scaleMatrix = mat4::identity();
	rotateMatrix = mat4::identity();

	modelMatrix = vmath::translate(-5.0f, -20.0f, -30.0f);
	scaleMatrix = scale(10.0f, 10.0f, 10.0f);
	rotateMatrix = rotate(-25.0f, 0.0f, 0.0f, 1.0f);
	//rotateMatrix = rotate(gAngle_rotateY, 0.0f, 1.0f, 0.0f);
	modelMatrix = modelMatrix *  scaleMatrix;

	glUniformMatrix4fv(gModelMatrixUniform_perFragmentLight, 1, GL_FALSE, modelMatrix);
	glBindVertexArray(gModel_SudarshanChakra.Vao);
	for (int i = 0; i < gModel_SudarshanChakra.model_mesh_data.size(); i++)
	{
		if (gbLight == true)
		{
			glUniform3fv(gKaUniform_perFragmentLight, 1, gModel_SudarshanChakra.model_material[gModel_SudarshanChakra.model_mesh_data[i].material_index].Ka);
			glUniform3fv(gKdUniform_perFragmentLight, 1, gModel_SudarshanChakra.model_material[gModel_SudarshanChakra.model_mesh_data[i].material_index].Kd);
			glUniform3fv(gKsUniform_perFragmentLight, 1, gModel_SudarshanChakra.model_material[gModel_SudarshanChakra.model_mesh_data[i].material_index].Ks);
			glUniform1f(gMaterialShininessUniform_perFragmentLight, material_shininess);
			glUniform1f(gAlphaUniform_perFragmentLight, gModel_SudarshanChakra.model_material[gModel_SudarshanChakra.model_mesh_data[i].material_index].d);
			if (gModel_SudarshanChakra.model_material[gModel_SudarshanChakra.model_mesh_data[i].material_index].ismap_Kd == true)
			{
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, gModel_SudarshanChakra.model_material[gModel_SudarshanChakra.model_mesh_data[i].material_index].gTexture_diffuse);
				glUniform1i(gTextureSamplerUniform_perFragmentLight, 0);
				glUniform1i(gTextureActiveUniform_perFragmentLight, 1);
			}
			else
				glUniform1i(gTextureActiveUniform_perFragmentLight, 0);
		}
		glDrawArrays(GL_TRIANGLES, gModel_SudarshanChakra.model_mesh_data[i].vertex_Index, gModel_SudarshanChakra.model_mesh_data[i].vertex_Count);
	}
	glBindVertexArray(0);

}

void display_perFragmentLight(void)
{
	static const GLfloat one = { 1.0f };

	glUseProgram(gShaderProgramObject_perFragmentLight);

	if (FadeInFactor_perFragmentLight <= 1.0f)
		FadeInFactor_perFragmentLight += 0.001f;

	if (bStartFadeOutSecondScene == true)
	{
		if (FadeOutFactor_perFragmentLight >= 0.0f)
			FadeOutFactor_perFragmentLight -= 0.001f;
		else
		{
			bFadeOutSecondSceneDone = true;
			iShowEndScene = true;
		}
	}

	glUniform1f(fadeoutFactor_perFragmentLight, FadeOutFactor_perFragmentLight);
	glUniform1f(fadeinFactor_perFragmentLight, FadeInFactor_perFragmentLight);

	glUniform1i(u_bloom_is_activeUniform_perFragmentLight, (GLint)one);
	glUniform1f(bloom_thresh_minUniform_perFragmentLight, bloom_thresh_min);
	glUniform1f(bloom_thresh_maxUniform_perFragmentLight, bloom_thresh_max);

	if (gbLight == true)
	{
		glUniform1i(gLKeyPressedUniform_perFragmentLight, 1);

		glUniform3fv(gLdUniform_perFragmentLight, 1, lightDiffuse);//white
		glUniform3fv(gLaUniform_perFragmentLight, 1, lightAmbient);
		glUniform3fv(gLsUniform_perFragmentLight, 1, lightSpecular);
		glUniform4fv(gLightPositionUniform_perFragmentLight, 1, lightPosition);

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

	//viewMatrix = lookat(vmath_camera_eye_coord, vmath_camera_center_coord, vmath_camera_up_coord);
	gViewMatrix = lookat(vmath_camera_eye_coord, vmath_camera_center_coord, vmath_camera_up_coord);
	//glUniformMatrix4fv(gViewMatrixUniform_perFragmentLight, 1, GL_FALSE, viewMatrix);
	glUniformMatrix4fv(gViewMatrixUniform_perFragmentLight, 1, GL_FALSE, gViewMatrix);
	//modelMatrix = vmath::translate(0.0f, 0.0f, -2.0f);

	glUniformMatrix4fv(gProjectionMatrixUniform_perFragmentLight, 1, GL_FALSE, gPerspectiveProjectionMatrix);
	
	glUniform1i(applyBloomUniform_perFragmentLight, 0);

	if (isFirstScene == false)
	{
		static float gAngle_rotateY = 0.0f;
		if (isHandsUpDone == true)
		{
			if (gAngle_rotateY >= 360.0f)
				gAngle_rotateY = 0.0f;
			gAngle_rotateY += 0.1f;

			//sudarshan chakra
			glUniform1i(applyBloomUniform_perFragmentLight, 1);
			modelMatrix = mat4::identity();
			scaleMatrix = mat4::identity();
			rotateMatrix = mat4::identity();

			modelMatrix = vmath::translate(XForSudarshan, YForSudarshan, ZForSudarshan);
			scaleMatrix = scale(50.0f, 50.0f, 50.0f);
			rotateMatrix = rotate(-25.0f, 0.0f, 0.0f, 1.0f);
			//rotateMatrix = rotate(gAngle_rotateY, 0.0f, 1.0f, 0.0f);
			modelMatrix = modelMatrix * rotateMatrix * scaleMatrix;

			glUniformMatrix4fv(gModelMatrixUniform_perFragmentLight, 1, GL_FALSE, modelMatrix);
			glBindVertexArray(gModel_SudarshanChakra.Vao);
			for (int i = 0; i < gModel_SudarshanChakra.model_mesh_data.size(); i++)
			{
				if (gbLight == true)
				{
					glUniform3fv(gKaUniform_perFragmentLight, 1, gModel_SudarshanChakra.model_material[gModel_SudarshanChakra.model_mesh_data[i].material_index].Ka);
					glUniform3fv(gKdUniform_perFragmentLight, 1, gModel_SudarshanChakra.model_material[gModel_SudarshanChakra.model_mesh_data[i].material_index].Kd);
					glUniform3fv(gKsUniform_perFragmentLight, 1, gModel_SudarshanChakra.model_material[gModel_SudarshanChakra.model_mesh_data[i].material_index].Ks);
					glUniform1f(gMaterialShininessUniform_perFragmentLight, material_shininess);
					glUniform1f(gAlphaUniform_perFragmentLight, gModel_SudarshanChakra.model_material[gModel_SudarshanChakra.model_mesh_data[i].material_index].d);
					if (gModel_SudarshanChakra.model_material[gModel_SudarshanChakra.model_mesh_data[i].material_index].ismap_Kd == true)
					{
						glActiveTexture(GL_TEXTURE0);
						glBindTexture(GL_TEXTURE_2D, gModel_SudarshanChakra.model_material[gModel_SudarshanChakra.model_mesh_data[i].material_index].gTexture_diffuse);
						glUniform1i(gTextureSamplerUniform_perFragmentLight, 0);
						glUniform1i(gTextureActiveUniform_perFragmentLight, 1);
					}
					else
						glUniform1i(gTextureActiveUniform_perFragmentLight, 0);
				}
				glDrawArrays(GL_TRIANGLES, gModel_SudarshanChakra.model_mesh_data[i].vertex_Index, gModel_SudarshanChakra.model_mesh_data[i].vertex_Count);
			}
			glBindVertexArray(0);
		}
	}

	glUseProgram(0);
}

void update_perFragmentLight()
{
	if (isShowStartingScene == false)
	{
		if (showEmptyMahalGoInDone == false &&
			vmath_camera_eye_coord[0] > 380.0f && vmath_camera_eye_coord[1] > 250.0f && vmath_camera_eye_coord[2] > -510.0f &&
			vmath_camera_center_coord[0] > -630.0f && vmath_camera_center_coord[1] < 125.0f
			)
		{
			vmath_camera_eye_coord[0] -= (3000.0f - 380.0f) / 250.0f;
			vmath_camera_eye_coord[1] -= (600.0f - 250.0f) / 250.0f;
			vmath_camera_eye_coord[2] -= (510.0f - 0.0f) / 250.0f;

			vmath_camera_center_coord[0] -= (630.0f - 0.0f) / 250.0f;
			vmath_camera_center_coord[1] += (415.0f + 125.0f) / 250.0f;
		}
		else
		{
			showEmptyMahalGoInDone = true;

			if (showEmptyMahalGoRtToLeftDone == false &&
				vmath_camera_eye_coord[2] < 510.0f
				)
			{
				vmath_camera_eye_coord[2] += (510.0f + 510.0f) / 200.0f;
			}
			else
			{
				showEmptyMahalGoRtToLeftDone = true;

				if (showEmptyMahalGoOutDone == false &&
					vmath_camera_eye_coord[0] < 3000.0f && vmath_camera_eye_coord[1] < 600.0f && vmath_camera_eye_coord[2] > 0.0f &&
					vmath_camera_center_coord[0] < 0.0f && vmath_camera_center_coord[1] > -415.0f
					)
				{
					vmath_camera_eye_coord[0] += (3000.0f - 380.0f) / 250.0f;
					vmath_camera_eye_coord[1] += (600.0f - 250.0f) / 250.0f;
					vmath_camera_eye_coord[2] -= (510.0f - 0.0f) / 250.0f;

					vmath_camera_center_coord[0] += (630.0f - 0.0f) / 250.0f;
					vmath_camera_center_coord[1] -= (415.0f + 125.0f) / 250.0f;
				}
				else
				{
					showEmptyMahalGoOutDone = true;

					//yadny kunat fir ali ani raje basle
					/*isFireInYadnya = true;
					isRajeInSabha = true;*/
					

					if (showEmptyMahalGoInVer2Done == false &&
						vmath_camera_eye_coord[0] > 2090.0f && vmath_camera_eye_coord[1] > 200.0f
						)
					{
						vmath_camera_eye_coord[0] -= (3000.0f - 2090.0f) / 500.0f;
						vmath_camera_eye_coord[1] -= (600.0f - 200.0f) / 500.0f;
					}
					else
					{
						showEmptyMahalGoInVer2Done = true;
						
						

						if (showEmptyMahalGoOutVer2Done == false &&
							vmath_camera_eye_coord[0] < 2910.0f && vmath_camera_eye_coord[1] > 75.0f && vmath_camera_eye_coord[2] < 560.0f &&
							vmath_camera_center_coord[1] < -255.0f
							)
						{
							//krishna ala, shishupal alay
							isFirstScene = false;
							isAssimpAnimatedModelShow = true;

							vmath_camera_eye_coord[0] += (2910.0f - 2090.0f) / 800.0f;
							vmath_camera_eye_coord[1] -= (200.0f - 75.0f) / 800.0f;
							vmath_camera_eye_coord[2] += (560.0f - 0.0f) / 800.0f;

							vmath_camera_center_coord[1] += (415.0f - 255.0f) / 800.0f;
						}
						else
						{
							showEmptyMahalGoOutVer2Done = true;

							if (GoKrishnaCloserLook == false &&
								vmath_camera_eye_coord[0] > -60.0f && vmath_camera_eye_coord[1] < 180.0f && vmath_camera_eye_coord[2] > 330.0f &&
								vmath_camera_center_coord[0] > -250.0f && vmath_camera_center_coord[1] < 195.0f
								)
							{
								vmath_camera_eye_coord[0] -= (2910.0f + 60.0f) / 750.0f;
								vmath_camera_eye_coord[1] += (180.0f - 75.0f) / 750.0f;
								vmath_camera_eye_coord[2] -= (560.0f - 330.0f) / 750.0f;

								vmath_camera_center_coord[0] -= (250.0f - 0.0f) / 750.0f;
								vmath_camera_center_coord[1] += (255.0f + 195.0f) / 750.0f;
							}
							else
							{
								GoKrishnaCloserLook = true;
								
							}



						}

					}
				}
			}
		}
	}
	if (gbStartCamera)
	{
		//if (vmath_camera_eye_coord[0] < 585.0f && vmath_camera_eye_coord[1] > 105.0f && vmath_camera_eye_coord[2] > -340.0f && vmath_camera_center_coord[0] < -260.0f && vmath_camera_center_coord[1] < 440.0f && gbZoomOutForFullView == false)
		if (vmath_camera_eye_coord[0] < 600.0f && vmath_camera_eye_coord[1] > 15.0f && vmath_camera_eye_coord[2] > -340.0f && vmath_camera_center_coord[0] < -270.0f && vmath_camera_center_coord[1] < 490.0f && gbZoomOutForFullView == false)
		{
			vmath_camera_eye_coord[0]		= vmath_camera_eye_coord[0]		+ ((600.0f + 35.0f) / 3000.0f);				// -35.0f	: inital,585.0f : final, 2000 number of steps 
			vmath_camera_eye_coord[1]		= vmath_camera_eye_coord[1]		- ((105.0f - 15.0f)	/ 3000.0f);					// 60.0f	: final, 105.0f : initial
			vmath_camera_eye_coord[2]		= vmath_camera_eye_coord[2]		- ((340.0f + 10.0f)	/ 3000.0f);					// -240.0f	: final, 10.0f : initial

			vmath_camera_center_coord[0]	= vmath_camera_center_coord[0]	+ ((350.0f - 270.0f)	/ 3000.0f);					
			vmath_camera_center_coord[1]	= vmath_camera_center_coord[1]	+ ((490.0f - 100.0f)	/ 3000.0f);	
		}
		
	}
	if (gbGoToFullViewKrishna)
	{
		//0 key handle
		
		if (vmath_camera_eye_coord[0] < 1055.0f && vmath_camera_eye_coord[1] < 890.0f && vmath_camera_eye_coord[2] < -20.0f  && vmath_camera_center_coord[1] < 630.0f)
		{
			vmath_camera_eye_coord[0] = vmath_camera_eye_coord[0] + ((1055.0f - 600.0f) / 800.0f);				// -35.0f	: inital,585.0f : final, 2000 number of steps 
			vmath_camera_eye_coord[1] = vmath_camera_eye_coord[1] + ((890.0f - 15.0f) / 800.0f);					// 60.0f	: final, 105.0f : initial
			vmath_camera_eye_coord[2] = vmath_camera_eye_coord[2] + ((340.0f - 10.0f) / 800.0f);					// -240.0f	: final, 10.0f : initial

			//vmath_camera_center_coord[0] = vmath_camera_center_coord[0] + ((350.0f - 270.0f) / 3000.0f);
			vmath_camera_center_coord[1] = vmath_camera_center_coord[1] + ((630.0f - 490.0f) / 800.0f);
		}

	}

	// -250.0f, 790.0f, 480.0f
	if (gbStartAnimationOfSudarshan)
	{
		if (XForSudarshan < 885.0f && YForSudarshan > 650.0f && ZForSudarshan > 260.0f)
		{
			XForSudarshan = XForSudarshan + ((250.0f + 885.0f) / 500.0f);
			YForSudarshan = YForSudarshan - ((790.0f - 650.0f) / 500.0f);
			ZForSudarshan = ZForSudarshan - ((480.0f - 260.0f) / 500.0f);
		}
		else
		{
			bStartFadeOutSecondScene = true;
		}
	}
}

void uninitialize_perFragmentLight()
{
	programObjectSafeRelease(gShaderProgramObject_perFragmentLight);
}

GLuint gVertexShaderObject_ShadowDepthMap, gGeometryShaderObject_ShadowDepthMap, gFragmentShaderObject_ShadowDepthMap;
GLuint gShaderProgramObject_ShadowDepthMap;
GLuint gModelMatrixUniform_ShadowDepthMap, gLightPosUniform_ShadowDepthMap, gFarPlaneUniform_ShadowDepthMap;
GLuint gShadowMatricesUniform_ShadowDepthMap[6];

void initShadowDepthShader(void)
{
	//VERTEX SHADER
	gVertexShaderObject_ShadowDepthMap = glCreateShader(GL_VERTEX_SHADER);

	const GLchar* vertexShaderSourceCode_ShadowDepthMap =
		"#version 400" \
		"\n" \

		"in vec3 vPosition;" \

		"uniform mat4 u_model_matrix;" \

		"void main(void)" \
		"{" \
		"gl_Position = u_model_matrix * vec4(vPosition, 1.0);" \
		"}";

	glShaderSource(gVertexShaderObject_ShadowDepthMap, 1, (const GLchar**)&vertexShaderSourceCode_ShadowDepthMap, NULL);

	//compile shader
	glCompileShader(gVertexShaderObject_ShadowDepthMap);

	// Error checking
	checkCompilationLog((char*)"gVertexShaderObject_ShadowDepthMap", gVertexShaderObject_ShadowDepthMap);


	//GEOMETRY SHADER
	gGeometryShaderObject_ShadowDepthMap = glCreateShader(GL_GEOMETRY_SHADER);

	const GLchar* geometryShaderSourceCode_ShadowDepthMap =
		"#version 400" \
		"\n" \

		"layout (triangles) in;" \
		"layout (triangle_strip, max_vertices = 18) out;" \

		"uniform mat4 shadowMatrices[6];" \

		"out vec4 FragPos;" \

		"void main(void)" \
		"{" \
		"for(int face = 0; face < 6; ++face)" \
		"{" \
		// built-in variable that specifies to which face we render.
		"gl_Layer = face;" \
		"for(int i = 0; i < 3; ++i)" \
		"{" \
		"FragPos = gl_in[i].gl_Position;" \
		"gl_Position = shadowMatrices[face] * FragPos;" \
		"EmitVertex();" \
		"}" \
		"EndPrimitive();" \
		"}" \

		"}";

	glShaderSource(gGeometryShaderObject_ShadowDepthMap, 1, (const GLchar**)&geometryShaderSourceCode_ShadowDepthMap, NULL);

	//compile shader
	glCompileShader(gGeometryShaderObject_ShadowDepthMap);

	// Error checking
	checkCompilationLog((char*)"gGeometryShaderObject_ShadowDepthMap", gGeometryShaderObject_ShadowDepthMap);

	//FRAGMENT SHADER

	gFragmentShaderObject_ShadowDepthMap = glCreateShader(GL_FRAGMENT_SHADER);

	const GLchar* fragmentShaderSourceCode_ShadowDepthMap =
		"#version 400" \
		"\n" \
		"in vec4 FragPos;" \

		"uniform vec3 lightPos;" \
		"uniform float far_plane;" \

		"void main(void)" \
		"{" \
		"float lightDistance = length(FragPos.xyz - lightPos);" \

		//map to [0;1] range by dividing by far_plane
		"lightDistance = lightDistance / far_plane;" \

		//write this as modified depth
		"gl_FragDepth  = lightDistance;" \
		"}";

	glShaderSource(gFragmentShaderObject_ShadowDepthMap, 1, (const GLchar**)&fragmentShaderSourceCode_ShadowDepthMap, NULL);

	glCompileShader(gFragmentShaderObject_ShadowDepthMap);

	// Error checking
	checkCompilationLog((char*)"gFragmentShaderObject_ShadowDepthMap", gFragmentShaderObject_ShadowDepthMap);


	//Shader Program

	gShaderProgramObject_ShadowDepthMap = glCreateProgram();

	glAttachShader(gShaderProgramObject_ShadowDepthMap, gVertexShaderObject_ShadowDepthMap);
	glAttachShader(gShaderProgramObject_ShadowDepthMap, gGeometryShaderObject_ShadowDepthMap);
	glAttachShader(gShaderProgramObject_ShadowDepthMap, gFragmentShaderObject_ShadowDepthMap);

	glBindAttribLocation(gShaderProgramObject_ShadowDepthMap, MATRIX_ATTRIBUTE_POSITION, "vPosition");
	
	glLinkProgram(gShaderProgramObject_ShadowDepthMap);

	// Error checking
	checkLinkLog((char*)"gShaderProgramObject_ShadowDepthMap", gShaderProgramObject_ShadowDepthMap);


	gModelMatrixUniform_ShadowDepthMap = glGetUniformLocation(gShaderProgramObject_ShadowDepthMap, "u_model_matrix");
	gLightPosUniform_ShadowDepthMap = glGetUniformLocation(gShaderProgramObject_ShadowDepthMap, "lightPos");
	gFarPlaneUniform_ShadowDepthMap = glGetUniformLocation(gShaderProgramObject_ShadowDepthMap, "far_plane");

	char Name[128];
	for (int i = 0; i < 6; i++)
	{
		memset(Name, 0, sizeof(Name));

		snprintf(Name, sizeof(Name), "shadowMatrices[%d]", i);
		gShadowMatricesUniform_ShadowDepthMap[i] = glGetUniformLocation(gShaderProgramObject_ShadowDepthMap, Name);
	}

}
