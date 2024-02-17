#include "DirectXCommon.h"
#include "Material.h"
#include "MyMath.h"
#include "SphereModel.h"
#include "Transform.h"
#include "VertexData.h"
#include "externals/imgui/imgui.h"
#include "Camera.h"

void SphereModel::Create(DirectXCommon* dxCommon)
{
	cameraTransform = {
			{1.0f,1.0f,1.0f},
			{0.0f,0.0f,0.0f},
			{0.0f,0.0f,-5.0f}
	};
	transform = { {1.0f, 1.0f, 1.0f}, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f } };
	vertexResource = dxCommon->CreateBufferResource(dxCommon->GetDevice(), sizeof(VertexData) * kVertexCount);
	wvpResource = dxCommon->CreateBufferResource(dxCommon->GetDevice(), sizeof(TransformationMatrix));
	vertexData = nullptr;
	wvpData = nullptr;
	vertexBufferView.BufferLocation = vertexResource->GetGPUVirtualAddress();
	vertexBufferView.SizeInBytes = UINT(sizeof(VertexData) * kVertexCount); //使用するリソースのサイズは頂点のサイズ
	vertexBufferView.StrideInBytes = sizeof(VertexData);
	vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));
	wvpResource->Map(0, nullptr, reinterpret_cast<void**>(&wvpData));
	wvpData->WVP = MakeIdentity4x4();
	wvpData->World = MakeIdentity4x4();

	materialResource = dxCommon->CreateBufferResource(dxCommon->GetDevice(), sizeof(Material));
	materialResource->Map(0, nullptr, reinterpret_cast<void**>(&materialData));
	materialData->color = { 1.0f,1.0f,1.0f,1.0f };
	materialData->enableLighting = true;
	materialData->shininess = 100.0f;

	//球体用頂点
	for (uint32_t latIndex = 0; latIndex < kSubdivision; ++latIndex) {
		float lat = -kPi / 2.0f + kLatEvery * latIndex;
		for (uint32_t lonIndex = 0; lonIndex < kSubdivision; ++lonIndex) {
			uint32_t start = (latIndex * kSubdivision + lonIndex) * 6;
			float lon = lonIndex * kLonEvery;
			//a
			vertexData[start].position.x = cos(lat) * cos(lon);
			vertexData[start].position.y = sin(lat);
			vertexData[start].position.z = cos(lat) * sin(lon);
			vertexData[start].position.w = 1.0f;
			vertexData[start].texcoord.x = float(lonIndex) / float(kSubdivision);
			vertexData[start].texcoord.y = 1.0f - float(latIndex) / float(kSubdivision);
			vertexData[start].normal.x = vertexData[start].position.x;
			vertexData[start].normal.y = vertexData[start].position.y;
			vertexData[start].normal.z = vertexData[start].position.z;
			//b
			vertexData[start + 1].position.x = cos(lat + kLatEvery) * cos(lon);
			vertexData[start + 1].position.y = sin(lat + kLatEvery);
			vertexData[start + 1].position.z = cos(lat + kLatEvery) * sin(lon);
			vertexData[start + 1].position.w = 1.0f;
			vertexData[start + 1].texcoord.x = float(lonIndex) / float(kSubdivision);
			vertexData[start + 1].texcoord.y = 1.0f - float(latIndex + 1) / float(kSubdivision);
			vertexData[start + 1].normal.x = vertexData[start + 1].position.x;
			vertexData[start + 1].normal.y = vertexData[start + 1].position.y;
			vertexData[start + 1].normal.z = vertexData[start + 1].position.z;
			//c
			vertexData[start + 2].position.x = cos(lat) * cos(lon + kLonEvery);
			vertexData[start + 2].position.y = sin(lat);
			vertexData[start + 2].position.z = cos(lat) * sin(lon + kLonEvery);
			vertexData[start + 2].position.w = 1.0f;
			vertexData[start + 2].texcoord.x = float(lonIndex + 1) / float(kSubdivision);
			vertexData[start + 2].texcoord.y = 1.0f - float(latIndex) / float(kSubdivision);
			vertexData[start + 2].normal.x = vertexData[start + 2].position.x;
			vertexData[start + 2].normal.y = vertexData[start + 2].position.y;
			vertexData[start + 2].normal.z = vertexData[start + 2].position.z;
			//c
			vertexData[start + 3] = vertexData[start + 2];
			vertexData[start + 3].normal.x = vertexData[start + 3].position.x;
			vertexData[start + 3].normal.y = vertexData[start + 3].position.y;
			vertexData[start + 3].normal.z = vertexData[start + 3].position.z;
			//b
			vertexData[start + 4] = vertexData[start + 1];
			vertexData[start + 4].normal.x = vertexData[start + 4].position.x;
			vertexData[start + 4].normal.y = vertexData[start + 4].position.y;
			vertexData[start + 4].normal.z = vertexData[start + 4].position.z;
			//d
			vertexData[start + 5].position.x = cos(lat + kLatEvery) * cos(lon + kLonEvery);
			vertexData[start + 5].position.y = sin(lat + kLatEvery);
			vertexData[start + 5].position.z = cos(lat + kLatEvery) * sin(lon + kLonEvery);
			vertexData[start + 5].position.w = 1.0f;
			vertexData[start + 5].texcoord.x = float(lonIndex + 1) / float(kSubdivision);
			vertexData[start + 5].texcoord.y = 1.0f - float(latIndex + 1) / float(kSubdivision);
			vertexData[start + 5].normal.x = vertexData[start + 5].position.x;
			vertexData[start + 5].normal.y = vertexData[start + 5].position.y;
			vertexData[start + 5].normal.z = vertexData[start + 5].position.z;
		}
	}
}

void SphereModel::Update()
{
	ImGui::Begin("Debug");
	ImGui::DragFloat3("Sphere Position", &transform.translate.x,0.05f);
	ImGui::End();
	
}

void SphereModel::Draw(ID3D12GraphicsCommandList* commandList, Camera* camera)
{
	//Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, float(WinApiManager::kClientWidth) / float(WinApiManager::kClientHeight), 0.1f, 100.0f);
	Matrix4x4 worldMatrix = MakeAffineMatrix(transform.scale, transform.rotate, transform.translate);
	Matrix4x4 worldViewProjectionMatrix = Multiply(worldMatrix, Multiply(camera->GetViewMatrix(), camera->GetProjectionMatrix()));
	wvpData->WVP = worldViewProjectionMatrix;
	wvpData->World = worldMatrix;
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	commandList->SetGraphicsRootConstantBufferView(0, materialResource->GetGPUVirtualAddress());
	commandList->SetGraphicsRootConstantBufferView(1, wvpResource->GetGPUVirtualAddress());
	commandList->IASetVertexBuffers(0, 1, &vertexBufferView);
	//wvp用のCBufferの場所を設定
	//描画！6頂点の板ポリゴンを、kNumInstance(今回は10)だけInstance描画を行う
	commandList->DrawInstanced(kVertexCount, 1, 0, 0);
}
