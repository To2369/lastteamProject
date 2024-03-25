#pragma once
#include"Character.h"
#include"Graphics/RenderContext.h"
#include"model.h"
class Player:public Character
{
public:

	Player(ID3D11Device* device);
	~Player()override;

	//更新処理
    void Update(float elapsedTime);

	//描画処理
	void Render(RenderContext* rc);

private:
    //待機ステートへ遷移
    void TransitionIdleState();

    //待機ステート更新処理
    void UpdateIdleState(float elapsedTime);

    //移動ステートへ遷移
    void TransitionMoveState();

    //移動ステート更新処理
    void UpdateMoveState(float elapsedTime);

    //ジャンプステートへ遷移
    void TransitionJumpState();

    //ジャンプステート更新処理
    void UpdateJumpState(float elapsedTime);

    //着地ステートへ遷移
    void TransitionLandState();

    //着地ステート更新処理
    void UpdateLandState(float elapsedTime);
private:
    //移動入力処理
    bool InputMove(float elapsedTime);

    //ジャンプ入力更新
    bool InputJump();

    //スティック入力値から移動ベクトルを取得
    DirectX::XMFLOAT3 GetMoveVec()const;

    float turnSpeed = DirectX::XMConvertToRadians(720);
protected:
    //着地したときによばれる
    void OnLanding()override;
private:
	const char* filename = ".\\resources\\3Dmodel\\Cube\\Cube.fbx";
    enum class State
    {
        Idle,
        Move,
        Jump,
        Land
    };
    State state = State::Idle;
    float moveSpeed = 5.0f;         //移動速度
    float jumpSpeed = 20.0f;        //ジャンプ速度
    int jumpCount = 0;              //ジャンプのカウント
    int jumpLimit = 2;              //ジャンプできる回数
};