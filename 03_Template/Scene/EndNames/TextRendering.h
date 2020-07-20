#include "../../Include/freetype/include/ft2build.h"
#include FT_FREETYPE_H

//function declaration
void initTextRendering();
void init_EndReference();
void DrawingLoop(std::string text, float x, float y);
void display_textRendering_FirstPage();

void init_EndReference();
void drawEndReference();

//variable declaration
GLuint gVertexShaderObject_TextRender, gFragmentShaderObject_TextRender, gShaderProgramObject_TextRender;

GLuint gModelMatrixUniform_TextRender, gViewMatrixUniform_TextRender, gProjectionMatrixUniform_TextRender;
GLuint gTextSamplerUniform_TextRender;
GLuint fadeinFactorUniform_TextRender, fadeoutFactorUniform_TextRender;
GLuint samplerUniform_TextRender, iTimeUniform_TextRender, glyph_colorUniform_TextRender;
GLuint applyBloomUniform_TextRender, u_bloom_is_activeUniform_TextRender;
GLuint bloom_thresh_minUniform_TextRender, bloom_thresh_maxUniform_TextRender;

GLuint gVao_Text_TextRender, gVbo_Text_TextRender;

std::map<GLchar, Character> Characters_TextRender;

//code
void initTextRendering()
{
	gVertexShaderObject_TextRender = glCreateShader(GL_VERTEX_SHADER);
	const GLchar* vertextShaderSourceCode_TextRender =
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

	glShaderSource(gVertexShaderObject_TextRender, 1, (const GLchar**)&vertextShaderSourceCode_TextRender, NULL);
	glCompileShader(gVertexShaderObject_TextRender);
	checkCompilationLog((char*)"gVertexShaderObject_TextRender", gVertexShaderObject_TextRender);

	// ---------------------------------------------------------------------------------------------------------------------------------

	gFragmentShaderObject_TextRender = glCreateShader(GL_FRAGMENT_SHADER);
	const GLchar* fragmentShaderSourceCode_TextRender =
		"#version 450" \
		"\n" \
		"uniform float iTime;" \

		"in vec2 TexCoords;" \
		
		"layout (location = 0) out vec4 FragColor;" \
		"layout (location = 1) out vec4 BloomColor;" \
		"layout (location = 2) out vec4 GodRaysColor;" \

		"uniform int applyBloom;" \
		"uniform float bloom_thresh_min = 0.8f;" \
		"uniform float bloom_thresh_max = 1.2f;" \
		"uniform int u_bloom_is_active;" \

		"uniform sampler2D text;" \
		/*"uniform sampler2D u_sampler;" \*/

		"uniform float fadeinFactor;" \
		"uniform float fadeoutFactor;" \

		"uniform vec3 glyph_color			= vec3(0.3, 1.0, 0.04);" \
		"const float glyph_center	= 0.5;" \
		"vec3 glow_color			= vec3(0.3, 1.0, 0.04);" \
		"const float glow_center	= 0.5;" \

		"void main(void)" \
		"{" \
		/*"vec4 fluid_tex = texture(u_sampler,TexCoords.xy);" \*/
		"vec4 color		= texture2D(text, TexCoords.xy);" \
		"float dist		= color.r;" \
		"float width	= fwidth(dist);" \
		"float alpha	= smoothstep(glyph_center - width, glyph_center+width, dist);" \

		"vec2 uv = TexCoords.xy;" \
		"uv.y = uv.y + 0.1 * sin(iTime + 2.0 * uv.x);" \

		"vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, uv).r);" \
		/*"FragColor = vec4(1.0, 1.0,1.0, 1.0) * sampled;" \*/
		"vec3 rgb		= mix(glyph_color, glyph_color, alpha);" \
		"float mu		= smoothstep(glyph_center, glow_center, sqrt(dist));" \
		/*"FragColor		= vec4(rgb, max(alpha, mu)) * sampled;" \*/
		"FragColor		= vec4(rgb, max(alpha, mu));" \


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

		/*"FragColor		= FragColor * fluid_tex * fadeinFactor * fadeoutFactor;" \*/
		"}";

	glShaderSource(gFragmentShaderObject_TextRender, 1, (const GLchar**)&fragmentShaderSourceCode_TextRender, NULL);
	glCompileShader(gFragmentShaderObject_TextRender);
	checkCompilationLog((char*)"gFragmentShaderObject_TextRender", gFragmentShaderObject_TextRender);

	// ---------------------------------------------------------------------------------------------------------------------------------

	gShaderProgramObject_TextRender = glCreateProgram();

	glAttachShader(gShaderProgramObject_TextRender, gVertexShaderObject_TextRender);
	glAttachShader(gShaderProgramObject_TextRender, gFragmentShaderObject_TextRender);
	glBindAttribLocation(gShaderProgramObject_TextRender, MATRIX_ATTRIBUTE_POSITION, "vPosition");
	glLinkProgram(gShaderProgramObject_TextRender);

	checkLinkLog((char*)"gShaderProgramObject_TextRender", gShaderProgramObject_TextRender);

	gModelMatrixUniform_TextRender = glGetUniformLocation(gShaderProgramObject_TextRender, "u_model_matrix");
	gViewMatrixUniform_TextRender = glGetUniformLocation(gShaderProgramObject_TextRender, "u_view_matrix");
	gProjectionMatrixUniform_TextRender = glGetUniformLocation(gShaderProgramObject_TextRender, "u_projection_matrix");

	gTextSamplerUniform_TextRender = glGetUniformLocation(gShaderProgramObject_TextRender, "text");
	samplerUniform_TextRender = glGetUniformLocation(gShaderProgramObject_TextRender, "u_sampler");

	glyph_colorUniform_TextRender = glGetUniformLocation(gShaderProgramObject_TextRender, "glyph_color");

	fadeinFactorUniform_TextRender = glGetUniformLocation(gShaderProgramObject_TextRender, "fadeinFactor");
	fadeoutFactorUniform_TextRender = glGetUniformLocation(gShaderProgramObject_TextRender, "fadeoutFactor");
	iTimeUniform_TextRender = glGetUniformLocation(gShaderProgramObject_TextRender, "iTime");

	applyBloomUniform_TextRender = glGetUniformLocation(gShaderProgramObject_TextRender, "applyBloom");
	u_bloom_is_activeUniform_TextRender = glGetUniformLocation(gShaderProgramObject_TextRender, "u_bloom_is_active");
	bloom_thresh_minUniform_TextRender = glGetUniformLocation(gShaderProgramObject_TextRender, "bloom_thresh_min");
	bloom_thresh_maxUniform_TextRender = glGetUniformLocation(gShaderProgramObject_TextRender, "bloom_thresh_max");


	glGenVertexArrays(1, &gVao_Text_TextRender);
	glBindVertexArray(gVao_Text_TextRender);

	glGenBuffers(1, &gVbo_Text_TextRender);
	glBindBuffer(GL_ARRAY_BUFFER, gVbo_Text_TextRender);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(MATRIX_ATTRIBUTE_POSITION, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(MATRIX_ATTRIBUTE_POSITION);
	glBindBuffer(GL_ARRAY_BUFFER, 0);


	glBindVertexArray(0);



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	FT_Library ft;
	if (FT_Init_FreeType(&ft))
	{
		fopen_s(&gpFile, "Log.txt", "a+");
		fprintf(gpFile, "ERROR : FREETYPE : Could Not Init freetype library\n");
		fclose(gpFile);
	}
	FT_Face face;

	FT_Error error = FT_New_Face(ft, "C:/Windows/Fonts/arial.ttf", 0, &face);

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
			(GLuint)face->glyph->advance.x
		};
		Characters_TextRender.insert(std::pair<GLchar, Character>(c, character));

	}
	glBindTexture(GL_TEXTURE_2D, 0);
	FT_Done_Face(face);
	FT_Done_FreeType(ft);
}

void DrawingLoop(std::string text, float x, float y)
{
	float scale = 0.1f;
	std::string::const_iterator c;
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
		glUniform1i(gTextSamplerUniform_TextRender, 0);

		glBindBuffer(GL_ARRAY_BUFFER, gVbo_Text_TextRender);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glDrawArrays(GL_TRIANGLES, 0, 6);

		x += (ch.Advance >> 6) * scale;

	}

}
float count_first = 0.0f;

void display_textRendering_FirstPage()
{
	count_first += 0.01f;
	std::string text;
	static float yTranslateLowerMove = 10.0f;
	static float yTranslateUpperMove = -10.0f;
	static float zTranslateMove = -50.0f;
	static float xTranslateMove = -30.0f;
	
	
	if (zTranslateMove <= -40.0f)
	{
		zTranslateMove += 0.05f;
	}
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glViewport(0, 0, (GLsizei)gWidth, (GLsizei)gHeight);

	glUseProgram(gShaderProgramObject_TextRender);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glUniform1f(fadeoutFactorUniform_TextRender, 1.0f);
	glUniform1f(fadeinFactorUniform_TextRender, 1.0f);

	glUniform1i(u_bloom_is_activeUniform_TextRender, 0);
	glUniform1f(bloom_thresh_minUniform_TextRender, 0.2);
	glUniform1f(bloom_thresh_maxUniform_TextRender, 0.4);
	glUniform1i(applyBloomUniform_TextRender, 1);

	mat4 modelMatrix = mat4::identity();
	mat4 viewMatrix = mat4::identity();
	mat4 scaleMatrix = mat4::identity();
	mat4 rotationMatrix = mat4::identity();

	text = "A`so^aemaeDIka`mpa ";
	
	glUniformMatrix4fv(gViewMatrixUniform_TextRender, 1, GL_FALSE, viewMatrix);
	glUniformMatrix4fv(gProjectionMatrixUniform_TextRender, 1, GL_FALSE, gPerspectiveProjectionMatrix);

	
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(gVao_Text_TextRender);
	float x;
	float y;
	if (count_first < 3.0f)
	{
		modelMatrix = translate(-20.0f, 0.0f, zTranslateMove);

		glUniformMatrix4fv(gModelMatrixUniform_TextRender, 1, GL_FALSE, modelMatrix);

		x = 3.5f;
		y = 3.5f;

		DrawingLoop(text, x, y);

		text = "pXastauta";
		x = 14.5f;
		y = -8.5f;

		DrawingLoop(text, x, y);
	}
	else if (count_first < 9.0f)
	{
		if (yTranslateLowerMove >= 0.0f)
		{
			yTranslateLowerMove -= 0.05f;
		}
		modelMatrix = translate(-20.0f, yTranslateLowerMove, -40.0f);

		glUniformMatrix4fv(gModelMatrixUniform_TextRender, 1, GL_FALSE, modelMatrix);
		text = "m`aio^|sa  gao";
		x = 0.5f;
		y = 3.5f;

		DrawingLoop(text, x, y);

		if (count_first > 6.0f)
		{
			if (yTranslateUpperMove <= 0.0f)
			{
				yTranslateUpperMove += 0.05f;
			}
			modelMatrix = translate(-20.0f, yTranslateUpperMove, -40.0f);
			glUniformMatrix4fv(gModelMatrixUniform_TextRender, 1, GL_FALSE, modelMatrix);
			text = "saadr krIta Aahe";
			x = 3.5f;
			y = -5.5f;

			DrawingLoop(text, x, y);
		}
		
	}
	else if (count_first < 16.0f)
	{
		if (count_first > 10.0f)
		{
			text = "iSaSaupaala";
			x = 3.5f;
			y = 0.5f;

			DrawingLoop(text, x, y);
		}
		if (count_first > 12.0f)
		{
			glUniform3fv(glyph_colorUniform_TextRender, 1, vec3(1.0f, 0.1f, 0.0f));
			text = "vaYa";
			x = 22.5f;
			y = 0.5f;

			DrawingLoop(text, x, y);
		}
	}
	glDisable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glUseProgram(0);

	if (count_first >= 16.0f)
	{
		isStartTitle = false;
		gpIXAudio2_SceneFirstSourceVoice->Start(0);
		startAnimation = true;
	}
}


GLuint gVertexShaderObject_end_reference, gFragmentShaderObject_end_reference, gShaderProgramObject_end_reference;
GLuint gModelMatrixUniform_end_reference;
GLuint gViewMatrixUniform_end_reference;
GLuint gProjectionMatrixUniform_end_reference;

GLuint samplerUniform1_end_reference;

GLuint texture1_end_reference;
GLuint texture2_end_reference;
GLuint fadeinFactorUniform_end_reference, fadeoutFactorUniform_end_reference;

void init_EndReference()
{
#pragma region MORPIS
	//VERTEX SHADER
	gVertexShaderObject_end_reference = glCreateShader(GL_VERTEX_SHADER);
	const GLchar* vertextShaderSourceCode_end_reference =
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

	glShaderSource(gVertexShaderObject_end_reference, 1, (const GLchar**)&vertextShaderSourceCode_end_reference, NULL);
	glCompileShader(gVertexShaderObject_end_reference);
	checkCompilationLog((char*)"gVertexShaderObject_end_reference", gVertexShaderObject_end_reference);


	//FRAGMENT SHADER

	gFragmentShaderObject_end_reference = glCreateShader(GL_FRAGMENT_SHADER);
	const GLchar* fragmentShaderSourceCode_end_reference =
		"#version 450 core" \
		"\n" \
		"in vec2 out_texcoord;" \
		"layout (location = 0) out vec4 FragColor;" \
		"layout (location = 1) out vec4 BloomColor;" \
		"layout (location = 2) out vec4 GodRaysColor; " \

		"uniform sampler2D u_sampler1;" \
		
		"uniform int applyBloom;" \
		"uniform float bloom_thresh_min = 0.8f;" \
		"uniform float bloom_thresh_max = 1.2f;" \
		"uniform int u_bloom_is_active;" \

		"uniform float fadeinFactor;" \
		"uniform float fadeoutFactor;" \

		"void main(void)" \
		"{" \
		"vec4 FragColor1 = texture(u_sampler1,out_texcoord);" \
		"FragColor = FragColor1 * fadeinFactor * fadeoutFactor;"
		"BloomColor = vec4(0.0f);" \
		"GodRaysColor = vec4(0.0f);" \
		"}";

	glShaderSource(gFragmentShaderObject_end_reference, 1, (const GLchar**)&fragmentShaderSourceCode_end_reference, NULL);
	glCompileShader(gFragmentShaderObject_end_reference);
	checkCompilationLog((char*)"gFragmentShaderObject_end_reference", gFragmentShaderObject_end_reference);


	//Shader Program
	gShaderProgramObject_end_reference = glCreateProgram();

	glAttachShader(gShaderProgramObject_end_reference, gVertexShaderObject_end_reference);
	glAttachShader(gShaderProgramObject_end_reference, gFragmentShaderObject_end_reference);

	glBindAttribLocation(gShaderProgramObject_end_reference, MATRIX_ATTRIBUTE_POSITION, "vPosition");
	glBindAttribLocation(gShaderProgramObject_end_reference, MATRIX_ATTRIBUTE_TEXTURE0, "vTexcoord");

	glLinkProgram(gShaderProgramObject_end_reference);

	// Error checking
	checkLinkLog((char*)"gShaderProgramObject_end_reference", gShaderProgramObject_end_reference);


	gModelMatrixUniform_end_reference = glGetUniformLocation(gShaderProgramObject_end_reference, "u_model_matrix");
	gViewMatrixUniform_end_reference = glGetUniformLocation(gShaderProgramObject_end_reference, "u_view_matrix");
	gProjectionMatrixUniform_end_reference = glGetUniformLocation(gShaderProgramObject_end_reference, "u_projection_matrix");

	samplerUniform1_end_reference = glGetUniformLocation(gShaderProgramObject_end_reference, "u_sampler1");

	fadeinFactorUniform_end_reference = glGetUniformLocation(gShaderProgramObject_end_reference, "fadeinFactor");
	fadeoutFactorUniform_end_reference = glGetUniformLocation(gShaderProgramObject_end_reference, "fadeoutFactor");

	glEnable(GL_TEXTURE_2D);

	loadTexture_mor_pis(&texture1_end_reference, MAKEINTRESOURCE(114));
	loadTexture_mor_pis(&texture2_end_reference, MAKEINTRESOURCE(115));
}


void drawEndReference()
{
	static float count = 0.0f;
	static float FadeOutFactor_end_reference = 1.0f;
	mat4 modelMatrix = mat4::identity();
	mat4 ViewMatrix = mat4::identity();
	mat4 scaleMatrix = mat4::identity();
	mat4 rotateMatrix = mat4::identity();

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(gShaderProgramObject_end_reference);

	modelMatrix = mat4::identity();
	scaleMatrix = mat4::identity();
	rotateMatrix = mat4::identity();

	modelMatrix = translate(0.0f, 0.0f, -3.0f);
	
	glUniform1f(fadeoutFactorUniform_end_reference, FadeOutFactor_end_reference);
	glUniform1f(fadeinFactorUniform_end_reference, 1.0f);

	glUniformMatrix4fv(gModelMatrixUniform_end_reference, 1, GL_FALSE, modelMatrix);
	glUniformMatrix4fv(gViewMatrixUniform_end_reference, 1, GL_FALSE, ViewMatrix);								    // globally camera set in perFrag file
	glUniformMatrix4fv(gProjectionMatrixUniform_end_reference, 1, GL_FALSE, gPerspectiveProjectionMatrix);			// globally pojection set

	count = count + 0.01f;
	if (count < 5.0f)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1_end_reference);
		glUniform1i(samplerUniform1_end_reference, 0);
	}
	else if(count >= 5.5f )
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture2_end_reference);
		glUniform1i(samplerUniform1_end_reference, 0);
		if (count >= 7.5f)
		{
			FadeOutFactor_end_reference -= 0.01f;
			if (FadeOutFactor_end_reference <= 0.0f)
				FadeOutFactor_end_reference = 0.0f;
		}
	}
	
	// bind with vao
	drawQuadShape();

	glUseProgram(0);

}