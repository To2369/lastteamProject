#include"Player.h"
#include"camera.h"
#include"GamePad.h"
#include"Input/gamepad.h"
#include"StageManager.h"
#include"objectManajer.h"
#include"variable_management_class_for_hit_test.h"
#include"Graphics/DebugRenderer.h"
/// <summary>
/// 
/// </summary>
/// <param name="sphere">objのsphere</param>
/// <param name="Position">playerの位置</param>
/// <param name="radius">playerの半径</param>
/// <param name="outsphere">結果保存用</param>
/// <returns></returns>
bool QuadPlacement_vs_PlayerSphere(const Object::SphereQuadPlacement& sphere, const DirectX::XMFLOAT3& Position, float radius, Object::ResultSphereQuadPlacement& outsphere)
{
    using namespace DirectX;

    XMVECTOR ThisPos{ XMLoadFloat3(&Position) };
    const int SphereCount = sphere.size;
    Object::SphereQuadPlacement param{};
    for (int i = 0; i < SphereCount; i++)
    {
        XMVECTOR QuadSpherePos{ XMLoadFloat3(&sphere.pos[i].Spherepos) };
        XMVECTOR Vec = XMVector3LengthSq(XMVectorSubtract(QuadSpherePos, ThisPos));

        float dist = XMVectorGetX(Vec);
        float rad = radius + sphere.SphereRadius;
        if (dist < rad * rad)
        {
            XMStoreFloat3(&outsphere.Spherepos, QuadSpherePos);
            outsphere.type = sphere.pos[i].Type;
            return true;
        }
    }
    return false;
}
//コンストラクタ
Player::Player(ID3D11Device* device)
{
    model = std::make_unique<Model>(device, filename, true);
    const float scale_fcator = 0.01f;	//モデルが大きいのでスケール調整
    scale = { scale_fcator, scale_fcator, scale_fcator };

    radius = 0.5f;
}

//デストラクタ
Player::~Player()
{

}

//更新処理
void Player::update(float elapsedTime)
{

    //////移動入力処理
    inputMove(elapsedTime);

    ////ジャンプ入力処理
    inputJump();

    ////速度処理更新
    updateVelocity(elapsedTime);

    //プレイヤーとギミックの当たり判定
    CollisionPlayerVsGimics(elapsedTime);

    ////ワールド行列の更新
    UpdateTransform();
   
    gamepad& gamePad = gamepad::Instance();
    if (gamePad.button_state(gamepad::button::b))
    {
        velocity.y = 0;
        position.y = 30;
        velocity.y = 0;
        position.x = 0;
        position.z = 0;
    }
}

//描画処理
void Player::render(RenderContext* rc)
{
    DebugRenderer debugRenderer = DebugRenderer::incetance(rc->device);
        //衝突判定用のデバッグ球を描画
        debugRenderer.DrawSphere(position, radius, { 1,0,0,1 });

    model->render(rc->deviceContext, transform, 0.0f, { 1.0f,1.0f,1.0f,1.0f });
}


//操作移動
void Player::inputMove(float elapsedTime)
{
    //進行ベクトルを取得
    DirectX::XMFLOAT3 moveVec = getMoveVec();

    move(moveVec.x, moveVec.z, this->moveSpeed);

    //旋回処理
    turn(elapsedTime, moveVec.x, moveVec.z, this->turnSpeed);

    //進行ベクトルがゼロベクトルでない場合は入力された
    moveVec.x != 0.0f || moveVec.y != 0.0f || moveVec.z != 0.0f;
}

//入力値から移動ベクトルを取得
DirectX::XMFLOAT3 Player::getMoveVec() const
{
    //入力情報を取得
    gamepad& gamePad = gamepad::Instance();
    float ax = gamePad.thumb_state_rx();//->getAxisLX();
    float ay = gamePad.thumb_state_ry();//->getAxisLY();

    //カメラ方向を取得
    Camera& camera = Camera::instance();
    const DirectX::XMFLOAT3 cameraFront = camera.GetFront();//camera->getFront();
    const DirectX::XMFLOAT3 cameraRight = camera.GetRight();


    //カメラ右方向ベクトルをXZ単位ベクトルに変換
    float cameraRightX = cameraRight.x;
    float cameraRightZ = cameraRight.z;
    float cameraRightLength = sqrtf(cameraRightX * cameraRightX + cameraRightZ * cameraRightZ);
    if (cameraRightLength > 0.0f)
    {
        cameraRightX = cameraRightX / cameraRightLength;
        cameraRightZ = cameraRightZ / cameraRightLength;
    }

    //カメラ前方向ベクトルをXZ単位ベクトルに変換
    float cameraFrontX = cameraFront.x;
    float cameraFrontZ = cameraFront.z;

    //カメラ前方向ベクトルを単位ベクトル化
    float cameraFrontLength = sqrtf(cameraFrontX * cameraFrontX + cameraFrontZ * cameraFrontZ);
    if (cameraFrontLength < 0.0f)
    {
        cameraFrontX = cameraFrontX / cameraFrontLength;
        cameraFrontZ = cameraFrontZ / cameraFrontLength;
    }

    //垂直入力値をカメラ前方向に反映し、水平方向をカメラ右方向に反映し進行ベクトルを計算する
    DirectX::XMFLOAT3 vec;
    vec.x = cameraFrontX * ay + cameraRightX * ax;
    vec.z = cameraFrontZ * ay + cameraRightZ * ax;

    //Y軸方向には移動しない
    vec.y = 0.0f;

    return vec;
}

//ジャンプ入力処理
void Player::inputJump()
{
    gamepad& gamePad = gamepad::Instance();
    if (gamePad.button_state(gamepad::button::a))
    {
        //ジャンプ回数制限
        if (jumpCount < jumpLimit)
        {
            //ジャンプ
            jumpCount++;
            jump(jumpSpeed);
        }
    }
}

void Player::OnLanding()
{
    jumpCount = 0;
}

//プレイヤーとギミックの当たり判定
void Player::CollisionPlayerVsGimics(float elapsedTime)
{
    Objectmanajer& objMgr = Objectmanajer::incetance();
    //全てのギミックと総当たりで衝突処理
    int objCount = objMgr.Get_GameObjCount();
    for (int i = 0; i < objCount; i++)
    {
        Object* obj = objMgr.Get_GameObject(i);
        //衝突判定
        Object::ResultSphereQuadPlacement outsphere;
        if (QuadPlacement_vs_PlayerSphere(obj->GetMySphere(), position, radius, outsphere))
        {
            switch (outsphere.type)
            {
            case Object::SphereAttribute::Right:
                position = outsphere.Spherepos;
                if(velocity.x*elapsedTime<0)
                obj->SetVelotyXZ({ velocity.x * elapsedTime,0 });
                
                break;
            case Object::SphereAttribute::Left:
                if (velocity.x * elapsedTime > 0)
                    obj->SetVelotyXZ({ velocity.x * elapsedTime,0 });
                position = outsphere.Spherepos;
                break;
            case Object::SphereAttribute::Front:
                if (velocity.z * elapsedTime > 0)
                    obj->SetVelotyXZ({ 0,velocity.z * elapsedTime });
                position = outsphere.Spherepos;
                 break;
            case Object::SphereAttribute::Backfront:
                if (velocity.z * elapsedTime < 0)
                    obj->SetVelotyXZ({ 0,velocity.z * elapsedTime });
                position = outsphere.Spherepos;
                break;
            case Object::SphereAttribute::null:
                break;
            }
        }
    }
}