#pragma once
#include"DirectXMath.h"
#include"d3d11.h"
#include"memory"
#include"Model.h"
#include"Graphics/graphics.h"

class TransformComp
{
private:
    DirectX::XMFLOAT3 Position{};
    DirectX::XMFLOAT3 Scale{1,1,1};
    DirectX::XMFLOAT3 Angle{};
    DirectX::XMFLOAT4X4 transform{ 1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1 };

public:
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

        //�X�P�[���s����쐬
        XMMATRIX S = XMMatrixScaling(Scale.x, Scale.y, Scale.z);
        //��]�s��쐬
        /*XMMATRIX X = XMMatrixRotationX(Angle.x);
        XMMATRIX Y = XMMatrixRotationY(Angle.y);
        XMMATRIX Z = XMMatrixRotationZ(Angle.z);*/
        XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(Angle.x, Angle.y, Angle.z);
        //�ʒu�s����쐬
        XMMATRIX T = XMMatrixTranslation(Position.x, Position.y, Position.z);
        //4�̍s���g�ݍ��킹�āA���[���h���W���쐬
        DirectX::XMMATRIX W = C * S * R * T;
        //�v�Z�������[���h���W�����o��
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
public://set
    void SetScale(DirectX::XMFLOAT3 scale)
    {
        Scale = scale;
    }
    void SetPosition(DirectX::XMFLOAT3 position)
    {
        Position = position;
    }

public://get
     DirectX::XMFLOAT4X4 Gettransform() { return transform; }
     DirectX::XMFLOAT3 Getscale() { return Scale; }
     DirectX::XMFLOAT3 GetPosition() { return Position; }
};
class RenderComp
{
  
public:
    RenderComp(const char* filename,Graphics&gr)
    {
        model = make_unique<Model>(gr.GetDevice(),filename,true);
    };
    RenderComp(const char* filename, Graphics& gr,vector<string>&filenames)
    {
        model = make_unique<Model>(gr.GetDevice(), filename,filenames, true);

        anim.animation_clips = model->animation_clips;
        anim.scene_view = model->GetScene_View();
    };
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
            animationEnd = false;
            if (animation_clips.data())
            {
                if (animation_clips.size() > 0)
                {
                    int clip_index{ 0 };
                    int frame_index{ 0 };
                    static float animation_tick{ 0 };

                    animation& animation{ animation_clips.at(clip_index) };
                    frame_index = static_cast<int>(animation_tick * animation.sampling_rate);
                    if (frame_index < 0)
                    {
                        animationEnd = true;
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
                            frame_index = animation.sequence.size() - 1;
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
        bool animationEnd = false;//animation�̏I�[
        animation::keyframe* keyfreame = nullptr;
        std::vector<animation>animation_clips;
        float animSpeed = 0;
       
     
    };
     Animation anim;
     XMFLOAT4 Color{ 1,1,1,1 };
    void render(Graphics& gr, DirectX::XMFLOAT4X4 transform)
    {
        
        if(Color.w<=0)
            gr.GetDeviceContext()->OMSetBlendState(gr.GetBlendState(1), nullptr, 0xFFFFFFFF);
        else
            gr.GetDeviceContext()->OMSetBlendState(gr.GetBlendState(2), nullptr, 0xFFFFFFFF);
        model->render(gr.GetDeviceContext(), transform, anim.animSpeed, Color);
        gr.GetDeviceContext()->OMSetBlendState(gr.GetBlendState(2), nullptr, 0xFFFFFFFF);
    }
    Model* GetModel() { return model.get(); }
    void RenderCompGui();
 
private:
    std::unique_ptr<Model>model;
   

};

class Comp
{
public:
    Comp() {};
  virtual ~Comp() {};
public:
    virtual void Update(float elapsedTime) {};
    void Render();
    void BaseGui();
    virtual void Gui()=0;
    TransformComp* GetTransformComp() { return transform.get();}
    RenderComp* GetRenderComp() { return render.get();}
protected:
    std::unique_ptr<TransformComp> transform;//�ʒu�A�X�P�[���A��]�A���[���h���W
    std::unique_ptr<RenderComp> render;//�`��n
};

