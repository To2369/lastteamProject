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
#include <Player.h>
#include <algorithm>
using namespace std;
using namespace DirectX;

void Objectmanajer::Initialize(ObjType type_name, ID3D11Device* device, bool static_flag, XMFLOAT3 pos,float radius, Object::CollitionList raylist)
{
    unique_ptr<Object> obj;
    
    switch (type_name)
    {
    case Obj_attribute::cution:
        obj = make_unique<Cution>(device);
        obj->SetPosition({ pos });
        break;
    case Obj_attribute::Super_cution:
        obj = make_unique<Super_Cution>(device);
        obj->SetPosition({ pos });
        break;
    case Obj_attribute::heavy:
        obj = make_unique<Heavy>(device);
        obj->SetPosition({ pos });
        break;
    case Obj_attribute::Super_heavy:
        obj = make_unique<Super_Heavy>(device);
        obj->SetPosition({ pos });
        break;
    case Obj_attribute::Fragile:
        obj = make_unique<Fragile>(device);
        obj->SetPosition({ pos });
        break;
    case Obj_attribute::Super_fragile:
        obj = make_unique<Super_fragile>(device);
        obj->SetPosition({ pos });
        break;
    case Obj_attribute::Hard_to_Break:
        obj = make_unique<Hard_to_Break>(device);
        obj->SetPosition({ pos });
        break;
    case Obj_attribute::Super_hard_to_Break:
        obj = make_unique<Super_hard_to_Break>(device);
        obj->SetPosition({ pos });
        break;
    case Obj_attribute::Crack:
        obj = make_unique<Crack>(device);
        obj->SetPosition({ pos });
        break;
    default:
        obj = make_unique<stage_OBJ>(device);
        break;
    }
    obj->SetRadius(radius);
    obj->SetStatic_Objflag(static_flag);
    obj->israycast = raylist;
    Rigister_obj(move(obj));

}

void Objectmanajer::Initialize(const char* filename,ObjType type_name, ID3D11Device* device, bool static_flag, XMFLOAT3 pos,float radius, Object::CollitionList raylist)
{
    unique_ptr<Object> obj;
    switch (type_name)
    {
    case Obj_attribute::cution:
        obj = make_unique<Cution>(device,filename);
        obj->SetPosition({ pos });
        break;
    case Obj_attribute::Super_cution:
        obj = make_unique<Super_Cution>(device,filename);
        obj->SetPosition({ pos });
        break;
    case Obj_attribute::heavy:
        obj = make_unique<Heavy>(device,filename);
        obj->SetPosition({ pos });
        break;
    case Obj_attribute::Super_heavy:
        obj = make_unique<Super_Heavy>(device,filename);
        obj->SetPosition({ pos });
        break;
    case Obj_attribute::Fragile:
        obj = make_unique<Fragile>(device,filename);
        obj->SetPosition({ pos });
        break;
    case Obj_attribute::Super_fragile:
        obj = make_unique<Super_fragile>(device,filename);
        obj->SetPosition({ pos });
        break;
    case Obj_attribute::Hard_to_Break:
        obj = make_unique<Hard_to_Break>(device,filename);
        obj->SetPosition({ pos });
        break;
    case Obj_attribute::Super_hard_to_Break:
        obj = make_unique<Super_hard_to_Break>(device,filename);
        obj->SetPosition({ pos });
        break;
    case Obj_attribute::Crack:
        obj = make_unique<Crack>(device,filename);
        obj->SetPosition({ pos });
        break;
    default:
        obj = make_unique<stage_OBJ>(device);
       
        break;
    }
    obj->SetRadius(radius);
    obj->SetStatic_Objflag(static_flag);
    obj->israycast = raylist;
    Rigister_obj(move(obj));
}

void Objectmanajer::Initialize(StageName s_name_, Gimic_Type type_name, ID3D11Device* device, XMFLOAT3 pos, std::string id, XMFLOAT3 endpos)
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
        gimic->SetScale({ 1.231f,1.f,1.f });
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
    case Gimic_Type::Lift:
        gimic = make_unique<Lift>(device,pos);
        gimic->SetEndPos(endpos);
        gimic->Set_MystageName(s_name_);
        gimic->SetGimicID(id);
        gimic->SetAngle({ 0.f,0.f,0.f });
        Rigister_Gimic(move(gimic));
        break;
    case Gimic_Type::Move_Object:
        gimic = make_unique<Lift>(device, pos);
        gimic->SetEndPos(endpos);
        gimic->Set_MystageName(s_name_);
        gimic->SetGimicID(id);
        gimic->SetAngle({ 0.f,0.f,0.f });
        Rigister_Gimic(move(gimic));
        break;
    case Gimic_Type::null:
        break;
    default:
        break;
    }
}


void Objectmanajer::Initialize(Chain_Type type, XMFLOAT3 pos, std::string id,XMFLOAT3 angle)
{
    unique_ptr<BaseChain> obj=nullptr;
    switch (type)
    {
    case Chain_Type::lift_chain_S:
        obj = make_unique<Lift_chain_s>();
         break;
    case Chain_Type::lift_chain_L:
        obj = make_unique<Lift_chain_l>();
        break;
    case Chain_Type::lift_P_Animatio_ndown:
        obj = make_unique<Lift_chain_Animatio_ndown>();
        break;
    case Chain_Type::lift_P_Not_Animation:
        obj = make_unique<Lift_chain_P>();
        break;
    }
    if (obj)
    {
        obj->GetComp()->GetComponent<TransformComp>()->SetPosition(pos);
        obj->SetID(id);
        obj->GetComp()->GetComponent<TransformComp>()->SetAngle(angle);

        Rigister_Lift_Chains(move(obj));
    }
}

void Objectmanajer::Initialize(const char* filename, Chain_Type type, XMFLOAT3 pos, std::string id, XMFLOAT3 angle)
{
    unique_ptr<BaseChain> obj=nullptr;
    switch (type)
    {
    case Chain_Type::lift_chain_S:
        obj = make_unique<Lift_chain_s>(filename);
        break;
    case Chain_Type::lift_chain_L:
        obj = make_unique<Lift_chain_l>(filename);
        break;
    case Chain_Type::lift_P_Animatio_ndown:
        obj = make_unique<Lift_chain_Animatio_ndown>(filename);
        break;
    case Chain_Type::lift_P_Not_Animation:
        obj = make_unique<Lift_chain_P>(filename);
        break;
    }
    if (obj)
    {
        obj->GetComp()->GetComponent<TransformComp>()->SetPosition(pos);
        obj->SetID(id);
        obj->GetComp()->GetComponent<TransformComp>()->SetAngle(angle);
        Rigister_Lift_Chains(move(obj));
    }
}

void Objectmanajer::Initialize(const char* filename, Chain_Type type, XMFLOAT3 pos, float offset, std::string id, XMFLOAT3 angle)
{
    unique_ptr<BaseChain> obj = nullptr;
    switch (type)
    {
    case Chain_Type::lift_chain_S:
        obj = make_unique<Lift_chain_s>(filename);
        break;
    case Chain_Type::lift_chain_L:
        obj = make_unique<Lift_chain_l>(filename,offset);
        break;
    case Chain_Type::lift_P_Animatio_ndown:
        obj = make_unique<Lift_chain_Animatio_ndown>(filename);
        break;
    case Chain_Type::lift_P_Not_Animation:
        obj = make_unique<Lift_chain_P>(filename);
        break;
    }
    if (obj)
    {
        obj->GetComp()->GetComponent<TransformComp>()->SetPosition(pos);
        obj->SetID(id);
        obj->GetComp()->GetComponent<TransformComp>()->SetAngle(angle);
        Rigister_Lift_Chains(move(obj));
    }
}


void Objectmanajer::Initialize_InvisibleBarria(ID3D11Device* device, DirectX::XMFLOAT3 pos,XMFLOAT3 Scale_ )
{
    
    unique_ptr<Static_Object>obj = make_unique<InvisibleBarrier>(device,Scale_);
    obj->SetPosition(pos);
    Rigister_Static_Object(move(obj));
}

void Objectmanajer::Update(float elapsedTime)
{
    StageManager& ince = StageManager::incetance();
    /* int count = game_objs.size();
      for (int i=0;i<count;i++)
      {
          game_objs[i]->Update(elapsedTime);
      }*/

    for (const auto& obj : game_objs)
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

    for (const auto& chains : game_lift_chains)
    {
        chains->Update(elapsedTime);
    }
    for (const auto& chains : game_lift_chains)
    {
        Set_eraceObgect(*chains.get());
    }

    for (const auto& static_object : game_static_objes)
    {
        static_object->Update(elapsedTime);
    }
    for (const auto& static_object : game_static_objes)
    {
        Set_eraceObgect(*static_object.get());
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

    for (const auto& chains : game_lift_chains)
    {
        if(chains->GetComp()-> GetComponent<RenderComp>())
        chains->GetComp()-> GetComponent<RenderComp>()->render(Graphics::Instance(),chains->GetComp()-> GetComponent<TransformComp>()->Gettransform());
    }
    for (const auto& static_object : game_static_objes)
    {
        static_object->Render(rc);
    }
    
}

void Objectmanajer::Clear()
{
    game_Gimics.clear();
    game_objs.clear();
    game_static_objes.clear();
    game_lift_chains.clear();
}

bool Objectmanajer::Sphere_VS_Player(const XMFLOAT3& plPos_, const float& pl_radius, 
                                     const XMFLOAT3& objPos, const float& obj_radius, XMFLOAT3& outPos)
{
    using namespace DirectX;
    XMVECTOR PL_pos{XMLoadFloat3(&plPos_)};
    XMVECTOR OBJ_pos{XMLoadFloat3(&objPos)};
    XMVECTOR VEC{ XMVectorSubtract(OBJ_pos, PL_pos) };
    float LengthSq{ XMVectorGetX(XMVector3LengthSq(VEC)) };
    float rad = pl_radius + obj_radius;
    if (LengthSq < rad * rad)
    {
        VEC = XMVector3Normalize(VEC);
        float remainLenSq = rad - sqrtf(LengthSq);
        float weight = 0.5f;
        XMVECTOR pullLength{ XMVectorReplicate(-remainLenSq * weight) };
        PL_pos = XMVectorMultiplyAdd(VEC, pullLength, PL_pos);
        XMStoreFloat3(&outPos, PL_pos);
        return true;
    }

    return false;
}



void Objectmanajer::Gui(ID3D11Device* device)
{

    DebugRenderer& ince_debug = DebugRenderer::incetance(device);
    enum
    {
        max,
        min,
    };
    ImGui::Begin("ObjectManager");
    int size = game_objs.size();
    ImGui::InputInt("size", &size);
    for (const auto& obj : game_static_objes)
    {
        XMFLOAT3 leftup_mae{};
        XMFLOAT3 rightup_mae{};

        XMFLOAT3 leftdown_mae{};
        XMFLOAT3 rightdown_mae{};

        XMFLOAT3 leftup_back{};
        XMFLOAT3 rightup_back{};

        XMFLOAT3 leftdown_back{};
        XMFLOAT3 rightdown_back{};



        XMFLOAT3 Maxpos = obj->GetModel()->bounding_box[max];
        XMFLOAT3 Minpos = obj->GetModel()->bounding_box[min];
        XMStoreFloat(&Maxpos.x, XMVectorScale(XMLoadFloat(&Maxpos.x), 0.1f * (obj->GetScale().x * 0.1f)));
        XMStoreFloat(&Maxpos.y, XMVectorScale(XMLoadFloat(&Maxpos.y), 0.1f * (obj->GetScale().y * 0.1f)));
        XMStoreFloat(&Maxpos.z, XMVectorScale(XMLoadFloat(&Maxpos.z), 0.1f * (obj->GetScale().z * 0.1f)));

        XMStoreFloat(&Minpos.x, XMVectorScale(XMLoadFloat(&Minpos.x), 0.1f * (obj->GetScale().x * 0.1f)));
        XMStoreFloat(&Minpos.y, XMVectorScale(XMLoadFloat(&Minpos.y), 0.1f * (obj->GetScale().y * 0.1f)));
        XMStoreFloat(&Minpos.z, XMVectorScale(XMLoadFloat(&Minpos.z), 0.1f * (obj->GetScale().z * 0.1f)));
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
            DirectX::XMStoreFloat3(&leftup_mae, DirectX::XMVectorAdd(POS, DirectX::XMLoadFloat3(&LeftUp_Max)));
            DirectX::XMStoreFloat3(&rightup_mae, DirectX::XMVectorAdd(POS, DirectX::XMLoadFloat3(&RightUp_Max)));

            DirectX::XMStoreFloat3(&leftdown_mae, DirectX::XMVectorAdd(POS, DirectX::XMLoadFloat3(&LeftDown_Max)));
            DirectX::XMStoreFloat3(&rightdown_mae, DirectX::XMVectorAdd(POS, DirectX::XMLoadFloat3(&RightDown_Max)));

            DirectX::XMStoreFloat3(&leftup_back, DirectX::XMVectorAdd(POS, DirectX::XMLoadFloat3(&LeftUp_Min)));
            DirectX::XMStoreFloat3(&rightup_back, DirectX::XMVectorAdd(POS, DirectX::XMLoadFloat3(&RightUp_Min)));

            DirectX::XMStoreFloat3(&leftdown_back, DirectX::XMVectorAdd(POS, DirectX::XMLoadFloat3(&LeftDown_Min)));
            DirectX::XMStoreFloat3(&rightdown_back, DirectX::XMVectorAdd(POS, DirectX::XMLoadFloat3(&RightDown_Min)));

        }
        //if (obj->Get_Old_Objtype(0)==ObjType::heavy)
        if (1)
        {
            XMFLOAT3 pos{ obj->GetPosition() };
            XMFLOAT4 color_{ 1,1,1,1 };

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
//
//bool sort_( FaceBoundingBoxCollition& num,  FaceBoundingBoxCollition& num2)
//{
//  
//    if (num.length < num2.length)
//    {
//        return true;
//    }
//    return false;
//}

bool Objectmanajer::Bounding_Box_vs_Bounding_Box(Object* obj, Object* gimic_obj, bool UpcheckFlag, float offset)
{
    enum
    {
        max,
        min,
    };

    XMFLOAT3 thisobj_min{};
    XMFLOAT3 thisobj_max{};
    XMFLOAT3 obj_min{};
    XMFLOAT3 obj_max{};

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
    DirectX::XMStoreFloat3(&obj_min, DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&Minpos2), DirectX::XMLoadFloat3(&gimic_obj->GetPosition())));
    DirectX::XMStoreFloat3(&obj_max, DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&Maxpos2), DirectX::XMLoadFloat3(&gimic_obj->GetPosition())));
    if (UpcheckFlag)
    {
        if (thisobj_min.x <= obj_max.x && thisobj_max.x >= obj_min.x &&
            thisobj_min.y <= obj_max.y && thisobj_max.y >= obj_min.y &&
            thisobj_min.z <= obj_max.z && thisobj_max.z >= obj_min.z)
        {
            float t_pos = obj->GetPosition().y;
            float o_pos2 = gimic_obj->GetPosition().y + offset;
            if (t_pos >= o_pos2)
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
        if (thisobj_min.x <= obj_max.x && thisobj_max.x >= obj_min.x &&
            thisobj_min.y <= obj_max.y && thisobj_max.y >= obj_min.y &&
            thisobj_min.z <= obj_max.z && thisobj_max.z >= obj_min.z)
        {


            return true;
        }

    }
    obj->Set_isGimic_UpPosNow(false);
    return false;
}

bool Objectmanajer::Bounding_Box_vs_Bounding_Box(DirectX::XMFLOAT3 PL_position, DirectX::XMFLOAT3 OB_position,
    DirectX::XMFLOAT3 PL_scale, DirectX::XMFLOAT3 OB_scale,
    DirectX::XMFLOAT3* Pl_boundingbox, DirectX::XMFLOAT3* OB_boundingbox)
{
    enum
    {
        min,
        max,
    };

    XMFLOAT3 playerPos_min{};
    XMFLOAT3 playerPos_max{};
    XMFLOAT3 obj_min{};
    XMFLOAT3 obj_max{};

    XMFLOAT3 Maxpos1 = Pl_boundingbox[max];
    XMFLOAT3 Minpos1 = Pl_boundingbox[min];
    XMFLOAT3 plscale = PL_scale;
    {
        XMStoreFloat(&Maxpos1.x, XMVectorScale(XMLoadFloat(&Maxpos1.x), 0.1f * (plscale.x * 0.1f)));
        XMStoreFloat(&Maxpos1.y, XMVectorScale(XMLoadFloat(&Maxpos1.y), 0.1f * (plscale.y * 0.1f)));
        XMStoreFloat(&Maxpos1.z, XMVectorScale(XMLoadFloat(&Maxpos1.z), 0.1f * (plscale.z * 0.1f)));
        XMStoreFloat(&Minpos1.x, XMVectorScale(XMLoadFloat(&Minpos1.x), 0.1f * (plscale.x * 0.1f)));
        XMStoreFloat(&Minpos1.y, XMVectorScale(XMLoadFloat(&Minpos1.y), 0.1f * (plscale.y * 0.1f)));
        XMStoreFloat(&Minpos1.z, XMVectorScale(XMLoadFloat(&Minpos1.z), 0.1f * (plscale.z * 0.1f)));
    }

    XMFLOAT3 Maxpos2 = OB_boundingbox[max];
    XMFLOAT3 Minpos2 = OB_boundingbox[min];
    XMFLOAT3 objscale = OB_scale;
    {
        XMStoreFloat(&Maxpos2.x, XMVectorScale(XMLoadFloat(&Maxpos2.x), 0.1f * (OB_scale.x * 0.1f)));
        XMStoreFloat(&Maxpos2.y, XMVectorScale(XMLoadFloat(&Maxpos2.y), 0.1f * (OB_scale.y * 0.1f)));
        XMStoreFloat(&Maxpos2.z, XMVectorScale(XMLoadFloat(&Maxpos2.z), 0.1f * (OB_scale.z * 0.1f)));
        XMStoreFloat(&Minpos2.x, XMVectorScale(XMLoadFloat(&Minpos2.x), 0.1f * (OB_scale.x * 0.1f)));
        XMStoreFloat(&Minpos2.y, XMVectorScale(XMLoadFloat(&Minpos2.y), 0.1f * (OB_scale.y * 0.1f)));
        XMStoreFloat(&Minpos2.z, XMVectorScale(XMLoadFloat(&Minpos2.z), 0.1f * (OB_scale.z * 0.1f)));
    }


    DirectX::XMStoreFloat3(&playerPos_min, DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&Minpos1), DirectX::XMLoadFloat3(&PL_position)));
    DirectX::XMStoreFloat3(&playerPos_max, DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&Maxpos1), DirectX::XMLoadFloat3(&PL_position)));
    DirectX::XMStoreFloat3(&obj_min, DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&Minpos2), DirectX::XMLoadFloat3(&OB_position)));
    DirectX::XMStoreFloat3(&obj_max, DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&Maxpos2), DirectX::XMLoadFloat3(&OB_position)));

    if (playerPos_min.x <= obj_max.x && playerPos_max.x >= obj_min.x &&
        playerPos_min.y <= obj_max.y && playerPos_max.y >= obj_min.y &&
        playerPos_min.z <= obj_max.z && playerPos_max.z >= obj_min.z)
    {
        return true;
    }
    return false;
}


//bool Objectmanajer::Bounding_Box_vs_Bounding_Box(Player* player, Static_Object* obj2, FaceBoundingBoxCollition::face& face_check)
//
bool Objectmanajer::Bounding_Box_vs_Bounding_Box(Player* player, Static_Object* obj2)
{
#if 1
    enum
    {
        max,
        min,
    };
    XMFLOAT3 thisobj_min{};
    XMFLOAT3 thisobj_max{};
    XMFLOAT3 obj_min{};
    XMFLOAT3 obj_max{};

    XMFLOAT3 Maxpos1 = player->GetModel()->bounding_box[min];
    XMFLOAT3 Minpos1 = player->GetModel()->bounding_box[max];
    XMFLOAT3 plscale = player->GetScale();
    {
        XMStoreFloat(&Maxpos1.x, XMVectorScale(XMLoadFloat(&Maxpos1.x), 0.1f * (plscale.x * 0.1f)));
        XMStoreFloat(&Maxpos1.y, XMVectorScale(XMLoadFloat(&Maxpos1.y), 0.1f * (plscale.y * 0.1f)));
        XMStoreFloat(&Maxpos1.z, XMVectorScale(XMLoadFloat(&Maxpos1.z), 0.1f * (plscale.z * 0.1f)));

        XMStoreFloat(&Minpos1.x, XMVectorScale(XMLoadFloat(&Minpos1.x), 0.1f * (plscale.x * 0.1f)));
        XMStoreFloat(&Minpos1.y, XMVectorScale(XMLoadFloat(&Minpos1.y), 0.1f * (plscale.y * 0.1f)));
        XMStoreFloat(&Minpos1.z, XMVectorScale(XMLoadFloat(&Minpos1.z), 0.1f * (plscale.z * 0.1f)));
    }
    XMFLOAT3 Maxpos2 = obj2->GetModel()->bounding_box[min];
    XMFLOAT3 Minpos2 = obj2->GetModel()->bounding_box[max];
    XMFLOAT3 obj2scale = obj2->GetScale();
    {
        XMStoreFloat(&Maxpos2.x, XMVectorScale(XMLoadFloat(&Maxpos2.x), 0.1f * (obj2scale.x * 0.1f)));
        XMStoreFloat(&Maxpos2.y, XMVectorScale(XMLoadFloat(&Maxpos2.y), 0.1f * (obj2scale.y * 0.1f)));
        XMStoreFloat(&Maxpos2.z, XMVectorScale(XMLoadFloat(&Maxpos2.z), 0.1f * (obj2scale.z * 0.1f)));

        XMStoreFloat(&Minpos2.x, XMVectorScale(XMLoadFloat(&Minpos2.x), 0.1f * (obj2scale.x * 0.1f)));
        XMStoreFloat(&Minpos2.y, XMVectorScale(XMLoadFloat(&Minpos2.y), 0.1f * (obj2scale.y * 0.1f)));
        XMStoreFloat(&Minpos2.z, XMVectorScale(XMLoadFloat(&Minpos2.z), 0.1f * (obj2scale.z * 0.1f)));
    }
    XMFLOAT3 plpos = player->GetPosition();
    XMFLOAT3 obj2pos = obj2->GetPosition();
    {
        DirectX::XMStoreFloat3(&thisobj_min, DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&Minpos1), DirectX::XMLoadFloat3(&plpos)));
        DirectX::XMStoreFloat3(&thisobj_max, DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&Maxpos1), DirectX::XMLoadFloat3(&plpos)));

        DirectX::XMStoreFloat3(&obj_min, DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&Minpos2), DirectX::XMLoadFloat3(&obj2pos)));
        DirectX::XMStoreFloat3(&obj_max, DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&Maxpos2), DirectX::XMLoadFloat3(&obj2pos)));
    }
    if (thisobj_min.x <= obj_max.x && thisobj_max.x >= obj_min.x &&
        thisobj_min.y <= obj_max.y && thisobj_max.y >= obj_min.y &&
        thisobj_min.z <= obj_max.z && thisobj_max.z >= obj_min.z)
    {
        /*
      //  using namespace DirectX;

      //  XMFLOAT3 leftup_mae{};
      //  XMFLOAT3 rightup_mae{};

      //  XMFLOAT3 leftdown_mae{};
      //  XMFLOAT3 rightdown_mae{};

      //  XMFLOAT3 leftup_back{};
      //  XMFLOAT3 rightup_back{};

      //  XMFLOAT3 leftdown_back{};
      //  XMFLOAT3 rightdown_back{};



      //  XMFLOAT3 Maxpos = obj2->GetModel()->bounding_box[max];
      //  XMFLOAT3 Minpos = obj2->GetModel()->bounding_box[min];
      //  XMStoreFloat(&Maxpos.x, XMVectorScale(XMLoadFloat(&Maxpos.x), 0.1f * (obj2scale.x * 0.1f)));
      //  XMStoreFloat(&Maxpos.y, XMVectorScale(XMLoadFloat(&Maxpos.y), 0.1f * (obj2scale.y * 0.1f)));
      //  XMStoreFloat(&Maxpos.z, XMVectorScale(XMLoadFloat(&Maxpos.z), 0.1f * (obj2scale.z * 0.1f)));
      //
      //  XMStoreFloat(&Minpos.x, XMVectorScale(XMLoadFloat(&Minpos.x), 0.1f * (obj2scale.x * 0.1f)));
      //  XMStoreFloat(&Minpos.y, XMVectorScale(XMLoadFloat(&Minpos.y), 0.1f * (obj2scale.y * 0.1f)));
      //  XMStoreFloat(&Minpos.z, XMVectorScale(XMLoadFloat(&Minpos.z), 0.1f * (obj2scale.z * 0.1f)));
      //  XMFLOAT3 LeftUp_Max{ Maxpos.x,Maxpos.y,Maxpos.z };
      //  XMFLOAT3 RightUp_Max{ -Maxpos.x,Maxpos.y,Maxpos.z };

      //  XMFLOAT3 LeftDown_Max{ Maxpos.x,-Maxpos.y,Maxpos.z };
      //  XMFLOAT3 RightDown_Max{ -Maxpos.x,-Maxpos.y,Maxpos.z };

      //  XMFLOAT3 LeftUp_Min{ Minpos.x, Minpos.y,Minpos.z };
      //  XMFLOAT3 RightUp_Min{ -Minpos.x, Minpos.y,Minpos.z };

      //  XMFLOAT3 LeftDown_Min{ Minpos.x,-Minpos.y,Minpos.z };
      //  XMFLOAT3 RightDown_Min{ -Minpos.x,-Minpos.y,Minpos.z };
      //  {
      //      XMVECTOR POS = DirectX::XMLoadFloat3(&obj2pos);
      //      DirectX::XMStoreFloat3(&leftup_mae, DirectX::XMVectorAdd(POS, DirectX::XMLoadFloat3(&LeftUp_Max)));
      //      DirectX::XMStoreFloat3(&rightup_mae, DirectX::XMVectorAdd(POS, DirectX::XMLoadFloat3(&RightUp_Max)));

      //      DirectX::XMStoreFloat3(&leftdown_mae, DirectX::XMVectorAdd(POS, DirectX::XMLoadFloat3(&LeftDown_Max)));
      //      DirectX::XMStoreFloat3(&rightdown_mae, DirectX::XMVectorAdd(POS, DirectX::XMLoadFloat3(&RightDown_Max)));

      //      DirectX::XMStoreFloat3(&leftup_back, DirectX::XMVectorAdd(POS, DirectX::XMLoadFloat3(&LeftUp_Min)));
      //      DirectX::XMStoreFloat3(&rightup_back, DirectX::XMVectorAdd(POS, DirectX::XMLoadFloat3(&RightUp_Min)));

      //      DirectX::XMStoreFloat3(&leftdown_back, DirectX::XMVectorAdd(POS, DirectX::XMLoadFloat3(&LeftDown_Min)));
      //      DirectX::XMStoreFloat3(&rightdown_back, DirectX::XMVectorAdd(POS, DirectX::XMLoadFloat3(&RightDown_Min)));

      //  }
      //  float RightLength;
      //  float LeftLength;
      //  float FrontLength;
      //  float BackLength;
      //std::vector<FaceBoundingBoxCollition> arry;
      //FaceBoundingBoxCollition face_;
      //  //right‚Ì’·‚³
      //  {
      //      XMVECTOR Right1 = XMVectorSubtract(XMLoadFloat3(&rightup_mae)    ,XMLoadFloat3(&plpos));
      //      XMVECTOR Right2 = XMVectorSubtract(XMLoadFloat3(&rightup_back)   ,XMLoadFloat3(&plpos));
      //      XMVECTOR Right3 = XMVectorSubtract(XMLoadFloat3(&rightdown_mae)  ,XMLoadFloat3(&plpos));
      //      XMVECTOR Right4 = XMVectorSubtract(XMLoadFloat3(&rightdown_back) ,XMLoadFloat3(&plpos));
      //      RightLength = XMVectorGetX(XMVector3Length(XMVectorAdd(Right1, XMVectorAdd(Right2, XMVectorAdd(Right3, Right4)))));
      //      face_.length =RightLength;
      //      face_.type   = FaceBoundingBoxCollition::face::Right;
      //      arry.push_back(face_);
      //  }
      //  //left‚Ì’·‚³
      //  {
      //      XMVECTOR Left1 = XMVectorSubtract(XMLoadFloat3(&leftup_mae),    XMLoadFloat3(&plpos));
      //      XMVECTOR Left2 = XMVectorSubtract(XMLoadFloat3(&leftup_back),   XMLoadFloat3(&plpos));
      //      XMVECTOR Left3 = XMVectorSubtract(XMLoadFloat3(&leftdown_mae),  XMLoadFloat3(&plpos));
      //      XMVECTOR Left4 = XMVectorSubtract(XMLoadFloat3(&leftdown_back), XMLoadFloat3(&plpos));
      //      LeftLength = XMVectorGetX(XMVector3Length(XMVectorAdd(Left1, XMVectorAdd(Left2, XMVectorAdd(Left3, Left4)))));
      //      face_.length = LeftLength;
      //      face_.type   =FaceBoundingBoxCollition::face::Left;
      //      arry.push_back(face_);

      //  }
      //  //Front‚Ì’·‚³
      //  {
      //      XMVECTOR Front1 = XMVectorSubtract(XMLoadFloat3(&rightup_mae),   XMLoadFloat3(&plpos));
      //      XMVECTOR Front2 = XMVectorSubtract(XMLoadFloat3(&leftup_mae),    XMLoadFloat3(&plpos));
      //      XMVECTOR Front3 = XMVectorSubtract(XMLoadFloat3(&rightdown_mae), XMLoadFloat3(&plpos));
      //      XMVECTOR Front4 = XMVectorSubtract(XMLoadFloat3(&leftdown_mae),  XMLoadFloat3(&plpos));
      //      FrontLength = XMVectorGetX(XMVector3Length(XMVectorAdd(Front1,   XMVectorAdd(Front2, XMVectorAdd(Front3, Front4)))));
      //      face_.length =FrontLength;
      //      face_.type   = FaceBoundingBoxCollition::face::Front;
      //      arry.push_back(face_);

      //  }
      //  //back‚Ì’·‚³
      //  {
      //      XMVECTOR Back1 = XMVectorSubtract(XMLoadFloat3(&rightup_back),   XMLoadFloat3(&plpos));
      //      XMVECTOR Back2 = XMVectorSubtract(XMLoadFloat3(&leftup_back),    XMLoadFloat3(&plpos));
      //      XMVECTOR Back3 = XMVectorSubtract(XMLoadFloat3(&rightdown_back), XMLoadFloat3(&plpos));
      //      XMVECTOR Back4 = XMVectorSubtract(XMLoadFloat3(&leftdown_back),  XMLoadFloat3(&plpos));
      //      BackLength = XMVectorGetX(XMVector3Length(XMVectorAdd(Back1, XMVectorAdd(Back2, XMVectorAdd(Back3, Back4)))));
      //      face_.length= BackLength;
      //      face_.type  = FaceBoundingBoxCollition::face::Back;
      //      arry.push_back(face_);
      //  }
      //
      //  std::sort(arry.begin(),arry.end(),sort_);
      //  face_check = arry[0].type;
      */
        return true;
    }

    return false;
#endif

}
