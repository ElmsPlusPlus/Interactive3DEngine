#pragma once
#include "Mesh.h"

// We could store the resource names in a separate structure such as a map and use that structure
// to map names to resource ids. This would be faster for large numbers of resources, but would add
// extra complication. 

struct ResourceStruct
{
	string		  ResourceName;
	unsigned int  ReferenceCount;
	void *		  Resource;
};

class ResourcesManager
{
public:
	ResourcesManager(void);
	~ResourcesManager(void);

	ResourceId					LoadModel(string modelName);
	Mesh *						GetMesh(ResourceId id);
	void						ReleaseMesh(ResourceId id);

	ResourceId					CreateMaterialFromTexture(string textureName);
    ResourceId                  CreateMaterialWithNoTexture(string materialName, XMFLOAT4 diffuseColour, XMFLOAT4 specularColour, float shininess);
    ResourceId                  CreateMaterial(string materialName, XMFLOAT4 diffuseColour, XMFLOAT4 specularColour, float shininess, string textureName);
	Material *	                GetMaterial(ResourceId id);
	void						ReleaseMaterial(ResourceId id);

    // The default texture is white.png (which should be provided along with the application).  This is the 
    // the texture to use if no other texture is available. Note that if white.png is not supplied, the default
    // texture will be null, i.e. black.

    ID3D11ShaderResourceView *  GetDefaultTexture(void);

private:
	vector<ResourceStruct>		 _resources;
    ID3D11ShaderResourceView *   _defaultTexture;
    
	ResourceId					AllocateResourceId(ResourceStruct * resourceStruct);
	ResourceId					FindResource(string resourceName);
	Mesh *						LoadModelFromFile(string modelName);
    ResourceId                  InitialiseMaterial(string materialName, XMFLOAT4 diffuseColour, XMFLOAT4 specularColour, float shininess, string textureName);
};

