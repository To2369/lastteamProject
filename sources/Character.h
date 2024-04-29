#pragma once
#include"model.h"
#include"DirectXMath.h"
#include"object.h"
//キャラクター
class Character
{
public:
    Character() {}
    virtual ~Character() {}

    //行列更新処理
    void UpdateTransform();

private:
    //垂直速度更新処理
    void updateVerticalVelocity(float elapsedTime);

    //垂直移動更新処理
    void updateVerticalMove(float elapsedTime);

    //水平速度更新処理
    void updateHorizontalVelocity(float elapsedSTIme);

    //水平移動更新処理
    void updateHorizontalMove(float elapsedTime);

    //レイとキャラ判定
    void RayVsCharacter(float moveX, float moveZ);
protected:
    //移動処理
    void move(float vx, float vz, float speed);

    //旋回処理
    void turn(float elapsedTime, float vx, float vz, float speed);

    //ジャンプ処理
    void jump(float speed);

    //速度処理更新
    void updateVelocity(float elapsedTime);

    //着地したときに呼ばれる
    virtual void OnLanding() {}
public:
    //位置取得
    const DirectX::XMFLOAT3 GetPosition()const { return position; };

    //位置設定
    void SetPosition(const DirectX::XMFLOAT3& position) { this->position = position; };

    //回転取得
    const DirectX::XMFLOAT3 GetAngle()const { return angle; };

    //回転設定
    void SetAngle(const DirectX::XMFLOAT3& angle) { this->angle = angle; };

    //スケール取得
    const DirectX::XMFLOAT3 GetScale()const { return scale; };

    //スケール取得
    void  SetScale(const DirectX::XMFLOAT3& scale) { this->scale = scale; };
    //速度取得
    DirectX::XMFLOAT3 GetVelocity() { return velocity; };
    void SetVelocity(XMFLOAT3 vel) { velocity = vel; }
    //半径の取得
    float getRadius()const { return radius; }
    //着地したか同課のフラグ取得
    bool isGrounded() const { return groundedFlag; }
    DirectX::XMFLOAT3 GetPosition() { return position; }
    Obj_attribute Getattribute() { return playerType; }
protected:
    DirectX::XMFLOAT3 position = { 0,0,0 };         //位置
    DirectX::XMFLOAT3 angle = { 0,0,0 };            //回転
    DirectX::XMFLOAT3 scale = { 1,1,1 };            //拡大縮小

    //姿勢行列
    DirectX::XMFLOAT4X4 transform = {
        1,0,0,0,
        0,1,0,0,
        0,0,1,0,
        0,0,0,1
    };
    float radius =0.f;                            //半径

    //重力
    float gravity = -1.0f;

    //速度
    DirectX::XMFLOAT3 velocity = { 0,0,0 };

    bool groundedFlag = false;

    float friction = 0.5f;                          //摩擦力

    float acceleration = 1.0f;                      //加速度
    float maxMoveSpeed = 5.0f;                      //最大速度
    DirectX::XMFLOAT3 direction = { 0,0,0 };        //移動方向
    float airControl = 0.3f;                        //空中制御用係数
    bool extractFlag = false;                       //オブジェクトの属性抽出できるか
    ObjType playerType = ObjType::null;
};
////キャラクター
//class Character
//{
//public:
//	Character() {};
//	virtual ~Character() {};
//public://Set,Get,flag関数
//    //位置取得
//    const DirectX::XMFLOAT3& GetPosition()const { return position; }
//
//    //位置設定
//    void SetPosition(const DirectX::XMFLOAT3& position) { this->position = position; }
//
//    //回転取得
//    const DirectX::XMFLOAT3& GetAngle()const { return angle; }
//
//    //回転設定
//    void SetAngle(const DirectX::XMFLOAT3& angle) { this->angle = angle; }
//
//    //スケール設定
//    const DirectX::XMFLOAT3& GetScale()const { return scale; }
//
//    //スケール取得
//    void SetScale(const DirectX::XMFLOAT3& scale) { this->scale = scale; }
//
//    //速度取得
//    const DirectX::XMFLOAT3& GetVelocity()const { return velocity; }
//
//   
//    //地面に接しているか
//    bool IsGround() const { return isGround; }
//public:
//    //行列更新処理
//    void UpdateTransform();
//private:
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
//protected:
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
//
//protected:
//    
//    DirectX::XMFLOAT3 position = { 0,0,0 };             //位置
//    DirectX::XMFLOAT3 angle = { 0,0,0 };                //向く角度
//    DirectX::XMFLOAT3 scale = { 1,1,1 };                //大きさ
//    DirectX::XMFLOAT4X4 transform = {
//        1,0,0,0,
//        0,1,0,0,
//        0,0,1,0,
//        0,0,0,1
//    };                                                  //形の変形(調べたら変身的な意味らしいよ)
//    DirectX::XMFLOAT4 color = { 1.0f,1.0f,1.0f,1.0f };              //色
//    std::unique_ptr<Model> model;                       //モデル
//protected:
//    float gravity = -1.0f;                              //重力
//    DirectX::XMFLOAT3 velocity = { 0,0,0 };             //速度
//
//    bool isGround = false;                              //地面と接地しているか
//    
//    float friction = 0.5f;                              //摩擦
//    float acceleration = 1.0f;                          //加速度
//    float maxMoveSpeed = 5.0f;                          //最大速度
//
//    float moveVecX = 0.0f;                              //計算用
//    float moveVecZ = 0.0f;                              //計算用
//
//    float airControl = 0.3f;                            //空中制御(移動)
//    
//    float stepOffset = 1.0f;                            //開始位置を上にするため
//
//    float slopeRate = 1.0f;                              //傾斜率
//};