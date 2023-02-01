#pragma once
#include <dxgi1_6.h>
#include <vector>
#include <string>
#include "WindowsApp.h"
#include <DirectXMath.h>
#include <DirectXTex.h>
#include "KeyboardInput.h"
using namespace DirectX;
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#include"Sound.h"
#include <wrl.h>
using namespace Microsoft::WRL;
#include<chrono>

class Directx final
{
private:
	//���ʕ��n
	ComPtr<IDXGIFactory7> dxgiFactory = nullptr;
	ComPtr < ID3D12CommandAllocator> commandAllocator = nullptr;
	ComPtr < ID3D12CommandQueue> commandQueue = nullptr;
	// �X���b�v�`�F�[���̐ݒ�
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};
	ComPtr<IDXGISwapChain4> swapChain = nullptr;
	// �f�X�N���v�^�q�[�v�̐ݒ�
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc{};
	ComPtr < ID3D12DescriptorHeap> rtvHeap = nullptr;
	// 2.�`���̕ύX
		// �����_�[�^�[�Q�b�g�r���[�̃n���h�����擾
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle;
	// �o�b�N�o�b�t�@
	std::vector< ComPtr <ID3D12Resource>> backBuffers;
	//
	ComPtr < ID3D12DescriptorHeap> dsvHeap = nullptr;

	// �t�F���X�̐���
	ComPtr < ID3D12Fence> fence = nullptr;
	UINT64 fenceVal = 0;

	// 1.���\�[�X�o���A�ŏ������݉\�ɕύX
	D3D12_RESOURCE_BARRIER barrierDesc{};



	//�O���ŎQ��
	ComPtr<ID3D12Device> device = nullptr;
	ComPtr < ID3D12GraphicsCommandList> commandList = nullptr;

	//�L�^���ԁiFPS�Œ�
	std::chrono::steady_clock::time_point reference_;

	FLOAT clearColor[4] = { 0.0f,0.0f,0.01f,0.0f };

	//�|�X�g�G�t�F�N�g�p
	ComPtr<ID3D12Resource> _peraResource;
	ComPtr<ID3D12DescriptorHeap> _peraRTVHeap;//�����_�[�^�[�Q�b�g�p
	ComPtr<ID3D12DescriptorHeap> _peraSRVHeap;//�e�N�X�`���p
	bool isPeraClear = false;

private:
	Directx();
	~Directx();

	//�������n
	void InitializeDevice();
	void InitializeCommand();
	void InitializeSwapchain();
	void InitializeRendertargetView();
	void InitializeDepthBuffer();
	void InitializeFence();

	//FPS�Œ菉����
	void InitializeFixFPS();
	//FPS�Œ�X�V
	void UpdateFixFPS();

public:
	//
	HRESULT result;


	//�R�s�[�R���X�g���N�^�𖳌�
	Directx(const Directx& obj) = delete;
	//������Z�q��
	Directx& operator=(const Directx& obj) = delete;

	//�֐�
	void Initialize();

	static Directx& GetInstance();

	void DrawInitialize();

	void DrawUpdate(const XMFLOAT4& winRGBA = { 0.0f,0.0f,0.01f,0.0f });

	void DrawUpdate2();

	void PostDrawToPera();

	void PreDrawToPera();

	//getter
	ID3D12Device* GetDevice() const { return device.Get(); }
	ID3D12GraphicsCommandList* GetCommandList() const { return commandList.Get(); }
	ComPtr<ID3D12DescriptorHeap> GetPeraSRVHeap() const { return _peraSRVHeap; }

	//�o�b�N�o�b�t�@�̐����擾
	size_t GetBackBufferCount() const { return backBuffers.size(); }
};


void ResourceProperties(D3D12_RESOURCE_DESC& resDesc, const UINT& size);

void BuffProperties(D3D12_HEAP_PROPERTIES& heap, D3D12_RESOURCE_DESC& resource,
	ID3D12Resource** buff);