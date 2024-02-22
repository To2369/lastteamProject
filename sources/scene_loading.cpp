#include "scene_loading.h"

void SceneLoading::initialize(ID3D11Device* device, float x, float y)
{
    //�X���b�h�J�n
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
    //�X���b�h�I����
    if (thread != nullptr)
    {
        thread->join();
        delete thread;
        thread = nullptr;
    }
}

void SceneLoading::LoadingThread(SceneLoading* scene, ID3D11Device* device, float x , float y)
{
    //COM�֘A�̏������ŃX���b�h���ɌĂԕK�v������
    CoInitialize(nullptr);

    //���̃V�[���̏��������s��
    scene->nextScene->initialize(device,x,y);
    //�X���b�h���I���O��COM�֘A�̏I����
    CoUninitialize();

    //���̃V�[���̏��������ݒ�
    scene->nextScene->SetReady();
}
