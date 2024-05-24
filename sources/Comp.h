#pragma once
#include"DirectXMath.h"
#include"d3d11.h"
#include"memory"
#include"Model.h"
#include"Graphics/graphics.h"
class object;
class Comp
{
    friend class object;
public:
    Comp() { pSelf = nullptr; };
    virtual ~Comp() { };
public:
    // virtual void Update(float elapsedTime) {};
     //void BaseGui();
      
    __forceinline	object* GetGameObject() { return pSelf; }
private:
    object* pSelf;
    
};
class object
{
public:

    object() {};
    ~object()
    {
        for (Comp* comp : pComponents) {
            delete comp;
        }
        // リストをクリア
        pComponents.clear();
    }
    /*
        GetComponent
        コンポーネントを継承した物を取得する　（何とかコンポーネントのインスタンスを取れる）
        TypeComponent	: ダウンキャスト型
        コンポーネントポインタを返却
    */
    template<typename	TypeComponent>
    TypeComponent* GetComponent()
    {
        TypeComponent* pPointer = NULL;
        for (auto& rrComponent : pComponents)
        {
            //登録されてる型pComponentsが戻り値の型に変えれるかどうか
            pPointer = dynamic_cast<TypeComponent*>(rrComponent);
            if (pPointer)
                break;
        }
        return pPointer;
    }
    /*!
       AddComponent
       コンポーネントを継承した物を追加する
       現状は処理をわかりやすくするため、
       同じ型のコンポーネントは1つだけしか追加できないように制限を加えている。
       TypeComponent	: 追加したいコンポーネントを継承した型
       Arguments		: 可変長引数型種
       _pArgs	: 可変長引数
       追加されたコンポーネントポインタを返却
   **/
    template<typename	TypeComponent, typename ... Arguments>
    TypeComponent* AddComponent(Arguments ... _pArgs)
    {
        //同じ型が存在してるか確認し、もしあったならnullptrを返す
        if (GetComponent<TypeComponent>())
        {
            return	nullptr;
        }
        // std::forward を使って、_pArgs で受け取った引数をそのままTypeComponentに転送し、
        // 引数の種類（左値または右値）を維持しながら展開しています。
        TypeComponent* pTypeComponent = new TypeComponent(std::forward<Arguments>(_pArgs)...);
        AddComponent(pTypeComponent);
        return	pTypeComponent;
    }

private:
    void AddComponent(Comp* _pComponent)
    {

        _pComponent->pSelf = this;
        pComponents.push_back(_pComponent);
    }
private:
    std::list<Comp*>	pComponents;
};


class TransformComp:public Comp
{
private:
    
    DirectX::XMFLOAT3 Position{};
    DirectX::XMFLOAT3 Scale{1,1,1};
    DirectX::XMFLOAT3 Angle{};
    DirectX::XMFLOAT4X4 transform{ 1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1 };

public:
    TransformComp() {};
    ~TransformComp() override {};
    void UpdateTransform()
    {
        const DirectX::XMFLOAT4X4 coordinate_system_transforms[]{
     { -1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 },	// 0:RHS Y-UP
     { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 },		// 1:LHS Y-UP
     { -1, 0, 0, 0, 0, 0, -1, 0, 0, 1, 0, 0, 0, 0, 0, 1 },	// 2:RHS Z-UP
     { 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1 },		// 3:LHS Z-UP
        };
        const float scale_factor = 0.01f;
        DirectX::XMMATRIX C{ DirectX::XMLoadFloat4x4(&coordinate_system_transforms[0]) * DirectX::XMMatrixScaling(scale_factor, scale_factor, scale_factor) };

        //スケール行列を作成
        XMMATRIX S = XMMatrixScaling(Scale.x, Scale.y, Scale.z);
        //回転行列作成
        /*XMMATRIX X = XMMatrixRotationX(Angle.x);
        XMMATRIX Y = XMMatrixRotationY(Angle.y);
        XMMATRIX Z = XMMatrixRotationZ(Angle.z);*/
        XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(Angle.x, Angle.y, Angle.z);
        //位置行列を作成
        XMMATRIX T = XMMatrixTranslation(Position.x, Position.y, Position.z);
        //4つの行列を組み合わせて、ワールド座標を作成
        DirectX::XMMATRIX W = C * S * R * T;
        //計算したワールド座標を取り出す
        DirectX::XMStoreFloat4x4(&transform, W);
    }
public:
    void AddPosition(DirectX::XMFLOAT3 pos) {
            Position.x += pos.x;
            Position.y += pos.y;
            Position.z += pos.z;
    };
    void AddScale(DirectX::XMFLOAT3 scale) {
       Scale.x += scale.x;
       Scale.y += scale.y;
       Scale.z += scale.z;
    };
    void AddAngle(DirectX::XMFLOAT3 angle)
    {
        Angle.x += angle.x;
        Angle.y += angle.y;
        Angle.z += angle.z;
    };
public://set
    void SetScale(DirectX::XMFLOAT3 scale)
    {
        Scale = scale;
    }
    void SetPosition(DirectX::XMFLOAT3 position)
    {
        Position = position;
    }
    void SetAngle(DirectX::XMFLOAT3 angle)
    {
        Angle = angle;
    }

public://get
     DirectX::XMFLOAT4X4 Gettransform() { return transform; }
     DirectX::XMFLOAT3 Getscale() { return Scale; }
     DirectX::XMFLOAT3 GetPosition() { return Position; }
     DirectX::XMFLOAT3 GetAngle() { return Angle; }
};
class RenderComp:public Comp
{
  
public:
    RenderComp(const char* filename,Graphics*gr)
    {
        model = make_unique<Model>(gr->GetDevice(),filename,true);
    };
    RenderComp(const char* filename, Graphics* gr,vector<string>&filenames)
    {
        model = make_unique<Model>(gr->GetDevice(), filename, filenames, true);
        anim.animation_clips = model->animation_clips;
        anim.scene_view = model->GetScene_View();
    };
    RenderComp() {};
    ~RenderComp() override{};
    struct Animation
    {
        ~Animation()
        {
            if (keyfreame)
            {
                delete keyfreame;
                keyfreame = nullptr;
            }
        }
        animation::keyframe* getKeyFreame(float elapsedTime,bool loop)
        {
            animation::keyframe* keyframe = nullptr;
            if (animation_clips.data())
            {
                if (animation_clips.size() > 0)
                {
                    int clip_index{ 0 };
                    int frame_index{ 0 };
                   

                    animation& animation{ animation_clips.at(clip_index) };
                    frame_index = static_cast<int>(animation_tick * animation.sampling_rate);
                    if (frame_index < 0)
                    {
                        animationStart = true;
                        frame_index = 0;
                        animation_tick = 0;
                        
                    }
                    else
                    if (frame_index > animation.sequence.size() - 1)
                    {
                        if (loop)
                        {
                            animationEnd = true;
                            frame_index = 0;
                            animation_tick = 0;
                      
                        }
                        else
                        {
                            animationEnd = true;
                            frame_index = (int)animation.sequence.size() - 1;
                            animation_tick = (animation.sequence.size() - 1)/ animation.sampling_rate;
                        }

                       
                    }
                    else
                    {
                      
                        if (!StopAnimation())
                            animation_tick += elapsedTime;
                    }
                    keyframe = &animation.sequence.at(frame_index);

                }
                return keyframe;

            }
            else
                return nullptr;
        }
        void update_animation(animation::keyframe& keyframe)
        {
            const size_t node_count{ keyframe.nodes.size() };
            for (size_t node_index = 0; node_index < node_count; ++node_index)
            {
                animation::keyframe::node& node{ keyframe.nodes.at(node_index) };
                XMMATRIX S{ XMMatrixScaling(node.scaling.x, node.scaling.y, node.scaling.z) };
                XMMATRIX R{ XMMatrixRotationQuaternion(XMLoadFloat4(&node.rotation)) };
                XMMATRIX T{ XMMatrixTranslation(node.translation.x, node.translation.y, node.translation.z) };

                const int64_t parent_index{ scene_view.nodes.at(node_index).parent_index };
                XMMATRIX P{ parent_index < 0 ? XMMatrixIdentity() : XMLoadFloat4x4(&keyframe.nodes.at(parent_index).global_transform) };

                XMStoreFloat4x4(&node.global_transform, S * R * T * P);
            }
        }
        bool StopAnimation()
        {
            return stop_animation;
        }
        scene scene_view;
        bool stop_animation = false;//
        bool LoopAnimation = true;
        bool animationStart = false;//animationの開始位置
        bool animationEnd = false;//animationの終端
        animation::keyframe* keyfreame = nullptr;
        std::vector<animation>animation_clips;
        float animSpeed = 0;
        float animation_tick{ 0 };
     
    };
     Animation anim;
     XMFLOAT4 Color{ 1,1,1,1 };
    void render(Graphics& gr, DirectX::XMFLOAT4X4 transform)
    {
        
        if (Color.w > 0)
            model->render(gr.GetDeviceContext(), transform, anim.animSpeed, Color);
       
           
    }
    Model* GetModel() { return model.get(); }
    void RenderCompGui();
 
private:
    std::unique_ptr<Model>model;
   

};



#if 0

class Comp
{
public:
    Comp() {};
    virtual ~Comp() {};
public:
    virtual void Update(float elapsedTime) {};
    void Render();
    void BaseGui();
    virtual void Gui() = 0;
private:

    void AddCompnent(std::map<string, unique_ptr<Comp>> Component);
    std::map<string, unique_ptr<Comp>>Conponent;
};


#endif