#include"Character.h"
#include"StageManager.h"
#include"Mathf.h"

//行列更新処理
void Character::UpdateTransform()
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
    DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);

    //回転行列を作成
    //DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);
    DirectX::XMMATRIX X = DirectX::XMMatrixRotationX(angle.x);
    DirectX::XMMATRIX Y = DirectX::XMMatrixRotationY(angle.y);
    DirectX::XMMATRIX Z = DirectX::XMMatrixRotationZ(angle.z);
    DirectX::XMMATRIX R = Y * X * Z;

    //位置行列を作成
    DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position.x, position.y, position.z);

    //4つの行列を組み合わせ。ワールド行列を作成
    DirectX::XMMATRIX W = C * S * R * T;

    //計算したワールド行列を取り出す
    DirectX::XMStoreFloat4x4(&transform, W);
}

//移動処理
void Character::Move(float vx, float vz, float speed)
{
    //移動方向ベクトルを設定
    moveVecX = vx;
    moveVecZ = vz;

    //最大速度設定
    maxMoveSpeed = speed;
}

//旋回処理
void Character::Turn(float elapsedTime, float vx, float vz, float speed)
{
    speed *= elapsedTime;
    float length = sqrtf(vx * vx + vz * vz);
    //進行ベクトルがゼロベクトルの場合は処理する必要なし
    if (length < 0.001f)
    {
        return;
    }

    //進行ベクトルを単位化
    vx /= length;
    vz /= length;

    //自身の回転値から前方向を求める
    float frontX = sinf(angle.y);//PLAYER
    float frontZ = cosf(angle.y);//PLAYER
    //回転角を求めるために２つの単位ベクトルの外積を計算する
    float dot = (frontX * vx) + (frontZ * vz);
    float rot = 1.0f - dot;

    if (rot > speed)
    {
        rot = speed;
    }
    //左右判定を行うために２つの単位ベクトルの外積を計算する
    float croos = (frontZ * vx) - (frontX * vz);

    //２Dの外積判定が正の場合か負の場合によって左右判定が行える
    //左右判定を行う事によって左右判定を選択する
    if (croos < 0.0f)
    {
        angle.y += -rot;
    }
    else
    {
        angle.y += rot;
    }
}

//ジャンプ処理
void Character::Jump(float speed)
{
    //上方向の力を設定  
    velocity.y = speed;
}

//速力処理更新
void Character::UpdateVelocity(float elapsedTime)
{
    //経過フレーム
    float elapsedFrame = 60.0f * elapsedTime;

    //垂直速力更新処理
    UpdateVerticalVelocity(elapsedFrame);

    //水平速力更新処理
    UpdateHorizontalVelocity(elapsedFrame);

    //垂直移動更新処理
    UpdateVerticalMove(elapsedTime);

    //水平移動更新処理
    UpdateHorizontalMove(elapsedTime);
}

//垂直速力更新処理
void Character::UpdateVerticalVelocity(float elapsedFrame)
{
    //重力処理
    velocity.y += gravity * elapsedFrame;
}

//水平速力更新処理
void Character::UpdateHorizontalVelocity(float elapsedFrame)
{
    //XZ平面の速力を減速する
    float length = sqrtf(velocity.x * velocity.x + velocity.z * velocity.z);//DirectX::XMVectorGetX(DirectX::XMVector2LengthSq(velVel));
    if (length > 0.0f)
    {
        //摩擦力
        float friction = this->friction * elapsedFrame;

        //空中にいるときは摩擦力を減らす
        if (!isGround)friction *= airControl;

        //摩擦による横方向の減速処理
        if (length > friction)
        {
            float vx = velocity.x / length;
            float vz = velocity.z / length;

            velocity.x -= vx * friction;
            velocity.z -= vz * friction;
        }
        else//横方向の速力が摩擦力以下になったので速力無効化
        {
            //速度ベクトルを０にする
            velocity.x = 0.0f;
            velocity.z = 0.0f;
        }
    }

    //XZ平面の速力を加速する
    if (length <= maxMoveSpeed)
    {
        //移動ベクトルがゼロベクトルでないなら加速する
        float moveVecLength = sqrtf(moveVecX * moveVecX + moveVecZ * moveVecZ);
        if (moveVecLength > 0.0f)
        {
            //加速力
            float acceleration = this->acceleration * elapsedFrame;

            //空中にいるときは加速力を減らす
            if (!isGround)acceleration *= airControl;

            //移動ベクトルによる加速処理
            velocity.x += moveVecX * acceleration;
            velocity.z += moveVecZ * acceleration;

            //最大速度制限
            float length = sqrtf(velocity.x * velocity.x + velocity.z * velocity.z);
            if (length > maxMoveSpeed)
            {
                float vx = velocity.x / length;
                float vz = velocity.z / length;
                velocity.x = vx * maxMoveSpeed;
                velocity.z = vz * maxMoveSpeed;
            }

            //下り坂でがたがたしないようにする
            if (isGround && slopeRate > 0.0f)
            {
                //斜面での落下速度-横移動の長さ*経過フレーム
                velocity.y -= length * slopeRate * elapsedFrame;
            }
        }
    }
    //移動ベクトルをリセット
    moveVecX = 0.0f;
    moveVecZ = 0.0f;
}

//垂直移動更新処理
void Character::UpdateVerticalMove(float elapsedTime)
{
    //垂直方向の移動量
    float my = velocity.y * elapsedTime;

    slopeRate = 0.0f;

    //キャラクターのY軸方向となる法線ベクトル
    DirectX::XMFLOAT3 normal = { 0,1,0 };

    //落下中
    /*
    if (my < 0.0f)
    {
        //レイの開始位置は足元より少し上
        DirectX::XMFLOAT3 start = { position.x,position.y + stepOffset,position.z };
        //レイの終点位置は移動後の位置
        DirectX::XMFLOAT3 end = { position.x,position.y + my,position.z };

        //レイキャストによる地面判定
        HitResult hit;
        if (StageManager::Instance().RayCast(start, end, hit))
        {
            //法線ベクトル取得
            normal = hit.normal;

            //地面に接他している
            position = hit.position;


            //回転
            angle.y -= hit.rotation.y;

            //着地した
            if (!isGround)
            {
                OnLanding();
            }
            isGround = true;
            velocity.y = 0.0f;

            //傾斜率の計算
            float normalLengthXZ = sqrtf(hit.normal.x * hit.normal.x + hit.normal.z * hit.normal.z);
            slopeRate = 1.0f - (hit.normal.y / (normalLengthXZ + hit.normal.y));
        }
        else
        {
            //空中に浮いている
            position.y += my;
            isGround = false;
        }
    }
    //上昇中
    else if (my > 0.0f)
    {
        position.y += my;
        isGround = false;
    }
    */
    //地面の向きに添うようにXZ軸回転
    {
        //Y軸が法線ベクトル方向に向くオイラー角回転を算出する
        float x = atan2f(normal.z, normal.y);//normal.z / normal.y;
        float z = -atan2f(normal.x, normal.y);//normal.x / normal.y;

        //線形補完で滑らかにする
        angle.x = Mathf::Leap(angle.x, x, 0.1f);
        angle.z = Mathf::Leap(angle.z, z, 0.1f);
    }
}

//水平移動更新
void Character::UpdateHorizontalMove(float elapsedTime)
{
    /*
    //水平速力量計算
    float velocityLengthXZ = sqrtf(velocity.x * velocity.x + velocity.z * velocity.z);
    if (velocityLengthXZ > 0.0f)
    {
        //水平移動値
        float mx = velocity.x * elapsedTime;
        float mz = velocity.z * elapsedTime;

        //レイの開始位置と終点位置
        DirectX::XMFLOAT3 start = { position.x,position.y + stepOffset,position.z };
        DirectX::XMFLOAT3 end = { position.x + mx,position.y + stepOffset,position.z + mz };

        //レイキャストによる壁判定
        HitResult hit;
        if (StageManager::Instance().RayCast(start, end, hit))
        {
            //壁までのベクトル
            DirectX::XMVECTOR Start = DirectX::XMLoadFloat3(&start);
            DirectX::XMVECTOR End = DirectX::XMLoadFloat3(&end);
            DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(End, Start);

            //壁の法線
            DirectX::XMVECTOR Normal = DirectX::XMLoadFloat3(&hit.normal);

            //入射ベクトルを法線に射影
            DirectX::XMVECTOR Dot = DirectX::XMVector3Dot(DirectX::XMVectorNegate(Vec), Normal);

            //補正位置の計算
            DirectX::XMVECTOR CollectPosition = DirectX::XMVectorMultiplyAdd(Normal, Dot, End);
            DirectX::XMFLOAT3 collectPosition;//p
            DirectX::XMStoreFloat3(&collectPosition, CollectPosition);

            //壁ずり方向へレイキャスト
            HitResult hit2;
            if (!StageManager::Instance().RayCast(hit.position, collectPosition, hit2))
            {
                //壁ずる方向で壁に当たらなかったら補正位置に移動
                position.x = collectPosition.x;
                position.z = collectPosition.z;
            }
            else
            {
                position.x = hit2.position.x;
                position.z = hit2.position.z;
            }
        }
        else
        {
            //移動
            position.x += mx;
            position.z += mz;
        }
    }
    */
}