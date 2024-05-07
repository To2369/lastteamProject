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
