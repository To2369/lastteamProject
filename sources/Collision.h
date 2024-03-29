#pragma once
#include<DirectXMath.h>
#include"model.h"
//ヒット結果
struct HitResult
{
    DirectX::XMFLOAT3 position = { 0,0,0 };//レイとポリゴンの交点
    DirectX::XMFLOAT3 normal = { 0, 0, 0 };//衝突したポリゴンの法線ベクトル
    float distance = 0.0f;//レイの始点から交点までの距離
    int materialIndex = -1;//衝突したポリゴンのマテリアル番号
    DirectX::XMFLOAT3 rotation = { 0,0,0 };//回転量
};

class Collision
{
public:
    static bool intersectRayAndModel(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, const std::unique_ptr<Model>& model, HitResult& result);
};
