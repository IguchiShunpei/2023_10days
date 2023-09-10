#pragma once

#include "Sprite.h"
#include "WinApp.h"

class PostEffect : public Sprite
{
private:
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

private:
	struct ConstBufferData {
		DirectX::XMFLOAT4 color; // �F (RGBA)
		DirectX::XMMATRIX mat; // ���W
		bool isBlur; // �u���[�t���O
		float alpha; // �A���t�@�l
	};

public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	PostEffect();

	/// <summary>
	/// ������
	/// </summary>
	void Initialize();

	/// <summary>
	/// �`��R�}���h
	/// </summary>
	/// <param name="cmdList">�R�}���h���X�g</param>
	void Draw(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// �V�[���`��O����
	/// </summary>
	/// <param name="cmdList">�R�}���h���X�g</param>
	void PreDraw(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// �V�[���`��㏈��
	/// </summary>
	/// <param name="cmdList">�R�}���h���X�g</param>
	void PostDraw(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// �p�C�v���C������
	/// </summary>
	void CreateGraphicsPipelineState();

public:
	void SetColor(const DirectX::XMFLOAT4& color);
	void SetBlur(const bool& blur);
	void SetAlpha(const float& alpha);

	DirectX::XMFLOAT4 GetColor() const { return color_; }

private:
	// �e�N�X�`���o�b�t�@
	ComPtr<ID3D12Resource> texBuff;
	// SRV�p�f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> descHeapSRV;
	// �k�x�o�b�t�@
	ComPtr<ID3D12Resource> depthBuff;
	// RTV�p�f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> descHeapRTV;
	// DSV�p�f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> descHeapDSV;
	// �O���t�B�N�X�p�C�v���C��
	ComPtr<ID3D12PipelineState> pipelineState;
	// ���[�g�V�O�l�`��
	ComPtr<ID3D12RootSignature> rootSignature;
	ComPtr<ID3D12Device> device;
	ComPtr<ID3D12GraphicsCommandList> cmdList;
	ConstBufferData* constMap = nullptr;

protected:
	// ��ʃN���A�J���[
	static const float clearColor[4];
	// ���_��
	static const int vertNum = 4;
	// �F
	DirectX::XMFLOAT4 color_ = { 1,1,1,1 };
	// �u���[�t���O
	bool isBlur_ = false;
	// �A���t�@�l
	float alpha_ = 1.0f;
};