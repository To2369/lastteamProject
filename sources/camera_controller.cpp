#include "camera_controller.h"
#include "Camera.h"
#include "Input/gamepad.h"

//更新処理
void CameraController::Update(float elapsedTime)
{
	if (angle.x < minAngleX)
	{
		angle.x = minAngleX;
	}
	if (angle.x > maxAngleX)
	{
		angle.x = maxAngleX;
	}
	
	//カメラ回転値を回転行列に変換
	DirectX::XMMATRIX Transform = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);

	//回転行列から前方向ベクトルを取り出す
	DirectX::XMVECTOR Front = Transform.r[2];//行列の3行目のデータを取り出している
	DirectX::XMFLOAT3 front;
	DirectX::XMStoreFloat3(&front, Front);

	//位置からどの方向にターゲットがいるか
	DirectX::XMFLOAT3 eye;
	eye.x = target.x - front.x * range;
	eye.y = target.y - front.y * range;
	eye.z = target.z - front.z * range;

	//カメラの視点と注視点を設定
	Camera::instance().SetLookAt(eye, target, DirectX::XMFLOAT3(0, 1, 0));
}