#include"Character.h"
#include"StageManager.h"
#include"Mathf.h"
#include <objectManajer.h>
#include"variable_management_class_for_hit_test.h"
#include"camera_controller.h"
using namespace DirectX;
//行列更新処理
void Character::UpdateTransform()
{
    //スケール行列を作成
    DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
    DirectX::XMMATRIX SS = DirectX::XMMatrixScaling(Sscale.x, Sscale.y, Sscale.z);
    //回転行列を作成
    DirectX::XMMATRIX X = DirectX::XMMatrixRotationX(angle.x);
    DirectX::XMMATRIX Y = DirectX::XMMatrixRotationY(angle.y);
    DirectX::XMMATRIX Z = DirectX::XMMatrixRotationZ(angle.z);
    DirectX::XMMATRIX R = Y * X * Z;

    DirectX::XMMATRIX SX = DirectX::XMMatrixRotationX(Sangle.x);
    DirectX::XMMATRIX SY = DirectX::XMMatrixRotationY(Sangle.y);
    DirectX::XMMATRIX SZ = DirectX::XMMatrixRotationZ(Sangle.z);
    DirectX::XMMATRIX SR = SY * SX * SZ;
    //位置行列
    DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
    DirectX::XMMATRIX ST = DirectX::XMMatrixTranslation(Sposition.x, Sposition.y, Sposition.z);
    //３つの行列を組み合わせ、ワールド行列を作成
    DirectX::XMMATRIX W = S * R * T;
    DirectX::XMMATRIX SW = SS * SR * ST;
    // 計算したワールド行列を transform に取り出す
    DirectX::XMStoreFloat4x4(&transform, W);
    DirectX::XMStoreFloat4x4(&Stransform, SW);
}

//移動処理
void Character::Move(float vx, float vz, float speed)
{
   
    // 移動方向ベクトルを決定
    direction.x = vx;
    direction.z = vz;
    //velocity.x = vx;
    //velocity.z = vz;
    // 最大速度設定
    maxMoveSpeed = speed;
}

//ジャンプ処理
void Character::jump(float speed)
{
    //上方向の力設定
    velocity.y = speed;
}

//速度処理更新
void Character::updateVelocity(float elapsedTime)
{
    //垂直速度更新処理
    updateVerticalVelocity(elapsedTime);
    updateHorizontalVelocity(elapsedTime);

    //垂直移動更新処理
    updateVerticalMove(elapsedTime);
    updateHorizontalMove(elapsedTime);

}

//垂直速度更新処理
void Character::updateVerticalVelocity(float elapsedTime)
{
    //重力処理
    velocity.y += gravity * elapsedTime;
}

//垂直移動更新処理
void Character::updateVerticalMove(float elapsedTime)
{
    //垂直方向の移動量
    float moveY = velocity.y * elapsedTime;

    slopeRate = 0.0f;

    //キャラクターのY軸方向となる法線ベクトル
    DirectX::XMFLOAT3 normal = { 0,1,0 };
    if (moveY < 0.0f)
    {
        //移動処理
        position.y += velocity.y * elapsedTime;
        VMCFHT& ince_ray = VMCFHT::instance();
        Objectmanajer& ince_o = Objectmanajer::incetance();
        Gimic* road = nullptr;
        Gimic* lift = nullptr;
        int gimic_count = ince_o.Get_GameGimicCount();
        int object_count = ince_o.Get_GameObjCount();
        for (int i = 0; i < gimic_count; i++)
        {
            Gimic* obj = ince_o.Get_GameGimic(i);
            if (obj->Get_GimicType() == Gimic_Type::Drop_Road)
            {
                road = obj;
            }
           
        }
       
        XMFLOAT3 start{ position };
        //start.y += 0.1f;
        XMFLOAT3 end{ position };
        end.y -= 0.1f;
        HitResult hit;
        Ray_ObjType type = Ray_ObjType::Static_objects;
        Ray_ObjType type2 = Ray_ObjType::DaynamicObjects;
        Ray_ObjType type3 = Ray_ObjType::DynamicGimics;
        Ray_ObjType type4 = Ray_ObjType::Stage;
        //地面判定
        if (ince_ray.RayCast(start, end, hit, type))//静的オブジェクト
        {
            position.y = hit.position.y+0.1f;
            velocity.y = 0.0f;
            normal = hit.normal;
            //着地した
            if (!groundedFlag)
            {
                OnLanding();
            }
            groundedFlag = true;

            //傾斜率の計算
            float normalLengthXZ = sqrtf(hit.normal.x * hit.normal.x + hit.normal.z * hit.normal.z);
            slopeRate = 1.0f - (hit.normal.y / (normalLengthXZ + hit.normal.y));
        }
        else if (road)
        {
            if (ince_ray.raycast(start, end, road->GetModel(), hit, road->GetTransform()))
            {
                position.y = hit.position.y + 0.1f;
                velocity.y = 0.0f;
                normal = hit.normal;
                //着地した
                if (!groundedFlag)
                {
                    OnLanding();
                }
                groundedFlag = true;

                //傾斜率の計算
                float normalLengthXZ = sqrtf(hit.normal.x * hit.normal.x + hit.normal.z * hit.normal.z);
                slopeRate = 1.0f - (hit.normal.y / (normalLengthXZ + hit.normal.y));
            }
        }
        else if (ince_ray.RayCast(start, end, hit, type2))//ゲームオブジェクトと
        {
            
            position.y = hit.position.y + 0.1f;
            velocity.y = 0.0f;
            normal = hit.normal;
            //着地した
            if (!groundedFlag)
            {
                OnLanding();
            }
            groundedFlag = true;

            //傾斜率の計算
            float normalLengthXZ = sqrtf(hit.normal.x * hit.normal.x + hit.normal.z * hit.normal.z);
            slopeRate = 1.0f - (hit.normal.y / (normalLengthXZ + hit.normal.y));
            
        }
        else if (ince_ray.RayCast(start, end, hit,type3))//ギミックと
        {
           
            position.y = hit.position.y + 0.1f;
            velocity.y = 0.0f;
            normal = hit.normal;
            //着地した
            if (!groundedFlag)
            {
                OnLanding();
            }
            groundedFlag = true;

            //傾斜率の計算
            float normalLengthXZ = sqrtf(hit.normal.x * hit.normal.x + hit.normal.z * hit.normal.z);
            slopeRate = 1.0f - (hit.normal.y / (normalLengthXZ + hit.normal.y));
            
        }
        else if (ince_ray.RayCast(start, end, hit, type4))//ステージ
        {

            position.y = hit.position.y + 0.1f;
            velocity.y = 0.0f;
            normal = hit.normal;
            //着地した
            if (!groundedFlag)
            {
                OnLanding();
            }
            groundedFlag = true;

            //傾斜率の計算
            float normalLengthXZ = sqrtf(hit.normal.x * hit.normal.x + hit.normal.z * hit.normal.z);
            slopeRate = 1.0f - (hit.normal.y / (normalLengthXZ + hit.normal.y));

        }
        else
        {
            //空中に浮いている
            position.y += moveY;
            groundedFlag = false;
        }
        
    }
    else if (moveY > 0.0f)
    {
        //上昇中
        position.y += moveY;
        groundedFlag = false;
    }

    //地面の向きに添うようにXZ軸回転
    {
        float x = atan2f(normal.z, normal.y);//normal.z / normal.y;
        float z = -atan2f(normal.x, normal.y);//normal.x / normal.y;

        //線形補完で滑らかにする
        angle.x = Mathf::Leap(angle.x, x, 0.1f);
        angle.z = Mathf::Leap(angle.z, z, 0.1f);
    }
}

//水平速度更新処理
void Character::updateHorizontalVelocity(float elapsedTime)
{
    // 速度に力が加わっていたら（0 じゃなかったら）減速処理を行う
    float length = sqrtf(velocity.x * velocity.x + velocity.z * velocity.z);
    if (length > 0.0f)
    {
        // 摩擦力（フレーム単位で計算）
        float friction = this->friction * elapsedTime;

        //空中にいるときは摩擦力を減少
        if (!groundedFlag)
        {
            friction*= airControl;
        }

        if (length > friction)
        {
            // 摩擦による横方向の減速処理
            float vx = velocity.x / length;
            float vz = velocity.z / length;

            velocity.x -= vx * friction;
            velocity.z -= vz * friction;
        }
        else
        {
            // 横方向の速度が摩擦力以下になったので速度を無効化
            velocity.x = 0.0f;
            velocity.z = 0.0f;
        }
    }

    //最大速度以下なら加速処理を行う
    if (length <= maxMoveSpeed)
    {
        //方向ベクトルがゼロでないなら加速処理を行う
        //ゼロの場合は入力されていない
        float directionLength = sqrtf(direction.x * direction.x + direction.z * direction.z);
        if (directionLength > 0.0f)
        {
            //加速度(フレーム単位で計算)
            float acceleration = this->acceleration * elapsedTime;

            //空中にいるときは加速度を減少
            if (!groundedFlag)
            {
                acceleration *= airControl;
            }

            //加速処理
            //方向ベクトルに加速度をスケーリングした値を速度ベクトルに加算
            velocity.x += direction.x * acceleration;
            velocity.z += direction.z * acceleration;

            //最大速度制限
            float length = sqrtf(velocity.x * velocity.x + velocity.z * velocity.z);
            if (length > maxMoveSpeed)
            {
                // 方向ベクトルに最大移動速度をスケーリングした値を速度ベクトルに代入
                float vx = velocity.x / length;
                float vz = velocity.z / length;
                velocity.x = vx * maxMoveSpeed;
                velocity.z = vz * maxMoveSpeed;
            }

            //下り坂でがたがたしないようにする
            if (groundedFlag && slopeRate > 0.0f)
            {
                //斜面での落下速度-横移動の長さ*
                velocity.y -= length * slopeRate*elapsedTime;
            }
        }
    }
    direction.x = 0;
    direction.z = 0;
}

//水平移動更新処理
void Character::updateHorizontalMove(float elapsedTime)
{
    float velocityLengthXZ = sqrtf(velocity.x * velocity.x + velocity.z * velocity.z);
    if (velocityLengthXZ > 0.0f)
    {
        using namespace DirectX;
        float mx = velocity.x * elapsedTime;
        float mz = velocity.z * elapsedTime;

        VMCFHT& ince_ray = VMCFHT::instance();
        HitResult hit;
        XMFLOAT3 start{ position.x,position.y+0.05f,position.z };
        XMFLOAT3 end{ (position.x + mx),position.y+0.05f,position.z + mz };
        Ray_ObjType type = Ray_ObjType::Static_objects;
        Ray_ObjType type2 = Ray_ObjType::DaynamicObjects;
        Ray_ObjType type3 = Ray_ObjType::DynamicGimics;
        Ray_ObjType type4 = Ray_ObjType::Stage;
        if (ince_ray.RayCast(start, end, hit, type))
        {

            XMVECTOR Start{ hit.position.x,hit.position.y,hit.position.z };

            XMVECTOR End{ XMLoadFloat3(&end) };
            XMVECTOR SEvec{ XMVectorSubtract(Start,End) };
            XMVECTOR Normal{ XMLoadFloat3(&hit.normal) };
            XMVECTOR Dot{ XMVector3Dot(Normal, SEvec) };
            float dot = 0;
            XMStoreFloat(&dot, Dot);
            XMVECTOR S{ XMVectorScale(Normal,dot*1.2f) };
            XMFLOAT3 p;
            XMStoreFloat3(&p, DirectX::XMVectorAdd(End, S));
            HitResult hit2;
            if (ince_ray.RayCast(hit.position, p, hit2, type))
            {
                position.x = hit.position.x;
                position.z = hit.position.z;
            }
            else
            {
                position.x = p.x;
                position.z = p.z;
            }


        }
        else if (ince_ray.RayCast(start, end, hit, type2))
        {
            XMVECTOR Start{ hit.position.x,hit.position.y,hit.position.z };

            XMVECTOR End{ XMLoadFloat3(&end) };

            XMVECTOR SEvec{ XMVectorSubtract(Start,End) };

            XMVECTOR Normal{ XMLoadFloat3(&hit.normal) };
            XMVECTOR Dot{ XMVector3Dot(Normal, SEvec) };
            float dot = 0;
            XMStoreFloat(&dot, Dot);
            XMVECTOR S{ XMVectorScale(Normal,dot*1.2f) };
            XMFLOAT3 p;
            XMStoreFloat3(&p, DirectX::XMVectorAdd(End, S));
            HitResult hit2;
            if (ince_ray.RayCast(hit.position, p, hit2, type2))
            {
                position.x = hit.position.x;
                position.z = hit.position.z;
            }
            else
            {
                position.x = p.x;
                position.z = p.z;
            }

        }
        else if (ince_ray.RayCast(start, end, hit, type3))
        {
            XMVECTOR Start{ hit.position.x,hit.position.y,hit.position.z };

            XMVECTOR End{ XMLoadFloat3(&end) };

            XMVECTOR SEvec{ XMVectorSubtract(Start,End) };

            XMVECTOR Normal{ XMLoadFloat3(&hit.normal) };
            XMVECTOR Dot{ XMVector3Dot(Normal, SEvec) };
            float dot = 0;
            XMStoreFloat(&dot, Dot);
            XMVECTOR S{ XMVectorScale(Normal,dot*1.2f) };
            XMFLOAT3 p;
            XMStoreFloat3(&p, DirectX::XMVectorAdd(End, S));
            HitResult hit2;
            if (ince_ray.RayCast(hit.position, p, hit2, type3))
            {
                position.x = hit.position.x;
                position.z = hit.position.z;
            }
            else
            {
                position.x = p.x;
                position.z = p.z;
            }

        }
        else if (ince_ray.RayCast(start, end, hit, type4))
        {
            XMVECTOR Start{ hit.position.x,hit.position.y,hit.position.z };

            XMVECTOR End{ XMLoadFloat3(&end) };

            XMVECTOR SEvec{ XMVectorSubtract(Start,End) };

            XMVECTOR Normal{ XMLoadFloat3(&hit.normal) };
            XMVECTOR Dot{ XMVector3Dot(Normal, SEvec) };
            float dot = 0;
            XMStoreFloat(&dot, Dot);
            XMVECTOR S{ XMVectorScale(Normal,dot*1.2f) };
            XMFLOAT3 p;
            XMStoreFloat3(&p, DirectX::XMVectorAdd(End, S));
            HitResult hit2;
            if (ince_ray.RayCast(hit.position, p, hit2, type4))
            {
                position.x = hit.position.x;
                position.z = hit.position.z;
            }
            else
            {
                position.x = p.x;
                position.z = p.z;
            }

        }
        else
        {
            // 移動処理
            position.x += mx;
            position.z += mz;
            extractFlag = false;
        }

    }
}
