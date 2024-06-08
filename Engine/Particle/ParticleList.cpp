#include "ParticleList.h"
#include "Particle.h"
#include "MyMath.h"
#include "PlayerCamera.h"
#include "2D/Texture.h"
#include "DirectXCommon.h"
#include "Transform.h"
#include "VertexData.h"
#include "Material.h"
#include "Math//Random.h"
#include "imgui/imgui.h"

ParticleList::~ParticleList()
{
	delete texture;
}

void ParticleList::Create(DirectXCommon* dxCommon) {
	instancingResourse = dxCommon->CreateBufferResource(dxCommon->GetDevice(), sizeof(ParticleForGPU) * kNumMaxInstance);
	instancingResourse->Map(0, nullptr, reinterpret_cast<void**>(&instancingData));
	//単位行列を書き込んでおく
	//for (uint32_t index = 0; index < kNumMaxInstance; ++index) {
	//	mParticles[index] = MakeNewParticle();
	//	instancingData[index].WVP = MakeIdentity4x4();
	//	instancingData[index].color = mParticles[index].color;
	//	//instancingData[index].World = MakeIdentity4x4();
	//}

	//SRVの作成
	D3D12_SHADER_RESOURCE_VIEW_DESC instancingSrvDesc{};
	instancingSrvDesc.Format = DXGI_FORMAT_UNKNOWN;
	instancingSrvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	instancingSrvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	instancingSrvDesc.Buffer.FirstElement = 0;
	instancingSrvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
	instancingSrvDesc.Buffer.NumElements = kNumMaxInstance;
	instancingSrvDesc.Buffer.StructureByteStride = sizeof(ParticleForGPU);
	D3D12_CPU_DESCRIPTOR_HANDLE instancingSrvHandleCPU = dxCommon->GetCPUDescriptorHandle();
	instancingSrvHandleGPU = dxCommon->GetGPUDescriptorHandle();
	dxCommon->GetDevice()->CreateShaderResourceView(instancingResourse.Get(), &instancingSrvDesc, instancingSrvHandleCPU);

	//Texture
	texture = new Texture();
	texture->Create(dxCommon, "resources/Particle/circle.png");

	vertexResource = dxCommon->CreateBufferResource(dxCommon->GetDevice(), sizeof(VertexData) * 6);
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
	mEmitter.count = 3; //発生数
	mEmitter.frequency = 0.5f; //0.5秒ごとに発生
	mEmitter.frequencyTime = 0.0f; //発生頻度用の時刻、0で初期化
	mEmitter.transform.translate = { -25.0f,5.0f,-25.0f };
	mEmitter.transform.rotate = { 0.0f,0.0f,0.0f };
	mEmitter.transform.scale = { 1.0f,1.0f,1.0f };
}

void ParticleList::Update() {
	ImGui::Begin("Debug");
	if (ImGui::Button("AddParticle")) {
		mParticles.splice(mParticles.end(), Emit(mEmitter));
		mParticles.splice(mParticles.end(), Emit(mEmitter));
		mParticles.splice(mParticles.end(), Emit(mEmitter));
	}
	ImGui::End();
	mEmitter.frequencyTime += kDeltaTime; //時刻を進める
	if (mEmitter.frequency <= mEmitter.frequencyTime) { //頻度より大きいなら発生
		mParticles.splice(mParticles.end(), Emit(mEmitter)); //発生処理
		mEmitter.frequencyTime -= mEmitter.frequency; //余計に過ぎた時間も加味して頻度計算する
	}
	for (std::list<Particle>::iterator particleIterator = mParticles.begin(); particleIterator != mParticles.end(); ++particleIterator) {
		mAlpha = 1.0f - ((*particleIterator).currentTime / (*particleIterator).lifeTime);
	}
}

void ParticleList::Draw(ID3D12GraphicsCommandList* commandList, Camera* camera, const Transform& mTransform) {
	//Particle用のWorldViewProjectionMatrixをつくる
	mNumInstance = 0;
	for (std::list<Particle>::iterator particleIterator = mParticles.begin(); particleIterator != mParticles.end();) {
		if ((*particleIterator).lifeTime <= (*particleIterator).currentTime) {
			particleIterator = mParticles.erase(particleIterator); //生存時間がすぎたparticleはlistから消す。戻り値が次のイテレータになる
			continue;
		}
		(*particleIterator).mTransform.translate += (*particleIterator).velocity * kDeltaTime;
		(*particleIterator).currentTime += kDeltaTime; //経過時間を足す
		if (mNumInstance < kNumMaxInstance) {
			//ビルボード
			Matrix4x4 backToFrontMatrix = MakeRotateYMatrix(0.0f);
			Matrix4x4 scaleMatrix = MakeScaleMatrix((*particleIterator).mTransform.scale);
			Matrix4x4 translateMatrix = MakeTranslateMatrix((*particleIterator).mTransform.translate);
			Matrix4x4 billboardMatrix = Multiply(backToFrontMatrix, Inverse(camera->GetViewMatrix()));
			billboardMatrix.m[3][0] = 0.0f; //平行移動成分はいらない
			billboardMatrix.m[3][1] = 0.0f;
			billboardMatrix.m[3][2] = 0.0f;
			Matrix4x4 worldMatrix = Multiply(scaleMatrix, Multiply(billboardMatrix, translateMatrix));
			//worldMatrix = MakeAffineMatrix((*particleIterator).mTransform.scale, (*particleIterator).mTransform.rotate, (*particleIterator).mTransform.translate);
			Matrix4x4 worldViewProjectionMatrix = Multiply(worldMatrix, Multiply(camera->GetViewMatrix(), camera->GetProjectionMatrix()));
			instancingData[mNumInstance].WVP = worldViewProjectionMatrix;
			instancingData[mNumInstance].World = worldMatrix;
			instancingData[mNumInstance].color = (*particleIterator).color;
			mAlpha = 1.0f - ((*particleIterator).currentTime / (*particleIterator).lifeTime);
			instancingData[mNumInstance].color.w = mAlpha; //算出したaをGPUに送る
			++mNumInstance; //生きているParticleの数を1つカウントする
		}
		++particleIterator; //次のイテレータに進める
	}
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	commandList->SetGraphicsRootConstantBufferView(0, materialResource->GetGPUVirtualAddress());
	commandList->SetGraphicsRootDescriptorTable(1, instancingSrvHandleGPU);
	commandList->IASetVertexBuffers(0, 1, &vertexBufferView);
	texture->Bind(commandList);
	commandList->DrawInstanced(6, mNumInstance, 0, 0);
}

Particle ParticleList::MakeNewParticle(const Vector3& translate) {
	Particle particle;
	particle.mTransform.scale = { 5.0f,5.0f,5.0f };
	particle.mTransform.rotate = { 0.0f,0.0f,0.0f };
	particle.mTransform.translate = { Random::Rand(-1.0f,1.0f), Random::Rand(-1.0f,1.0f), Random::Rand(-1.0f,1.0f) };
	particle.velocity = { Random::Rand(-5.0f,5.0f), Random::Rand(5.0f,50.0f), Random::Rand(-5.0f,5.0f) };
	particle.color = { Random::Rand(0.0f,1.0f),  Random::Rand(0.0f,1.0f), Random::Rand(0.0f,1.0f), 1.0f };
	particle.lifeTime = Random::Rand(5.0f, 10.0f);
	particle.currentTime = 0;
	Vector3 randomTranslate{ Random::Rand(0.0f,1.0f),Random::Rand(0.0f,1.0f),Random::Rand(0.0f,1.0f) };
	particle.mTransform.translate = translate + randomTranslate;
	return particle;
}

std::list<Particle> ParticleList::Emit(const Emitter& mEmitter) {
	std::list<Particle> particles;
	for (uint32_t count = 0; count < mEmitter.count; ++count) {
		particles.push_back(MakeNewParticle(mEmitter.transform.translate));
	}
	return particles;
}
