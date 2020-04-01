
GLuint godRays_fbo;
GLuint godRays_texture_attachment;
GLuint godRays_depth_attachment;

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
}