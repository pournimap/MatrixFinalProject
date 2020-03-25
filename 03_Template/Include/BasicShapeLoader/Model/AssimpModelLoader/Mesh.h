#pragma once

#include "../../../math_lib_headers/glm/glm.hpp"
#include "../../../ExternalLibraries/Assimp/include/assimp/Importer.hpp"

#define NUM_BONES_PER_VEREX 4


struct Vertex
{
	glm::vec3 position;
	//glm::vec4 color;
	glm::vec3 normal;
	glm::vec2 tex_coords;
	glm::vec3 Tangent;
	glm::vec3 Bitangent;
};

struct BoneMatrix
{
	//in my code here is glm::vec4
	 aiMatrix4x4 offset_matrix;
	 aiMatrix4x4 final_world_transform;

};

struct Texture
{
	GLuint id;
	std::string type;
	//my code here string
	aiString path;
};

struct Material
{
	//unsigned int id;
	std::string type;
	float Value[3];
};

struct VertexBoneData
{
	//4 bone ids for each vertex and 4 weights for each vertex
	UINT ids[NUM_BONES_PER_VEREX];
	float weights[NUM_BONES_PER_VEREX];

	VertexBoneData()
	{
		memset(ids, 0, sizeof(ids));
		memset(weights, 0, sizeof(weights));
	}

	void addBoneData(UINT bone_id, float weight);

};

void VertexBoneData::addBoneData(UINT bone_id, float weight)
{
	for (unsigned int i = 0; i < NUM_BONES_PER_VEREX; i++)
	{
		if (weights[i] == 0.0)
		{
			ids[i] = bone_id;
			weights[i] = weight;
			return;
		}
	}
}
class Mesh
{
public:
	Mesh(std::vector<Vertex> vertex, std::vector<GLuint> ind, std::vector<Texture> texture, std::vector<Material> material, std::vector<VertexBoneData> bone_id_weights);

	Mesh() {};

	~Mesh();

	//Render Mesh
	void Draw(GLuint shaders_program);

private:
	//mesh data
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<Texture> textures;
	std::vector<Material> materials;
	std::vector<VertexBoneData> bones_id_weights_for_each_vertex;

	//buffers
	GLuint VAO;
	GLuint VBO_Vertices;
	GLuint VBO_bones;
	GLuint EBO_indices;

	//init buffers
	void SetupMesh();

};



Mesh::Mesh(std::vector<Vertex> vertex, std::vector<GLuint> ind, std::vector<Texture> texture, std::vector<Material> material, std::vector<VertexBoneData> bone_id_weights)
{
	vertices = vertex;
	indices = ind;
	textures = texture;
	materials = material;

	bones_id_weights_for_each_vertex = bone_id_weights;

	SetupMesh();
}

Mesh::~Mesh()
{
}

void Mesh::SetupMesh()
{
	

	//vertices data
	glGenBuffers(1, &VBO_Vertices);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_Vertices);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices[0]), &vertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &VBO_bones);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_bones);
	glBufferData(GL_ARRAY_BUFFER, bones_id_weights_for_each_vertex.size() * sizeof(bones_id_weights_for_each_vertex[0]), &bones_id_weights_for_each_vertex[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//indices
	glGenBuffers(1, &EBO_indices);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_indices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_Vertices);
	glVertexAttribPointer(MATRIX_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
	glEnableVertexAttribArray(MATRIX_ATTRIBUTE_POSITION);
	/*glVertexAttribPointer(MATRIX_ATTRIBUTE_COLOR, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, color));
	glEnableVertexAttribArray(MATRIX_ATTRIBUTE_COLOR);*/
	glVertexAttribPointer(MATRIX_ATTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));
	glEnableVertexAttribArray(MATRIX_ATTRIBUTE_NORMAL);
	glVertexAttribPointer(MATRIX_ATTRIBUTE_TEXTURE0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, tex_coords));
	glEnableVertexAttribArray(MATRIX_ATTRIBUTE_TEXTURE0);
	glEnableVertexAttribArray(MATRIX_ATTRIBUTE_TANGENT);
	glVertexAttribPointer(MATRIX_ATTRIBUTE_TANGENT, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Tangent));
		//// vertex bitangent
	glEnableVertexAttribArray(MATRIX_ATTRIBUTE_BITANGENT);
	glVertexAttribPointer(MATRIX_ATTRIBUTE_BITANGENT, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Bitangent));
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	
	glBindBuffer(GL_ARRAY_BUFFER, VBO_bones);
	//bones data
	glVertexAttribIPointer(MATRIX_ATTRIBUTE_ID, 4, GL_INT, sizeof(VertexBoneData), (GLvoid*)0);
	glEnableVertexAttribArray(MATRIX_ATTRIBUTE_ID);
	glVertexAttribPointer(MATRIX_ATTRIBUTE_WEIGHTS, 4, GL_FLOAT, GL_FALSE, sizeof(VertexBoneData), (GLvoid*)offsetof(VertexBoneData, weights));
	glEnableVertexAttribArray(MATRIX_ATTRIBUTE_WEIGHTS);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_indices);
	glBindVertexArray(0);

	


}

void Mesh::Draw(GLuint shaders_program)
{
	unsigned int diffuse_nr = 1;
	unsigned int specular_nr = 1;
	unsigned int normalNr = 1;
	unsigned int heightNr = 1;

	bool isTexturePresent = false;
	for (int i = 0; i < textures.size(); i++)
	{
		isTexturePresent = true;
		glActiveTexture(GL_TEXTURE0 + i);
		std::string number;
		std::string name = textures[i].type;

		/*fopen_s(&gpFile, "Log.txt", "a+");
		fprintf(gpFile, "Name = %s \n", name.c_str());
		fclose(gpFile);*/

		if (name == "texture_diffuse")
			number = std::to_string(diffuse_nr++);
		else if (name == "texture_specular")
			number = std::to_string(specular_nr++); // transfer unsigned int to stream
		else if (name == "texture_normal")
			number = std::to_string(normalNr++); // transfer unsigned int to stream
		else if (name == "texture_height")
			number = std::to_string(heightNr++); // transfer unsigned int to stream


		glBindTexture(GL_TEXTURE_2D, textures[i].id);

		glUniform1i(glGetUniformLocation(shaders_program, (name + number).c_str()), i);
	}

	for (int i = 0; i < materials.size(); i++)
	{
		std::string name = materials[i].type;
		if ((name == "material_ambient") || (name == "material_diffuse") || (name == "material_specular"))
		{
			glUniform3fv(glGetUniformLocation(shaders_program, (name).c_str()), 1, materials[i].Value);
		}
	}
	
	if(isTexturePresent == true)
		glUniform1i(glGetUniformLocation(shaders_program, "u_is_texture"), 1);
	else
		glUniform1i(glGetUniformLocation(shaders_program, "u_is_texture"), 0);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_Vertices);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	
	for (int i = 0; i < textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}