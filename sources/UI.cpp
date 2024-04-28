#include "UI.h"
#include"UIManajer.h"
#include"Windows.h"
using namespace std;
using namespace DirectX;
#include"object.h"
#ifdef USE_IMGUI
#include "../imgui/imgui.h"
#include "../imgui/imgui_internal.h"
#include "../imgui/imgui_impl_dx11.h"
#include "../imgui/imgui_impl_win32.h"
#endif
CanBas::CanBas(std::vector<std::unique_ptr<UI>>& uis)
{

    for (int i = 0; i < uis.size(); i++)
    {
        Uis.push_back(move(uis[i]));
    }
   
}

CanBas::~CanBas()
{
    
}

void CanBas::Update(float elapsedTime)
{
    for (auto& ui : Uis)
    {
        ui->SetCanBasPosition(MainPos);
    }
}

void CanBas::Render(RenderContext* rc)
{
    for (auto& ui : Uis)
    {
        ui->Render(rc);
    }
}

void CanBas::Gui()
{
    using namespace ImGui;
    if (ImGui::CollapsingHeader("Canbas", ImGuiTreeNodeFlags_DefaultOpen))
    {
        if (TreeNode("CanbasPos"))
        {
            Gui_parameter_Valu gui_param;
            XMFLOAT3 pos{};
            ImGui::SliderFloat2("maisPos",&pos.x,gui_param.Min.x,gui_param.Max.x);
            MainPos.x += pos.x;
            MainPos.y += pos.y;
            TreePop();
        }
        if (CollapsingHeader("UI_param", ImGuiTreeNodeFlags_DefaultOpen))
        {
            
            for (auto&ui:Uis)
            {
                ui->Gui();
            }
        }
    }
}


UI::UI(ID3D11Device*device,const wchar_t* filename, DirectX::XMFLOAT2 scale, DirectX::XMFLOAT2 pos)
{

    Ui = make_unique<sprite>(device,filename);
    int wlength = wcslen(filename);
    int length = WideCharToMultiByte(
        CP_UTF8,
        0,
        filename,//変換したいワイド文字列のポインター
        wlength,//変換したいワイド文字列の長さ
        nullptr,//変換したワイド文字列を入れるためのchar型のポインター
        0,
        nullptr,
        nullptr);
    string str(length, 0);
    WideCharToMultiByte(CP_UTF8, 0, filename, wlength, &str[0], length, nullptr, nullptr);
    filename_gui = str;
    Scale = scale;
    Pos = pos;
    random_value = rand();
}



void UI::Render(RenderContext* rc)
{
    XMFLOAT3 sp_pos;
    XMVECTOR UC_pos=XMVectorSubtract(XMLoadFloat2(&Pos),XMLoadFloat2(&canbasPos));
    XMStoreFloat3(&sp_pos, UC_pos);
    Ui->render(rc->deviceContext, sp_pos.x, sp_pos.y,Scale.x,Scale.y);
}

void UI::Gui()
{
    using namespace ImGui;
    using namespace DirectX;
    PushID(random_value);
    const int Buffer = 256;
    char* name = const_cast<char*>(filename_gui.c_str());
    InputText("filename", const_cast<char*>(filename_gui.c_str()), Buffer);
    Gui_parameter_Valu gui_param;
    if (CollapsingHeader("Param"))
    {
        if (TreeNode("Position"))
        {
          
            SliderFloat3("Pos.x", &Pos.x, gui_param.Min.x, gui_param.Max.x);
            SliderFloat3("Pos.y", &Pos.y, gui_param.Min.x, gui_param.Max.x);
            
            TreePop();
        }
        if (TreeNode("Scale"))
        {
            SliderFloat3("Scale.x", &Scale.x, gui_param.Min.x, gui_param.Max.x);
            SliderFloat3("Scale.y", &Scale.y, gui_param.Min.x, gui_param.Max.x);
            TreePop();
        }
    }
    if (CollapsingHeader("MoveParam"))
    {
        if (TreeNode("Move_Position"))
        {
            XMFLOAT2 pos_gui{};
            SliderFloat3("Pos_Gui.x", &pos_gui.x, gui_param.Min.x, gui_param.Max.x);
            SliderFloat3("Pos_Gui.y", &pos_gui.y, gui_param.Min.x, gui_param.Max.x);
            Pos.x += pos_gui.x;
            Pos.y += pos_gui.y;
            TreePop();
        }
        if (TreeNode("Move_Scale"))
        {
            XMFLOAT2 scale_gui{};
            SliderFloat3("Scale_Gui.x", &scale_gui.x, gui_param.Min.x, gui_param.Max.x);
            SliderFloat3("Scale_Gui.y", &scale_gui.y, gui_param.Min.x, gui_param.Max.x);
            Scale.x += scale_gui.x;
            Scale.y += scale_gui.y;
            TreePop();
        }
    }
    PopID();
}
