#pragma once
#include<DirectXMath.h>
#include"model.h"
//�q�b�g����
struct HitResult
{
    DirectX::XMFLOAT3 position = { 0,0,0 };//���C�ƃ|���S���̌�_
    DirectX::XMFLOAT3 normal = { 0, 0, 0 };//�Փ˂����|���S���̖@���x�N�g��
    float distance = 0.0f;//���C�̎n�_�����_�܂ł̋���
    int materialIndex = -1;//�Փ˂����|���S���̃}�e���A���ԍ�
    DirectX::XMFLOAT3 rotation = { 0,0,0 };//��]��
};

class Collision
{
public:
    static bool intersectRayAndModel(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, const std::unique_ptr<Model>& model, HitResult& result);
};
