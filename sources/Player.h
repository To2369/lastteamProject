//#pragma once
//#include"object.h"
//#include"model.h"
//class Player:public Object
//{
//public:
//
//	Player(ID3D11Device* device);
//	~Player()override;
//
//	//更新処理
//	void Update(float elapsedTime)override;
//
//	//描画処理
//	void Render(RenderContext* rc)override;
//
//private:
//    //待機ステートへ遷移
//    void TransitionIdleState();
//
//    //待機ステート更新処理
//    void UpdateIdleState(float elapsedTime);
//
//    //移動ステートへ遷移
//    void TransitionMoveState();
//
//    //移動ステート更新処理
//    void UpdateMoveState(float elapsedTime);
//
//    //ジャンプステートへ遷移
//    void TransitionJumpState();
//
//    //ジャンプステート更新処理
//    void UpdateJumpState(float elapsedTime);
//
//    //着地ステートへ遷移
//    void TransitionLandState();
//
//    //着地ステート更新処理
//    void UpdateLandState(float elapsedTime);
//private:
//    //移動入力処理
//    bool InputMove(float elapsedTime);
//
//    //ジャンプ入力更新
//    bool InputJump();
//
//    //スティック入力値から移動ベクトルを取得
//    DirectX::XMFLOAT3 GetMoveVec()const;
//
//    float turnSpeed = DirectX::XMConvertToRadians(720);
//
//public:
//    
//
//    //垂直速力更新処理
//    void UpdateVerticalVelocity(float elapsedFrame);
//
//    //垂直移動更新処理
//    void UpdateVerticalMove(float elapsedTime);
//
//    //水平速力更新処理
//    void UpdateHorizontalVelocity(float elapsedFrame);
//
//    //水平移動更新処理
//    void UpdateHorizontalMove(float elapsedTime);
//
//public:
//    //移動処理
//    void Move(float vx, float vz, float speed);
//
//    //旋回処理
//    void Turn(float elapsedTime, float vx, float vz, float speed);
//
//    //ジャンプ処理
//    void Jump(float speed);
//
//    //速力処理更新
//    void UpdateVelocity(float elapsedTime);
//
//    //着地したときに呼ばれる
//    virtual void OnLanding() {}
//private:
//    //重力
//    float gravity = -1.0f;
//    //地面に着地したか
//    bool isGround = false;
//    //摩擦
//    float friction = 0.5f;
//
//    //加速度
//    float acceleration = 1.0f;
//
//    //最大速度
//    float maxMoveSpeed = 5.0f;
//
//    //計算用
//    float moveVecX = 0.0f;
//    float moveVecZ = 0.0f;
//
//    //空中移動
//    float airControl = 0.3f;
//
//    //開始位置を上にするため
//    float stepOffset = 1.0f;
//
//    //斜めの時間制限
//    float slopeRate = 1.0f;
//protected:
//    //着地したときによばれる
//    void OnLanding()override;
//private:
//	const char* filename = ".\\resources\\3Dmodel\\Cube\\Cube.fbx";
//    enum class State
//    {
//        Idle,
//        Move,
//        Jump,
//        Land
//    };
//    State state = State::Idle;
//    float moveSpeed = 5.0f;         //移動速度
//    float jumpSpeed = 20.0f;        //ジャンプ速度
//    int jumpCount = 0;              //ジャンプのカウント
//    int jumpLimit = 2;              //ジャンプできる回数
//};