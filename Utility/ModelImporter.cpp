#include "stdafx.h"
#include "ModelImporter.h"


ModelImporter::ModelImporter(ID3D11Device* pDevice, std::string& filename)
{
    this->pDevice = pDevice;
    this->filename = filename;
    assimp_flag =
        aiProcess_CalcTangentSpace |
        aiProcess_GenSmoothNormals |
        aiProcess_JoinIdenticalVertices |
        aiProcess_OptimizeMeshes |
        aiProcess_ImproveCacheLocality |
        aiProcess_LimitBoneWeights |
        aiProcess_SplitLargeMeshes |
        aiProcess_Triangulate |
        aiProcess_GenUVCoords |
        aiProcess_SortByPType |
        aiProcess_FindDegenerates |
        aiProcess_FindInvalidData |
        aiProcess_FindInstances |
        aiProcess_ValidateDataStructure |
        aiProcess_Debone |
        aiProcess_ConvertToLeftHanded;
    Assimp::Importer importer;
    Scene = importer.ReadFile(filename, assimp_flag);
    if (!Scene)
        return;

    ParseNode(Scene->mRootNode, Scene, DirectX::XMMatrixIdentity());
}

ModelImporter::~ModelImporter()
{
}

void ModelImporter::ParseNode(aiNode* node, const aiScene* scene, const DirectX::XMMATRIX& parentTransformMatrix)
{
    DirectX::XMMATRIX nodetransfromMatrix = DirectX::XMMatrixTranspose(DirectX::XMMATRIX(&node->mTransformation.a1)) * parentTransformMatrix;

    for (int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        Meshes.push_back(ParseMesh(mesh,scene, nodetransfromMatrix));
    }

    for (int i = 0; i < node->mNumChildren; i++)
    {
        ParseNode(node->mChildren[i], scene, nodetransfromMatrix);
    }
}

Mesh ModelImporter::ParseMesh(aiMesh* mesh, const aiScene* scene, const DirectX::XMMATRIX& TransformMatrix)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned short> indices;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;

        vertex.pos.x = mesh->mVertices[i].x;
        vertex.pos.y = mesh->mVertices[i].y;
        vertex.pos.z = mesh->mVertices[i].z;

        vertex.normal.x = mesh->mNormals[i].x;
        vertex.normal.y = mesh->mNormals[i].y;
        vertex.normal.z = mesh->mNormals[i].z;


        if (mesh->mTextureCoords[0])
        {
            vertex.uv.x = static_cast<float>(mesh->mTextureCoords[0][i].x);
            vertex.uv.y = static_cast<float>(mesh->mTextureCoords[0][i].y);
        }

        vertices.push_back(vertex);
    }

    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];

        for (unsigned int i = 0; i < face.mNumIndices; i++)
            indices.push_back(face.mIndices[i]);
    }
   
    std::vector<Texture2D> textures;
    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
    std::vector<Texture2D> diffuseTextures = LoadMaterialTextures(material, aiTextureType::aiTextureType_DIFFUSE, scene);
    textures.insert(textures.end(), diffuseTextures.begin(), diffuseTextures.end());
    aiTextureType b;
    Mesh temp(textures);
    temp.SetVerteices(vertices);
    temp.SetIndices(indices);
    temp.SetMatrix(TransformMatrix);
    return temp;
}

std::vector<Texture2D> ModelImporter::LoadMaterialTextures(aiMaterial* material, aiTextureType type, const aiScene* scene)
{
    std::vector<Texture2D> materialTextures;
    TextureStorageType storetype = TextureStorageType::Invalid;
    unsigned int textureCount = material->GetTextureCount(type);
    UINT bind_flag = 0;
    if (textureCount == 0) //텍스쳐가 없다면
    {
        storetype = TextureStorageType::None;
        aiColor3D aiColor(0.0f, 0.0f, 0.0f);
        switch (type)
        {
        case aiTextureType_DIFFUSE:
            bind_flag |= ResourceViewFlags::ResourceView_SRV;
            //머테리얼의 Diffuse 색을 얻어옴
            material->Get(AI_MATKEY_COLOR_DIFFUSE, aiColor);
            if (aiColor.IsBlack()) //If color = black, just use grey
            {
                materialTextures.push_back(Texture2D(pDevice, Bytes::UnLoadedTextureDatas, 1, 1, type, bind_flag));
                return materialTextures;
            }
            std::vector<std::byte> bytes{ std::byte{aiColor.r * 255},std::byte{aiColor.g * 255},std::byte{aiColor.b * 255} };
            materialTextures.push_back(Texture2D(pDevice, bytes, 1, 1, type, bind_flag));
            return materialTextures;
        }
    }
    else
    {
        for (UINT i = 0; i < textureCount; i++)
        {
            aiString path;
            material->GetTexture(type, i, &path);
            TextureStorageType storetype = DetermineTextureStorageType(scene, material, i, type);
            switch (storetype)
            {
            case TextureStorageType::EmbeddedIndexCompressed:
            {
                int index = GetTextureIndex(&path);
                Texture2D embeddedIndexedTexture(this->pDevice,
                    reinterpret_cast<uint8_t*>(scene->mTextures[index]->pcData),
                    scene->mTextures[index]->mWidth,
                    type);
                materialTextures.push_back(embeddedIndexedTexture);
                break;
            }
            case TextureStorageType::EmbeddedCompressed:
            {
                const aiTexture* pTexture = scene->GetEmbeddedTexture(path.C_Str());
                Texture2D embeddedTexture(this->pDevice,
                    reinterpret_cast<uint8_t*>(pTexture->pcData),
                    pTexture->mWidth,
                    type);
                materialTextures.push_back(embeddedTexture);
                break;
            }
            case TextureStorageType::Disk:
            {
                std::string filename = StringHelper::GetDirectoryFromPath(this->filename) + path.C_Str();
                Texture2D diskTexture(this->pDevice, filename, type);
                materialTextures.push_back(diskTexture);
                break;
            }
            }
        }
    }
    //텍스쳐가 하나도 생성이 안됐을경우
    if (materialTextures.size() == 0)
    {
        bind_flag |= ResourceViewFlags::ResourceView_SRV;
        materialTextures.push_back(Texture2D(pDevice, Bytes::UnHandledTextureDatas, 1, 1, aiTextureType::aiTextureType_DIFFUSE, bind_flag));
    }
     return materialTextures;
}

TextureStorageType ModelImporter::DetermineTextureStorageType(const aiScene* scene, aiMaterial* material, UINT index, aiTextureType type)
{
    if (material->GetTextureCount(type) == 0)
        return TextureStorageType::None;

    aiString path;
    material->GetTexture(type, index, &path);
    std::string texturePath = path.C_Str();
    //Check if texture is an embedded indexed texture by seeing if the file path is an index #
    if (texturePath[0] == '*')
    {
        if (scene->mTextures[0]->mHeight == 0)
        {
            return TextureStorageType::EmbeddedIndexCompressed;
        }
        else
        {
            assert("SUPPORT DOES NOT EXIST YET FOR INDEXED NON COMPRESSED TEXTURES!" && 0);
            return TextureStorageType::EmbeddedIndexNonCompressed;
        }
    }
    //Check if texture is an embedded texture but not indexed (path will be the texture's name instead of #)
    if (auto pTex = scene->GetEmbeddedTexture(texturePath.c_str()))
    {
        if (pTex->mHeight == 0)
        {
            return TextureStorageType::EmbeddedCompressed;
        }
        else
        {
            assert("SUPPORT DOES NOT EXIST YET FOR EMBEDDED NON COMPRESSED TEXTURES!" && 0);
            return TextureStorageType::EmbeddedNonCompressed;
        }
    }
    //Lastly check if texture is a filepath by checking for period before extension name
    if (texturePath.find('.') != std::string::npos)
    {
        return TextureStorageType::Disk;
    }

    return TextureStorageType::None; // No texture exists
}

int ModelImporter::GetTextureIndex(aiString* pStr)
{
    assert(pStr->length >= 2);
    return atoi(&pStr->C_Str()[1]);
}




