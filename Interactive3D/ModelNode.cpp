#include "ModelNode.h"
#include <assimp.hpp>
#include <aiScene.h>
#include <aiPostProcess.h>

#pragma comment(lib, "assimp.lib")

ModelNode::ModelNode(wstring name, string modelFile, int type, string defaultTextureName) : SceneNode(name)
{
	_modelFile = modelFile;
	_defaultTextureName = defaultTextureName;
	
}

ModelNode::~ModelNode(void)
{
}

bool ModelNode::Initialise(void)
{
	_meshResourceId = Framework::This()->GetResourcesManager()->LoadModel(_modelFile);
	if (_meshResourceId < 0)
	{
		return false;
	}
    _defaultMaterialResourceId = Framework::This()->GetResourcesManager()->CreateMaterialFromTexture(_defaultTextureName);
    if (_defaultMaterialResourceId < 0)
    {
	    return false;
    }

	if (_type == 1)
	{
		_bBox = ModelNode::CreateBoundingBox(_meshResourceId);
	}

	if (_type == 2)
	{
		_bSphere = ModelNode::CreateBoundingSphere(_meshResourceId);
	}

	return true;
}

void ModelNode::Render(void)
{
	Renderer * renderer = Framework::This()->GetCurrentRenderer();
	ResourcesManager * resourcesManager = Framework::This()->GetResourcesManager();
	Mesh * mesh = resourcesManager->GetMesh(_meshResourceId);
    renderer->SetWorldTransformation(_worldTransformation);
	unsigned int subMeshCount = mesh->GetSubMeshCount();
    // Loop through all submeshes in the mesh
	for (unsigned int i = 0; i < subMeshCount; i++)
	{
		SubMesh * subMesh = mesh->GetSubMesh(i);
		renderer->SetVertexBuffer(subMesh->GetVertexBuffer());
		renderer->SetIndexBuffer(subMesh->GetIndexBuffer());
		renderer->SetConstantBuffer(subMesh->GetConstantBuffer());
        ResourceId materialId = subMesh->GetMaterial();
        // The material handling here is more complicated that it really should be since it is intended to handle
        // all cases so far encounted with .x files, i.e.
        // - A submesh contains a material that contains a valid texture
        // - A submesh contains a material that does not reference a texture. In this case, we use the texture
        //   provided in the LoadModel call.  If none was provided, then we just use the default (white.png)
        // - A submesh does not contain a material. In this case, we use the default (white.png with white colours).
        //
        // In a real game, you would hopefully have more consistency in your models and so would not need to cover
        // all of these cases
        //
        if (materialId < 0)
        {
            materialId = _defaultMaterialResourceId;
        }
        Material * material = resourcesManager->GetMaterial(materialId);
        // We need to add the handling of specular colours/shininess here as well once the shader
        // can handle it.
        // Uncomment out the following line if you have added the support for diffuse colours to your
        // renderer and shader
        renderer->SetDiffuseColour(material->GetDiffuseColour());
        ID3D11ShaderResourceView * texture = material->GetTexture();
        if (texture == NULL)
        {
            texture = resourcesManager->GetMaterial(_defaultMaterialResourceId)->GetTexture();
            if (texture == NULL)
            {
                texture = resourcesManager->GetDefaultTexture();
            }
        }
		renderer->SetTexture(texture);
		renderer->Render(subMesh->GetNumberOfIndices(), false);
	}

	if (_type == 1)
	{
		ModelNode::BBoxRender();
	}

	if (_type == 2)
	{
		ModelNode::BSphereRender();
	}

	
	
}

void ModelNode::Shutdown(void)
{
	Framework::This()->GetResourcesManager()->ReleaseMesh(_meshResourceId);
	Framework::This()->GetResourcesManager()->ReleaseMaterial(_defaultMaterialResourceId);
    SceneNode::Shutdown();
}

BoundingBox ModelNode::CreateBoundingBox(ResourceId meshResourceId)
{
	Mesh * mesh = Framework::This()->GetResourcesManager()->GetMesh(meshResourceId);
	BoundingBox temp, temp2;

	unsigned int size = mesh->GetSubMeshCount();

	temp.CreateFromPoints(temp, mesh->GetSubMesh(0)->GetNumberOfVertices(), mesh->GetSubMesh(0)->GetSubMeshVertices(), sizeof(XMFLOAT3));

	for (unsigned int i = 1; i < size; i++)
	{
		temp2.CreateFromPoints(temp2, mesh->GetSubMesh(i)->GetNumberOfVertices(), mesh->GetSubMesh(i)->GetSubMeshVertices(), sizeof(XMFLOAT3));
		temp.CreateMerged(temp, temp, temp2);
	}

	return temp;
}

void ModelNode::BBoxRender(void)
{
   	D3D11_BUFFER_DESC bufferDesc;
	D3D11_MAPPED_SUBRESOURCE ms;
	ID3D11Buffer * vertexBuffer;
	ID3D11Buffer * indexBuffer;

	ID3D11Device * device = Framework::This()->Device();
	ID3D11DeviceContext * deviceContext = Framework::This()->DeviceContext();
	Renderer * renderer = Framework::This()->GetCurrentRenderer();

	//BoundingBox transformedBoundingBox;
	//transformedBoundingBox = _bBox;
	//transformedBoundingBox.Transform(transformedBoundingBox, _worldTransformation);


    XMFLOAT3 cubeVertices[] =
    {
        XMFLOAT3(-1.0f, 1.0f, -1.0f),    
	    XMFLOAT3(1.0f, 1.0f, -1.0f),
	    XMFLOAT3(-1.0f, -1.0f, -1.0f), 
	    XMFLOAT3(1.0f, -1.0f, -1.0f), 
	    XMFLOAT3(-1.0f, 1.0f, 1.0f),  
	    XMFLOAT3(1.0f, 1.0f, 1.0f),   
	    XMFLOAT3(-1.0f, -1.0f, 1.0f), 
	    XMFLOAT3(1.0f, -1.0f, 1.0f),  
    };

    DWORD cubeIndices[] = 
    {
	    0, 1, 2,    
	    2, 1, 3,
	    4, 0, 6,    
	    6, 0, 2,
	    7, 5, 6,    
	    6, 5, 4,
	    3, 1, 7,    
	    7, 1, 5,
	    4, 5, 0,    
	    0, 5, 1,
	    3, 7, 2,    
	    2, 7, 6,
    };

    vector<VERTEX> vertices;

    for (int i = 0; i < 8; i++)
    {
        VERTEX vertex;
        vertex.Position = cubeVertices[i];
        /*vertex.Position.x = vertex.Position.x * transformedBoundingBox.Extents.x + transformedBoundingBox.Center.x;
        vertex.Position.y = vertex.Position.y * transformedBoundingBox.Extents.y + transformedBoundingBox.Center.y;
        vertex.Position.z = vertex.Position.z * transformedBoundingBox.Extents.z + transformedBoundingBox.Center.z;*/
		vertex.Position.x = vertex.Position.x * _transBBox.Extents.x + _transBBox.Center.x;
        vertex.Position.y = vertex.Position.y * _transBBox.Extents.y + _transBBox.Center.y;
        vertex.Position.z = vertex.Position.z * _transBBox.Extents.z + _transBBox.Center.z;
        // Set the normal to 0, 0, 0 since we do not care about the values to render a wireframe bounding box
        vertex.Normal = XMFLOAT3(0, 0, 0);
        //vertex.Color = XMFLOAT4(0, 0, 0, 0);
        vertex.TexCoord = XMFLOAT2(0, 0);
        vertices.push_back(vertex);
    }

	// Now create our vertex and index buffers
    ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.ByteWidth = sizeof(VERTEX) * vertices.size();
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	device->CreateBuffer(&bufferDesc, NULL, &vertexBuffer);
	// Copy vertices to the vertex buffer
	deviceContext->Map(vertexBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
	memcpy(ms.pData, &vertices.front(), sizeof(VERTEX) * vertices.size());
	deviceContext->Unmap(vertexBuffer, NULL);

    // Create the index buffer
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.ByteWidth = sizeof(cubeIndices);
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferDesc.MiscFlags = 0;
	device->CreateBuffer(&bufferDesc, NULL, &indexBuffer);
	deviceContext->Map(indexBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);    
	memcpy(ms.pData, cubeIndices, sizeof(cubeIndices));                 
	deviceContext->Unmap(indexBuffer, NULL);

	// Now render it
    renderer->SetWorldTransformation(XMMatrixIdentity());
	renderer->SetVertexBuffer(vertexBuffer);
	renderer->SetIndexBuffer(indexBuffer);
	renderer->SetTexture(Framework::This()->GetResourcesManager()->GetDefaultTexture());
	renderer->Render(36, true);

	// cleanup
	vertexBuffer->Release();
	indexBuffer->Release();
}

BoundingBox ModelNode::GetBoundingBox(void)
{
	return _transBBox;
}

BoundingSphere ModelNode::CreateBoundingSphere(ResourceId meshResourceId)
{
	Mesh * mesh = Framework::This()->GetResourcesManager()->GetMesh(meshResourceId);
	BoundingSphere temp, temp2;

	unsigned int size = mesh->GetSubMeshCount();

	temp.CreateFromPoints(temp, mesh->GetSubMesh(0)->GetNumberOfVertices(), mesh->GetSubMesh(0)->GetSubMeshVertices(), sizeof(XMFLOAT3));

	for (unsigned int i = 1; i < size; i++)
	{
		temp2.CreateFromPoints(temp2, mesh->GetSubMesh(i)->GetNumberOfVertices(), mesh->GetSubMesh(i)->GetSubMeshVertices(), sizeof(XMFLOAT3));
		temp.CreateMerged(temp, temp, temp2);
	}

	return temp;
}

BoundingSphere ModelNode::GetBoundingSphere(void)
{
	return _transBSphere;
}

void ModelNode::BSphereRender(void)
{
	D3D11_BUFFER_DESC bufferDesc;
	D3D11_MAPPED_SUBRESOURCE ms;
	ID3D11Buffer * vertexBuffer;
	ID3D11Buffer * indexBuffer;

	ID3D11Device * device = Framework::This()->Device();
	ID3D11DeviceContext * deviceContext = Framework::This()->DeviceContext();
	Renderer * renderer = Framework::This()->GetCurrentRenderer();

	//BoundingSphere transformedBoundingSphere;
	//transformedBoundingSphere = _bSphere;
	//_bSphere.Transform(transformedBoundingSphere, _worldTransformation);

	size_t tessellation = 10;
	float diameter = _bSphere.Radius * 2;
	vector<VERTEX> vertices;
    vector<DWORD> indices;

    size_t verticalSegments = tessellation;
    size_t horizontalSegments = tessellation * 2;

    // Create rings of vertices at progressively higher latitudes.
    for (size_t i = 0; i <= verticalSegments; i++)
    {
        float v = 1 - (float)i / verticalSegments;

        float latitude = (i * XM_PI / verticalSegments) - XM_PIDIV2;
        float dy, dxz;

        XMScalarSinCos(&dy, &dxz, latitude);

        // Create a single ring of vertices at this latitude.
        for (size_t j = 0; j <= horizontalSegments; j++)
        {
            float u = (float)j / horizontalSegments;

            float longitude = j * XM_2PI / horizontalSegments;
            float dx, dz;

            XMScalarSinCos(&dx, &dz, longitude);

            dx *= dxz;
            dz *= dxz;

            XMVECTOR normal = XMVectorSet(dx, dy, dz, 0);

			VERTEX vertex;
			//XMStoreFloat3(&vertex.Position, normal * transformedBoundingSphere.Radius);
			/*vertex.Position.x += transformedBoundingSphere.Center.x;
			vertex.Position.y += transformedBoundingSphere.Center.y;
			vertex.Position.z += transformedBoundingSphere.Center.z;*/
			XMStoreFloat3(&vertex.Position, normal * _transBSphere.Radius);
			vertex.Position.x += _transBSphere.Center.x;
			vertex.Position.y += _transBSphere.Center.y;
			vertex.Position.z += _transBSphere.Center.z;
			XMStoreFloat3(&vertex.Normal, normal);
			//vertex.Color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
			vertex.TexCoord = XMFLOAT2(0.0f, 0.0f);
            vertices.push_back(vertex);
        }
    }

    // Fill the index buffer with triangles joining each pair of latitude rings.
    size_t stride = horizontalSegments + 1;

    for (size_t i = 0; i < verticalSegments; i++)
    {
        for (size_t j = 0; j <= horizontalSegments; j++)
        {
            size_t nextI = i + 1;
            size_t nextJ = (j + 1) % stride;

            indices.push_back(i * stride + j);
            indices.push_back(nextI * stride + j);
            indices.push_back(i * stride + nextJ);

            indices.push_back(i * stride + nextJ);
            indices.push_back(nextI * stride + j);
            indices.push_back(nextI * stride + nextJ);
        }
    }
	// Now create our vertex and index buffers
    ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.ByteWidth = sizeof(VERTEX) * vertices.size();
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	device->CreateBuffer(&bufferDesc, NULL, &vertexBuffer);
	// Copy vertices to the vertex buffer
	deviceContext->Map(vertexBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
	memcpy(ms.pData, &vertices.front(), sizeof(VERTEX) * vertices.size());
	deviceContext->Unmap(vertexBuffer, NULL);

    // Create the index buffer
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.ByteWidth = sizeof(DWORD) * indices.size();
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferDesc.MiscFlags = 0;
	device->CreateBuffer(&bufferDesc, NULL, &indexBuffer);
	deviceContext->Map(indexBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);    
	memcpy(ms.pData, &indices.front(), sizeof(DWORD) * indices.size());                 
	deviceContext->Unmap(indexBuffer, NULL);

	// Now render it
    renderer->SetWorldTransformation(XMMatrixIdentity());
	renderer->SetVertexBuffer(vertexBuffer);
	renderer->SetIndexBuffer(indexBuffer);
	renderer->SetTexture(Framework::This()->GetResourcesManager()->GetDefaultTexture());
	renderer->Render(indices.size(), true);

	// cleanup
	vertexBuffer->Release();
	indexBuffer->Release();
}