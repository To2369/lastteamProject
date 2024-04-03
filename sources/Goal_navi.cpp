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
    DirectX::XMFLOAT3 n(0, 1, 0);		//軸（正規化）
  
    float angle = 0 * PIDIV180;	//角度（ラジアン）

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

    //playerの代わり
    XMFLOAT3 camerapos{ ince_c.GetEye() };
   

    // ゴールの位置を取得
    XMFLOAT3 p = { 0.f,1.f,0.f }; // デフォルトのゴールの位置
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
    //スケール行列を作成
    XMMATRIX S = XMMatrixScaling(Scale.x, Scale.y, Scale.z);
    //回転行列作成
    XMMATRIX R = DirectX::XMMatrixRotationQuaternion(DirectX::XMLoadFloat4(&Quaternion));
    //位置行列を作成
    XMMATRIX T = XMMatrixTranslation(Position.x, Position.y, Position.z);
    //4つの行列を組み合わせて、ワールド座標を作成
     M*= C *S * R * T;
    //計算したワールド座標を取り出す
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

    //①矢印の現在位置（position）から、目標座標（target）への方向（d）を求める
    //※正規化しておくこと
    //XMFLOATからXMVectorへの変換の仕方
    DirectX::XMVECTOR targetvec_ = DirectX::XMLoadFloat3(&target);
    DirectX::XMVECTOR positionvec_ = DirectX::XMLoadFloat3(&position);
    DirectX::XMVECTOR d_ = DirectX::XMVectorSubtract(targetvec_, positionvec_);
    //正規化
    d_ = DirectX::XMVector3Normalize(d_);
    //②矢印の現在姿勢（orientation）における前方（forward）をホーム方向とし、
    //方向（d）への回転軸（axis）と回転角（angle）を求める
    DirectX::XMVECTOR axis_;//回転軸
    float angle_;			//回転角
    // 回転軸の計算
    axis_ = DirectX::XMVector3Cross(forward_, d_);
    axis_ = DirectX::XMVector3Normalize(axis_);

    // 回転角の計算
    angle_ = acosf(DirectX::XMVectorGetX(DirectX::XMVector3Dot(forward_, d_)));


    //回転角（angle）が微小な場合は、回転を行わない
    if (fabs(angle_) > 1e-8f)
    {
        //③回転軸（axis）と回転角（axis）から回転クオータニオン（q）を求める
        DirectX::XMVECTOR q;
        q = DirectX::XMQuaternionRotationAxis(axis_, angle_);

        //④矢印の現在姿勢（orientationVec）に回転クオータニオン（q）を合成する
        //orientationVec = DirectX::XMQuaternionMultiply(orientationVec , q);
        XMVECTOR Q= DirectX::XMQuaternionMultiply(orientationVec_, q);
        XMFLOAT4 Qfloat4;
        XMFLOAT4 ofloat4;
        XMStoreFloat4(&Qfloat4,Q);
        XMStoreFloat4(&ofloat4,orientationVec_);

        //⑤ ④をコメントアウトし、矢印を徐々に目標座標に向ける
        //※DirectX::XMQuaternionSlerp関数を使う（補間パラメータtは0.03とする）
        return orientationVec_ = DirectX::XMQuaternionSlerp(orientationVec_,Q, 0.06f);
    }

    return orientationVec_;
#endif

    
}
