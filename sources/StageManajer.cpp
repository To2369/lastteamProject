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
#include"PlayerManager.h"
void StageManager::Initialize_GameStage(StageName name, ID3D11Device* device)
{
	using namespace std;
	Objectmanajer& obj_Manager = Objectmanajer::incetance();
	UIManager& ince_UI = UIManager::incetance();
	XMFLOAT3 pos{};
	switch (name)
	{
	case StageName::stage1_1:
	{
		PlayerManager& ince_p = PlayerManager::Instance();
		ince_p.GetPlayer(0)->SetPosition({ 1.146f,0.900f,0.100f });
		Object::CollitionList raylist;
		raylist.IsRayCastGround = true;
		raylist.IsRayCastInvisibleWall = true;
		raylist.IsBoundhingBoxVSGimic = true;
		raylist.IsRayCastObject = true;
		obj_Manager.Initialize(ObjType::heavy, device, false, {

			  -1.037836f,
			   1.747557f,
			  -0.430794f
			},0.123f,raylist);
		raylist={};
		raylist.IsRayCastGround = true;
		raylist.IsSphereCollition = true;
		obj_Manager.Initialize(ObjType::cution, device, true, {
			 -1.336630f,
			 1.747557f,
			  0.460227f
			},0.123f,raylist);
		obj_Manager.Initialize(name, Gimic_Type::Switch, device, {
		-2.010655f,
		0.640562f,
		-0.38644f,
			}, "kabe");
		obj_Manager.Initialize(name, Gimic_Type::Goal, device, {
		-0.970918f,
		0.747557f,
		-1.307508f
			});


		{
			unique_ptr<Static_Object>obj = make_unique<stage_OBJ>(device);
			obj_Manager.Initialize_Static_Object(move(obj));

		}
		{

			obj_Manager.Initialize_InvisibleBarria(device,
				{
					0.762000f,
					1.473000f,
					-1.133001f
				},
				{

					305.402313f,
					198.938995f,
					164.204910f
				});
			obj_Manager.Initialize_InvisibleBarria(device,
				{
					-1.001000f,
					0.841f,
					-0.812f

				},
				{
					50.922291f,
					58.687035f,
					14.180f
				});
			obj_Manager.Initialize_InvisibleBarria(device,
				{
					-2.001124f,
					0.841f,
					-0.812f
				},
				{
					50.922291f,
					58.687035f,
					14.180f
				});
			obj_Manager.Initialize_InvisibleBarria(device,
				{
					-0.810468f,
					0.382496f,
					0.076248f
				},
				{
					-27.452732f,
					 57.414883f,
					 116.421349f
				});
			unique_ptr<Static_Object>invisiblewall;
			invisiblewall = make_unique<InvisibleBarrier>(device);
			invisiblewall->SetPosition(
				{
				-1.493f,
				0.937f,
				-0.822f });
			invisiblewall->SetScale({ 194.727f,116.687f,28.109f });
			invisiblewall->WallSetID("kabe");
			invisiblewall->SetIsWallFlagType(Gimic_Type::Door);
			obj_Manager.Rigister_Static_Object(move(invisiblewall));

		}
		{
			unique_ptr<Stage>stage = make_unique<Stage_1_1>(device);
			stage->SetPosition({ 0.f, 0.5f, -0.5f });
			Rigister(move(stage));
		}
		obj_Manager.Initialize(name, Gimic_Type::Door, device, {
		 -1.490f,
		 0.636f,
		 -0.925f
			}, "kabe");
		//ince_UI.CreateGameSceneUI(device);
	}
	break;
	case StageName::stage1_2:
	{
		string filename;
		PlayerManager& ince_p = PlayerManager::Instance();
		Object::CollitionList raylist;
		unique_ptr<Gimic> lift;
		ince_p.GetPlayer(0)->SetPosition({ 3.702f,-3.020f,-0.443f });
		obj_Manager.Initialize(StageName::stage1_2,
			Gimic_Type::Goal, device,
			{ 7.577f,7.223f,-0.535f });
		//{
		//	//Lift
		//	{
		//		obj_Manager.Initialize(name, Gimic_Type::Lift, device,
		//			{ 4.252f,7.290f,-0.020f },
		//			"num1",
		//			{ 4.252f,5.154f,-0.020f });
		//		
		//		
		//		filename = failepath::Stage_failepath::Stage3_Path + "rihutoB.fbx";
		//		obj_Manager.Initialize(filename.c_str(), Chain_Type::lift_P_Not_Animation,
		//			{ 4.30f,7.287f,-0.948f },
		//			"num1",
		//			{ 0.f,-1.6f,0.f }

		//		);
		//		filename = failepath::Stage_failepath::Stage3_Path + "rihuto_Bdown_B.fbx";
		//		obj_Manager.Initialize(filename.c_str(), Chain_Type::lift_P_Animatio_ndown,
		//			{ 4.247f,8.342f,-0.941f },
		//			"num1",
		//			{ 0.f,1.6f,0.f }
		//		);

		//		obj_Manager.Initialize(Chain_Type::lift_chain_L,
		//			{ 4.304f,0,-0.432f, },
		//			"num1",
		//			{ 0.000000f,-1.620995f,0.000000f });

		//		obj_Manager.Initialize(Chain_Type::lift_chain_S,
		//			{ 4.208f,7.794f,-0.432f, },
		//			"num1",
		//			{ 0.000000f,-1.590007f,0.000000f });
		//	}
		//	//object
		//	{
		//		raylist = {};
		//		obj_Manager.Initialize(failepath::Object_failepath::Object::Pengin.filename.c_str(), ObjType::cution, device, true,
		//			{
		//			 1.879f,
		//			 5.094f,
		//			-1.005f
		//			},0.123f,raylist);
		//		raylist.IsBoundhingBoxVSGimic = true;
		//		raylist.IsRayCastInvisibleWall = false;
		//		raylist.IsRayCastObject = false;
		//		raylist.IsRayCastGround = true;
		//		obj_Manager.Initialize(failepath::Object_failepath::Object::Denti.filename.c_str(), ObjType::Super_heavy, device, false,
		//			{
		//			4.421f,
		//			 8.920f,
		//			-0.061f
		//			},0.228f,raylist);
		//	}
		//}
		//{
		//	//lift
		//	{
		//	obj_Manager.Initialize(name, Gimic_Type::Lift, device,
		//		{ -0.891f,5.178f,0.012f },
		//		"num2",
		//		{ -0.891f,3.089f,0.012f });

		//	obj_Manager.Initialize(Chain_Type::lift_P_Not_Animation,
		//		{ -0.839f,5.218f,-0.929f },
		//		"num2",
		//		{ 0.f,1.6f,0.f });
		//	obj_Manager.Initialize(Chain_Type::lift_P_Animatio_ndown,
		//		{ -0.851f,4.808f,-0.914f },
		//		"num2",
		//		{ 0.f,-1.6f,0.f });
		//	obj_Manager.Initialize(Chain_Type::lift_chain_L,
		//		{ -0.860f,0.f,-0.440, },
		//		"num2",
		//		{ 0.000000f,-1.620995f,0.000000f });

		//	obj_Manager.Initialize(Chain_Type::lift_chain_S,
		//		{ -0.871f,5.644f,-0.440f, },
		//		"num2",
		//		{ 0.000000f,-1.590007f,0.000000f });
		//    }
		//	//object
		//	{
		//		raylist = {};
		//		obj_Manager.Initialize(failepath::Object_failepath::Object::Pengin.filename.c_str(), ObjType::cution, device, true,
		//			{
		//			 -2.073f,
		//			2.986f,
		//			-1.005f
		//			},0.228 ,raylist);
		//		raylist.IsBoundhingBoxVSGimic = true;
		//		raylist.IsRayCastInvisibleWall = false;
		//		raylist.IsRayCastObject = false;
		//		raylist.IsRayCastGround = true;
		//		obj_Manager.Initialize(failepath::Object_failepath::Object::Denti.filename.c_str(), ObjType::heavy, device, false,
		//			{
		//			-0.747f,
		//			6.347f,
		//			0.093f
		//			}, 0.228f, raylist);
		//	}

		//}

		//{
		//	//lift
		//	{
		//		obj_Manager.Initialize(name, Gimic_Type::Lift, device,
		//			{ -5.158f,1.562f,-0.945f },
		//			"num3",
		//			{ -5.158f,-0.574f,-0.945f });
		//		filename = failepath::Stage_failepath::Stage3_Path + "rihutoB.fbx";
		//		obj_Manager.Initialize(filename.c_str(), Chain_Type::lift_P_Not_Animation,
		//			{ -5.096f,1.555f,-0.013f },
		//			"num3",
		//			{ 0.f,-1.6f,0.f }
		//		);
		//		filename = failepath::Stage_failepath::Stage3_Path + "rihuto_Bdown_B.fbx";
		//		obj_Manager.Initialize(filename.c_str(), Chain_Type::lift_P_Animatio_ndown,
		//			{ -5.157f,2.687f,-0.013f },
		//			"num3",
		//			{ 0.f,1.6f,0.f }
		//		);

		//		obj_Manager.Initialize(Chain_Type::lift_chain_L,
		//			{ -5.110f,0.f,-0.483f },
		//			"num3",
		//			{ 0.000000f,1.6f,0.000000f });

		//		obj_Manager.Initialize(Chain_Type::lift_chain_S,
		//			{ -5.158f,2.025f, -0.492f },
		//			"num3",
		//			{ 0.000000f,1.6f,0.000000f });
		//	}
		//	//object
		//	{
		//		raylist = {};
		//		raylist.IsSphereCollition = true;
		//		unique_ptr<Object>obj = make_unique<Cution>(device, failepath::Object_failepath::Object::Clip.filename.c_str());
		//		obj->SetScale({6.361f,6.208f,6.130f});
		//		obj->israycast = raylist;
		//		obj->SetPosition({ -3.076f,-0.776f,-0.51f });
		//		obj->SetRadius(0.228f);
		//		obj_Manager.Rigister_obj(move(obj));
		//		/*obj_Manager.Initialize(failepath::Object_failepath::Object::Clip.filename.c_str(), ObjType::Super_cution, device, false,
		//			{
		//			 -3.076f,
		//			-0.486f,
		//			-0.581f
		//			}, 0.228f,raylist);*/
		//		raylist.IsBoundhingBoxVSGimic = true;
		//		raylist.IsRayCastInvisibleWall = false;
		//		raylist.IsRayCastObject = false;
		//		raylist.IsRayCastGround = true;
		//		obj_Manager.Initialize(failepath::Object_failepath::Object::Denti.filename.c_str(), ObjType::heavy, device, false,
		//			{
		//			-5.284f,
		//			2.f,
		//			-1.016f
		//			}, 0.228f,raylist);
		//	}
		//}
		//{
		//	//lift
		//     {
		//     	 obj_Manager.Initialize(name, Gimic_Type::Lift, device,
		//     	 	{ -0.008f,-0.600f,-0.932f },
		//     	 	"num4",
		//     	 	{ -0.008f,-2.707f,-0.932f });
		//     	 obj_Manager.Initialize(Chain_Type::lift_P_Not_Animation,
		//     	 	{ -0.002f,-0.573f,-0.013f },
		//     	 	"num4",
		//     	 	{ 0.f,-1.6f,0.f }
		//     	 );
		//     	 obj_Manager.Initialize(Chain_Type::lift_P_Animatio_ndown,
		//     	 	{ -0.009f,-0.944f,-0.013f },
		//     	 	"num4",
		//     	 	{ 0.f,1.6f,0.f }
		//     	 );
		//     	 
		//     	 obj_Manager.Initialize(Chain_Type::lift_chain_L,
		//     	 	{ -0.019f,0.f,-0.483f },
		//     	 	"num4",
		//     	 	{ 0.000000f,1.6f,0.000000f });
		//     	 
		//     	 obj_Manager.Initialize(Chain_Type::lift_chain_S,
		//     	 	{ -0.004f,-0.120f, -0.492f },
		//     	 	"num4",
		//     	 	{ 0.000000f,1.6f,0.000000f });
		//     }
		//	//object
		//	{
		//		raylist = {};
		//		obj_Manager.Initialize(failepath::Object_failepath::Object::Pengin.filename.c_str(), ObjType::cution, device, false,
		//			{
		//			 2.397f,
		//			-2.823f,
		//			-1.227f
		//			},0.228f,raylist);
		//		raylist.IsBoundhingBoxVSGimic = true;
		//		raylist.IsRayCastInvisibleWall = false;
		//		raylist.IsRayCastObject = false;
		//		raylist.IsRayCastGround = true;
		//		obj_Manager.Initialize(failepath::Object_failepath::Object::Denti.filename.c_str(), ObjType::heavy, device, false,
		//			{
		//			-0.093f,
		//			-0.393f,
		//			-0.961f
		//			},0.228f,raylist);
		//		/*unique_ptr<Object> sobj = make_unique<Heavy>(device, failepath::Object_failepath::Object::Wall.filename.c_str());
		//		sobj->SetPosition({0,1.f,0.f});
		//		obj_Manager.Rigister_obj(move(sobj));*/

		//	}

		//}

		unique_ptr<Stage>stage = make_unique<Stage_1_2>(device);
		stage->SetPosition({ 0.f, 0.5f, -0.5f });
		Rigister(move(stage));
		ince_UI.CreateGameSceneUI(device);
	}
	break;
	case StageName::stage1_3:
	{
		unique_ptr<Gimic> gimic;
		unique_ptr<Object>obj;
		PlayerManager& ince_p = PlayerManager::Instance();
		Object::CollitionList raylist;
		ince_p.GetPlayer(0)->SetPosition({ 0.702f,-0.020f,-0.443f });
		obj_Manager.Initialize(StageName::stage1_3,
			Gimic_Type::Goal, device,
			{ -4.378f,3.147f,-2.451f });
		bool flags[3] = {};
		//object
		{
			raylist={};
			raylist.IsBoundhingBoxVSGimic = true;
			raylist.IsRayCastInvisibleWall = false;
			raylist.IsRayCastObject = false;
			raylist.IsRayCastGround = true;
			obj_Manager.Initialize( ObjType::heavy, device, true,
				{
				 -3.119f,
				1.956f,
				-1.004f
				}, 0.228f,raylist);

			raylist = {};
			raylist.IsBoundhingBoxVSGimic = true;
			raylist.IsRayCastInvisibleWall = false;
			raylist.IsRayCastObject = false;
			raylist.IsRayCastGround = true;
			obj = make_unique<Super_Heavy>(device,failepath::Object_failepath::Object::Light.filename.c_str());
			obj->SetStatic_Objflag(true);
			obj->israycast = raylist;
			obj->SetPosition({-0.946f,4.0f,-2.625f });
			obj->SetScale({1.f,1.f,1.f});
			obj_Manager.Rigister_obj(move(obj));

			obj = nullptr;
			//obj_Manager.Initialize(failepath::Object_failepath::Object::E_kezuri.filename.c_str(), ObjType::Super_heavy, device, true, { 3.740f,1.238f,-0.445f }, {});
			obj = make_unique<Super_Heavy>(device,failepath::Object_failepath::Object::E_kezuri.filename.c_str());
			obj->SetPosition({ 3.740f,0.86f,-0.966f });
			obj->SetScale({1.f,0.68f,1.f});
			obj->SetStatic_Objflag(true);
			obj_Manager.Rigister_obj(move(obj));
			
			raylist = {};
			/*obj_Manager.Initialize(failepath::Object_failepath::Object::Clip.filename.c_str(), ObjType::Super_cution, device, true,
				{
				 0.076f,
				0.486f,
				-0.581f
				}, 0.228f, raylist);*/
			obj = make_unique<Super_Cution>(device, failepath::Object_failepath::Object::Clip.filename.c_str());
			obj->SetScale({10,10,10});
			obj->SetPosition({ -4.397f,0.265f,-0.581f });
			obj->SetStatic_Objflag(true);
			obj->SetRadius(0.228f);
			obj->israycast = raylist;
			obj_Manager.Rigister_obj(move(obj));
			raylist.IsRayCastGround = true;


			obj_Manager.Initialize(failepath::Object_failepath::Object::kyappu.filename.c_str(), ObjType::Super_cution, device, true, { 3.566f,0.869f,-3.641f }, 0.123f, raylist);
			/*	obj = make_unique<Super_Cution>(device,failepath::Object_failepath::Object::kyappu.filename.c_str());
				obj->SetPosition({ 3.566f,0.869f,-3.641f });
				obj->SetStatic_Objflag(true);
				obj->SetRadius(0.123f);*/
			obj = nullptr;
			obj = make_unique<Heavy>(device, failepath::Object_failepath::Object::Book.filename.c_str());
			obj->spehereRadius = 0.394f;
			obj->spehereLength = 0.901f;
			obj->SetRadius(0.730f);
			obj->SetPosition({ -0.169f,0.261f,3.585f });
			obj->CustomSphereFlag = true;
			obj->CustomSpherePos.offsetpos[2].z = -0.224000022f;
			obj->CustomSpherePos.offsetpos[3].z = 0.202999920f;
			obj->SetCustomizationSpherePos(obj->CustomSpherePos);
			obj_Manager.Rigister_obj(move(obj));
			obj = nullptr;

			obj = make_unique<Super_Cution>(device, failepath::Object_failepath::Object::kesigomu.filename.c_str());
			obj->SetPosition({ -1.806f,0.263f,3.721f });
			obj->SetAngle({ 0.0f,1.458f,0.0f });
			raylist = {};
			obj->israycast = raylist;
			obj->SetStatic_Objflag(true);
			obj_Manager.Rigister_obj(move(obj));
			obj = nullptr;

			obj = make_unique<Cution>(device, failepath::Object_failepath::Object::enpitu.filename.c_str());
			obj->SetPosition({ -3.069f,0.263f,3.066f });
			obj->SetAngle({ 0.0f,0.0f,0.0f });
			raylist = {};
			obj->israycast = raylist;
			obj->SetStatic_Objflag(true);
			obj_Manager.Rigister_obj(move(obj));
			obj = nullptr;

			

			obj = make_unique<Heavy>(device,failepath::Object_failepath::Object::Book.filename.c_str());
			obj->SetPosition({ 3.154f,1.923f,-2.508f});
			raylist = {};
			obj->SetRadius(0.730f);
			raylist.IsRayCastGround = true;
			raylist.IsBoundhingBoxVSGimic = true;
			obj->spehereRadius = 0.394f;
			obj->spehereLength = 0.901f;
			obj->CustomSphereFlag = true;
			obj->CustomSpherePos.offsetpos[2].z = -0.224000022f;
			obj->CustomSpherePos.offsetpos[3].z = 0.202999920f;
			obj->SetCustomizationSpherePos(obj->CustomSpherePos);
			obj->israycast = raylist;
			obj_Manager.Rigister_obj(move(obj));
			obj = nullptr;

			

			obj = make_unique<Cution>(device, failepath::Object_failepath::Object::Pengin.filename.c_str());
			obj->SetPosition({4.651f,2.030f,-1.637f});
			obj->SetStatic_Objflag(true);
			raylist = {};
			raylist.IsSphereCollition = true;
			obj->israycast = raylist;
			obj_Manager.Rigister_obj(move(obj));
			obj = nullptr;
		}
		//gimic
		{
			//ｚ軸だけ線形補完
			flags[0] = false;
			flags[1] = false;
			flags[2] = true;
			gimic = make_unique<Move_Object>(device, failepath::Object_failepath::Gimic::thissyu.filename.c_str(), XMFLOAT3(1.841f, 0.409f, -2.351f), flags, 0.010f);
			gimic->SetScale(XMFLOAT3(0.928f, 0.804f, 2.385f));
			gimic->SetGimicID("enpitukezurimae");
			gimic->SetEndPos({ 0.f,0,0, });
			obj_Manager.Rigister_Gimic(move(gimic));
			gimic = nullptr;

			gimic = make_unique<Switch>(device);
			gimic->SetScale({ 2.416f,3.456f,1.178f });
			gimic->SetPosition({ 3.603f,0.146f,-3.622f });
			gimic->SetMyObjeFlagtype(ObjType::Super_heavy);
			gimic->SetGimicID("enpitukezurimae");
			obj_Manager.Rigister_Gimic(move(gimic));
			gimic = nullptr;

			gimic = make_unique<Switch>(device);
			gimic->SetPosition({ 0.869f,1.810f,-2.252f });
			gimic->SetMyObjeFlagtype(ObjType::heavy);
			gimic->SetGimicID("2danme");
			gimic->SetScale({ 2.115f,3.456f,3.496f });
			obj_Manager.Rigister_Gimic(move(gimic));
			gimic = nullptr;

			flags[0] = false;
			flags[1] = false;
			flags[2] = true;
			gimic = make_unique<Move_Object>(device, failepath::Object_failepath::Gimic::hikidasi_k.filename.c_str(), XMFLOAT3(-1.407f, 2.294f, -2.335f), flags, 0.010f);
			gimic->SetEndPos({ 0,0,-0.749, });
			gimic->SetGimicID("2danme");
			obj_Manager.Rigister_Gimic(move(gimic));
			gimic = nullptr;

			gimic = make_unique<Switch>(device);
			gimic->SetPosition({ -0.830f,3.139f,-2.539f });
			gimic->SetMyObjeFlagtype(ObjType::Super_heavy);
			gimic->SetGimicID("goalmae");
			gimic->SetScale({ 2.115f,14.207f,3.496f });
			obj_Manager.Rigister_Gimic(move(gimic));
			gimic = nullptr;
			flags[0] = true;
			flags[1] = false;
			flags[2] = false;
			gimic = make_unique<Move_Object>(device, failepath::Object_failepath::Gimic::hontate.filename.c_str(), XMFLOAT3(-2.796f, 4.049f, -2.529f), flags, 0.010f);
			gimic->SetEndPos({ -1.920f,0,-0.f, });
			gimic->SetGimicID("goalmae");
			obj_Manager.Rigister_Gimic(move(gimic));
			gimic = nullptr;
			

		}
		//Lift
		{
			obj_Manager.Initialize(name, Gimic_Type::Lift, device,
				{ -3.114f,2.300f,-0.911f },
				"gimic3",
				{ -3.114f,0.941f,-0.911f });
			
			obj_Manager.Initialize(failepath::Object_failepath::Gimic::rihuto_downNotAnimation.filename.c_str(), Chain_Type::lift_P_Not_Animation,
				{ -4.992f,2.293f,-1.071f },
				"gimic3",
				{ 0.f,6.223f,0.f }
			);
			obj_Manager.Initialize(failepath::Object_failepath::Gimic::rihuto_downAnimation.filename.c_str(), Chain_Type::lift_P_Animatio_ndown,
				{ -4.992f,3.087f,-1.067f },
				"gimic3",
				{ 0.f,6.223f,0.f }
			);

			obj_Manager.Initialize(failepath::Object_failepath::Gimic::rihuto_ChainL.filename.c_str(),Chain_Type::lift_chain_L,
				{ -4.023f,1.180f,-0.982f },1.f,
				"gimic3",
				{ 0.000000f,0.f,0.000000f });

			obj_Manager.Initialize(failepath::Object_failepath::Gimic::rihuto_ChainS.filename.c_str(),Chain_Type::lift_chain_S,
				{ -4.023f,2.737f,-0.982f },
				"gimic3",
				{ 0.000000f,0.f,0.000000f });

		}
		{
			unique_ptr<Stage>stage = make_unique<Stage_1_3>(device);
			stage->SetPosition({ 0.f, 0.0f, -0.0f });
			Rigister(move(stage));
			//ince_UI.CreateGameSceneUI(device);
		}
	}
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
		OutputDebugStringA("/////////////Lift_Chain_INFO///////////////"); OutputDebugStringA("\n"); OutputDebugStringA("\n");
		count = ince.Get_GameLiftChainCount();
		for (int i = 0; i < count; i++)
		{
			BaseChain* obj = ince.Get_GameLiftChain(i);
			obj->ResultInfo();
		}
	}

	if (Debug_ParameterObj)
	{
		Debug_ParameterObj = nullptr;
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
			break;
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
	auto typeList = [](Gimic_Type type) {
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
		case Gimic_Type::Lift:
			return "Lift";
			break;
		case Gimic_Type::null:
			return "null";
			break;
		default:
			break;
		}
		return "";
		};

	string s = typeList(obj.Get_GimicType());


	OutputDebugStringA(s.c_str());
	obj.ResultInfo();
}

void StageManager::Result_Static_Object_Info(Static_Object& obj)
{


	auto typeList = [](Static_ObjType type) {

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

	string s = typeList(obj.GetStatic_ObjType());
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
		case Gimic_Type::Lift:
			return "Lift";
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
	auto chain = [](Chain_Type type)
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
	ImGui::Text("Debug_Mode : %s", m(Getmode()));
	if (ImGui::CollapsingHeader("SelectMode", ImGuiTreeNodeFlags_DefaultOpen))
	{

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
			if (ImGui::Button("LiftChain"))o_or_g = debugType::LiftChain;

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
			case debugType::LiftChain:
				return "LiftChain";
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
			if (ImGui::Button("LiftChain"))o_or_g = debugType::LiftChain;
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
					if (ImGui::Button("Lift")) CreateGimicType = Gimic_Type::Lift;
					if (ImGui::Button("Move_Object")) CreateGimicType = Gimic_Type::Move_Object;
					

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
			if (CreateGimicType != Gimic_Type::null &&
				CreateObjeType != ObjType::null &&
				CreateStaticObjeType != Static_ObjType::null)
			{
				CreateStaticObjeType = Static_ObjType::null;
			}

		}
		else if (o_or_g == debugType::LiftChain)
		{
			if (CreateChainType == Chain_Type::null)
			{
				if (ImGui::Button("lift_chain_L")) CreateChainType = Chain_Type::lift_chain_L;
				if (ImGui::Button("lift_chain_S"))CreateChainType = Chain_Type::lift_chain_S;
				if (ImGui::Button("lift_P_Animatio_ndown"))CreateChainType = Chain_Type::lift_P_Animatio_ndown;
				if (ImGui::Button("lift_P_Not_Animation"))CreateChainType = Chain_Type::lift_P_Not_Animation;
			}
			if (CreateGimicType != Gimic_Type::null &&
				CreateObjeType != ObjType::null &&
				CreateStaticObjeType != Static_ObjType::null &&
				CreateChainType != Chain_Type::null)
			{
				CreateChainType = Chain_Type::null;
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
				ImGui::InputText("Createfilename", const_cast<char*>(static_objFilename.c_str()), Buffer);
			}
			if (ImGui::CollapsingHeader("NewCreateObj", ImGuiTreeNodeFlags_DefaultOpen))
			{
				ImGui::Text("CreateObjType:%s", sc(CreateStaticObjeType));
				DebugMode_MouseRayCast(mode, device);

			}

		}
		if (CreateChainType != Chain_Type::null)
		{
			if (ImGui::Button("NULL")) CreateChainType = Chain_Type::null;
			// テキスト入力フィールドを表示
			const int Buffer = 256;
			ImGui::InputText("CreateID", const_cast<char*>(ID.c_str()), Buffer);
			ImGui::Checkbox("CreateFlag", &Object_CreateFlag);
			if (ImGui::CollapsingHeader("NewCreateObj", ImGuiTreeNodeFlags_DefaultOpen))
			{
				ImGui::Text("CreateObjType:%s", chain(CreateChainType));
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

void StageManager::CreateObject_2(const char* filename_, ObjType type, ID3D11Device* device, Intersection in)
{
	Objectmanajer& ince = Objectmanajer::incetance();

	unique_ptr<Object>obj;
	switch (type)
	{
	case ObjType::cution:
		obj = make_unique<Cution>(device,filename_);
		obj->SetPosition(in.intersection_position);
		ince.Rigister_obj(move(obj));
		break;
	case ObjType::Super_cution:
		obj = make_unique<Super_Cution>(device,filename_);
		obj->SetPosition(in.intersection_position);
		ince.Rigister_obj(move(obj));
		break;
	case ObjType::heavy:
		obj = make_unique<Heavy>(device,filename_);
		obj->SetPosition(in.intersection_position);
		ince.Rigister_obj(move(obj));
		break;
	case ObjType::Super_heavy:
		obj = make_unique<Super_Heavy>(device,filename_);
		obj->SetPosition(in.intersection_position);
		ince.Rigister_obj(move(obj));

		break;
	case ObjType::Fragile:
		obj = make_unique<Fragile>(device,filename_);
		obj->SetPosition(in.intersection_position);
		ince.Rigister_obj(move(obj));

		break;
	case ObjType::Super_fragile:
		obj = make_unique<Super_fragile>(device,filename_);
		obj->SetPosition(in.intersection_position);
		ince.Rigister_obj(move(obj));

		break;
	case ObjType::Hard_to_Break:
		obj = make_unique<Hard_to_Break>(device,filename_);
		obj->SetPosition(in.intersection_position);
		ince.Rigister_obj(move(obj));

		break;
	case ObjType::Super_hard_to_Break:
		obj = make_unique<Super_hard_to_Break>(device,filename_);
		obj->SetPosition(in.intersection_position);
		ince.Rigister_obj(move(obj));

		break;
	case ObjType::Crack:
		obj = make_unique<Crack>(device,filename_);
		obj->SetPosition(in.intersection_position);
		ince.Rigister_obj(move(obj));

		break;
	case ObjType::null:
		break;
	default:
		break;
	}
}

void StageManager::CreateGimic_2(const char* filename_, Gimic_Type type, ID3D11Device* device, Intersection in, std::string id)
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
		obj = make_unique<Goal>(device,filename_);
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
	case Gimic_Type::Lift:
		obj = make_unique<Lift>(device,in.intersection_position,filename_);
		obj->SetGimicID(id);
		ince.Rigister_Gimic(move(obj));
		break;
	case Gimic_Type::Move_Object:
	{
		bool flags[3] = {};
		obj = make_unique<Move_Object>(device,filename_, in.intersection_position, flags);
		obj->SetGimicID(id);
		ince.Rigister_Gimic(move(obj));
	}
	break;
	case Gimic_Type::null:

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
	case Gimic_Type::Lift:
		obj = make_unique<Lift>(device, in.intersection_position);
		obj->SetGimicID(id);
		ince.Rigister_Gimic(move(obj));
		break;
	case Gimic_Type::Move_Object:
	{
		bool flags[3] = {};
		obj = make_unique<Move_Object>(device, in.intersection_position, flags);
		obj->SetGimicID(id);
		ince.Rigister_Gimic(move(obj));
	}
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
		break;
	case Static_ObjType::null:
		break;
	}
}

void StageManager::CreateLiftChain(Chain_Type type, ID3D11Device* device, Intersection in, std::string id)
{
	unique_ptr<BaseChain>obj;
	Objectmanajer& ince = Objectmanajer::incetance();
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
	case Chain_Type::null:
		break;

	}
	obj->GetTransformComp()->SetPosition(in.intersection_position);
	obj->SetID(id);
	ince.Rigister_Lift_Chains(move(obj));
}
void StageManager::CreateLiftChain_2(const char* filename, Chain_Type type, ID3D11Device* device, Intersection in, std::string id)
{
	unique_ptr<BaseChain>obj;
	Objectmanajer& ince = Objectmanajer::incetance();
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
	case Chain_Type::null:
		break;

	}
	obj->GetTransformComp()->SetPosition(in.intersection_position);
	obj->SetID(id);
	ince.Rigister_Lift_Chains(move(obj));
}
void StageManager::DebugMode_MouseRayCast(DebugMode mode, ID3D11Device* device)
{


	if (mode == DebugMode::Object_Info)
	{
		//右クリック
		Object_Info();
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
					if (SetObjType == ObjType::null)
					{
						if (ImGui::Button("heavy")) SetObjType = ObjType::heavy;
						if (ImGui::Button("Super_heavy")) SetObjType = ObjType::Super_heavy;
						if (ImGui::Button("Cution")) SetObjType = ObjType::cution;
						if (ImGui::Button("Super_cution")) SetObjType = ObjType::Super_cution;
						if (ImGui::Button("Hard_to_Break")) SetObjType = ObjType::Hard_to_Break;
						if (ImGui::Button("Super_hard_to_Break")) SetObjType = ObjType::Super_hard_to_Break;
						if (ImGui::Button("Fragile")) SetObjType = ObjType::Fragile;
						if (ImGui::Button("Super_fragile")) SetObjType = ObjType::Super_fragile;
						if (ImGui::Button("Crack")) SetObjType = ObjType::Crack;
					}
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

		if (Debug_ParameterObj && !Debug_ParameterObj->GetDestroyObje())
		{
			Debug_ParameterObj->Gui();
		}
		break;
	case debugType::LiftChain:
		if (GetAsyncKeyState(VK_RBUTTON) & 1)
		{
			VMCFHT::instance().update(scene_data.view_projection, scene_data.camera_position);

			result_intersection = {};
			int gameliftchain_count = Objectmanajer::incetance().Get_GameLiftChainCount();
			for (int i = 0; i < gameliftchain_count; i++)
			{
				Objectmanajer& ince_obj = Objectmanajer::incetance();
				BaseChain* obj = ince_obj.Get_GameLiftChain(i);
				collision_mesh* mesh = obj->GetRenderComp()->GetModel()->Get_RaycastCollition();

				XMFLOAT4X4 transform = obj->GetTransformComp()->Gettransform();
				if (VMCFHT::instance().raycast(*mesh, transform, result_intersection))
				{
					Debug_ParameterChain = obj;
					break;
				}
				else
				{
					Debug_ParameterChain = nullptr;

				}
			}
		}

		if (Debug_ParameterChain && !Debug_ParameterChain->GetDestroy())
		{
			Debug_ParameterChain->Gui();
		}

		break;
	}



}

void StageManager::Create_Object(ID3D11Device* device)
{
	VMCFHT::instance().update(scene_data.view_projection, scene_data.camera_position);

	result_intersection = {};
	int stage_count = static_cast<int>(Stages.size());

	switch (o_or_g)
	{
	case debugType::obj:
	{
		failepath::Object_failepath::ObjectName* name[99]
		{
			&failepath::Object_failepath::Object::Book,
			&failepath::Object_failepath::Object::E_kezuri,
			&failepath::Object_failepath::Object::kesigomu,
			&failepath::Object_failepath::Object::kyappu,
			&failepath::Object_failepath::Object::enpitu,
			&failepath::Object_failepath::Object::Light,
			&failepath::Object_failepath::Object::Clip,
			&failepath::Object_failepath::Object::Denti,
			&failepath::Object_failepath::Object::Kami,
			&failepath::Object_failepath::Object::Pengin,
			&failepath::Object_failepath::Object::Wall,
			nullptr,
		};
		ImGui::Checkbox("chengeThred", &CreateGimicTypeChenge);
		if (CreateGimicTypeChenge)
		{
			ImGui::Text("Notfilename");
		}
		else
		{

			ImGui::Text("Createfilenames");
			ImGui::InputInt("Createfilename", &ObjectfilenameIndex);
			if (ObjectfilenameIndex < 0)
			{
				ObjectfilenameIndex = 0;
			}
			else if (!name[ObjectfilenameIndex])
			{
				while (!name[ObjectfilenameIndex])
				{
					ObjectfilenameIndex--;
				}

			}
			ImGui::Text(name[ObjectfilenameIndex]->filename.c_str());
		}
		for (int i = 0; i < stage_count; i++)
		{
			StageManager& ince = incetance();

			Object* stage_ = ince.GetStages(i);

			if (VMCFHT::instance().raycast(*stage_->GetModel()->Get_RaycastCollition(), stage_->GetTransform(), result_intersection))
			{

				if (GetAsyncKeyState(VK_RBUTTON) & 1)
				{
					if (CreateGimicTypeChenge)
					{
						if (ince.GetCreateObjeType() != ObjType::null && Object_CreateFlag)
						{
							result_intersection.intersection_position.y += 0.9f;
							ince.Set_CreateObject_Thred(ince.GetCreateObjeType(), device, result_intersection);
							ince.DeleteThred();
							break;
						}
					}
					else
					{
						if (ince.GetCreateObjeType() != ObjType::null && Object_CreateFlag)
						{
							result_intersection.intersection_position.y += 0.9f;
							ince.Set_CreateObject_Thred(name[ObjectfilenameIndex]->filename.c_str(),ince.GetCreateObjeType(), device, result_intersection);
							ince.DeleteThred();
							break;
						}

					}

				}
			}
			else
			{
				Debug_ParameterObj = nullptr;
			}

		}
	}
		break;
	case debugType::gimic:
	{
		failepath::Object_failepath::ObjectName* name[99]
		{
			&failepath::Object_failepath::Gimic::hikidasi_k,
			&failepath::Object_failepath::Gimic::thissyu,
			&failepath::Object_failepath::Gimic::hontate,
			nullptr,
		};
		ImGui::Checkbox("chengeThred", &CreateGimicTypeChenge);
		if (CreateGimicTypeChenge)
		{
			ImGui::Text("Notfilename");
		}
		else
		{

			ImGui::Text("Createfilenames");
			ImGui::InputInt("Createfilename", &ObjectfilenameIndex);
			if (ObjectfilenameIndex < 0)
			{
				ObjectfilenameIndex = 0;
			}
			else if (!name[ObjectfilenameIndex])
			{
				while (!name[ObjectfilenameIndex])
				{
					ObjectfilenameIndex--;
				}
			}
			ImGui::Text(name[ObjectfilenameIndex]->filename.c_str());
		}
		for (int i = 0; i < stage_count; i++)
		{
			StageManager& ince = incetance();
			Object* stage_ = ince.GetStages(i);

			if (VMCFHT::instance().raycast(*stage_->GetModel()->Get_RaycastCollition(), stage_->GetTransform(), result_intersection))
			{

				if (GetAsyncKeyState(VK_RBUTTON) & 1)
				{

					if (ince.GetCreateGimicType() != Gimic_Type::null && Object_CreateFlag)
					{
						result_intersection.intersection_position.y += 0.9f;
						if (CreateGimicTypeChenge)
						{
							ince.Set_CreateGimic_Thred(ince.GetCreateGimicType(), device, result_intersection, ID.c_str());
							ince.DeleteThred();
							ID = "null";
						}
						else
						{

							ince.Set_CreateGimic_Thred(name[ObjectfilenameIndex]->filename.c_str(), ince.GetCreateGimicType(), device, result_intersection, ID.c_str());
							ince.DeleteThred();
							ID = "null";
						}
						break;
					}

				}
			}
			else
			{
				Debug_ParameterObj = nullptr;
			}

		}
	}
		break;
	case debugType::static_obj:
		for (int i = 0; i < stage_count; i++)
		{
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
	case debugType::LiftChain:
	{
		failepath::Object_failepath::ObjectName* name[99]
		{
			&failepath::Object_failepath::Gimic::rihuto_ChainL,
			&failepath::Object_failepath::Gimic::rihuto_ChainS,
			&failepath::Object_failepath::Gimic::rihuto_downAnimation,
			&failepath::Object_failepath::Gimic::rihuto_downNotAnimation,
			nullptr,
		};
		ImGui::Checkbox("chengeThred", &CreateGimicTypeChenge);
		if (CreateGimicTypeChenge)
		{
			ImGui::Text("Notfilename");
		}
		else
		{

			ImGui::Text("Createfilenames");
			ImGui::InputInt("Createfilename", &ObjectfilenameIndex);
			if (ObjectfilenameIndex < 0)
			{
				ObjectfilenameIndex = 0;
			}
			else if (!name[ObjectfilenameIndex])
			{
				while (!name[ObjectfilenameIndex])
				{
					ObjectfilenameIndex--;
				}
			}
			ImGui::Text(name[ObjectfilenameIndex]->filename.c_str());
		}
		for (int i = 0; i < stage_count; i++)
		{
			Object* stage_ = GetStages(i);


			if (VMCFHT::instance().raycast(*stage_->GetModel()->Get_RaycastCollition(), stage_->GetTransform(), result_intersection))
			{

				if (GetAsyncKeyState(VK_RBUTTON) & 1)
				{

					if (GetCreateChain() != Chain_Type::null && Object_CreateFlag)
					{
						//result_intersection.intersection_position.y += 0.9f;
						if (CreateGimicTypeChenge)
						{
							Set_CreateLiftChain_Thred(GetCreateChain(), device, result_intersection, ID);
						}
						else
						{
							Set_CreateLiftChain_Thred(name[ObjectfilenameIndex]->filename.c_str(), GetCreateChain(), device, result_intersection, ID);

						}
						DeleteThred();
						break;
					}

				}
			}
			else
			{
				Debug_ParameterObj = nullptr;
			}

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