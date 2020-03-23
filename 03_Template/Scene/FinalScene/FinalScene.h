
GLuint ShaderProgramObject_Final_Rendering, inputSamplerone_Final_Rendering, inputSamplertwo_Final_Rendering;

void initFinalShaderProgramObject(void) {
	// variable declarations
	GLuint finalAdditiveVertexShaderObject;
	GLuint finalAdditiveFragmentShaderObject;
	// code
	// **** VERTEX SHADER ***
	finalAdditiveVertexShaderObject = glCreateShader(GL_VERTEX_SHADER);
	// Shader source code
	const GLchar* finalAdditiveVertexShaderSourceCode =
		"#version 450 core" \
		"\n" \
		"in vec2 vPosition;" \
		"in vec2 vTexcoord;" \
		"out vec2 outTexcoord;" \
		"void main(void) {" \
		"	const vec4 vertices[] = vec4[](vec4(-1.0, -1.0, 0.5, 1.0),									\n" \
		"									vec4(1.0, -1.0, 0.5, 1.0),									\n" \
		"									vec4(-1.0, 1.0, 0.5, 1.0),									\n" \
		"									vec4(1.0, 1.0, 0.5, 1.0));									\n" \
		/*"gl_Position = vertices[gl_VertexID];\n" \*/
		"outTexcoord = vTexcoord;" \
		"gl_Position = vec4(vPosition.xy, 0.0, 1.0);" \
		"}";
	// Shader source
	glShaderSource(finalAdditiveVertexShaderObject, 1, (const GLchar**)&finalAdditiveVertexShaderSourceCode, NULL);
	// Compile Shader
	glCompileShader(finalAdditiveVertexShaderObject);
	// Error checking
	checkCompilationLog("finalAdditiveVertexShaderObject", finalAdditiveVertexShaderObject);
	// *** FRAGMENT SHADER OBJECT ***
	finalAdditiveFragmentShaderObject = glCreateShader(GL_FRAGMENT_SHADER);
	// Shader source code
	const GLchar* finalAdditiveFragmentShaderSourceCode =
		"#version 450 core" \
		"\n" \
		"in vec2 outTexcoord;" \
		
		"layout (binding = 0) uniform sampler2D input_one;" \
		"layout (binding = 1) uniform sampler2D input_two;" \
		
		"uniform float exposure = 0.9;" \
		"out vec4 FragColor;" \
		"void main(void) {" \
		
		"vec4 c = vec4(0.0);" \
		
		"c += texelFetch(input_one, ivec2(gl_FragCoord.xy), 0);" \
		"c += texelFetch(input_two, ivec2(gl_FragCoord.xy), 0);" \
		"vec3 result = texture(input_one, outTexcoord).rgb;" \
		"result += texture(input_two, outTexcoord).rgb;" \
		
		"FragColor = vec4(result, 1.0);" \
		
		"}";
	// Shader source
	glShaderSource(finalAdditiveFragmentShaderObject, 1, (const GLchar**)&finalAdditiveFragmentShaderSourceCode, NULL);
	// Compile Shader
	glCompileShader(finalAdditiveFragmentShaderObject);
	// Error checking
	checkCompilationLog("finalAdditiveFragmentShaderObject", finalAdditiveFragmentShaderObject);
	// *** SHADER PROGRAM OBJECT ***
	ShaderProgramObject_Final_Rendering = glCreateProgram();
	// Attach shaders
	glAttachShader(ShaderProgramObject_Final_Rendering, finalAdditiveVertexShaderObject);
	glAttachShader(ShaderProgramObject_Final_Rendering, finalAdditiveFragmentShaderObject);
	glBindAttribLocation(ShaderProgramObject_Final_Rendering, MATRIX_ATTRIBUTE_POSITION, "vPosition");
	glBindAttribLocation(ShaderProgramObject_Final_Rendering, MATRIX_ATTRIBUTE_TEXTURE0, "vTexcoord");
	// Link Program
	glLinkProgram(ShaderProgramObject_Final_Rendering);
	// Error checking
	checkLinkLog("ShaderProgramObject_Final_Rendering", ShaderProgramObject_Final_Rendering);
	// Post-linking
	inputSamplerone_Final_Rendering = glGetUniformLocation(ShaderProgramObject_Final_Rendering, "input_one");
	inputSamplertwo_Final_Rendering = glGetUniformLocation(ShaderProgramObject_Final_Rendering, "input_two");
}


void display_FinalScene()
{
	glUseProgram(ShaderProgramObject_Final_Rendering);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, framebuffer_for_Bloom_color_attachment);
	glGenerateMipmap(GL_TEXTURE_2D);
	glUniform1i(inputSamplerone_Final_Rendering, 0);
	
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, framebuffer_for_All_color_attachment);
	glGenerateMipmap(GL_TEXTURE_2D);
	glUniform1i(inputSamplertwo_Final_Rendering, 1);
	drawQuadShape();
	glUseProgram(0);
}