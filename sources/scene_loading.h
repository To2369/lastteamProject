#pragma once

#include "scene_management.h"
#include <thread>

//ローディングシーン
class SceneLoading : public Scene
{
public:
    SceneLoading() {}
    SceneLoading(Scene* nextScene) :nextScene(nextScene) {}
    ~SceneLoading()override {}

    //開始処理
    void initialize(ID3D11Device* device, float x = 1280, float y = 720)override;
    //更新処理
    void update(float elapsed_time, ID3D11Device* device, float,float)override;
    //描画処理
    void render(float elapsed_time, RenderContext& rc)override;
    //終了処理
    void finalize()override;

private:
    //ローディングスレッド
    static void LoadingThread(SceneLoading* scene, ID3D11Device* device, float x = 1280, float y = 720);
private:
    
    float angle = 0.0f;

    Scene* nextScene = nullptr;
    std::thread* thread = nullptr;
    
};