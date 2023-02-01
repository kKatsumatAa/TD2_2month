#include "PostPera.h"
#include <d3dx12.h>

void PostPera::Initialize()
{
	//�萔�o�b�t�@0��(��ʌ��ʃt���O)
	rootParams[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;//�萔�o�b�t�@�r���[
	rootParams[0].Descriptor.ShaderRegister = 0;//�萔�o�b�t�@�ԍ�(b0)
	rootParams[0].Descriptor.RegisterSpace = 0;//�f�t�H���g�l
	rootParams[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;//�S�ẴV�F�[�_���猩����

	//��ʌ��ʗp
	{
		//�q�[�v�ݒ�
		D3D12_HEAP_PROPERTIES cbHeapProp{};
		cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;//GPU�ւ̓]���p
		//���\�[�X�ݒ�
		D3D12_RESOURCE_DESC cbResourceDesc{};
		cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;//GPU�ւ̓]���p
		//���\�[�X�ݒ�
		ResourceProperties(cbResourceDesc,
			((UINT)sizeof(EffectConstBuffer) + 0xff) & ~0xff/*256�o�C�g�A���C�����g*/);
		//�萔�o�b�t�@�̐���
		BuffProperties(cbHeapProp, cbResourceDesc, &effectFlagsBuff);
		//�萔�o�b�t�@�̃}�b�s���O
		Directx::GetInstance().result = effectFlagsBuff->Map(0, nullptr, (void**)&mapEffectFlagsBuff);//�}�b�s���O
		assert(SUCCEEDED(Directx::GetInstance().result));
	}


	//�o�b�t�@
	auto resDesc = CD3DX12_RESOURCE_DESC::Buffer(sizeof(pv));

	D3D12_HEAP_PROPERTIES heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);

	auto result = Directx::GetInstance().GetDevice()->CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&resDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(_peraVB.ReleaseAndGetAddressOf())
	);

	//�r���[
	_peraVBV.BufferLocation = _peraVB->GetGPUVirtualAddress();
	_peraVBV.SizeInBytes = sizeof(pv);
	_peraVBV.StrideInBytes = sizeof(PeraVertex);

	//�}�b�v
	PeraVertex* mappedPera = nullptr;
	_peraVB->Map(0, nullptr, (void**)&mappedPera);
	std::copy(std::begin(pv), std::end(pv), mappedPera);
	_peraVB->Unmap(0, nullptr);

	GenerateRSPL();
}

void PostPera::GenerateRSPL()
{
	D3D12_INPUT_ELEMENT_DESC layout[2] =
	{
		{
			"POSITION",
			0,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
			0
		},
		{
			"TEXCOORD",
			0,
			DXGI_FORMAT_R32G32_FLOAT,
			0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
			0
		},
	};

	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpsDesc = {};
	gpsDesc.InputLayout.NumElements = _countof(layout);
	gpsDesc.InputLayout.pInputElementDescs = layout;


	//pipeline
	//���_
	HRESULT result = D3DCompileFromFile(
		L"Resources/shaders/PeraVertexShader.hlsl",
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"VS",
		"vs_5_0",
		0,
		0,
		vs.ReleaseAndGetAddressOf(),
		errBlob.ReleaseAndGetAddressOf()
	);
	assert(SUCCEEDED(result));
	//�s�N�Z��
	result = D3DCompileFromFile(
		L"Resources/shaders/PeraPixelShader.hlsl",
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"PS",
		"ps_5_0",
		0,
		0,
		ps.ReleaseAndGetAddressOf(),
		errBlob.ReleaseAndGetAddressOf()
	);
	assert(SUCCEEDED(result));

	gpsDesc.VS.pShaderBytecode = vs->GetBufferPointer();
	gpsDesc.VS.BytecodeLength = vs->GetBufferSize();
	gpsDesc.PS.pShaderBytecode = ps->GetBufferPointer();
	gpsDesc.PS.BytecodeLength = ps->GetBufferSize();

	gpsDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	gpsDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	gpsDesc.NumRenderTargets = 1;
	gpsDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	gpsDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	gpsDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
	gpsDesc.SampleDesc.Count = 1;
	gpsDesc.SampleDesc.Quality = 0;
	gpsDesc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;

	D3D12_ROOT_SIGNATURE_DESC rsDesc = {};
	rsDesc.NumParameters = _countof(rootParams);//���[�g�p�����[�^��;
	rsDesc.NumStaticSamplers = 0;
	rsDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	//rootsig

	D3D12_DESCRIPTOR_RANGE range = {};
	range.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;//t
	range.BaseShaderRegister = 0;//0
	range.NumDescriptors = 1;

	D3D12_ROOT_PARAMETER rp[2] = {};
	rp[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rp[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	rp[0].DescriptorTable.pDescriptorRanges = &range;
	rp[0].DescriptorTable.NumDescriptorRanges = 1;
	rp[1] = rootParams[0];


	D3D12_STATIC_SAMPLER_DESC sampler = CD3DX12_STATIC_SAMPLER_DESC(0); //s0

	//�f�X�N���v�^�e�[�u��+���[�g�p�����[�^
	rsDesc.NumParameters = 1 + _countof(rootParams);
	rsDesc.pParameters = rp;
	rsDesc.NumStaticSamplers = 1;
	rsDesc.pStaticSamplers = &sampler;


	ComPtr<ID3DBlob>rsBlob;
	result = D3D12SerializeRootSignature(
		&rsDesc,
		D3D_ROOT_SIGNATURE_VERSION_1,
		rsBlob.ReleaseAndGetAddressOf(),
		errBlob.ReleaseAndGetAddressOf()
	);
	assert(SUCCEEDED(result));

	result = Directx::GetInstance().GetDevice()->CreateRootSignature(
		0,
		rsBlob.Get()->GetBufferPointer(),
		rsBlob.Get()->GetBufferSize(),
		IID_PPV_ARGS(_peraRS.ReleaseAndGetAddressOf())
	);
	assert(SUCCEEDED(result));

	//�p�C�v���C���X�e�[�g
	gpsDesc.pRootSignature = _peraRS.Get();
	result = Directx::GetInstance().GetDevice()->CreateGraphicsPipelineState(
		&gpsDesc,
		IID_PPV_ARGS(_peraPipeline.GetAddressOf())
	);
	assert(SUCCEEDED(result));
}

void PostPera::Draw(EffectConstBuffer effectFlags)
{
	this->mapEffectFlagsBuff->isEmboss = effectFlags.isEmboss;
	this->mapEffectFlagsBuff->isFog = effectFlags.isFog;
	this->mapEffectFlagsBuff->isGaussian = effectFlags.isGaussian;
	this->mapEffectFlagsBuff->isGaussian2 = effectFlags.isGaussian2;
	this->mapEffectFlagsBuff->isGradation = effectFlags.isGradation;
	this->mapEffectFlagsBuff->isOutLine = effectFlags.isOutLine;
	this->mapEffectFlagsBuff->isSharpness = effectFlags.isSharpness;


	Directx::GetInstance().GetCommandList()->SetGraphicsRootSignature(_peraRS.Get());
	Directx::GetInstance().GetCommandList()->SetPipelineState(_peraPipeline.Get());
	//�q�[�v���Z�b�g
	Directx::GetInstance().GetCommandList()
		->SetDescriptorHeaps(1, Directx::GetInstance().GetPeraSRVHeap().GetAddressOf());

	auto handle = Directx::GetInstance().GetPeraSRVHeap()->GetGPUDescriptorHandleForHeapStart();
	//�p�����[�^�[0�Ԃƃq�[�v���֘A�t����
	Directx::GetInstance().GetCommandList()->SetGraphicsRootDescriptorTable(0, handle);

	Directx::GetInstance().GetCommandList()->IASetPrimitiveTopology(
		D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	Directx::GetInstance().GetCommandList()->IASetVertexBuffers(0, 1, &_peraVBV);

	//index0�͂ŃX�N���v�^�e�[�u��
	Directx::GetInstance().GetCommandList()->SetGraphicsRootConstantBufferView(1, effectFlagsBuff->GetGPUVirtualAddress());

	Directx::GetInstance().GetCommandList()->DrawInstanced(4, 1, 0, 0);
}
