#pragma once

#include <DirectXMath.h>

//カメラコントローラー
class CameraController
{
public:
	CameraController() {}
	~CameraController() {}

	//更新処理
	void Update(float elapsedTime);

	//ターゲット位置設定
	//void SetTarget(const DirectX::XMFLOAT3& target) { this->target = target; }

	void SetEye(const DirectX::XMFLOAT3& eye) { this->eye = eye; }

	void SetAngle(const DirectX::XMFLOAT3& angle) { 
		this->angle.x = DirectX::XMConvertToRadians(angle.x);
		this->angle.y = DirectX::XMConvertToRadians(angle.y);
		this->angle.z = DirectX::XMConvertToRadians(angle.z);
	}

	void AddAngle(const DirectX::XMFLOAT3& angle) {
		this->angle.x += angle.x;
		this->angle.y += angle.y;
		this->angle.z += angle.z;
	}

	DirectX::XMFLOAT3 GetAngle() { return angle; }

	float GetMinAngle() { return minAngleX; }
	float GetMaxAngle() { return maxAngleX; }

private:
	DirectX::XMFLOAT3 eye = { 0,0,0 };
	//DirectX::XMFLOAT3 target = { 0,0,0 };
	DirectX::XMFLOAT3 angle = { 0,0,0 };
	float rollSpeed = DirectX::XMConvertToRadians(10);
	float range = 20.0f;
	float maxAngleX = DirectX::XMConvertToRadians(80);
	float minAngleX = DirectX::XMConvertToRadians(-80);
};