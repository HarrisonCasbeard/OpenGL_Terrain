#include "Model.h"

#include <glew\glew.h>
#include <assimp\Importer.hpp>
#include <assimp\postprocess.h>
#include "stb_image.h"

Model::Model(const char* path)
{
	loadModel(path);
	findDimensions();
}

Model::Model()
{
}

Model::~Model()
{
}
//-------------------------------------------------------------------------------------------------
void Model::draw(Shader& shader)
{
	for (unsigned int i = 0; i < m_vMeshes.size(); i++)
		m_vMeshes[i].draw(shader);
}
//-------------------------------------------------------------------------------------------------
bool Model::loadModel(const std::string& path)
{
	//Load in the file - Flips the UV's and triangulate the verts
	Assimp::Importer importer;
	//Other convenient post-processing : genNormals, SplitLargeMeshes, OptimizeMeshes
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		printf("FAILED TO LOAD MODEL : %s!", path.c_str());
		return false;
	}
	m_directory = path.substr(0, path.find_last_of('/'));

	processNode(*scene->mRootNode, *scene);
	
	return true;
}
//-------------------------------------------------------------------------------------------------
void Model::processNode(const aiNode& node, const aiScene& scene)
{
	//Loop through all of this nodes meshes
	for (unsigned int curMesh = 0; curMesh < node.mNumMeshes; curMesh++)
	{
		aiMesh* mesh = scene.mMeshes[node.mMeshes[curMesh]];
		m_vMeshes.push_back(processMesh(*mesh, scene));
	}

	//Loop through all of this nodes children
	for (unsigned int child = 0; child < node.mNumChildren; child++)
	{
		processNode(*node.mChildren[child], scene);
	}
}
//-------------------------------------------------------------------------------------------------
Mesh Model::processMesh(const aiMesh& mesh, const aiScene& scene)
{
	Mesh tmp;

	//Process all the vertices
	for (unsigned int v = 0; v < mesh.mNumVertices; v++)
	{
		Vertex vert;

		vert.position = glm::vec3(mesh.mVertices[v].x, mesh.mVertices[v].y, mesh.mVertices[v].z);
		vert.normal	  = glm::vec3(mesh.mNormals[v].x,  mesh.mNormals[v].y,  mesh.mNormals[v].z);

		if(mesh.mTextureCoords[0])
			vert.uv = glm::vec2(mesh.mTextureCoords[0][v].x, mesh.mTextureCoords[0][v].y);
		else
			vert.uv = glm::vec2(0.0f, 0.0f);
		
		tmp.m_vVertices.push_back(vert);
	}

	//Process all the indices
	for (unsigned int i = 0; i < mesh.mNumFaces; i++)
	{
		aiFace face = mesh.mFaces[i];
		for (unsigned int ind = 0; ind < face.mNumIndices; ind++)
			tmp.m_vIndices.push_back(face.mIndices[ind]);
	}

	//Process all the textures
	if (mesh.mMaterialIndex >= 0)
	{
		aiMaterial* material = scene.mMaterials[mesh.mMaterialIndex];
		loadMaterialTextures(*material, aiTextureType_DIFFUSE, "textureDiffuse", tmp.m_vTextures);
		loadMaterialTextures(*material, aiTextureType_SPECULAR, "textureSpecular", tmp.m_vTextures);
	}

	tmp.setupMesh();
	return tmp;
}
//-------------------------------------------------------------------------------------------------
void Model::loadMaterialTextures(const aiMaterial& mat, aiTextureType texType, const std::string& typeName, std::vector<Texture>& textures)
{
	for (unsigned int i = 0; i < mat.GetTextureCount(texType); i++)
	{
		aiString path;
		mat.GetTexture(texType, i, &path);

		bool skip = false;
		for (unsigned int j = 0; j < m_vLoadedTextures.size(); j++)
		{
			if (m_vLoadedTextures[j].path == path)
			{
				textures.push_back(m_vLoadedTextures[j]);
				skip = true;
				break;
			}
		}

		if (!skip)
		{
			Texture tex;
			tex.id = textureFromFile(path.C_Str(), m_directory.c_str());
			tex.type = typeName;
			tex.path = path;
			textures.push_back(tex);
		}
	}
}
//-------------------------------------------------------------------------------------------------
unsigned int Model::textureFromFile(const char* path, const std::string& directory, bool gamma)
{
	std::string filename = std::string(path);
	filename = directory + "/" + filename;

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
		//When MINifying the image, use a LINEAR blend of two mipmaps, each filtered LINEARLY too
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		//When MAGnifying the image (no bigger mipmap available) use . . .
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		printf("FAILED TO LOAD TEXTURE : %s \n", path);
		stbi_image_free(data);
	};

	return textureID;
}
//-------------------------------------------------------------------------------------------------
void Model::findDimensions()
{
	glm::vec3 min(INT_MAX);
	glm::vec3 max(-INT_MAX);

	for (unsigned int mesh = 0; mesh < m_vMeshes.size(); mesh++)
	{
		Mesh tmp = m_vMeshes[mesh];
		for (unsigned int vert = 0; vert < tmp.m_vVertices.size(); vert++)
		{
			if (tmp.m_vVertices[vert].position.x < min.x)	min.x = tmp.m_vVertices[vert].position.x;
			if (tmp.m_vVertices[vert].position.y < min.y)	min.y = tmp.m_vVertices[vert].position.y;
			if (tmp.m_vVertices[vert].position.z < min.z)	min.z = tmp.m_vVertices[vert].position.z;

			if (tmp.m_vVertices[vert].position.x > max.x)	max.x = tmp.m_vVertices[vert].position.x;
			if (tmp.m_vVertices[vert].position.y > max.y)	max.y = tmp.m_vVertices[vert].position.y;
			if (tmp.m_vVertices[vert].position.z > max.z)	max.z = tmp.m_vVertices[vert].position.z;
		}
	}

	m_dimensions = max - min;
}
//-------------------------------------------------------------------------------------------------
const glm::vec3& Model::dimensions() const
{
	return m_dimensions;
}