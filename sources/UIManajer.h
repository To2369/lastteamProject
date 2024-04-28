#pragma once
#include"UI.h"
#include"object.h"
struct CreateUI_Info
{
    const char* filename;
};
class UIManager
{
public:
    ~UIManager()
    {
        ObjectTypes.clear();
    }
    static UIManager& incetance()
    {
        static UIManager ince;
        return ince;
    };
    void CreateUI(ID3D11Device*device);
    void Gui();
    void Update(float elapsedTime);
    void Render(RenderContext* rc);
    ObjType Get_Objetypes(int i) { return ObjectTypes[i]; }
private:
    void GetStage_objectTypes();
    std::vector<ObjType>ObjectTypes;
    std::vector<unique_ptr<CanBas>>CanBass;
};
