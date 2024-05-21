#pragma once

#include "scene_management.h"
#include <thread>
#include"Graphics/sprite.h"
//ローディングシーン
class SceneLoading : public Scene
{
public:
    SceneLoading() {}
    SceneLoading(Scene* nextScene) :nextScene(nextScene) {}
    ~SceneLoading()override {}

    //開始処理
    void initialize()override;
    //更新処理
    void update(float elapsed_time)override;
    //描画処理
    void render(float elapsed_time)override;
    //終了処理
    void finalize()override;
    //framebuffer設定
    void setFramebuffer() override {};
private:
    //ローディングスレッド
    static void LoadingThread(SceneLoading* scene);
private:
    
    float angle = 0.0f;
    std::unique_ptr<sprite>sp;
    Scene* nextScene = nullptr;
    std::thread* thread = nullptr;
    
};