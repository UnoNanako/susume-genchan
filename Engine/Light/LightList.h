#pragma once
#include <cstdint>
#include "Light/DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "MyMath.h"
#include <d3d12.h>
#include <wrl.h>

class DirectXCommon;

class LightList
{
public:
	void Create(DirectXCommon* dxCommon);
	void Update();
	void Bind(ID3D12GraphicsCommandList* commandList);

	void SetSpotLightPos(Vector3 pos) { mSpotLightData->position = pos; }
	void SetSpotLightDirection(Vector3 direction) { mSpotLightData->direction = direction; }
	void SetDirectionalLightIntensity(float intensity) { mDirectionalLightData->intensity = intensity; }
private:
	//定数(シェーダー側と合わせる)
	struct DirectionalLightForGPU {
		Vector4 color;
		Vector3 direction;
		float intensity;
	};

	struct PointLightForGPU {
		Vector4 color; //ライトの色
		Vector3 position; //ライトの位置
		float intensity; //輝度
		float radius; //ライトの届く最大距離
		float decay; //減衰率
	};

	struct SpotLightForGPU {
		Vector4 color; //ライトの色
		Vector3 position; //ライトの位置
		float intensity; //輝度
		Vector3 direction; //スポットライトの方向
		float distance; //ライトの届く最大距離
		float decay; //減衰率
		float cosAngle; //スポットライトの余弦
		float cosFalloffStart;
		float pad;
	};

	//DirectionalLight
	DirectionalLight directionalLight;
	Microsoft::WRL::ComPtr<ID3D12Resource> mDirectionalLightResource;
	DirectionalLightForGPU* mDirectionalLightData;

	//PointLight
	PointLight pointLight;
	Microsoft::WRL::ComPtr<ID3D12Resource> mPointLightResource;
	PointLightForGPU* mPointLightData;

	//SpotLight
	SpotLight spotLight;
	Microsoft::WRL::ComPtr<ID3D12Resource> mSpotLightResource;
	SpotLightForGPU* mSpotLightData;
};

