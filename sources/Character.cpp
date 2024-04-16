#include"Character.h"
#include"StageManager.h"
#include"Mathf.h"

//行列更新処理
void Character::UpdateTransform()
{
    //スケール行列を作成
    DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);

    //回転行列を作成
    DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);

    //位置行列
    DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position.x, position.y, position.z);

    //３つの行列を組み合わせ、ワールド行列を作成
    DirectX::XMMATRIX W = S * R * T;

    // 計算したワールド行列を transform に取り出す
    DirectX::XMStoreFloat4x4(&transform, W);
}

//移動処理
void Character::move(float vx, float vz, float speed)
{
    // 移動方向ベクトルを決定
    direction.x = vx;
    direction.z = vz;

    // 最大速度設定
    maxMoveSpeed = speed;
}

//旋回処理
void Character::turn(float elapsedTime, float vx, float vz, float speed)
{
    speed *= elapsedTime;

    //進むべき進行ベクトルがゼロの阿合は旋回処理の必要なし
    float length = sqrtf(vx * vx + vz * vz);
    if (length < 0.001f)
    {
        return;
    }

    //進行ベクトルと単位ベクトル化
    vx /= length;
    vz /= length;

    //前方向ベクトルのXZ成分を取得し単位ベクトル化
    float frontX = transform._31;   //前方向ベクトルのX成分
    float frontZ = transform._33;   //前方向ベクトルのZ成分
    float frontLength = sqrtf(frontX * frontX + frontZ * frontZ);
    if (frontLength > 0.0f)
    {
        frontX /= frontLength;
        frontZ /= frontLength;
    }

    //左右チェックのための外積計算
    float cross = (frontZ * vx) - (frontX * vz);

    //回転角を求めるため、2つの単位ベクトルの内積を計算する
    //2つのベクトルの内積値は-1.0~1.0で表現されます.
    float dot = (frontX * vx) + (frontZ * vz);

    //2つのベクトルが重なった時、回転速度は0.0fになる
    float rot = (1.0f - dot);

    //あまり離れすぎると回転速度が早くなりすぎるのでspeed以上の回転速度にはならないよう制限
    if (rot > speed)
    {
        rot = speed;
    }

    //外積が正の場合は右回転、負の場合は左回転
    if (cross < 0.0f)
    {
        angle.y -= rot;
    }
    else
    {
        angle.y += rot;
    }
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

    //垂直移動更新処理
    updateVerticalMove(elapsedTime);
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

    //キャラクターのY軸方向となる法線ベクトル
    DirectX::XMFLOAT3 normal = { 0,-1,0 };
    if (moveY < 0.0f)
    {
        //移動処理
        position.y += velocity.y*elapsedTime;
        
        StageManager& ince_st = StageManager::incetance();
        VMCFHT& ince_ray = VMCFHT::instance();
        XMFLOAT3 pos = position;
        
        ince_ray.update(pos, normal);
        Object* st = ince_st.GetStages(ince_st.GetNowStage());
        collision_mesh& mesh = *st->GetModel()->Get_RaycastCollition();
        Intersection inter{};
        //地面判定
        if(ince_ray.raycast(mesh, st->GetTransform(), inter, 10.0f))
        {
            position.y = inter.intersection_position.y+0.005;
            velocity.y = 0.0f;

            //着地した
            if (!groundedFlag)
            {
                OnLanding();
            }
            groundedFlag = true;
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
}

//水平速度更新処理
void Character::updateHorizontalVelocity(float elapsedTime)
{
    // 速度に力が加わっていたら（0 じゃなかったら）減速処理を行う
    float length = sqrtf(velocity.x * velocity.x + velocity.z * velocity.z);
    if (length > 0.0f)
    {
        // 摩擦力（フレーム単位で計算）
        float friction = this->friction * elapsedTime * 60.0f;

        //空中にいるときは摩擦力を減少
        if (!groundedFlag)
        {
            friction* airControl;
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
            float acceleration = this->acceleration * elapsedTime * 60.0f;

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
                velocity.x = direction.x * maxMoveSpeed;
                velocity.z = direction.z * maxMoveSpeed;
            }
        }
    }
}

//水平移動更新処理
void Character::updateHorizontalMove(float elapsedTime)
{
    // 移動処理
    position.x += velocity.x * elapsedTime;
    position.z += velocity.z * elapsedTime;
}
////行列更新処理
//void Character::UpdateTransform()
//{
//    const DirectX::XMFLOAT4X4 coordinate_system_transforms[]{
//      { -1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 },	// 0:RHS Y-UP
//      { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 },		// 1:LHS Y-UP
//      { -1, 0, 0, 0, 0, 0, -1, 0, 0, 1, 0, 0, 0, 0, 0, 1 },	// 2:RHS Z-UP
//      { 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1 },		// 3:LHS Z-UP
//    };
//    const float scale_factor = 0.01f;
//    DirectX::XMMATRIX C{ DirectX::XMLoadFloat4x4(&coordinate_system_transforms[0]) * DirectX::XMMatrixScaling(scale_factor, scale_factor, scale_factor) };
//    //スケール行列を作成
//    DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
//
//    //回転行列を作成
//    DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);
//
//    //位置行列を作成
//    DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
//
//    //4つの行列を組み合わせ。ワールド行列を作成
//    DirectX::XMMATRIX W = C * S * R * T;
//
//    //計算したワールド行列を取り出す
//    DirectX::XMStoreFloat4x4(&transform, W);
//}
//
////移動処理
//void Character::Move(float vx, float vz, float speed)
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
//void Character::Turn(float elapsedTime, float vx, float vz, float speed)
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
//    float frontX = sinf(angle.y);//PLAYER
//    float frontZ = cosf(angle.y);//PLAYER
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
//        angle.y += -rot;
//    }
//    else
//    {
//        angle.y += rot;
//    }
//}
//
////ジャンプ処理
//void Character::Jump(float speed)
//{
//    //上方向の力を設定  
//    velocity.y = speed;
//}
//
////速力処理更新
//void Character::UpdateVelocity(float elapsedTime)
//{
//    //経過フレーム
//    float elapsedFrame = 60.0f * elapsedTime;
//
//    //垂直速力更新処理
//    UpdateVerticalVelocity(elapsedTime);
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
//void Character::UpdateVerticalVelocity(float elapsedTime)
//{
//    //重力処理
//    velocity.y += gravity * elapsedTime * 60.0f;
//}
//
////水平速力更新処理
//void Character::UpdateHorizontalVelocity(float elapsedFrame)
//{
//    //XZ平面の速力を減速する
//    float length = sqrtf(velocity.x * velocity.x + velocity.z * velocity.z);//DirectX::XMVectorGetX(DirectX::XMVector2LengthSq(velVel));
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
//            float vx = velocity.x / length;
//            float vz = velocity.z / length;
//
//            velocity.x -= vx * friction;
//            velocity.z -= vz * friction;
//        }
//        else//横方向の速力が摩擦力以下になったので速力無効化
//        {
//            //速度ベクトルを０にする
//            velocity.x = 0.0f;
//            velocity.z = 0.0f;
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
//            velocity.x += moveVecX * acceleration;
//            velocity.z += moveVecZ * acceleration;
//
//            //最大速度制限
//            float length = sqrtf(velocity.x * velocity.x + velocity.z * velocity.z);
//            if (length > maxMoveSpeed)
//            {
//                float vx = velocity.x / length;
//                float vz = velocity.z / length;
//                velocity.x = vx * maxMoveSpeed;
//                velocity.z = vz * maxMoveSpeed;
//            }
//
//            //下り坂でがたがたしないようにする
//            if (isGround && slopeRate > 0.0f)
//            {
//                //斜面での落下速度-横移動の長さ*経過フレーム
//                velocity.y -= length * slopeRate * elapsedFrame;
//            }
//        }
//    }
//    //移動ベクトルをリセット
//    moveVecX = 0.0f;
//    moveVecZ = 0.0f;
//}
//
////垂直移動更新処理
//void Character::UpdateVerticalMove(float elapsedTime)
//{
//    //垂直方向の移動量
//    float my = velocity.y * elapsedTime;
//
//    slopeRate = 0.0f;
//
//    //キャラクターのY軸方向となる法線ベクトル
//    DirectX::XMFLOAT3 normal = { 0,1,0 };
//
//    //落下中
//    
//    if (my < 0.0f)
//    {
//        //レイの開始位置は足元より少し上
//        DirectX::XMFLOAT3 start = { position.x,position.y + stepOffset,position.z };
//        //レイの終点位置は移動後の位置
//        DirectX::XMFLOAT3 end = { position.x,position.y + my,position.z };
//
//        DirectX::XMVECTOR startVec = DirectX::XMLoadFloat3(&start);
//        DirectX::XMVECTOR EndVec = DirectX::XMLoadFloat3(&end);
//        DirectX::XMVECTOR vec = DirectX::XMVectorSubtract(EndVec, startVec);
//
//        DirectX::XMFLOAT3 p;
//        DirectX::XMStoreFloat3(&p, vec);
//
//        //レイキャストによる地面判定
//        StageManager& ince_st = StageManager::incetance();
//        VMCFHT& ince_ray = VMCFHT::instance();
//        XMFLOAT3 pos = position;
//        ince_ray.update(pos, normal);
//        Object* st = ince_st.GetStages(ince_st.GetNowStage());
//        collision_mesh& mesh = *st->GetModel()->Get_RaycastCollition();
//        Intersection inter{};
//        bool hit = ince_ray.raycast(mesh, st->GetTransform(), inter, 100.0f);
//        if (hit)
//        {
//            //法線ベクトル取得
//            normal = inter.intersection_normal;
//
//            //地面に接他している
//            position.y = inter.intersection_position.y;
//            
//                
//            //回転
//            //angle.y -= int
//
//            //着地した
//            if (!isGround)
//            {
//                OnLanding();
//            }
//            isGround = true;
//            velocity.y = 0.0f;
//
//            //傾斜率の計算
//           /* float normalLengthXZ = sqrtf(inter.intersection_normal.x * inter.intersection_normal.x + inter.intersection_normal.z * inter.intersection_normal.z);
//            slopeRate = 1.0f - (inter.intersection_normal.y / (normalLengthXZ + inter.intersection_normal.y));*/
//        }
//        else
//        {
//            //空中に浮いている
//            position.y += my;
//            isGround = false;
//        }
//    }
//    //上昇中
//    else if (my > 0.0f)
//    {
//        position.y += my;
//        isGround = false;
//    }
//    
//    //地面の向きに添うようにXZ軸回転
//    {
//        ////Y軸が法線ベクトル方向に向くオイラー角回転を算出する
//        //float x = atan2f(normal.z, normal.y);//normal.z / normal.y;
//        //float z = -atan2f(normal.x, normal.y);//normal.x / normal.y;
//
//        ////線形補完で滑らかにする
//        //angle.x = Mathf::Leap(angle.x, x, 0.1f);
//        //angle.z = Mathf::Leap(angle.z, z, 0.1f);
//    }
//}
//
////水平移動更新
//void Character::UpdateHorizontalMove(float elapsedTime)
//{
//    ////水平速力量計算
//    //float velocityLengthXZ = sqrtf(velocity.x * velocity.x + velocity.z * velocity.z);
//    //if (velocityLengthXZ > 0.0f)
//    //{
//    //    //水平移動値
//    //    float mx = velocity.x * elapsedTime;
//    //    float mz = velocity.z * elapsedTime;
//
//    //    //レイの開始位置と終点位置
//    //    DirectX::XMFLOAT3 start = { position.x,position.y + stepOffset,position.z };
//    //    DirectX::XMFLOAT3 end = { position.x + mx,position.y + stepOffset,position.z + mz };
//
//    //    //レイキャストによる壁判定
//    //    HitResult hit;
//    //    if (StageManager::incetance().RayCast(start, end, hit))
//    //    {
//    //        //壁までのベクトル
//    //        DirectX::XMVECTOR Start = DirectX::XMLoadFloat3(&start);
//    //        DirectX::XMVECTOR End = DirectX::XMLoadFloat3(&end);
//    //        DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(End, Start);
//
//    //        //壁の法線
//    //        DirectX::XMVECTOR Normal = DirectX::XMLoadFloat3(&hit.normal);
//
//    //        //入射ベクトルを法線に射影
//    //        DirectX::XMVECTOR Dot = DirectX::XMVector3Dot(DirectX::XMVectorNegate(Vec), Normal);
//
//    //        //補正位置の計算
//    //        DirectX::XMVECTOR CollectPosition = DirectX::XMVectorMultiplyAdd(Normal, Dot, End);
//    //        DirectX::XMFLOAT3 collectPosition;//p
//    //        DirectX::XMStoreFloat3(&collectPosition, CollectPosition);
//
//    //        //壁ずり方向へレイキャスト
//    //        HitResult hit2;
//    //        if (!StageManager::incetance().RayCast(hit.position, collectPosition, hit2))
//    //        {
//    //            //壁ずる方向で壁に当たらなかったら補正位置に移動
//    //            position.x = collectPosition.x;
//    //            position.z = collectPosition.z;
//    //        }
//    //        else
//    //        {
//    //            position.x = hit2.position.x;
//    //            position.z = hit2.position.z;
//    //        }
//    //    }
//    //    else
//    //    {
//    //        //移動
//    //        position.x += mx;
//    //        position.z += mz;
//    //    }
//    //}
//}