#pragma once
#include <map>

#include "Mesh.h"

/*#define STB_IMAGE_IMPLEMENTATION
#include "../../../stb/stb_image.h"
*/


#include "../../../ExternalLibraries/Assimp/include/assimp/scene.h"
#include "../../../ExternalLibraries/Assimp/include/assimp/postprocess.h"



unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma);

class Model
{
public:
	Model();
	~Model();
	static const unsigned int MAX_BONES = 100;

	void initShaders(GLuint shader_program);
	void loadModel(const std::string& path);
	//void update();
	void draw(GLuint shaders_program, bool isAnimated, int sequence);
	void showNodeName(aiNode* node);

	glm::mat4 aiToGlm(aiMatrix4x4 ai_matr);
	aiQuaternion nlerp(aiQuaternion a, aiQuaternion b, float blend);

private:
	Assimp::Importer import;
	const aiScene* scene;
	std::vector<Mesh> meshes; //one mesh in one object
	std::string directory;

	std::map<std::string, unsigned int> m_bone_mapping; //maps a bone name and their index
	unsigned int m_num_bones = 0;
	std::vector<BoneMatrix> m_bone_matrices;
	aiMatrix4x4 m_global_inverse_transform;

	GLuint m_bone_location[MAX_BONES];
	float ticks_per_second = 0.0f;


	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string type_name);
	std::vector<Material> loadMaterialColor(aiMaterial* mat, const char* type, int one, int two, std::string typeName);


	int findPosition(float p_animation_time, const aiNodeAnim* p_node_anim);
	int findRotation(float p_animation_time, const aiNodeAnim* p_node_anim);
	int findScaling(float p_animation_time, const aiNodeAnim* p_node_anim);

	const aiNodeAnim* findNodeAnim(const aiAnimation* p_animation, const std::string p_node_name);
	// calculate transform matrix
	aiVector3D calcInterpolatedPosition(float p_animation_time, const aiNodeAnim* p_node_anim);
	aiQuaternion calcInterpolatedRotation(float p_animation_time, const aiNodeAnim* p_node_anim);
	aiVector3D calcInterpolatedScaling(float p_animation_time, const aiNodeAnim* p_node_anim);

	void readNodeHierarchy(float p_animation_time, const aiNode* p_node, const aiMatrix4x4 parent_transform);
	void boneTransform(double time_in_sec, std::vector<aiMatrix4x4>& transforms);


};

Model::Model()
{
	scene = nullptr;
}


Model::~Model()
{
	import.FreeScene();
}

void Model::initShaders(GLuint shader_program)
{
	for (int i = 0; i < MAX_BONES; i++) // get location all matrices of bones
	{
		std::string name = "gBones[" + std::to_string(i) + "]";// name like in shader
		m_bone_location[i] = glGetUniformLocation(shader_program, name.c_str());
	}
}


static float count1 = 0.1f;


void Model::draw(GLuint shaders_program, bool isAnimated, int sequence)
{
	std::vector<aiMatrix4x4> transforms;

	/*if(count1 < 900.0f)
	boneTransform((double)timer.getTime() / 10000.0f, transforms);*/

	if (sequence == 1)
	{
		if (count1 < 1.6f)
			boneTransform(count1, transforms);
	}
	else
	{
		if (count1 < 1.2f)
			boneTransform(count1, transforms);
	}
	if (isAnimated)
	{
		if (sequence == 1)
		{
			count1 = count1 + 0.01f;
		}
		else
		{
			count1 = count1 + 0.005f;
		}
	}
	else
		count1 = 0.0f;

	for (int i = 0; i < transforms.size(); i++) // move all matrices for actual model position to shader
	{
		glUniformMatrix4fv(m_bone_location[i], 1, GL_TRUE, (const GLfloat*)&transforms[i]);
	}


	for (int i = 0; i < meshes.size(); i++)
	{
		meshes[i].Draw(shaders_program);
	}
}


void Model::loadModel(const std::string& path)
{
	/*fopen_s(&gpFile, "Log.txt", "a+");
	fprintf(gpFile, "Enter LoadMyModel\n");
	fclose(gpFile);*/
	scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
	if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		fprintf(gpFile, "ERROR::ASSIMP::MODEL::ReadFile failed = %s\n", import.GetErrorString());
		fflush(gpFile);
		return;
	}

	fprintf(gpFile, "Success::ASSIMP::MODEL::ReadFile success\n");
	fflush(gpFile);

	m_global_inverse_transform = scene->mRootNode->mTransformation;
	m_global_inverse_transform.Inverse();

	if (scene->mAnimations[0]->mTicksPerSecond != 0.0)
	{
		ticks_per_second = scene->mAnimations[0]->mTicksPerSecond;
	}
	else
	{
		ticks_per_second = 25.0f;
	}

	
	directory = path.substr(0, path.find_last_of('/'));

	fprintf(gpFile, "ASSIMP::MODEL:: Directory-> %s\n", directory.c_str());
	fprintf(gpFile, "scene->HasAnimations() 1: %d\n", scene->HasAnimations());
	fprintf(gpFile, "scene->mNumMeshes 1: %d\n", scene->mNumMeshes);
	fprintf(gpFile, "scene->mAnimations[0]->mNumChannels 1: %d\n", scene->mAnimations[0]->mNumChannels);
	fprintf(gpFile, "scene->mAnimations[0]->mDuration 1: %f\n", scene->mAnimations[0]->mDuration);
	fprintf(gpFile, "scene->mAnimations[0]->mTicksPerSecond 1:  %f\n", scene->mAnimations[0]->mTicksPerSecond);

	fprintf(gpFile, "		name nodes : \n");
	fflush(gpFile);

	showNodeName(scene->mRootNode);

	processNode(scene->mRootNode, scene);

	for (int i = 0; i < scene->mAnimations[0]->mNumChannels; i++)
	{
		fprintf(gpFile, "nodes animation name : %s \n", scene->mAnimations[0]->mChannels[i]->mNodeName.C_Str());
		fflush(gpFile);
	}
	
}

void Model::showNodeName(aiNode* node)
{
	fprintf(gpFile, "Data : %s\n", node->mName.data);
	fflush(gpFile);
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		showNodeName(node->mChildren[i]);
	}
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
	Mesh mesh;
	for (int i = 0; i < scene->mNumMeshes; i++)
	{
		aiMesh* aiMesh = scene->mMeshes[i];
		mesh = processMesh(aiMesh, scene);
		meshes.push_back(mesh);
	}


}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
	fprintf(gpFile, "bones: %d , vertices: %d\n", mesh->mNumBones , mesh->mNumVertices );
	fflush(gpFile);

	std::vector<Vertex> vertices;
	std::vector<GLuint>indices;
	std::vector<Texture> textures;
	std::vector<Material>materials;
	std::vector<VertexBoneData> bones_id_weights_for_each_vertex;

	vertices.reserve(mesh->mNumVertices);
	indices.reserve(mesh->mNumVertices);

	bones_id_weights_for_each_vertex.resize(mesh->mNumVertices);

	//vertices
	for (int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		glm::vec3 vector1;
		vector1.x = mesh->mVertices[i].x;
		vector1.y = mesh->mVertices[i].y;
		vector1.z = mesh->mVertices[i].z;
		vertex.position = vector1;

		/*glm::vec4 vector2;
		vector2.x = 1.0;
		vector2.y = 1.0;
		vector2.z = 1.0;
		vector2.w = 1.0;
		vertex.color = vector2;*/

		if (mesh->mNormals != NULL)
		{
			vector1.x = mesh->mNormals[i].x;
			vector1.y = mesh->mNormals[i].y;
			vector1.z = mesh->mNormals[i].z;
			vertex.normal = vector1;
		}
		else
		{
			vertex.normal = glm::vec3();
		}
		// texture coordinates
		if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
		{
			glm::vec2 vec;
			vec[0] = mesh->mTextureCoords[0][i].x;
			vec[1] = mesh->mTextureCoords[0][i].y;
			vertex.tex_coords = vec;
		}
		else
			vertex.tex_coords = glm::vec2(0.0f, 0.0f);


		vector1.x = mesh->mTangents[i].x;
		vector1.y = mesh->mTangents[i].y;
		vector1.z = mesh->mTangents[i].z;
		vertex.Tangent = vector1;

		vector1.x = mesh->mBitangents[i].x;
		vector1.y = mesh->mBitangents[i].y;
		vector1.z = mesh->mBitangents[i].z;
		vertex.Bitangent = vector1;

		vertices.push_back(vertex);

	}

	//indices
	for (int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		indices.push_back(face.mIndices[0]);
		indices.push_back(face.mIndices[1]);
		indices.push_back(face.mIndices[2]);
	}

	
	//material
	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		// 1. diffuse maps
		std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		// 2. specular maps
		std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
		// 3. normal maps
		std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
		textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
		// 4. height maps
		std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
		textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());



		//ambient
		std::vector<Material> MaterialAmbientColorMaps = loadMaterialColor(material, AI_MATKEY_COLOR_AMBIENT, "material_ambient");
		materials.insert(materials.end(), MaterialAmbientColorMaps.begin(), MaterialAmbientColorMaps.end());
		//diffuse
		std::vector<Material> MaterialDiffuseColorMaps = loadMaterialColor(material, AI_MATKEY_COLOR_DIFFUSE, "material_diffuse");
		materials.insert(materials.end(), MaterialDiffuseColorMaps.begin(), MaterialDiffuseColorMaps.end());
		//specular
		std::vector<Material> MaterialSpecularColorMaps = loadMaterialColor(material, AI_MATKEY_COLOR_SPECULAR, "material_specular");
		materials.insert(materials.end(), MaterialSpecularColorMaps.begin(), MaterialSpecularColorMaps.end());
		
	}

	//load bones
	for (int i = 0; i < mesh->mNumBones; i++)
	{
		int bone_index = 0;
		std::string bone_name(mesh->mBones[i]->mName.data);

		fprintf(gpFile, "MODEL::%s \n", mesh->mBones[i]->mName.data);
		fflush(gpFile);

		if (m_bone_mapping.find(bone_name) == m_bone_mapping.end())
		{
			//allocate an index for a new bone
			bone_index = m_num_bones;
			m_num_bones++;
			BoneMatrix bi;
			m_bone_matrices.push_back(bi);
			m_bone_matrices[bone_index].offset_matrix = mesh->mBones[i]->mOffsetMatrix;
			m_bone_mapping[bone_name] = bone_index;
		}
		else
		{
			bone_index = m_bone_mapping[bone_name];
		}

		for (int j = 0; j < mesh->mBones[i]->mNumWeights; j++)
		{
			int vertex_id = mesh->mBones[i]->mWeights[j].mVertexId;
			float weight = mesh->mBones[i]->mWeights[j].mWeight;
			bones_id_weights_for_each_vertex[vertex_id].addBoneData(bone_index, weight);
		}
	}

	return Mesh(vertices, indices, textures, materials, bones_id_weights_for_each_vertex);
}



std::vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string type_name)
{
	std::vector<Texture> textures;
	for (int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString ai_str;
		mat->GetTexture(type, i, &ai_str);

		std::string filename = std::string(ai_str.C_Str());
		filename = directory + '/' + filename;

		Texture texture;
		//texture.id = TextureFromFile(filename.c_str()); // return prepaired openGL texture
		texture.id = TextureFromFile(ai_str.C_Str(), this->directory, true); // return prepaired openGL texture
		texture.type = type_name;
		texture.path = ai_str;
		textures.push_back(texture);

	}
	return textures;
}

std::vector<Material> Model::loadMaterialColor(aiMaterial* mat, const char* type, int one, int two, std::string typeName)
{
	std::vector<Material> materials;

	aiColor3D color;
	mat->Get(type, one, two, color);

	Material materialInfo;
	materialInfo.type = typeName;
	materialInfo.Value[0] = color[0];
	materialInfo.Value[1] = color[1];
	materialInfo.Value[2] = color[2];
	materials.push_back(materialInfo);
	return materials;
}


int Model::findPosition(float p_animation_time, const aiNodeAnim* p_node_anim)
{
	for (int i = 0; i < p_node_anim->mNumPositionKeys - 1; i++) 
	{
		if (p_animation_time < (float)p_node_anim->mPositionKeys[i + 1].mTime) 
		{
			return i; 
		}
	}

	assert(0);
	return 0;
}

int Model::findRotation(float p_animation_time, const aiNodeAnim* p_node_anim)
{
	
	for (int i = 0; i < p_node_anim->mNumRotationKeys - 1; i++)
	{
		if (p_animation_time < (float)p_node_anim->mRotationKeys[i + 1].mTime)
		{
			return i;
		}
	}

	assert(0);
	return 0;
}

int Model::findScaling(float p_animation_time, const aiNodeAnim* p_node_anim)
{
	for (int i = 0; i < p_node_anim->mNumScalingKeys - 1; i++) 
	{
		if (p_animation_time < (float)p_node_anim->mScalingKeys[i + 1].mTime)
		{
			return i;
		}
	}

	assert(0);
	return 0;
}

aiVector3D Model::calcInterpolatedPosition(float p_animation_time, const aiNodeAnim* p_node_anim)
{
	if (p_node_anim->mNumPositionKeys == 1)
	{
		return p_node_anim->mPositionKeys[0].mValue;
	}

	int position_index = findPosition(p_animation_time, p_node_anim);
	int next_position_index = position_index + 1; 
	
	assert(next_position_index < p_node_anim->mNumPositionKeys);
	
	float delta_time = (float)(p_node_anim->mPositionKeys[next_position_index].mTime - p_node_anim->mPositionKeys[position_index].mTime);
	
	float factor = (p_animation_time - (float)p_node_anim->mPositionKeys[position_index].mTime) / delta_time;
	
	assert(factor >= 0.0f && factor <= 1.0f);
	aiVector3D start = p_node_anim->mPositionKeys[position_index].mValue;
	aiVector3D end = p_node_anim->mPositionKeys[next_position_index].mValue;
	aiVector3D delta = end - start;

	return start + factor * delta;
}

aiQuaternion Model::calcInterpolatedRotation(float p_animation_time, const aiNodeAnim* p_node_anim)
{
	if (p_node_anim->mNumRotationKeys == 1) 
	{
		return p_node_anim->mRotationKeys[0].mValue;
	}

	int rotation_index = findRotation(p_animation_time, p_node_anim);
	int next_rotation_index = rotation_index + 1;
	assert(next_rotation_index < p_node_anim->mNumRotationKeys);
	
	float delta_time = (float)(p_node_anim->mRotationKeys[next_rotation_index].mTime - p_node_anim->mRotationKeys[rotation_index].mTime);
	
	float factor = (p_animation_time - (float)p_node_anim->mRotationKeys[rotation_index].mTime) / delta_time;

	//cout << "p_node_anim->mRotationKeys[rotation_index].mTime: " << p_node_anim->mRotationKeys[rotation_index].mTime << endl;
	//cout << "p_node_anim->mRotationKeys[next_rotaion_index].mTime: " << p_node_anim->mRotationKeys[next_rotation_index].mTime << endl;
	//cout << "delta_time: " << delta_time << endl;
	//cout << "animation_time: " << p_animation_time << endl;
	//cout << "animation_time - mRotationKeys[rotation_index].mTime: " << (p_animation_time - (float)p_node_anim->mRotationKeys[rotation_index].mTime) << endl;
	//cout << "factor: " << factor << endl << endl << endl;

	assert(factor >= 0.0f && factor <= 1.0f);
	aiQuaternion start_quat = p_node_anim->mRotationKeys[rotation_index].mValue;
	aiQuaternion end_quat = p_node_anim->mRotationKeys[next_rotation_index].mValue;

	return nlerp(start_quat, end_quat, factor);
}

aiVector3D Model::calcInterpolatedScaling(float p_animation_time, const aiNodeAnim* p_node_anim)
{
	if (p_node_anim->mNumScalingKeys == 1) 
	{
		return p_node_anim->mScalingKeys[0].mValue;
	}

	int scaling_index = findScaling(p_animation_time, p_node_anim);
	int next_scaling_index = scaling_index + 1; 
	assert(next_scaling_index < p_node_anim->mNumScalingKeys);

	float delta_time = (float)(p_node_anim->mScalingKeys[next_scaling_index].mTime - p_node_anim->mScalingKeys[scaling_index].mTime);
	
	float  factor = (p_animation_time - (float)p_node_anim->mScalingKeys[scaling_index].mTime) / delta_time;
	assert(factor >= 0.0f && factor <= 1.0f);
	aiVector3D start = p_node_anim->mScalingKeys[scaling_index].mValue;
	aiVector3D end = p_node_anim->mScalingKeys[next_scaling_index].mValue;
	aiVector3D delta = end - start;

	return start + factor * delta;
}

const aiNodeAnim* Model::findNodeAnim(const aiAnimation* p_animation, const std::string p_node_name)
{
	// channel in animation contains aiNodeAnim (aiNodeAnim its transformation for bones)
	// numChannels == numBones
	for (int i = 0; i < p_animation->mNumChannels; i++)
	{
		const aiNodeAnim* node_anim = p_animation->mChannels[i];
		if (std::string(node_anim->mNodeName.data) == p_node_name)
		{
			return node_anim;
		}
	}

	return nullptr;
}
// start from RootNode
void Model::readNodeHierarchy(float p_animation_time, const aiNode* p_node, const aiMatrix4x4 parent_transform)
{

	std::string node_name(p_node->mName.data);

	
	const aiAnimation* animation = scene->mAnimations[0];
	aiMatrix4x4 node_transform = p_node->mTransformation;

	const aiNodeAnim* node_anim = findNodeAnim(animation, node_name);

	if (node_anim)
	{

		//scaling
		//aiVector3D scaling_vector = node_anim->mScalingKeys[2].mValue;
		aiVector3D scaling_vector = calcInterpolatedScaling(p_animation_time, node_anim);
		aiMatrix4x4 scaling_matr;
		aiMatrix4x4::Scaling(scaling_vector, scaling_matr);

		//rotation
		//aiQuaternion rotate_quat = node_anim->mRotationKeys[2].mValue;
		aiQuaternion rotate_quat = calcInterpolatedRotation(p_animation_time, node_anim);
		aiMatrix4x4 rotate_matr = aiMatrix4x4(rotate_quat.GetMatrix());

		//translation
		//aiVector3D translate_vector = node_anim->mPositionKeys[2].mValue;
		aiVector3D translate_vector = calcInterpolatedPosition(p_animation_time, node_anim);
		aiMatrix4x4 translate_matr;
		aiMatrix4x4::Translation(translate_vector, translate_matr);

		node_transform = translate_matr * rotate_matr * scaling_matr;

	}

	aiMatrix4x4 global_transform = parent_transform * node_transform;

	
	if (m_bone_mapping.find(node_name) != m_bone_mapping.end()) // true if node_name exist in bone_mapping
	{
		int bone_index = m_bone_mapping[node_name];
		m_bone_matrices[bone_index].final_world_transform = m_global_inverse_transform * global_transform * m_bone_matrices[bone_index].offset_matrix;
	}

	for (int i = 0; i < p_node->mNumChildren; i++)
	{
		readNodeHierarchy(p_animation_time, p_node->mChildren[i], global_transform);
	}

}

void Model::boneTransform(double time_in_sec, std::vector<aiMatrix4x4>& transforms)
{
	aiMatrix4x4 identity_matrix; // = mat4(1.0f);

	double time_in_ticks = time_in_sec * ticks_per_second;
	float animation_time = fmod(time_in_ticks, scene->mAnimations[0]->mDuration);

	readNodeHierarchy(animation_time, scene->mRootNode, identity_matrix);

	transforms.resize(m_num_bones);

	for (int i = 0; i < m_num_bones; i++)
	{
		transforms[i] = m_bone_matrices[i].final_world_transform;
	}
}

glm::mat4 Model::aiToGlm(aiMatrix4x4 ai_matr)
{
	glm::mat4 result;
	result[0].x = ai_matr.a1; result[0].y = ai_matr.b1; result[0].z = ai_matr.c1; result[0].w = ai_matr.d1;
	result[1].x = ai_matr.a2; result[1].y = ai_matr.b2; result[1].z = ai_matr.c2; result[1].w = ai_matr.d2;
	result[2].x = ai_matr.a3; result[2].y = ai_matr.b3; result[2].z = ai_matr.c3; result[2].w = ai_matr.d3;
	result[3].x = ai_matr.a4; result[3].y = ai_matr.b4; result[3].z = ai_matr.c4; result[3].w = ai_matr.d4;

	return result;
}

aiQuaternion Model::nlerp(aiQuaternion a, aiQuaternion b, float blend)
{
	//cout << a.w + a.x + a.y + a.z << endl;
	a.Normalize();
	b.Normalize();

	aiQuaternion result;
	float dot_product = a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
	float one_minus_blend = 1.0f - blend;

	if (dot_product < 0.0f)
	{
		result.x = a.x * one_minus_blend + blend * -b.x;
		result.y = a.y * one_minus_blend + blend * -b.y;
		result.z = a.z * one_minus_blend + blend * -b.z;
		result.w = a.w * one_minus_blend + blend * -b.w;
	}
	else
	{
		result.x = a.x * one_minus_blend + blend * b.x;
		result.y = a.y * one_minus_blend + blend * b.y;
		result.z = a.z * one_minus_blend + blend * b.z;
		result.w = a.w * one_minus_blend + blend * b.w;
	}

	return result.Normalize();
}


unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma)
{
	std::string filename = std::string(path);
	filename = directory + '/' + filename;


	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glBindTexture(GL_TEXTURE_2D, 0);
		stbi_image_free(data);

		fprintf(gpFile, "Texture load at path : %s \n", path);
		fflush(gpFile);
	}
	else
	{

		fprintf(gpFile, "Texture failed to load at path : %s \n", path);
		fflush(gpFile);

		stbi_image_free(data);
	}

	return textureID;
}