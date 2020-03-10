#pragma once

#include "../../Include/BasicShapeLoader/Model/Matrix_Obj_Loader.h"

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

#define gNumPointLights_pointLight  3
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


GLfloat lightAmbient[] = { 0.0f,0.0f,0.0f,1.0f };
GLfloat lightDiffuse[] = { 1.0f,1.0f,1.0f,1.0f };
GLfloat lightSpecular[] = { 1.0f,1.0f,1.0f,1.0f };
GLfloat lightPosition[] = { 100.0f,100.0f,100.0f,1.0f };

GLfloat material_ambient[] = { 0.0f,0.0f,0.0f,1.0f };
GLfloat material_diffuse[] = { 1.0f,1.0f,1.0f,1.0f };
GLfloat material_specular[] = { 1.0f,1.0f,1.0f,1.0f };
GLfloat material_shininess = 50.0f;

// FOR CAMERA
/*
vec3 vmath_camera_eye_coord		= { -40.0f, 180.0f,1.0f };
vec3 vmath_camera_center_coord	= { -1000.0f,200.00f,0.0f };
vec3 vmath_camera_up_coord		= { 0.0f,1.0f,0.0f };
*/

vec3 vmath_camera_eye_coord		= { 3190.0f, 80.0f, 610.0f };
vec3 vmath_camera_center_coord	= { 0.0f,-235.0f,0.0f };
vec3 vmath_camera_up_coord		= { 0.0f,1.0f,0.0f };

bool gbStartCamera			= false;
bool gbZoomOutForFullView	= false;
 

void initPointLightShader()
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
		"uniform PointLight pointLight[3];" \
		"uniform vec3 viewPos;" \
	
		"in vec3 transformed_normals;" \
		"in vec3 light_direction;" \
		"in vec3 viewer_vector;" \
		"in vec3 fragment_position;" \
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

		"vec3 calculatePointLight(int index)" \
		"{" \
		"vec3 pointLightColor;" \
		
		"vec3 normalized_transformed_normals=normalize(transformed_normals);" \
		"vec3 normalized_light_direction=normalize(pointLight[index].position);" \
		"vec3 normalized_viewer_vector=normalize(viewPos - fragment_position);" \
		
		"vec3 ambient = pointLight[index].u_La * u_Ka;" \
		"float tn_dot_ld = max(dot(normalized_transformed_normals, normalized_light_direction), 0.0);" \
		"vec3 diffuse = pointLight[index].u_Ld * u_Kd * tn_dot_ld;" \
		"vec3 reflection_vector = reflect(-normalized_light_direction, normalized_transformed_normals);" \
		"vec3 specular = pointLight[index].u_Ls * pow(max(dot(reflection_vector, normalized_viewer_vector), 0.0), u_material_shininess);" \
		
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
				"vec3 normalized_transformed_normals=normalize(transformed_normals);" \
				"vec3 normalized_light_direction=normalize(light_direction);" \
				"vec3 normalized_viewer_vector=normalize(viewer_vector);" \
				"vec3 ambient = u_La * u_Ka;" \
				"float tn_dot_ld = max(dot(normalized_transformed_normals, normalized_light_direction),0.0);" \
				"vec3 diffuse = u_Ld * u_Kd * tn_dot_ld;" \
				"vec3 reflection_vector = reflect(-normalized_light_direction, normalized_transformed_normals);" \
				"vec3 specular = u_Ls * u_Ks * pow(max(dot(reflection_vector, normalized_viewer_vector), 0.0), u_material_shininess);" \
				"phong_ads_color=ambient + diffuse + specular;" \
				
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
	
	
}


void initialize_pointLight()
{
	initPointLightShader();

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

void display_pointLight()
{
	float PI = 3.14;
	static float gAngle = 0.0f;
	if(gAngle > 360.0f)
		gAngle = 0.0f;
	gAngle += 0.001f;
	PointLight pointLight[gNumPointLights_pointLight];
	pointLight[0].u_La = vec3(0.00f, 0.00f, 0.00f);
	pointLight[0].u_Ld = vec3(0.0f, 0.0f, 1.0f);
	pointLight[0].u_Ls = vec3(0.0f, 0.0f, 1.0f);
	pointLight[0].u_linear = 0.001;
	pointLight[0].u_constant = 0.01;
	pointLight[0].u_quadratic = 0.0;
	pointLight[0].DiffuseIntensity = 0.5f;
	
	//pointLight[0].position = vec3(0.0, 5.0f * cos(2 * PI * gAngle), 5.0f * sin(2 * PI * gAngle));
	
		pointLight[0].position = vec3(-300.0f, 0.0f, 0.0f);
	

	pointLight[1].u_La = vec3(0.00f, 0.0f, 0.0f);
	pointLight[1].u_Ld = vec3(0.0f, 1.0f, 0.0f);
	pointLight[1].u_Ls = vec3(0.0f, 1.0f, 0.0f);
	pointLight[1].u_linear = 0.001;
	pointLight[1].u_constant = 0.01;
	pointLight[1].u_quadratic = 0.0;
	pointLight[1].DiffuseIntensity = 0.5f;
	
	//pointLight[1].position = vec3(5.0f * cos(2 * PI * gAngle), 5.0f * sin(2 * PI * gAngle), 0.0);
	
	pointLight[1].position = vec3(-150.0f, 0.0f, 0.0f);


	pointLight[2].u_La = vec3(0.0f, 0.0f, 0.0f);
	pointLight[2].u_Ld = vec3(1.0f, 0.0f, 0.0f);
	pointLight[2].u_Ls = vec3(1.0f, 0.0f, 0.0f);
	pointLight[2].u_linear = 0.001;
	pointLight[2].u_constant = 0.01;
	pointLight[2].u_quadratic = 0.0;
	pointLight[2].DiffuseIntensity = 0.5f;
	
	//pointLight[2].position = vec3(5.0f * cos(2 * PI * gAngle), 0.0f, 5.0f * sin(2 * PI * gAngle));
	
	pointLight[2].position = vec3(-0.0f, 0.0f, 0.0);
	
	
	glUseProgram(gShaderProgramObject_pointLight);

	if (gbLight == true)
	{
		glUniform1i(gLKeyPressedUniform_pointLight, 1);

		glUniform3fv(gLdUniform_pointLight, 1, lightDiffuse);//white
		glUniform3fv(gLaUniform_pointLight, 1, lightAmbient);
		glUniform3fv(gLsUniform_pointLight, 1, lightSpecular);
		glUniform4fv(gLightPositionUniform_pointLight, 1, lightPosition);
		
		//pointLight
		glUniform1i(gNumPointLightsUniform_pointLight, 3);
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

		/*glUniform3fv(gKdUniform_perFragmentLight, 1, material_diffuse);
		glUniform3fv(gKaUniform_perFragmentLight, 1, material_ambient);
		glUniform3fv(gKsUniform_perFragmentLight, 1, material_specular);
		glUniform1f(gMaterialShininessUniform_perFragmentLight, material_shininess);*/

	}
	else
	{
		glUniform1i(gLKeyPressedUniform_pointLight, 0);
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
	glUniformMatrix4fv(gViewMatrixUniform_pointLight, 1, GL_FALSE, gViewMatrix);
	
	glUniform3fv(gViewPosUniform_pointLight, 1, vmath_camera_eye_coord);
	modelMatrix = vmath::translate(0.0f, 0.0f, -2.0f);

	glUniformMatrix4fv(gProjectionMatrixUniform_pointLight, 1, GL_FALSE, gPerspectiveProjectionMatrix);

	
	//object
	//Draw Mahal Model
	modelMatrix = mat4::identity();
	//modelMatrix = vmath::translate(0.0f, 0.0f, -6.0f);
	modelMatrix = vmath::translate(0.0f, -2.0f, -6.0f);
	scaleMatrix = scale(10.0f, 10.0f, 10.0f);
	//modelMatrix = modelMatrix * scaleMatrix;

	glUniformMatrix4fv(gModelMatrixUniform_pointLight, 1, GL_FALSE, modelMatrix);
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
				glBindTexture(GL_TEXTURE_2D, gModel_Mahal.model_material[gModel_Mahal.model_mesh_data[i].material_index].gTexture);
				glUniform1i(gTextureSamplerUniform_pointLight, 0);
				glUniform1i(gTextureActiveUniform_pointLight, 1);
				/*fprintf(gpFile, "perFragmentLight Mahal ismapKd true \n");
				fflush(gpFile);*/
			}
			else
				glUniform1i(gTextureActiveUniform_pointLight, 0);
		}
		glDrawArrays(GL_TRIANGLES, gModel_Mahal.model_mesh_data[i].vertex_Index, gModel_Mahal.model_mesh_data[i].vertex_Count);
	}
	glBindVertexArray(0);
	
	/*
	// KRISHNA MODEL
	modelMatrix		= mat4::identity();
	scaleMatrix		= mat4::identity();
	//modelMatrix = vmath::translate(50.0f, 0.0f, 5.0f);
	modelMatrix		= vmath::translate(-300.0f, 55.0f, 1.0f);
	scaleMatrix		= scale(gfKrishnaModelScale, gfKrishnaModelScale, gfKrishnaModelScale);
	rotateMatrix	= rotate(90.0f, 0.0f, 1.0f, 0.0f);
	
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
			if (gModel_Krishna.model_material[gModel_Krishna.model_mesh_data[i].material_index].ismap_Kd == true)
			{
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, gModel_Krishna.model_material[gModel_Krishna.model_mesh_data[i].material_index].gTexture);
				glUniform1i(gTextureSamplerUniform_perFragmentLight, 0);
				glUniform1i(gTextureActiveUniform_perFragmentLight, 1);
			}
			else
				glUniform1i(gTextureActiveUniform_perFragmentLight, 0);
		}
		glDrawArrays(GL_TRIANGLES, gModel_Krishna.model_mesh_data[i].vertex_Index, gModel_Krishna.model_mesh_data[i].vertex_Count);
	}
	glBindVertexArray(0);

	*/

	// shishupal model

	modelMatrix = mat4::identity();
	scaleMatrix = mat4::identity();
	
	modelMatrix = vmath::translate(2500.0f, 0.0f, 600.0f);
	scaleMatrix = scale(50.0f, 50.0f, 50.0f);
	rotateMatrix = rotate(230.0f, 0.0f, 1.0f, 0.0f);

	modelMatrix = modelMatrix * rotateMatrix * scaleMatrix;

	glUniformMatrix4fv(gModelMatrixUniform_pointLight, 1, GL_FALSE, modelMatrix);
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
				glBindTexture(GL_TEXTURE_2D, gModel_Krishna.model_material[gModel_Krishna.model_mesh_data[i].material_index].gTexture);
				glUniform1i(gTextureSamplerUniform_pointLight, 0);
				glUniform1i(gTextureActiveUniform_pointLight, 1);
				/*fprintf(gpFile, "perFragmentLight Krishna ismapKd true \n");
				fflush(gpFile);*/
			}
			else
				glUniform1i(gTextureActiveUniform_pointLight, 0);
		}
		glDrawArrays(GL_TRIANGLES, gModel_Krishna.model_mesh_data[i].vertex_Index, gModel_Krishna.model_mesh_data[i].vertex_Count);
	}
	glBindVertexArray(0);


	glUseProgram(0);
}

void update_pointLight()
{
	
	if (gbStartCamera)
	{

		//if (vmath_camera_eye_coord[0] < 20.0f && vmath_camera_eye_coord[1] < 840.0f && vmath_camera_center_coord[1] < 1270.0f && gbZoomOutForFullView == false)
		//if (vmath_camera_eye_coord[0] < 20.0f && vmath_camera_eye_coord[1] < 820.0f && vmath_camera_center_coord[1] < 1270.0f && gbZoomOutForFullView == false)
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
			/*
			if (vmath_camera_eye_coord[0] < 670.0f)
			{
				vmath_camera_eye_coord[0] = vmath_camera_eye_coord[0] + ((670.0f - 20.0f) / 1000.0f);			// 20.0f is old position , 670.0f is new, so old position to final position in 1000 steps
			}
			if (vmath_camera_eye_coord[1] < 1100.0f)
			{
				vmath_camera_eye_coord[1] = vmath_camera_eye_coord[1] + ((1100.0f - 840.0f) / 1000.0f);			
			}
			if (vmath_camera_eye_coord[2] < 11.0f)
			{
				vmath_camera_eye_coord[2] = vmath_camera_eye_coord[2] + ((11.0f - 1.0f) / 1000.0f);				// 1 old postion, 11 new position	
			}

			if (vmath_camera_center_coord[1] > 360.0f)
			{
				vmath_camera_center_coord[1] = vmath_camera_center_coord[1] - ((1270.0f - 360.0f) / 1000.0f);
			}
			

			if (vmath_camera_eye_coord[0] < 1900.0f)
			{
				vmath_camera_eye_coord[0] = vmath_camera_eye_coord[0] + ((1900.0f - 20.0f) / 1000.0f);			// 20.0f is old position , 1900.0f is new, so old position to final position in 1000 steps
			}
			if (vmath_camera_eye_coord[1] < 1130.2f)
			{
				vmath_camera_eye_coord[1] = vmath_camera_eye_coord[1] + ((1130.2f - 840.0f) / 1000.0f);
			}
			if (vmath_camera_eye_coord[2] > -550.0f)
			{
				vmath_camera_eye_coord[2] = vmath_camera_eye_coord[2] - ((550.0f + 1.0f) / 1000.0f);				// 1 old postion, -550.0f new position	
			}

			if (vmath_camera_center_coord[1] > -160.0f)
			{
				vmath_camera_center_coord[1] = vmath_camera_center_coord[1] - ((1270.0f + 160.0f) / 1000.0f);
			}
			

			if (vmath_camera_eye_coord[0] < 480.0f)
			{
				vmath_camera_eye_coord[0] = vmath_camera_eye_coord[0] + ((480.0f - 20.0f) / 1000.0f);			// 20.0f is old position , 480.0f is new, so old position to final position in 1000 steps
			}
			if (vmath_camera_eye_coord[1] > 20.0f)
			{
				vmath_camera_eye_coord[1] = vmath_camera_eye_coord[1] - ((840.0f - 20.0f) / 1000.0f);
			}
			if (vmath_camera_eye_coord[2] > -340.50f)
			{
				vmath_camera_eye_coord[2] = vmath_camera_eye_coord[2] - ((340.50f - 1.0f) / 1000.0f);				// 1 old postion, 11 new position	
			}

			if (vmath_camera_center_coord[1] > 1030.0f)
			{
				vmath_camera_center_coord[1] = vmath_camera_center_coord[1] - ((1270.0f - 1030.0f) / 1000.0f);
			}
			*/
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

void uninitialize_pointLight()
{
	uninitializeAllModelData();

	uninitializeSphereShape();

	programObjectSafeRelease(gShaderProgramObject_pointLight);

}