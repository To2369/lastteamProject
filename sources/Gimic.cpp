#include"Gimic.h"
#include"objectManajer.h"
#include"variable_management_class_for_hit_test.h"

void Gimic::Gimic_effect(Gimic_Type type)
{

#if true
    switch (type)
    {
    case Gimic_Type::Switch:
        if (gimic_VS_Object())
        {
            switchFlag = true;
        }
        else switchFlag = false;
        break;
    case Gimic_Type::Door:
        Gimic_VS_GimicFlagBoot();
        if (bootFlag.size() > 0)
        {
            if (bootFlag[0])
            {
                ObjType oldtype = this->Get_Old_Objtype(0);
                this->Set_attribute(ObjType::Super_fragile, 0);
                ObjType newtype = this->Get_Old_Objtype(0);
                if (oldtype == newtype)
                {
                    this->SetReturnTimer();
                }

            }
        }
        break;
    case Gimic_Type::null:
        break;
    default:
        break;
    }
#endif



}

