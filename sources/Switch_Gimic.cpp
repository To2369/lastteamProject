#include"Gimic.h"
#include"objectManajer.h"


Switch::Switch(ID3D11Device* device)
{
    initialaize_Set_attribute(ObjType::null, ObjType::null);
    model = make_unique<Model>(device,filename,true);
    Scale = { 0.05f,0.05f,0.05f };
    Gimic_type = Gimic_Type::Switch;
    MyObjeFlagType = ObjType::heavy;
    Normal = { 0.f,1.f,0.f };
}

void Switch::Update(float elapsedTime)
{
   
    color = InitColor();
    Gimic_effect(Gimic_Type::Switch);
    UpdateTransform();
}

void Switch::Render(RenderContext* rc)
{
    model->render(rc->deviceContext, Transform, 0.0f, color);
    
}

bool Switch::gimic_VS_Object()
{
    //raycastでのギミック起動
#if false
    get_gameobj = ObjType::null;
    VMCFHT& ince_rc = VMCFHT::instance();
    Objectmanajer& ince_ob = Objectmanajer::incetance();

    XMFLOAT3 raynormal = GetNormal();
    XMFLOAT3 pos = Position;

    pos.y += -0.05f;
    ince_rc.update(pos, raynormal);
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

        if (ince.Bounding_Box_vs_Bounding_Box(obj, this, true, 0.045f))
        {
            if (obj->Get_isGimic_UpPosNow())
            {
                if (obj->Get_isGimic_UpPosNow())type.push_back(obj->Get_Old_Objtype(0));
            }
        }
    }

    count = type.size();
    if (count == 0 || count > 1)return false;
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
