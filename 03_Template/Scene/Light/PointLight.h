#pragma once

//header files
#include "../../Include/BasicShapeLoader/Model/BasicOBJLoader/Matrix_Obj_Loader.h"

//function declarations
void initialize_pointLight(void);

void renderAllModelsInMahal(GLuint& ModelMatrixUniform);

void display_pointLight(void);

void update_pointLight(void);

void uninitialize_pointLight(void);

//variable declarations
GLuint gVertexShaderObject_pointLight;
GLuint gFragmentShaderObject_pointLight;
GLuint gShaderProgramObject_pointLight;

GLuint gModelMatrixUniform_pointLight;
GLuint gViewMatrixUniform_pointLight;
GLuint gProjectionMatrixUniform_pointLight;
GLuint gLdUniform_pointLight, gLaUniform_pointLight, gLsUniform_pointLight;
GLuint gLightPositionUniform_pointLight;
GLuint gKdUniform_pointLight, gKaUniform_pointLight, gKsUniform_pointLight;
GLuint gMaterialShininessUniform_pointLight;
GLuint gLKeyPressedUniform_pointLight;
GLuint gTextureSamplerUniform_pointLight, gTextureActiveUniform_pointLight, gAlphaUniform_pointLight;
GLuint gViewPosUniform_pointLight, gNumPointLightsUniform_pointLight;
GLuint applyBloomUniform_pointLight, u_bloom_is_activeUniform_pointLight;
GLuint bloom_thresh_minUniform_pointLight, bloom_thresh_maxUniform_pointLight;
GLuint fadeinFactorUniform_pointLight, fadeoutFactorUniform_pointLight;

#define gNumPointLights_pointLight  20
struct PointLightUniform
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
PointLightUniform m_pointLightsLocation[gNumPointLights_pointLight];


struct PointLight
{
	vec3 u_La;
	vec3 u_Ld;
	vec3 u_Ls;
	float DiffuseIntensity;
	float u_constant;
	float u_linear;
	float u_quadratic;
	vec3 position;
	PointLight()
	{
		position = vec3(0.0, 0.0, 0.0);
		u_constant = 0.0f;
		u_linear = 0.0f;
		u_quadratic = 0.0f;
		u_La = vec3(0.0, 0.0, 0.0);
		u_Ld = vec3(0.0, 0.0, 0.0);
		u_Ls = vec3(0.0, 0.0, 0.0);
	}
};
 
float FadeOutFactor_pointLight = 1.0f;
float FadeInFactor_pointLight = 0.0f;


void initialize_pointLight(void)
{
	void uninitialize(int);

	//VERTEX SHADER
	gVertexShaderObject_pointLight = glCreateShader(GL_VERTEX_SHADER);

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
		"out vec3 fragment_position;" \
		"out  vec2 out_texcord;" \

		"void main(void)" \
		"{" \
			"if(u_LKeyPressed == 1)" \
			"{" \
				"fragment_position = vec3(u_model_matrix * vPosition);" \
				"vec4 eye_coordinates=u_view_matrix * u_model_matrix * vPosition;" \
				"transformed_normals=mat3(u_view_matrix * u_model_matrix) * vNormal;" \
				"light_direction = vec3(u_light_position) - eye_coordinates.xyz;" \
				"viewer_vector = -eye_coordinates.xyz;" \
			"}"\

			"out_texcord = vTexcoord;" \
			"gl_Position = u_projection_matrix * u_view_matrix * u_model_matrix * vPosition;" \
		"}";

	glShaderSource(gVertexShaderObject_pointLight, 1, (const GLchar**)&vertextShaderSourceCode, NULL);

	//compile shader
	glCompileShader(gVertexShaderObject_pointLight);

	// Error checking
	checkCompilationLog((char*)"gVertexShaderObject_pointLight", gVertexShaderObject_pointLight);


	//FRAGMENT SHADER

	gFragmentShaderObject_pointLight = glCreateShader(GL_FRAGMENT_SHADER);

	const GLchar* fragmentShaderSourceCode =
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
		"float diffuseIntensity;" \
		"};" \
		
		"uniform int gNumPointLights;" \
		"uniform PointLight pointLight[20];" \
		"uniform vec3 viewPos;" \
	
		"in vec3 transformed_normals;" \
		"in vec3 light_direction;" \
		"in vec3 viewer_vector;" \
		"in vec3 fragment_position;" \
		"in vec2 out_texcord;" \

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

		"uniform int applyBloom;" \
		"uniform float bloom_thresh_min = 0.8f;" \
		"uniform float bloom_thresh_max = 1.2f;" \
		"uniform int u_bloom_is_active;" \

		"uniform float fadeinFactor;" \
		"uniform float fadeoutFactor;" \

		"vec3 calculatePointLight(int index)" \
		"{" \
		"vec3 pointLightColor;" \
		
		"vec3 Final_Texture_color = texture(u_texture0_sampler, out_texcord).rgb; " \

		"vec3 normalized_transformed_normals=normalize(transformed_normals);" \
		
		"vec3 normalized_light_direction;" \
		"vec3 normalized_viewer_vector;" \
		
		"if(index > 8)" \
		"{" \
		"normalized_light_direction = normalize(pointLight[index].position - fragment_position); " \
		"normalized_viewer_vector=normalize(viewPos - fragment_position);" \
		"}" \
		"else if(index <= 8)" \
		"{" \
		"normalized_light_direction = normalize(pointLight[index].position + fragment_position); " \
		"normalized_viewer_vector=normalize(viewPos + fragment_position);" \
		"}" \
		
		
		"vec3 ambient = vec3(0.0f);" \
		"vec3 diffuse = vec3(0.0f);" \
		"vec3 specular = vec3(0.0f);" \

		"if(u_is_texture == 1)" \
		"{" \
		"ambient = pointLight[index].u_La * u_Ka * Final_Texture_color;" \
		"}" \
		"else" \
		"{" \
		"ambient = pointLight[index].u_La * u_Ka;" \
		"}" \
		"float tn_dot_ld = max(dot(normalized_transformed_normals, normalized_light_direction), 0.0);" \

		"if(u_is_texture == 1)" \
		"{" \
		"diffuse = pointLight[index].u_Ld * u_Kd * tn_dot_ld * Final_Texture_color;" \
		"}" \
		"else" \
		"{" \
		"diffuse = pointLight[index].u_Ld * u_Kd * tn_dot_ld;" \
		"}" \

		"vec3 reflection_vector = reflect(-normalized_light_direction, normalized_transformed_normals);" \
		
		"if(u_is_texture == 1)" \
		"{" \
			"specular = pointLight[index].u_Ls * pow(max(dot(reflection_vector, normalized_viewer_vector), 0.0), u_material_shininess);" \
		"}" \
		"else" \
		"{" \
			"specular = pointLight[index].u_Ls * pow(max(dot(reflection_vector, normalized_viewer_vector), 0.0), u_material_shininess);" \
		"}" \
		
		"float distance = length(pointLight[index].position - fragment_position);" \
		"float attenuation = 1.0 / (pointLight[index].u_constant + pointLight[index].u_linear * distance + "\
		"pointLight[index].u_quadratic * (distance * distance));" \
		
		"diffuse = diffuse * attenuation;" \
		"specular = specular * attenuation;" \
		"pointLightColor = ambient + diffuse + specular;" \
		"return pointLightColor;" \
		
		"}" \
		
		"void main(void)" \
		"{" \
			"vec3 phong_ads_color;" \
			"vec4 Final_Texture;" \
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
			"GodRaysColor = vec4(0.0);" \
		"}";

	glShaderSource(gFragmentShaderObject_pointLight, 1, (const GLchar**)&fragmentShaderSourceCode, NULL);

	glCompileShader(gFragmentShaderObject_pointLight);

	// Error checking
	checkCompilationLog((char*)"gFragmentShaderObject_pointLight", gFragmentShaderObject_pointLight);


	//Shader Program

	gShaderProgramObject_pointLight = glCreateProgram();

	glAttachShader(gShaderProgramObject_pointLight, gVertexShaderObject_pointLight);
	glAttachShader(gShaderProgramObject_pointLight, gFragmentShaderObject_pointLight);

	glBindAttribLocation(gShaderProgramObject_pointLight, MATRIX_ATTRIBUTE_POSITION, "vPosition");
	glBindAttribLocation(gShaderProgramObject_pointLight, MATRIX_ATTRIBUTE_NORMAL, "vNormal");
	glBindAttribLocation(gShaderProgramObject_pointLight, MATRIX_ATTRIBUTE_TEXTURE0, "vTexcoord");

	glLinkProgram(gShaderProgramObject_pointLight);

	// Error checking
	checkLinkLog((char*)"gShaderProgramObject_pointLight", gShaderProgramObject_pointLight);


	gModelMatrixUniform_pointLight = glGetUniformLocation(gShaderProgramObject_pointLight, "u_model_matrix");
	gViewMatrixUniform_pointLight = glGetUniformLocation(gShaderProgramObject_pointLight, "u_view_matrix");
	gProjectionMatrixUniform_pointLight = glGetUniformLocation(gShaderProgramObject_pointLight, "u_projection_matrix");

	gLKeyPressedUniform_pointLight = glGetUniformLocation(gShaderProgramObject_pointLight, "u_LKeyPressed");

	gLdUniform_pointLight = glGetUniformLocation(gShaderProgramObject_pointLight, "u_Ld");
	gLaUniform_pointLight = glGetUniformLocation(gShaderProgramObject_pointLight, "u_La");
	gLsUniform_pointLight = glGetUniformLocation(gShaderProgramObject_pointLight, "u_Ls");

	gKdUniform_pointLight = glGetUniformLocation(gShaderProgramObject_pointLight, "u_Kd");
	gKsUniform_pointLight = glGetUniformLocation(gShaderProgramObject_pointLight, "u_Ks");
	gKaUniform_pointLight = glGetUniformLocation(gShaderProgramObject_pointLight, "u_Ka");

	gLightPositionUniform_pointLight		= glGetUniformLocation(gShaderProgramObject_pointLight, "u_light_position");
	gMaterialShininessUniform_pointLight	= glGetUniformLocation(gShaderProgramObject_pointLight, "u_material_shininess");

	gTextureSamplerUniform_pointLight = glGetUniformLocation(gShaderProgramObject_pointLight, "u_texture0_sampler");

	gTextureActiveUniform_pointLight	= glGetUniformLocation(gShaderProgramObject_pointLight, "u_is_texture");
	gAlphaUniform_pointLight			= glGetUniformLocation(gShaderProgramObject_pointLight, "u_alpha");
	
	gViewPosUniform_pointLight = glGetUniformLocation(gShaderProgramObject_pointLight, "viewPos");
	gNumPointLightsUniform_pointLight = glGetUniformLocation(gShaderProgramObject_pointLight, "gNumPointLights");
	
	char Name[128];
	for(int i = 0; i < gNumPointLights_pointLight; i++)
	{
		memset(Name, 0, sizeof(Name));
		
		snprintf(Name, sizeof(Name), "pointLight[%d].u_La", i);
		m_pointLightsLocation[i].u_La = glGetUniformLocation(gShaderProgramObject_pointLight, Name);

		snprintf(Name, sizeof(Name), "pointLight[%d].u_Ld", i);
		m_pointLightsLocation[i].u_Ld = glGetUniformLocation(gShaderProgramObject_pointLight, Name);

		snprintf(Name, sizeof(Name), "pointLight[%d].u_Ls", i);
		m_pointLightsLocation[i].u_Ls = glGetUniformLocation(gShaderProgramObject_pointLight, Name);

		snprintf(Name, sizeof(Name), "pointLight[%d].u_constant", i);
		m_pointLightsLocation[i].u_constant = glGetUniformLocation(gShaderProgramObject_pointLight, Name);

		snprintf(Name, sizeof(Name), "pointLight[%d].u_linear", i);
		m_pointLightsLocation[i].u_linear = glGetUniformLocation(gShaderProgramObject_pointLight, Name);

		snprintf(Name, sizeof(Name), "pointLight[%d].u_quadratic", i);
		m_pointLightsLocation[i].u_quadratic = glGetUniformLocation(gShaderProgramObject_pointLight, Name);

		snprintf(Name, sizeof(Name), "pointLight[%d].position", i);
		m_pointLightsLocation[i].position = glGetUniformLocation(gShaderProgramObject_pointLight, Name);

		snprintf(Name, sizeof(Name), "pointLight[%d].diffuseIntensity", i);
		m_pointLightsLocation[i].DiffuseIntensity = glGetUniformLocation(gShaderProgramObject_pointLight, Name);
	}
	
	applyBloomUniform_pointLight = glGetUniformLocation(gShaderProgramObject_pointLight, "applyBloom");
	u_bloom_is_activeUniform_pointLight = glGetUniformLocation(gShaderProgramObject_pointLight, "u_bloom_is_active");
	bloom_thresh_minUniform_pointLight = glGetUniformLocation(gShaderProgramObject_pointLight, "bloom_thresh_min");
	bloom_thresh_maxUniform_pointLight = glGetUniformLocation(gShaderProgramObject_pointLight, "bloom_thresh_max");

	fadeinFactorUniform_pointLight = glGetUniformLocation(gShaderProgramObject_pointLight, "fadeinFactor");
	fadeoutFactorUniform_pointLight = glGetUniformLocation(gShaderProgramObject_pointLight, "fadeoutFactor");
}



vec3 positionLamp[] = { vec3(-720.0f, 600.0f, 440.0f), vec3(-720.0f, 600.0f, 750.0f), vec3(-250.0f, 400.0f, 750.0f), vec3(320.0f, 400.0f, 750.0f), vec3(850.0f, 400.0f, 750.0f),vec3(1420.0f, 400.0f, 750.0f) , vec3(1950.0f, 400.0f, 750.0f) , vec3(2520.0f, 400.0f, 750.0f) ,
						vec3(3105.0f, 400.0f, 750.0f) , vec3(3650.0f,400.0f, 750.0f) ,  
						vec3(-750.0f, 600.0f, -510.0f), vec3(-750.0f, 600.0f, -810.0f), vec3(-250.0f, 400.0f, -810.0f), vec3(320.0f, 400.0f, -810.0f), vec3(850.0f, 400.0f, -810.0f),vec3(1420.0f, 400.0f, -810.0f) , vec3(2000.0f, 400.0f, -810.0f) , vec3(2570.0f, 400.0f, -810.0f) ,
						vec3(3100.0f, 400.0f, -810.0f) , vec3(3700.0f, 400.0f, -810.0f) , };

float TranslateMeasure_pointLight[] = { 200.0f, 700.0f, 1200.0f, 1800.0f, 2300.0f, 2900.0f, 3500.0f };


void display_pointLight(void)
{
	float PI = 3.14;
	static float gAngle = 0.0f;
	if (gAngle > 360.0f)
		gAngle = 0.0f;
	gAngle += 0.001f;
	PointLight pointLight[gNumPointLights_pointLight];

	for (int i = 0; i < gNumPointLights_pointLight; i++)
	{
		pointLight[i].u_La = vec3(0.01f, 0.01f, 0.01f);
		pointLight[i].u_Ld = vec3(1.0f, 1.0f, 1.0f);
		pointLight[i].u_Ls = vec3(0.1f, 0.1f, 0.1f);
		pointLight[i].u_linear = 0.01;
		pointLight[i].u_constant = 0.01;
		pointLight[i].u_quadratic = 0.0;
		pointLight[i].DiffuseIntensity = 1.0f;

		pointLight[i].position = positionLamp[i];
	}


	glUseProgram(gShaderProgramObject_pointLight);

	if (FadeInFactor_pointLight <= 1.0f)
		FadeInFactor_pointLight += 0.001f;


	if (bStartFadeOutSecondScene == true)
	{
		if (FadeOutFactor_pointLight >= 0.0f)
			FadeOutFactor_pointLight -= 0.001f;
		else
		{
			bFadeOutSecondSceneDone = true;
			iShowEndScene = true;
		}
	}
	glUniform1f(fadeoutFactorUniform_pointLight, FadeOutFactor_pointLight);
	glUniform1f(fadeinFactorUniform_pointLight, FadeInFactor_pointLight);

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
		glUniform1i(gNumPointLightsUniform_pointLight, gNumPointLights_pointLight);
		for (int i = 0; i < gNumPointLights_pointLight; i++)
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
	}
	else
	{
		glUniform1i(gLKeyPressedUniform_pointLight, 0);
	}

	gViewMatrix = lookat(vmath_camera_eye_coord, vmath_camera_center_coord, vmath_camera_up_coord);

	glUniformMatrix4fv(gViewMatrixUniform_pointLight, 1, GL_FALSE, gViewMatrix);

	glUniform3fv(gViewPosUniform_pointLight, 1, vmath_camera_eye_coord);

	glUniformMatrix4fv(gProjectionMatrixUniform_pointLight, 1, GL_FALSE, gPerspectiveProjectionMatrix);


	renderAllModelsInMahal(gModelMatrixUniform_pointLight);

	glUseProgram(0);
}

void renderAllModelsInMahal(GLuint& ModelMatrixUniform)
{

	mat4 modelMatrix = mat4::identity();
	mat4 scaleMatrix = mat4::identity();
	mat4 rotateMatrix = mat4::identity();

	//Object Drawing
	
	//1. Draw Mahal Model
	modelMatrix = mat4::identity();
	modelMatrix = vmath::translate(0.0f, -2.0f, -6.0f);


	glUniformMatrix4fv(ModelMatrixUniform, 1, GL_FALSE, modelMatrix);
	glBindVertexArray(gModel_Mahal.Vao);

	for (int i = 0; i < gModel_Mahal.model_mesh_data.size(); i++)
	{
		if (gbLight == true)
		{
			glUniform3fv(gKaUniform_pointLight, 1, gModel_Mahal.model_material[gModel_Mahal.model_mesh_data[i].material_index].Ka);
			glUniform3fv(gKdUniform_pointLight, 1, gModel_Mahal.model_material[gModel_Mahal.model_mesh_data[i].material_index].Kd);
			glUniform3fv(gKsUniform_pointLight, 1, gModel_Mahal.model_material[gModel_Mahal.model_mesh_data[i].material_index].Ks);
			glUniform1f(gMaterialShininessUniform_pointLight, material_shininess);
			glUniform1f(gAlphaUniform_pointLight, gModel_Mahal.model_material[gModel_Mahal.model_mesh_data[i].material_index].d);

			if (gModel_Mahal.model_material[gModel_Mahal.model_mesh_data[i].material_index].ismap_Kd == true)
			{
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, gModel_Mahal.model_material[gModel_Mahal.model_mesh_data[i].material_index].gTexture_diffuse);
				glUniform1i(gTextureSamplerUniform_pointLight, 0);
				glUniform1i(gTextureActiveUniform_pointLight, 1);
			}
			else
				glUniform1i(gTextureActiveUniform_pointLight, 0);
		}
		glDrawArrays(GL_TRIANGLES, gModel_Mahal.model_mesh_data[i].vertex_Index, gModel_Mahal.model_mesh_data[i].vertex_Count);
	}
	glBindVertexArray(0);

	//2. mashal Model
	glUniform1i(applyBloomUniform_pointLight, 0);
	for (int i = 0; i < gNumPointLights_pointLight; i++)
	{
		modelMatrix = mat4::identity();
		scaleMatrix = mat4::identity();
		rotateMatrix = mat4::identity();

		scaleMatrix = scale(2.0f, 2.0f, 2.0f);
		if (i < 10)
		{
			modelMatrix = vmath::translate(positionLamp[i][0], positionLamp[i][1] - 65.0f, positionLamp[i][2] + 20.0f);
			rotateMatrix = rotate(-20.0f, 1.0f, 0.0f, 0.0f);
		}
		else
		{
			modelMatrix = vmath::translate(positionLamp[i][0], positionLamp[i][1] - 65.0f, positionLamp[i][2] - 20.0f);
			rotateMatrix = rotate(20.0f, 1.0f, 0.0f, 0.0f);
		}
		modelMatrix = modelMatrix * rotateMatrix * scaleMatrix;

		glUniformMatrix4fv(ModelMatrixUniform, 1, GL_FALSE, modelMatrix);
		glBindVertexArray(gModel_Mashal.Vao);
		for (int i = 0; i < gModel_Mashal.model_mesh_data.size(); i++)
		{
			if (gbLight == true)
			{
				glUniform3fv(gKaUniform_pointLight, 1, gModel_Mashal.model_material[gModel_Mashal.model_mesh_data[i].material_index].Ka);
				glUniform3fv(gKdUniform_pointLight, 1, gModel_Mashal.model_material[gModel_Mashal.model_mesh_data[i].material_index].Kd);
				glUniform3fv(gKsUniform_pointLight, 1, gModel_Mashal.model_material[gModel_Mashal.model_mesh_data[i].material_index].Ks);
				glUniform1f(gMaterialShininessUniform_pointLight, material_shininess);
				glUniform1f(gAlphaUniform_pointLight, gModel_Mashal.model_material[gModel_Mashal.model_mesh_data[i].material_index].d);
				if (gModel_Mashal.model_material[gModel_Mashal.model_mesh_data[i].material_index].ismap_Kd == true)
				{
					glActiveTexture(GL_TEXTURE0);
					glBindTexture(GL_TEXTURE_2D, gModel_Mashal.model_material[gModel_Mashal.model_mesh_data[i].material_index].gTexture_diffuse);
					glUniform1i(gTextureSamplerUniform_pointLight, 0);
					glUniform1i(gTextureActiveUniform_pointLight, 1);

				}
				else
					glUniform1i(gTextureActiveUniform_pointLight, 0);
			}
			glDrawArrays(GL_TRIANGLES, gModel_Mashal.model_mesh_data[i].vertex_Index, gModel_Mashal.model_mesh_data[i].vertex_Count);
		}
		glBindVertexArray(0);
	}

	glUniform1i(applyBloomUniform_pointLight, 1);
	for (int i = 0; i < gNumPointLights_pointLight; i++)
	{
		modelMatrix = mat4::identity();
		scaleMatrix = mat4::identity();
		rotateMatrix = mat4::identity();

		modelMatrix = vmath::translate(positionLamp[i]);
		scaleMatrix = scale(5.0f, 5.0f, 5.0f);
		modelMatrix = modelMatrix * rotateMatrix * scaleMatrix;

		glUniformMatrix4fv(ModelMatrixUniform, 1, GL_FALSE, modelMatrix);

		drawCubeShape();
	}

	//3. Other raje
	glUniform1i(applyBloomUniform_pointLight, 0);

	if (isRajeInSabha == true)
	{
		//other raje
		for (int i = 0; i < 14; i++)
		{
			modelMatrix = mat4::identity();
			scaleMatrix = mat4::identity();
			rotateMatrix = mat4::identity();
			if (i < 7)
			{
				modelMatrix = vmath::translate(TranslateMeasure_pointLight[i], -10.0f, 650.0f);
				if (isFirstScene == false)
				{
					if (i == 4)
						goto NEXT;
				}
			}
			else {
				modelMatrix = vmath::translate(TranslateMeasure_pointLight[i - 7], -10.0f, -650.0f);
			}
			scaleMatrix = scale(170.0f, 170.0f, 170.0f);
			if (i < 7)
			{
				rotateMatrix = rotate(180.0f, 0.0f, 1.0f, 0.0f);
			}

			modelMatrix = modelMatrix * rotateMatrix * scaleMatrix;

			glUniformMatrix4fv(ModelMatrixUniform, 1, GL_FALSE, modelMatrix);

			if (i % 2 == 0)
			{
				glBindVertexArray(gModel_Krishna_Seated.Vao);
				for (int i = 0; i < gModel_Krishna_Seated.model_mesh_data.size(); i++)
				{
					if (gbLight == true)
					{
						glUniform3fv(gKaUniform_pointLight, 1, gModel_Krishna_Seated.model_material[gModel_Krishna_Seated.model_mesh_data[i].material_index].Ka);
						glUniform3fv(gKdUniform_pointLight, 1, gModel_Krishna_Seated.model_material[gModel_Krishna_Seated.model_mesh_data[i].material_index].Kd);
						glUniform3fv(gKsUniform_pointLight, 1, gModel_Krishna_Seated.model_material[gModel_Krishna_Seated.model_mesh_data[i].material_index].Ks);
						glUniform1f(gMaterialShininessUniform_pointLight, material_shininess);
						glUniform1f(gAlphaUniform_pointLight, gModel_Krishna_Seated.model_material[gModel_Krishna_Seated.model_mesh_data[i].material_index].d);
						if (gModel_Krishna_Seated.model_material[gModel_Krishna_Seated.model_mesh_data[i].material_index].ismap_Kd == true)
						{
							glActiveTexture(GL_TEXTURE0);
							glBindTexture(GL_TEXTURE_2D, gModel_Krishna_Seated.model_material[gModel_Krishna_Seated.model_mesh_data[i].material_index].gTexture_diffuse);
							glUniform1i(gTextureSamplerUniform_pointLight, 0);
							glUniform1i(gTextureActiveUniform_pointLight, 1);

						}
						else
							glUniform1i(gTextureActiveUniform_pointLight, 0);
					}
					glDrawArrays(GL_TRIANGLES, gModel_Krishna_Seated.model_mesh_data[i].vertex_Index, gModel_Krishna_Seated.model_mesh_data[i].vertex_Count);
				}

			}
			else
			{
				glBindVertexArray(gModel_Krishna_Seated2.Vao);
				for (int i = 0; i < gModel_Krishna_Seated2.model_mesh_data.size(); i++)
				{
					if (gbLight == true)
					{
						glUniform3fv(gKaUniform_pointLight, 1, gModel_Krishna_Seated2.model_material[gModel_Krishna_Seated2.model_mesh_data[i].material_index].Ka);
						glUniform3fv(gKdUniform_pointLight, 1, gModel_Krishna_Seated2.model_material[gModel_Krishna_Seated2.model_mesh_data[i].material_index].Kd);
						glUniform3fv(gKsUniform_pointLight, 1, gModel_Krishna_Seated2.model_material[gModel_Krishna_Seated2.model_mesh_data[i].material_index].Ks);
						glUniform1f(gMaterialShininessUniform_pointLight, material_shininess);
						glUniform1f(gAlphaUniform_pointLight, gModel_Krishna_Seated2.model_material[gModel_Krishna_Seated2.model_mesh_data[i].material_index].d);
						if (gModel_Krishna_Seated2.model_material[gModel_Krishna_Seated2.model_mesh_data[i].material_index].ismap_Kd == true)
						{
							glActiveTexture(GL_TEXTURE0);
							glBindTexture(GL_TEXTURE_2D, gModel_Krishna_Seated2.model_material[gModel_Krishna_Seated2.model_mesh_data[i].material_index].gTexture_diffuse);
							glUniform1i(gTextureSamplerUniform_pointLight, 0);
							glUniform1i(gTextureActiveUniform_pointLight, 1);

						}
						else
							glUniform1i(gTextureActiveUniform_pointLight, 0);
					}
					glDrawArrays(GL_TRIANGLES, gModel_Krishna_Seated2.model_mesh_data[i].vertex_Index, gModel_Krishna_Seated2.model_mesh_data[i].vertex_Count);
				}


			}
		NEXT:
			glBindVertexArray(0);

		}

		//sadhu

		modelMatrix = mat4::identity();
		scaleMatrix = mat4::identity();
		rotateMatrix = mat4::identity();

		modelMatrix = vmath::translate(1455.0f, -45.0f, -30.0f);
		scaleMatrix = scale(300.0f, 300.0f, 300.0f);
		rotateMatrix = rotate(-90.0f, 0.0f, 1.0f, 0.0f);

		modelMatrix = modelMatrix * rotateMatrix * scaleMatrix;

		glUniformMatrix4fv(ModelMatrixUniform, 1, GL_FALSE, modelMatrix);
		glBindVertexArray(gModel_Krishna_Seated.Vao);
		for (int i = 0; i < gModel_Krishna_Seated.model_mesh_data.size(); i++)
		{
			if (gbLight == true)
			{
				glUniform3fv(gKaUniform_pointLight, 1, gModel_Krishna_Seated.model_material[gModel_Krishna_Seated.model_mesh_data[i].material_index].Ka);
				glUniform3fv(gKdUniform_pointLight, 1, gModel_Krishna_Seated.model_material[gModel_Krishna_Seated.model_mesh_data[i].material_index].Kd);
				glUniform3fv(gKsUniform_pointLight, 1, gModel_Krishna_Seated.model_material[gModel_Krishna_Seated.model_mesh_data[i].material_index].Ks);
				glUniform1f(gMaterialShininessUniform_pointLight, material_shininess);
				glUniform1f(gAlphaUniform_pointLight, gModel_Krishna_Seated.model_material[gModel_Krishna_Seated.model_mesh_data[i].material_index].d);
				if (gModel_Krishna_Seated.model_material[gModel_Krishna_Seated.model_mesh_data[i].material_index].ismap_Kd == true)
				{
					glActiveTexture(GL_TEXTURE0);
					glBindTexture(GL_TEXTURE_2D, gModel_Krishna_Seated.model_material[gModel_Krishna_Seated.model_mesh_data[i].material_index].gTexture_diffuse);
					glUniform1i(gTextureSamplerUniform_pointLight, 0);
					glUniform1i(gTextureActiveUniform_pointLight, 1);

				}
				else
					glUniform1i(gTextureActiveUniform_pointLight, 0);
			}
			glDrawArrays(GL_TRIANGLES, gModel_Krishna_Seated.model_mesh_data[i].vertex_Index, gModel_Krishna_Seated.model_mesh_data[i].vertex_Count);
		}
		glBindVertexArray(0);
	}

	//other chair
	glUniform1i(applyBloomUniform_pointLight, 0);
	for (int i = 0; i < 14; i++)
	{

		modelMatrix = mat4::identity();
		scaleMatrix = mat4::identity();
		rotateMatrix = mat4::identity();
		if (i < 7)
		{
			modelMatrix = vmath::translate(TranslateMeasure_pointLight[i], 40.0f, 700.0f);
		}
		else {
			modelMatrix = vmath::translate(TranslateMeasure_pointLight[i - 7], 40.0f, -700.0f);
		}
		scaleMatrix = scale(30.0f, 30.0f, 30.0f);
		if (i < 7)
		{
			rotateMatrix = rotate(180.0f, 0.0f, 1.0f, 0.0f);
		}

		modelMatrix = modelMatrix * rotateMatrix * scaleMatrix;

		glUniformMatrix4fv(ModelMatrixUniform, 1, GL_FALSE, modelMatrix);
		glBindVertexArray(gModel_OtherChair.Vao);
		for (int i = 0; i < gModel_OtherChair.model_mesh_data.size(); i++)
		{
			if (gbLight == true)
			{
				glUniform3fv(gKaUniform_pointLight, 1, gModel_OtherChair.model_material[gModel_OtherChair.model_mesh_data[i].material_index].Ka);
				glUniform3fv(gKdUniform_pointLight, 1, gModel_OtherChair.model_material[gModel_OtherChair.model_mesh_data[i].material_index].Kd);
				glUniform3fv(gKsUniform_pointLight, 1, gModel_OtherChair.model_material[gModel_OtherChair.model_mesh_data[i].material_index].Ks);
				glUniform1f(gMaterialShininessUniform_pointLight, material_shininess);
				glUniform1f(gAlphaUniform_pointLight, gModel_OtherChair.model_material[gModel_OtherChair.model_mesh_data[i].material_index].d);
				if (gModel_OtherChair.model_material[gModel_OtherChair.model_mesh_data[i].material_index].ismap_Kd == true)
				{
					glActiveTexture(GL_TEXTURE0);
					glBindTexture(GL_TEXTURE_2D, gModel_OtherChair.model_material[gModel_OtherChair.model_mesh_data[i].material_index].gTexture_diffuse);
					glUniform1i(gTextureSamplerUniform_pointLight, 0);
					glUniform1i(gTextureActiveUniform_pointLight, 1);

				}
				else
					glUniform1i(gTextureActiveUniform_pointLight, 0);
			}
			glDrawArrays(GL_TRIANGLES, gModel_OtherChair.model_mesh_data[i].vertex_Index, gModel_OtherChair.model_mesh_data[i].vertex_Count);
		}

		glBindVertexArray(0);


	}


	//krishna chair
	modelMatrix = mat4::identity();
	scaleMatrix = mat4::identity();
	rotateMatrix = mat4::identity();
	modelMatrix = vmath::translate(-380.0f, 135.0f, 1.0f);
	scaleMatrix = scale(45.0f, 45.0f, 45.0f);
	rotateMatrix = rotate(90.0f, 0.0f, 1.0f, 0.0f);

	modelMatrix = modelMatrix * rotateMatrix * scaleMatrix;

	glUniformMatrix4fv(ModelMatrixUniform, 1, GL_FALSE, modelMatrix);
	glBindVertexArray(gModel_KrishnaChair.Vao);
	for (int i = 0; i < gModel_KrishnaChair.model_mesh_data.size(); i++)
	{
		if (gbLight == true)
		{
			glUniform3fv(gKaUniform_pointLight, 1, gModel_KrishnaChair.model_material[gModel_KrishnaChair.model_mesh_data[i].material_index].Ka);
			glUniform3fv(gKdUniform_pointLight, 1, gModel_KrishnaChair.model_material[gModel_KrishnaChair.model_mesh_data[i].material_index].Kd);
			glUniform3fv(gKsUniform_pointLight, 1, gModel_KrishnaChair.model_material[gModel_KrishnaChair.model_mesh_data[i].material_index].Ks);
			glUniform1f(gMaterialShininessUniform_pointLight, material_shininess);
			glUniform1f(gAlphaUniform_pointLight, gModel_KrishnaChair.model_material[gModel_KrishnaChair.model_mesh_data[i].material_index].d);
			if (gModel_KrishnaChair.model_material[gModel_KrishnaChair.model_mesh_data[i].material_index].ismap_Kd == true)
			{
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, gModel_KrishnaChair.model_material[gModel_KrishnaChair.model_mesh_data[i].material_index].gTexture_diffuse);
				glUniform1i(gTextureSamplerUniform_pointLight, 0);
				glUniform1i(gTextureActiveUniform_pointLight, 1);
				
			}
			else
				glUniform1i(gTextureActiveUniform_pointLight, 0);
		}
		glDrawArrays(GL_TRIANGLES, gModel_KrishnaChair.model_mesh_data[i].vertex_Index, gModel_KrishnaChair.model_mesh_data[i].vertex_Count);
	}
	glBindVertexArray(0);

	if (isFirstScene == false)
	{

		// shishupal model
		glUniform1i(applyBloomUniform_pointLight, 0);
		modelMatrix = mat4::identity();
		scaleMatrix = mat4::identity();
		rotateMatrix = mat4::identity();

		modelMatrix = vmath::translate(2300.0f, -15.0f, 600.0f);
		scaleMatrix = scale(200.0f, 200.0f, 200.0f);
		rotateMatrix = rotate(230.0f, 0.0f, 1.0f, 0.0f);

		modelMatrix = modelMatrix * rotateMatrix * scaleMatrix;

		glUniformMatrix4fv(ModelMatrixUniform, 1, GL_FALSE, modelMatrix);
		glBindVertexArray(gModel_Krishna.Vao);
		for (int i = 0; i < gModel_Krishna.model_mesh_data.size(); i++)
		{
			if (gbLight == true)
			{
				glUniform3fv(gKaUniform_pointLight, 1, gModel_Krishna.model_material[gModel_Krishna.model_mesh_data[i].material_index].Ka);
				glUniform3fv(gKdUniform_pointLight, 1, gModel_Krishna.model_material[gModel_Krishna.model_mesh_data[i].material_index].Kd);
				glUniform3fv(gKsUniform_pointLight, 1, gModel_Krishna.model_material[gModel_Krishna.model_mesh_data[i].material_index].Ks);
				glUniform1f(gMaterialShininessUniform_pointLight, material_shininess);
				glUniform1f(gAlphaUniform_pointLight, gModel_Krishna.model_material[gModel_Krishna.model_mesh_data[i].material_index].d);
				if (gModel_Krishna.model_material[gModel_Krishna.model_mesh_data[i].material_index].ismap_Kd == true)
				{
					glActiveTexture(GL_TEXTURE0);
					glBindTexture(GL_TEXTURE_2D, gModel_Krishna.model_material[gModel_Krishna.model_mesh_data[i].material_index].gTexture_diffuse);
					glUniform1i(gTextureSamplerUniform_pointLight, 0);
					glUniform1i(gTextureActiveUniform_pointLight, 1);
					
				}
				else
					glUniform1i(gTextureActiveUniform_pointLight, 0);
			}
			glDrawArrays(GL_TRIANGLES, gModel_Krishna.model_mesh_data[i].vertex_Index, gModel_Krishna.model_mesh_data[i].vertex_Count);
		}
		glBindVertexArray(0);

	}
}

void update_pointLight(void)
{
	if (gbStartCamera)
	{
		if (vmath_camera_eye_coord[0] < 207.0f && vmath_camera_eye_coord[1] < 395.0f && vmath_camera_center_coord[1] < 514.0f && gbZoomOutForFullView == false)
		{
			vmath_camera_eye_coord[0]		= vmath_camera_eye_coord[0]		+ ((207.0f	- 144.0f)	/ 2000.0f);				// 144.0f	: inital, 207.0f : final, 2000 number of steps 
			vmath_camera_eye_coord[1]		= vmath_camera_eye_coord[1]		+ ((395.0f	- 39.0f)	/ 2000.0f);				// 395.0f	: final, 39.0f : initial
			vmath_camera_center_coord[1]	= vmath_camera_center_coord[1]	+ ((514.0f - 91.0f)	/ 2000.0f);					// 91.0f	: final,  514.0f: initial
		}
		else
		{
			gbZoomOutForFullView = true;
		}

		if (gbZoomOutForFullView == true)
		{
			if (vmath_camera_eye_coord[0] < 676.0f)
			{
				vmath_camera_eye_coord[0] = vmath_camera_eye_coord[0] + ((676.0f - 207.0f) / 1000.0f);
			}
			if (vmath_camera_eye_coord[1] > 80.0f)
			{
				vmath_camera_eye_coord[1] = vmath_camera_eye_coord[1] - ((395.0f - 80.0f) / 1000.0f);						// initial position is 395, final position is 80
			}

			if (vmath_camera_center_coord[1] > 351.0f)
			{
				vmath_camera_center_coord[1] = vmath_camera_center_coord[1] - ((514.0f - 351.0f) / 1000.0f);				// initial position is 514.0f, final position is 351.0
			}
		}
	}
}

void uninitialize_pointLight(void)
{
	programObjectSafeRelease(gShaderProgramObject_pointLight);
}