#include"Player.h"
#include"camera.h"
#include"GamePad.h"
#include"Input/gamepad.h"
Player::Player(ID3D11Device*device)
{
    model = make_unique<Model>(device, filename, true);

    //Playerが所持する能力の初期設定
    old_attribute_state.push_back(Obj_attribute::null);

    //モデルの大きさ設定
    Scale.x = Scale.y = Scale.z = 100.0f;

    //初期の行動設定
    TransitionIdleState();
}

Player::~Player()
{

}

void Player::Update(float elapsedTime)
{
    //ステート毎の処理
    switch (state)
    {
    case State::Idle:UpdateIdleState(elapsedTime);
        break;
    case State::Move:UpdateMoveState(elapsedTime);
        break;
    case State::Jump:UpdateJumpState(elapsedTime);
        break;
    case State::Land:UpdateLandState(elapsedTime);
        break;
    }

    //速力処理更新
    UpdateVelocity(elapsedTime);

    //オブジェクト行列を更新
    UpdateTransform();
}

void Player::Render(RenderContext* rc)
{
    model->render(rc->deviceContext, Transform);
}

bool Player::InputMove(float elapsedTime)
{
    // 進行ベクトル取得
    DirectX::XMFLOAT3 moveVec = GetMoveVec();

    //移動処理
    Move(moveVec.x, moveVec.z, moveSpeed);

    //旋回処理
    Turn(elapsedTime, moveVec.x, moveVec.z, turnSpeed);

    //進行ベクトルがゼロベクトルでない場合は入力された
    return moveVec.x != 0.0f || moveVec.y != 0.0f || moveVec.z != 0.0f;
}

//スティック入力値から移動ベクトルを取得
DirectX::XMFLOAT3 Player::GetMoveVec()const
{
    //入力情報を取得
    gamepad& pad = gamepad::Instance();
    float ax = pad.thumb_state_rx();
    float ay = pad.thumb_state_ry();
    //カメラ方向を取得
    Camera& camera = Camera::instance();
    const DirectX::XMFLOAT3& cameraRight = camera.GetRight();
    const DirectX::XMFLOAT3& cameraFront = camera.GetFront();
    //移動ベクトルはXZ平面なベクトルになるようにする

    //カメラ右方向ベクトルをXZ単位ベクトルに変換
    float cameraRightX = cameraRight.x;
    float cameraRightZ = cameraRight.z;
    float cameraRightLength = sqrtf(cameraRightX * cameraRightX + cameraRightZ * cameraRightZ);
    if (cameraRightLength > 0.0f)
    {
        //単位ベクトル化
        cameraRightX /= cameraRightLength;
        cameraRightZ /= cameraRightLength;
    }
    //カメラ前方向ベクトルをXZ単位ベクトルに変換
    float cameraFrontX = cameraFront.x;
    float cameraFrontZ = cameraFront.z;
    float cameraFrontLength = sqrtf(cameraFrontX * cameraFrontX + cameraFrontZ * cameraFrontZ);
    if (cameraFrontLength < 0.0f)
    {
        //単位ベクトル化
        cameraFrontX /= cameraFrontLength;
        cameraFrontZ /= cameraFrontLength;
    }
    //スティックの水平入力値をカメラ右方向に反映し
    //スティックの垂直入力値をカメラ前方向に反映し
    //進行ベクトルを計算する
    DirectX::XMFLOAT3 vec;
    vec.x = (cameraRightX * ax) + (cameraFrontX * ay);
    vec.z = (cameraRightZ * ax) + (cameraFrontZ * ay);
    //Y軸方向には移動しない
    vec.y = 0.0f;
    return vec;
}

//ジャンプ入力処理
bool Player::InputJump()
{
    gamepad& pad = gamepad::Instance();
    if (GetKeyState(' ') & 1)//コントローラーのAにしたいがやり方わからない
    {
        //ジャンプ回数制限
        if (jumpCount < jumpLimit)
        {
            //ジャンプ
            jumpCount++;
            Jump(jumpSpeed);
            //ジャンプ入力した
            return true;
        }
    }
    return false;
}

void Player::OnLanding()
{
    jumpCount = 0;
    if (Velocty.y >= -1)
    {
        TransitionLandState();
    }
    //下方向の速力が一定以上なら着地ステートへ
    if (Velocty.y < gravity * 5.0f)
    {
        //着地ステートへ遷移
        TransitionLandState();
    }
}

//待機ステートへ遷移
void Player::TransitionIdleState()
{
    state = State::Idle;

    //待機アニメーション再生
    //model->PlayAnimation(Anim_Idle, true);
}

//待機ステート更新処理
void Player::UpdateIdleState(float elapsedTime)
{
    //移動入力処理
    InputMove(elapsedTime);
    if (InputMove(elapsedTime) == true)
    {
        //移動ステートへの遷移
        TransitionMoveState();
    }

    //ジャンプ入力処理
    InputJump();
    if (InputJump() == true)
    {
        //ジャンプステートへの遷移
        TransitionJumpState();
    }
}

//移動ステートへ遷移
void Player::TransitionMoveState()
{
    state = State::Move;

    //走りアニメーション再生
    //model->PlayAnimation(Anim_Running, true);
}

//移動ステート更新処理
void Player::UpdateMoveState(float elapsedTime)
{
    //移動入力処理
    if (InputMove(elapsedTime) == false)
    {
        TransitionIdleState();
    }

    //ジャンプ入力処理
    if (InputJump() == true)
    {
        //ジャンプステートへの遷移
        TransitionJumpState();
    }
}

//ジャンプステートへ遷移
void Player::TransitionJumpState()
{
    state = State::Jump;

    //ジャンプアニメーション再生
    //model->PlayAnimation(Anim_jump, false);
}

//ジャンプステート更新処理
void Player::UpdateJumpState(float elapsedTime)
{
    //移動入力処理
    InputMove(elapsedTime);

    //ジャンプ入力処理
    if (InputJump())
    {
        //ジャンプフリップアニメーション再生
        //model->PlayAnimation(Anim_jump_Flip, false);
    }
    //ジャンプアニメーション終了後
    //if (!model->IsPlayAnimation())
    //{
    //    //落下アニメーション再生
    //    model->PlayAnimation(Anim_Falling, true);
    //}
}

//着地ステートへ遷移
void Player::TransitionLandState()
{
    state = State::Land;

    //着地アニメーションを再生
    //model->PlayAnimation(Anim_Landing, false);
}

//着地ステート更新処理
void Player::UpdateLandState(float elapsedTime)
{
    //攻撃アニメーション終了後
    //if (!model->IsPlayAnimation())
    //{
    //    //待機ステートへ遷移
    //    TransitionIdleState();
    //}
}