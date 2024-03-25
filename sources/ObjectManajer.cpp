#include"objectManajer.h"
#include "../imgui/imgui.h"
#include"StageManager.h"
#include"Heavy.h"
#include"Cution.h"
#include <cassert>
#include"Graphics/DebugRenderer.h"
#include <Fragile.h>
#include <Hard_to_Break.h>
#include"Crack.h"
using namespace std;
using namespace DirectX;
void Objectmanajer::Initialize_Obj(StageName s_name_,ObjType type_name, ID3D11Device* device,XMFLOAT3 pos)
{
    unique_ptr<Object> obj;

    switch (type_name)
    {
    case Obj_attribute::cution:
        obj=make_unique<Cution>(device);
        obj->SetPosition({pos});
        obj->Set_MystageName(s_name_);
        Rigister_obj(move(obj));
        break;
    case Obj_attribute::Super_cution:
        obj = make_unique<Super_Cution>(device);
        obj->SetPosition({ pos });
        obj->Set_MystageName(s_name_);
        Rigister_obj(move(obj));
        break;
    case Obj_attribute::heavy:
        obj = make_unique<Heavy>(device);
        obj->SetPosition({pos});
        obj->Set_MystageName(s_name_);
        Rigister_obj(move(obj));
        break;
    case Obj_attribute::Super_heavy:
        obj = make_unique<Super_Heavy>(device);
        obj->SetPosition({ pos });
        obj->Set_MystageName(s_name_);
        Rigister_obj(move(obj));
        break;
    case Obj_attribute::Fragile:
        obj = make_unique<Fragile>(device);
        obj->SetPosition({ pos });
        obj->Set_MystageName(s_name_);
        Rigister_obj(move(obj));
        break;
    case Obj_attribute::Super_fragile:
        obj = make_unique<Super_fragile>(device);
        obj->SetPosition({ pos });
        obj->Set_MystageName(s_name_);
        Rigister_obj(move(obj));
        break;
    case Obj_attribute::Hard_to_Break:
        obj = make_unique<Hard_to_Break>(device);
        obj->SetPosition({ pos });
        obj->Set_MystageName(s_name_);
        Rigister_obj(move(obj));
        break;
    case Obj_attribute::Super_hard_to_Break:
        obj = make_unique<Super_hard_to_Break>(device);
        obj->SetPosition({ pos });
        obj->Set_MystageName(s_name_);
        Rigister_obj(move(obj));
        break;
    case Obj_attribute::Crack:
        obj = make_unique<Crack>(device);
        obj->SetPosition({ pos });
        obj->Set_MystageName(s_name_);
        Rigister_obj(move(obj));
        break;
    case Obj_attribute::null:
        break;
    default:
        break;
    }
   
   
}

void Objectmanajer::Initialize_Gimic(StageName s_name_, Gimic_Type type_name, ID3D11Device* device, XMFLOAT3 pos)
{
    unique_ptr<Gimic>gimic;
    switch (type_name)
    {
    case Gimic_Type::Switch:
        gimic = make_unique<Switch>(device);
        gimic->SetPosition(pos);
        gimic->Set_MystageName(s_name_);
        Rigister_Gimic(move(gimic));
        break;
    case Gimic_Type::Door:
        gimic = make_unique<Door>(device);
        gimic->SetPosition(pos);
        gimic->Set_MystageName(s_name_);
        Rigister_Gimic(move(gimic));
        break;
    case Gimic_Type::Goal:
        gimic = make_unique<Goal>(device);
        gimic->SetPosition(pos);
        gimic->Set_MystageName(s_name_);
        Rigister_Gimic(move(gimic));
        break;
    case Gimic_Type::Drop_Road:
        gimic = make_unique<DropBox_Road>(device);
        gimic->SetPosition(pos);
        gimic->Set_MystageName(s_name_);
        Rigister_Gimic(move(gimic));
        break;
    case Gimic_Type::null:
        break;
    default:
        break;
    }
}

void Objectmanajer::Update(float elapsedTime)
{
    StageManager& ince = StageManager::incetance();
  /* int count = game_objs.size();
    for (int i=0;i<count;i++)
    {
        game_objs[i]->Update(elapsedTime);
    }*/

    for (const auto& obj: game_objs)
    {
        obj->Update(elapsedTime);   
    }
    for (const auto& obj : game_objs)
    {
        Set_eraceObgect(*obj.get());
    }

    for (const auto& gimic : game_Gimics)
    {
        gimic->Update(elapsedTime);
    }
    for (const auto& gimic : game_Gimics)
    {
        Set_eraceObgect(*gimic.get());
    }



  

}

void Objectmanajer::render(RenderContext* rc)
{
    for (const auto& obj : game_objs)
    {
        obj->Render(rc);
    }
    for (const auto& gimic : game_Gimics)
    {
        gimic->Render(rc);
    }
}

void Objectmanajer::Clear()
{
    game_Gimics.clear();
    game_objs.clear();
}


void Objectmanajer::Gui(ID3D11Device* device)
{

    DebugRenderer&ince_debug=DebugRenderer::incetance(device);
    enum
    {
        max,
        min,
    };
	ImGui::Begin("ObjectManager");
    int size = game_objs.size();
    ImGui::InputInt("size", &size);
    for (const auto& obj : game_objs)
    {
        XMFLOAT3 leftup_mae{};
        XMFLOAT3 rightup_mae{};
       
        XMFLOAT3 leftdown_mae{};
        XMFLOAT3 rightdown_mae{};

        XMFLOAT3 leftup_back{};
        XMFLOAT3 rightup_back{};

        XMFLOAT3 leftdown_back{};
        XMFLOAT3 rightdown_back{};

       /* XMFLOAT3 LeftUp_Max{  0.05f,0.05f,0.05f };
        XMFLOAT3 RightUp_Max{ -0.05f,0.05f,0.05f };

        XMFLOAT3 LeftDown_Max{ 0.05,-0.05f,0.05f };
        XMFLOAT3 RightDown_Max{ -0.05f,-0.05f,0.05f };

        XMFLOAT3 LeftUp_Min{ 0.05f,0.05f,-0.05f };
        XMFLOAT3 RightUp_Min{ -0.05f,0.05f,-0.05f };
                          
        XMFLOAT3 LeftDown_Min{ 0.05f,-0.05f,-0.05f };
        XMFLOAT3 RightDown_Min{ -0.05f,-0.05f,-0.05f };*/


        XMFLOAT3 Maxpos = obj->GetModel()->bounding_box[max];
        XMFLOAT3 Minpos = obj->GetModel()->bounding_box[min];
        XMStoreFloat(&Maxpos.x,XMVectorScale(XMLoadFloat(&Maxpos.x),0.1f*(obj->GetScale().x*0.1f)));
        XMStoreFloat(&Maxpos.y,XMVectorScale(XMLoadFloat(&Maxpos.y),0.1f*(obj->GetScale().y*0.1f)));
        XMStoreFloat(&Maxpos.z,XMVectorScale(XMLoadFloat(&Maxpos.z),0.1f*(obj->GetScale().z*0.1f)));

        XMStoreFloat(&Minpos.x,XMVectorScale(XMLoadFloat(&Minpos.x),0.1f*(obj->GetScale().x*0.1f)));
        XMStoreFloat(&Minpos.y,XMVectorScale(XMLoadFloat(&Minpos.y),0.1f*(obj->GetScale().y*0.1f)));
        XMStoreFloat(&Minpos.z,XMVectorScale(XMLoadFloat(&Minpos.z),0.1f*(obj->GetScale().z*0.1f)));
        XMFLOAT3 LeftUp_Max{ Maxpos.x,Maxpos.y,Maxpos.z };
        XMFLOAT3 RightUp_Max{ -Maxpos.x,Maxpos.y,Maxpos.z };

        XMFLOAT3 LeftDown_Max{ Maxpos.x,-Maxpos.y,Maxpos.z };
        XMFLOAT3 RightDown_Max{ -Maxpos.x,-Maxpos.y,Maxpos.z };

        XMFLOAT3 LeftUp_Min{ Minpos.x, Minpos.y,Minpos.z };
        XMFLOAT3 RightUp_Min{ -Minpos.x, Minpos.y,Minpos.z };

        XMFLOAT3 LeftDown_Min{ Minpos.x,-Minpos.y,Minpos.z };
        XMFLOAT3 RightDown_Min{ -Minpos.x,-Minpos.y,Minpos.z };
        {
            XMVECTOR POS = DirectX::XMLoadFloat3(&obj->GetPosition());
            DirectX::XMStoreFloat3(&leftup_mae, DirectX::XMVectorAdd   (POS, DirectX::XMLoadFloat3(&LeftUp_Max)));
            DirectX::XMStoreFloat3(&rightup_mae, DirectX::XMVectorAdd  (POS, DirectX::XMLoadFloat3(&RightUp_Max)));
                                                                        
            DirectX::XMStoreFloat3(&leftdown_mae, DirectX::XMVectorAdd (POS, DirectX::XMLoadFloat3(&LeftDown_Max)));
            DirectX::XMStoreFloat3(&rightdown_mae, DirectX::XMVectorAdd(POS, DirectX::XMLoadFloat3(&RightDown_Max)));
                                                                        
            DirectX::XMStoreFloat3(&leftup_back, DirectX::XMVectorAdd  (POS, DirectX::XMLoadFloat3(&LeftUp_Min)));
            DirectX::XMStoreFloat3(&rightup_back, DirectX::XMVectorAdd (POS, DirectX::XMLoadFloat3(&RightUp_Min)));
                                                                        
            DirectX::XMStoreFloat3(&leftdown_back, DirectX::XMVectorAdd(POS, DirectX::XMLoadFloat3(&LeftDown_Min)));
            DirectX::XMStoreFloat3(&rightdown_back,DirectX::XMVectorAdd(POS, DirectX::XMLoadFloat3(&RightDown_Min)));

        }
        if (obj->Get_Old_Objtype(0)==ObjType::cution)
        {
            XMFLOAT3 pos{ obj->GetPosition() };
            XMFLOAT4 color_{ 1,1,1,1 };
            for (const auto& obj1 : game_objs)
            {
                if (obj1 == obj)continue;
    
              
            }
            ince_debug.DrawSphere(leftup_mae, 0.01f, color_);
            ince_debug.DrawSphere(rightup_mae, 0.01f, color_);

            ince_debug.DrawSphere(leftdown_mae, 0.01f, color_);
            ince_debug.DrawSphere(rightdown_mae, 0.01f, color_);

            ince_debug.DrawSphere(leftup_back, 0.01f, color_);
            ince_debug.DrawSphere(rightup_back, 0.01f, color_);

            ince_debug.DrawSphere(leftdown_back, 0.01f, color_);
            ince_debug.DrawSphere(rightdown_back, 0.01f, color_);
            ImGui::InputFloat("PosX", &pos.x);
            ImGui::InputFloat("PosY", &pos.y);
            ImGui::InputFloat("PosZ", &pos.z);
          
        }
    }
	ImGui::End();

}


bool Objectmanajer::Bounding_Box_vs_Bounding_Box(Object* obj, Object* gimic_obj, bool UpcheckFlag, float offset)
{
    enum
    {
        max,
        min,
    };
  
    XMFLOAT3 thisobj_min{};
    XMFLOAT3 thisobj_max{};
    XMFLOAT3 obj2_min{};
    XMFLOAT3 obj2_max{};

    XMFLOAT3 Maxpos1 = obj->GetModel()->bounding_box[min];
    XMFLOAT3 Minpos1 = obj->GetModel()->bounding_box[max];
    {
        XMStoreFloat(&Maxpos1.x, XMVectorScale(XMLoadFloat(&Maxpos1.x), 0.1f * (obj->GetScale().x * 0.1f)));
        XMStoreFloat(&Maxpos1.y, XMVectorScale(XMLoadFloat(&Maxpos1.y), 0.1f * (obj->GetScale().y * 0.1f)));
        XMStoreFloat(&Maxpos1.z, XMVectorScale(XMLoadFloat(&Maxpos1.z), 0.1f * (obj->GetScale().z * 0.1f)));

        XMStoreFloat(&Minpos1.x, XMVectorScale(XMLoadFloat(&Minpos1.x), 0.1f * (obj->GetScale().x * 0.1f)));
        XMStoreFloat(&Minpos1.y, XMVectorScale(XMLoadFloat(&Minpos1.y), 0.1f * (obj->GetScale().y * 0.1f)));
        XMStoreFloat(&Minpos1.z, XMVectorScale(XMLoadFloat(&Minpos1.z), 0.1f * (obj->GetScale().z * 0.1f)));
    }

    XMFLOAT3 Maxpos2 = gimic_obj->GetModel()->bounding_box[min];
    XMFLOAT3 Minpos2 = gimic_obj->GetModel()->bounding_box[max];
    {
        XMStoreFloat(&Maxpos2.x, XMVectorScale(XMLoadFloat(&Maxpos2.x), 0.1f * (gimic_obj->GetScale().x * 0.1f)));
        XMStoreFloat(&Maxpos2.y, XMVectorScale(XMLoadFloat(&Maxpos2.y), 0.1f * (gimic_obj->GetScale().y * 0.1f)));
        XMStoreFloat(&Maxpos2.z, XMVectorScale(XMLoadFloat(&Maxpos2.z), 0.1f * (gimic_obj->GetScale().z * 0.1f)));

        XMStoreFloat(&Minpos2.x, XMVectorScale(XMLoadFloat(&Minpos2.x), 0.1f * (gimic_obj->GetScale().x * 0.1f)));
        XMStoreFloat(&Minpos2.y, XMVectorScale(XMLoadFloat(&Minpos2.y), 0.1f * (gimic_obj->GetScale().y * 0.1f)));
        XMStoreFloat(&Minpos2.z, XMVectorScale(XMLoadFloat(&Minpos2.z), 0.1f * (gimic_obj->GetScale().z * 0.1f)));
    }


    DirectX::XMStoreFloat3(&thisobj_min, DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&Minpos1), DirectX::XMLoadFloat3(&obj->GetPosition())));
    DirectX::XMStoreFloat3(&thisobj_max, DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&Maxpos1), DirectX::XMLoadFloat3(&obj->GetPosition())));
    DirectX::XMStoreFloat3(&obj2_min, DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&Minpos2), DirectX::XMLoadFloat3(&gimic_obj->GetPosition())));
    DirectX::XMStoreFloat3(&obj2_max, DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&Maxpos2), DirectX::XMLoadFloat3(&gimic_obj->GetPosition())));
    
    if (thisobj_min.x <= obj2_max.x && thisobj_max.x >= obj2_min.x &&
        thisobj_min.y <= obj2_max.y && thisobj_max.y >= obj2_min.y &&
        thisobj_min.z <= obj2_max.z && thisobj_max.z >= obj2_min.z)
    {
        if (UpcheckFlag)
        {
            
            
                float t_pos = obj->GetPosition().y;
                float o_pos2 = gimic_obj->GetPosition().y - offset;
                if (t_pos > o_pos2)
                {
                    obj->Set_isGimic_UpPosNow(true);
                }
                else obj->Set_isGimic_UpPosNow(false);
            
            
        }
        return true;
    }
    obj->Set_isGimic_UpPosNow(false);
    return false;
}
