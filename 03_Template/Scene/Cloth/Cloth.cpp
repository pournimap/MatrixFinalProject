
#include<windows.h>
#include<C:\glew\include\GL\glew.h>
#include<gl\GL.h>
#include<stdio.h>
//#include<gl/glu.h>
#define _USE_MATH_DEFINES 1
#include<math.h>
#include<time.h>
#include <vector>
#include "vmath.h"
#include "../../Include/Camera/Camera_2.h"

#include "cuda.h" // for CUDA


#include <cuda_gl_interop.h>
#include <cuda_runtime.h>
#include "helper_cuda.h"
#include "helper_timer.h"

using namespace vmath;

//******** Linker to link the OpenGl Libraray **********//
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "C:\\glew\\lib\\Release\\x64\\glew32.lib")
#pragma comment(lib, "cudart.lib")


//******* Macros **********//
#define WIN_WIDTH          800
#define WIN_HEIGHT         600


extern Camera Scene_camera;

enum {
	AMC_ATTRIBUTE_POSITION = 0,
	AMC_ATTRIBUTE_COLOR,
	AMC_ATTRIBUTE_NORMAL,
	AMC_ATTRIBUTE_TEXCOORD0
};

//**** Global variable declaration ************//

FILE *ClothgpFile = NULL;
struct cudaGraphicsResource *ClothgraphicResourcePos = NULL;
struct cudaGraphicsResource *ClothgraphicResourceNor = NULL;
cudaError_t error;

// Shaders Varible
GLuint ClothgVertexShaderObject;
GLuint ClothgFragmentShaderObject;
GLuint ClothglShaderProgramObject;

GLuint Clothvao;
//GLuint Clothvbo,vbo_normal;
GLuint Clothvbo_pos_gpu,Clothvbo_normal_gpu;
GLuint ClothmodelMatrix_Uniform, ClothviewMatrix_Uniform, Clothprojection_Uniform;
GLuint Clothld_Uniform, Clothla_Uniform, Clothls_Uniform, ClothlightPosition_Uniform;
GLuint Clothka_Uniform, Clothkd_Uniform, Clothks_Uniform;
GLuint ClothLKeyPress_Uniform, ClothmaterialShinyness_Uniform;
GLuint ClothApplyBloomUniform, ClothBloomIsActiveUniform;
GLuint ClothBloom_thresh_minUniform, ClothBloom_thresh_maxUniform;
GLuint ClothFadeinFactorUniform, ClothFadeoutFactorUniform;

float ClothlightAmbient[4] = { 0.2f,0.2f,0.2f,0.2f };
float ClothlightDiffuse[4] = { 0.5f,0.0f,0.0f,1.0f };
float ClothlightSpecular[4] = { 0.7f,0.5f,0.2f,1.0f };
float ClothlightPosition[4] = { 100.0f,100.0f,100.0f,1.0f };

float ClothmaterialAmbient[4] = { 0.01f,0.01f,0.01f,1.0f };
float ClothmaterialDiffuse[4] = { 1.0f,1.0f,1.0f,1.0f };
float ClothmaterialSpecular[4] = { 1.0f,1.0f,1.0f,1.0f };
float ClothmaterialShinyness = 128.0f;

mat4 ClothperspectiveProjectionMatrix;  //this is from Vmath
bool ClothbLight = true;


float Clothlength(float* v)
{
	return sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
}


#define DAMPING 0.0 // how much to damp the cloth simulation each frame
#define TIME_STEPSIZE2 0.3f  // how large time step each particle takes each frame
#define CONSTRAINT_ITERATIONS 50 // how many iterations of constraint satisfaction each frame (more is rigid, less is soft)



#define CLOTHWIDTH   40.0
#define CLOTHHEIGHT  60.0
#define CLOTHWIDTH_NOOFPARTICLES  120
#define CLOTHHEIGHT_NOOFPARTICLES  80
#define NOOFPARTICLE CLOTHWIDTH_NOOFPARTICLES*CLOTHHEIGHT_NOOFPARTICLES
#define NEIGHBOUR_PARTICLES_ARRAY_INDEX 37802 //((CLOTHWIDTH_NOOFPARTICLES*CLOTHHEIGHT_NOOFPARTICLES*4)-598)


float3 ClothParticles_pos_array[NOOFPARTICLE];
float3 ClothParticles_old_pos_array[NOOFPARTICLE];
float3 *ClothGPUParticles_pos_array = NULL;
float3 *ClothGPUParticles_old_pos_array = NULL;

int2 NeighbourParticlesInddex[NEIGHBOUR_PARTICLES_ARRAY_INDEX];
int2 *GPUNeighbourParticlesInddex = NULL;
bool MovableStatus[NOOFPARTICLE];
bool *ClothGPUMovableStatus = NULL;
float ClothRestDistance[NEIGHBOUR_PARTICLES_ARRAY_INDEX];
float* ClothGPURestDistance = NULL;
float3 ClothParticles_Normals[NOOFPARTICLE];
float3 *ClothGPUParticles_Normal = NULL;
float3 ClothParticles_Accelaration[NOOFPARTICLE];
float3 *ClothGPUParticles_Accelaration = NULL;
size_t tPitch = 0;

float windForceY = 0.4f;
extern bool angry_cloth_flag;

int Clothparticle_width =CLOTHWIDTH_NOOFPARTICLES, Clothparticle_height =CLOTHHEIGHT_NOOFPARTICLES;
float mass = 1.0;
//float3 Clothaccelaration={0.0 * TIME_STEPSIZE2, -0.01 , 0.000000001};
float3 Clothaccelaration = { 0.0 * TIME_STEPSIZE2, -0.09 , 0.000000005 };

//float3 ClothwindForce = {0.0f * TIME_STEPSIZE2, 0.0f * TIME_STEPSIZE2, 0.0f * TIME_STEPSIZE2};
float3 ClothwindForce = { 0.0f * TIME_STEPSIZE2, windForceY * TIME_STEPSIZE2, 0.05f * TIME_STEPSIZE2 };
int widthaddjustment = 0;
//********* Global function declaration ***********//

float3 ClothtriangleVertices[NOOFPARTICLE * 6] ;
float3 ClothtriangleVertices_normal[NOOFPARTICLE * 6];

float3 *ClothGPUtriangleVertices = NULL;
float3 *ClothGPUtriangleVertices_normal = NULL;


float3 getParticle(int x, int y) { return ClothParticles_pos_array[y*Clothparticle_width + x]; }
float3 getParticleNormals(int x, int y) { return ClothParticles_Normals[y*Clothparticle_width + x]; }

int ClothgetParticles_pos_array_index(int x, int y) { return y*Clothparticle_width + x; }

#define gNumPointLights_Cloth  20
struct PointLightUniform_Cloth
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
PointLightUniform_Cloth m_pointLightsLocation[gNumPointLights_Cloth];

struct PointLight_Cloth
{
	vec3 u_La;
	vec3 u_Ld;
	vec3 u_Ls;
	float DiffuseIntensity;
	float u_constant;
	float u_linear;
	float u_quadratic;
	vec3 position;
	PointLight_Cloth()
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
GLuint ClothViewPosUniform, ClothNumPointLightsUniform;

extern FILE* gpFile;

void ClothMovable()
{
	for (int i = 0; i < (Clothparticle_width - widthaddjustment)/2; i = i+20)
		{
			// Decide Particles which should not move
			MovableStatus[ClothgetParticles_pos_array_index(0 + i,0)] = false;
			MovableStatus[ClothgetParticles_pos_array_index(0 + i+1,0)] = false;
			MovableStatus[ClothgetParticles_pos_array_index(0 + i+2,0)] = false;
			MovableStatus[ClothgetParticles_pos_array_index(0 + i+3,0)] = false;
			MovableStatus[ClothgetParticles_pos_array_index(0 + i+4,0)] = false;
			
			MovableStatus[ClothgetParticles_pos_array_index(Clothparticle_width-1-i,0)] = false;
			MovableStatus[ClothgetParticles_pos_array_index(Clothparticle_width-1-i+1,0)] = false;
			MovableStatus[ClothgetParticles_pos_array_index(Clothparticle_width-1-i+2,0)] = false;
			MovableStatus[ClothgetParticles_pos_array_index(Clothparticle_width-1-i+3,0)] = false;
			MovableStatus[ClothgetParticles_pos_array_index(Clothparticle_width-1-i+4,0)] = false;
						
		}
		
		
}


void Cloth(float width, float height, int num_particles_width, int num_particles_height) 
{

	// creating particles in a grid of particles from (0,0,0) to (width,-height,0)
		for (int x = 0; x < num_particles_width; x++)
		{
			for (int y = 0; y < num_particles_height; y++)
			{
				float pos[3] = {width * (x / (float)num_particles_width),
					-height * (y / (float)num_particles_height),
				0};
				
				ClothParticles_pos_array[y*num_particles_width + x].x = pos[0];
				ClothParticles_pos_array[y*num_particles_width + x].y = pos[1];
				ClothParticles_pos_array[y*num_particles_width + x].z = pos[2];
				
				
				
				ClothParticles_old_pos_array[y*num_particles_width + x].x = pos[0];
				ClothParticles_old_pos_array[y*num_particles_width + x].y = pos[1];
				ClothParticles_old_pos_array[y*num_particles_width + x].z = pos[2];
								
			}
		}
		
		int index = 0;
		float distance[3];
		
		for (int x = 0; x < num_particles_width; x++)
		{
			for (int y = 0; y < num_particles_height; y++)
			{
				if (x < num_particles_width - 1) 
				{
					NeighbourParticlesInddex[index].x = ClothgetParticles_pos_array_index(x,y);
					NeighbourParticlesInddex[index].y = ClothgetParticles_pos_array_index(x+1,y);
					
					
					// the Clothlength between particle p1 and p2 in rest configuration
				
					distance[0] = ClothParticles_pos_array[NeighbourParticlesInddex[index].x].x - ClothParticles_pos_array[NeighbourParticlesInddex[index].y].x;
					distance[1] = ClothParticles_pos_array[NeighbourParticlesInddex[index].x].y - ClothParticles_pos_array[NeighbourParticlesInddex[index].y].y;
					distance[2] = ClothParticles_pos_array[NeighbourParticlesInddex[index].x].z - ClothParticles_pos_array[NeighbourParticlesInddex[index].y].z;
					ClothRestDistance[index] = Clothlength(distance);
					distance[0] = 0.0f;
					distance[1] = 0.0f;
					distance[2] = 0.0f;
					
					index++;
				}
				
				if (y < num_particles_height - 1) 
				{
					NeighbourParticlesInddex[index].x = ClothgetParticles_pos_array_index(x,y);
					NeighbourParticlesInddex[index].y = ClothgetParticles_pos_array_index(x,y+1);
					
					distance[0] = ClothParticles_pos_array[NeighbourParticlesInddex[index].x].x - ClothParticles_pos_array[NeighbourParticlesInddex[index].y].x;
					distance[1] = ClothParticles_pos_array[NeighbourParticlesInddex[index].x].y - ClothParticles_pos_array[NeighbourParticlesInddex[index].y].y;
					distance[2] = ClothParticles_pos_array[NeighbourParticlesInddex[index].x].z - ClothParticles_pos_array[NeighbourParticlesInddex[index].y].z;
					ClothRestDistance[index] = Clothlength(distance);
					distance[0] = 0.0f;
					distance[1] = 0.0f;
					distance[2] = 0.0f;
					
					index++;
				}
				
				if (x < num_particles_width - 1 && y < num_particles_height - 1)
				{
					NeighbourParticlesInddex[index].x = ClothgetParticles_pos_array_index(x,y);
					NeighbourParticlesInddex[index].y = ClothgetParticles_pos_array_index(x+1,y+1);
					
					distance[0] = ClothParticles_pos_array[NeighbourParticlesInddex[index].x].x - ClothParticles_pos_array[NeighbourParticlesInddex[index].y].x;
					distance[1] = ClothParticles_pos_array[NeighbourParticlesInddex[index].x].y - ClothParticles_pos_array[NeighbourParticlesInddex[index].y].y;
					distance[2] = ClothParticles_pos_array[NeighbourParticlesInddex[index].x].z - ClothParticles_pos_array[NeighbourParticlesInddex[index].y].z;
					ClothRestDistance[index] = Clothlength(distance);
					distance[0] = 0.0f;
					distance[1] = 0.0f;
					distance[2] = 0.0f;
					
					index++;
				}
				
				if (x < num_particles_width - 1 && y < num_particles_height - 1)
				{
					NeighbourParticlesInddex[index].x = ClothgetParticles_pos_array_index(x+1,y);
					NeighbourParticlesInddex[index].y = ClothgetParticles_pos_array_index(x,y+1);
					
					distance[0] = ClothParticles_pos_array[NeighbourParticlesInddex[index].x].x - ClothParticles_pos_array[NeighbourParticlesInddex[index].y].x;
					distance[1] = ClothParticles_pos_array[NeighbourParticlesInddex[index].x].y - ClothParticles_pos_array[NeighbourParticlesInddex[index].y].y;
					distance[2] = ClothParticles_pos_array[NeighbourParticlesInddex[index].x].z - ClothParticles_pos_array[NeighbourParticlesInddex[index].y].z;
					ClothRestDistance[index] = Clothlength(distance);
					distance[0] = 0.0f;
					distance[1] = 0.0f;
					distance[2] = 0.0f;
					
					index++;
					
				}
			}
		}

		ClothMovable();
		
}


int Clothintialize(void)
{
	//fopen_s(&ClothgpFile, "ClothLog.txt", "w");
	fprintf(gpFile, "Log File Successfully Created.\n");
	fflush(gpFile);

	//Define VertexShader Object
	ClothgVertexShaderObject = glCreateShader(GL_VERTEX_SHADER);
	//Specify/Write vertex shader code

	const GLchar *VertexShaderSourseCode =
		"#version 450 core" \
		"\n" \
		"in vec4 vPosition;" \
		"in vec3 vNormal;" \

		"uniform int u_Lkeypress;" \
		"uniform mat4 u_model_matrix;" \
		"uniform mat4 u_view_matrix;" \
		"uniform mat4 u_projection_matrix;" \
		"uniform vec4 u_lightPosition;" \

		"out vec3 Transformednormal;" \
		"out vec3 lightDirection;" \
		"out vec3 viewer_vector;" \
		"out vec3 fragment_position;" \

		"void main(void)" \
		"{" \
		"if(u_Lkeypress==1)" \
		"{" \
		"fragment_position = vec3(u_model_matrix * vPosition);" \
		"vec4 eye_coordinate = u_view_matrix * u_model_matrix * vPosition;" \
		"Transformednormal=mat3(u_view_matrix * u_model_matrix) * vNormal;" \
		"lightDirection = vec3(u_lightPosition - eye_coordinate);" \
		"viewer_vector =vec3(-eye_coordinate.xyz);" \
		"}" \
		"gl_Position = u_projection_matrix * u_view_matrix * u_model_matrix * vPosition;" \
		"}";

	//Specify above code to Vertex Shader object
	glShaderSource(ClothgVertexShaderObject, 1, (GLchar const**)&VertexShaderSourseCode, NULL);

	GLint iShaderCompileStatus = 0;
	GLint iInfologLength = 0;
	GLchar *szInfoLog = NULL;

	//Compile the Vertex shader
	glCompileShader(ClothgVertexShaderObject);
	glGetShaderiv(ClothgVertexShaderObject, GL_COMPILE_STATUS, &iShaderCompileStatus);

	if (iShaderCompileStatus == GL_FALSE)
	{
		glGetShaderiv(ClothgVertexShaderObject, GL_INFO_LOG_LENGTH, &iInfologLength);
		if (iInfologLength > 0)
		{
			szInfoLog = (GLchar*)malloc(iInfologLength);
			if (szInfoLog != NULL)
			{
				GLsizei written;
				glGetShaderInfoLog(ClothgVertexShaderObject, iInfologLength, &written, szInfoLog);

				fprintf(gpFile, "\n\nClothgVertexShaderObject Compilation Error: \n%s\n\n", szInfoLog);
				fflush(gpFile);

				free(szInfoLog);
		
				exit(0);

			}
		}
	}


	//Define FragmentShader Object
	ClothgFragmentShaderObject = glCreateShader(GL_FRAGMENT_SHADER);
	//Specify/Write fragment shader code

	const GLchar *FragmentShaderSourseCode =
		
		"#version 450 core" \
		"\n"
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


		"layout (location = 0) out vec4 fragColor;" \
		"layout (location = 1) out vec4 BloomColor;" \
		"layout (location = 2) out vec4 GodRaysColor;" \
		"uniform int u_Lkeypress;" \
		"uniform vec3 u_la;" \
		"uniform vec3 u_ld;" \
		"uniform vec3 u_ls;" \
		"uniform vec3 u_ka;" \
		"uniform vec3 u_kd;" \
		"uniform vec3 u_ks;" \
		"uniform float u_materialShinyness;" \

		"uniform int applyBloom;" \
		"uniform float bloom_thresh_min = 0.8f;" \
		"uniform float bloom_thresh_max = 1.2f;" \
		"uniform int u_bloom_is_active;" \

		"uniform float fadeinFactor;" \
		"uniform float fadeoutFactor;" \

		"vec3 phong_ads_light;" \
		"in vec3 Transformednormal;" \
		"in vec3 lightDirection;" \
		"in vec3 viewer_vector;" \
		"in vec3 fragment_position;" \


			"vec3 calculatePointLight(int index)" \
			"{" \
			"vec3 pointLightColor;" \

			"vec3 normalized_transformed_normals=normalize(Transformednormal);" \

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


			"vec3 ambient = pointLight[index].u_La * u_ka;" \
			"float tn_dot_ld = max(dot(normalized_transformed_normals, normalized_light_direction), 0.0);" \
			"vec3 diffuse = pointLight[index].u_Ld * u_kd * tn_dot_ld;" \

			"vec3 specular = vec3(0.0, 0.0, 0.0);" \
			/*"if(u_is_texture == 1)" \
			"{" \*/
			"vec3 reflection_vector = reflect(-normalized_light_direction, normalized_transformed_normals);" \
			"specular = pointLight[index].u_Ls * pow(max(dot(reflection_vector, normalized_viewer_vector), 0.0), u_materialShinyness);" \
			/*"}" \*/

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
		"if(u_Lkeypress==1)" \
		"{" \
			"vec3 pointLightColor = vec3(0.0, 0.0, 0.0);" \
			"for(int i = 0; i < gNumPointLights; i++)" \
			"{" \
			"pointLightColor += calculatePointLight(i);" \
			"}" \
			"phong_ads_light = pointLightColor;" \
		"}" \
		"else" \
		"{" \
		"phong_ads_light = vec3(1.0f,1.0f,1.0f);" \
		"}" \
		"fragColor = vec4(phong_ads_light,1.0) * fadeinFactor * fadeoutFactor;" \

		"if(applyBloom == 1)" \
		"{" \
		"vec4 c = fragColor;" \
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

	//Specify above code to Fragment Shader object
	glShaderSource(ClothgFragmentShaderObject, 1, (GLchar const**)&FragmentShaderSourseCode, NULL);

	iShaderCompileStatus = 0;
	iInfologLength = 0;


	//Compile the Fragment shader
	glCompileShader(ClothgFragmentShaderObject);
	glGetShaderiv(ClothgFragmentShaderObject, GL_COMPILE_STATUS, &iShaderCompileStatus);

	if (iShaderCompileStatus == GL_FALSE)
	{
		glGetShaderiv(ClothgFragmentShaderObject, GL_INFO_LOG_LENGTH, &iInfologLength);
		if (iInfologLength > 0)
		{
			szInfoLog = (GLchar*)malloc(iInfologLength);
			if (szInfoLog != NULL)
			{
				GLsizei written;
				glGetShaderInfoLog(ClothgFragmentShaderObject, iInfologLength, &written, szInfoLog);

				fprintf(gpFile, "\nClothgFragmentShaderObject Compilation Error: \n%s\n\n", szInfoLog);
				fflush(gpFile);
				free(szInfoLog);
			
				exit(0);

			}
		}
	}

	// Create Shader Program Object
	ClothglShaderProgramObject = glCreateProgram();

	//Attach Vertex Shader to the Shader Program
	glAttachShader(ClothglShaderProgramObject, ClothgVertexShaderObject);

	//Attach Fragnebt Shader
	glAttachShader(ClothglShaderProgramObject, ClothgFragmentShaderObject);

	// Prelinking binding to Vertex attribute
	glBindAttribLocation(ClothglShaderProgramObject, AMC_ATTRIBUTE_POSITION, "vPosition");
	glBindAttribLocation(ClothglShaderProgramObject, AMC_ATTRIBUTE_NORMAL, "vNormal");

	//Link the Shader Program
	glLinkProgram(ClothglShaderProgramObject);

	GLint iProgramLinkStatus = 0;
	iInfologLength = 0;
	//ErrorCheking
	glGetProgramiv(ClothglShaderProgramObject, GL_LINK_STATUS, &iProgramLinkStatus);

	if (iProgramLinkStatus == GL_FALSE)
	{
		glGetProgramiv(ClothglShaderProgramObject, GL_INFO_LOG_LENGTH, &iInfologLength);
		if (iInfologLength > 0)
		{
			szInfoLog = (GLchar*)malloc(iInfologLength);
			if (szInfoLog != NULL)
			{
				GLsizei written;
				glGetProgramInfoLog(ClothglShaderProgramObject, iInfologLength, &written, szInfoLog);
				
				fprintf(gpFile, "\n ClothglShaderProgramObject Compilation Error: \n%s\n\n", szInfoLog);
				fflush(gpFile);

				free(szInfoLog);
				exit(0);
			}
		}
	}
	
	//Post linking retriving/getting uniform location 
	ClothmodelMatrix_Uniform = glGetUniformLocation(ClothglShaderProgramObject, "u_model_matrix");
	ClothviewMatrix_Uniform = glGetUniformLocation(ClothglShaderProgramObject, "u_view_matrix");
	Clothprojection_Uniform = glGetUniformLocation(ClothglShaderProgramObject, "u_projection_matrix");

	Clothla_Uniform = glGetUniformLocation(ClothglShaderProgramObject, "u_la");
	Clothld_Uniform = glGetUniformLocation(ClothglShaderProgramObject, "u_ld");
	Clothls_Uniform = glGetUniformLocation(ClothglShaderProgramObject, "u_ls");
	ClothlightPosition_Uniform = glGetUniformLocation(ClothglShaderProgramObject, "u_lightPosition");

	Clothka_Uniform = glGetUniformLocation(ClothglShaderProgramObject, "u_ka");
	Clothkd_Uniform = glGetUniformLocation(ClothglShaderProgramObject, "u_kd");
	Clothks_Uniform = glGetUniformLocation(ClothglShaderProgramObject, "u_ks");
	ClothmaterialShinyness_Uniform = glGetUniformLocation(ClothglShaderProgramObject, "u_materialShinyness");
	ClothLKeyPress_Uniform = glGetUniformLocation(ClothglShaderProgramObject, "u_Lkeypress");

	ClothApplyBloomUniform = glGetUniformLocation(ClothglShaderProgramObject, "applyBloom");
	ClothBloomIsActiveUniform = glGetUniformLocation(ClothglShaderProgramObject, "u_bloom_is_active");
	ClothBloom_thresh_minUniform = glGetUniformLocation(ClothglShaderProgramObject, "bloom_thresh_min");
	ClothBloom_thresh_maxUniform = glGetUniformLocation(ClothglShaderProgramObject, "bloom_thresh_max");

	ClothFadeinFactorUniform = glGetUniformLocation(ClothglShaderProgramObject, "fadeinFactor");
	ClothFadeoutFactorUniform = glGetUniformLocation(ClothglShaderProgramObject, "fadeoutFactor");

	ClothViewPosUniform = glGetUniformLocation(ClothglShaderProgramObject, "viewPos");
	ClothNumPointLightsUniform = glGetUniformLocation(ClothglShaderProgramObject, "gNumPointLights");
	

	char Name[128];
	for (int i = 0; i < gNumPointLights_Cloth; i++)
	{
		memset(Name, 0, sizeof(Name));

		snprintf(Name, sizeof(Name), "pointLight[%d].u_La", i);
		m_pointLightsLocation[i].u_La = glGetUniformLocation(ClothglShaderProgramObject, Name);

		snprintf(Name, sizeof(Name), "pointLight[%d].u_Ld", i);
		m_pointLightsLocation[i].u_Ld = glGetUniformLocation(ClothglShaderProgramObject, Name);

		snprintf(Name, sizeof(Name), "pointLight[%d].u_Ls", i);
		m_pointLightsLocation[i].u_Ls = glGetUniformLocation(ClothglShaderProgramObject, Name);

		snprintf(Name, sizeof(Name), "pointLight[%d].u_constant", i);
		m_pointLightsLocation[i].u_constant = glGetUniformLocation(ClothglShaderProgramObject, Name);

		snprintf(Name, sizeof(Name), "pointLight[%d].u_linear", i);
		m_pointLightsLocation[i].u_linear = glGetUniformLocation(ClothglShaderProgramObject, Name);

		snprintf(Name, sizeof(Name), "pointLight[%d].u_quadratic", i);
		m_pointLightsLocation[i].u_quadratic = glGetUniformLocation(ClothglShaderProgramObject, Name);

		snprintf(Name, sizeof(Name), "pointLight[%d].position", i);
		m_pointLightsLocation[i].position = glGetUniformLocation(ClothglShaderProgramObject, Name);

		snprintf(Name, sizeof(Name), "pointLight[%d].diffuseIntensity", i);
		m_pointLightsLocation[i].DiffuseIntensity = glGetUniformLocation(ClothglShaderProgramObject, Name);
	}


	//Ortho fixfunction program
	
	//Create vao
	glGenVertexArrays(1, &Clothvao);
	glBindVertexArray(Clothvao);
	
	glGenBuffers(1, &Clothvbo_pos_gpu);
	glBindBuffer(GL_ARRAY_BUFFER, Clothvbo_pos_gpu);

	glBufferData(GL_ARRAY_BUFFER, NOOFPARTICLE * 6 * sizeof(float3), NULL, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(AMC_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(AMC_ATTRIBUTE_POSITION);
	
	error = cudaGraphicsGLRegisterBuffer(&ClothgraphicResourcePos,Clothvbo_pos_gpu,cudaGraphicsMapFlagsWriteDiscard);
	
	if(error !=cudaSuccess)
	{
		exit(0);
	}
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	
	glGenBuffers(1, &Clothvbo_normal_gpu);
	glBindBuffer(GL_ARRAY_BUFFER, Clothvbo_normal_gpu);
	glBufferData(GL_ARRAY_BUFFER, NOOFPARTICLE * 6 * sizeof(float3), NULL, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(AMC_ATTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(AMC_ATTRIBUTE_NORMAL);

	
	
	error = cudaGraphicsGLRegisterBuffer(&ClothgraphicResourceNor,Clothvbo_normal_gpu,cudaGraphicsMapFlagsWriteDiscard);
	
	if(error !=cudaSuccess)
	{
		exit(0);	
	}
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

  
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glClearDepth(1.0f);

	glDepthFunc(GL_LEQUAL);


	glEnable(GL_TEXTURE_2D);
	
	for(int index = 0; index < NOOFPARTICLE; index++)
		MovableStatus[index] = true;
	
	for(int x = 0; x < NOOFPARTICLE; x++)
	{
		ClothParticles_Accelaration[x].x = 0.0f;
		ClothParticles_Accelaration[x].y = 0.0f;
		ClothParticles_Accelaration[x].z = 0.0f;
	}
	
	 
	size_t tPitch = 0;
	
	cudaMallocPitch((void **)&ClothGPUParticles_Accelaration, &tPitch, sizeof(float3)*CLOTHWIDTH_NOOFPARTICLES, CLOTHHEIGHT_NOOFPARTICLES);
	cudaMallocPitch((void **)&ClothGPUParticles_pos_array, &tPitch, sizeof(float3)*CLOTHWIDTH_NOOFPARTICLES, CLOTHHEIGHT_NOOFPARTICLES);
	cudaMallocPitch((void **)&ClothGPUParticles_old_pos_array, &tPitch, sizeof(float3)*CLOTHWIDTH_NOOFPARTICLES, CLOTHHEIGHT_NOOFPARTICLES);
	cudaMallocPitch((void **)&ClothGPUParticles_Normal, &tPitch, sizeof(float3)*CLOTHWIDTH_NOOFPARTICLES, CLOTHHEIGHT_NOOFPARTICLES);
	
	
	cudaMalloc((void**)&GPUNeighbourParticlesInddex, sizeof(int2) * (NEIGHBOUR_PARTICLES_ARRAY_INDEX));
	cudaMalloc((void**)&ClothGPURestDistance, sizeof(float) * (NEIGHBOUR_PARTICLES_ARRAY_INDEX));
	cudaMalloc((void**)&ClothGPUMovableStatus, sizeof(bool) * NOOFPARTICLE);
	
	cudaMalloc((void**)&ClothGPUtriangleVertices, sizeof(float3) * NOOFPARTICLE * 6);
	cudaMalloc((void**)&ClothGPUtriangleVertices_normal, sizeof(float3) * NOOFPARTICLE * 6);
	
	
	
    Cloth(CLOTHWIDTH,CLOTHHEIGHT,CLOTHWIDTH_NOOFPARTICLES,CLOTHHEIGHT_NOOFPARTICLES);
	
	cudaMemcpy(GPUNeighbourParticlesInddex, NeighbourParticlesInddex, (NEIGHBOUR_PARTICLES_ARRAY_INDEX) * sizeof(int2)  , cudaMemcpyHostToDevice);
	cudaMemcpy(ClothGPURestDistance, ClothRestDistance, (NEIGHBOUR_PARTICLES_ARRAY_INDEX) * sizeof(float)  , cudaMemcpyHostToDevice);
	cudaMemcpy(ClothGPUMovableStatus, MovableStatus, NOOFPARTICLE * sizeof(bool)  , cudaMemcpyHostToDevice);
	
	cudaMemcpy(ClothGPUParticles_old_pos_array, ClothParticles_old_pos_array, sizeof(float3)* NOOFPARTICLE   , cudaMemcpyHostToDevice);
	cudaMemcpy(ClothGPUParticles_pos_array, ClothParticles_pos_array, NOOFPARTICLE * sizeof(float3)  , cudaMemcpyHostToDevice);
		
	fprintf(gpFile, "\n End of Cloth initialize\n\n");
	fflush(gpFile);
	return(0);
}

void ClothtimeStep()
	{
		//void timeStepCudaKernel(float3* ClothGPUParticles_pos_array,int2* GPUNeighbourParticlesInddex,float* ClothGPURestDistance,bool* ClothGPUMovableStatus, int NoOfwidthparticles,int NoOfheightparticle,int Totalthreads);
		cudaMemcpy(ClothParticles_pos_array , ClothGPUParticles_pos_array, NOOFPARTICLE * sizeof(float3)  , cudaMemcpyDeviceToHost);
		
		//cudaMemcpy(ClothGPUParticles_pos_array, ClothParticles_pos_array, NOOFPARTICLE * sizeof(float3)  , cudaMemcpyHostToDevice);
		for (int i = 0; i < 10; i++) // iterate over all constraints several times
		{
			
			//timeStepCudaKernel(ClothGPUParticles_pos_array,GPUNeighbourParticlesInddex,ClothGPURestDistance,ClothGPUMovableStatus,CLOTHWIDTH_NOOFPARTICLES,(CLOTHHEIGHT_NOOFPARTICLES*4),NEIGHBOUR_PARTICLES_ARRAY_INDEX);
			

			for (int index = 0; index < NEIGHBOUR_PARTICLES_ARRAY_INDEX; index++)
			{
				// vector from p1 to p2
				float p1_to_p2[3];
				p1_to_p2[0] = ClothParticles_pos_array[NeighbourParticlesInddex[index].y].x - ClothParticles_pos_array[NeighbourParticlesInddex[index].x].x;
				p1_to_p2[1] = ClothParticles_pos_array[NeighbourParticlesInddex[index].y].y - ClothParticles_pos_array[NeighbourParticlesInddex[index].x].y;
				p1_to_p2[2] = ClothParticles_pos_array[NeighbourParticlesInddex[index].y].z - ClothParticles_pos_array[NeighbourParticlesInddex[index].x].z;
												
				float current_distance = Clothlength(p1_to_p2); // current distance between p1 and p2
				
				// the Clothlength between particle p1 and p2 in rest configuration
								
				// The offset vector that could moves p1 into a distance of rest_distance to p2
				//vec3 correctionVector = p1_to_p2 * (1 - rest_distance / current_distance); 
				float correctionVector[3];
				correctionVector[0] = p1_to_p2[0] * (1 - ClothRestDistance[index] / current_distance);
				correctionVector[1] = p1_to_p2[1] * (1 - ClothRestDistance[index] / current_distance);
				correctionVector[2] = p1_to_p2[2] * (1 - ClothRestDistance[index] / current_distance);
				
				// Lets make it half that Clothlength, so that we can move BOTH p1 and p2.
				//vec3 correctionVectorHalf = correctionVector * 0.5; 
				float correctionVectorHalf[3];
				correctionVectorHalf[0] = correctionVector[0] * 0.5;
				correctionVectorHalf[1] = correctionVector[1] * 0.5;
				correctionVectorHalf[2] = correctionVector[2] * 0.5;
				
				
				// correctionVectorHalf is pointing from p1 to p2, so the Clothlength should move p1 half the Clothlength needed to satisfy the constraint.
				if(MovableStatus[NeighbourParticlesInddex[index].x])
				{
					ClothParticles_pos_array[NeighbourParticlesInddex[index].x].x = ClothParticles_pos_array[NeighbourParticlesInddex[index].x].x + correctionVectorHalf[0];
					ClothParticles_pos_array[NeighbourParticlesInddex[index].x].y = ClothParticles_pos_array[NeighbourParticlesInddex[index].x].y + correctionVectorHalf[1];
					ClothParticles_pos_array[NeighbourParticlesInddex[index].x].z = ClothParticles_pos_array[NeighbourParticlesInddex[index].x].z + correctionVectorHalf[2];
				}
				
				if(MovableStatus[NeighbourParticlesInddex[index].y])
				{
					ClothParticles_pos_array[NeighbourParticlesInddex[index].y].x = ClothParticles_pos_array[NeighbourParticlesInddex[index].y].x - correctionVectorHalf[0];
					ClothParticles_pos_array[NeighbourParticlesInddex[index].y].y = ClothParticles_pos_array[NeighbourParticlesInddex[index].y].y - correctionVectorHalf[1];
					ClothParticles_pos_array[NeighbourParticlesInddex[index].y].z = ClothParticles_pos_array[NeighbourParticlesInddex[index].y].z - correctionVectorHalf[2];
				}
			}
			
			
		}
		
		cudaMemcpy(ClothGPUParticles_pos_array, ClothParticles_pos_array, NOOFPARTICLE * sizeof(float3)  , cudaMemcpyHostToDevice);
		void timeStepDisplacementCudaKernel(float3* KParticles_pos_array,float3* KParticles_old_pos_array,bool* ClothGPUMovableStatus,float dampingFactor, float3* partcleArray,int Clothparticle_width, int Clothparticle_height);
		
		timeStepDisplacementCudaKernel(ClothGPUParticles_pos_array,ClothGPUParticles_old_pos_array,ClothGPUMovableStatus,DAMPING, ClothGPUParticles_Accelaration,Clothparticle_width, Clothparticle_height);
		
	
	}



GLfloat ClothangleX = 0.0f;
GLfloat ClothangleY = 0.0f;
GLfloat ClothangleZ = 0.0f;

/*float* Clothcross(const float* v1, const float* v2)
{
	float crossProduct[3] = {v1[1] * v2[2] - v1[2] * v2[1], v1[2] * v2[0] - v1[0] * v2[2], v1[0] * v2[1] - v1[1] * v2[0]};
	return &crossProduct[0];
}*/

void Clothaddforce()
{
	void addforceCudaKernel(float3* partcleArray, float3 Clothaccelaration, int numOfParticles, int Clothparticle_width, int Clothparticle_height);
	
	addforceCudaKernel(ClothGPUParticles_Accelaration, Clothaccelaration, NOOFPARTICLE, Clothparticle_width,  Clothparticle_height);
	   
}

void ClothapplyWindForce()
{
	
	void applyWindForceCudaKernel(float3* ClothGPUParticles_pos_array,float3* partcleArray, int Clothparticle_width, int Clothparticle_height, float3 windforce);
	
	applyWindForceCudaKernel(ClothGPUParticles_pos_array,ClothGPUParticles_Accelaration, Clothparticle_width, Clothparticle_height, ClothwindForce);
		
}

vec3 positionLamp_Cloth[] = { vec3(-720.0f, 600.0f, 440.0f), vec3(-720.0f, 600.0f, 750.0f), vec3(-250.0f, 400.0f, 750.0f), vec3(320.0f, 400.0f, 750.0f), vec3(850.0f, 400.0f, 750.0f),vec3(1420.0f, 400.0f, 750.0f) , vec3(1950.0f, 400.0f, 750.0f) , vec3(2520.0f, 400.0f, 750.0f) ,
						vec3(3105.0f, 400.0f, 750.0f) , vec3(3650.0f,400.0f, 750.0f) ,
						vec3(-750.0f, 600.0f, -510.0f), vec3(-750.0f, 600.0f, -810.0f), vec3(-250.0f, 400.0f, -810.0f), vec3(320.0f, 400.0f, -810.0f), vec3(850.0f, 400.0f, -810.0f),vec3(1420.0f, 400.0f, -810.0f) , vec3(2000.0f, 400.0f, -810.0f) , vec3(2570.0f, 400.0f, -810.0f) ,
						vec3(3100.0f, 400.0f, -810.0f) , vec3(3700.0f, 400.0f, -810.0f) , };


extern mat4 gPerspectiveProjectionMatrix;
extern mat4 gViewMatrix;
extern vec3 vmath_camera_eye_coord;
extern float bloom_thresh_min;
extern float bloom_thresh_max;
extern bool startJoin_krishnaAnimate;
extern bool isShowStartingScene;
extern bool bStartFadeOutSecondScene;
float FadeInFactor_cloth = 0.0f;
float FadeOutFactor_cloth = 1.0f;
void Clothdisplay()
{
	//code
	void unintialize(void);
	void DrawCloth(vec3 translate, vec3 scaleM, bool isRotation,  float angle = 0.0f);
	void DrawCentralCloth(vec3 translate, vec3 scaleM);

	glUseProgram(ClothglShaderProgramObject);

	if (FadeInFactor_cloth <= 1.0f)
		FadeInFactor_cloth += 0.001f;
	

	if (bStartFadeOutSecondScene == true)
	{
		if (FadeOutFactor_cloth >= 0.0f)
			FadeOutFactor_cloth -= 0.001f;
	}
	glUniform1f(ClothFadeoutFactorUniform, FadeOutFactor_cloth);
	glUniform1f(ClothFadeinFactorUniform, 1.0f);

	glUniform1i(ClothBloomIsActiveUniform, 1);
	glUniform1i(ClothApplyBloomUniform, 1);
	
	glUniform1f(ClothBloom_thresh_minUniform, bloom_thresh_min);
	glUniform1f(ClothBloom_thresh_maxUniform, bloom_thresh_max);
	//Declaration of Matrix
	mat4 modelMatrix;
	mat4 viewMatrix;
	mat4 translateMatrix;
	mat4 rotationMatrixX;
	mat4 rotationMatrixY;
	mat4 rotationMatrixZ;
	mat4 scalematrix;
	mat4 modelViewProjectionMatrix;

	//Intialize above matrix to Identity;
	modelMatrix = mat4::identity();
	viewMatrix = mat4::identity();
	//viewMatrix = Scene_camera.GetViewMatrix();
	
	translateMatrix = mat4::identity();
	rotationMatrixX = mat4::identity();
	rotationMatrixY = mat4::identity();
	rotationMatrixZ = mat4::identity();
	scalematrix = mat4::identity();
	modelViewProjectionMatrix = mat4::identity();
	ClothlightPosition[0] = 100.0f;
	ClothlightPosition[1] = 100.0f;
	ClothlightPosition[2] = 100.0f;
	ClothlightPosition[3] = 1.0f;
	
	//Do necessary transformation
	
	PointLight_Cloth pointLight[gNumPointLights_Cloth];

	for (int i = 0; i < gNumPointLights_Cloth; i++)
	{
		pointLight[i].u_La = vec3(0.01f, 0.01f, 0.01f);
		pointLight[i].u_Ld = vec3(0.5f, 0.0f, 0.0f);
		pointLight[i].u_Ls = vec3(0.7f, 0.5f, 0.2f);
		pointLight[i].u_linear = 0.01;
		pointLight[i].u_constant = 0.01;
		pointLight[i].u_quadratic = 0.0;
		pointLight[i].DiffuseIntensity = 1.0f;

		pointLight[i].position = positionLamp_Cloth[i];
	}
	
	glUniform3fv(ClothViewPosUniform, 1, vmath_camera_eye_coord);
	glUniform1i(ClothNumPointLightsUniform, gNumPointLights_Cloth);
	for (int i = 0; i < gNumPointLights_Cloth; i++)
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


	glUniformMatrix4fv(ClothviewMatrix_Uniform, 1, GL_FALSE, gViewMatrix);
	glUniformMatrix4fv(Clothprojection_Uniform, 1, GL_FALSE, gPerspectiveProjectionMatrix);


	glUniform1i(ClothLKeyPress_Uniform, 1);
	glUniform3fv(Clothla_Uniform, 1, ClothlightAmbient);
	glUniform3fv(Clothld_Uniform, 1, ClothlightDiffuse);
	glUniform3fv(Clothls_Uniform, 1, ClothlightSpecular);
	glUniform4fv(ClothlightPosition_Uniform, 1, ClothlightPosition);

	glUniform3fv(Clothka_Uniform, 1, ClothmaterialAmbient);
	glUniform3fv(Clothkd_Uniform, 1, ClothmaterialDiffuse);
	glUniform3fv(Clothks_Uniform, 1, ClothmaterialSpecular);
	glUniform1f(ClothmaterialShinyness_Uniform, ClothmaterialShinyness);


	Clothaddforce();
	ClothapplyWindForce();

	ClothtimeStep();
	glBindVertexArray(Clothvao);

	void calculateNormalCudaKernel(float3* KParticles_pos_array,float3* KParticles_Normal, int Kparticle_width, int Kparticle_height);
	
	calculateNormalCudaKernel(ClothGPUParticles_pos_array,ClothGPUParticles_Normal, Clothparticle_width, Clothparticle_height);
		
	void renderCudaKernel(float3* KtriangleVertices,float3* KtriangleVertices_normal, float3* KParticles_pos_array, float3* KParticles_Normal, int Kparticle_width, int Kparticle_height);
	
	//renderCudaKernel(ClothGPUtriangleVertices,ClothGPUtriangleVertices_normal, ClothGPUParticles_pos_array, ClothGPUParticles_Normal, Clothparticle_width, Clothparticle_height);
	
	size_t byteCount;
	
	error = cudaGraphicsMapResources(1,&ClothgraphicResourcePos,0);
		if (error != cudaSuccess) {
			exit(0);
		}
		//Step 2 
		error = cudaGraphicsResourceGetMappedPointer((void**)&ClothGPUtriangleVertices, &byteCount, ClothgraphicResourcePos);
		if (error != cudaSuccess) {
			exit(0);
		}
		
		
		
		error = cudaGraphicsMapResources(1,&ClothgraphicResourceNor,0);
		if (error != cudaSuccess) {
			exit(0);
		}
		//Step 2 
		error = cudaGraphicsResourceGetMappedPointer((void**)&ClothGPUtriangleVertices_normal, &byteCount, ClothgraphicResourceNor);
		if (error != cudaSuccess) {
			exit(0);
		}
	
			
		//Step 3 launch cuda kernel
		renderCudaKernel(ClothGPUtriangleVertices,ClothGPUtriangleVertices_normal, ClothGPUParticles_pos_array, ClothGPUParticles_Normal, Clothparticle_width, Clothparticle_height);
		
		// Step 4 unmap resorce
		error = cudaGraphicsUnmapResources(1,&ClothgraphicResourcePos,0);
		if (error != cudaSuccess) {
			exit(0);
		}
		
		error = cudaGraphicsUnmapResources(1,&ClothgraphicResourceNor,0);
		if (error != cudaSuccess) {
			exit(0);
		}
		//first right
		DrawCloth(vec3(-750.0f, 700.0f, -900.0f), vec3(12.0f, 13.0f, 1.0f), 0);
		
		//secondcloth
		DrawCloth(vec3(-200.0f, 700.0f, -900.0f), vec3(12.0f, 13.0f, 1.0f), 0);

		//Third cloth
		DrawCloth(vec3(400.0f, 700.0f, -900.0f), vec3(12.0f, 13.0f, 1.0f), 0);

		//fourth cloth
		DrawCloth(vec3(900.0f, 700.0f, -900.0f), vec3(12.0f, 13.0f, 1.0f), 0);

		//fifth cloth
		DrawCloth(vec3(1500.0f, 700.0f, -900.0f), vec3(12.0f, 13.0f, 1.0f), 0);

		//sixth cloth
		DrawCloth(vec3(2100.0f, 700.0f, -900.0f), vec3(12.0f, 13.0f, 1.0f), 0);

		//seventh cloth
		DrawCloth(vec3(2600.0f, 700.0f, -900.0f), vec3(12.0f, 13.0f, 1.0f), 0);
	
		//eight cloth
		DrawCloth(vec3(3200.0f, 700.0f, -900.0f), vec3(12.0f, 13.0f, 1.0f), 0);
	//////////////////////
	
		//1st left
		DrawCloth(vec3(-320.0f, 700.0f, 900.0f), vec3(12.0f, 13.0f, 1.0f), 1, 180.0f);

		//2nd left
		DrawCloth(vec3(200.0f, 700.0f, 900.0f), vec3(12.0f, 13.0f, 1.0f), 1, 180.0f);
		
		//3rd left
		DrawCloth(vec3(700.0f, 700.0f, 900.0f), vec3(12.0f, 13.0f, 1.0f), 1, 180.0f);

		//4rth left
		DrawCloth(vec3(1200.0f, 700.0f, 900.0f), vec3(12.0f, 13.0f, 1.0f), 1, 180.0f);
		
		//5th left
		DrawCloth(vec3(1700.0f, 700.0f, 900.0f), vec3(12.0f, 13.0f, 1.0f), 1, 180.0f);

		//6th left
		DrawCloth(vec3(2200.0f, 700.0f, 900.0f), vec3(12.0f, 13.0f, 1.0f), 1, 180.0f);

		//7th left
		DrawCloth(vec3(2700.0f, 700.0f, 900.0f), vec3(12.0f, 13.0f, 1.0f), 1, 180.0f);

		//6th left
		DrawCloth(vec3(3250.0f, 700.0f, 900.0f), vec3(12.0f, 13.0f, 1.0f), 1, 180.0f);
		//////////////////////////////////////////////////////
		
		//Front cloth , Stage cloth , Center cloth	
	
		ClothlightPosition[0] = -815.0f;
		ClothlightPosition[1] = 75.0f;
		ClothlightPosition[2] = 17.0f;
		ClothlightPosition[3] = 1.0f;
		glUniform4fv(ClothlightPosition_Uniform, 1, ClothlightPosition);
		DrawCentralCloth(vec3(-820.0f, 57.5f, 17.0f), vec3(1.0f, 15.0f, 15.0f));

		DrawCentralCloth(vec3(-900.0f, 57.5f, 17.0f), vec3(1.0f, 15.0f, 30.0f));
		//unbind Clothvao
		glBindVertexArray(0);
	

	glUseProgram(0);


	if (angry_cloth_flag == true)
	{
		ClothwindForce.y = 0.29f;
		ClothwindForce.z = 0.02f;
	}
	else {
		ClothwindForce.y = 0.0f;
		ClothwindForce.z = 0.0f;
	}

}

void DrawCentralCloth(vec3 translateM, vec3 scaleM)
{
	mat4 modelMatrix = mat4::identity();
	mat4 translateMatrix = mat4::identity();
	mat4 rotationMatrixX = mat4::identity();
	mat4 rotationMatrixY = mat4::identity();
	mat4 rotationMatrixZ = mat4::identity();
	mat4 scalematrix = mat4::identity();
	mat4 modelViewProjectionMatrix = mat4::identity();

	translateMatrix = translate(translateM);
	scalematrix = scale(scaleM);
	rotationMatrixY = rotate(90.0f, 0.0f, 1.0f, 0.0f);

	translateMatrix = scalematrix * translateMatrix ;

	modelMatrix = modelMatrix * translateMatrix * rotationMatrixY;
	

	glUniformMatrix4fv(ClothmodelMatrix_Uniform, 1, GL_FALSE, modelMatrix);

	glBindBuffer(GL_ARRAY_BUFFER, Clothvbo_pos_gpu);
	glBindBuffer(GL_ARRAY_BUFFER, 0);


	glBindBuffer(GL_ARRAY_BUFFER, Clothvbo_normal_gpu);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glDrawArrays(GL_TRIANGLES, 0, sizeof(ClothtriangleVertices) / sizeof(ClothtriangleVertices[0]));

}
void DrawCloth(vec3 translateM, vec3 scaleM, bool isRotation, float angle)
{
	mat4 modelMatrix = mat4::identity();
	mat4 translateMatrix = mat4::identity();
	mat4 rotationMatrixX = mat4::identity();
	mat4 rotationMatrixY = mat4::identity();
	mat4 rotationMatrixZ = mat4::identity();
	mat4 scalematrix = mat4::identity();
	mat4 modelViewProjectionMatrix = mat4::identity();

	translateMatrix = translate(translateM);
	scalematrix = scale(scaleM);
	rotationMatrixY = rotate(angle, 0.0f, 1.0f, 0.0f);

	translateMatrix = translateMatrix * scalematrix;
	if(isRotation)
		modelMatrix = modelMatrix * translateMatrix * rotationMatrixY;
	else
		modelMatrix = modelMatrix * translateMatrix;

	glUniformMatrix4fv(ClothmodelMatrix_Uniform, 1, GL_FALSE, modelMatrix);

	glBindBuffer(GL_ARRAY_BUFFER, Clothvbo_pos_gpu);
	glBindBuffer(GL_ARRAY_BUFFER, 0);


	glBindBuffer(GL_ARRAY_BUFFER, Clothvbo_normal_gpu);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glDrawArrays(GL_TRIANGLES, 0, sizeof(ClothtriangleVertices) / sizeof(ClothtriangleVertices[0]));

}

void Clothunintialize(void) {

	// ************* If screen is fullscreen then resize then Unintialize ***********//
	
	if (Clothvbo_pos_gpu) {
		glDeleteBuffers(1, &Clothvbo_pos_gpu);
		Clothvbo_pos_gpu = 0;
	}

	if (Clothvao) {
		glDeleteBuffers(1, &Clothvao);
		Clothvao = 0;
	}

	//safe release shader
	if (ClothglShaderProgramObject) {
		GLsizei shaderCount;
		GLsizei shaderNumber;

		glUseProgram(ClothglShaderProgramObject);
		//ask program How many shader attached with you

		glGetProgramiv(ClothglShaderProgramObject, GL_ATTACHED_SHADERS, &shaderCount);

		GLuint *pShader = (GLuint*)malloc(shaderCount * sizeof(GLuint));
		if (pShader) {
			glGetAttachedShaders(ClothglShaderProgramObject, shaderCount, &shaderCount, pShader);

			for (shaderNumber = 0; shaderNumber < shaderCount; shaderNumber++)
			{
				//Detached one by one
				glDetachShader(ClothglShaderProgramObject, pShader[shaderNumber]);
				//delete detach shader
				glDeleteShader(pShader[shaderNumber]);
				pShader[shaderNumber] = 0;

			}
			free(pShader);
		}
		glDeleteProgram(ClothglShaderProgramObject);
		ClothglShaderProgramObject = 0;
		glUseProgram(0);
	}

}


void display_clothOnly() {
	//code
	void unintialize(void);
	void DrawCloth(vec3 translate, vec3 scaleM, bool isRotation, float angle = 0.0f);
	void DrawCentralCloth(vec3 translate, vec3 scaleM);

	glUseProgram(ClothglShaderProgramObject);

	if (FadeInFactor_cloth <= 1.0f)
		FadeInFactor_cloth += 0.001f;


	if (bStartFadeOutSecondScene == true)
	{
		if (FadeOutFactor_cloth >= 0.0f)
			FadeOutFactor_cloth -= 0.001f;
	}
	glUniform1f(ClothFadeoutFactorUniform, FadeOutFactor_cloth);
	glUniform1f(ClothFadeinFactorUniform, 1.0f);

	glUniform1i(ClothBloomIsActiveUniform, 1);
	glUniform1i(ClothApplyBloomUniform, 1);

	glUniform1f(ClothBloom_thresh_minUniform, bloom_thresh_min);
	glUniform1f(ClothBloom_thresh_maxUniform, bloom_thresh_max);
	//Declaration of Matrix
	mat4 modelMatrix;
	mat4 viewMatrix;
	mat4 translateMatrix;
	mat4 rotationMatrixX;
	mat4 rotationMatrixY;
	mat4 rotationMatrixZ;
	mat4 scalematrix;
	mat4 modelViewProjectionMatrix;

	//Intialize above matrix to Identity;
	modelMatrix = mat4::identity();
	viewMatrix = mat4::identity();

	translateMatrix = mat4::identity();
	rotationMatrixX = mat4::identity();
	rotationMatrixY = mat4::identity();
	rotationMatrixZ = mat4::identity();
	scalematrix = mat4::identity();
	modelViewProjectionMatrix = mat4::identity();
	ClothlightPosition[0] = 100.0f;
	ClothlightPosition[1] = 100.0f;
	ClothlightPosition[2] = 100.0f;
	ClothlightPosition[3] = 1.0f;

	//Do necessary transformation

	PointLight_Cloth pointLight[gNumPointLights_Cloth];

	for (int i = 0; i < gNumPointLights_Cloth; i++)
	{
		pointLight[i].u_La = vec3(0.01f, 0.01f, 0.01f);
		pointLight[i].u_Ld = vec3(0.5f, 0.0f, 0.0f);
		pointLight[i].u_Ls = vec3(0.7f, 0.5f, 0.2f);
		pointLight[i].u_linear = 0.01;
		pointLight[i].u_constant = 0.01;
		pointLight[i].u_quadratic = 0.0;
		pointLight[i].DiffuseIntensity = 1.0f;

		pointLight[i].position = positionLamp_Cloth[i];
	}

	glUniform3fv(ClothViewPosUniform, 1, vmath_camera_eye_coord);
	glUniform1i(ClothNumPointLightsUniform, gNumPointLights_Cloth);
	for (int i = 0; i < gNumPointLights_Cloth; i++)
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


	glUniformMatrix4fv(ClothviewMatrix_Uniform, 1, GL_FALSE, viewMatrix);
	glUniformMatrix4fv(Clothprojection_Uniform, 1, GL_FALSE, gPerspectiveProjectionMatrix);


	glUniform1i(ClothLKeyPress_Uniform, 1);
	glUniform3fv(Clothla_Uniform, 1, ClothlightAmbient);
	glUniform3fv(Clothld_Uniform, 1, ClothlightDiffuse);
	glUniform3fv(Clothls_Uniform, 1, ClothlightSpecular);
	glUniform4fv(ClothlightPosition_Uniform, 1, ClothlightPosition);

	glUniform3fv(Clothka_Uniform, 1, ClothmaterialAmbient);
	glUniform3fv(Clothkd_Uniform, 1, ClothmaterialDiffuse);
	glUniform3fv(Clothks_Uniform, 1, ClothmaterialSpecular);
	glUniform1f(ClothmaterialShinyness_Uniform, ClothmaterialShinyness);


	Clothaddforce();
	ClothapplyWindForce();

	ClothtimeStep();
	glBindVertexArray(Clothvao);

	void calculateNormalCudaKernel(float3 * KParticles_pos_array, float3 * KParticles_Normal, int Kparticle_width, int Kparticle_height);

	calculateNormalCudaKernel(ClothGPUParticles_pos_array, ClothGPUParticles_Normal, Clothparticle_width, Clothparticle_height);

	void renderCudaKernel(float3 * KtriangleVertices, float3 * KtriangleVertices_normal, float3 * KParticles_pos_array, float3 * KParticles_Normal, int Kparticle_width, int Kparticle_height);

	//renderCudaKernel(ClothGPUtriangleVertices,ClothGPUtriangleVertices_normal, ClothGPUParticles_pos_array, ClothGPUParticles_Normal, Clothparticle_width, Clothparticle_height);

	size_t byteCount;

	error = cudaGraphicsMapResources(1, &ClothgraphicResourcePos, 0);
	if (error != cudaSuccess) {
		exit(0);
	}
	//Step 2 
	error = cudaGraphicsResourceGetMappedPointer((void**)&ClothGPUtriangleVertices, &byteCount, ClothgraphicResourcePos);
	if (error != cudaSuccess) {
		exit(0);
	}



	error = cudaGraphicsMapResources(1, &ClothgraphicResourceNor, 0);
	if (error != cudaSuccess) {
		exit(0);
	}
	//Step 2 
	error = cudaGraphicsResourceGetMappedPointer((void**)&ClothGPUtriangleVertices_normal, &byteCount, ClothgraphicResourceNor);
	if (error != cudaSuccess) {
		exit(0);
	}


	//Step 3 launch cuda kernel
	renderCudaKernel(ClothGPUtriangleVertices, ClothGPUtriangleVertices_normal, ClothGPUParticles_pos_array, ClothGPUParticles_Normal, Clothparticle_width, Clothparticle_height);

	// Step 4 unmap resorce
	error = cudaGraphicsUnmapResources(1, &ClothgraphicResourcePos, 0);
	if (error != cudaSuccess) {
		exit(0);
	}

	error = cudaGraphicsUnmapResources(1, &ClothgraphicResourceNor, 0);
	if (error != cudaSuccess) {
		exit(0);
	}
	//first right
	DrawCloth(vec3(10.0f, 40.0f, -100.0f), vec3(1.0f, 1.0f, 1.0f), 0);

	ClothlightPosition[0] = -815.0f;
	ClothlightPosition[1] = 75.0f;
	ClothlightPosition[2] = 17.0f;
	ClothlightPosition[3] = 1.0f;
	glUniform4fv(ClothlightPosition_Uniform, 1, ClothlightPosition);


	glBindVertexArray(0);


	glUseProgram(0);


	if (angry_cloth_flag == true)
	{
		
		ClothwindForce.y = 0.29f;
		ClothwindForce.z = 0.02f;
	}
	else {
		ClothwindForce.y = 0.0f;
		ClothwindForce.z = 0.0f;
	}
}