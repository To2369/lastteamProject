#pragma once
#include"d3d11.h"
#include<vector>
#include"Gimic.h"
#include"Player.h"
#include"InvisibleBarria.h"
#include"chain.h"
class Objectmanajer
{
public:
    Objectmanajer() {}
    ~Objectmanajer() {}
    static Objectmanajer& incetance()
    {
        static Objectmanajer ince;
        return ince;
    }
    void Initialize(ObjType type_name, ID3D11Device* device, bool static_flag, XMFLOAT3 pos, Object::RayCastList raylist = {});
    void Initialize(const char* filename, ObjType type_name, ID3D11Device* device, bool static_flag, XMFLOAT3 pos = { 0,0,0 }, Object::RayCastList raylist = {});
    void Initialize(StageName s_name_, Gimic_Type type_name, ID3D11Device* device, XMFLOAT3 pos, std::string id = "null", XMFLOAT3 endpos = {});
    void Initialize(Chain_Type type, XMFLOAT3 pos, std::string id = "null", XMFLOAT3 angle = {});
    void Initialize(const char*filename,Chain_Type type, XMFLOAT3 pos, std::string id = "null", XMFLOAT3 angle = {});
    template <typename T>
    void Initialize_Static_Object(std::unique_ptr<T> obj)
    {
        Rigister_Static_Object(std::move(obj));
    }
    void Initialize_InvisibleBarria(ID3D11Device* device, DirectX::XMFLOAT3 pos,XMFLOAT3 Scale);
    void Update(float elapsedTime);
    void render(RenderContext* rc);

    void Clear();
    void Set_eraceObgect(Object& obj) {
        auto destroyPredicate = [&](const unique_ptr<Object>& element) {
            return element.get() == &obj && obj.GetDestroyObje();
            };
        auto it = remove_if(game_objs.begin(), game_objs.end(), destroyPredicate);
        game_objs.erase(it, game_objs.end());

    }
    void Set_eraceObgect(Gimic& obj) {
        auto destroyPredicate = [&](const unique_ptr<Gimic>& element) {
            return element.get() == &obj && obj.GetDestroyObje();
            };
        auto it = remove_if(game_Gimics.begin(), game_Gimics.end(), destroyPredicate);
        game_Gimics.erase(it, game_Gimics.end());

    }
    void Set_eraceObgect(Static_Object& obj) {
        auto destroyPredicate = [&](const unique_ptr<Static_Object>& element) {
            return element.get() == &obj && obj.GetDestroyObje();
            };
        auto it = remove_if(game_static_objes.begin(), game_static_objes.end(), destroyPredicate);
        game_static_objes.erase(it, game_static_objes.end());

    }
    void Set_eraceObgect(BaseChain& obj) {
        auto destroyPredicate = [&](const unique_ptr<BaseChain>& element) {
            return element.get() == &obj && obj.GetDestroy();
            };
        auto it = remove_if(game_lift_chains.begin(), game_lift_chains.end(), destroyPredicate);
        game_lift_chains.erase(it, game_lift_chains.end());

    }
    bool Sphere_VS_Player(const XMFLOAT3& plPos_,const float& pl_radius,
                          const XMFLOAT3& objPos,const float& obj_radius,XMFLOAT3&outPos);
    //この関数を使うときはそのオブジェクトがステージに存在してるのが1つの時だけ
    Gimic* Select_GetGimic(Gimic_Type type)
    {
        int count = static_cast<int>(game_Gimics.size());
        for (int i = 0; i < count; i++)
        {
            
            if (game_Gimics[i].get()->Get_GimicType() == type)
            {
               
                return game_Gimics[i].get();

            }
        }
        return nullptr;
    };
    void Rigister_obj(std::unique_ptr<Object> obj) { game_objs.push_back(std::move(obj)); }
    void Rigister_Gimic(std::unique_ptr<Gimic> obj) { game_Gimics.push_back(std::move(obj)); }
    void Rigister_Static_Object(std::unique_ptr<Static_Object> obj) { game_static_objes.push_back(std::move(obj));}
    void Rigister_Lift_Chains(std::unique_ptr<BaseChain> obj) { game_lift_chains.push_back(std::move(obj)); }
    int Get_GameObjCount() { return static_cast<int>(game_objs.size()); }
    int Get_GameGimicCount() { return static_cast<int>(game_Gimics.size()); }
    int Get_GameStatic_ObjectCount() { return static_cast<int>(game_static_objes.size()); }
    int Get_GameLiftChainCount() { return (int)game_lift_chains.size(); }
    Object* Get_GameObject(int i) { return game_objs[i].get(); }
    Gimic* Get_GameGimic(int i) { return game_Gimics[i].get(); }
    Static_Object* Get_GameStatic_Object(int i) { return game_static_objes[i].get(); }
    BaseChain* Get_GameLiftChain(int i) { return game_lift_chains[i].get(); }
    vector<unique_ptr<BaseChain>>Get_GameLiftChains() { return move(game_lift_chains); }
    void Gui(ID3D11Device* device);
    /// <summary>
  /// 
  /// </summary>
  /// <param name="thisobj">自分がgimicの上に乗ってるかチェックを行うオブジェクト</param>
  /// <param name="gimic"></param>
  /// <param name="UpcheckFlag">これがfalseなら普通のBoundingBox同士の当たり判定になる</param>
  /// <param name="offset">調整用</param>
  /// <returns></returns>
    bool Bounding_Box_vs_Bounding_Box(Object* obj, Object* gimic_obj, bool UpcheckFlag = false, float offset = 0.0f);
    //上記とは別物
    bool Bounding_Box_vs_Bounding_Box(Player* player_, Static_Object* obj2);
    bool Bounding_Box_vs_Bounding_Box(DirectX::XMFLOAT3 PL_position,//playerPOs
        DirectX::XMFLOAT3 OB_position,//objectPos
        DirectX::XMFLOAT3 PL_scale,//playerScale
        DirectX::XMFLOAT3 OB_scale,//objectScale
        DirectX::XMFLOAT3* Pl_boundingbox,//playerboundingbox
        DirectX::XMFLOAT3* OB_boundingbox);//objectboundingbox
private:

    vector<unique_ptr<Object>> game_objs;
    vector<unique_ptr<Gimic>>  game_Gimics;
    vector<unique_ptr<Static_Object>>  game_static_objes;
    vector<unique_ptr<BaseChain>> game_lift_chains;


public:

};