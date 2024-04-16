#include "hu.h"

#include "camera.h"

void WallCollide(PhysicsData& p)
{
	if (p.position.y < 0)
	{
		p.Collide({ 0.0f,1.0f,0.0f }, 0.0f, -p.position.y);
	}
}

void Hu::move(PhysicsData& p, float elapsed_time)
{

	Camera& camera = Camera::Instance();
	DirectX::XMFLOAT3 cameraFront = camera.GetFront();
	
	DirectX::XMVECTOR cameraFrontVec = DirectX::XMLoadFloat3(&cameraFront);
	DirectX::XMVECTOR normalizedCameraFront = DirectX::XMVector3Normalize(cameraFrontVec);

	float power = 20.0f;

	DirectX::XMFLOAT3 shotVec{ sinf(angle * 0.01745f) * power,0,-cosf(angle * 0.01745f) * power };

	shotVec.y = 20.0f;
	
	p.AddForce(DirectX::XMVectorScale(DirectX::XMLoadFloat3(&p.velocity),-0.01f));
	if (state_timer >= 5.0f)p.AddForce(DirectX::XMLoadFloat3(&shotVec));

	DirectX::XMVECTOR gravity{ 0.0f,-9.8f,0.0f };
	p.AddForce(DirectX::XMVectorScale(gravity, p.mass));

	WallCollide(p);

	if (state_timer < 5.0f) { 
		state_timer += elapsed_time;
		return;
	}


	state_timer = 0.0f;
}
