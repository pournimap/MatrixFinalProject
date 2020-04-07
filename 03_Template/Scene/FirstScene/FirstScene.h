GLuint gWoodTexture_FirstScene;

vec3 first_scene_camera_eye_coord		= { 0.0f,0.0f,5.0f };
vec3 first_scene_camera_center_coord	= { 0.0f,0.0f,0.0f };
vec3 first_scene_camera_up_coord		= { 0.0f,1.0f,0.0f }; 

void initFirstScene()
{
	glEnable(GL_TEXTURE_2D);
	
	bool status = LoadGLTextures_fire(&gWoodTexture_FirstScene, MAKEINTRESOURCE(104));
	if (status == FALSE)
	{
		fprintf(gpFile, "LoadGLTextures_fire Failed in FirstScene\n");
	}
}

float t_fire_FirstScene = 0.0f;
vec3 positionLamp_FirstScene[] = { vec3(0.0f, 0.0f, -2.0f), vec3(0.5f, -0.5f, -2.0f) };
#define gNumPointLights_pointLight_FirstScene  2

void renderLampWithPointLight()
{
		mat4 modelMatrix				= mat4::identity();
		mat4 scaleMatrix				= mat4::identity();
		mat4 rotateMatrix				= mat4::identity();
		mat4 viewMatrix_for_firstScene	= mat4::identity();							// we are going to use this, only here. so no neet to declare anywhere else


		PointLight pointLight[gNumPointLights_pointLight_FirstScene];
	
		for (int i = 0; i < gNumPointLights_pointLight_FirstScene; i++)
		{
				pointLight[i].u_La = vec3(0.0f, 0.0f, 0.0f);
				pointLight[i].u_Ld = vec3(1.0f, 0.749f, 0.0f);
				pointLight[i].u_Ls = vec3(1.0f, 0.749f, 0.0f);
				pointLight[i].u_linear = 0.01;
				pointLight[i].u_constant = 0.01;
				pointLight[i].u_quadratic = 0.0;
				pointLight[i].DiffuseIntensity = 1.0f;

				pointLight[i].position = positionLamp_FirstScene[i];
		}
	
	
		glUseProgram(gShaderProgramObject_pointLight);


		glUniform1i(u_bloom_is_activeUniform_pointLight, 1);
		glUniform1f(bloom_thresh_minUniform_pointLight, bloom_thresh_min);
		glUniform1f(bloom_thresh_maxUniform_pointLight, bloom_thresh_max);
		glUniform1i(applyBloomUniform_pointLight, 0);
		if (gbLight == true)
		{
			glUniform1i(gLKeyPressedUniform_pointLight, 1);

			glUniform3fv(gLdUniform_pointLight, 1, lightDiffuse);//white
			glUniform3fv(gLaUniform_pointLight, 1, lightAmbient);
			glUniform3fv(gLsUniform_pointLight, 1, lightSpecular);
			glUniform4fv(gLightPositionUniform_pointLight, 1, lightPosition);

				//pointLight
			glUniform1i(gNumPointLightsUniform_pointLight, gNumPointLights_pointLight_FirstScene);
			for (int i = 0; i < gNumPointLights_pointLight_FirstScene; i++)
			{
				glUniform3fv(m_pointLightsLocation[i].u_La, 1, pointLight[i].u_La);
				glUniform3fv(m_pointLightsLocation[i].u_Ls, 1, pointLight[i].u_Ls);
				glUniform3fv(m_pointLightsLocation[i].u_Ld, 1, pointLight[i].u_Ld);
				glUniform1f(m_pointLightsLocation[i].DiffuseIntensity, pointLight[i].DiffuseIntensity);

				glUniform3fv(m_pointLightsLocation[i].position, 1, pointLight[i].position);
				glUniform1f(m_pointLightsLocation[i].u_constant, pointLight[i].u_constant);
				glUniform1f(m_pointLightsLocation[i].u_linear, pointLight[i].u_linear);
				glUniform1f(m_pointLightsLocation[i].u_quadratic, pointLight[i].u_quadratic);
			}

			glUniform3fv(gKdUniform_pointLight, 1, material_diffuse);
			glUniform3fv(gKaUniform_pointLight, 1, material_ambient);
			glUniform3fv(gKsUniform_pointLight, 1, material_specular);
			glUniform1f(gMaterialShininessUniform_pointLight, material_shininess);

		}
		else
		{
			glUniform1i(gLKeyPressedUniform_pointLight, 0);
		}

		
		
		viewMatrix_for_firstScene = lookat(first_scene_camera_eye_coord, first_scene_camera_center_coord, first_scene_camera_up_coord);					// updating view matrix
		glUniformMatrix4fv(gProjectionMatrixUniform_pointLight, 1, GL_FALSE, gPerspectiveProjectionMatrix);
		
		glUniformMatrix4fv(gViewMatrixUniform_pointLight, 1, GL_FALSE, viewMatrix_for_firstScene);
		modelMatrix = vmath::translate(0.0f, 0.0f, -2.0f);


		glUniform1i(applyBloomUniform_pointLight, 0);

		modelMatrix = mat4::identity();
		scaleMatrix = mat4::identity();
		modelMatrix = vmath::translate(0.0f, -20.0f, -100.0f);
		scaleMatrix = vmath::scale(50.0f, 2.0f, 30.0f);
		modelMatrix = modelMatrix * scaleMatrix;
		glUniformMatrix4fv(gModelMatrixUniform_pointLight, 1, GL_FALSE, modelMatrix);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, gWoodTexture_FirstScene);
		glUniform1i(gTextureSamplerUniform_pointLight, 0);
		glUniform1i(gTextureActiveUniform_pointLight, 1);
		//glUniform1i(samplerUniform1_mor_pis, 0);

		drawCubeShape();

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//candle model
		modelMatrix = mat4::identity();
		scaleMatrix = mat4::identity();
		rotateMatrix = mat4::identity();

		modelMatrix = vmath::translate(-20.0f, -16.0f, -90.0f);
		scaleMatrix = scale(0.07f, 0.07f, 0.07f);
		//rotateMatrix = rotate(-90.0f, 0.0f, 1.0f, 0.0f);

		modelMatrix = modelMatrix * scaleMatrix;

		glUniformMatrix4fv(gModelMatrixUniform_pointLight, 1, GL_FALSE, modelMatrix);
		glBindVertexArray(gModel_Candle.Vao);
		for (int i = 0; i < gModel_Candle.model_mesh_data.size(); i++)
		{
			if (gbLight == true)
			{
				glUniform3fv(gKaUniform_pointLight, 1, gModel_Candle.model_material[gModel_Candle.model_mesh_data[i].material_index].Ka);
				glUniform3fv(gKdUniform_pointLight, 1, gModel_Candle.model_material[gModel_Candle.model_mesh_data[i].material_index].Kd);
				glUniform3fv(gKsUniform_pointLight, 1, gModel_Candle.model_material[gModel_Candle.model_mesh_data[i].material_index].Ks);
				glUniform1f(gMaterialShininessUniform_pointLight, material_shininess);
				glUniform1f(gAlphaUniform_pointLight, gModel_Candle.model_material[gModel_Candle.model_mesh_data[i].material_index].d);
				if (gModel_Candle.model_material[gModel_Candle.model_mesh_data[i].material_index].ismap_Kd == true)
				{
					glActiveTexture(GL_TEXTURE0);
					glBindTexture(GL_TEXTURE_2D, gModel_Candle.model_material[gModel_Candle.model_mesh_data[i].material_index].gTexture);
					glUniform1i(gTextureSamplerUniform_pointLight, 0);
					glUniform1i(gTextureActiveUniform_pointLight, 1);
					/*fprintf(gpFile, "perFragmentLight Krishna ismapKd true \n");
					fflush(gpFile);*/
				}
				else
					glUniform1i(gTextureActiveUniform_pointLight, 0);
			}
			glDrawArrays(GL_TRIANGLES, gModel_Candle.model_mesh_data[i].vertex_Index, gModel_Candle.model_mesh_data[i].vertex_Count);
		}
		glBindVertexArray(0);


		modelMatrix = mat4::identity();
		scaleMatrix = mat4::identity();
		rotateMatrix = mat4::identity();

		for (int i = 0; i < gNumPointLights_pointLight_FirstScene; i++)
		{
			modelMatrix = vmath::translate(positionLamp_FirstScene[i]);
			scaleMatrix = scale(0.07f, 0.07f, 0.07f);
			//rotateMatrix = rotate(-90.0f, 0.0f, 1.0f, 0.0f);

			modelMatrix = modelMatrix * scaleMatrix;

			glUniformMatrix4fv(gModelMatrixUniform_pointLight, 1, GL_FALSE, modelMatrix);

			drawCubeShape();
		}

		glUseProgram(0);
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		glUseProgram(gShaderProgramObject_fire);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		glClearDepth(1.0f);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		glEnable(GL_POINT_SPRITE);

		t_fire_FirstScene += 0.01f;
		if (t_fire_FirstScene > 360.0f)
		t_fire_FirstScene = 0.0f;

		glUseProgram(gShaderProgramObject_fire);

		glUniform1i(u_bloom_is_activeUniform_fire, 1);
		glUniform1f(bloom_thresh_minUniform_fire, bloom_thresh_min);
		glUniform1f(bloom_thresh_maxUniform_fire, bloom_thresh_max);
		glUniform1i(applyBloomUniform_fire, 1);

		mat4 modelMatrix_fire = mat4::identity();
		mat4 scaleMatrix_fire = mat4::identity();
		mat4 rotateMatrix_fire = mat4::identity();

		modelMatrix_fire = vmath::translate(-20.0f, 1.0f, -90.0f);
		//scaleMatrix_fire = vmath::scale(2.0f, 2.0f, 2.0f);
		//scaleMatrix_fire	= vmath::scale(50.0f, 50.0f, 100.0f);
		//rotateMatrix_fire = rotate(75.0f, 0.0f, 1.0f, 0.0f);

		modelMatrix_fire = modelMatrix_fire * rotateMatrix_fire * scaleMatrix_fire;

		glUniformMatrix4fv(gModelMatrixUniform_fire, 1, GL_FALSE, modelMatrix_fire);
		glUniformMatrix4fv(gViewMatrixUniform_fire, 1, GL_FALSE, viewMatrix_for_firstScene);

		glUniformMatrix4fv(gProjectionMatrixUniform_fire, 1, GL_FALSE, gPerspectiveProjectionMatrix);
		glUniform1f(timeUniform_fire, t_fire_FirstScene);

		//glPointSize(gWidth_fire / 10);
		glPointSize(50);
		glActiveTexture(GL_TEXTURE0);

		glBindTexture(GL_TEXTURE_2D, gParticleTexture_fire);
		glUniform1i(sTextureUniform_fire, 0);


		glBindVertexArray(vao_fire);
		glDrawArrays(GL_POINTS, 0, numParticles_fire);
		glBindVertexArray(0);

		glBindTexture(GL_TEXTURE_2D, 0);

		glDisable(GL_BLEND);
		glUseProgram(0);


}

void updteForFirstScene()
{
	// here we should write 1st scene camera movement


}