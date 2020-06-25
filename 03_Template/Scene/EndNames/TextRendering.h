#include "../../Include/freetype/include/ft2build.h"
#include FT_FREETYPE_H

GLuint gVertexShaderObject_TextRender, gFragmentShaderObject_TextRender, gShaderProgramObject_TextRender;

GLuint gModelMatrixUniform_TextRender, gViewMatrixUniform_TextRender, gProjectionMatrixUniform_TextRender;
GLuint gTextSamplerUniform_TextRender;
GLuint fadeinFactorUniform_TextRender, fadeoutFactorUniform_TextRender;
GLuint samplerUniform_TextRender, iTimeUniform_TextRender;

GLuint gVao_Text_TextRender, gVbo_Text_TextRender;


std::map<GLchar, Character> Characters_TextRender;
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
		"out vec4 FragColor;" \

		"uniform sampler2D text;" \
		/*"uniform sampler2D u_sampler;" \*/

		"uniform float fadeinFactor;" \
		"uniform float fadeoutFactor;" \

		"vec3 glyph_color			= vec3(1.0, 1.0, 1.0);" \
		"const float glyph_center	= 0.5;" \
		"vec3 glow_color			= vec3(1.0, 1.0, 1.0);" \
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
		"vec3 rgb		= mix(glow_color, glyph_color, alpha);" \
		"float mu		= smoothstep(glyph_center, glow_center, sqrt(dist));" \
		/*"FragColor		= vec4(rgb, max(alpha, mu)) * sampled;" \*/
		"FragColor		= vec4(rgb, max(alpha, mu));" \

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

	fadeinFactorUniform_TextRender = glGetUniformLocation(gShaderProgramObject_TextRender, "fadeinFactor");
	fadeoutFactorUniform_TextRender = glGetUniformLocation(gShaderProgramObject_TextRender, "fadeoutFactor");
	iTimeUniform_TextRender = glGetUniformLocation(gShaderProgramObject_TextRender, "iTime");




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

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glViewport(0, 0, (GLsizei)gWidth, (GLsizei)gHeight);

	glUseProgram(gShaderProgramObject_TextRender);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glUniform1f(fadeoutFactorUniform_TextRender, 1.0f);
	glUniform1f(fadeinFactorUniform_TextRender, 1.0f);

	//glUniform1f(iTimeUniform_TextRender, time_fluid_Text);

	mat4 modelMatrix = mat4::identity();
	mat4 viewMatrix = mat4::identity();
	mat4 scaleMatrix = mat4::identity();
	mat4 rotationMatrix = mat4::identity();

	text = "A`so^aemaeDIka`mpa ";
	modelMatrix = translate(-20.0f, -0.0f, -40.0f);


	glUniformMatrix4fv(gModelMatrixUniform_TextRender, 1, GL_FALSE, modelMatrix);
	glUniformMatrix4fv(gViewMatrixUniform_TextRender, 1, GL_FALSE, viewMatrix);
	glUniformMatrix4fv(gProjectionMatrixUniform_TextRender, 1, GL_FALSE, gPerspectiveProjectionMatrix);

	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(gVao_Text_TextRender);
	
	float x = 3.5f;
	float y = 3.5f;

	DrawingLoop(text, x, y);

	text = "pXastauta";
	x = 14.5f;
	y = -8.5f;

	DrawingLoop(text, x, y);
	
	glDisable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glUseProgram(0);
	if (count_first >= 9.0f)
	{
		isStartTitle = false;
		gpIXAudio2_SceneFirstSourceVoice->Start(0);
		startAnimation = true;
	}
}


float count = 0.0f;
void display_textRendering_SecondLastPage()
{
	std::string text;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glViewport(0, 0, (GLsizei)gWidth, (GLsizei)gHeight);

	glUseProgram(gShaderProgramObject_TextRender);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glUniform1f(fadeoutFactorUniform_TextRender, 1.0f);
	glUniform1f(fadeinFactorUniform_TextRender, 1.0f);

	//glUniform1f(iTimeUniform_TextRender, time_fluid_Text);

	mat4 modelMatrix = mat4::identity();
	mat4 viewMatrix = mat4::identity();
	mat4 scaleMatrix = mat4::identity();
	mat4 rotationMatrix = mat4::identity();

	modelMatrix = translate(-17.0f, -0.0f, -40.0f);

	glUniformMatrix4fv(gModelMatrixUniform_TextRender, 1, GL_FALSE, modelMatrix);
	glUniformMatrix4fv(gViewMatrixUniform_TextRender, 1, GL_FALSE, viewMatrix);
	glUniformMatrix4fv(gProjectionMatrixUniform_TextRender, 1, GL_FALSE, gPerspectiveProjectionMatrix);

	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(gVao_Text_TextRender);
	float x;
	float y;
	if (count <= 1.0f)
	{
		text = "ivaSaeVa AaBaar";
		x = 1.0f;
		y = 0.0f;
		DrawingLoop(text, x, y);
	}
	if ((count >= 1.0f) && (count <= 7.0f))
	{
		if ((count >= 2.0f))
		{
			text = "yaaegaeSvar sar";
			x = -5.5f;
			y = 4.5f;

			DrawingLoop(text, x, y);
		}
		if (count >= 4.0f)
		{
			text = "AaiNa raiYaka ma`Dma";
			x = -2.5f;
			y = -8.5f;

			DrawingLoop(text, x, y);
		}
	}
	if ((count >= 7.0f) && (count <= 13.0f))
	{
		if (count >= 8.0f)
		{
			text = "ihmaania caaEYari";
			x = -5.5f;
			y = 5.5f;

			DrawingLoop(text, x, y);
		}
		if (count >= 10.0f)
		{
			text = "RVia maaekaSia";
			x = 3.5f;
			y = -8.5f;

			DrawingLoop(text, x, y);
		}
	}
	if ((count >= 13.0f))
	{
		if (count >= 14.0f)
		{
			text = "Da`. ivajaya gaaeKalae";
			x = -5.5f;
			y = -0.5f;

			DrawingLoop(text, x, y);
		}
		if (count >= 16.0f)
		{
			text = "rmaa gaaeKalae";
			x = 2.5f;
			y = -9.5f;

			DrawingLoop(text, x, y);
		}
	}
	count += 0.01f;
	

	glDisable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glUseProgram(0);
}