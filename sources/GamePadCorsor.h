#pragma once
#include"Graphics\sprite.h"
#include"memory"
#include<vector>
#include "scene_management.h"
class GamePadCorsor
{
public:
    GamePadCorsor() {};
    ~GamePadCorsor() {};

    static GamePadCorsor& Instance()
    {
        static GamePadCorsor ins;
        return ins;
    };

    void Initialize();

    void Update(float elapsedTime);

    void Render(RenderContext* rc);

public:
    //�J�[�\���Ǝl�p�`�̓����蔻��
    bool hitChechLect(DirectX::XMFLOAT2 pos1, DirectX::XMFLOAT2 pos2, DirectX::XMFLOAT2 size1, DirectX::XMFLOAT2 size2);

    DirectX::XMFLOAT2 GetPadCursorsprPos(){return PadCursorsprPos;}
    void SetPadCursorsprPos(const DirectX::XMFLOAT2 pos) { PadCursorsprPos = pos; }
private:
    std::unique_ptr<sprite> padcursorspr;		//�}�E�X�̃X�v���C�g�`��
    const wchar_t* filename = L".\\resources\\mouse\\padcursor.png";      //�J�[�\���̃t�@�C��
    DirectX::XMFLOAT2 PadCursorsprPos = { 0,0 };        //�p�b�h�J�[�\���̌��݈ʒu

    DirectX::XMFLOAT2 PadcursorPos = {};                //�}�E�X�J�[�\���̌��݈ʒu
    DirectX::XMFLOAT2 currentPadCursorPos = {};

    float speed = 0;

    struct square
    {
        float left, right, top, bottom;
    };
    DirectX::XMFLOAT4 color = { 0,0,0,0 };

    bool padCorsorFlag = false;     //�p�b�h�J�[�\�����g����
    bool disPlayFlag = false;       //�p�b�h�J�[�\����\�����邩

    float timer = 0;                //�p�b�h�J�[�\����������܂ł̎��Ԃ��Z�b�g����ꏊ
    float timerset = 20;            //�p�b�h�J�[�\����������܂ł̎���
};