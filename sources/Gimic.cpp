#include"Gimic.h"
#include"objectManajer.h"
#include"variable_management_class_for_hit_test.h"
bool Gimic::gimic_VS_Object()
{
    //raycastでのギミック起動
#if false
    get_gameobj = ObjType::null;
   VMCFHT& ince_rc = VMCFHT::instance();
   Objectmanajer& ince_ob = Objectmanajer::incetance();
   
   XMFLOAT3 raynormal = GetNormal();
   XMFLOAT3 pos = Position;
  
   pos.y += -0.05f;
   ince_rc.update(pos,raynormal);
   float Length = 0.5f;
   int count = ince_ob.Get_GameObjCount();
   vector<ObjType>type;
   for (int i = 0; i < count; i++)
   {
       Object* obj = ince_ob.Get_GameObject(i);
       collision_mesh* mesh = obj->GetModel()->Get_RaycastCollition();
        if (ince_rc.raycast(*mesh, obj->GetTransform(), result_intersection, Length))
        {
            type.push_back(obj->Get_attribute());
        }
   }
   count = type.size();
   if (count == 0)return false;
   for (int i = 0; i < count; i++)
   {
       if (MyGimicFlagType != type[i])
       {
           return false;
       }
   }
  
   get_gameobj = MyGimicFlagType;
   type.clear();//
#endif
   Objectmanajer& ince = Objectmanajer::incetance();
   int count = ince.Get_GameObjCount();
   vector<ObjType>type;
   
   for (int i = 0; i < count; i++)
   {
       Object* obj = ince.Get_GameObject(i);

       if (ince.Bounding_Box_vs_Bounding_Box(obj,this,true,0.045f))
       {
           if (obj->Get_isGimic_UpPosNow())
           {
               if (obj->Get_isGimic_UpPosNow())type.push_back(obj->Get_attribute(0));
           }
       }
   }

   count = type.size();
   if (count == 0||count>1)return false;
   for (int i = 0; i < count; i++)
   {
       if (MyObjeFlagType != type[i])
       {
           return false;
       }
   }
 
   type.clear();
  return true;
}

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
        break;
    case Gimic_Type::Door:
        Gimic_VS_GimicFlagBoot();
        if (bootFlag.size() > 0)
        {
            if (bootFlag[0])
            {
                this->Set_attribute(ObjType::Super_fragile, 0);
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

void Gimic::Gimic_VS_GimicFlagBoot()
{
    bootFlag.clear();
    Objectmanajer& ince = Objectmanajer::incetance();
    int count = ince.Get_GameGimicCount();
    for (int i = 0; i < count;i++)
    {
        Gimic* gimic = ince.Get_GameGimic(i);
        if (this->Get_GimicType() == gimic->Get_GimicType())continue;
        if (gimic->Get_GimicType() == this->GetMyGimicFlagType() && gimic->Get_SwitchFlag())
        {
            this->SetBootFlag(true);
        }
    }
}
