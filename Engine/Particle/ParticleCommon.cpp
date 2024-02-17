#include "ParticleCommon.h"
#include "DirectXCommon.h"
#include "Engine/Logger.h"
#include <cassert>

void ParticleCommon::Initialize(DirectXCommon* dxCommon)
{
	D3D12_DESCRIPTOR_RANGE particleDescriptorRange[1] = {};
	particleDescriptorRange[0].BaseShaderRegister = 0;  // 0から始まる
	particleDescriptorRange[0].NumDescriptors = 1;  // 数は1つ
	particleDescriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV; // SRVを使う
	particleDescriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND; // Offsetを自動計算

	//RootSignatureを生成
	D3D12_ROOT_SIGNATURE_DESC particleDescriptionRootSignature{};
	particleDescriptionRootSignature.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	// RootParameter作成。複数設定できるので配列。今回は結果1つだけなので長さ1の配列
	D3D12_ROOT_PARAMETER particleRootParameters[3] = {};

	//Material
	particleRootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;    // CBVを使う
	particleRootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;   // PixelShaderで使う
	particleRootParameters[0].Descriptor.ShaderRegister = 0;    // レジスタ番号0とバインド

	//instancingResource
	particleRootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE; //CBVを使う
	particleRootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX; //VertexShaderで使う
	particleRootParameters[1].DescriptorTable.pDescriptorRanges = particleDescriptorRange;  // Tableの中身の配列を指定
	particleRootParameters[1].DescriptorTable.NumDescriptorRanges = _countof(particleDescriptorRange); // Tableで利用する数

	//Texture
	particleRootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE; // DescriptorTableを使う
	particleRootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // PixelShaderで使う
	particleRootParameters[2].DescriptorTable.pDescriptorRanges = particleDescriptorRange;  // Tableの中身の配列を指定
	particleRootParameters[2].DescriptorTable.NumDescriptorRanges = _countof(particleDescriptorRange); // Tableで利用する数

	particleDescriptionRootSignature.pParameters = particleRootParameters;  // ルートパラメータ配列へのポインタ
	particleDescriptionRootSignature.NumParameters = _countof(particleRootParameters);  // 配列の長さ

	D3D12_STATIC_SAMPLER_DESC particleStaticSamplers[1] = {};
	particleStaticSamplers[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR; // バイリニアフィルタ
	particleStaticSamplers[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;   // 0~1の範囲外をリピート
	particleStaticSamplers[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	particleStaticSamplers[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	particleStaticSamplers[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER; // 比較しない
	particleStaticSamplers[0].MaxLOD = D3D12_FLOAT32_MAX;   // ありったけのMipmapを使う
	particleStaticSamplers[0].ShaderRegister = 0;   // レジスタ番号0を使う
	particleStaticSamplers[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // PixelShaderで使う
	particleDescriptionRootSignature.pStaticSamplers = particleStaticSamplers;
	particleDescriptionRootSignature.NumStaticSamplers = _countof(particleStaticSamplers);

	//シリアライズしてバイナリにする
	Microsoft::WRL::ComPtr<ID3DBlob> particleSignatureBlob = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> particleErrorBlob = nullptr;
	HRESULT pHr;
	pHr = D3D12SerializeRootSignature(&particleDescriptionRootSignature, D3D_ROOT_SIGNATURE_VERSION_1, &particleSignatureBlob, &particleErrorBlob);
	if (FAILED(pHr)) {
		Logger::Log(reinterpret_cast<char*>(particleErrorBlob->GetBufferPointer()));
		assert(false);
	}
	//バイナリを元に生成
	particleRootSignature = nullptr;
	pHr = dxCommon->GetDevice()->CreateRootSignature(0, particleSignatureBlob->GetBufferPointer(),
		particleSignatureBlob->GetBufferSize(), IID_PPV_ARGS(&particleRootSignature));
	assert(SUCCEEDED(pHr));

	//InputLayout
	D3D12_INPUT_ELEMENT_DESC particleInputElementDescs[3] = {};
	particleInputElementDescs[0].SemanticName = "POSITION";
	particleInputElementDescs[0].SemanticIndex = 0;
	particleInputElementDescs[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	particleInputElementDescs[0].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
	particleInputElementDescs[1].SemanticName = "TEXCOORD";
	particleInputElementDescs[1].SemanticIndex = 0;
	particleInputElementDescs[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	particleInputElementDescs[1].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
	particleInputElementDescs[2].SemanticName = "NORMAL";
	particleInputElementDescs[2].SemanticIndex = 0;
	particleInputElementDescs[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	particleInputElementDescs[2].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
	D3D12_INPUT_LAYOUT_DESC particleInputLayoutDesc{};
	particleInputLayoutDesc.pInputElementDescs = particleInputElementDescs;
	particleInputLayoutDesc.NumElements = _countof(particleInputElementDescs);

	// BlendStateの設定
	D3D12_BLEND_DESC particleBlendDesc{};
	// すべての色要素を書き込む
	particleBlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	//NormalBlendを行うための設定
	particleBlendDesc.RenderTarget[0].BlendEnable = TRUE;
	particleBlendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
	particleBlendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
	particleBlendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
	particleBlendDesc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
	particleBlendDesc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
	particleBlendDesc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;

	// RasiterzerStateの設定
	D3D12_RASTERIZER_DESC particleRasterizerDesc{};
	// 裏面（時計回り）を表示しない
	particleRasterizerDesc.CullMode = D3D12_CULL_MODE_BACK;
	// 三角形の中を塗りつぶす
	particleRasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;

	// Shaderをコンパイルする
	Microsoft::WRL::ComPtr<IDxcBlob> particleVertexShaderBlob = dxCommon->CompileShader(L"resources/shaders/Particle.VS.hlsl", L"vs_6_0", dxCommon->GetUtils(), dxCommon->GetCompiler(), dxCommon->GetHandler());
	assert(particleVertexShaderBlob != nullptr);

	Microsoft::WRL::ComPtr<IDxcBlob> particlePixelShaderBlob = dxCommon->CompileShader(L"resources/shaders/Particle.PS.hlsl", L"ps_6_0", dxCommon->GetUtils(), dxCommon->GetCompiler(), dxCommon->GetHandler());
	assert(particlePixelShaderBlob != nullptr);

	D3D12_GRAPHICS_PIPELINE_STATE_DESC particleGraphicsPipelineStateDesc{};
	particleGraphicsPipelineStateDesc.pRootSignature = particleRootSignature.Get();// RootSignature
	particleGraphicsPipelineStateDesc.InputLayout = particleInputLayoutDesc;// InputLayout
	particleGraphicsPipelineStateDesc.VS = { particleVertexShaderBlob->GetBufferPointer(), particleVertexShaderBlob->GetBufferSize() };// VertexShader
	particleGraphicsPipelineStateDesc.PS = { particlePixelShaderBlob->GetBufferPointer(), particlePixelShaderBlob->GetBufferSize() };// PixelShader
	particleGraphicsPipelineStateDesc.BlendState = particleBlendDesc;// BlendState
	particleGraphicsPipelineStateDesc.RasterizerState = particleRasterizerDesc;// RasterizerState
	// 書き込むRTVの情報
	particleGraphicsPipelineStateDesc.NumRenderTargets = 1;
	particleGraphicsPipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	// 利用するトポロジ（形状）のタイプ。三角形
	particleGraphicsPipelineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	// どのように画面に色を打ち込むかの設定（気にしなくて良い）
	particleGraphicsPipelineStateDesc.SampleDesc.Count = 1;
	particleGraphicsPipelineStateDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;//DepthStencilStateの設定
	D3D12_DEPTH_STENCIL_DESC particleDepthStencilDesc{};
	// Depthの機能を有効化する
	particleDepthStencilDesc.DepthEnable = true;
	// 書き込みします
	particleDepthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	//depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO; //Depthの書き込みを行わない
	// 比較関数はLessEqual。つまり、近ければ描画される
	particleDepthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;

	// DepthStencilの設定
	particleGraphicsPipelineStateDesc.DepthStencilState = particleDepthStencilDesc;
	particleGraphicsPipelineStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

	// 実際に生成
	particleGraphicsPipelineState = nullptr;
	pHr = dxCommon->GetDevice()->CreateGraphicsPipelineState(&particleGraphicsPipelineStateDesc, IID_PPV_ARGS(&particleGraphicsPipelineState));
	assert(SUCCEEDED(pHr));

}

void ParticleCommon::Bind(DirectXCommon* dxCommon)
{
	// RootSignatureを設定。PSOに設定しているけど別途設定が必要
	dxCommon->GetCommandList()->SetGraphicsRootSignature(particleRootSignature.Get());
	dxCommon->GetCommandList()->SetPipelineState(particleGraphicsPipelineState.Get());   // PSOを設定		
}
