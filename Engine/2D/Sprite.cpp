#include "DirectXCommon.h"
#include "Material.h"
#include "Sprite.h"
#include "Texture.h"
#include "Transform.h"
#include "VertexData.h"
#include <Windows.h>

void Sprite::Create(DirectXCommon* dxCommon, const std::string& filePath)
{
	//テクスチャ
	texture = new Texture();
	texture->Create(dxCommon,filePath);

	//インデックス
	indexResource = dxCommon->CreateBufferResource(dxCommon->GetDevice(), sizeof(uint32_t) * 6);
	indexBufferView.BufferLocation = indexResource->GetGPUVirtualAddress();
	indexBufferView.SizeInBytes = sizeof(uint32_t) * 6;
	indexBufferView.Format = DXGI_FORMAT_R32_UINT;
	indexData = nullptr;
	indexResource->Map(0, nullptr, reinterpret_cast<void**>(&indexData));
	indexData[0] = 0; indexData[1] = 1; indexData[2] = 2;
	indexData[3] = 1; indexData[4] = 3; indexData[5] = 2;

	//頂点
	vertexResource = dxCommon->CreateBufferResource(dxCommon->GetDevice(), sizeof(VertexData) * 4);
	vertexBufferView.BufferLocation = vertexResource->GetGPUVirtualAddress();
	vertexBufferView.SizeInBytes = sizeof(VertexData) * 4;
	vertexBufferView.StrideInBytes = sizeof(VertexData);

	// 頂点リソースにデータを書き込む
	VertexData* vertexData = nullptr;
	// 書き込むためのアドレスを取得
	vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));
	float width = float(texture->GetWidth());
	float height = float(texture->GetHeight());
	// 左上
	vertexData[0].position = { 0.0f, 0.0f, 0.0f, 1.0f };
	vertexData[0].texcoord = { 0.0f,0.0f };
	// 右上
	vertexData[1].position = { width, 0.0f, 0.0f, 1.0f };
	vertexData[1].texcoord = { 1.0f,0.0f };
	// 左下
	vertexData[2].position = { 0.0f, height, 0.0f, 1.0f };
	vertexData[2].texcoord = { 0.0f,1.0f };
	//右下
	vertexData[3].position = { width,height,0.0f,1.0f };
	vertexData[3].texcoord = { 1.0f,1.0f };

	//色
	//Sprite用のマテリアルリソースを作る
	materialResource = dxCommon->CreateBufferResource(dxCommon->GetDevice(), sizeof(Material));
	//書き込むためのアドレスを取得
	materialResource->Map(0, nullptr, reinterpret_cast<void**>(&materialData));
	materialData->color = { 1.0f,1.0f,1.0f,1.0f };
	//SpriteはLightingしないのでfalseを設定する
	materialData->enableLighting = false;

	//Transform
	//Sprite用のTransformMatrix用のリソースを作る。Matrix4x4。1つ分のサイズを用意する
	transformationMatrixResource = dxCommon->CreateBufferResource(dxCommon->GetDevice(), sizeof(TransformationMatrix));
	//書き込むためのアドレスを取得
	transformationMatrixResource->Map(0, nullptr, reinterpret_cast<void**>(&transformationMatrixData));
	//単位行列を書き込んでおく
	transformationMatrixData->WVP = MakeIdentity4x4();
	transformationMatrixData->World = MakeIdentity4x4();
	//CPUで動かす用のTransformWVP作る
	transform = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
}

void Sprite::Update()
{
	// Sprite用のWorldViewProjectionMatrixを作る
	Matrix4x4 worldMatrix = MakeAffineMatrix(transform.scale, transform.rotate, transform.translate);
	Matrix4x4 viewMatrix = MakeIdentity4x4();
	Matrix4x4 projectionMatrix = MakeOrthographicMatrix(0.0f, 0.0f, float(WinApiManager::kClientWidth), float(WinApiManager::kClientHeight), 0.0f, 100.0f);
	Matrix4x4 worldViewProjectionMatrix = Multiply(worldMatrix, Multiply(viewMatrix, projectionMatrix));
	transformationMatrixData->WVP = worldViewProjectionMatrix;
	transformationMatrixData->World = worldMatrix;
}

void Sprite::Draw(ID3D12GraphicsCommandList* commandList)
{
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	commandList->SetGraphicsRootConstantBufferView(0, materialResource->GetGPUVirtualAddress());
	commandList->SetGraphicsRootConstantBufferView(1, transformationMatrixResource->GetGPUVirtualAddress());
	texture->Bind(commandList);
	commandList->IASetVertexBuffers(0, 1, &vertexBufferView);
	commandList->IASetIndexBuffer(&indexBufferView);
	commandList->DrawIndexedInstanced(6, 1, 0, 0, 0);
}

Sprite::~Sprite()
{
	delete texture;
}
