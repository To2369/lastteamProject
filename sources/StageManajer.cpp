#include "StageManager.h"
#include"variable_management_class_for_hit_test.h"

#include "Cution.h"
#include"Heavy.h"
#include"Fragile.h"
#include"Hard_to_Break.h"
#include"Crack.h"
#include"camera.h"

#ifdef USE_IMGUI
#include "../imgui/imgui.h"
#include "../imgui/imgui_internal.h"
#include "../imgui/imgui_impl_dx11.h"
#include "../imgui/imgui_impl_win32.h"
#endif
#include"objectManajer.h"
#include"UIManajer.h"
void StageManager::Initialize_GameStage(StageName name, ID3D11Device* device)
{
	using namespace std;
	Objectmanajer& obj_Manager = Objectmanajer::incetance();
	UIManager& ince_UI = UIManager::incetance();
	XMFLOAT3 pos{};
	switch (name)
	{
	case StageName::Tutolial:
		break;
	case StageName::stage1_1:

		obj_Manager.Initialize(name, ObjType::heavy, device, {
			
			  -1.037836f,
			   23.747557f,
			  -0.430794f
			});
		obj_Manager.Initialize(name, ObjType::cution, device, {
			 -1.336630f,
			 23.747557f,
			  0.460227f
			});
		obj_Manager.Initialize(name, Gimic_Type::Switch, device, {
		-2.010655f,
		22.640562f,
		-0.632938f
			}, "kabe");
		obj_Manager.Initialize(name, Gimic_Type::Goal, device, {
		-0.970918,
		22.747557,
		-1.307508
			});

	

	
		{
			unique_ptr<Static_Object>obj = make_unique<stage_OBJ>(device);
			obj_Manager.Initialize_Static_Object(move(obj));
			
		}
		{

			obj_Manager.Initialize_InvisibleBarria(device,
				{
			    	0.762000,
			    	23.473000,
			    	-1.133001 
				},
				{ 

				    305.402313f,
				    198.938995f,
				    164.204910f
				});
			obj_Manager.Initialize_InvisibleBarria(device, 
				{
					-1.001000f,
					22.841f,
					-0.772001f
				
				},
				{
					50.922291f,
					58.687035f,
					28.108971f
				});
			obj_Manager.Initialize_InvisibleBarria(device,
				{
					-2.001124,
					22.949366,
				    -0.818629
				},
				{
				    50.922291f,
				    58.687035f,
				    27.466879f
				});
			obj_Manager.Initialize_InvisibleBarria(device,
				{
					-0.810468,
					22.382496,
					0.076248
				},
				{
				    -27.452732f,
				     57.414883f,
				     116.421349f
				});
		}
		{

			unique_ptr<Stage>stage = make_unique<Stage_1_1>(device);
			stage->SetPosition({ 0.f, 0.5f, -0.5f });
			Rigister(move(stage));

			unique_ptr<Stage>hokyou_yuka = make_unique<stage_Yuka>(device);
			Rigister(move(hokyou_yuka));
			
		}
		obj_Manager.Initialize(name, Gimic_Type::Door, device, {
		 -1.515878f,
		 23.275631f,
		 -0.925332f
			}, "kabe");
		ince_UI.CreateUI(device);
		break;
	case StageName::null:
		break;
	default:
		break;
	}




}
void StageManager::Update(float elapsedTime)
{
	const int stage_count = static_cast<int>(Stages.size());
	for (int i = 0; i < stage_count; i++)
	{
		Stages[i]->Update(elapsedTime);
	}
}

void StageManager::Render(RenderContext* rc)
{
	const int stage_count = static_cast<int>(Stages.size());
	for (int i = 0; i < stage_count; i++)
	{
		Stages[i]->Render(rc);
	}
}

void StageManager::Clear()
{
	{
		Objectmanajer& ince = Objectmanajer::incetance();
		int count = static_cast<int>(ince.Get_GameObjCount());
		OutputDebugStringA("/////////////OBJECT_INFO///////////////"); OutputDebugStringA("\n"); OutputDebugStringA("\n");
		for (int i = 0; i < count; i++)
		{
			Object* obj = ince.Get_GameObject(i);
			Result_Object_Info(*obj);
		}
		count = ince.Get_GameGimicCount();
		OutputDebugStringA("/////////////GIMIC_INFO///////////////"); OutputDebugStringA("\n"); OutputDebugStringA("\n");
		for (int i = 0; i < count; i++)
		{
			Gimic* gimic = ince.Get_GameGimic(i);
			Result_Gimic_Info(*gimic);
		}
		OutputDebugStringA("/////////////STATIC_OBJECT_INFO///////////////"); OutputDebugStringA("\n"); OutputDebugStringA("\n");
		count = ince.Get_GameStatic_ObjectCount();
		for (int i = 0; i < count; i++)
		{
			Static_Object* obj = ince.Get_GameStatic_Object(i);
			Result_Static_Object_Info(*obj);
		}
	}



	Stages.clear();
}

void StageManager::Result_Object_Info(Object& obj)
{

	auto typeMap = [](ObjType type) {
		switch (type)
		{
		case ObjType::cution:
			return "cushion";
			break;
		case ObjType::Super_cution:
			return "Super_cution";
			break;
		case ObjType::Hard_to_Break:
			return "Hard_to_Break";
			break;
		case ObjType::Super_hard_to_Break:
			return "Super_hard_to_Break";
			break;
		case ObjType::heavy:
			return "heavy";
			break;
		case ObjType::Super_heavy:
			return "Super_heavy";
			break;
		case ObjType::Fragile:
			return "Fragile";
			break;
		case ObjType::Super_fragile:
			return "Super_fragile";
			break;
		case ObjType::null:
			return "null";
		case ObjType::Crack:
			return"Crack";
			break;

		}
		return "";
		};

	string s = typeMap(obj.Get_Old_Objtype(0));
	string pos;
	string scale;
	OutputDebugStringA(s.c_str());
	OutputDebugStringA("\n");
	OutputDebugStringA("Position.x: "); OutputDebugStringA(to_string(obj.GetPosition().x).c_str()); OutputDebugStringA(","); OutputDebugStringA("\n");
	OutputDebugStringA("Position.y: "); OutputDebugStringA(to_string(obj.GetPosition().y).c_str()); OutputDebugStringA(","); OutputDebugStringA("\n");
	OutputDebugStringA("Position.z: "); OutputDebugStringA(to_string(obj.GetPosition().z).c_str()); OutputDebugStringA("\n");
	OutputDebugStringA("\n");
	OutputDebugStringA("Scale.x: "); OutputDebugStringA(to_string(obj.GetScale().x).c_str()); OutputDebugStringA("\n");
	OutputDebugStringA("Scale.y: "); OutputDebugStringA(to_string(obj.GetScale().y).c_str()); OutputDebugStringA("\n");
	OutputDebugStringA("Scale.z: "); OutputDebugStringA(to_string(obj.GetScale().z).c_str()); OutputDebugStringA("\n");


	OutputDebugStringA("\n");

}

void StageManager::Result_Gimic_Info(Gimic& obj)
{

	auto typeMap = [](Gimic_Type type) {
		switch (type)
		{
		case Gimic_Type::Switch:
			return "Switch";
			break;
		case Gimic_Type::Door:
			return "Door";
			break;
		case Gimic_Type::Goal:
			return "Goal";
			break;
		case Gimic_Type::Drop_Road:
			return "Drop_Road";
			break;
		case Gimic_Type::null:
			return "null";
			break;
		default:
			break;
		}
		return "";
		};

	string s = typeMap(obj.Get_GimicType());
	string pos;
	string scale;
	OutputDebugStringA(s.c_str());
	OutputDebugStringA("\n");
	OutputDebugStringA("Position.x: "); OutputDebugStringA(to_string(obj.GetPosition().x).c_str()); OutputDebugStringA(","); OutputDebugStringA("\n");
	OutputDebugStringA("Position.y: "); OutputDebugStringA(to_string(obj.GetPosition().y).c_str()); OutputDebugStringA(","); OutputDebugStringA("\n");
	OutputDebugStringA("Position.z: "); OutputDebugStringA(to_string(obj.GetPosition().z).c_str()); OutputDebugStringA("\n");
	OutputDebugStringA("\n");
	OutputDebugStringA("Scale.x: "); OutputDebugStringA(to_string(obj.GetScale().x).c_str()); OutputDebugStringA("\n");
	OutputDebugStringA("Scale.y: "); OutputDebugStringA(to_string(obj.GetScale().y).c_str()); OutputDebugStringA("\n");
	OutputDebugStringA("Scale.z: "); OutputDebugStringA(to_string(obj.GetScale().z).c_str()); OutputDebugStringA("\n");


	OutputDebugStringA("\n");
}

void StageManager::Result_Static_Object_Info(Static_Object& obj)
{


	auto typeMap = [](Static_ObjType type) {

		switch (type)
		{
		case Static_Object_name::Static_Object:
			return "Static_Object";
			break;
		case Static_Object_name::Invisible_Wall:
			return"Invisible_Wall";
			break;
		}
		return "";
		};

	string s = typeMap(obj.GetStatic_ObjType());
	string pos;
	string scale;
	OutputDebugStringA(s.c_str());
	OutputDebugStringA("\n");
	OutputDebugStringA("Position.x: "); OutputDebugStringA(to_string(obj.GetPosition().x).c_str()); OutputDebugStringA(","); OutputDebugStringA("\n");
	OutputDebugStringA("Position.y: "); OutputDebugStringA(to_string(obj.GetPosition().y).c_str()); OutputDebugStringA(","); OutputDebugStringA("\n");
	OutputDebugStringA("Position.z: "); OutputDebugStringA(to_string(obj.GetPosition().z).c_str()); OutputDebugStringA("\n");
	OutputDebugStringA("\n");
	OutputDebugStringA("Scale.x: "); OutputDebugStringA(to_string(obj.GetScale().x).c_str()); OutputDebugStringA("\n");
	OutputDebugStringA("Scale.y: "); OutputDebugStringA(to_string(obj.GetScale().y).c_str()); OutputDebugStringA("\n");
	OutputDebugStringA("Scale.z: "); OutputDebugStringA(to_string(obj.GetScale().z).c_str()); OutputDebugStringA("\n");


	OutputDebugStringA("\n");
}

void StageManager::Gui(ID3D11Device* device, RenderContext* rc)
{
	{
		rc->view = Camera::instance().GetView();
		rc->projection = Camera::instance().GetProjection();
		rc->lightDirection = { 0.0f,-0.5f,0.5f,0.0f };

		DirectX::XMMATRIX VP = DirectX::XMMatrixMultiply(DirectX::XMLoadFloat4x4(&rc->view), DirectX::XMLoadFloat4x4(&rc->projection));
		DirectX::XMStoreFloat4x4(&scene_data.view_projection, VP);

		Camera& camera = Camera::instance();


		scene_data.light_direction.x = rc->lightDirection.x;
		scene_data.light_direction.y = rc->lightDirection.y;
		scene_data.light_direction.z = rc->lightDirection.z;

		scene_data.light_direction.w = 0.0f;

		scene_data.camera_position.x = camera.GetEye().x;
		scene_data.camera_position.y = camera.GetEye().y;
		scene_data.camera_position.z = camera.GetEye().z;
		scene_data.camera_position.w = 1.0f;

		DirectX::XMStoreFloat4x4(&scene_data.view_projection, VP);
	}

#if USE_IMGUI
	ImGui::Begin("Stage_SetUp");
	auto m = [](DebugMode mode)
		{
			switch (mode)
			{
			case DebugMode::Object_Info:
				return "Object_Info";
				break;
			case DebugMode::StageSetUp:
				return "StageSetUp";
				break;
			case DebugMode::Create_Object:
				return "Create_Object";
				break;
			case DebugMode::Delete_Object:
				return "Delete_object";
				break;
			case DebugMode::null:
				return "null";
				break;

			}
			return "";
		};
	auto o = [](ObjType type) {


		switch (type)
		{
		case ObjType::cution:
			return "cushion";
			break;
		case ObjType::Super_cution:
			return "Super_cution";
			break;
		case ObjType::Hard_to_Break:
			return "Hard_to_Break";
			break;
		case ObjType::Super_hard_to_Break:
			return "Super_hard_to_Break";
			break;
		case ObjType::heavy:
			return "heavy";
			break;
		case ObjType::Super_heavy:
			return "Super_heavy";
			break;
		case ObjType::Fragile:
			return "Fragile";
			break;
		case ObjType::Super_fragile:
			return "Super_fragile";
			break;
		case ObjType::Crack:
			return "Crack";
			break;
		case ObjType::null:
			return "null";
			break;

		}
		return "";
		};
	auto g = [](Gimic_Type type) {

		switch (type)
		{
		case Gimic_Type::Switch:
			return "switch";
			break;
		case Gimic_Type::Door:
			return "Door";
			break;
		case Gimic_Type::Goal:
			return "Goal";
			break;
		case Gimic_Type::Drop_Road:
			return "Drop_Load";
			break;
		case Gimic_Type::null:
			return "null";
			break;
		default:
			break;
		}
		return"";
		};
	auto sc = [](Static_ObjType type)
		{
			switch (type)
			{
			case Static_ObjType::Static_Object:
				return "Static_Object";
				break;
			case Static_ObjType::Invisible_Wall:
				return "Invisible_Wall";
				break;
			}
			return "";
		};
	ImGui::Text("Debug_Mode : %s", m(Getmode()));
	if (ImGui::CollapsingHeader("SelectMode", ImGuiTreeNodeFlags_DefaultOpen))
	{

		if (ImGui::Button("Change_StageSetUp")) mode = DebugMode::StageSetUp;//stageのobjectをmouseのraycastを使用して位置を設定できるように切り替える
		if (ImGui::Button("Change_Object_Info")) mode = DebugMode::Object_Info;//object情報をmouseのraycastで確認できる用に切り替える
		if (ImGui::Button("Change_Create_Object")) mode = DebugMode::Create_Object;//objectを作る
		if (ImGui::Button("Change_Delete_Object")) mode = DebugMode::Delete_Object;//objectをmouseで選択して消す
		if (oldMode != mode)
		{
			oldMode = mode;
			o_or_g = debugType::null;
		}
	}
	if (mode == DebugMode::Object_Info)
	{
		if (o_or_g == debugType::null)
		{
			if (ImGui::Button("Object"))o_or_g = debugType::obj;
			if (ImGui::Button("Gimic"))o_or_g = debugType::gimic;
			if (ImGui::Button("Static_object"))o_or_g = debugType::static_obj;
		}
		else
		{
			if (ImGui::Button("Cancel"))
			{
				o_or_g = debugType::null;
				Debug_ParameterObj = nullptr;
			}
		}
		DebugMode_MouseRayCast(mode, device);
	}
	else if (mode == DebugMode::StageSetUp)
	{
		if (o_or_g == debugType::null)
		{
			if (ImGui::Button("Object"))o_or_g = debugType::obj;
			if (ImGui::Button("Gimic"))o_or_g = debugType::gimic;
		}
		else
		{
			if (ImGui::Button("Cancel"))o_or_g = debugType::null;
		}
		DebugMode_MouseRayCast(mode, device);
	}
	else if (mode == DebugMode::Create_Object)
	{
		auto p = [](debugType type) {


			switch (type)
			{
			case debugType::obj:
				return"Obj";
				break;
			case debugType::gimic:
				return"Gimic";
				break;
			case debugType::static_obj:
				return"static_obj";
				break;
			case debugType::null:
				return"null";
				break;
			default:
				break;
			}
			return "";
			};
		ImGui::Text("Select_CreateObject:%s", p(o_or_g));

		if (o_or_g == debugType::null)
		{
			if (ImGui::Button("Object"))o_or_g = debugType::obj;
			if (ImGui::Button("Gimic"))o_or_g = debugType::gimic;
			if (ImGui::Button("static_object"))o_or_g = debugType::static_obj;
		}
		else
		{
			if (ImGui::Button("Cancel"))
			{
				CreateObjeType = ObjType::null;
				o_or_g = debugType::null;
			}
		}
		if (o_or_g == debugType::obj)
		{
			if (ImGui::CollapsingHeader("SelectType", ImGuiTreeNodeFlags_DefaultOpen))
			{
				if (CreateObjeType == ObjType::null)
				{
					if (ImGui::Button("heavy")) CreateObjeType = ObjType::heavy;
					if (ImGui::Button("Super_heavy")) CreateObjeType = ObjType::Super_heavy;
					if (ImGui::Button("Cution")) CreateObjeType = ObjType::cution;
					if (ImGui::Button("Super_cution")) CreateObjeType = ObjType::Super_cution;
					if (ImGui::Button("Hard_to_Break")) CreateObjeType = ObjType::Hard_to_Break;
					if (ImGui::Button("Super_hard_to_Break")) CreateObjeType = ObjType::Super_hard_to_Break;
					if (ImGui::Button("Fragile")) CreateObjeType = ObjType::Fragile;
					if (ImGui::Button("Super_fragile")) CreateObjeType = ObjType::Super_fragile;
					if (ImGui::Button("Crack")) CreateObjeType = ObjType::Crack;
				}
			}
			if (CreateGimicType != Gimic_Type::null && CreateObjeType != ObjType::null)
			{
				CreateObjeType = ObjType::null;
			}
		}
		else if (o_or_g == debugType::gimic)
		{
			if (ImGui::CollapsingHeader("SelectType", ImGuiTreeNodeFlags_DefaultOpen))
			{
				if (CreateGimicType == Gimic_Type::null)
				{
					if (ImGui::Button("Switch")) CreateGimicType = Gimic_Type::Switch;
					if (ImGui::Button("Door")) CreateGimicType = Gimic_Type::Door;
					if (ImGui::Button("Goal")) CreateGimicType = Gimic_Type::Goal;
					if (ImGui::Button("Drop_Load")) CreateGimicType = Gimic_Type::Drop_Road;

				}
			}
			if (CreateGimicType != Gimic_Type::null && CreateObjeType != ObjType::null && CreateStaticObjeType != Static_ObjType::null)
			{
				CreateGimicType = Gimic_Type::null;
			}
		}
		else if (o_or_g == debugType::static_obj)
		{
			if (CreateStaticObjeType == Static_ObjType::null)
			{
				if (ImGui::Button("Static_Object")) CreateStaticObjeType = Static_ObjType::Static_Object;
				if (ImGui::Button("Invisible_Wall")) CreateStaticObjeType = Static_ObjType::Invisible_Wall;


			}
			if (CreateGimicType != Gimic_Type::null && CreateObjeType != ObjType::null && CreateStaticObjeType != Static_ObjType::null)
			{
				CreateStaticObjeType = Static_ObjType::null;
			}

		}
		if (CreateObjeType != ObjType::null)
		{
			if (ImGui::Button("NULL")) CreateObjeType = ObjType::null;
			ImGui::Checkbox("CreateFlag", &Object_CreateFlag);

			if (ImGui::CollapsingHeader("NewCreateObj", ImGuiTreeNodeFlags_DefaultOpen))
			{
				ImGui::Text("CreateObjType:%s", o(CreateObjeType));
				DebugMode_MouseRayCast(mode, device);

			}
		}
		if (CreateGimicType != Gimic_Type::null)
		{
			if (ImGui::Button("NULL")) CreateGimicType = Gimic_Type::null;
			// テキスト入力フィールドを表示
			const int Buffer = 256;
			ImGui::InputText("CreateID", const_cast<char*>(ID.c_str()), Buffer);
			ImGui::Checkbox("CreateFlag", &Object_CreateFlag);

			if (ImGui::CollapsingHeader("NewCreateObj", ImGuiTreeNodeFlags_DefaultOpen))
			{
				ImGui::Text("CreateObjType:%s", g(CreateGimicType));
				DebugMode_MouseRayCast(mode, device);

			}

		}
		if (CreateStaticObjeType != Static_ObjType::null)
		{

			if (ImGui::Button("NULL")) CreateStaticObjeType = Static_ObjType::null;
			ImGui::Checkbox("CreateFlag", &Object_CreateFlag);
			if (CreateStaticObjeType == Static_ObjType::Static_Object)
			{
				// テキスト入力フィールドを表示
				const int Buffer = 256;
				ImGui::InputInt("filenameIndex", &filenameIndex);
				if (!filenames[filenameIndex])
				{
					auto num = [](int& n, const char* filename[])
						{

							while (!filename[n])
							{
								--n;
							};
						};
					num(filenameIndex, filenames);
				}
				else if (filenameIndex < 0)
				{
					filenameIndex = 0;
				}

				static_objFilename = filenames[filenameIndex];
				ImGui::InputText("CreateID", const_cast<char*>(static_objFilename.c_str()), Buffer);
			}
			if (ImGui::CollapsingHeader("NewCreateObj", ImGuiTreeNodeFlags_DefaultOpen))
			{
				ImGui::Text("CreateObjType:%s", sc(CreateStaticObjeType));
				DebugMode_MouseRayCast(mode, device);

			}

		}
	}
	else if (mode == DebugMode::Delete_Object)
	{
		if (o_or_g == debugType::null)
		{
			if (ImGui::Button("Object"))o_or_g = debugType::obj;
			if (ImGui::Button("Gimic"))o_or_g = debugType::gimic;
			if (ImGui::Button("Static_Object"))o_or_g = debugType::static_obj;
		}
		else
		{
			if (ImGui::Button("Cancel"))o_or_g = debugType::null;
		}
		DebugMode_MouseRayCast(mode, device);
	}
	ImGui::End();

#endif
}


void StageManager::CreateObject(ObjType type, ID3D11Device* device, Intersection in)
{
	Objectmanajer& ince = Objectmanajer::incetance();

	unique_ptr<Object>obj;
	switch (type)
	{
	case ObjType::cution:
		obj = make_unique<Cution>(device);
		obj->SetPosition(in.intersection_position);
		ince.Rigister_obj(move(obj));
		break;
	case ObjType::Super_cution:
		obj = make_unique<Super_Cution>(device);
		obj->SetPosition(in.intersection_position);
		ince.Rigister_obj(move(obj));
		break;
	case ObjType::heavy:
		obj = make_unique<Heavy>(device);
		obj->SetPosition(in.intersection_position);
		ince.Rigister_obj(move(obj));
		break;
	case ObjType::Super_heavy:
		obj = make_unique<Super_Heavy>(device);
		obj->SetPosition(in.intersection_position);
		ince.Rigister_obj(move(obj));

		break;
	case ObjType::Fragile:
		obj = make_unique<Fragile>(device);
		obj->SetPosition(in.intersection_position);
		ince.Rigister_obj(move(obj));

		break;
	case ObjType::Super_fragile:
		obj = make_unique<Super_fragile>(device);
		obj->SetPosition(in.intersection_position);
		ince.Rigister_obj(move(obj));

		break;
	case ObjType::Hard_to_Break:
		obj = make_unique<Hard_to_Break>(device);
		obj->SetPosition(in.intersection_position);
		ince.Rigister_obj(move(obj));

		break;
	case ObjType::Super_hard_to_Break:
		obj = make_unique<Super_hard_to_Break>(device);
		obj->SetPosition(in.intersection_position);
		ince.Rigister_obj(move(obj));

		break;
	case ObjType::Crack:
		obj = make_unique<Crack>(device);
		obj->SetPosition(in.intersection_position);
		ince.Rigister_obj(move(obj));

		break;
	case ObjType::null:
		break;
	default:
		break;
	}

}

void StageManager::CreateGimic(Gimic_Type type, ID3D11Device* device, Intersection in, std::string id)
{
	Objectmanajer& ince = Objectmanajer::incetance();

	unique_ptr<Gimic>obj;
	switch (type)
	{
	case Gimic_Type::Switch:
		obj = make_unique<Switch>(device);
		obj->SetPosition(in.intersection_position);
		obj->SetGimicID(id);
		ince.Rigister_Gimic(move(obj));
		break;
	case Gimic_Type::Door:
		obj = make_unique<Door>(device);
		obj->SetPosition(in.intersection_position);
		obj->SetGimicID(id);
		ince.Rigister_Gimic(move(obj));
		break;
	case Gimic_Type::Goal:
		obj = make_unique<Goal>(device);
		obj->SetPosition(in.intersection_position);
		obj->SetGimicID(id);
		ince.Rigister_Gimic(move(obj));
		break;
	case Gimic_Type::Drop_Road:
		obj = make_unique<DropBox_Road>(device);
		obj->SetPosition(in.intersection_position);
		obj->SetGimicID(id);
		ince.Rigister_Gimic(move(obj));
		break;
	case Gimic_Type::null:

		break;
	}

}

void StageManager::CreateStaticObject(Static_ObjType type, ID3D11Device* device, Intersection in, const char* filename)
{
	Objectmanajer& ince = Objectmanajer::incetance();
	unique_ptr<Static_Object>obj;
	switch (type)
	{
	case Static_ObjType::Static_Object:
		obj = make_unique<stage_OBJ>(device, filename);
		obj->SetPosition(in.intersection_position);
		ince.Rigister_Static_Object(move(obj));
		break;
	case Static_ObjType::Invisible_Wall:
		obj = make_unique<InvisibleBarrier>(device);
		obj->SetPosition(in.intersection_position);
		ince.Rigister_Static_Object(move(obj));
	case Static_ObjType::null:
		break;
	}
}

void StageManager::DebugMode_MouseRayCast(DebugMode mode, ID3D11Device* device)
{


	if (mode == DebugMode::Object_Info)
	{
		//右クリック
		Object_Info();
	}
	else if (mode == DebugMode::StageSetUp)
	{
		//右クリック
		StageSetup();
	}
	else if (mode == DebugMode::Create_Object)
	{
		//右クリック
		Create_Object(device);
	}

	else if (mode == DebugMode::Delete_Object)
	{
		//右クリック
		Delete_Object();
	}

}

void StageManager::Object_Info()
{

	switch (o_or_g)
	{
	case debugType::obj:
		if (GetAsyncKeyState(VK_RBUTTON) & 1)
		{
			VMCFHT::instance().update(scene_data.view_projection, scene_data.camera_position);

			result_intersection = {};
			int gameobject_count = Objectmanajer::incetance().Get_GameObjCount();
			for (int i = 0; i < gameobject_count; i++)
			{
				Objectmanajer& ince_obj = Objectmanajer::incetance();
				Object* obj = ince_obj.Get_GameObject(i);

				if (VMCFHT::instance().raycast(*obj->GetModel()->Get_RaycastCollition(), obj->GetTransform(), result_intersection))
				{
					Debug_ParameterObj = obj;
					break;
				}
				else
				{
					Debug_ParameterObj = nullptr;

				}
			}

		}
		{

			auto s = [](string s1, int s2, bool stringFlag = false)
				{
					if (stringFlag)return s1 += to_string(s2) += ":%s";
					return s1 += to_string(s2) += ":";
				};
			if (Debug_ParameterObj && !Debug_ParameterObj->GetDestroyObje())
			{
				ImGui::SliderFloat("Obgect_Camera_Length:", &objLength, 0.3f, 5.f);
				const int ii = 2;
				for (int i = 0; i < ii; i++)
				{
					string name = GetObjectType_S(Debug_ParameterObj->Get_Old_Objtype(i));
					ImGui::Text(s("ObjectType", i, true).c_str(), name.c_str());
					float t = Debug_ParameterObj->GetReturnTimer(i);

					ImGui::InputFloat(s("returnTimer", i).c_str(), &t);
				}
				Debug_ParameterObj->Gui();
				if (ImGui::CollapsingHeader("Change_ObjectType", ImGuiTreeNodeFlags_DefaultOpen))
				{
					enum class Num
					{
						num0,
						num1,
						null
					};

					static Num num = Num::null;
					if (ImGui::Button("heavy")) SetObjType = ObjType::heavy;
					if (ImGui::Button("Super_heavy")) SetObjType = ObjType::Super_heavy;
					if (ImGui::Button("Cution")) SetObjType = ObjType::cution;
					if (ImGui::Button("Super_cution")) SetObjType = ObjType::Super_cution;
					if (ImGui::Button("Hard_to_Break")) SetObjType = ObjType::Hard_to_Break;
					if (ImGui::Button("Super_hard_to_Break")) SetObjType = ObjType::Super_hard_to_Break;
					if (ImGui::Button("Fragile")) SetObjType = ObjType::Fragile;
					if (ImGui::Button("Super_fragile")) SetObjType = ObjType::Super_fragile;
					if (ImGui::Button("Crack")) SetObjType = ObjType::Crack;
					if (SetObjType != ObjType::null)
					{
						if (ImGui::Button("SetTypeNumber0"))num = Num::num0;
						if (ImGui::Button("SetTypeNumber1"))num = Num::num1;
						if (num != Num::null)
						{
							Debug_ParameterObj->Set_attribute(SetObjType, static_cast<int>(num));
							SetObjType = ObjType::null;
							num = Num::null;
						}
					}

				}

			}
		}
		break;
	case debugType::gimic:
		if (GetAsyncKeyState(VK_RBUTTON) & 1)
		{
			VMCFHT::instance().update(scene_data.view_projection, scene_data.camera_position);

			result_intersection = {};
			int gamegimic_count = Objectmanajer::incetance().Get_GameGimicCount();
			for (int i = 0; i < gamegimic_count; i++)
			{
				Objectmanajer& ince_obj = Objectmanajer::incetance();
				Gimic* obj = ince_obj.Get_GameGimic(i);

				if (VMCFHT::instance().raycast(*obj->GetModel()->Get_RaycastCollition(), obj->GetTransform(), result_intersection))
				{
					Debug_ParameterObj = obj;
					break;
				}
				else
				{
					Debug_ParameterObj = nullptr;

				}
			}
		}

		{

			auto s = [](string s1, int s2, bool stringFlag = false)
				{
					if (stringFlag)return s1 += to_string(s2) += ":%s";
					return s1 += to_string(s2) += ":";
				};
			if (Debug_ParameterObj && !Debug_ParameterObj->GetDestroyObje())
			{
				ImGui::SliderFloat("Obgect_Camera_Length:", &objLength, 1.3f, 5.f);
				const int ii = 2;
				for (int i = 0; i < ii; i++)
				{
					string name = GetObjectType_S(Debug_ParameterObj->Get_Old_Objtype(i));
					ImGui::Text(s("ObjectType", i, true).c_str(), name.c_str());
					float t = Debug_ParameterObj->GetReturnTimer(i);

					ImGui::InputFloat(s("returnTimer", i).c_str(), &t);
				}
				Debug_ParameterObj->Gui();
			}
		}
		break;
	case debugType::static_obj:
		if (GetAsyncKeyState(VK_RBUTTON) & 1)
		{
			VMCFHT::instance().update(scene_data.view_projection, scene_data.camera_position);

			result_intersection = {};
			int gamestaticobject_count = Objectmanajer::incetance().Get_GameStatic_ObjectCount();
			for (int i = 0; i < gamestaticobject_count; i++)
			{
				Objectmanajer& ince_obj = Objectmanajer::incetance();
				Static_Object* obj = ince_obj.Get_GameStatic_Object(i);

				if (VMCFHT::instance().raycast(*obj->GetModel()->Get_RaycastCollition(), obj->GetTransform(), result_intersection))
				{
					Debug_ParameterObj = obj;
					break;
				}
				else
				{
					Debug_ParameterObj = nullptr;

				}
			}
		}

		{


			if (Debug_ParameterObj && !Debug_ParameterObj->GetDestroyObje())
			{
				Debug_ParameterObj->Gui();
			}
		}

	}



}

void StageManager::StageSetup()
{

	VMCFHT::instance().update(scene_data.view_projection, scene_data.camera_position);

	result_intersection = {};
	int gameobject_count = Objectmanajer::incetance().Get_GameObjCount();
	int gamegimic_count = Objectmanajer::incetance().Get_GameGimicCount();

	switch (o_or_g)
	{

	case debugType::obj:
		for (int i = 0; i < gameobject_count; i++)
		{
			Objectmanajer& ince_obj = Objectmanajer::incetance();
			StageManager& ince = StageManager::incetance();
			int stageCount = ince.GetStageCount();
			for (int j = 0; j < stageCount; j++)
			{
				Object* stage_ = ince.GetStages(j);
				Object* obj = ince_obj.Get_GameObject(i);
				if (VMCFHT::instance().raycast(*obj->GetModel()->Get_RaycastCollition(), obj->GetTransform(), result_intersection))
				{
					if (GetAsyncKeyState(VK_RBUTTON) & 1)
					{
						Debug_ParameterObj = obj;
					}
					break;
				}
				else if (Debug_ParameterObj)
				{
					if (VMCFHT::instance().raycast(*stage_->GetModel()->Get_RaycastCollition(), stage_->GetTransform(), result_intersection))
					{
						result_intersection.intersection_position.y = 0.750f;
						Debug_ParameterObj->SetPosition(result_intersection.intersection_position);
						if (GetAsyncKeyState(VK_RBUTTON) & 1)
						{
							Debug_ParameterObj = nullptr;
						}
					}
					else
					{
						XMFLOAT3 cameraeye{ Camera::instance().GetEye() };
						XMFLOAT3 camerafront{ Camera::instance().GetFront() };
						cameraeye.z += camerafront.z * objLength;
						cameraeye.x += camerafront.x * objLength;
						cameraeye.y += camerafront.y * objLength;

						Debug_ParameterObj->SetPosition(cameraeye);
						if (GetAsyncKeyState(VK_RBUTTON) & 1)
						{
							Debug_ParameterObj = nullptr;
						}


					}
					break;
				}
			}
		}
		break;
	case debugType::gimic:
		for (int i = 0; i < gamegimic_count; i++)
		{
			Objectmanajer& ince_obj = Objectmanajer::incetance();
			StageManager& ince = StageManager::incetance();
			Object* obj_gimic = ince_obj.Get_GameGimic(i);
			int stageCount = ince.GetStageCount();
			for (int j = 0; j < stageCount; j++)
			{
				Object* stage_ = ince.GetStages(j);

				if (VMCFHT::instance().raycast(*obj_gimic->GetModel()->Get_RaycastCollition(), obj_gimic->GetTransform(), result_intersection) && !Debug_ParameterObj)
				{
					if (GetAsyncKeyState(VK_RBUTTON) & 1)
					{
						Debug_ParameterObj = obj_gimic;
					}
					break;
				}
				else if (Debug_ParameterObj)
				{
					if (VMCFHT::instance().raycast(*stage_->GetModel()->Get_RaycastCollition(), stage_->GetTransform(), result_intersection))
					{
						result_intersection.intersection_position.y = 0.750f;
						Debug_ParameterObj->SetPosition(result_intersection.intersection_position);
						if (GetAsyncKeyState(VK_RBUTTON) & 1)
						{
							Debug_ParameterObj = nullptr;
						}
					}
					else
					{
						XMFLOAT3 cameraeye{ Camera::instance().GetEye() };
						XMFLOAT3 camerafront{ Camera::instance().GetFront() };
						cameraeye.z += camerafront.z * objLength;
						cameraeye.x += camerafront.x * objLength;
						cameraeye.y += camerafront.y * objLength;

						Debug_ParameterObj->SetPosition(cameraeye);
						if (GetAsyncKeyState(VK_RBUTTON) & 1)
						{
							Debug_ParameterObj = nullptr;
						}


					}
					break;
				}
			}
		}
		break;
	case debugType::null:
		break;
	}
}

void StageManager::Create_Object(ID3D11Device* device)
{
	VMCFHT::instance().update(scene_data.view_projection, scene_data.camera_position);

	result_intersection = {};
	int stage_count = Stages.size();

	switch (o_or_g)
	{
	case debugType::obj:
		for (int i = 0; i < stage_count; i++)
		{
			Objectmanajer& ince_obj = Objectmanajer::incetance();
			StageManager& ince = incetance();
			int stagecount = ince.GetStageCount();

			Object* stage_ = ince.GetStages(i);

			if (VMCFHT::instance().raycast(*stage_->GetModel()->Get_RaycastCollition(), stage_->GetTransform(), result_intersection))
			{

				if (GetAsyncKeyState(VK_RBUTTON) & 1)
				{

					if (ince.GetCreateObjeType() != ObjType::null && Object_CreateFlag)
					{
						result_intersection.intersection_position.y += 0.9f;
						ince.Set_CreateObject_Thred(ince.GetCreateObjeType(), device, result_intersection);
						ince.DeleteThred();
						break;
					}

				}
			}
			else
			{
				Debug_ParameterObj = nullptr;
			}

		}
		break;
	case debugType::gimic:

		for (int i = 0; i < stage_count; i++)
		{
			Objectmanajer& ince_obj = Objectmanajer::incetance();
			StageManager& ince = incetance();
			Object* stage_ = ince.GetStages(i);

			if (VMCFHT::instance().raycast(*stage_->GetModel()->Get_RaycastCollition(), stage_->GetTransform(), result_intersection))
			{

				if (GetAsyncKeyState(VK_RBUTTON) & 1)
				{

					if (ince.GetCreateGimicType() != Gimic_Type::null && Object_CreateFlag)
					{
						result_intersection.intersection_position.y += 0.9f;
						ince.Set_CreateGimic_Thred(ince.GetCreateGimicType(), device, result_intersection, ID.c_str());
						ince.DeleteThred();
						ID = "null";

						break;
					}

				}
			}
			else
			{
				Debug_ParameterObj = nullptr;
			}

		}
		break;
	case debugType::static_obj:
		for (int i = 0; i < stage_count; i++)
		{
			Objectmanajer& ince_obj = Objectmanajer::incetance();

			int stagecount = GetStageCount();

			Object* stage_ = GetStages(i);

			if (VMCFHT::instance().raycast(*stage_->GetModel()->Get_RaycastCollition(), stage_->GetTransform(), result_intersection))
			{

				if (GetAsyncKeyState(VK_RBUTTON) & 1)
				{

					if (GetCreateStaticObjeType() != Static_ObjType::null && Object_CreateFlag)
					{
						result_intersection.intersection_position.y += 0.9f;
						if (Static_ObjType::Static_Object == GetCreateStaticObjeType())
						{

							Set_CreateStaticObject_Thred(GetCreateStaticObjeType(), device, result_intersection, static_cast<const char*>(static_objFilename.c_str()));
						}
						else
						{
							Set_CreateStaticObject_Thred(GetCreateStaticObjeType(), device, result_intersection);
						}
						DeleteThred();
						static_objFilename = "";
						break;
					}

				}
			}
			else
			{
				Debug_ParameterObj = nullptr;
			}

		}
		break;
	case debugType::null:
		break;
	default:
		break;
	}




}

void StageManager::Delete_Object()
{
	Objectmanajer& ince_obj = Objectmanajer::incetance();

	switch (o_or_g)
	{
	case debugType::obj:
		if (GetAsyncKeyState(VK_RBUTTON) & 1)
		{
			VMCFHT::instance().update(scene_data.view_projection, scene_data.camera_position);

			result_intersection = {};
			int gameobject_count = ince_obj.Get_GameObjCount();
			for (int i = 0; i < gameobject_count; i++)
			{

				Object* obj = ince_obj.Get_GameObject(i);
				if (VMCFHT::instance().raycast(*obj->GetModel()->Get_RaycastCollition(), obj->GetTransform(), result_intersection))
				{
					obj->Destroy();
					break;
				}
				else
				{
					Debug_ParameterObj = nullptr;
				}
			}
		}
		break;
	case debugType::gimic:
		if (GetAsyncKeyState(VK_RBUTTON) & 1)
		{
			VMCFHT::instance().update(scene_data.view_projection, scene_data.camera_position);

			result_intersection = {};
			int gamegimic_count = Objectmanajer::incetance().Get_GameGimicCount();
			for (int i = 0; i < gamegimic_count; i++)
			{
				Gimic* obj = ince_obj.Get_GameGimic(i);

				if (VMCFHT::instance().raycast(*obj->GetModel()->Get_RaycastCollition(), obj->GetTransform(), result_intersection))
				{
					obj->Destroy();
					break;
				}
				else
				{
					Debug_ParameterObj = nullptr;
				}
			}
		}
		break;
	case debugType::static_obj:
		if (GetAsyncKeyState(VK_RBUTTON) & 1)
		{
			VMCFHT::instance().update(scene_data.view_projection, scene_data.camera_position);

			result_intersection = {};
			int gamegimic_count = Objectmanajer::incetance().Get_GameStatic_ObjectCount();
			for (int i = 0; i < gamegimic_count; i++)
			{
				Static_Object* obj = ince_obj.Get_GameStatic_Object(i);

				if (VMCFHT::instance().raycast(*obj->GetModel()->Get_RaycastCollition(), obj->GetTransform(), result_intersection))
				{
					obj->Destroy();
					break;
				}
				else
				{
					Debug_ParameterObj = nullptr;
				}
			}
		}

		break;
	}
}