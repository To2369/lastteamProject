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
#include"Goal_navi.h"
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

void Objectmanajer::Initialize_Gimic(StageName s_name_, Gimic_Type type_name, ID3D11Device* device, XMFLOAT3 pos,std::string id)
{
    unique_ptr<Gimic>gimic;
    switch (type_name)
    {
    case Gimic_Type::Switch:
        gimic = make_unique<Switch>(device);
        gimic->SetPosition(pos);
        gimic->Set_MystageName(s_name_);
        gimic->SetGimicID(id);
        Rigister_Gimic(move(gimic));
        break;
    case Gimic_Type::Door:
        gimic = make_unique<Door>(device);
        gimic->SetPosition(pos);
        gimic->Set_MystageName(s_name_);
        gimic->SetGimicID(id);
        Rigister_Gimic(move(gimic));
        break;
    case Gimic_Type::Goal:
        gimic = make_unique<Goal>(device);
        gimic->SetPosition(pos);
        gimic->Set_MystageName(s_name_);
        gimic->SetGimicID(id);
        Rigister_Gimic(move(gimic));
        break;
    case Gimic_Type::Drop_Road:
        gimic = make_unique<DropBox_Road>(device);
        gimic->SetPosition(pos);
        gimic->Set_MystageName(s_name_);
        gimic->SetGimicID(id);
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
        if (obj->Get_Old_Objtype(0)==ObjType::heavy)
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

bool IsCollision(const XMFLOAT3& minPos1, const XMFLOAT3& maxPos1, const XMFLOAT3& minPos2, const XMFLOAT3& maxPos2) {
    if (minPos1.x <= maxPos2.x && maxPos1.x >= minPos2.x &&
        minPos1.y <= maxPos2.y && maxPos1.y >= minPos2.y &&
        minPos1.z <= maxPos2.z && maxPos1.z >= minPos2.z) {
        return true; // 重なっている
    }
    return false; // 重なっていない
}
void CorrectPosition(XMFLOAT3& objectPosition1, const XMFLOAT3& minPos1, const XMFLOAT3& maxPos1, const XMFLOAT3& minPos2, const XMFLOAT3& maxPos2) {
    // オブジェクト1とオブジェクト2の境界ボックスの重なりを解消するための補正ベクトルを計算する
    float moveX = 0.0f, moveY = 0.0f, moveZ = 0.0f;

    // X方向の重なりを解消するための補正ベクトルを計算する
    if (minPos1.x <= maxPos2.x && maxPos1.x >= minPos2.x) {
        float overlapX1 = maxPos2.x - minPos1.x;
        float overlapX2 = maxPos1.x - minPos2.x;
        moveX = (overlapX1 < overlapX2) ? -overlapX1 : overlapX2;
    }

    // Y方向の重なりを解消するための補正ベクトルを計算する
    if (minPos1.y <= maxPos2.y && maxPos1.y >= minPos2.y) {
        float overlapY1 = maxPos2.y - minPos1.y;
        float overlapY2 = maxPos1.y - minPos2.y;
        moveY = (overlapY1 < overlapY2) ? -overlapY1 : overlapY2;
    }

    // Z方向の重なりを解消するための補正ベクトルを計算する
    if (minPos1.z <= maxPos2.z && maxPos1.z >= minPos2.z) {
        float overlapZ1 = maxPos2.z - minPos1.z;
        float overlapZ2 = maxPos1.z - minPos2.z;
        moveZ = (overlapZ1 < overlapZ2) ? -overlapZ1 : overlapZ2;
    }

    // 補正ベクトルを使ってオブジェクト1を移動させる
    objectPosition1.x += moveX;
    objectPosition1.y += moveY;
    objectPosition1.z += moveZ;
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
    if (UpcheckFlag)
    {
        if (thisobj_min.x <= obj2_max.x && thisobj_max.x >= obj2_min.x &&
            thisobj_min.y <= obj2_max.y && thisobj_max.y >= obj2_min.y &&
            thisobj_min.z <= obj2_max.z && thisobj_max.z >= obj2_min.z)
        {



            float t_pos = obj->GetPosition().y;
            float o_pos2 = gimic_obj->GetPosition().y - offset;
            if (t_pos > o_pos2)
            {
                obj->Set_isGimic_UpPosNow(true);
            }
            else obj->Set_isGimic_UpPosNow(false);



            return true;
        }
        obj->Set_isGimic_UpPosNow(false);
        return false;
    }
    else
    {
        if (IsCollision(thisobj_min, thisobj_max, obj2_min, obj2_max)) {
            // オブジェクト1をオブジェクト2の境界ボックス内に移動させるための補正処理を行う
            XMFLOAT3 pos = obj->GetPosition();

            CorrectPosition(pos, thisobj_min, thisobj_max, obj2_min, obj2_max);
            obj->SetPosition(pos);
            return true;
        }
        //{
        //    using namespace DirectX;

        //    XMVECTOR MinthisPos = XMLoadFloat3(&thisobj_min);
        //    XMVECTOR MaxthisPos = XMLoadFloat3(&thisobj_max);
        //    XMVECTOR Minobj2Pos = XMLoadFloat3(&obj2_min);
        //    XMVECTOR Maxobj2Pos = XMLoadFloat3(&obj2_max);

        //    XMVECTOR penetration = XMVectorZero();
        //    penetration = XMVectorMax(penetration, XMVectorSubtract(Minobj2Pos, MaxthisPos));
        //    penetration = XMVectorMax(penetration, XMVectorSubtract(MinthisPos, Maxobj2Pos));

        //    XMFLOAT3 p = obj->GetPosition();
        //    XMVECTOR pos = XMLoadFloat3(&p);
        //    // 補正位置を計算
        //    XMVECTOR correction = XMVectorMultiply(penetration, XMVectorReplicate(0.5f)); // めり込んだ量の半分を補正量とする
        //    XMVECTOR correctedPosition = XMVectorAdd(pos, correction); // 現在の位置に補正量を加算して補正位置を計算
        //    XMFLOAT3 correctedPositionFloat3;
        //    XMStoreFloat3(&correctedPositionFloat3, correctedPosition); // XMVECTORをXMFLOAT3に変換    
        //    obj->SetPosition(correctedPositionFloat3);

        //    return true;
        //}
        obj->Set_isGimic_UpPosNow(false);
        return false;
    }
}

