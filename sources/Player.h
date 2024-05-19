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
    void updateSyringepos();

    //ジャンプ入力処理
    void inputJump();

    //プレイヤーとギミックの当たり判定
    void CollisionPlayerVsGimics(float elapsedTime);

    void ExtractionAttribute(float elapsedTime);

    void pullpushAnime(float elapsedTime);
protected:
    //着地したときに呼び出される
    void OnLanding()override;
private:
    std::unique_ptr<Model> model;
    std::unique_ptr<Model> Smodel;          //注射器用
    const char* filename = ".\\Resources\\Character\\Hand\\hand.fbx";
    const char* Sfilename = ".\\Resources\\Character\\Syringe\\syringe_M.fbx";      //注射器用

    DirectX::XMFLOAT3 resetPosition = {};       //ステージから落下した際に初期位置に戻す
    //移動スピード
    float moveSpeed = 1.0f;

    //旋回スピード
    float turnSpeed = DirectX::XMConvertToRadians(720);

    //ジャンプ力
    float jumpSpeed = 0.8f;

    //ジャンプ回数
    int jumpCount = 0;

    //ジャンプ制限
    int jumpLimit = 1;

    //プレイヤーの手出現フラグ
    bool isHand = true;

    float elapsedTime_ = 0;

    bool wasKeyPressed = false;

    bool pullType = false;
    bool pushType = false;
    
    bool pushFlag = false;  //物押せるとき

    bool SphereHitFlag = false;
    bool CubeHitFlag = false;

    enum Sanime
    {
        wait,
        pull,
        push,
    };
};
