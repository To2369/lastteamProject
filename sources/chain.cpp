#include "chain.h"
#include"objectManajer.h"
Gimic* BaseChain::LiftCheck()
{
    Objectmanajer& ince_o = Objectmanajer::incetance();
    int count = ince_o.Get_GameGimicCount();
    for (int i = 0; i < count; i++)
    {
        
        Gimic* obj = ince_o.Get_GameGimic(i);
        if(obj->Get_GimicType() != Gimic_Type::Lift)continue;
        if (obj->GetGimicID() == this->GetID())
        {
            if (obj->GetIsLift() == ObjType::heavy || obj->GetIsLift() == ObjType::Super_heavy)
            {
                this->render->anim.animSpeed = -this->render->anim.animSpeed;
            }
            else if (obj->GetIsLift() == ObjType::cution || obj->GetIsLift() == ObjType::Super_cution)
            {
                this->render->anim.animSpeed = +this->render->anim.animSpeed;
            }
            return obj;
        }
    }
    return nullptr;
}

void BaseChain::ResultInfo()
{
    auto type_map = [](Chain_Type type)
        {
            switch (type)
            {
            case Chain_Type::lift_chain_S:
                return "lift_chain_S";

                break;
            case Chain_Type::lift_chain_L:

                return "lift_chain_L";
                break;
            case Chain_Type::lift_P_Animatio_ndown:

                return "lift_P_Animatio_ndown";
                break;
            case Chain_Type::lift_P_Not_Animation:

                return "lift_P_Not_Animation";
                break;
            case Chain_Type::null:

                return "null";
                break;
           
            }
            return "";
        };

    OutputDebugStringA(type_map(this->MyType));
    OutputDebugStringA("\n");
    OutputDebugStringA("ID:"); OutputDebugStringA(ID.c_str());
    OutputDebugStringA("\n");
    OutputDebugStringA("Position.x: "); OutputDebugStringA(to_string(transform->GetPosition().x).c_str()); OutputDebugStringA(","); OutputDebugStringA("\n");
    OutputDebugStringA("Position.y: "); OutputDebugStringA(to_string(transform->GetPosition().y).c_str()); OutputDebugStringA(","); OutputDebugStringA("\n");
    OutputDebugStringA("Position.z: "); OutputDebugStringA(to_string(transform->GetPosition().z).c_str()); OutputDebugStringA("\n");
    OutputDebugStringA("\n");
    OutputDebugStringA("Scale.x: "); OutputDebugStringA(to_string(transform->Getscale().x).c_str()); OutputDebugStringA(","); OutputDebugStringA("\n");
    OutputDebugStringA("Scale.y: "); OutputDebugStringA(to_string(transform->Getscale().y).c_str()); OutputDebugStringA(","); OutputDebugStringA("\n");
    OutputDebugStringA("Scale.z: "); OutputDebugStringA(to_string(transform->Getscale().z).c_str()); OutputDebugStringA("\n");
    OutputDebugStringA("\n");
    OutputDebugStringA("Angle.x: "); OutputDebugStringA(to_string(transform->GetAngle().x).c_str()); OutputDebugStringA(","); OutputDebugStringA("\n");
    OutputDebugStringA("Angle.y: "); OutputDebugStringA(to_string(transform->GetAngle().y).c_str()); OutputDebugStringA(","); OutputDebugStringA("\n");
    OutputDebugStringA("Angle.z: "); OutputDebugStringA(to_string(transform->GetAngle().z).c_str()); OutputDebugStringA("\n");


    OutputDebugStringA("\n");
}