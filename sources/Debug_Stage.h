#pragma once
#pragma once
#include"object.h"
enum class StageName
{
    Tutolial,
    stage1,
    null,
};
class DebugStage :public Object
{
public:
    DebugStage(ID3D11Device*device);
    ~DebugStage()override {};
    void Update(float elapsedTime)override;
    void Render(ID3D11DeviceContext* dc)override;
    //�X�e�[�W�ɑ΂��ăI�u�W�F�N�g�̃��C�L���X�g����
    bool modelRay_VS_Stage(Intersection& inter, XMFLOAT3 FastPos, XMFLOAT3 normal/*���C�̌���*/, float rayLength/*���C�̒���*/);
    void SetStage(StageName n) { s_name = n; }
    StageName GetNowStage() { return s_name; }
public:
    //���X�C�b�`�ɏ���Ă�I�u�W�F�̂��Z�b�g
    void Set_GameObjType(ObjType obj) { get_gameobj = obj; };
    void Set_Color(XMFLOAT4 color_) { color = color_; }
public:
    ObjType Get_GameObjType() { return get_gameobj; }
    XMFLOAT4 GetColor() { return color; }
private:
    ObjType get_gameobj = ObjType::null;
    //const char* filename = ".\\resources\\3Dmodel\\room.fbx";
    const char* filename = ".\\resources\\ground.fbx";
    StageName s_name = StageName::stage1;
};
