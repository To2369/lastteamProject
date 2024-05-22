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

#include "Graphics/graphics.h"
#include"scene_management.h"
#include"variable_management_class_for_hit_test.h"
CanBas::CanBas(std::vector<std::unique_ptr<UI>>& uis)
{

    for (int i = 0; i < uis.size(); i++)
    {
        Uis.push_back(move(uis[i]));
    }
   // random_value = rand();
}

CanBas::CanBas(std::vector<std::unique_ptr<UI>>& uis, std::string canbasID_)
{
    for (int i = 0; i < uis.size(); i++)
    {
        Uis.push_back(move(uis[i]));
    }
    canbasID = canbasID_;
}

CanBas::~CanBas()
{
    
}

void CanBas::Update(float elapsedTime)
{
    for (auto& ui : Uis)
    {
        ui->SetCanBasPosition(MainPos);
        ui->Update(elapsedTime);
    }
}

void CanBas::Render(Graphics& gr)
{
    for (auto& ui : Uis)
    {
        ui->Render(gr);
    }
}

void CanBas::Render(Graphics&gr, std::string ui_id)
{
    for (auto& ui : Uis)
    {
        if(ui->GetID()==ui_id)
        ui->Render(gr);
    }
}


void CanBas::Gui()
{
    using namespace ImGui;
    //PushID(random_value);
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
        if (TreeNode("param"))
        {
            
            for (auto&ui:Uis)
            {
                ui->Gui();
            }
            ImGui::TreePop();
        }
    }
    if (TreeNode("Mouse"))
    {
        POINT cursor;
        GetCursorPos(&cursor);
        XMFLOAT2 cursorPos{ static_cast<float>(cursor.x),static_cast<float>(cursor.y) };
        InputFloat("mouse.x", &cursorPos.x);
        InputFloat("mouse.y", &cursorPos.y);
    }
    /*
    InputFloat("Window.x", &winPos.x);
    InputFloat("Window.y", &winPos.y);*/
}

void CanBas::ResultInfo()
{
    for (auto& ui : Uis)
    {
        OutputDebugStringA(to_string(ui->GetPosition().x).c_str()); OutputDebugStringA("\n");
        OutputDebugStringA(to_string(ui->GetPosition().x).c_str()); OutputDebugStringA("\n");
        OutputDebugStringA(to_string(ui->GetPosition().x).c_str()); OutputDebugStringA("\n");

    }
}

void CanBas::UIClear()
{
    Uis.clear();
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
    Scale.x = GetTexture2DDesc().Width;
    Scale.y = GetTexture2DDesc().Height;
 
}

UI::UI(ID3D11Device* device, const wchar_t* filename)
{
    Ui = make_unique<sprite>(device, filename);
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
    random_value = rand();
    Scale.x = GetTexture2DDesc().Width;
    Scale.y = GetTexture2DDesc().Height;
}



void UI::Update(float elapsedTime)
{
    if (Ismouse)
    {

        if (Color.w > 1)
            hanten = !hanten;
        if (Color.w < 0.4f)
            hanten = !hanten;

        if (hanten)
            Color.w += Color_alpha*2;
        else
            Color.w -= Color_alpha*2;
       
    }
    else
    {
        if(hanteiflag)
        Color.w = 1;
    }
    Ismouse = false;

}

void UI::Render(Graphics& gr)
{
   
    
    XMVECTOR UC_pos=XMVectorSubtract(XMLoadFloat2(&Pos),XMLoadFloat2(&canbasPos));
   
    XMStoreFloat2(&sp_pos, UC_pos);
    gr.GetDeviceContext()->OMSetBlendState(gr.GetBlendState(1), nullptr, 0xFFFFFFFF);
    Ui->render(gr.GetDeviceContext(), sp_pos.x, sp_pos.y, Scale.x, Scale.y,
        Color.x,Color.y,Color.z,Color.w,
        0);
    gr.GetDeviceContext()->OMSetBlendState(gr.GetBlendState(2), nullptr, 0xFFFFFFFF);
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
          
            SliderFloat("Pos.x", &Pos.x, gui_param.Min.x, gui_param.Max.x);
            SliderFloat("Pos.y", &Pos.y, gui_param.Min.x, gui_param.Max.x);
            
            TreePop();
        }
        if (TreeNode("Scale"))
        {
            SliderFloat("Scale.x", &Scale.x, gui_param.Min.x, gui_param.Max.x);
            SliderFloat("Scale.y", &Scale.y, gui_param.Min.x, gui_param.Max.x);
            TreePop();
        }
        if (TreeNode("Move_Position"))
        {
            XMFLOAT2 pos_gui{};
            SliderFloat("Pos_Gui.x", &pos_gui.x, gui_param.Min.x, gui_param.Max.x);
            SliderFloat("Pos_Gui.y", &pos_gui.y, gui_param.Min.x, gui_param.Max.x);
            Pos.x += pos_gui.x * 3;
            Pos.y += pos_gui.y * 3;
            TreePop();
        }
        if (TreeNode("Move_Scale"))
        {
            XMFLOAT2 scale_gui{};
            SliderFloat("Scale_Gui.x", &scale_gui.x, gui_param.Min.x, gui_param.Max.x);
            SliderFloat("Scale_Gui.y", &scale_gui.y, gui_param.Min.x, gui_param.Max.x);
            Scale.x += scale_gui.x;
            Scale.y += scale_gui.y;
            TreePop();
        }
    }
    InputFloat4("Color",&Color.x);
    PopID();
}
