#pragma once

//function declaration
void initializeBloom(void);
void ApplyingBloom(void);
void uninitializeBloom(void);

//variable declaration
bool bShowBloom_bloom = false;

struct {
	struct {
		GLuint exposure_bloom;
		GLuint bloom_factor;
		GLuint scene_factor;
		GLuint godRays_factor;
		GLuint godRays_image;
	} resolve;
} uniforms;

float bloomFactor = 1.0f;

GLuint render_fbo_bloom;
GLuint gaussian_blur_fbo_bloom[2];

GLuint texLUT_bloom;
GLuint texScene_bloom;
GLuint texBrightPass_bloom;
GLuint texDepth_bloom;
GLuint texGodRaysPass;
GLuint texGaussianBlur_bloom[2];

GLuint vao_framebuffer_bloom;

float exposure_bloom = 1.0f;

GLuint ShaderProgramObject_hdrBloomScene;
GLuint bloomIsActiveUniform_hdrBloomScene;
GLuint mvpUniform_hdrBloomScene;
GLuint bloom_thresh_min_hdrBloomScene;
GLuint bloom_thresh_max_hdrBloomScene;

GLuint ShaderProgramObject_GaussianBlur_Y_Direction;
GLuint ShaderProgramObject_GaussianBlur_X_Direction;
GLuint ShaderProgramObject_Final_Display_Resolution;

void initializeBloom(void) {
	// Shader Objects
	GLuint hdrBloomSceneVertexShaderObject = 0;
	GLuint hdrBloomSceneFragmentShaderObject = 0;
	GLuint hdrBloomFilterVertexShaderObject = 0;
	GLuint hdrBloomFilterFragmentShaderObject = 0;
	GLuint hdrBloomResolveVertexShaderObject = 0;
	GLuint hdrBloomResolveFragmentShaderObject = 0;

	// code
	// ******** HDRBloom Scene Vertex Shader ********
	hdrBloomSceneVertexShaderObject = glCreateShader(GL_VERTEX_SHADER);

	// Shader Source Code
	const GLchar *hdrBloomSceneVertexShaderSourceCode =
	{ 
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
		"}"
	};

	glShaderSource(hdrBloomSceneVertexShaderObject, 1, (const GLchar **)&hdrBloomSceneVertexShaderSourceCode, NULL);
	glCompileShader(hdrBloomSceneVertexShaderObject);

	// Error Checking
	checkCompilationLog("hdrBloomSceneVertexShaderObject", hdrBloomSceneVertexShaderObject);

	// ***** HDRBloom Scene Fragment Shader ******
	hdrBloomSceneFragmentShaderObject = glCreateShader(GL_FRAGMENT_SHADER);

	// Shader Source Code
	const GLchar *hdrBloomSceneFragmentShaderSourceCode =
	{
		"#version 450 core" \
		"\n" \
		"in vec4 out_color;" \
		"uniform int u_bloom_is_active;" \
		"uniform float bloom_thresh_min;" \
		"uniform float bloom_thresh_max;" \
		"layout (location = 0) out vec4 color0;" \
		"layout (location = 1) out vec4 color1;" \
		"void main(void)" \
		"{" \
		"color0 = out_color;"
		"vec4 c = out_color;"
		"	if (u_bloom_is_active == 1) {																\n" \
		"		float Y = dot(out_color, vec4(0.299, 0.587, 0.144, 1.0));										\n" \
		"		c = out_color * 4.0 * smoothstep(bloom_thresh_min, bloom_thresh_max, Y);				\n" \
		"		color1 = vec4(c);																\n" \
		"	}																							\n" \
		"}"
	};

	glShaderSource(hdrBloomSceneFragmentShaderObject, 1, (const GLchar **)&hdrBloomSceneFragmentShaderSourceCode, NULL);
	glCompileShader(hdrBloomSceneFragmentShaderObject);

	// Error checking 
	checkCompilationLog("hdrBloomSceneFragmentShaderObject", hdrBloomSceneFragmentShaderObject);

	// ****** HDRBloom Scene Shader Program Object ******
	ShaderProgramObject_hdrBloomScene = glCreateProgram();

	glAttachShader(ShaderProgramObject_hdrBloomScene, hdrBloomSceneVertexShaderObject);
	glAttachShader(ShaderProgramObject_hdrBloomScene, hdrBloomSceneFragmentShaderObject);

	// Pre-linking
	glBindAttribLocation(ShaderProgramObject_hdrBloomScene, MATRIX_ATTRIBUTE_POSITION, "vPosition");
	glBindAttribLocation(ShaderProgramObject_hdrBloomScene, MATRIX_ATTRIBUTE_COLOR, "vColor");

	// Link Program
	glLinkProgram(ShaderProgramObject_hdrBloomScene);

	// Link Log
	checkLinkLog("ShaderProgramObject_hdrBloomScene", ShaderProgramObject_hdrBloomScene);

	// Post-linking
	bloomIsActiveUniform_hdrBloomScene = glGetUniformLocation(ShaderProgramObject_hdrBloomScene, "u_bloom_is_active");
	mvpUniform_hdrBloomScene = glGetUniformLocation(ShaderProgramObject_hdrBloomScene, "u_mvp_matrix");
	bloom_thresh_min_hdrBloomScene = glGetUniformLocation(ShaderProgramObject_hdrBloomScene, "bloom_thresh_min");
	bloom_thresh_max_hdrBloomScene = glGetUniformLocation(ShaderProgramObject_hdrBloomScene, "bloom_thresh_max");
	// **** HDRBloom Filter Vertex Shader *****
	hdrBloomFilterVertexShaderObject = glCreateShader(GL_VERTEX_SHADER);

	const GLchar *hdrBloomFilterVertexShaderSourceCode =
	{
		"#version 450 core																				\n" \
		"void main(void) {																				\n" \
		"	const vec4 vertices[] = vec4[](vec4(-1.0, -1.0, 0.5, 1.0),									\n" \
		"									vec4(1.0, -1.0, 0.5, 1.0),									\n" \
		"									vec4(-1.0, 1.0, 0.5, 1.0),									\n" \
		"									vec4(1.0, 1.0, 0.5, 1.0));									\n" \
		"	gl_Position = vertices[gl_VertexID];														\n" \
		"}																								\n"
	};

	glShaderSource(hdrBloomFilterVertexShaderObject, 1, (const GLchar **)&hdrBloomFilterVertexShaderSourceCode, NULL);
	glCompileShader(hdrBloomFilterVertexShaderObject);

	// Error Checking
	checkCompilationLog("hdrBloomFilterVertexShaderObject", hdrBloomFilterVertexShaderObject);

	// ***** HDRBloom Filter Fragment Shader *****
	hdrBloomFilterFragmentShaderObject = glCreateShader(GL_FRAGMENT_SHADER);

	const GLchar *hdrBloomFilterFragmentShaderSourceCode =
	{
		"#version 430 core																				\n" \
		"layout (binding = 0) uniform sampler2D hdr_image;												\n" \
		"out vec4 FragColor;																			\n" \
		"const float weights[] = float[](0.0024499299678342,											\n" \
		"	0.0043538453346397,																			\n" \
		"	0.0073599963704157,																			\n" \
		"	0.0118349786570722,																			\n" \
		"	0.0181026699707781,																			\n" \
		"	0.0263392293891488,																			\n" \
		"	0.0364543006660986,																			\n" \
		"	0.0479932050577658,																			\n" \
		"	0.0601029809166942,																			\n" \
		"	0.0715974486241365,																			\n" \
		"	0.0811305381519717,																			\n" \
		"	0.0874493212267511,																			\n" \
		"	0.0896631113333857,																			\n" \
		"	0.0874493212267511,																			\n" \
		"	0.0811305381519717,																			\n" \
		"	0.0715974486241365,																			\n" \
		"	0.0601029809166942,																			\n" \
		"	0.0479932050577658,																			\n" \
		"	0.0364543006660986,																			\n" \
		"	0.0263392293891488,																			\n" \
		"	0.0181026699707781,																			\n" \
		"	0.0118349786570722,																			\n" \
		"	0.0073599963704157,																			\n" \
		"	0.0043538453346397,																			\n" \
		"	0.0024499299678342);																		\n" \
		"void main(void) {																				\n" \
		"	vec4 c = vec4(0.0);																			\n" \
		"	ivec2 P = ivec2(gl_FragCoord.xy) - ivec2(0, weights.length() >> 1);							\n" \
		"	int i;																						\n" \
		"	for (i = 0; i < weights.length(); i++) {													\n" \
		"		c += texelFetch(hdr_image, P + ivec2(0, i), 0) * weights[i];							\n" \
		"	}																							\n" \
		"	FragColor = c;																				\n" \
		"}																								\n"
	};

	glShaderSource(hdrBloomFilterFragmentShaderObject, 1, (const GLchar**)&hdrBloomFilterFragmentShaderSourceCode, NULL);
	glCompileShader(hdrBloomFilterFragmentShaderObject);

	// Error checking
	checkCompilationLog("hdrBloomFilterFragmentShaderObject", hdrBloomFilterFragmentShaderObject);

	// ******* FILTER-X FRAGMENT SHADER ***********
	// ***** HDRBloom Filter Fragment Shader *****
	GLuint hdrBloomXFilterFragmentShaderObject = glCreateShader(GL_FRAGMENT_SHADER);

	const GLchar *hdrBloomXFilterFragmentShaderSourceCode =
	{
		"#version 430 core																				\n" \
		"layout (binding = 0) uniform sampler2D hdr_image;												\n" \
		"out vec4 FragColor;																			\n" \
		"const float weights[] = float[](0.0024499299678342,											\n" \
		"	0.0043538453346397,																			\n" \
		"	0.0073599963704157,																			\n" \
		"	0.0118349786570722,																			\n" \
		"	0.0181026699707781,																			\n" \
		"	0.0263392293891488,																			\n" \
		"	0.0364543006660986,																			\n" \
		"	0.0479932050577658,																			\n" \
		"	0.0601029809166942,																			\n" \
		"	0.0715974486241365,																			\n" \
		"	0.0811305381519717,																			\n" \
		"	0.0874493212267511,																			\n" \
		"	0.0896631113333857,																			\n" \
		"	0.0874493212267511,																			\n" \
		"	0.0811305381519717,																			\n" \
		"	0.0715974486241365,																			\n" \
		"	0.0601029809166942,																			\n" \
		"	0.0479932050577658,																			\n" \
		"	0.0364543006660986,																			\n" \
		"	0.0263392293891488,																			\n" \
		"	0.0181026699707781,																			\n" \
		"	0.0118349786570722,																			\n" \
		"	0.0073599963704157,																			\n" \
		"	0.0043538453346397,																			\n" \
		"	0.0024499299678342);																		\n" \
		"void main(void) {																				\n" \
		"	vec4 c = vec4(0.0);																			\n" \
		"	ivec2 P = ivec2(gl_FragCoord.xy) - ivec2(weights.length() >> 1, 0);							\n" \
		"	int i;																						\n" \
		"	for (i = 0; i < weights.length(); i++) {													\n" \
		"		c += texelFetch(hdr_image, P + ivec2(i, 0), 0) * weights[i];							\n" \
		"	}																							\n" \
		"	FragColor = c;																				\n" \
		"}																								\n"
	};

	glShaderSource(hdrBloomXFilterFragmentShaderObject, 1, (const GLchar**)&hdrBloomXFilterFragmentShaderSourceCode, NULL);
	glCompileShader(hdrBloomXFilterFragmentShaderObject);

	// Error checking
	checkCompilationLog("hdrBloomXFilterFragmentShaderObject", hdrBloomXFilterFragmentShaderObject);

	// ***** Filter Shader Program Object ****
	ShaderProgramObject_GaussianBlur_X_Direction = glCreateProgram();

	// Attach Shaders
	glAttachShader(ShaderProgramObject_GaussianBlur_X_Direction, hdrBloomFilterVertexShaderObject);
	glAttachShader(ShaderProgramObject_GaussianBlur_X_Direction, hdrBloomXFilterFragmentShaderObject);

	// Link Program
	glLinkProgram(ShaderProgramObject_GaussianBlur_X_Direction);

	// Error checking
	checkLinkLog("ShaderProgramObject_GaussianBlur_X_Direction", ShaderProgramObject_GaussianBlur_X_Direction);

	// ***** Filter Shader Program Object ****
	ShaderProgramObject_GaussianBlur_Y_Direction = glCreateProgram();

	// Attach Shaders
	glAttachShader(ShaderProgramObject_GaussianBlur_Y_Direction, hdrBloomFilterVertexShaderObject);
	glAttachShader(ShaderProgramObject_GaussianBlur_Y_Direction, hdrBloomFilterFragmentShaderObject);

	// Link Program
	glLinkProgram(ShaderProgramObject_GaussianBlur_Y_Direction);

	// Error checking
	checkLinkLog("ShaderProgramObject_GaussianBlur_Y_Direction", ShaderProgramObject_GaussianBlur_Y_Direction);

	// ****** HDRBloom Resolve Vertex Shader ******
	hdrBloomResolveVertexShaderObject = glCreateShader(GL_VERTEX_SHADER);

	const GLchar *hdrBloomResolveVertexShaderSourceCode =
	{
		"#version 450 core																				\n" \
		"void main(void) {																				\n" \
		"	const vec4 vertices[] = vec4[](vec4(-1.0, -1.0, 0.5, 1.0),									\n" \
		"									vec4(1.0, -1.0, 0.5, 1.0),									\n" \
		"									vec4(-1.0, 1.0, 0.5, 1.0),									\n" \
		"									vec4(1.0, 1.0, 0.5, 1.0));									\n" \
		"	gl_Position = vertices[gl_VertexID];														\n" \
		"}																								\n" \
	};
	
	/*const GLchar *hdrBloomResolveVertexShaderSourceCode =
	{
		"#version 450 core																				\n" \
		"in vec3 vPosition;																				\n" \
		"uniform mat4 u_mvp_matrix;																		\n" \
		"void main(void) {																				\n" \
		"	gl_Position = u_mvp_matrix * vPosition;														\n" \
		"}																								\n" \
	};*/

	glShaderSource(hdrBloomResolveVertexShaderObject, 1, (const GLchar **)&hdrBloomResolveVertexShaderSourceCode, NULL);
	glCompileShader(hdrBloomResolveVertexShaderObject);

	// Error checking
	checkCompilationLog("hdrBloomResolveVertexShaderObject", hdrBloomResolveVertexShaderObject);

	// ****** HDRBloom Resolve Fragment Shader *****
	hdrBloomResolveFragmentShaderObject = glCreateShader(GL_FRAGMENT_SHADER);

	const GLchar *hdrBloomResolveFragmentShaderSourceCode =
	{
		"#version 450 core																				\n" \
		"layout (binding = 0) uniform sampler2D hdr_image;												\n" \
		"layout (binding = 1) uniform sampler2D bloom_image;											\n" \
		"layout (binding = 2) uniform sampler2D godRays_image;											\n" \
		"uniform float exposure = 0.9;																	\n" \
		"uniform float bloom_factor = 1.0;																\n" \
		"uniform float scene_factor = 1.0;																\n" \
		"uniform float godRays_factor = 1.0;																\n" \
		"out vec4 color;																				\n" \
		"void main(void) {																				\n" \
		"const float gamma = 1.2;" \
		"	vec4 c = vec4(0.0);																			\n" \
		"	c += texelFetch(hdr_image, ivec2(gl_FragCoord.xy), 0) * scene_factor;						\n" \
		"	c += texelFetch(bloom_image, ivec2(gl_FragCoord.xy), 0) * bloom_factor;						\n" \
		"	c += texelFetch(godRays_image, ivec2(gl_FragCoord.xy), 0) * godRays_factor;						\n" \
		"	c.rgb = vec3(1.0) - exp(-c.rgb * exposure);													\n" \
		
		//gamma correction
		"c.rgb = pow(c.rgb, vec3(1 / gamma));" \
		"color = vec4(c.rgb, 1.0);																					\n" \
		"}																								\n"
	};

	glShaderSource(hdrBloomResolveFragmentShaderObject, 1, (const GLchar **)&hdrBloomResolveFragmentShaderSourceCode, NULL);
	glCompileShader(hdrBloomResolveFragmentShaderObject);

	// Error checking 
	checkCompilationLog("hdrBloomResolveFragmentShaderObject", hdrBloomResolveFragmentShaderObject);

	// ***** HDRBloom Resolve Shader Program Object *****
	ShaderProgramObject_Final_Display_Resolution = glCreateProgram();

	// Attach shaders
	glAttachShader(ShaderProgramObject_Final_Display_Resolution, hdrBloomResolveVertexShaderObject);
	glAttachShader(ShaderProgramObject_Final_Display_Resolution, hdrBloomResolveFragmentShaderObject);
	
	// Pre-linking
	//glBindAttribLocation(ShaderProgramObject_Final_Display_Resolution, MATRIX_ATTRIBUTE_POSITION, "vPosition");

	// Link Program
	glLinkProgram(ShaderProgramObject_Final_Display_Resolution);

	// Error checking
	checkLinkLog("ShaderProgramObject_Final_Display_Resolution", ShaderProgramObject_Final_Display_Resolution);

	// Post-linking
	uniforms.resolve.exposure_bloom = glGetUniformLocation(ShaderProgramObject_Final_Display_Resolution, "exposure");
	uniforms.resolve.bloom_factor = glGetUniformLocation(ShaderProgramObject_Final_Display_Resolution, "bloom_factor");
	uniforms.resolve.scene_factor = glGetUniformLocation(ShaderProgramObject_Final_Display_Resolution, "scene_factor");
	uniforms.resolve.godRays_factor = glGetUniformLocation(ShaderProgramObject_Final_Display_Resolution, "godRays_factor");
	uniforms.resolve.godRays_image = glGetUniformLocation(ShaderProgramObject_Final_Display_Resolution, "godRays_image");


	//glEnable(GL_MULTISAMPLE);

	// **** Framebuffer With Two Color Attachment ****
	static const GLenum buffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1,  GL_COLOR_ATTACHMENT2};

	glGenVertexArrays(1, &vao_framebuffer_bloom);
	glBindVertexArray(vao_framebuffer_bloom);

	glGenFramebuffers(1, &render_fbo_bloom);
	glBindFramebuffer(GL_FRAMEBUFFER, render_fbo_bloom);

	glGenTextures(1, &texScene_bloom);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, texScene_bloom);
	//glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGBA8, MAX_SCENE_WIDTH, MAX_SCENE_HEIGHT, GL_TRUE);
	//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, texScene_bloom, 0);
	glTextureStorage2DMultisample(texScene_bloom, 8, GL_RGBA8, MAX_SCENE_WIDTH, MAX_SCENE_HEIGHT, GL_TRUE);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texScene_bloom, 0);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
	
	glGenTextures(1, &texBrightPass_bloom);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, texBrightPass_bloom);
	/*glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGBA16F, MAX_SCENE_WIDTH, MAX_SCENE_HEIGHT, GL_TRUE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D_MULTISAMPLE, texBrightPass_bloom, 0);*/
	glTextureStorage2DMultisample(texBrightPass_bloom, 8, GL_RGBA16F, MAX_SCENE_WIDTH, MAX_SCENE_HEIGHT, GL_TRUE);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, texBrightPass_bloom, 0);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);

	glGenTextures(1, &texGodRaysPass);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, texGodRaysPass);
	/*glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGBA8, MAX_SCENE_WIDTH, MAX_SCENE_HEIGHT, GL_TRUE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D_MULTISAMPLE, texGodRaysPass, 0);*/
	glTextureStorage2DMultisample(texGodRaysPass, 8, GL_RGBA8, MAX_SCENE_WIDTH, MAX_SCENE_HEIGHT, GL_TRUE);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, texGodRaysPass, 0);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
	
	glGenRenderbuffers(1, &texDepth_bloom);
	glBindRenderbuffer(GL_RENDERBUFFER, texDepth_bloom);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER,8,  GL_DEPTH24_STENCIL8, MAX_SCENE_WIDTH, MAX_SCENE_HEIGHT);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, texDepth_bloom);
	glDrawBuffers(3, buffers);
	
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		fprintf(gpFile, "texScene_bloom frameBuffer not successful\n");
		fflush(gpFile);
	}
	fprintf(gpFile, "texScene_bloom frameBuffer  successful\n");
	fflush(gpFile);


	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	
	// Array of Framebuffer Object
	glGenFramebuffers(2, &gaussian_blur_fbo_bloom[0]);
	glGenTextures(2, &texGaussianBlur_bloom[0]);

	for (int i = 0; i < 2; i++) {
		glBindFramebuffer(GL_FRAMEBUFFER, gaussian_blur_fbo_bloom[i]);
		glBindTexture(GL_TEXTURE_2D, texGaussianBlur_bloom[i]);
		glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA16F, i ? MAX_SCENE_WIDTH : MAX_SCENE_HEIGHT, i ? MAX_SCENE_HEIGHT : MAX_SCENE_WIDTH);
		//glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB16F, 4096, 4096);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texGaussianBlur_bloom[i], 0);
		glDrawBuffers(1, buffers);
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// Creating a 1D Array
	static const GLfloat exposureLUT[20] =
	{
		11.0f, 6.0f, 3.2f, 2.8f, 2.2f,
		1.90f, 1.80f, 1.80f, 1.70f, 1.70f,
		1.60f, 1.60f, 1.50f, 1.50f, 1.40f,
		1.40f, 1.30f, 1.20f, 1.10f, 1.00f
	};

	glGenTextures(1, &texLUT_bloom);
	glBindTexture(GL_TEXTURE_1D, texLUT_bloom);

	glTexStorage1D(GL_TEXTURE_1D, 1, GL_R32F, 20);
	glTexSubImage1D(GL_TEXTURE_1D, 0, 0, 20, GL_RED, GL_FLOAT, exposureLUT);

	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	
}

void ApplyingBloom(void) {
	// variable declarations
	static const GLfloat black[] = { 0.0f, 0.0f, 0.0, 1.0f };
	static const GLfloat one = { 1.0f };

	glUseProgram(ShaderProgramObject_GaussianBlur_Y_Direction);

	glBindVertexArray(vao_framebuffer_bloom);

	glBindFramebuffer(GL_FRAMEBUFFER, gaussian_blur_fbo_bloom[0]);
	glClearBufferfv(GL_COLOR, 0, black);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, intermediate_texBrightPass_bloom);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glUseProgram(ShaderProgramObject_GaussianBlur_X_Direction);
	glBindFramebuffer(GL_FRAMEBUFFER, gaussian_blur_fbo_bloom[1]);
	glClearBufferfv(GL_COLOR, 0, black);


	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texGaussianBlur_bloom[0]);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glUseProgram(0);


	glClearBufferfv(GL_COLOR, 0, black);
	glClearBufferfv(GL_DEPTH, 0, &one);

	glUseProgram(ShaderProgramObject_Final_Display_Resolution);

	glUniform1f(uniforms.resolve.exposure_bloom, exposure_bloom);
	glUniform1f(uniforms.resolve.scene_factor, 1.0f);
	glUniform1f(uniforms.resolve.bloom_factor, bShowBloom_bloom ? bloomFactor : 0.0f);
	glUniform1f(uniforms.resolve.godRays_factor, 1.0f);
	

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texGaussianBlur_bloom[1]);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, intermediate_texScene_bloom);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, godRays_texture_attachment);
	
	glBindVertexArray(vao_framebuffer_bloom);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glBindVertexArray(0);

	glUseProgram(0);

	glDepthFunc(GL_LEQUAL);
}



void uninitializeBloom(void) {
	// code
	// Safe Release

	if (render_fbo_bloom) {
		glDeleteFramebuffers(1, &render_fbo_bloom);
		render_fbo_bloom = 0;
	}

	if (gaussian_blur_fbo_bloom[0]) {
		glDeleteFramebuffers(2, gaussian_blur_fbo_bloom);
		gaussian_blur_fbo_bloom[0] = 0;
		gaussian_blur_fbo_bloom[1] = 0;
	}

	if (texGaussianBlur_bloom[0]) {
		glDeleteTextures(2, texGaussianBlur_bloom);
		texGaussianBlur_bloom[0] = 0;
		texGaussianBlur_bloom[1] = 0;
	}

	if (texDepth_bloom) {
		glDeleteTextures(1, &texDepth_bloom);
		texDepth_bloom = 0;
	}

	if (texBrightPass_bloom) {
		glDeleteTextures(1, &texBrightPass_bloom);
		texBrightPass_bloom = 0;
	}

	if (texScene_bloom) {
		glDeleteTextures(1, &texScene_bloom);
		texScene_bloom = 0;
	}

	if (texLUT_bloom) {
		glDeleteTextures(1, &texLUT_bloom);
		texLUT_bloom = 0;
	}

	if (vao_framebuffer_bloom) {
		glDeleteVertexArrays(1, &vao_framebuffer_bloom);
		vao_framebuffer_bloom = 0;
	}

	programObjectSafeRelease(ShaderProgramObject_Final_Display_Resolution);
	programObjectSafeRelease(ShaderProgramObject_GaussianBlur_X_Direction);
	programObjectSafeRelease(ShaderProgramObject_GaussianBlur_Y_Direction);
	programObjectSafeRelease(ShaderProgramObject_hdrBloomScene);
}