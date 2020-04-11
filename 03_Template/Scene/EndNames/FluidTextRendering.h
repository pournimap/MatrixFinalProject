#pragma once

#include "../../Include/freetype/include/ft2build.h"
#include FT_FREETYPE_H
#include <map>
#include"../../Include/solver.h"

#pragma comment(lib,"freetype.lib")

GLuint gVertexShaderObject_fluidText;
GLuint gFragmentShaderObject_fluidText;
GLuint gShaderProgramObject_fluidText;

GLuint gVao_Text_fluidText;
GLuint gVbo_Text_fluidText;
GLuint gVbo_Text_Texture_fluidText;

GLuint gModelMatrixUniform_fluidText, gViewMatrixUniform_fluidText, gProjectionMatrixUniform_fluidText;
GLuint gTextSamplerUniform_fluidText;
GLuint gTextColorUniform_fluidText;
GLuint fadeinFactorUniform_fluidText, fadeoutFactorUniform_fluidText;
mat4 gPerspectiveProjectionMatrix_fluidText;

struct Character {
	GLuint TextureID;
	ivec2 Size;
	ivec2 Bearing;
	GLuint Advance;
};

std::map<GLchar, Character> Characters;


// fbo_fluidText AND FLUID
GLuint gVertexShaderObject_FBO_fluidText;
GLuint gFragmentShaderObject_FBO_fluidText;
GLuint gShaderProgramObject_FBO_fluidText;
GLuint mvpUniform_FBO_fluidText;
GLuint samplerUniform_fluidText;
GLuint fadeinFactorUniform_FBO_fluidText, fadeoutFactorUniform_FBO_fluidText;

GLuint fbo_fluidText;

GLuint color_texture_fluidText;
GLuint depth_texture_fluidText;

GLuint vao_fluidText;
GLuint vbo_fluidText;

GLuint vaoQuads_fluidText;
GLuint vboPositionQuads_fluidText;
GLuint vboColorsQuads_fluidText;

// from paper

#define  N_fluidText  70
#define size_fluidText  (N_fluidText + 2) * (N_fluidText + 2)

float dt_fluidText			= 0.01f;
float diff_fluidText		= 0.00f;
float visc_fluidText		= 0.00f;
float force_fluidText		= 10.0f;
float source_fluidText		= 500.0f;

float u_fluidText[size_fluidText] = { 0.0 };
float v_fluidText[size_fluidText] = { 0.0 };

float u_prev_fluidText[size_fluidText] = { 0.0 };
float v_prev_fluidText[size_fluidText] = { 0.0 };

float dens_fluidText[size_fluidText]		= { 0.0 };
float dens_prev_fluidText[size_fluidText]	= { 0.0 };

int omx_fluidText, omy_fluidText, mx_fluidText, my_fluidText;

float QuadCoordinates_fluidText[12];
float QuadColors_fluidText[12];

float global_temp_count_fluidText = 2.0;
bool startAnimation_fluidText		= false;

int gWidgth_fluidText, gHeight_fluidText;

bool gbIsAnimationStart_fluidText = true;

GLfloat ZTransitionForName_fluidText = -35.0f;
int NameCount_fluidText = 1;

#pragma region FLUID_FUNCTION
void free_data(void)
{
	if (u_fluidText)
		free(u_fluidText);
	if (v_fluidText)
		free(v_fluidText);
	if (u_prev_fluidText)
		free(u_prev_fluidText);
	if (v_prev_fluidText)
		free(v_prev_fluidText);
	if (dens_fluidText)
		free(dens_fluidText);
	if (dens_prev_fluidText)
		free(dens_prev_fluidText);
}

void clear_data(void)
{
	int i;

	for (i = 0; i < size_fluidText; i++) {
		u_fluidText[i] = v_fluidText[i] = u_prev_fluidText[i] = v_prev_fluidText[i] = dens_fluidText[i] = dens_prev_fluidText[i] = 0.0f;
	}
}

void get_from_UI(float* d, float* u_fluidText, float* v_fluidText)
{

	static int i, i1, i2, i3, j;

	for (int p = 0; p < size_fluidText; p++)
	{
		u_fluidText[p] = v_fluidText[p] = d[p] = 0.0f;
	}

	// default i and j (source_fluidText points)
	i = 32;
	i2 = 30;
	j = 2;


	global_temp_count_fluidText = global_temp_count_fluidText + 0.01f;

	if (global_temp_count_fluidText > 2.00)
	{
		global_temp_count_fluidText = 0.0f;
	}

	mx_fluidText = rand() % 1;
	omx_fluidText = rand() % 1;

	// below combo used for upward direction fluid, like smoke
	my_fluidText = 1;
	omy_fluidText = 500;



	if (i<1 || i>N_fluidText || j<1 || j>N_fluidText)
	{
		//return;
	}


	u_fluidText[IX(i2, j)] = force_fluidText * (mx_fluidText - omx_fluidText);
	v_fluidText[IX(i2, j)] = force_fluidText * (omy_fluidText - my_fluidText);
	
	d[IX(i2, j)] = source_fluidText;

	return;
}

void draw_density(void)
{
	int i, j;
	float x, y, h, d00, d01, d10, d11;

	h = 1.0f / N_fluidText;

	for (i = 0; i <= N_fluidText; i++)
	{
		x = (i - 0.5f) * h;
		for (j = 0; j <= N_fluidText; j++)
		{
			y = (j - 0.5f) * h;

			d00 = dens_fluidText[IX(i, j)];
			d01 = dens_fluidText[IX(i, j + 1)];
			d10 = dens_fluidText[IX(i + 1, j)];
			d11 = dens_fluidText[IX(i + 1, j + 1)];

			QuadCoordinates_fluidText[0] = x;
			QuadCoordinates_fluidText[1] = y;
			QuadCoordinates_fluidText[2] = 0.0f;

			QuadCoordinates_fluidText[3] = x + h;
			QuadCoordinates_fluidText[4] = y;
			QuadCoordinates_fluidText[5] = 0.0f;

			QuadCoordinates_fluidText[6] = x + h;
			QuadCoordinates_fluidText[7] = y + h;
			QuadCoordinates_fluidText[8] = 0.0f;

			QuadCoordinates_fluidText[9] = x;
			QuadCoordinates_fluidText[10] = y + h;
			QuadCoordinates_fluidText[11] = 0.0f;


			//QuadColors_fluidText[0] = 1.0f;
			QuadColors_fluidText[0] = d00;
			QuadColors_fluidText[1] = d00;
			QuadColors_fluidText[2] = d00;

			//QuadColors_fluidText[3] = 1.0f;
			QuadColors_fluidText[3] = d10;
			QuadColors_fluidText[4] = d10;
			QuadColors_fluidText[5] = d10;

			//QuadColors_fluidText[6] = 1.0f;
			QuadColors_fluidText[6] = d11;
			QuadColors_fluidText[7] = d11;
			QuadColors_fluidText[8] = d11;

			//QuadColors_fluidText[9] = 1.0f;
			QuadColors_fluidText[9] = d01;
			QuadColors_fluidText[10] = d01;
			QuadColors_fluidText[11] = d01;


			glBindVertexArray(vaoQuads_fluidText);

			glBindBuffer(GL_ARRAY_BUFFER, vboPositionQuads_fluidText);
			glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), QuadCoordinates_fluidText, GL_DYNAMIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			glBindBuffer(GL_ARRAY_BUFFER, vboColorsQuads_fluidText);
			glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), QuadColors_fluidText, GL_DYNAMIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

			glBindVertexArray(0);


		}
	}
}

#pragma endregion

void Update_FluidText(void)
{
	if (ZTransitionForName_fluidText < 0.1f && NameCount_fluidText < 5)
	{
		if(ZTransitionForName_fluidText < -17.0f)
			ZTransitionForName_fluidText = ZTransitionForName_fluidText + 0.02f;
		else
			ZTransitionForName_fluidText = ZTransitionForName_fluidText + 0.3f;
	}
	else
	{
		ZTransitionForName_fluidText = -35.0f;
		NameCount_fluidText = NameCount_fluidText + 1;
		clear_data();
	}
}

void uninitialize(void)
{
	free_data();

	glDetachShader(gShaderProgramObject_fluidText, gVertexShaderObject_fluidText);
	glDetachShader(gShaderProgramObject_fluidText, gFragmentShaderObject_fluidText);

	glDeleteShader(gVertexShaderObject_fluidText);
	gVertexShaderObject_fluidText = 0;
	glDeleteShader(gFragmentShaderObject_fluidText);
	gFragmentShaderObject_fluidText = 0;

	glDeleteProgram(gShaderProgramObject_fluidText);
	gShaderProgramObject_fluidText = 0;

}

void initFluidText(void)
{
	// ---------------------------------------------------------------------------------------------------------------------------------

	gVertexShaderObject_fluidText = glCreateShader(GL_VERTEX_SHADER);
	const GLchar* vertextShaderSourceCode_fluidText =
		"#version 450" \
		"\n" \

		"in vec4 vPosition;" \

		"uniform mat4 u_model_matrix;" \
		"uniform mat4 u_view_matrix;" \
		"uniform mat4 u_projection_matrix;" \

		"out vec2 TexCoords;" \

		"void main(void)" \
		"{" \
			"gl_Position	= u_projection_matrix * u_view_matrix * u_model_matrix * vec4(vPosition.xy, 0.0, 1.0);" \
			"TexCoords		= vPosition.zw;" \
		"}";

	glShaderSource(gVertexShaderObject_fluidText, 1, (const GLchar**)&vertextShaderSourceCode_fluidText, NULL);
	glCompileShader(gVertexShaderObject_fluidText);
	checkCompilationLog((char*)"gVertexShaderObject_fluidText", gVertexShaderObject_fluidText);

// ---------------------------------------------------------------------------------------------------------------------------------

	gFragmentShaderObject_fluidText = glCreateShader(GL_FRAGMENT_SHADER);
	const GLchar* fragmentShaderSourceCode_fluidText =
		"#version 450" \
		"\n" \
		"in vec2 TexCoords;" \
		"out vec4 FragColor;" \

		"uniform sampler2D text;" \
		"uniform sampler2D u_sampler;" \

		"uniform float fadeinFactor;" \
		"uniform float fadeoutFactor;" \

		"vec3 glyph_color			= vec3(1.0, 1.0, 0.0);" \
		"const float glyph_center	= 0.5;" \
		"vec3 glow_color			= vec3(1.0, 1.0, 1.0);" \
		"const float glow_center	= 0.5;" \

		"void main(void)" \
		"{" \
			"vec4 fluid_tex = texture(u_sampler,TexCoords.xy);" \
			"vec4 color		= texture2D(text, TexCoords.xy);" \
			"float dist		= color.r;" \
			"float width	= fwidth(dist);" \
			"float alpha	= smoothstep(glyph_center - width, glyph_center+width, dist);" \
			"vec3 rgb		= mix(glow_color, glyph_color, alpha);" \
			"float mu		= smoothstep(glyph_center, glow_center, sqrt(dist));" \
			"FragColor		= vec4(rgb, max(alpha, mu));" \
			"FragColor		= FragColor * fluid_tex * fadeinFactor * fadeoutFactor;" \
		"}";

	glShaderSource(gFragmentShaderObject_fluidText, 1, (const GLchar**)&fragmentShaderSourceCode_fluidText, NULL);
	glCompileShader(gFragmentShaderObject_fluidText);
	checkCompilationLog((char*)"gFragmentShaderObject_fluidText", gFragmentShaderObject_fluidText);

// ---------------------------------------------------------------------------------------------------------------------------------

	gShaderProgramObject_fluidText = glCreateProgram();

	glAttachShader(gShaderProgramObject_fluidText, gVertexShaderObject_fluidText);
	glAttachShader(gShaderProgramObject_fluidText, gFragmentShaderObject_fluidText);
	glBindAttribLocation(gShaderProgramObject_fluidText, MATRIX_ATTRIBUTE_POSITION, "vPosition");
	glLinkProgram(gShaderProgramObject_fluidText);

	checkLinkLog((char*)"gShaderProgramObject_fluidText", gShaderProgramObject_fluidText);

	gModelMatrixUniform_fluidText			= glGetUniformLocation(gShaderProgramObject_fluidText, "u_model_matrix");
	gViewMatrixUniform_fluidText			= glGetUniformLocation(gShaderProgramObject_fluidText, "u_view_matrix");
	gProjectionMatrixUniform_fluidText		= glGetUniformLocation(gShaderProgramObject_fluidText, "u_projection_matrix");

	gTextSamplerUniform_fluidText	= glGetUniformLocation(gShaderProgramObject_fluidText, "text");
	samplerUniform_fluidText		= glGetUniformLocation(gShaderProgramObject_fluidText, "u_sampler");

	fadeinFactorUniform_fluidText = glGetUniformLocation(gShaderProgramObject_fluidText, "fadeinFactor");
	fadeoutFactorUniform_fluidText = glGetUniformLocation(gShaderProgramObject_fluidText, "fadeoutFactor");
	


// fbo_fluidText .....................................

	gVertexShaderObject_FBO_fluidText = glCreateShader(GL_VERTEX_SHADER);
	const GLchar* vertexShaderSourceCode_FBO_fluidText =
		"#version 450 core" \
		"\n" \
		"in vec4 vPosition;" \
		"in vec4 vColor;" \

		"out vec4 out_color;" \
		"uniform mat4 u_mvp_matrix;" \
		"void main(void)" \
		"{" \
			"gl_Position = u_mvp_matrix * vPosition;" \
			"out_color = vColor;" \
		"}";

	glShaderSource(gVertexShaderObject_FBO_fluidText, 1, (const GLchar**)&vertexShaderSourceCode_FBO_fluidText, NULL);
	glCompileShader(gVertexShaderObject_FBO_fluidText);
	checkCompilationLog((char*)"gVertexShaderObject_fluidText", gVertexShaderObject_FBO_fluidText);

// ---------------------------------------------------------------------------------------------------------------------------------

	gFragmentShaderObject_FBO_fluidText = glCreateShader(GL_FRAGMENT_SHADER);
	const GLchar* fragmentShaderSourceCode_FBO_fluidText =
		"#version 450 core" \
		"\n" \
		"in vec4 out_color;" \
		"out vec4 FragColor;" \

		"uniform float fadeinFactor;" \
		"uniform float fadeoutFactor;" \

		"void main(void)" \
		"{" \
			"FragColor = out_color * vec4(1.0,0.1,0.0,1.0) * fadeinFactor * fadeoutFactor;" \
			/*"if(FragColor.r < 0.1 && FragColor.g < 0.1 && FragColor.b < 0.1)" \
					"discard;" \*/
		"}";

	glShaderSource(gFragmentShaderObject_FBO_fluidText, 1, (const GLchar**)&fragmentShaderSourceCode_FBO_fluidText, NULL);
	glCompileShader(gFragmentShaderObject_FBO_fluidText);
	checkCompilationLog((char*)"gVertexShaderObject_fluidText", gFragmentShaderObject_FBO_fluidText);

// ---------------------------------------------------------------------------------------------------------------------------------

	gShaderProgramObject_FBO_fluidText = glCreateProgram();
	glAttachShader(gShaderProgramObject_FBO_fluidText, gVertexShaderObject_FBO_fluidText);
	glAttachShader(gShaderProgramObject_FBO_fluidText, gFragmentShaderObject_FBO_fluidText);

	glBindAttribLocation(gShaderProgramObject_FBO_fluidText, MATRIX_ATTRIBUTE_POSITION, "vPosition");
	glBindAttribLocation(gShaderProgramObject_FBO_fluidText, MATRIX_ATTRIBUTE_COLOR, "vColor");

	

	glLinkProgram(gShaderProgramObject_FBO_fluidText);
	checkLinkLog((char*)"gShaderProgramObject_fluidText", gShaderProgramObject_FBO_fluidText);
	

	mvpUniform_FBO_fluidText = glGetUniformLocation(gShaderProgramObject_FBO_fluidText, "u_mvp_matrix");
	fadeinFactorUniform_FBO_fluidText = glGetUniformLocation(gShaderProgramObject_FBO_fluidText, "fadeinFactor");
	fadeoutFactorUniform_FBO_fluidText = glGetUniformLocation(gShaderProgramObject_FBO_fluidText, "fadeoutFactor");

// --------------------------------------------------------------------------------------------------------------------------------

	
}


//FUNCTION DEFINITIONS
void initialize_FluidText(void)
{
	initFluidText();

#pragma region TEXT_PART

	glGenVertexArrays(1, &gVao_Text_fluidText);
	glBindVertexArray(gVao_Text_fluidText);

	glGenBuffers(1, &gVbo_Text_fluidText);
	glBindBuffer(GL_ARRAY_BUFFER, gVbo_Text_fluidText);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(MATRIX_ATTRIBUTE_POSITION, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(MATRIX_ATTRIBUTE_POSITION);
	glBindBuffer(GL_ARRAY_BUFFER, 0);


	glBindVertexArray(0);


	FT_Library ft;
	if (FT_Init_FreeType(&ft))
	{
		fopen_s(&gpFile, "Log.txt", "a+");
		fprintf(gpFile, "ERROR : FREETYPE : Could Not Init freetype library\n");
		fclose(gpFile);
	}
	FT_Face face;

	FT_Error error = FT_New_Face(ft, "AMS Chhatrapati Regular.ttf", 0, &face);

	if (error == FT_Err_Unknown_File_Format)
	{
		fopen_s(&gpFile, "Log.txt", "a+");
		fprintf(gpFile, "ERROR : FREETYPE : font file unsupported \n");
		fclose(gpFile);
	}
	else if (error)
	{
		fopen_s(&gpFile, "Log.txt", "a+");
		fprintf(gpFile, "ERROR : FREETYPE : could not be opened \n");
		fclose(gpFile);
	}
	FT_Set_Pixel_Sizes(face, 0, 48);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	for (GLubyte c = 0; c < 128; c++)
	{
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			fopen_s(&gpFile, "Log.txt", "a+");
			fprintf(gpFile, "ERROR:: FREETYPE : Failed tpo load Glyph\n");
			fclose(gpFile);
			continue;
		}

		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED,
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			0, GL_RED,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


		Character character = {
			texture,
			ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			face->glyph->advance.x
		};
		Characters.insert(std::pair<GLchar, Character>(c, character));

	}
	glBindTexture(GL_TEXTURE_2D, 0);
	FT_Done_Face(face);
	FT_Done_FreeType(ft);

#pragma endregion

#pragma region fbo_fluidText
	glGenFramebuffers(1, &fbo_fluidText);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo_fluidText);


	glGenTextures(1, &color_texture_fluidText);
	glBindTexture(GL_TEXTURE_2D, color_texture_fluidText);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, 1024, 1024);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


	glGenTextures(1, &depth_texture_fluidText);
	glBindTexture(GL_TEXTURE_2D, depth_texture_fluidText);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH_COMPONENT32F, 1024, 1024);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, color_texture_fluidText, 0);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depth_texture_fluidText, 0);


	static const GLenum draw_buffers[] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, draw_buffers);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
	}
	else
	{
	}
#pragma endregion

#pragma region FLUID

	// QUADS
	glGenVertexArrays(1, &vaoQuads_fluidText);
	glBindVertexArray(vaoQuads_fluidText);

	glGenBuffers(1, &vboPositionQuads_fluidText);
	glBindBuffer(GL_ARRAY_BUFFER, vboPositionQuads_fluidText);
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), NULL, GL_DYNAMIC_DRAW);				// x, y * 4 (points)	
	glVertexAttribPointer(MATRIX_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(MATRIX_ATTRIBUTE_POSITION);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &vboColorsQuads_fluidText);
	glBindBuffer(GL_ARRAY_BUFFER, vboColorsQuads_fluidText);
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), NULL, GL_DYNAMIC_DRAW);				// r, g, b * 4 (points)
	glVertexAttribPointer(MATRIX_ATTRIBUTE_COLOR, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(MATRIX_ATTRIBUTE_COLOR);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

#pragma endregion

}

void display_FluidText(void)
{

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	std::string text;

	mat4 modelViewProjectionMatrixOrtho_fluidText = mat4::identity();

	// START fbo_fluidText ..........................................................................................
	glBindFramebuffer(GL_FRAMEBUFFER, fbo_fluidText);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glViewport(0, 0, (GLsizei)1024, (GLsizei)1024);
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(gShaderProgramObject_FBO_fluidText);


	glUniform1f(fadeoutFactorUniform_FBO_fluidText, 1.0f);
	glUniform1f(fadeinFactorUniform_FBO_fluidText, 1.0f);

	modelViewProjectionMatrixOrtho_fluidText = ortho(0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f);

	glUniformMatrix4fv(mvpUniform_FBO_fluidText, 1, GL_FALSE, modelViewProjectionMatrixOrtho_fluidText);

	draw_density();
	get_from_UI(dens_prev_fluidText, u_prev_fluidText, v_prev_fluidText);
	vel_step(N_fluidText, u_fluidText, v_fluidText, u_prev_fluidText, v_prev_fluidText, visc_fluidText, dt_fluidText);
	dens_step(N_fluidText, dens_fluidText, dens_prev_fluidText, u_fluidText, v_fluidText, diff_fluidText, dt_fluidText);
	

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glUseProgram(0);

	// END fbo_fluidText .................................................................................................


	//code
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glViewport(0, 0, (GLsizei)gWidth, (GLsizei)gHeight);

	glUseProgram(gShaderProgramObject_fluidText);

	glUniform1f(fadeoutFactorUniform_fluidText, 1.0f);
	glUniform1f(fadeinFactorUniform_fluidText, 1.0f);

	mat4 modelMatrix_fluidText		= mat4::identity();
	mat4 viewMatrix_fluidText		= mat4::identity();
	mat4 scaleMatrix_fluidText		= mat4::identity();
	mat4 rotationMatrix_fluidText	= mat4::identity();

	if (NameCount_fluidText == 1)
	{
		text = "maalataI desaaQ";
		modelMatrix_fluidText = translate(-12.0f, -0.0f, ZTransitionForName_fluidText);
	}
	else if (NameCount_fluidText == 2)
	{
		text = "BauVaNa kLmakr";
		modelMatrix_fluidText = translate(-14.0f, -0.0f, ZTransitionForName_fluidText);
	}
	else if (NameCount_fluidText == 3)
	{
		text = "Ajaya Ambaure";
		modelMatrix_fluidText = translate(-15.0f, -0.0f, ZTransitionForName_fluidText);
	}
	else
	{
		//std::string text = "Baarta maaiJare";
		text = "Baarta maaiJare";
		modelMatrix_fluidText = translate(-12.0f, -0.0f, ZTransitionForName_fluidText);
	}

	glUniformMatrix4fv(gModelMatrixUniform_fluidText, 1, GL_FALSE, modelMatrix_fluidText);
	glUniformMatrix4fv(gViewMatrixUniform_fluidText, 1, GL_FALSE, viewMatrix_fluidText);
	glUniformMatrix4fv(gProjectionMatrixUniform_fluidText, 1, GL_FALSE, gPerspectiveProjectionMatrix);			// from source.cpp
	
	glBindVertexArray(gVao_Text_fluidText);
	
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, color_texture_fluidText);
	glUniform1i(samplerUniform_fluidText, 1);


	glActiveTexture(GL_TEXTURE0);

	std::string::const_iterator c;
	float scale = 0.1f;
	float x		= -0.5f;
	float y		= -0.5f;
	
	for (c = text.begin(); c != text.end(); c++)
	{
		Character ch = Characters[*c];

		GLfloat xpos = x + ch.Bearing[0] * scale;
		GLfloat ypos = y - (ch.Size[1] - ch.Bearing[1]) * scale;

		GLfloat w = ch.Size[0] * scale;
		GLfloat h = ch.Size[1] * scale;

		GLfloat vertices[6][4] = {
			{ xpos,     ypos + h,   0.0, 0.0 },
			{ xpos,     ypos,       0.0, 1.0 },
			{ xpos + w, ypos,       1.0, 1.0 },

			{ xpos,     ypos + h,   0.0, 0.0 },
			{ xpos + w, ypos,       1.0, 1.0 },
			{ xpos + w, ypos + h,   1.0, 0.0 }
		};

		glBindTexture(GL_TEXTURE_2D, ch.TextureID);
		glUniform1i(gTextSamplerUniform_fluidText, 0);

		glBindBuffer(GL_ARRAY_BUFFER, gVbo_Text_fluidText);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glDrawArrays(GL_TRIANGLES, 0, 6);

		x += (ch.Advance >> 6)* scale;
	}

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);

	
	glUseProgram(0);

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// update

	Update_FluidText();
}



