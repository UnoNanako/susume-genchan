#include "ParticleList.h"
#include "Particle.h"
#include "MyMath.h"
#include "PlayerCamera.h"
#include "2D/Texture.h"
#include "DirectXCommon.h"
#include "Transform.h"
#include "VertexData.h"
#include "Material.h"

ParticleList::~ParticleList()
{
	delete texture;
}

void ParticleList::Create(DirectXCommon* dxCommon){
	instancingResourse = dxCommon->CreateBufferResource(dxCommon->GetDevice(), sizeof(ParticleForGPU) * kNumInstance);
	instancingResourse->Map(0, nullptr, reinterpret_cast<void**>(&instancingData));
	//単位行列を書き込んでおく
	for (uint32_t index = 0; index < kNumInstance; ++index) {
		instancingData[index].WVP = MakeIdentity4x4();
		instancingData[index].color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
		//instancingData[index].World = MakeIdentity4x4();

		transforms[index].scale = { 1.0f,1.0f,1.0f };
		transforms[index].rotate = { 0.0f,0.0f,0.0f };
		transforms[index].translate = { index * 0.1f, index * 0.1f, index * 0.1f };
	}

	//SRVの作成
	D3D12_SHADER_RESOURCE_VIEW_DESC instancingSrvDesc{};
	instancingSrvDesc.Format = DXGI_FORMAT_UNKNOWN;
	instancingSrvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	instancingSrvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	instancingSrvDesc.Buffer.FirstElement = 0;
	instancingSrvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
	instancingSrvDesc.Buffer.NumElements = kNumInstance;
	instancingSrvDesc.Buffer.StructureByteStride = sizeof(TransformationMatrix);
	D3D12_CPU_DESCRIPTOR_HANDLE instancingSrvHandleCPU = dxCommon->GetCPUDescriptorHandle();
	instancingSrvHandleGPU = dxCommon->GetGPUDescriptorHandle();
	dxCommon->GetDevice()->CreateShaderResourceView(instancingResourse.Get(), &instancingSrvDesc, instancingSrvHandleCPU);

	//Texture
	texture = new Texture();
	texture->Create(dxCommon, "resources/uvChecker.png");

	vertexResource = dxCommon->CreateBufferResource(dxCommon->GetDevice(), sizeof(VertexData)*6);
	vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));
	vertexBufferView.BufferLocation = vertexResource->GetGPUVirtualAddress();
	vertexBufferView.SizeInBytes = sizeof(VertexData) * 6;
	vertexBufferView.StrideInBytes = sizeof(VertexData); //1頂点のサイズ

	// 左上
	vertexData[0].position = { -0.5f, 0.5f, 0.0f, 1.0f };
	vertexData[0].texcoord = { 0.0f, 0.0f };
	// 右上
	vertexData[1].position = { 0.5f, 0.5f, 0.0f, 1.0f };
	vertexData[1].texcoord = { 1.0f, 0.0f };
	// 右下
	vertexData[2].position = { 0.5f, -0.5f, 0.0f, 1.0f };
	vertexData[2].texcoord = { 1.0f, 1.0f };
	// 左上2
	vertexData[3].position = { -0.5f, 0.5f, 0.0f, 1.0f };
	vertexData[3].texcoord = { 0.0f, 0.0f };
	// 右下2
	vertexData[4].position = { 0.5f, -0.5f, 0.0f, 1.0f };
	vertexData[4].texcoord = { 1.0f, 1.0f };
	// 左下
	vertexData[5].position = { -0.5f, -0.5f, 0.0f, 1.0f };
	vertexData[5].texcoord = { 0.0f, 1.0f };

	materialResource = dxCommon->CreateBufferResource(dxCommon->GetDevice(), sizeof(Material));
	materialResource->Map(0, nullptr, reinterpret_cast<void**>(&materialData));
	materialData->color = { 1.0f,1.0f,1.0f,1.0f };
}

void ParticleList::Update(Camera* camera){
	/*for (uint32_t index = 0; index < kNumInstance; ++index) {
		Matrix4x4 worldMatrix = MakeAffineMatrix(transforms[index].scale, transforms[index].rotate, transforms[index].translate);
		Matrix4x4 worldViewProjectionMatrix = Multiply(worldMatrix,Multiply(camera->GetViewMatrix(),camera->GetProjectionMatrix()));
		instancingData[index].WVP = worldViewProjectionMatrix;
		instancingData[index].World = worldMatrix;
	}*/
}

void ParticleList::Draw(ID3D12GraphicsCommandList* commandList, Camera* camera,const Transform &mTransform){
	//Particle用のWorldViewProjectionMatrixをつくる
	/*Matrix4x4 worldMatrix = MakeAffineMatrix(transform.scale, transform.rotate, transform.translate);
	Matrix4x4 worldViewProjectionMatrix = Multiply(worldMatrix, Multiply(camera->GetViewMatrix(), camera->GetProjectionMatrix()));
	for (uint32_t index = 0; index < kNumInstance; ++index) {
		instancingData[index].WVP = worldViewProjectionMatrix;
		instancingData[index].World = worldMatrix;
	}*/
	for (uint32_t index = 0; index < kNumInstance; ++index) {
		Matrix4x4 worldMatrix = MakeAffineMatrix(transforms[index].scale, transforms[index].rotate, transforms[index].translate);
		Matrix4x4 worldViewProjectionMatrix = Multiply(worldMatrix, Multiply(camera->GetViewMatrix(), camera->GetProjectionMatrix()));
		instancingData[index].WVP = worldViewProjectionMatrix;
		instancingData[index].World = worldMatrix;
	}
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	commandList->SetGraphicsRootConstantBufferView(0, materialResource->GetGPUVirtualAddress());
	commandList->SetGraphicsRootDescriptorTable(1, instancingSrvHandleGPU);
	commandList->IASetVertexBuffers(0, 1, &vertexBufferView);
	texture->Bind(commandList);
	commandList->DrawInstanced(6, kNumInstance, 0, 0);
}
