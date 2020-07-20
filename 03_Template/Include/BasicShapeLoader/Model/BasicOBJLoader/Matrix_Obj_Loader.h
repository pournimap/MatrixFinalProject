#pragma once

#include "Obj_Loader1.h"
#include "Read_Mtl.h"
#include "Arrange_Material.h"


GLuint gVbo_Model_Position, gVbo_Model_Normal, gVbo_Model_Texture, gVbo_Model_Tangents, gVbo_Model_BiTangents;

struct Model_Obj
{
	GLuint Vao;
	std::vector<float> model_vertices;
	std::vector<float> model_textures;
	std::vector<float> model_normals;
	
	std::vector<float> model_tangents;
	std::vector<float> model_bitangents;

	std::vector<material> model_material;
	std::vector<Mesh_Data> model_mesh_data;
	char model_materialFileName[256];
};

Model_Obj gModel_Mahal;

Model_Obj gModel_Krishna;
Model_Obj gModel_Krishna_Seated;
Model_Obj gModel_Krishna_Seated2;

Model_Obj gModel_KrishnaChair;
Model_Obj gModel_OtherChair;

Model_Obj gModel_Mashal;
Model_Obj gModel_SudarshanChakra;

Model_Obj gModel_Candle;

void LoadAllModels()
{

	/*****************Load Mahal Model*****************/
	std::string mahalModelFilePath = "Resources/MahalOBJVersion1WithTextures/Mahal_Version_5.4.obj";

	LoadMeshData(mahalModelFilePath.c_str(), gModel_Mahal.model_vertices, gModel_Mahal.model_textures, gModel_Mahal.model_normals, 
		gModel_Mahal.model_tangents, gModel_Mahal.model_bitangents, gModel_Mahal.model_mesh_data, gModel_Mahal.model_materialFileName);

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
	std::string krishnaModelFilePath = "Resources/SeatPositionOtherKing/Shishupal.obj";
	//std::string krishnaModelFilePath = "11MarchKrishnaModel/krishna.obj";

	LoadMeshData(krishnaModelFilePath.c_str(), gModel_Krishna.model_vertices, gModel_Krishna.model_textures, gModel_Krishna.model_normals, 
		gModel_Krishna.model_tangents, gModel_Krishna.model_bitangents, gModel_Krishna.model_mesh_data, gModel_Krishna.model_materialFileName);

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
	
	
	/*****************Load Krishna Seated Model*****************/
	std::string krishnaSeatedModelFilePath = "Resources/SeatPositionOtherKing/SeatPosWithOrna.obj";
	//std::string krishnaModelFilePath = "11MarchKrishnaModel/krishna.obj";

	LoadMeshData(krishnaSeatedModelFilePath.c_str(), gModel_Krishna_Seated.model_vertices, gModel_Krishna_Seated.model_textures, gModel_Krishna_Seated.model_normals,
		gModel_Krishna_Seated.model_tangents, gModel_Krishna_Seated.model_bitangents, gModel_Krishna_Seated.model_mesh_data, gModel_Krishna_Seated.model_materialFileName);

	LoadMaterialData(gModel_Krishna_Seated.model_materialFileName, gModel_Krishna_Seated.model_material);

	Rearrange_Material_Data(gModel_Krishna_Seated.model_mesh_data, gModel_Krishna_Seated.model_material);

	/*****************VAO For Model*****************/
	glGenVertexArrays(1, &gModel_Krishna_Seated.Vao);
	glBindVertexArray(gModel_Krishna_Seated.Vao);

	/*****************Model Position****************/
	glGenBuffers(1, &gVbo_Model_Position);
	glBindBuffer(GL_ARRAY_BUFFER, gVbo_Model_Position);
	glBufferData(GL_ARRAY_BUFFER, gModel_Krishna_Seated.model_vertices.size() * sizeof(float), &gModel_Krishna_Seated.model_vertices[0], GL_STATIC_DRAW);
	glVertexAttribPointer(MATRIX_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(MATRIX_ATTRIBUTE_POSITION);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	/*****************Model Color****************/
	glGenBuffers(1, &gVbo_Model_Normal);
	glBindBuffer(GL_ARRAY_BUFFER, gVbo_Model_Normal);
	glBufferData(GL_ARRAY_BUFFER, gModel_Krishna_Seated.model_normals.size() * sizeof(float), &gModel_Krishna_Seated.model_normals[0], GL_STATIC_DRAW);
	glVertexAttribPointer(MATRIX_ATTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(MATRIX_ATTRIBUTE_NORMAL);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	/*******************Texture******************/
	glGenBuffers(1, &gVbo_Model_Texture);
	glBindBuffer(GL_ARRAY_BUFFER, gVbo_Model_Texture);
	glBufferData(GL_ARRAY_BUFFER, gModel_Krishna_Seated.model_textures.size() * sizeof(float), &gModel_Krishna_Seated.model_textures[0], GL_STATIC_DRAW);
	glVertexAttribPointer(MATRIX_ATTRIBUTE_TEXTURE0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(MATRIX_ATTRIBUTE_TEXTURE0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
	
	
	gModel_Krishna_Seated2 = gModel_Krishna_Seated;
	LoadMaterialData("Resources/SeatPositionOtherKing/SeatPosWithOrna2.mtl", gModel_Krishna_Seated2.model_material);
	Rearrange_Material_Data(gModel_Krishna_Seated2.model_mesh_data, gModel_Krishna_Seated2.model_material);
	/*****************VAO For Model*****************/
	glGenVertexArrays(1, &gModel_Krishna_Seated2.Vao);
	glBindVertexArray(gModel_Krishna_Seated2.Vao);
	/*****************Model Position****************/
	glGenBuffers(1, &gVbo_Model_Position);
	glBindBuffer(GL_ARRAY_BUFFER, gVbo_Model_Position);
	glBufferData(GL_ARRAY_BUFFER, gModel_Krishna_Seated2.model_vertices.size() * sizeof(float), &gModel_Krishna_Seated2.model_vertices[0], GL_STATIC_DRAW);
	glVertexAttribPointer(MATRIX_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(MATRIX_ATTRIBUTE_POSITION);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	/*****************Model Color****************/
	glGenBuffers(1, &gVbo_Model_Normal);
	glBindBuffer(GL_ARRAY_BUFFER, gVbo_Model_Normal);
	glBufferData(GL_ARRAY_BUFFER, gModel_Krishna_Seated2.model_normals.size() * sizeof(float), &gModel_Krishna_Seated2.model_normals[0], GL_STATIC_DRAW);
	glVertexAttribPointer(MATRIX_ATTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(MATRIX_ATTRIBUTE_NORMAL);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	/*******************Texture******************/
	glGenBuffers(1, &gVbo_Model_Texture);
	glBindBuffer(GL_ARRAY_BUFFER, gVbo_Model_Texture);
	glBufferData(GL_ARRAY_BUFFER, gModel_Krishna_Seated2.model_textures.size() * sizeof(float), &gModel_Krishna_Seated2.model_textures[0], GL_STATIC_DRAW);
	glVertexAttribPointer(MATRIX_ATTRIBUTE_TEXTURE0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(MATRIX_ATTRIBUTE_TEXTURE0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	
	/*****************Load Krishna chair Model*****************/
	std::string krishnaChairModelFilePath = "Resources/MahalOBJVersion1WithTextures/krishnaChair.obj";

	LoadMeshData(krishnaChairModelFilePath.c_str(), gModel_KrishnaChair.model_vertices, gModel_KrishnaChair.model_textures, gModel_KrishnaChair.model_normals, 
		gModel_KrishnaChair.model_tangents, gModel_KrishnaChair.model_bitangents, gModel_KrishnaChair.model_mesh_data, gModel_KrishnaChair.model_materialFileName);

	LoadMaterialData(gModel_KrishnaChair.model_materialFileName, gModel_KrishnaChair.model_material);

	Rearrange_Material_Data(gModel_KrishnaChair.model_mesh_data, gModel_KrishnaChair.model_material);


	/*****************VAO For Model*****************/
	glGenVertexArrays(1, &gModel_KrishnaChair.Vao);
	glBindVertexArray(gModel_KrishnaChair.Vao);

	/*****************Model Position****************/
	glGenBuffers(1, &gVbo_Model_Position);
	glBindBuffer(GL_ARRAY_BUFFER, gVbo_Model_Position);
	glBufferData(GL_ARRAY_BUFFER, gModel_KrishnaChair.model_vertices.size() * sizeof(float), &gModel_KrishnaChair.model_vertices[0], GL_STATIC_DRAW);
	glVertexAttribPointer(MATRIX_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(MATRIX_ATTRIBUTE_POSITION);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	/*****************Model Color****************/
	glGenBuffers(1, &gVbo_Model_Normal);
	glBindBuffer(GL_ARRAY_BUFFER, gVbo_Model_Normal);
	glBufferData(GL_ARRAY_BUFFER, gModel_KrishnaChair.model_normals.size() * sizeof(float), &gModel_KrishnaChair.model_normals[0], GL_STATIC_DRAW);
	glVertexAttribPointer(MATRIX_ATTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(MATRIX_ATTRIBUTE_NORMAL);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	/*******************Texture******************/
	glGenBuffers(1, &gVbo_Model_Texture);
	glBindBuffer(GL_ARRAY_BUFFER, gVbo_Model_Texture);
	glBufferData(GL_ARRAY_BUFFER, gModel_KrishnaChair.model_textures.size() * sizeof(float), &gModel_KrishnaChair.model_textures[0], GL_STATIC_DRAW);
	glVertexAttribPointer(MATRIX_ATTRIBUTE_TEXTURE0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(MATRIX_ATTRIBUTE_TEXTURE0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
	
	
	
	/*****************Load other chairs Model*****************/
	std::string OtherChairModelFilePath = "Resources/MahalOBJVersion1WithTextures/SideRajeChair.obj";

	LoadMeshData(OtherChairModelFilePath.c_str(), gModel_OtherChair.model_vertices, gModel_OtherChair.model_textures, gModel_OtherChair.model_normals, 
		gModel_OtherChair.model_tangents, gModel_OtherChair.model_bitangents,  gModel_OtherChair.model_mesh_data, gModel_OtherChair.model_materialFileName);

	LoadMaterialData(gModel_OtherChair.model_materialFileName, gModel_OtherChair.model_material);

	Rearrange_Material_Data(gModel_OtherChair.model_mesh_data, gModel_OtherChair.model_material);


	/*****************VAO For Model*****************/
	glGenVertexArrays(1, &gModel_OtherChair.Vao);
	glBindVertexArray(gModel_OtherChair.Vao);

	/*****************Model Position****************/
	glGenBuffers(1, &gVbo_Model_Position);
	glBindBuffer(GL_ARRAY_BUFFER, gVbo_Model_Position);
	glBufferData(GL_ARRAY_BUFFER, gModel_OtherChair.model_vertices.size() * sizeof(float), &gModel_OtherChair.model_vertices[0], GL_STATIC_DRAW);
	glVertexAttribPointer(MATRIX_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(MATRIX_ATTRIBUTE_POSITION);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	/*****************Model Color****************/
	glGenBuffers(1, &gVbo_Model_Normal);
	glBindBuffer(GL_ARRAY_BUFFER, gVbo_Model_Normal);
	glBufferData(GL_ARRAY_BUFFER, gModel_OtherChair.model_normals.size() * sizeof(float), &gModel_OtherChair.model_normals[0], GL_STATIC_DRAW);
	glVertexAttribPointer(MATRIX_ATTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(MATRIX_ATTRIBUTE_NORMAL);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	/*******************Texture******************/
	glGenBuffers(1, &gVbo_Model_Texture);
	glBindBuffer(GL_ARRAY_BUFFER, gVbo_Model_Texture);
	glBufferData(GL_ARRAY_BUFFER, gModel_OtherChair.model_textures.size() * sizeof(float), &gModel_OtherChair.model_textures[0], GL_STATIC_DRAW);
	glVertexAttribPointer(MATRIX_ATTRIBUTE_TEXTURE0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(MATRIX_ATTRIBUTE_TEXTURE0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
	
	
	
	/*****************Mashal Model*****************/
	std::string MashalModelFilePath = "Resources/Mashal/Mashal.obj";

	LoadMeshData(MashalModelFilePath.c_str(), gModel_Mashal.model_vertices, gModel_Mashal.model_textures, gModel_Mashal.model_normals, 
		gModel_Mashal.model_tangents, gModel_Mashal.model_bitangents,  gModel_Mashal.model_mesh_data, gModel_Mashal.model_materialFileName);

	LoadMaterialData(gModel_Mashal.model_materialFileName, gModel_Mashal.model_material);

	Rearrange_Material_Data(gModel_Mashal.model_mesh_data, gModel_Mashal.model_material);


	/*****************VAO For Model*****************/
	glGenVertexArrays(1, &gModel_Mashal.Vao);
	glBindVertexArray(gModel_Mashal.Vao);

	/*****************Model Position****************/
	glGenBuffers(1, &gVbo_Model_Position);
	glBindBuffer(GL_ARRAY_BUFFER, gVbo_Model_Position);
	glBufferData(GL_ARRAY_BUFFER, gModel_Mashal.model_vertices.size() * sizeof(float), &gModel_Mashal.model_vertices[0], GL_STATIC_DRAW);
	glVertexAttribPointer(MATRIX_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(MATRIX_ATTRIBUTE_POSITION);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	/*****************Model Color****************/
	glGenBuffers(1, &gVbo_Model_Normal);
	glBindBuffer(GL_ARRAY_BUFFER, gVbo_Model_Normal);
	glBufferData(GL_ARRAY_BUFFER, gModel_Mashal.model_normals.size() * sizeof(float), &gModel_Mashal.model_normals[0], GL_STATIC_DRAW);
	glVertexAttribPointer(MATRIX_ATTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(MATRIX_ATTRIBUTE_NORMAL);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	/*******************Texture******************/
	glGenBuffers(1, &gVbo_Model_Texture);
	glBindBuffer(GL_ARRAY_BUFFER, gVbo_Model_Texture);
	glBufferData(GL_ARRAY_BUFFER, gModel_Mashal.model_textures.size() * sizeof(float), &gModel_Mashal.model_textures[0], GL_STATIC_DRAW);
	glVertexAttribPointer(MATRIX_ATTRIBUTE_TEXTURE0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(MATRIX_ATTRIBUTE_TEXTURE0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);


	/*****************Sudarshan Chakra Model*****************/
	std::string SudarshanChakraModelFilePath = "Resources/SudarshanChakra/SudharshanChakra.obj";

	LoadMeshData(SudarshanChakraModelFilePath.c_str(), gModel_SudarshanChakra.model_vertices, gModel_SudarshanChakra.model_textures, gModel_SudarshanChakra.model_normals, 
		gModel_SudarshanChakra.model_tangents, gModel_SudarshanChakra.model_bitangents, gModel_SudarshanChakra.model_mesh_data, gModel_SudarshanChakra.model_materialFileName);

	LoadMaterialData(gModel_SudarshanChakra.model_materialFileName, gModel_SudarshanChakra.model_material);

	Rearrange_Material_Data(gModel_SudarshanChakra.model_mesh_data, gModel_SudarshanChakra.model_material);


	/*****************VAO For Model*****************/
	glGenVertexArrays(1, &gModel_SudarshanChakra.Vao);
	glBindVertexArray(gModel_SudarshanChakra.Vao);

	/*****************Model Position****************/
	glGenBuffers(1, &gVbo_Model_Position);
	glBindBuffer(GL_ARRAY_BUFFER, gVbo_Model_Position);
	glBufferData(GL_ARRAY_BUFFER, gModel_SudarshanChakra.model_vertices.size() * sizeof(float), &gModel_SudarshanChakra.model_vertices[0], GL_STATIC_DRAW);
	glVertexAttribPointer(MATRIX_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(MATRIX_ATTRIBUTE_POSITION);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	/*****************Model Color****************/
	glGenBuffers(1, &gVbo_Model_Normal);
	glBindBuffer(GL_ARRAY_BUFFER, gVbo_Model_Normal);
	glBufferData(GL_ARRAY_BUFFER, gModel_SudarshanChakra.model_normals.size() * sizeof(float), &gModel_SudarshanChakra.model_normals[0], GL_STATIC_DRAW);
	glVertexAttribPointer(MATRIX_ATTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(MATRIX_ATTRIBUTE_NORMAL);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	/*******************Texture******************/
	glGenBuffers(1, &gVbo_Model_Texture);
	glBindBuffer(GL_ARRAY_BUFFER, gVbo_Model_Texture);
	glBufferData(GL_ARRAY_BUFFER, gModel_SudarshanChakra.model_textures.size() * sizeof(float), &gModel_SudarshanChakra.model_textures[0], GL_STATIC_DRAW);
	glVertexAttribPointer(MATRIX_ATTRIBUTE_TEXTURE0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(MATRIX_ATTRIBUTE_TEXTURE0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);



	/*****************Candle Model*****************/
	std::string CandleModelFilePath = "Resources/Candle/NormalPanati/panati.obj";

	LoadMeshData(CandleModelFilePath.c_str(), gModel_Candle.model_vertices, gModel_Candle.model_textures, gModel_Candle.model_normals, 
		gModel_Candle.model_tangents, gModel_Candle.model_bitangents, gModel_Candle.model_mesh_data, gModel_Candle.model_materialFileName);

	LoadMaterialData(gModel_Candle.model_materialFileName, gModel_Candle.model_material);

	Rearrange_Material_Data(gModel_Candle.model_mesh_data, gModel_Candle.model_material);

	/*****************VAO For Model*****************/
	glGenVertexArrays(1, &gModel_Candle.Vao);
	glBindVertexArray(gModel_Candle.Vao);

	/*****************Model Position****************/
	glGenBuffers(1, &gVbo_Model_Position);
	glBindBuffer(GL_ARRAY_BUFFER, gVbo_Model_Position);
	glBufferData(GL_ARRAY_BUFFER, gModel_Candle.model_vertices.size() * sizeof(float), &gModel_Candle.model_vertices[0], GL_STATIC_DRAW);
	glVertexAttribPointer(MATRIX_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(MATRIX_ATTRIBUTE_POSITION);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	/*****************Model Color****************/
	glGenBuffers(1, &gVbo_Model_Normal);
	glBindBuffer(GL_ARRAY_BUFFER, gVbo_Model_Normal);
	glBufferData(GL_ARRAY_BUFFER, gModel_Candle.model_normals.size() * sizeof(float), &gModel_Candle.model_normals[0], GL_STATIC_DRAW);
	glVertexAttribPointer(MATRIX_ATTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(MATRIX_ATTRIBUTE_NORMAL);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	/*****************Model Tangents****************/
	glGenBuffers(1, &gVbo_Model_Tangents);
	glBindBuffer(GL_ARRAY_BUFFER, gVbo_Model_Tangents);
	glBufferData(GL_ARRAY_BUFFER, gModel_Candle.model_tangents.size() * sizeof(float), &gModel_Candle.model_tangents[0], GL_STATIC_DRAW);
	glVertexAttribPointer(MATRIX_ATTRIBUTE_TANGENT, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(MATRIX_ATTRIBUTE_TANGENT);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	/*****************Model BiTangents****************/
	glGenBuffers(1, &gVbo_Model_BiTangents);
	glBindBuffer(GL_ARRAY_BUFFER, gVbo_Model_BiTangents);
	glBufferData(GL_ARRAY_BUFFER, gModel_Candle.model_bitangents.size() * sizeof(float), &gModel_Candle.model_bitangents[0], GL_STATIC_DRAW);
	glVertexAttribPointer(MATRIX_ATTRIBUTE_BITANGENT, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(MATRIX_ATTRIBUTE_BITANGENT);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	/*******************Texture******************/
	glGenBuffers(1, &gVbo_Model_Texture);
	glBindBuffer(GL_ARRAY_BUFFER, gVbo_Model_Texture);
	glBufferData(GL_ARRAY_BUFFER, gModel_Candle.model_textures.size() * sizeof(float), &gModel_Candle.model_textures[0], GL_STATIC_DRAW);
	glVertexAttribPointer(MATRIX_ATTRIBUTE_TEXTURE0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(MATRIX_ATTRIBUTE_TEXTURE0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

}

void uninitializeAllModelData()
{
	/*****************Candle Model*****************/
	for (int i = 0; i < gModel_Candle.model_mesh_data.size(); i++)
	{
		if (gModel_Candle.model_material[gModel_Candle.model_mesh_data[i].material_index].gTexture_diffuse)
		{
			glDeleteTextures(1, &gModel_Candle.model_material[gModel_Candle.model_mesh_data[i].material_index].gTexture_diffuse);
			gModel_Candle.model_material[gModel_Candle.model_mesh_data[i].material_index].gTexture_diffuse = 0;
		}
	}

	/*****************Sudarshan Chakra Model*****************/
	for (int i = 0; i < gModel_SudarshanChakra.model_mesh_data.size(); i++)
	{
		if (gModel_SudarshanChakra.model_material[gModel_SudarshanChakra.model_mesh_data[i].material_index].gTexture_diffuse)
		{
			glDeleteTextures(1, &gModel_SudarshanChakra.model_material[gModel_SudarshanChakra.model_mesh_data[i].material_index].gTexture_diffuse);
			gModel_SudarshanChakra.model_material[gModel_SudarshanChakra.model_mesh_data[i].material_index].gTexture_diffuse = 0;
		}
	}

	/*****************Mashal Model*****************/
	for (int i = 0; i < gModel_Mashal.model_mesh_data.size(); i++)
	{
		if (gModel_Mashal.model_material[gModel_Mashal.model_mesh_data[i].material_index].gTexture_diffuse)
		{
			glDeleteTextures(1, &gModel_Mashal.model_material[gModel_Mashal.model_mesh_data[i].material_index].gTexture_diffuse);
			gModel_Mashal.model_material[gModel_Mashal.model_mesh_data[i].material_index].gTexture_diffuse = 0;
		}
	}

	/*****************other chairs Model*****************/
	for (int i = 0; i < gModel_OtherChair.model_mesh_data.size(); i++)
	{
		if (gModel_OtherChair.model_material[gModel_OtherChair.model_mesh_data[i].material_index].gTexture_diffuse)
		{
			glDeleteTextures(1, &gModel_OtherChair.model_material[gModel_OtherChair.model_mesh_data[i].material_index].gTexture_diffuse);
			gModel_OtherChair.model_material[gModel_OtherChair.model_mesh_data[i].material_index].gTexture_diffuse = 0;
		}
	}
	
	/*****************Load Krishna chair Model*****************/
	for (int i = 0; i < gModel_KrishnaChair.model_mesh_data.size(); i++)
	{
		if (gModel_KrishnaChair.model_material[gModel_KrishnaChair.model_mesh_data[i].material_index].gTexture_diffuse)
		{
			glDeleteTextures(1, &gModel_KrishnaChair.model_material[gModel_KrishnaChair.model_mesh_data[i].material_index].gTexture_diffuse);
			gModel_KrishnaChair.model_material[gModel_KrishnaChair.model_mesh_data[i].material_index].gTexture_diffuse = 0;
		}
	}
	
	/*****************gModel_Krishna_Seated2*****************/
	for (int i = 0; i < gModel_Krishna_Seated2.model_mesh_data.size(); i++)
	{
		if (gModel_Krishna_Seated2.model_material[gModel_Krishna_Seated2.model_mesh_data[i].material_index].gTexture_diffuse)
		{
			glDeleteTextures(1, &gModel_Krishna_Seated2.model_material[gModel_Krishna_Seated2.model_mesh_data[i].material_index].gTexture_diffuse);
			gModel_Krishna_Seated2.model_material[gModel_Krishna_Seated2.model_mesh_data[i].material_index].gTexture_diffuse = 0;
		}
	}

	/*****************Krishna Seated Model*****************/
	for (int i = 0; i < gModel_Krishna_Seated.model_mesh_data.size(); i++)
	{
		if (gModel_Krishna_Seated.model_material[gModel_Krishna_Seated.model_mesh_data[i].material_index].gTexture_diffuse)
		{
			glDeleteTextures(1, &gModel_Krishna_Seated.model_material[gModel_Krishna_Seated.model_mesh_data[i].material_index].gTexture_diffuse);
			gModel_Krishna_Seated.model_material[gModel_Krishna_Seated.model_mesh_data[i].material_index].gTexture_diffuse = 0;
		}
	}
	
	/*****************Krishna Model*****************/
	for (int i = 0; i < gModel_Krishna.model_mesh_data.size(); i++)
	{
		if (gModel_Krishna.model_material[gModel_Krishna.model_mesh_data[i].material_index].gTexture_diffuse)
		{
			glDeleteTextures(1, &gModel_Krishna.model_material[gModel_Krishna.model_mesh_data[i].material_index].gTexture_diffuse);
			gModel_Krishna.model_material[gModel_Krishna.model_mesh_data[i].material_index].gTexture_diffuse = 0;
		}
	}
	
	/*****************Mahal Model*****************/
	for (int i = 0; i < gModel_Mahal.model_mesh_data.size(); i++)
	{
		if (gModel_Mahal.model_material[gModel_Mahal.model_mesh_data[i].material_index].gTexture_diffuse)
		{
			glDeleteTextures(1, &gModel_Mahal.model_material[gModel_Mahal.model_mesh_data[i].material_index].gTexture_diffuse);
			gModel_Mahal.model_material[gModel_Mahal.model_mesh_data[i].material_index].gTexture_diffuse = 0;
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

	if (gModel_Krishna_Seated.Vao)
	{
		glDeleteVertexArrays(1, &gModel_Krishna_Seated.Vao);
		gModel_Krishna_Seated.Vao = 0;
	}
	
	if (gModel_Krishna_Seated2.Vao)
	{
		glDeleteVertexArrays(1, &gModel_Krishna_Seated2.Vao);
		gModel_Krishna_Seated2.Vao = 0;
	}

	if (gModel_KrishnaChair.Vao)
	{
		glDeleteVertexArrays(1, &gModel_KrishnaChair.Vao);
		gModel_KrishnaChair.Vao = 0;
	}
	
	if (gModel_OtherChair.Vao)
	{
		glDeleteVertexArrays(1, &gModel_OtherChair.Vao);
		gModel_OtherChair.Vao = 0;
	}
	
	if (gModel_Mashal.Vao)
	{
		glDeleteVertexArrays(1, &gModel_Mashal.Vao);
		gModel_Mashal.Vao = 0;
	}

	if (gModel_SudarshanChakra.Vao)
	{
		glDeleteVertexArrays(1, &gModel_SudarshanChakra.Vao);
		gModel_SudarshanChakra.Vao = 0;
	}

	if (gModel_Candle.Vao)
	{
		glDeleteVertexArrays(1, &gModel_Candle.Vao);
		gModel_Candle.Vao = 0;
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
	if (gVbo_Model_Tangents)
	{
		glDeleteBuffers(1, &gVbo_Model_Tangents);
		gVbo_Model_Tangents = 0;
	}
	if (gVbo_Model_BiTangents)
	{
		glDeleteBuffers(1, &gVbo_Model_BiTangents);
		gVbo_Model_BiTangents = 0;
	}
}
