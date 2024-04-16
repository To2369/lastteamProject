#include"Player.h"
#include"camera.h"
#include"GamePad.h"
#include"Input/gamepad.h"
#include"StageManager.h"
//コンストラクタ
Player::Player(ID3D11Device* device)
{
    model = std::make_unique<Model>(device, filename, true);

    const float scale_fcator = 0.001f;	//モデルが大きいのでスケール調整
    scale = { scale_fcator, scale_fcator, scale_fcator };
}

//デストラクタ
Player::~Player()
{

}

//更新処理
void Player::update(float elapsedTime)
{
    //移動入力処理
    inputMove(elapsedTime);

    //ジャンプ入力処理
    inputJump();

    //速度処理更新
    updateVelocity(elapsedTime);

    //ワールド行列の更新
    UpdateTransform();
}

//描画処理
void Player::render(RenderContext* rc)
{
    model->render(rc->deviceContext, transform, 0.0f,{ 1.0f,1.0f,1.0f,1.0f });
}


//操作移動
void Player::inputMove(float elapsedTime)
{
    //進行ベクトルを取得
    DirectX::XMFLOAT3 moveVec = getMoveVec();

    move(moveVec.x, moveVec.z, this->moveSpeed);

    //旋回処理
    turn(elapsedTime, moveVec.x, moveVec.z, this->turnSpeed);
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

    //カメラ前方向ベクトルをXZ単位ベクトルに変換
    float cameraFrontX = cameraFront.x;
    float cameraFrontZ = cameraFront.z;

    //カメラ前方向ベクトルを単位ベクトル化
    float cameraFrontLength = sqrtf(cameraFrontX * cameraFrontX + cameraFrontZ * cameraFrontZ);
    if (cameraFrontLength > 0.0f)
    {
        cameraFrontX = cameraFrontX / cameraFrontLength;
        cameraFrontZ = cameraFrontZ / cameraFrontLength;
    }

    //カメラ右方向ベクトルをXZ単位ベクトルに変換
    float cameraRightX = cameraRight.x;
    float cameraRightZ = cameraRight.z;
    float cameraRightLength = sqrtf(cameraRightX * cameraRightX + cameraRightZ * cameraRightZ);
    if (cameraRightLength > 0.0f)
    {
        cameraRightX = cameraRightX / cameraRightLength;
        cameraRightZ = cameraRightZ / cameraFrontLength;
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
    if (GetKeyState('Z') & 1)
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