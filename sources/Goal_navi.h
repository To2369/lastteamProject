#pragma once
#include"object.h"

class Goal_navigation_Arrow:public Object
{
public:
    Goal_navigation_Arrow(ID3D11Device*dc);

    void Update(float elapsedTime)override;
    void Render(RenderContext* rc)override;
public:
    void SetPlayerPos(DirectX::XMFLOAT3& pl) { playerPos = pl; };
    void SetGoalPos(DirectX::XMFLOAT3& go) { GoalPos = go; };
    void UpdateArrow_Front();
    void UpdateTransform()override;
    DirectX::XMVECTOR D_X_Vec(DirectX::XMVECTOR orientationVec_, DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 target);
    void Gui()override;
private:
    bool boot=true;
    DirectX::XMFLOAT3 playerPos;
    DirectX::XMFLOAT3 GoalPos;
    DirectX::XMFLOAT4 Quaternion{};//例外としてこのオブジェクトはクウォータ二オンを回転として使用する
    const float PIDIV180 = 0.017452f;	// PAI/180
    const char* filename = ".\\resources\\Goal\\Arrow.fbx";
    float turnSpeed = 0;
    float TurnSpeed = 1.5f;
};
