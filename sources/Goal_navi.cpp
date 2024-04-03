#include "Goal_navi.h"
#include"objectManajer.h"
#include"camera.h"

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



   UpdateArrow_Front();
   UpdateTransform();
}

void Goal_navigation_Arrow::Render(RenderContext* rc)
{
    model->render(rc->deviceContext, Transform, 0.f, color);
}

void Goal_navigation_Arrow::UpdateArrow_Front()
{
   
    Camera& ince_c = Camera::instance();
    Objectmanajer& ince_obj = Objectmanajer::incetance();

    //player�̑���
    XMFLOAT3 camerapos{ ince_c.GetEye() };
   

    // �S�[���̈ʒu���擾
    XMFLOAT3 p = { 0.f,1.f,0.f }; // �f�t�H���g�̃S�[���̈ʒu
    XMFLOAT3 goalpos = { ince_obj.Select_GetGimic(Gimic_Type::Goal) != nullptr ?
                            ince_obj.Select_GetGimic(Gimic_Type::Goal)->GetPosition() : p };
    XMVECTOR QuaternionVec=XMLoadFloat4(&Quaternion);
    if (boot)
    {
     
    }   
    if (boot)
    {
        QuaternionVec = D_X_Vec(QuaternionVec, camerapos, goalpos);
        
    }
    XMStoreFloat4(&Quaternion, QuaternionVec);
    
    Position.x = camerapos.x + ince_c.GetFront().x * 1.5f;
    Position.y = camerapos.y + ince_c.GetFront().y * 1.5f;
    Position.z = camerapos.z + ince_c.GetFront().z * 1.5f;
  
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

    DirectX::XMMATRIX M=DirectX::XMMatrixRotationX(DirectX::XM_PIDIV2);
    //�X�P�[���s����쐬
    XMMATRIX S = XMMatrixScaling(Scale.x, Scale.y, Scale.z);
    //��]�s��쐬
    XMMATRIX R = DirectX::XMMatrixRotationQuaternion(DirectX::XMLoadFloat4(&Quaternion));
    //�ʒu�s����쐬
    XMMATRIX T = XMMatrixTranslation(Position.x, Position.y, Position.z);
    //4�̍s���g�ݍ��킹�āA���[���h���W���쐬
     M*= C *S * R * T;
    //�v�Z�������[���h���W�����o��
    DirectX::XMStoreFloat4x4(&Transform, M);
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
        else if(!boot)
        {
            boot = true;
        }
    } 
    XMFLOAT4 Q=Quaternion;
    ImGui::InputFloat("Quaternion.x", &Q.x, -10.f, 10.f);
    ImGui::InputFloat("Quaternion.y", &Q.y, -10.f, 10.f);
    ImGui::InputFloat("Quaternion.z", &Q.z, -10.f, 10.f);
    ImGui::InputFloat("Quaternion.w", &Q.w, -10.f, 10.f);
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

    }
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
    angle_ = acosf(DirectX::XMVectorGetX(DirectX::XMVector3Dot(forward_, d_)));


    //��]�p�iangle�j�������ȏꍇ�́A��]���s��Ȃ�
    if (fabs(angle_) > 1e-8f)
    {
        //�B��]���iaxis�j�Ɖ�]�p�iaxis�j�����]�N�I�[�^�j�I���iq�j�����߂�
        DirectX::XMVECTOR q;
        q = DirectX::XMQuaternionRotationAxis(axis_, angle_);

        //�C���̌��ݎp���iorientationVec�j�ɉ�]�N�I�[�^�j�I���iq�j����������
        //orientationVec = DirectX::XMQuaternionMultiply(orientationVec , q);
        XMVECTOR Q= DirectX::XMQuaternionMultiply(orientationVec_, q);
        XMFLOAT4 Qfloat4;
        XMFLOAT4 ofloat4;
        XMStoreFloat4(&Qfloat4,Q);
        XMStoreFloat4(&ofloat4,orientationVec_);

        //�D �C���R�����g�A�E�g���A�������X�ɖڕW���W�Ɍ�����
        //��DirectX::XMQuaternionSlerp�֐����g���i��ԃp�����[�^t��0.03�Ƃ���j
        return orientationVec_ = DirectX::XMQuaternionSlerp(orientationVec_,Q, 0.06f);
    }

    return orientationVec_;
#endif

    
}
