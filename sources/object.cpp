#include "object.h"
#include"StageManager.h"
#include"objectManajer.h"
#include"variable_management_class_for_hit_test.h"
using namespace DirectX;

void Object::box_Collition_obj()
{

    Objectmanajer& ince = Objectmanajer::incetance();
    int count = ince.Get_GameGimicCount();
    for (int i = 0; i < count; i++)
    {
        Object* obj = ince.Get_GameGimic(i);

        if (obj == this)continue;
        if (ince.Bounding_Box_vs_Bounding_Box(this, obj, false, 0.045f))
        {
            break;
        }
    }
}

void Object::RayCastGround()
{
 
    
    StageManager& ince_st = StageManager::incetance();
    VMCFHT& ince_vf = VMCFHT::instance();
    XMFLOAT3 normal = GetNormal();;
    float legth = 5.0f;
    XMFLOAT3 pos = Position;
    float start_point = Scale.y*0.005f;
    if (start_point < 0.1f) start_point = 0.1f;
    else if (start_point > 0.5f)start_point = 0.5f;
    pos.y += -start_point;
    ince_vf.update(pos, normal);
    collision_mesh* mesh = ince_st.GetStages(ince_st.GetNowStage())->GetModel()->Get_RaycastCollition();
    if (ince_vf.raycast(*mesh, ince_st.GetStages(ince_st.GetNowStage())->GetTransform(), result_intersection, legth))
    {
        Velocty.y =0.0f;
    }
    else
    {
        Position.y += Velocty.y;
    }
}

void Object::ObjType_effect(float elapsedTime)
{
    
    enum class Num
    {
        attribute1 = 0,
        attribute2
    };
    {
        switch (old_attribute_state[static_cast<int>(Num::attribute1)])
        {
        case ObjType::cution:

          
            break;
        case ObjType::Super_cution:

            break;
        case ObjType::Hard_to_Break:

            break;
        case ObjType::Super_hard_to_Break:


            break;
        case ObjType::heavy:


            break;
        case ObjType::Super_heavy:

            break;
        case ObjType::Fragile:
            if (this->Get_Original_Objtype(0) == ObjType::Crack)this->Destroy();
            break;
        case ObjType::Super_fragile:
            this->SetColor({0.f,0.f,0.f,0.f});
            break;
        case ObjType::Crack:
            
            break;
        case ObjType::null:

            break;
        default:
            break;
        }
    }

    {
        switch (old_attribute_state[static_cast<int>(Num::attribute2)])
        {
        case ObjType::cution:
            break;
        case ObjType::Super_cution:
            break;
        case ObjType::heavy:
            break;
        case ObjType::Super_heavy:
            break;
        case ObjType::Hard_to_Break:

            break;
        case ObjType::Super_hard_to_Break:

            break;
     
        case ObjType::Fragile:

            break;
        case ObjType::Super_fragile:
            this->SetColor({ 0.f,0.f,0.f,0.f });
            break;
        case ObjType::Crack:

            break;
        case ObjType::null:

            break;
        default:
            break;
        }
    }
}

void Object::Return_orijinal_ObjType(float elapsedTime)
{
    const int count = 2;

    for (int i = 0; i < count; i++)
    {
        if (original_attribute_state[i] == old_attribute_state[i])return;
        else
        {
            ReturnTimer[i] += elapsedTime;
            if (ReturnTimer[i] >= 10.0f)
            {
                old_attribute_state[i] = original_attribute_state[i];
                ReturnTimer[i] = 0.0f;
            }
        }
    }

}


void Object::UpdateTransform()
{
    const DirectX::XMFLOAT4X4 coordinate_system_transforms[]{
      { -1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 },	// 0:RHS Y-UP
      { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 },		// 1:LHS Y-UP
      { -1, 0, 0, 0, 0, 0, -1, 0, 0, 1, 0, 0, 0, 0, 0, 1 },	// 2:RHS Z-UP
      { 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1 },		// 3:LHS Z-UP
    };
    const float scale_factor = 0.01f;
    DirectX::XMMATRIX C{ DirectX::XMLoadFloat4x4(&coordinate_system_transforms[0]) * DirectX::XMMatrixScaling(scale_factor, scale_factor, scale_factor) };

    //スケール行列を作成
    XMMATRIX S = XMMatrixScaling(Scale.x, Scale.y, Scale.z);
    //回転行列作成
    /*XMMATRIX X = XMMatrixRotationX(Angle.x);
    XMMATRIX Y = XMMatrixRotationY(Angle.y);
    XMMATRIX Z = XMMatrixRotationZ(Angle.z);*/
    XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(Angle.x, Angle.y, Angle.z);
    //位置行列を作成
    XMMATRIX T = XMMatrixTranslation(Position.x, Position.y, Position.z);
    //4つの行列を組み合わせて、ワールド座標を作成
    DirectX::XMMATRIX W = C * S * R * T;
    //計算したワールド座標を取り出す
    DirectX::XMStoreFloat4x4(&Transform, W);

}

////移動処理
//void Object::Move(float vx, float vz, float speed)
//{
//    //移動方向ベクトルを設定
//    moveVecX = vx;
//    moveVecZ = vz;
//
//    //最大速度設定
//    maxMoveSpeed = speed;
//}
//
////旋回処理
//void Object::Turn(float elapsedTime, float vx, float vz, float speed)
//{
//    speed *= elapsedTime;
//    float length = sqrtf(vx * vx + vz * vz);
//    //進行ベクトルがゼロベクトルの場合は処理する必要なし
//    if (length < 0.001f)
//    {
//        return;
//    }
//
//    //進行ベクトルを単位化
//    vx /= length;
//    vz /= length;
//
//    //自身の回転値から前方向を求める
//    float frontX = sinf(Angle.y);//PLAYER
//    float frontZ = cosf(Angle.y);//PLAYER
//    //回転角を求めるために２つの単位ベクトルの外積を計算する
//    float dot = (frontX * vx) + (frontZ * vz);
//    float rot = 1.0f - dot;
//
//    if (rot > speed)
//    {
//        rot = speed;
//    }
//    //左右判定を行うために２つの単位ベクトルの外積を計算する
//    float croos = (frontZ * vx) - (frontX * vz);
//
//    //２Dの外積判定が正の場合か負の場合によって左右判定が行える
//    //左右判定を行う事によって左右判定を選択する
//    if (croos < 0.0f)
//    {
//        Angle.y += -rot;
//    }
//    else
//    {
//        Angle.y += rot;
//    }
//}
//
////ジャンプ処理
//void Object::Jump(float speed)
//{
//    //上方向の力を設定  
//    Velocty.y = speed;
//}
//
////速力処理更新
//void Object::UpdateVelocity(float elapsedTime)
//{
//    //経過フレーム
//    float elapsedFrame = 60.0f * elapsedTime;
//
//    //垂直速力更新処理
//    UpdateVerticalVelocity(elapsedFrame);
//
//    //水平速力更新処理
//    UpdateHorizontalVelocity(elapsedFrame);
//
//    //垂直移動更新処理
//    UpdateVerticalMove(elapsedTime);
//
//    //水平移動更新処理
//    UpdateHorizontalMove(elapsedTime);
//}
//
////垂直速力更新処理
//void Object::UpdateVerticalVelocity(float elapsedFrame)
//{
//    //重力処理
//    Velocty.y += gravity * elapsedFrame;
//}
//
////水平速力更新処理
//void Object::UpdateHorizontalVelocity(float elapsedFrame)
//{
//    //XZ平面の速力を減速する
//    float length = sqrtf(Velocty.x * Velocty.x + Velocty.z * Velocty.z);//DirectX::XMVectorGetX(DirectX::XMVector2LengthSq(velVel));
//    if (length > 0.0f)
//    {
//        //摩擦力
//        float friction = this->friction * elapsedFrame;
//
//        //空中にいるときは摩擦力を減らす
//        if (!isGround)friction *= airControl;
//
//        //摩擦による横方向の減速処理
//        if (length > friction)
//        {
//            float vx = Velocty.x / length;
//            float vz = Velocty.z / length;
//
//            Velocty.x -= vx * friction;
//            Velocty.z -= vz * friction;
//        }
//        else
//            //横方向の速力が摩擦力以下になったので速力無効化
//        {
//            //速度ベクトルを０にする
//            Velocty.x = 0.0f;
//            Velocty.z = 0.0f;
//        }
//    }
//
//    //XZ平面の速力を加速する
//    if (length <= maxMoveSpeed)
//    {
//        //移動ベクトルがゼロベクトルでないなら加速する
//        float moveVecLength = sqrtf(moveVecX * moveVecX + moveVecZ * moveVecZ);
//        if (moveVecLength > 0.0f)
//        {
//            //加速力
//            float acceleration = this->acceleration * elapsedFrame;
//
//            //空中にいるときは加速力を減らす
//            if (!isGround)acceleration *= airControl;
//
//            //移動ベクトルによる加速処理
//            Velocty.x += moveVecX * acceleration;
//            Velocty.z += moveVecZ * acceleration;
//
//            //最大速度制限
//            float length = sqrtf(Velocty.x * Velocty.x + Velocty.z * Velocty.z);
//            if (length > maxMoveSpeed)
//            {
//                float vx = Velocty.x / length;
//                float vz = Velocty.z / length;
//                Velocty.x = vx * maxMoveSpeed;
//                Velocty.z = vz * maxMoveSpeed;
//            }
//
//            //下り坂でがたがたしないようにする
//            if (isGround && slopeRate > 0.0f)
//            {
//                //斜面での落下速度-横移動の長さ*経過フレーム
//                Velocty.y -= length * slopeRate * elapsedFrame;
//            }
//        }
//    }
//    //移動ベクトルをリセット
//    moveVecX = 0.0f;
//    moveVecZ = 0.0f;
//}
//
////垂直移動更新処理
//void Object::UpdateVerticalMove(float elapsedTime)
//{
//    //垂直方向の移動量
//    float my = Velocty.y * elapsedTime;
//
//    slopeRate = 0.0f;
//
//    //キャラクターのY軸方向となる法線ベクトル
//    DirectX::XMFLOAT3 normal = { 0,1,0 };
//
//    //落下中
//    if (my < 0.0f)
//    {
//        //レイの開始位置は足元より少し上
//        DirectX::XMFLOAT3 start = { Position.x,Position.y + stepOffset,Position.z };
//        //レイの終点位置は移動後の位置
//        DirectX::XMFLOAT3 end = { Position.x,Position.y + my,Position.z };
//
//        //レイキャストによる地面判定
//        HitResult hit;
//        //ステージ完成までコメント
//        //if (StageManager::Instance().RayCast(start, end, hit))
//        //{
//        //    //法線ベクトル取得
//        //    normal = hit.normal;
//
//        //    //地面に接他している
//        //    position = hit.position;
//
//
//        //    //回転
//        //    angle.y -= hit.rotation.y;
//
//        //    //着地した
//        //    if (!isGround)
//        //    {
//        //        OnLanding();
//        //    }
//        //    isGround = true;
//        //    velocity.y = 0.0f;
//
//        //    //傾斜率の計算
//        //    float normalLengthXZ = sqrtf(hit.normal.x * hit.normal.x + hit.normal.z * hit.normal.z);
//        //    slopeRate = 1.0f - (hit.normal.y / (normalLengthXZ + hit.normal.y));
//        //}
//        //else
//        //{
//        //    //空中に浮いている
//        //    position.y += my;
//        //    isGround = false;
//        //}
//    }
//    //上昇中
//    else if (my > 0.0f)
//    {
//        Position.y += my;
//        isGround = false;
//    }
//
//    //地面の向きに添うようにXZ軸回転
//    {
//        //Y軸が法線ベクトル方向に向くオイラー角回転を算出する
//        //angle.x =  atan2f(normal.z, normal.y);//normal.z / normal.y;
//        //angle.z = -atan2f(normal.x,normal.y);//normal.x / normal.y;
//
//        float x = atan2f(normal.z, normal.y);//normal.z / normal.y;
//        float z = -atan2f(normal.x, normal.y);//normal.x / normal.y;
//
//        //線形補完で滑らかにする
//        Angle.x = Mathf::Leap(Angle.x, x, 0.1f);
//        Angle.z = Mathf::Leap(Angle.z, z, 0.1f);
//    }
//}
//
////水平移動更新処理
//void Object::UpdateHorizontalMove(float elapsedTime)
//{
//    //水平速力量計算
//    float velocityLengthXZ = sqrtf(Velocty.x * Velocty.x + Velocty.z * Velocty.z);
//    if (velocityLengthXZ > 0.0f)
//    {
//        //水平移動値
//        float mx = Velocty.x * elapsedTime;
//        float mz = Velocty.z * elapsedTime;
//
//        //レイの開始位置と終点位置
//        DirectX::XMFLOAT3 start = { Position.x,Position.y + stepOffset,Position.z };
//        DirectX::XMFLOAT3 end = { Position.x + mx,Position.y + stepOffset,Position.z + mz };
//
//        //レイキャストによる壁判定
//        HitResult hit;
//        //if (StageManager::instance().RayCast(start, end, hit))
//        //{
//        //    //壁までのベクトル
//        //    DirectX::XMVECTOR Start = DirectX::XMLoadFloat3(&start);
//        //    DirectX::XMVECTOR End = DirectX::XMLoadFloat3(&end);
//        //    DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(End, Start);
//
//        //    //壁の法線
//        //    DirectX::XMVECTOR Normal = DirectX::XMLoadFloat3(&hit.normal);
//
//        //    //入射ベクトルを法線に射影
//        //    DirectX::XMVECTOR Dot = DirectX::XMVector3Dot(DirectX::XMVectorNegate(Vec), Normal);
//
//        //    //補正位置の計算
//        //    DirectX::XMVECTOR CollectPosition/*S*/ = DirectX::XMVectorMultiplyAdd(Normal, Dot, End);
//        //    DirectX::XMFLOAT3 collectPosition;//p
//        //    DirectX::XMStoreFloat3(&collectPosition, CollectPosition);
//
//        //    //反射ベクトル
//        //    //DirectX::XMVECTOR CollectPosition/*S*/ = DirectX::XMVectorMultiplyAdd(Normal, Dot, End);
//        //    //DirectX::XMFLOAT3 collectPosition;//p
//        //    //DirectX::XMStoreFloat3(&collectPosition, DirectX::XMVectorAdd(End, CollectPosition));
//        //    //DirectX::XMVECTOR reflect = DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&collectPosition), DirectX::XMLoadFloat3(&hit.position));
//
//        //    //壁ずり方向へレイキャスト
//        //    HitResult hit2;
//        //    if (!StageManager::Instance().RayCast(hit.position, collectPosition, hit2))
//        //    {
//        //        //壁ずる方向で壁に当たらなかったら補正位置に移動
//        //        position.x = collectPosition.x;
//        //        position.z = collectPosition.z;
//        //    }
//        //    else
//        //    {
//        //        position.x = hit2.position.x;
//        //        position.z = hit2.position.z;
//        //    }
//        //}
//        //else
//        //{
//        //    //移動
//        //    position.x += mx;
//        //    position.z += mz;
//        //}
//    }
//}