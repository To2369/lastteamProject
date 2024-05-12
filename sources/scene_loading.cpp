#include "scene_loading.h"
#include "Graphics/graphics.h"

void SceneLoading::initialize()
{
    Graphics& graphics = Graphics::Instance();

    //�X���b�h�J�n
    thread = new std::thread(LoadingThread, this);    
}

void SceneLoading::update(float elapsed_time)
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

void SceneLoading::render(float elapsed_time)
{
    RenderContext rc;
    Graphics& graphics = Graphics::Instance();

    graphics.renderinit();

    graphics.GetDeviceContext()->OMSetDepthStencilState(graphics.GetDepthStencilState(0), 1);

    graphics.GetDeviceContext()->OMSetBlendState(graphics.GetBlendState(2), nullptr, 0xFFFFFFFF);

    D3D11_VIEWPORT viewport;
    UINT num_viewports{ 1 };
    graphics.GetDeviceContext()->RSGetViewports(&num_viewports, &viewport);
    
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

void SceneLoading::LoadingThread(SceneLoading* scene)
{
    //COM�֘A�̏������ŃX���b�h���ɌĂԕK�v������
    CoInitialize(nullptr);

    //���̃V�[���̏��������s��
   // OutputDebugString("00000000000000000000000000000000000000000\n");
    scene->nextScene->initialize();
    //OutputDebugString("11111111111111111111111111111111111111111\n");
    //�X���b�h���I���O��COM�֘A�̏I����
    CoUninitialize();

    //���̃V�[���̏��������ݒ�
    scene->nextScene->SetReady();
}
