GLuint currentWidth;
GLuint currentHeight;

struct {
	struct {
		GLint focal_distance;
		GLint focal_depth;
	} dof;
} uniform;

GLuint depth_fbo_dof;
GLuint depth_tex_dof;
GLuint color_tex_dof;
GLuint temp_tex_dof;
GLuint quad_vao_dof;

GLfloat focal_distance = 6800.0f;
GLfloat focal_depth = 30120.0f;

extern bool gbLight;

GLuint ShaderProgramObject_display_dof;
GLuint ShaderProgramObject_gensatCompute_dof;

void initializeDepthOfField(void) {
	// function declarations
	void initializeFrameBuffers(void);

	// variable declarations
	GLuint renderVertexShaderObject;
	GLuint renderFragmentShaderObject;
	GLuint displayVertexShaderObject;
	GLuint displayFragmentShaderObject;
	GLuint gensatComputeShaderObject;

	// code

	// ***** DISPLAY VERTEX SHADER OBJECT *****
	displayVertexShaderObject = glCreateShader(GL_VERTEX_SHADER);

	// Shader source code
	const GLchar* displayVertexShaderSourceCode =
	{
		"#version 450 core																				\n" \
		"																								\n" \
		"void main(void) {																				\n" \
		"	const vec4 vertex[] = vec4[] (  vec4(-1.0, -1.0, 0.5, 1.0),									\n" \
		"									vec4(1.0, -1.0, 0.5, 1.0),									\n" \
		"									vec4(-1.0, 1.0, 0.5, 1.0),									\n" \
		"									vec4(1.0, 1.0, 0.5, 1.0)  );								\n" \
		"																								\n" \
		"	gl_Position = vertex[gl_VertexID];															\n" \
		"}																								\n"
	};

	// Shader source
	glShaderSource(displayVertexShaderObject, 1, (const GLchar **)&displayVertexShaderSourceCode, NULL);

	// Compile shader
	glCompileShader(displayVertexShaderObject);

	// Error checking
	checkCompilationLog("displayVertexShaderObject", displayVertexShaderObject);

	// **** DISPLAY FRAGMENT SHADER ***
	displayFragmentShaderObject = glCreateShader(GL_FRAGMENT_SHADER);

	// Display Fragment Shader Source Code
	const GLchar *displayFragmentShaderSourceCode =
	{
		"#version 450 core																				\n" \
		"																								\n" \
		"layout (binding = 0) uniform sampler2D input_image;											\n" \
		"																								\n" \
		"layout (location = 0) out vec4 color;															\n" \
		"																								\n" \
		"uniform float focal_distance = 50.0;															\n" \
		"uniform float focal_depth = 30.0;																\n" \
		"																								\n" \
		"void main(void) {																				\n" \
		"	// S, used to scale the texture coordinates before											\n" \
		"	// looking up data in our SAT image															\n" \
		"	vec2 s = 1.0 / textureSize(input_image, 0);													\n" \
		"																								\n" \
		"	// C is the center of the filter															\n" \
		"	vec2 C = gl_FragCoord.xy;																	\n" \
		"																								\n" \
		"	// First, retrieve the value of the SAT at the center										\n" \
		"	// of the filter. The last channel of this value stores										\n" \
		"	// the view-space depth of the pixel.														\n" \
		"	vec4 v = texelFetch(input_image, ivec2(gl_FragCoord.xy), 0).rgba;							\n" \
		"																								\n" \
		"	// M will be the radius of our filter kernel												\n" \
		"	float m;																					\n" \
		"																								\n" \
		"	// For this application, we clear our depth image to zero									\n" \
		"	// before rendering to it, so if it's still zero we haven't									\n" \
		"	// rendered to the image here. Thus we set our radius to									\n" \
		"	// 0.5 (i.e., a diameter of 1.0) and move on.												\n" \
		"	if (v.w == 0.0) {																			\n" \
		"		m = 0.5;																				\n" \
		"	}																							\n" \
		"	else {																						\n" \
		"		// Calculate a circle of confusion														\n" \
		"		m = abs(v.w - focal_distance);															\n" \
		"																								\n" \
		"		// Simple smoothStep scale and bias. Minimum radius is									\n" \
		"		// 0.5 (diameter 1.0), maximum is 8.0. Box filter kernels								\n" \
		"		// greater than about 16 pixels don't look good at all.									\n" \
		"		m = 0.5 + smoothstep(0.0, focal_depth, m) * 7.5;										\n" \
		"	}																							\n" \
		"																								\n" \
		"	// Calculate the positions of the four corners of our										\n" \
		"	// area to sample from.																		\n" \
		"	vec2 P0 = vec2(C * 1.0) + vec2(-m, -m);														\n" \
		"	vec2 P1 = vec2(C * 1.0) + vec2(-m, m);														\n" \
		"	vec2 P2 = vec2(C * 1.0) + vec2(m, -m);														\n" \
		"	vec2 P3 = vec2(C * 1.0) + vec2(m, m);														\n" \
		"																								\n" \
		"	// Scale our coordinates																	\n" \
		"	P0 *= s;																					\n" \
		"	P1 *= s;																					\n" \
		"	P2 *= s;																					\n" \
		"	P3 *= s;																					\n" \
		"																								\n" \
		"	// Fetch the values of the SAT at the four corners											\n" \
		"	vec3 a = textureLod(input_image, P0, 0).rgb;												\n" \
		"	vec3 b = textureLod(input_image, P1, 0).rgb;												\n" \
		"	vec3 c = textureLod(input_image, P2, 0).rgb;												\n" \
		"	vec3 d = textureLod(input_image, P3, 0).rgb;												\n" \
		"																								\n" \
		"	// Calculate the sum of all pixels inside the kernel.										\n" \
		"	vec3 f = a - b - c + d;																		\n" \
		"																								\n" \
		"	// Scale radius->diameter																	\n" \
		"	m *= 2;																						\n" \
		"																								\n" \
		"	// Divide through the area																	\n" \
		"	f /= float(m * m);																			\n" \
		"																								\n" \
		"	// Output final color																		\n" \
		"	color = vec4(f, 1.0);																		\n" \
		"}																								\n"
	};

	// Shader source
	glShaderSource(displayFragmentShaderObject, 1, (const GLchar **)&displayFragmentShaderSourceCode, NULL);

	// Compile shader
	glCompileShader(displayFragmentShaderObject);

	// Error checking
	checkCompilationLog("displayFragmentShaderObject", displayFragmentShaderObject);

	// **** DISPLAY SHADER PROGRAM OBJECT ****
	ShaderProgramObject_display_dof = glCreateProgram();

	// Attach shaders
	glAttachShader(ShaderProgramObject_display_dof, displayVertexShaderObject);
	glAttachShader(ShaderProgramObject_display_dof, displayFragmentShaderObject);

	// Link Program
	glLinkProgram(ShaderProgramObject_display_dof);

	// Error checking
	checkLinkLog("ShaderProgramObject_display_dof", ShaderProgramObject_display_dof);

	// Uniform location
	uniform.dof.focal_distance = glGetUniformLocation(ShaderProgramObject_display_dof, "focal_distance");
	uniform.dof.focal_depth = glGetUniformLocation(ShaderProgramObject_display_dof, "focal_depth");

	//	**** COMPUTE SHADER OBJECT ****
	gensatComputeShaderObject = glCreateShader(GL_COMPUTE_SHADER);

	// Shader source code
	const GLchar *gensatComputeShaderSourceCode =
	{
		"#version 450 core																				\n" \
		"																								\n" \
		"layout (local_size_x = 1024) in;																\n" \
		"																								\n" \
		"shared vec3 shared_data[gl_WorkGroupSize.x * 2];												\n" \
		"																								\n" \
		"layout (binding = 0, rgba32f) readonly uniform image2D input_image;							\n" \
		"layout (binding = 1, rgba32f) writeonly uniform image2D output_image;							\n" \
		"																								\n" \
		"void main(void) {																				\n" \
		"	uint id = gl_LocalInvocationID.x;															\n" \
		"	uint rd_id;																					\n" \
		"	uint wr_id;																					\n" \
		"	uint mask;																					\n" \
		"	ivec2 P0 = ivec2(id * 2, gl_WorkGroupID.x);													\n" \
		"	ivec2 P1 = ivec2(id * 2 + 1, gl_WorkGroupID.x);												\n" \
		"																								\n" \
		"																								\n" \
		"	const uint steps = uint(log2(gl_WorkGroupSize.x)) + 1;										\n" \
		"	uint step = 0;																				\n" \
		"																								\n" \
		"	vec4 i0 = imageLoad(input_image, P0);														\n" \
		"	vec4 i1 = imageLoad(input_image, P1);														\n" \
		"																								\n" \
		"	shared_data[P0.x] = i0.rgb;																	\n" \
		"	shared_data[P1.x] = i1.rgb;																	\n" \
		"																								\n" \
		"	barrier();																					\n" \
		"																								\n" \
		"	for (step = 0; step < steps; step++) {														\n" \
		"		mask = (1 << step) - 1;																	\n" \
		"		rd_id = ((id >> step) << (step + 1)) + mask;											\n" \
		"		wr_id = rd_id + 1 + (id & mask);														\n" \
		"																								\n" \
		"		shared_data[wr_id] += shared_data[rd_id];												\n" \
		"																								\n" \
		"		barrier();																				\n" \
		"	}																							\n" \
		"																								\n" \
		"	imageStore(output_image, P0.yx, vec4(shared_data[P0.x], i0.a));								\n" \
		"	imageStore(output_image, P1.yx, vec4(shared_data[P1.x], i1.a));								\n" \
		"}																								\n"
	};

	// Shader source
	glShaderSource(gensatComputeShaderObject, 1, (const GLchar **)&gensatComputeShaderSourceCode, NULL);

	// Error checking
	checkCompilationLog("gensatComputeShaderObject", gensatComputeShaderObject);

	// **** GENSAT COMPUTE SHADER OBJECT *****
	ShaderProgramObject_gensatCompute_dof = glCreateProgram();

	// Attach shaders
	glAttachShader(ShaderProgramObject_gensatCompute_dof, gensatComputeShaderObject);

	// Link Program
	glLinkProgram(ShaderProgramObject_gensatCompute_dof);

	// Error checking
	checkLinkLog("ShaderProgramObject_gensatCompute_dof", ShaderProgramObject_gensatCompute_dof);

	// Initialize FrameBuffers
	initializeFrameBuffers();
}

void initializeFrameBuffers(void) {
	// code
	glGenFramebuffers(1, &depth_fbo_dof);
	glBindFramebuffer(GL_FRAMEBUFFER, depth_fbo_dof);

	glGenTextures(1, &depth_tex_dof);
	glBindTexture(GL_TEXTURE_2D, depth_tex_dof);
	glTexStorage2D(GL_TEXTURE_2D, 11, GL_DEPTH_COMPONENT32F, FBO_SIZE, FBO_SIZE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glGenTextures(1, &color_tex_dof);
	glBindTexture(GL_TEXTURE_2D, color_tex_dof);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA32F, FBO_SIZE, FBO_SIZE);

	glGenTextures(1, &temp_tex_dof);
	glBindTexture(GL_TEXTURE_2D, temp_tex_dof);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA32F, FBO_SIZE, FBO_SIZE);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depth_tex_dof, 0);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, color_tex_dof, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
	// Initialize Vertex Arrays 
	//initializeVertexArrays();
	
	// Vertex Array Initialization
	glGenVertexArrays(1, &quad_vao_dof);
	glBindVertexArray(quad_vao_dof);
}

void uninitializeDOF(void) {
	// code
	if (depth_tex_dof) {
		glDeleteTextures(1, &depth_tex_dof);
		depth_tex_dof = 0;
	}

	if (color_tex_dof) {
		glDeleteTextures(1, &color_tex_dof);
		color_tex_dof = 0;
	}

	if (temp_tex_dof) {
		glDeleteTextures(1, &temp_tex_dof);
		temp_tex_dof = 0;
	}

	if (quad_vao_dof) {
		glDeleteVertexArrays(1, &quad_vao_dof);
		quad_vao_dof = 0;
	}

	if (depth_fbo_dof) {
		glDeleteFramebuffers(1, &depth_fbo_dof);
		depth_fbo_dof = 0;
	}
	
	programObjectSafeRelease(ShaderProgramObject_display_dof);
	programObjectSafeRelease(ShaderProgramObject_gensatCompute_dof);
}


void applyDOF(void) {
	// code
	if (!gbLight) {
		return;
	}
	
	// variable declarations
	static const GLfloat ones[] = { 1.0f };
	static const GLfloat zero[] = { 0.0f };
	static const GLfloat gray[] = { 0.1f, 0.1f, 0.1f, 0.0f };
	static const GLenum attachments[] = { GL_COLOR_ATTACHMENT0 };
	static const mat4 scale_bias_matrix = mat4(vec4(0.5f, 0.0f, 0.0f, 0.0f),
											vec4(0.0f, 0.5f, 0.0f, 0.0f),
											vec4(0.0f, 0.0f, 0.5f, 0.0f), 
											vec4(0.5f, 0.5f, 0.5f, 1.0f));

	glBindFramebuffer(GL_FRAMEBUFFER, depth_fbo_dof);
	glDrawBuffers(1, attachments);

	glClearBufferfv(GL_COLOR, 0, gray);
	glClearBufferfv(GL_DEPTH, 0, ones);

	// render the objects
	glEnable(GL_DEPTH_TEST);
}

void stopApplyingDOF(void) {
	// code
	if (!gbLight) {
		return;
	}
	
	// Unbind Framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
	// **** COMPUTE SHADER PROCESSING ****
	glUseProgram(ShaderProgramObject_gensatCompute_dof);

	glBindImageTexture(0, color_tex_dof, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);
	glBindImageTexture(1, temp_tex_dof, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);

	glDispatchCompute(currentHeight, 1, 1);

	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

	glBindImageTexture(0, temp_tex_dof, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);
	glBindImageTexture(1, color_tex_dof, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);

	glDispatchCompute(currentWidth, 1, 1);

	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, color_tex_dof);

	glDisable(GL_DEPTH_TEST);

	glUseProgram(ShaderProgramObject_display_dof);
	glUniform1f(uniform.dof.focal_distance, focal_distance);
	glUniform1f(uniform.dof.focal_depth, focal_depth);

	glBindVertexArray(quad_vao_dof);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}
