#pragma once
#include"d3d11.h"
#include<vector>
#include"Gimic.h"
#include"Player.h"
#include"InvisibleBarria.h"

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
    void Initialize(StageName s_name_, ObjType type_name, ID3D11Device* device, XMFLOAT3 pos = { 0,0,0 });
    void Initialize(StageName s_name_, Gimic_Type type_name, ID3D11Device* device, XMFLOAT3 pos, std::string id = "null");
    template <typename T>
    void Initialize_Static_Object(std::unique_ptr<T> obj)
    {
        Rigister_Static_Object(std::move(obj));
    }
    void Initialize_InvisibleBarria(ID3D11Device* device, DirectX::XMFLOAT3 pos);
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
    //この関数を使うときはそのオブジェクトがステージに存在してるのが1つの時だけ
    Gimic* Select_GetGimic(Gimic_Type type)
    {
        int count = static_cast<int>(game_Gimics.size());
        Gimic* gi = nullptr;
        int type_count = 0;
        for (int i = 0; i < count; i++)
        {
            gi = game_Gimics[i].get();
            if (gi->Get_GimicType() == type)
            {
                type_count++;
                if (type_count >= 2)return nullptr;

            }
        }
        return gi;
    };
    void Rigister_obj(std::unique_ptr<Object> obj) { game_objs.push_back(std::move(obj)); }
    void Rigister_Gimic(std::unique_ptr<Gimic> obj) { game_Gimics.push_back(std::move(obj)); }
    void Rigister_Static_Object(std::unique_ptr<Static_Object> obj) { game_static_objes.push_back(std::move(obj)); }
    int Get_GameObjCount() { return static_cast<int>(game_objs.size()); }
    int Get_GameGimicCount() { return static_cast<int>(game_Gimics.size()); }
    int Get_GameStatic_ObjectCount() { return static_cast<int>(game_static_objes.size()); }
    Object* Get_GameObject(int i) { return game_objs[i].get(); }
    Gimic* Get_GameGimic(int i) { return game_Gimics[i].get(); }
    Static_Object* Get_GameStatic_Object(int i) { return game_static_objes[i].get(); }

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



public:

};