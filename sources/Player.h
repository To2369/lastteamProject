#pragma once
#include"Character.h"
#include"Graphics/RenderContext.h"
#include"model.h"
//プレイヤー
class Player : public Character
{
public:
    Player(ID3D11Device* device);
    ~Player() override;

    //更新処理
    void update(float elapsedTime);

    //描画処理
    void render(RenderContext* rc);

    Model* GetModel() { return model.get(); }

private:
    //操作移動
    void inputMove(float elapsedTime);

    //入力値から移動ベクトルを取得
    DirectX::XMFLOAT3 getMoveVec() const;
public:

    //ジャンプ入力処理
    void inputJump();

    //プレイヤーとギミックの当たり判定
    void CollisionPlayerVsGimics(float elapsedTime);

    void ExtractionAttribute(float elapsedTime);
protected:
    //着地したときに呼び出される
    void OnLanding()override;
private:
    std::unique_ptr<Model> model;
    const char* filename = ".\\Resources\\Character\\Hand\\hand.fbx";

    //移動スピード
    float moveSpeed = 1.0f;

    //旋回スピード
    float turnSpeed = DirectX::XMConvertToRadians(720);

    //ジャンプ力
    float jumpSpeed = 1.0f;

    //ジャンプ回数
    int jumpCount = 0;

    //ジャンプ制限
    int jumpLimit = 2;

    //プレイヤーの手出現フラグ
    bool isHand = true;
};
