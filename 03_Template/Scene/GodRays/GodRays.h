
GLuint gVertexShaderObject_color;
GLuint gFragmentShaderObject_color;
GLuint gShaderProgramObject_color;

GLuint gModelMatrixUniform_Color, gViewMatrixUniform_Color, gProjMatrixUniform_Color, gColorUniform_Color;

void InitColorProgramShaders()
{
	//VERTEX SHADER
	gVertexShaderObject_color = glCreateShader(GL_VERTEX_SHADER);

	const GLchar* vertextShaderSourceCode =
		"#version 460" \
		"\n" \
		"in vec3 vPosition;" \
	
		"uniform mat4 u_model_matrix;" \
		"uniform mat4 u_view_matrix;" \
		"uniform mat4 u_projection_matrix;" \
		"void main(void)" \
		"{" \
		"gl_Position = u_projection_matrix * u_view_matrix * u_model_matrix * vec4(vPosition, 1.0f);" \
		"}";

	glShaderSource(gVertexShaderObject_color, 1, (const GLchar**)&vertextShaderSourceCode, NULL);

	//compile shader
	glCompileShader(gVertexShaderObject_color);

	// Error Checking
	checkCompilationLog("gVertexShaderObject_color", gVertexShaderObject_color);

	//FRAGMENT SHADER
	gFragmentShaderObject_color = glCreateShader(GL_FRAGMENT_SHADER);

	const GLchar* fragmentShaderSourceCode =
		"#version 460" \
		"\n" \
		"uniform vec3 uColor;" \
		"layout (location = 0) out vec4 FragColor;" \
		"layout (location = 1) out vec4 BloomColor;" \
		"layout (location = 2) out vec4 GodRaysColor;" \

		"void main(void)" \
		"{" \
		"FragColor = vec4(uColor, 1.0);" \
		"BloomColor = vec4(0.0);" \
		"GodRaysColor = FragColor;" \
		"}";

	glShaderSource(gFragmentShaderObject_color, 1, (const GLchar**)&fragmentShaderSourceCode, NULL);

	glCompileShader(gFragmentShaderObject_color);

	// Error Checking
	checkCompilationLog("gFragmentShaderObject_color", gFragmentShaderObject_color);

	//Shader Program

	gShaderProgramObject_color = glCreateProgram();

	glAttachShader(gShaderProgramObject_color, gVertexShaderObject_color);
	glAttachShader(gShaderProgramObject_color, gFragmentShaderObject_color);

	glBindAttribLocation(gShaderProgramObject_color, MATRIX_ATTRIBUTE_POSITION, "vPosition");

	glLinkProgram(gShaderProgramObject_color);

	// Link Log
	checkLinkLog("gShaderProgramObject_color", gShaderProgramObject_color);

	gModelMatrixUniform_Color = glGetUniformLocation(gShaderProgramObject_color, "u_model_matrix");
	gViewMatrixUniform_Color = glGetUniformLocation(gShaderProgramObject_color, "u_view_matrix");
	gProjMatrixUniform_Color = glGetUniformLocation(gShaderProgramObject_color, "u_projection_matrix");
	gColorUniform_Color = glGetUniformLocation(gShaderProgramObject_color, "uColor");

}

vec3 SourceDir;

void renderBrightSource()
{
	mat4 modelMatrix = mat4::identity();
	mat4 scaleMatrix = mat4::identity();
	
	if (startJoin_krishnaAnimate)
	{
		gfKrishnaModelScale = 300.0f;
		SourceDir = vec3(-450.0f, 955.0f, 1.0f);
	}
	else
	{
		gfKrishnaModelScale = 100.0f;
		SourceDir = vec3(-400.0f, 255.0f, 1.0f);
	}

	glUseProgram(gShaderProgramObject_color);

	modelMatrix = vmath::translate(SourceDir);
	scaleMatrix = vmath::scale(gfKrishnaModelScale, gfKrishnaModelScale, gfKrishnaModelScale);
	modelMatrix = modelMatrix * scaleMatrix;

	glUniformMatrix4fv(gModelMatrixUniform_Color, 1, GL_FALSE, modelMatrix);
	glUniformMatrix4fv(gViewMatrixUniform_Color, 1, GL_FALSE, gViewMatrix);
	glUniformMatrix4fv(gProjMatrixUniform_Color, 1, GL_FALSE, gPerspectiveProjectionMatrix);
	glUniform3fv(gColorUniform_Color, 1, vec3(0.98, 0.83, 0.25));

	drawSphereShape();

	glUseProgram(0);
}

vec4 vec4Transform(vec4& vSrcVector, mat4& matSrcMatrix)
{
	vec4 vDstVector;
	vDstVector[0] = matSrcMatrix[0][0] * vSrcVector[0] +
		matSrcMatrix[1][0] * vSrcVector[1] +
		matSrcMatrix[2][0] * vSrcVector[2] +
		matSrcMatrix[3][0] * vSrcVector[3];
	vDstVector[1] = matSrcMatrix[0][1] * vSrcVector[0] +
		matSrcMatrix[1][1] * vSrcVector[1] +
		matSrcMatrix[2][1] * vSrcVector[2] +
		matSrcMatrix[3][1] * vSrcVector[3];
	vDstVector[2] = matSrcMatrix[0][2] * vSrcVector[0] +
		matSrcMatrix[1][2] * vSrcVector[1] +
		matSrcMatrix[2][2] * vSrcVector[2] +
		matSrcMatrix[3][2] * vSrcVector[3];
	vDstVector[3] = matSrcMatrix[0][3] * vSrcVector[0] +
		matSrcMatrix[1][3] * vSrcVector[1] +
		matSrcMatrix[2][3] * vSrcVector[2] +
		matSrcMatrix[3][3] * vSrcVector[3];
	return vDstVector;
}

vec2 getScreenSpaceSunPos()
{
	mat4 model = mat4::identity();
	model = translate(0.0f, 0.0f, 0.0f);
	if (startJoin_krishnaAnimate)
	{
		gfKrishnaModelScale = 300.0f;
		SourceDir = vec3(-450.0f, 955.0f, 1.0f);
	}
	else
	{
		gfKrishnaModelScale = 100.0f;
		SourceDir = vec3(-400.0f, 255.0f, 1.0f);
	}

	mat4 scaleMatrix = vmath::scale(gfKrishnaModelScale, gfKrishnaModelScale, gfKrishnaModelScale);

	mat4 projection = gPerspectiveProjectionMatrix;

	vec4 v = vec4Transform(vec4(SourceDir, 1.0f), model);
	//v = vec4Transform(v, scaleMatrix);
	v = vec4Transform(v, gViewMatrix);

	v = vec4Transform(v, projection);

	v = vec4(v[0] / v[3], v[1] / v[3], v[2] / v[3], v[3] / v[3]);


	v = v + vec4(1.0, 1.0, 0.0, 0.0);

	v = vec4(v[0] * 0.5, v[1] * 0.5, v[2] * 0.5, v[3] * 0.5);

	return (vec2(v[2], v[0]));

}

GLuint gVertexShaderObject_godRays;
GLuint gFragmentShaderObject_godRays;
GLuint gShaderProgramObject_godRays;

GLuint gScreenSpaceSunPosUniform_godRays, gDensityUniform_godRays, gWeightUniform_godRays, gDecayUniform_godRays,
		gExposureUniform_godRays, gNumSamplesUniform_godRays, gOcclusionTextureUniform_godRays;
GLuint gModelMatrixUniform_godRays, gViewMatrixUniform_godRays, gProjectionMatrixUniform_godRays;

void initGodRaysPostProcessing()
{

	//VERTEX SHADER
	gVertexShaderObject_godRays = glCreateShader(GL_VERTEX_SHADER);

	const GLchar* vertextShaderSourceCode =
		"#version 460" \
		"\n" \
		"in vec2 vPosition;" \
		"out vec2 vUv;" \

		"void main(void)" \
		"{" \
		"vUv = 0.5 * (vPosition + 1.0);" \
		"gl_Position = vec4(vPosition.xy, 0.0, 1.0);" \
		"}";

	glShaderSource(gVertexShaderObject_godRays, 1, (const GLchar**)&vertextShaderSourceCode, NULL);

	//compile shader
	glCompileShader(gVertexShaderObject_godRays);

	// Error Checking
	checkCompilationLog("gVertexShaderObject_godRays", gVertexShaderObject_godRays);
	
	
	//FRAGMENT SHADER

	gFragmentShaderObject_godRays = glCreateShader(GL_FRAGMENT_SHADER);

	const GLchar* fragmentShaderSourceCode =
		"#version 460" \
		"\n" \
		"layout (binding = 0) uniform sampler2D uOcclusionTexture;" \
		"in vec2 vUv;" \

		"uniform vec2 uScreenSpaceSunPos;" \
		"uniform float uDensity;" \
		"uniform float uWeight;" \
		"uniform float uDecay;" \
		"uniform float uExposure;" \
		"uniform int uNumSamples;" \

		"out vec4 FragColor;" \

		"void main(void)" \
		"{" \
		"vec3 fragColor = vec3(0.0, 0.0, 0.0);" \
		"vec2 deltaTexCoord = vec2(vUv - uScreenSpaceSunPos.xy);" \

		"vec2 texCoo = vUv.xy;" \
		"deltaTexCoord *= (1.0 / float(uNumSamples)) * uDensity;" \
		"float illuminationDecay = 1.0;" \

		"for(int i = 0; i < uNumSamples; i++)" \
		"{" \
		"texCoo -= deltaTexCoord;" \
		"vec3 samp = texture2D(uOcclusionTexture, texCoo).xyz;" \
		"samp *= illuminationDecay * uWeight;" \
		"fragColor += samp;" \
		"illuminationDecay *= uDecay;" \
		"}" \

		"fragColor *= uExposure;" \
		"FragColor = vec4(fragColor, 1.0) * vec4(0.98, 0.83f, 0.25, 1.0f);" \
		"}";

	glShaderSource(gFragmentShaderObject_godRays, 1, (const GLchar**)&fragmentShaderSourceCode, NULL);

	glCompileShader(gFragmentShaderObject_godRays);

	// Error Checking
	checkCompilationLog("gFragmentShaderObject_godRays", gFragmentShaderObject_godRays);

	//Shader Program

	gShaderProgramObject_godRays = glCreateProgram();

	glAttachShader(gShaderProgramObject_godRays, gVertexShaderObject_godRays);
	glAttachShader(gShaderProgramObject_godRays, gFragmentShaderObject_godRays);

	glBindAttribLocation(gShaderProgramObject_godRays, MATRIX_ATTRIBUTE_POSITION, "vPosition");

	glLinkProgram(gShaderProgramObject_godRays);

	// Link Log
	checkLinkLog("gShaderProgramObject_godRays", gShaderProgramObject_godRays);

	gScreenSpaceSunPosUniform_godRays = glGetUniformLocation(gShaderProgramObject_godRays, "uScreenSpaceSunPos");
	gDensityUniform_godRays = glGetUniformLocation(gShaderProgramObject_godRays, "uDensity");
	gWeightUniform_godRays = glGetUniformLocation(gShaderProgramObject_godRays, "uWeight");
	gDecayUniform_godRays = glGetUniformLocation(gShaderProgramObject_godRays, "uDecay");
	gExposureUniform_godRays = glGetUniformLocation(gShaderProgramObject_godRays, "uExposure");
	gNumSamplesUniform_godRays = glGetUniformLocation(gShaderProgramObject_godRays, "uNumSamples");
	gOcclusionTextureUniform_godRays = glGetUniformLocation(gShaderProgramObject_godRays, "uOcclusionTexture");
	
}


void display_GodRaysPostProcessing()
{
	glBindFramebuffer(GL_FRAMEBUFFER, godRays_fbo);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, MAX_SCENE_WIDTH, MAX_SCENE_HEIGHT);

	glUseProgram(gShaderProgramObject_godRays);

	vec2 screenPos = getScreenSpaceSunPos();
	glUniform2fv(gScreenSpaceSunPosUniform_godRays, 1, vec2(screenPos[0], screenPos[1]));
	glUniform1f(gDecayUniform_godRays, 1.0f);
	glUniform1f(gWeightUniform_godRays, 0.1f);
	glUniform1f(gDensityUniform_godRays, 1.0f);
	glUniform1f(gExposureUniform_godRays, 1.0);
	glUniform1i(gNumSamplesUniform_godRays, 100);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texGodRaysPass);
	glUniform1i(gOcclusionTextureUniform_godRays, 0);

	drawQuadShape();

	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}