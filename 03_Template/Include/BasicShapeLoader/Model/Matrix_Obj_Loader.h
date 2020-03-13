#pragma once

#include "Obj_Loader1.h"
#include "Read_Mtl.h"
#include "Arrange_Material.h"


GLuint gVbo_Model_Position, gVbo_Model_Normal, gVbo_Model_Texture;

struct Model
{
	GLuint Vao;
	std::vector<float> model_vertices;
	std::vector<float> model_textures;
	std::vector<float> model_normals;
	std::vector<material> model_material;
	std::vector<Mesh_Data> model_mesh_data;
	char model_materialFileName[256];
};

Model gModel_Mahal;
Model gModel_Krishna;

void LoadAllModels()
{

	/*****************Load Mahal Model*****************/
	std::string mahalModelFilePath = "Resources/MahalOBJVersion1WithTextures/Mahal_Version_5.2_MahalWithoutChair.obj";

	LoadMeshData(mahalModelFilePath.c_str(), gModel_Mahal.model_vertices, gModel_Mahal.model_textures, gModel_Mahal.model_normals, gModel_Mahal.model_mesh_data, gModel_Mahal.model_materialFileName);

	LoadMaterialData(gModel_Mahal.model_materialFileName, gModel_Mahal.model_material);

	Rearrange_Material_Data(gModel_Mahal.model_mesh_data, gModel_Mahal.model_material);


	/*****************VAO For Model*****************/
	glGenVertexArrays(1, &gModel_Mahal.Vao);
	glBindVertexArray(gModel_Mahal.Vao);

	/*****************Model Position****************/
	glGenBuffers(1, &gVbo_Model_Position);
	glBindBuffer(GL_ARRAY_BUFFER, gVbo_Model_Position);
	glBufferData(GL_ARRAY_BUFFER, gModel_Mahal.model_vertices.size() * sizeof(float), &gModel_Mahal.model_vertices[0], GL_STATIC_DRAW);
	glVertexAttribPointer(MATRIX_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(MATRIX_ATTRIBUTE_POSITION);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	/*****************Model Color****************/
	glGenBuffers(1, &gVbo_Model_Normal);
	glBindBuffer(GL_ARRAY_BUFFER, gVbo_Model_Normal);
	glBufferData(GL_ARRAY_BUFFER, gModel_Mahal.model_normals.size() * sizeof(float), &gModel_Mahal.model_normals[0], GL_STATIC_DRAW);
	glVertexAttribPointer(MATRIX_ATTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(MATRIX_ATTRIBUTE_NORMAL);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	/*******************Texture******************/
	glGenBuffers(1, &gVbo_Model_Texture);
	glBindBuffer(GL_ARRAY_BUFFER, gVbo_Model_Texture);
	glBufferData(GL_ARRAY_BUFFER, gModel_Mahal.model_textures.size() * sizeof(float), &gModel_Mahal.model_textures[0], GL_STATIC_DRAW);
	glVertexAttribPointer(MATRIX_ATTRIBUTE_TEXTURE0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(MATRIX_ATTRIBUTE_TEXTURE0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);




	/*****************Load Krishna Model*****************/
	std::string krishnaModelFilePath = "Resources/krishna/krishna.obj";
	//std::string krishnaModelFilePath = "11MarchKrishnaModel/krishna.obj";

	LoadMeshData(krishnaModelFilePath.c_str(), gModel_Krishna.model_vertices, gModel_Krishna.model_textures, gModel_Krishna.model_normals, gModel_Krishna.model_mesh_data, gModel_Krishna.model_materialFileName);

	LoadMaterialData(gModel_Krishna.model_materialFileName, gModel_Krishna.model_material);

	Rearrange_Material_Data(gModel_Krishna.model_mesh_data, gModel_Krishna.model_material);

	/*****************VAO For Model*****************/
	glGenVertexArrays(1, &gModel_Krishna.Vao);
	glBindVertexArray(gModel_Krishna.Vao);

	/*****************Model Position****************/
	glGenBuffers(1, &gVbo_Model_Position);
	glBindBuffer(GL_ARRAY_BUFFER, gVbo_Model_Position);
	glBufferData(GL_ARRAY_BUFFER, gModel_Krishna.model_vertices.size() * sizeof(float), &gModel_Krishna.model_vertices[0], GL_STATIC_DRAW);
	glVertexAttribPointer(MATRIX_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(MATRIX_ATTRIBUTE_POSITION);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	/*****************Model Color****************/
	glGenBuffers(1, &gVbo_Model_Normal);
	glBindBuffer(GL_ARRAY_BUFFER, gVbo_Model_Normal);
	glBufferData(GL_ARRAY_BUFFER, gModel_Krishna.model_normals.size() * sizeof(float), &gModel_Krishna.model_normals[0], GL_STATIC_DRAW);
	glVertexAttribPointer(MATRIX_ATTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(MATRIX_ATTRIBUTE_NORMAL);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	/*******************Texture******************/
	glGenBuffers(1, &gVbo_Model_Texture);
	glBindBuffer(GL_ARRAY_BUFFER, gVbo_Model_Texture);
	glBufferData(GL_ARRAY_BUFFER, gModel_Krishna.model_textures.size() * sizeof(float), &gModel_Krishna.model_textures[0], GL_STATIC_DRAW);
	glVertexAttribPointer(MATRIX_ATTRIBUTE_TEXTURE0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(MATRIX_ATTRIBUTE_TEXTURE0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
}


void uninitializeAllModelData()
{
	for (int i = 0; i < gModel_Krishna.model_mesh_data.size(); i++)
	{
		if (gModel_Krishna.model_material[gModel_Krishna.model_mesh_data[i].material_index].gTexture)
		{
			glDeleteTextures(1, &gModel_Krishna.model_material[gModel_Krishna.model_mesh_data[i].material_index].gTexture);
			gModel_Krishna.model_material[gModel_Krishna.model_mesh_data[i].material_index].gTexture = 0;
		}
	}
	
	for (int i = 0; i < gModel_Mahal.model_mesh_data.size(); i++)
	{
		if (gModel_Mahal.model_material[gModel_Mahal.model_mesh_data[i].material_index].gTexture)
		{
			glDeleteTextures(1, &gModel_Mahal.model_material[gModel_Mahal.model_mesh_data[i].material_index].gTexture);
			gModel_Mahal.model_material[gModel_Mahal.model_mesh_data[i].material_index].gTexture = 0;
		}
	}
	
	if (gModel_Mahal.Vao)
	{
		glDeleteVertexArrays(1, &gModel_Mahal.Vao);
		gModel_Mahal.Vao = 0;
	}

	if (gModel_Krishna.Vao)
	{
		glDeleteVertexArrays(1, &gModel_Krishna.Vao);
		gModel_Krishna.Vao = 0;
	}

	if (gVbo_Model_Position)
	{
		glDeleteBuffers(1, &gVbo_Model_Position);
		gVbo_Model_Position = 0;
	}
	if (gVbo_Model_Normal)
	{
		glDeleteBuffers(1, &gVbo_Model_Normal);
		gVbo_Model_Normal = 0;
	}
	if (gVbo_Model_Texture)
	{
		glDeleteBuffers(1, &gVbo_Model_Texture);
		gVbo_Model_Texture = 0;
	}
	
}
