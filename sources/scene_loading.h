#pragma once

#include "scene_management.h"
#include <thread>

//���[�f�B���O�V�[��
class SceneLoading : public Scene
{
public:
    SceneLoading() {}
    SceneLoading(Scene* nextScene) :nextScene(nextScene) {}
    ~SceneLoading()override {}

    //�J�n����
    void initialize(ID3D11Device* device, float x = 1280, float y = 720)override;
    //�X�V����
    void update(float elapsed_time, ID3D11Device* device, float,float)override;
    //�`�揈��
    void render(float elapsed_time, RenderContext& rc)override;
    //�I������
    void finalize()override;

private:
    //���[�f�B���O�X���b�h
    static void LoadingThread(SceneLoading* scene, ID3D11Device* device, float x = 1280, float y = 720);
private:
    
    float angle = 0.0f;

    Scene* nextScene = nullptr;
    std::thread* thread = nullptr;
    
};