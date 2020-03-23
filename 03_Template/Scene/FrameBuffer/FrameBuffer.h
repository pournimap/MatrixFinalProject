#define FRAMEBUFFER_WIDTH 1024
#define FRAMEBUFFER_HEIGHT 1024

GLuint framebuffer_for_Bloom;
GLuint framebuffer_for_Bloom_color_attachment;
GLuint framebuffer_for_Bloom_depth_attachment;

GLuint framebuffer_for_All;
GLuint framebuffer_for_All_color_attachment;
GLuint framebuffer_for_All_depth_attachment;


void initFramebufferForBloom()
{
	// code
	// **** Framebuffer ****
	static const GLenum buffers[] = { GL_COLOR_ATTACHMENT0 };

	glGenTextures(1, &framebuffer_for_Bloom_depth_attachment);
	glBindTexture(GL_TEXTURE_2D, framebuffer_for_Bloom_depth_attachment);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glBindTexture(GL_TEXTURE_2D, 0);

	glGenFramebuffers(1, &framebuffer_for_Bloom);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_for_Bloom);

	glGenTextures(1, &framebuffer_for_Bloom_color_attachment);
	glBindTexture(GL_TEXTURE_2D, framebuffer_for_Bloom_color_attachment);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, framebuffer_for_Bloom_color_attachment, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, framebuffer_for_Bloom_depth_attachment, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		fprintf(gpFile, "framebuffer_for_bloom frameBuffer not successful\n");
		fflush(gpFile);
	}
	fprintf(gpFile, "framebuffer_for_bloom frameBuffer  successful\n");
	fflush(gpFile);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void initFramebufferForAll(void) {
	// code
	// **** Framebuffer ****
	static const GLenum buffers[] = { GL_COLOR_ATTACHMENT0 };

	glGenTextures(1, &framebuffer_for_All_depth_attachment);
	glBindTexture(GL_TEXTURE_2D, framebuffer_for_All_depth_attachment);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glBindTexture(GL_TEXTURE_2D, 0);


	glGenFramebuffers(1, &framebuffer_for_All);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_for_All);

	glGenTextures(1, &framebuffer_for_All_color_attachment);
	glBindTexture(GL_TEXTURE_2D, framebuffer_for_All_color_attachment);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, framebuffer_for_All_color_attachment, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, framebuffer_for_All_depth_attachment, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		fprintf(gpFile, "framebuffer_for_All frameBuffer not successful\n");
		fflush(gpFile);
	}
	fprintf(gpFile, "framebuffer_for_All frameBuffer  successful\n");
	fflush(gpFile);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void UpdateFrameBuffer(float width, float height)
{
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_for_Bloom);
	//depth
	glBindTexture(GL_TEXTURE_2D, framebuffer_for_Bloom_depth_attachment);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, framebuffer_for_Bloom_depth_attachment, 0);

	glBindTexture(GL_TEXTURE_2D, framebuffer_for_Bloom_color_attachment);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, framebuffer_for_Bloom_color_attachment, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		fprintf(gpFile, "framebuffer_for_bloom frameBuffer not successful\n");
		fflush(gpFile);
	}
	fprintf(gpFile, "framebuffer_for_bloom frameBuffer  successful\n");
	fflush(gpFile);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	///////////////////////////////////////////////

	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_for_All);
	//depth
	glBindTexture(GL_TEXTURE_2D, framebuffer_for_All_depth_attachment);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, framebuffer_for_All_depth_attachment, 0);

	glBindTexture(GL_TEXTURE_2D, framebuffer_for_All_color_attachment);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, framebuffer_for_All_color_attachment, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		fprintf(gpFile, "framebuffer_for_All frameBuffer not successful\n");
		fflush(gpFile);
	}
	fprintf(gpFile, "framebuffer_for_All frameBuffer  successful\n");
	fflush(gpFile);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
