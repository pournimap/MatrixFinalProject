
GLuint godRays_fbo;
GLuint godRays_texture_attachment;
GLuint godRays_depth_attachment;



GLuint intermediate_fbo;
GLuint intermediate_texScene_bloom;
GLuint intermediate_texBrightPass_bloom;
GLuint intermediate_texGodRaysPass;
GLuint intermediate_texDepth_bloom;
void initAllFrameBuffer()
{
	static const GLenum buffers[] = { GL_COLOR_ATTACHMENT0};

	//render color program need for processing
	glGenFramebuffers(1, &godRays_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, godRays_fbo);

	glGenTextures(1, &godRays_texture_attachment);
	glBindTexture(GL_TEXTURE_2D, godRays_texture_attachment);

	//glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, MAX_SCENE_WIDTH, MAX_SCENE_HEIGHT);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, MAX_SCENE_WIDTH, MAX_SCENE_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, godRays_texture_attachment, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glGenTextures(1, &godRays_depth_attachment);
	glBindTexture(GL_TEXTURE_2D, godRays_depth_attachment);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH_COMPONENT16, MAX_SCENE_WIDTH, MAX_SCENE_HEIGHT);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, godRays_depth_attachment, 0);
	glDrawBuffers(1, buffers);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		fprintf(gpFile, "fbo_godRays frameBuffer not successful\n");
		fflush(gpFile);
	}
	fprintf(gpFile, "fbo_godRays frameBuffer  successful\n");
	fflush(gpFile);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);




	static const GLenum buffers_new[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1,  GL_COLOR_ATTACHMENT2 };

	glGenFramebuffers(1, &intermediate_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, intermediate_fbo);


	glGenTextures(1, &intermediate_texScene_bloom);
	glBindTexture(GL_TEXTURE_2D, intermediate_texScene_bloom);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, MAX_SCENE_WIDTH, MAX_SCENE_HEIGHT);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, intermediate_texScene_bloom, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glGenTextures(1, &intermediate_texBrightPass_bloom);
	glBindTexture(GL_TEXTURE_2D, intermediate_texBrightPass_bloom);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA16F, MAX_SCENE_WIDTH, MAX_SCENE_HEIGHT);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, intermediate_texBrightPass_bloom, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glGenTextures(1, &intermediate_texGodRaysPass);
	glBindTexture(GL_TEXTURE_2D, intermediate_texGodRaysPass);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, MAX_SCENE_WIDTH, MAX_SCENE_HEIGHT);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, intermediate_texGodRaysPass, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glGenTextures(1, &intermediate_texDepth_bloom);
	glBindTexture(GL_TEXTURE_2D, intermediate_texDepth_bloom);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH_COMPONENT32F, MAX_SCENE_WIDTH, MAX_SCENE_HEIGHT);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, intermediate_texDepth_bloom, 0);
	glDrawBuffers(3, buffers_new);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		fprintf(gpFile, "intermediate_fbo frameBuffer not successful\n");
		fflush(gpFile);
	}
	fprintf(gpFile, "intermediate_fbo frameBuffer  successful\n");
	fflush(gpFile);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}