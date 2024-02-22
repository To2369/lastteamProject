#include "scene_loading.h"

void SceneLoading::initialize(ID3D11Device* device, float x, float y)
{
    //スレッド開始
    thread = new std::thread(LoadingThread, this,device,x,y);
    
}

void SceneLoading::update(float elapsed_time, ID3D11Device* device, float x,float y)
{
    if (nextScene->IsReady() == true)
    {
        SceneManagement::instance().SceneChange(nextScene);
        nextScene = nullptr;
    }

#ifdef _DEBUG
#ifdef USE_IMGUI
    ImGui::Begin("SceneLoading");
    ImGui::End();
#endif
#endif
}

void SceneLoading::render(float elapsed_time, RenderContext& rc)
{

}

void SceneLoading::finalize()
{
    //スレッド終了化
    if (thread != nullptr)
    {
        thread->join();
        delete thread;
        thread = nullptr;
    }
}

void SceneLoading::LoadingThread(SceneLoading* scene, ID3D11Device* device, float x , float y)
{
    //COM関連の初期化でスレッド毎に呼ぶ必要がある
    CoInitialize(nullptr);

    //次のシーンの初期化を行う
    scene->nextScene->initialize(device,x,y);
    //スレッドが終わる前にCOM関連の終了化
    CoUninitialize();

    //次のシーンの準備完了設定
    scene->nextScene->SetReady();
}
