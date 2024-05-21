#pragma once

#include "scene_management.h"
#include <thread>
#include"Graphics/sprite.h"
//���[�f�B���O�V�[��
class SceneLoading : public Scene
{
public:
    SceneLoading() {}
    SceneLoading(Scene* nextScene) :nextScene(nextScene) {}
    ~SceneLoading()override {}

    //�J�n����
    void initialize()override;
    //�X�V����
    void update(float elapsed_time)override;
    //�`�揈��
    void render(float elapsed_time)override;
    //�I������
    void finalize()override;
    //framebuffer�ݒ�
    void setFramebuffer() override {};
private:
    //���[�f�B���O�X���b�h
    static void LoadingThread(SceneLoading* scene);
private:
    
    float angle = 0.0f;
    std::unique_ptr<sprite>sp;
    Scene* nextScene = nullptr;
    std::thread* thread = nullptr;
    
};