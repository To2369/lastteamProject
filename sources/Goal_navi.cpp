#include "Goal_navi.h"
#include"objectManajer.h"
#include"camera.h"
#include "Graphics/graphics.h"
#ifdef USE_IMGUI
#include "../imgui/imgui.h"
#include "../imgui/imgui_internal.h"
#include "../imgui/imgui_impl_dx11.h"
#include "../imgui/imgui_impl_win32.h"
#endif
using namespace DirectX;
Goal_navigation_Arrow::Goal_navigation_Arrow(ID3D11Device* dc)
{
    model = make_unique<Model>(dc, filename, true);
    Scale = { 0.1f,0.1f,0.1f };
    //  Angle.x = Angle.y = Angle.z = 0.0f;
    DirectX::XMFLOAT3 n(0, 1, 0);		//���i���K���j

    float angle = 0 * PIDIV180;	//�p�x�i���W�A���j

    Quaternion = {
        sinf(angle / 2) * n.x,
        sinf(angle / 2) * n.y,
        sinf(angle / 2) * n.z,
        cosf(angle / 2)
    };
}

void Goal_navigation_Arrow::Update(float elapsedTime)
{
    turnSpeed = TurnSpeed * elapsedTime;

    XMFLOAT3 cameraeye{ Camera::instance().GetEye() };
    XMFLOAT3 camerafront{ Camera::instance().GetFront() };
    cameraeye.z += camerafront.z * 1.3f;
    cameraeye.x += camerafront.x * 1.3f;
    cameraeye.y += camerafront.y * 1.3f + 0.5f;
    //if(cameraeye.y 
   // Position = cameraeye;


    UpdateArrow_Front();
    UpdateTransform();
}

void Goal_navigation_Arrow::Render(RenderContext* rc)
{
    model->render(Graphics::Instance().GetDeviceContext(), Transform, 0.f, color);
}
void Goal_navigation_Arrow::UpdateArrow_Front()
{
    using namespace DirectX;

    DirectX::XMVECTOR quaVec = DirectX::XMLoadFloat4(&Quaternion);
    DirectX::XMMATRIX quaternionM = DirectX::XMMatrixRotationQuaternion(quaVec);
    DirectX::XMVECTOR Pos = DirectX::XMLoadFloat3(&Position);

    // �S�[���̈ʒu���擾
    Objectmanajer& ince_obj = Objectmanajer::incetance();
    XMFLOAT3 p = { 0.f,1.f,0.f }; // �f�t�H���g�̃S�[���̈ʒu
    XMFLOAT3 goalpos = { ince_obj.Select_GetGimic(Gimic_Type::Goal) != nullptr ?
                            ince_obj.Select_GetGimic(Gimic_Type::Goal)->GetPosition() : p };

    DirectX::XMVECTOR GoalPos = DirectX::XMLoadFloat3(&goalpos);
    DirectX::XMVECTOR TargetVec = DirectX::XMVectorSubtract(GoalPos, Pos);

    DirectX::XMVECTOR Front = quaternionM.r[2];
    DirectX::XMVECTOR cross = DirectX::XMVector3Cross(TargetVec, Front);
    float ang = DirectX::XMVectorGetX(DirectX::XMVector3Dot(DirectX::XMVector3Normalize(Front), DirectX::XMVector3Normalize(TargetVec)));
    if (cross.m128_f32[0] == 0 && cross.m128_f32[1] == 0 && cross.m128_f32[2] == 0)return;

    DirectX::XMVECTOR qua = DirectX::XMQuaternionRotationAxis(cross, acosf(ang));
    DirectX::XMVECTOR Goal = DirectX::XMQuaternionMultiply(quaVec, qua);
    Goal = { 0,Goal.m128_f32[1],0,Goal.m128_f32[3] };
    DirectX::XMVECTOR slerpvec = (DirectX::XMQuaternionSlerp(quaVec, Goal, 0.02f));

    DirectX::XMStoreFloat4(&Quaternion, slerpvec);


}

void Goal_navigation_Arrow::UpdateTransform()
{
    const DirectX::XMFLOAT4X4 coordinate_system_transforms[]{
    { -1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 },	// 0:RHS Y-UP
    { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 },		// 1:LHS Y-UP
    { -1, 0, 0, 0, 0, 0, -1, 0, 0, 1, 0, 0, 0, 0, 0, 1 },	// 2:RHS Z-UP
    { 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1 },		// 3:LHS Z-UP
    };
    const float scale_factor = 0.01f;
    DirectX::XMMATRIX C{ DirectX::XMLoadFloat4x4(&coordinate_system_transforms[0]) * DirectX::XMMatrixScaling(scale_factor, scale_factor, scale_factor) };

    //�X�P�[���s����쐬
    XMMATRIX S = XMMatrixScaling(Scale.x, Scale.y, Scale.z);
    //��]�s��쐬
    /*XMMATRIX X = XMMatrixRotationX(Angle.x);
    XMMATRIX Y = XMMatrixRotationY(Angle.y);
    XMMATRIX Z = XMMatrixRotationZ(Angle.z);*/
    XMMATRIX R = DirectX::XMMatrixRotationQuaternion(DirectX::XMLoadFloat4(&Quaternion));
    //�ʒu�s����쐬
    XMMATRIX T = XMMatrixTranslation(Position.x, Position.y, Position.z);
    //4�̍s���g�ݍ��킹�āA���[���h���W���쐬
    DirectX::XMMATRIX W = C * S * R * T;
    //�v�Z�������[���h���W�����o��
    DirectX::XMStoreFloat4x4(&Transform, W);
}
void Goal_navigation_Arrow::Gui()
{
    //static XMFLOAT3 ang{};
    using namespace ImGui;
    if (Button("GoalAngle"))
    {
        if (boot)
        {
            boot = false;
        }
        else if (!boot)
        {
            boot = true;
        }
    }
   // SliderFloat("", &ang, 0.f, 10.f);
    SliderFloat("Quaternion.x", &Quaternion.x, -10.f, 10.f);
    SliderFloat("Quaternion.y", &Quaternion.y, -10.f, 10.f);
    SliderFloat("Quaternion.z", &Quaternion.z, -10.f, 10.f);
    SliderFloat("Quaternion.w", &Quaternion.w, -10.f, 10.f);
    /*XMFLOAT4 Q=Quaternion;

    DirectX::XMVECTOR forward, right, up;
    DirectX::XMVECTOR QuaternionVec = DirectX::XMLoadFloat4(&Quaternion);

    DirectX::XMMATRIX m = DirectX::XMMatrixRotationQuaternion(QuaternionVec);
    DirectX::XMFLOAT4X4 m4x4 = {};
    DirectX::XMStoreFloat4x4(&m4x4, m);
    right = { m4x4._11, m4x4._12, m4x4._13 };
    up = { m4x4._21, m4x4._22, m4x4._23 };
    forward = { m4x4._31, m4x4._32, m4x4._33 };
    if (GetKeyState('I')<0)
    {
        DirectX::XMVECTOR q = DirectX::XMQuaternionRotationAxis(up, 1 * PIDIV180);
        QuaternionVec = DirectX::XMQuaternionMultiply(QuaternionVec, q);
        XMStoreFloat4(&Quaternion, QuaternionVec);

    }
    if (GetKeyState('K')<0)
    {
        DirectX::XMVECTOR q = DirectX::XMQuaternionRotationAxis(up, -1 * PIDIV180);
        QuaternionVec = DirectX::XMQuaternionMultiply(QuaternionVec, q);
        XMStoreFloat4(&Quaternion, QuaternionVec);

    }
    if (GetKeyState('L')<0)
    {
        DirectX::XMVECTOR q = DirectX::XMQuaternionRotationAxis(right, 1 * PIDIV180);
        QuaternionVec = DirectX::XMQuaternionMultiply(QuaternionVec, q);
        XMStoreFloat4(&Quaternion, QuaternionVec);

    }
    if (GetKeyState('J')<0)
    {
        DirectX::XMVECTOR q = DirectX::XMQuaternionRotationAxis(right, -1 * PIDIV180);
        QuaternionVec = DirectX::XMQuaternionMultiply(QuaternionVec, q);
        XMStoreFloat4(&Quaternion, QuaternionVec);

    }
    if (GetKeyState('N')<0)
    {
        DirectX::XMVECTOR q = DirectX::XMQuaternionRotationAxis(forward, 1 * PIDIV180);
        QuaternionVec = DirectX::XMQuaternionMultiply(QuaternionVec, q);
        XMStoreFloat4(&Quaternion, QuaternionVec);

    }
    if (GetKeyState('M')<0)
    {
        DirectX::XMVECTOR q = DirectX::XMQuaternionRotationAxis(forward, -1 * PIDIV180);
        QuaternionVec = DirectX::XMQuaternionMultiply(QuaternionVec, q);
        XMStoreFloat4(&Quaternion, QuaternionVec);

    }*/
    //ImGui::SliderFloat("change_angle.x",&ang.x,-100.f,100.f);
    //ImGui::SliderFloat("change_angle.y",&ang.y,-100.f,100.f);
    //ImGui::SliderFloat("change_angle.z",&ang.z,-100.f,100.f);
    ////ang.x = cosf(ang.x);
    //ang.y = XMConvertToRadians(ang.y);
    //ang.z = XMConvertToRadians(ang.z);
    //SetAngle(ang);

}


DirectX::XMVECTOR Goal_navigation_Arrow::D_X_Vec(DirectX::XMVECTOR orientationVec_, DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 target)
{
#if 1
    DirectX::XMVECTOR forward_;
    DirectX::XMFLOAT4X4 m4x4_ = {};
    DirectX::XMMATRIX m_ = DirectX::XMMatrixRotationQuaternion(orientationVec_);
    DirectX::XMStoreFloat4x4(&m4x4_, m_);
    forward_ = { m4x4_._31, m4x4_._32, m4x4_._33 };

    //�@���̌��݈ʒu�iposition�j����A�ڕW���W�itarget�j�ւ̕����id�j�����߂�
    //�����K�����Ă�������
    //XMFLOAT����XMVector�ւ̕ϊ��̎d��
    DirectX::XMVECTOR targetvec_ = DirectX::XMLoadFloat3(&target);
    DirectX::XMVECTOR positionvec_ = DirectX::XMLoadFloat3(&position);
    DirectX::XMVECTOR d_ = DirectX::XMVectorSubtract(targetvec_, positionvec_);
    //���K��
    d_ = DirectX::XMVector3Normalize(d_);
    //�A���̌��ݎp���iorientation�j�ɂ�����O���iforward�j���z�[�������Ƃ��A
    //�����id�j�ւ̉�]���iaxis�j�Ɖ�]�p�iangle�j�����߂�
    DirectX::XMVECTOR axis_;//��]��
    float angle_;			//��]�p
    // ��]���̌v�Z
    axis_ = DirectX::XMVector3Cross(forward_, d_);
    axis_ = DirectX::XMVector3Normalize(axis_);

    // ��]�p�̌v�Z
    angle_ = XMVectorGetX((DirectX::XMVector3AngleBetweenNormals(forward_, d_)));


    //��]�p�iangle�j�������ȏꍇ�́A��]���s��Ȃ�
    if (fabs(angle_) > 1e-8f)
    {
        //�B��]���iaxis�j�Ɖ�]�p�iaxis�j�����]�N�I�[�^�j�I���iq�j�����߂�
        DirectX::XMVECTOR q;
        q = DirectX::XMQuaternionRotationAxis(axis_, angle_);

        //�C���̌��ݎp���iorientationVec�j�ɉ�]�N�I�[�^�j�I���iq�j����������
        //orientationVec = DirectX::XMQuaternionMultiply(orientationVec , q);
        XMVECTOR Q = DirectX::XMQuaternionMultiply(orientationVec_, q);
        XMFLOAT4 Qfloat4;
        XMFLOAT4 ofloat4;
        XMStoreFloat4(&Qfloat4, Q);
        XMStoreFloat4(&ofloat4, orientationVec_);

        //�D �C���R�����g�A�E�g���A�������X�ɖڕW���W�Ɍ�����
        //��DirectX::XMQuaternionSlerp�֐����g���i��ԃp�����[�^t��0.03�Ƃ���j
        return orientationVec_ = DirectX::XMQuaternionSlerp(orientationVec_, Q, 0.06f);
    }

    return orientationVec_;
#endif


}
