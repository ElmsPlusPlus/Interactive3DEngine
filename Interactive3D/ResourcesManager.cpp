#include "ResourcesManager.h"
#include "Framework.h"
#include <assimp.hpp>
#include <aiScene.h>
#include <aiPostProcess.h>
#include <sstream>
#include <assert.h>

#pragma comment(lib, "assimp.lib")

ResourcesManager::ResourcesManager(void)
{
    // Create a default texture for use where none is specified.  If white.png is not available, then
    // the default texture will be null, i.e. black
	if (FAILED(D3DX11CreateShaderResourceViewFromFileA(Framework::This()->Device(),        
			    				  				       "white.png",   
				    								   NULL,          
					    							   NULL,           
						    						   &_defaultTexture,      
							    					   NULL)))
    {
        _defaultTexture = NULL;
    }

	assert(_defaultTexture != NULL);
}

ResourcesManager::~ResourcesManager(void)
{
    SAFE_RELEASE(_defaultTexture);
}

ResourceId ResourcesManager::LoadModel(string modelName)
{
	ResourceId location = FindResource(modelName);	
	if (location < 0)
	{
		// We are loading the model for the first time
		Mesh * mesh = LoadModelFromFile(modelName);
		ResourceStruct resourceInfo;
		resourceInfo.ResourceName = modelName;
		resourceInfo.Resource = (void *)mesh;
		resourceInfo.ReferenceCount = 1;
		return AllocateResourceId(&resourceInfo);
	}
	else
	{
		// Increment the reference count and return the id
		_resources[location].ReferenceCount++;
		return location;
	}
}

Mesh * ResourcesManager::GetMesh(ResourceId id)
{
	if (id >= 0 && id < (int)_resources.size())
	{
		return (Mesh *)_resources[id].Resource;
	}
	else
	{
		return NULL;
	}
}

void ResourcesManager::ReleaseMesh(ResourceId id)
{
	if (id >= 0 && id < (int)_resources.size())
	{
		_resources[id].ReferenceCount--;
		if (_resources[id].ReferenceCount == 0)
		{
			// If no more use of this resource, free up the resource and release the slot
			delete (Mesh *)_resources[id].Resource;
			_resources[id].Resource = NULL;
		}
	}
}

ResourceId ResourcesManager::CreateMaterialFromTexture(string textureName)
{
    // We have no diffuse or specular colours here since we are just building a default material structure
    // based on a provided texture. Just use the texture name as the material name in this case
    return InitialiseMaterial(textureName, 
                              XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 
                              XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f),
                              0,
                              textureName);
}

ResourceId ResourcesManager::CreateMaterialWithNoTexture(string materialName, XMFLOAT4 diffuseColour, XMFLOAT4 specularColour, float shininess)
{
    return InitialiseMaterial(materialName, diffuseColour, specularColour, shininess, "");
}

ResourceId ResourcesManager::CreateMaterial(string materialName, XMFLOAT4 diffuseColour, XMFLOAT4 specularColour, float shininess, string textureName)
{
    return InitialiseMaterial(materialName, diffuseColour, specularColour, shininess, textureName);
}

Material * ResourcesManager::GetMaterial(ResourceId id)
{
	if (id >= 0 && id < (int)_resources.size())
	{
		return (Material *)_resources[id].Resource;
	}
	else
	{
		return NULL;
	}
}

void ResourcesManager::ReleaseMaterial(ResourceId id)
{
	if (id >= 0 && id < (int)_resources.size())
	{
		_resources[id].ReferenceCount--;
		if (_resources[id].ReferenceCount == 0)
		{
			// If no more use of this resource, free up the resource and release the slot
			delete ((Material *)_resources[id].Resource);
			_resources[id].Resource = NULL;
}
	}
}

ID3D11ShaderResourceView * ResourcesManager::GetDefaultTexture(void)
{
    return _defaultTexture;
}

ResourceId ResourcesManager::InitialiseMaterial(string materialName, XMFLOAT4 diffuseColour, XMFLOAT4 specularColour, float shininess, string textureName)
{
	ResourceId location = FindResource(textureName);	
	if (location < 0)
	{
		// We are creating the material for the first time
		ID3D11ShaderResourceView * texture = NULL;
        if (textureName.size() > 0)
        {
		    if (FAILED(D3DX11CreateShaderResourceViewFromFileA(Framework::This()->Device(),        
			    				  							   textureName.c_str(),   
				    										   NULL,          
					    									   NULL,           
						    								   &texture,      
							    							   NULL)))
    		{
	    		return -1;
		    }
        }
        Material * material = new Material(diffuseColour, specularColour, shininess, texture);
		ResourceStruct resourceInfo;
		resourceInfo.ResourceName = materialName;
		resourceInfo.Resource = (void *)material;
		resourceInfo.ReferenceCount = 1;
		return AllocateResourceId(&resourceInfo);
	}
	else
	{
		// Increment the reference count and return the id
		_resources[location].ReferenceCount++;
		return location;
	}

}

ResourceId ResourcesManager::AllocateResourceId(ResourceStruct * resourceStruct)
{
	unsigned int size = _resources.size();
	unsigned int i = 0;
	while (i < size && _resources[i].Resource != NULL)
	{
		i++;
	}
	if (i < size)
	{
		// Reuse an existing slot
		_resources[i] = *resourceStruct;
	}
	else
	{
		_resources.push_back(*resourceStruct);
	}
	return i;
}

ResourceId ResourcesManager::FindResource(string resourceName)
{
    // See if the specified resource already exists.  Using a hashtable (map) would probably be a better
    // solution here.
	unsigned int size = _resources.size();
	unsigned int i = 0;
	bool found = false;

	while (i < size && !found)
	{
		if (_resources[i].ResourceName == resourceName)
		{
			found = true;
		}
		else
		{
			i++;
		}
	}
	if (!found)
	{
		return -1;
	}
	return i;
}

Mesh * ResourcesManager::LoadModelFromFile(string modelName)
{
   	D3D11_BUFFER_DESC bufferDesc;
	D3D11_MAPPED_SUBRESOURCE ms;
	ID3D11Buffer * vertexBuffer;
	ID3D11Buffer * indexBuffer;
	ID3D11Buffer * constantBuffer;
    ResourceId * materialIds = NULL;
	
	Assimp::Importer importer;

	ID3D11Device * device = Framework::This()->Device();
	ID3D11DeviceContext * deviceContext = Framework::This()->DeviceContext();

	unsigned int postProcessSteps = aiProcess_CalcTangentSpace		   | // calculate tangents and bitangents if possible
									aiProcess_ValidateDataStructure    | // perform a full validation of the loader's output
									aiProcess_ImproveCacheLocality     | // improve the cache locality of the output vertices
									aiProcess_RemoveRedundantMaterials | // remove redundant materials
									aiProcess_GenUVCoords              | // convert spherical, cylindrical, box and planar mapping to proper UVs
									aiProcess_FindInstances            | // search for instanced meshes and remove them by references to one master
									aiProcess_LimitBoneWeights         | // limit bone weights to 4 per vertex
									aiProcess_OptimizeMeshes		   | // join small meshes, if possible;
		   		   					aiProcess_GenSmoothNormals		   | // generate smooth normal vectors if not existing
									aiProcess_SplitLargeMeshes         | // split large, unrenderable meshes into submeshes
									aiProcess_Triangulate			   | // triangulate polygons with more than 3 edges
									aiProcess_SortByPType              | // make 'clean' meshes which consist of a single typ of primitives
									aiProcess_FlipUVs				   | // Set upper left hand corner as origin for UVs
									aiProcess_MakeLeftHanded		   |
                                    aiProcess_FlipWindingOrder         | 
                                    0;

	const aiScene * scene = importer.ReadFile(modelName.c_str(), postProcessSteps);
	if (!scene)
	{
        // If failed to load, there is nothing to do
		return NULL;
	}
    if (!scene->HasMeshes())
    {
        //If there are no meshes, then there is nothing to do.
        return NULL;
    }
    if (scene->HasMaterials())
    {
        // We need to find the directory part of the model name since we will need to add it to any texture names. 
        // There is definately a more elegant and accurate way to do this using Windows API calls, but this is a quick
        // and dirty approach
        string::size_type slashIndex = modelName.find_last_of("\\");
        string directory;
        if (slashIndex == string::npos) 
        {
            directory = ".";
        }
        else if (slashIndex == 0) 
        {
            directory = "/";
        }
        else 
        {
            directory = modelName.substr(0, slashIndex);
        }
        // Let's deal with the materials/textures first
        materialIds = new ResourceId[scene->mNumMaterials];
        for (unsigned int i = 0; i < scene->mNumMaterials; i++)
        {
            // Get the core material properties.  Ideally, we would be looking for more information
            // e.g. transparency, etc.  This is a task for later.
            aiMaterial * material = scene->mMaterials[i];
            aiColor3D& diffuseColour = aiColor3D(0.0f, 0.0f, 0.0f);
            material->Get(AI_MATKEY_COLOR_DIFFUSE, diffuseColour);
            aiColor3D& specularColour = aiColor3D(0.0f, 0.0f, 0.0f);
            material->Get(AI_MATKEY_COLOR_SPECULAR, specularColour);
            float defaultShininess = 0.0f;
            float& shininess = defaultShininess;
            material->Get(AI_MATKEY_SHININESS, shininess);
            string fullTextureNamePath = "";
            if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0)
            {
                aiString textureName;
                if (material->GetTexture(aiTextureType_DIFFUSE, 0, &textureName, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS)
                {
                    // Get full path to texture by prepending the same folder as included in the model name. This
                    // does assume that textures are in the same folder as the model files
                    fullTextureNamePath = directory + "\\" + textureName.data;
                }
            }
            // Now create a unique name for the material based on the model name and loop count
            stringstream materialNameStream;
            materialNameStream << modelName << i;
            string materialName = materialNameStream.str();
            ResourceId resourceId = CreateMaterial(materialName, 
                                                   XMFLOAT4(diffuseColour.r, diffuseColour.g, diffuseColour.b, 1.0f),
                                                   XMFLOAT4(specularColour.r, specularColour.g, specularColour.b, 1.0f),
                                                   shininess,
                                                   fullTextureNamePath);
            if (resourceId < 0)
            {
                return NULL;
            }
            materialIds[i] = resourceId;
        }
    }
    // Now we have created all of the materials, build up the mesh
	Mesh * resourceMesh = new Mesh();
    for (unsigned int sm = 0; sm < scene->mNumMeshes; sm++)
    {
	    aiMesh * subMesh = scene->mMeshes[sm];
	    unsigned int numVertices = subMesh->mNumVertices;
	    bool hasNormals = subMesh->HasNormals();
	    bool hasTexCoords = subMesh->HasTextureCoords(0);
	    if (numVertices == 0 || !hasNormals)
	    {
		    return NULL;
	    }
	    // Build up our vertex structure
	    aiVector3D * subMeshVertices = subMesh->mVertices;
	    aiVector3D * subMeshNormals = subMesh->mNormals;
        // We only handle one set of UV coordinates at the moment.  Again, handling multiple sets of UV
        // coordinates is a future enhancement.
	    aiVector3D * subMeshTexCoords = subMesh->mTextureCoords[0];
	    VERTEX * modelVertices = new VERTEX[numVertices];
	    VERTEX * currentVertex = modelVertices;
		XMFLOAT3*	subMeshVerts = new XMFLOAT3[numVertices];
		XMFLOAT3*	vertices = subMeshVerts;
		unsigned int	numberOfVerts = 0;
	    for (unsigned int i = 0; i < numVertices; i++)
	    {
		    currentVertex->Position.x = subMeshVertices->x;
		    currentVertex->Position.y = subMeshVertices->y;
		    currentVertex->Position.z = subMeshVertices->z;
			subMeshVerts[i].x = subMeshVertices->x;
			subMeshVerts[i].y = subMeshVertices->y;
			subMeshVerts[i].z = subMeshVertices->z;
			numberOfVerts++;
		    subMeshVertices++;
		    currentVertex->Normal.x = subMeshNormals->x;
		    currentVertex->Normal.y = subMeshNormals->y;
		    currentVertex->Normal.z = subMeshNormals->z;
		    subMeshNormals++;
           // currentVertex->Color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
            if (!hasTexCoords)
            {
                // If the model does not have texture coordinates, set them to 0
                currentVertex->TexCoord.x = 0.0f;
                currentVertex->TexCoord.y = 0.0f;
            }
            else
            {
                // Handle negative texture coordinates by wrapping them to positive.  This should
                // ideally be handled in the shader.  Note we are assuming that negative coordinates
                // here are no smaller than -1.0 - this may not be a valid assumption.
		        if (subMeshTexCoords->x < 0)
		        {
			        currentVertex->TexCoord.x = subMeshTexCoords->x + 1.0f;
		        }
		        else
		        {
			        currentVertex->TexCoord.x = subMeshTexCoords->x;
		        }
		        if (subMeshTexCoords->y < 0)
		        {
			        currentVertex->TexCoord.y = subMeshTexCoords->y + 1.0f;
		        }
		        else
		        {
			        currentVertex->TexCoord.y = subMeshTexCoords->y;
		        }
		        subMeshTexCoords++;
            }
		    currentVertex++;
	    }
	    // Create the Direct3D vertex buffer
	    ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	    bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	    bufferDesc.ByteWidth = sizeof(VERTEX) * numVertices;
	    bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	    bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	    if (FAILED(device->CreateBuffer(&bufferDesc, NULL, &vertexBuffer)))
	    {
		    return NULL;
	    }
	    // Copy vertices to the vertex buffer
	    if (FAILED(deviceContext->Map(vertexBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms)))
	    {
		    return NULL;
	    }
	    memcpy(ms.pData, modelVertices, sizeof(VERTEX) * numVertices);
	    deviceContext->Unmap(vertexBuffer, NULL);

	    // Now extract the indices from the file
	    unsigned int numberOfFaces = subMesh->mNumFaces;
	    unsigned int numberOfIndices = numberOfFaces * 3;
	    aiFace * subMeshFaces = subMesh->mFaces;
	    if (subMeshFaces->mNumIndices != 3)
	    {
		    // We are not dealing with triangles, so we cannot handle it
		    return NULL;
	    }
	    unsigned int * modelIndices = new unsigned int[numberOfIndices * 3];
	    unsigned int * currentIndex  = modelIndices;
	    for (unsigned int i = 0; i < numberOfFaces; i++)
	    {
		    *currentIndex++ = subMeshFaces->mIndices[0];
		    *currentIndex++ = subMeshFaces->mIndices[1];
		    *currentIndex++ = subMeshFaces->mIndices[2];
		    subMeshFaces++;
	    }
        // Create the index buffer
	    ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	    bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	    bufferDesc.ByteWidth = sizeof(DWORD) * numberOfIndices;
	    bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	    bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	    bufferDesc.MiscFlags = 0;
	    if (FAILED(device->CreateBuffer(&bufferDesc, NULL, &indexBuffer)))
	    {
		    return NULL;
	    }
	    deviceContext->Map(indexBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);    
	    memcpy(ms.pData, modelIndices, sizeof(DWORD) * numberOfIndices);                 
	    deviceContext->Unmap(indexBuffer, NULL);

		// Create a constant buffer
		ZeroMemory(&bufferDesc, sizeof(bufferDesc));
		bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc.ByteWidth = sizeof(CBUFFER);
		bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		if (FAILED(device->CreateBuffer(&bufferDesc, NULL, &constantBuffer)))
		{
			return false;
		}

        // Do we have a material associated with this mesh?
        ResourceId materialId = -1;
        if (scene->HasMaterials())
        {
            materialId = materialIds[subMesh->mMaterialIndex];
        }
	    SubMesh * resourceSubMesh = new SubMesh(vertexBuffer, indexBuffer, constantBuffer, numberOfIndices, 
			materialId, vertices, numVertices);
	    resourceMesh->AddSubMesh(resourceSubMesh);
    }
	return resourceMesh;
}
