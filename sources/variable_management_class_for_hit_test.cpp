#include "variable_management_class_for_hit_test.h"
#include "scene_management.h"
#include <StageManager.h>
#include <objectManajer.h>


DirectX::XMFLOAT3 VMCFHT::convert_screen_to_world(LONG x/*screen*/, LONG y/*screen*/, FLOAT z/*ndc*/, D3D11_VIEWPORT vp, const DirectX::XMFLOAT4X4& view_projection)
{
    using namespace DirectX;
    XMFLOAT3 position;
    XMStoreFloat3(&position,
        XMVector3TransformCoord(
            XMVector3TransformCoord(
                XMVectorSet(static_cast<float>(x), static_cast<float>(y), z, 1),
                XMMatrixInverse(NULL,
                    XMMatrixSet(
                        vp.Width * 0.5f, 0.0f, 0.0f, 0.0f,
                        0.0f, -vp.Height * 0.5f, 0.0f, 0.0f,
                        0.0f, 0.0f, vp.MaxDepth - vp.MinDepth, 0.0f,
                        vp.TopLeftX + vp.Width * 0.5f, vp.Height * 0.5f + vp.TopLeftY, vp.MinDepth, 1.0f))
            ), XMMatrixInverse(NULL, XMLoadFloat4x4(&view_projection))
        )
    );
    return position;
}

//カメラ専用
void VMCFHT::update(DirectX::XMFLOAT4X4& view_projection, DirectX::XMFLOAT4& camera_position)
{
    DirectX::XMFLOAT2 cursor_pos = SceneManagement::instance().GetCursorPosition();

    DirectX::XMFLOAT3 position_on_near_plane = convert_screen_to_world(cursor_pos.x, cursor_pos.y, 0.0f, view_port, view_projection);

    DirectX::XMVECTOR P = XMLoadFloat4(&camera_position);
    DirectX::XMVECTOR D = DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&position_on_near_plane) - P);
    DirectX::XMStoreFloat3(&ray_position, P);
    DirectX::XMStoreFloat3(&ray_direction, D);
}

//カメラ以外
void VMCFHT::update(DirectX::XMFLOAT3& pos0, DirectX::XMFLOAT3& normal)
{
    DirectX::XMVECTOR P = DirectX::XMLoadFloat3(&pos0);
    DirectX::XMVECTOR D = DirectX::XMLoadFloat3(&normal);

    DirectX::XMVector3Normalize(D);

    DirectX::XMStoreFloat3(&ray_position, P);
    DirectX::XMStoreFloat3(&ray_direction, D);
}

bool VMCFHT::raycast(collision_mesh& collision_mesh, DirectX::XMFLOAT4X4 transform, Intersection& intersection, float length, bool skip)
{

    return collision_mesh.raycast(ray_position, ray_direction, transform, intersection, length, skip);
}

bool VMCFHT::RayCast(DirectX::XMFLOAT3 Start, DirectX::XMFLOAT3 End, HitResult& hit, Ray_ObjType type)
{

    StageManager& ince_st = StageManager::incetance();
    Objectmanajer& ince_o = Objectmanajer::incetance();
    int count = 0;
    switch (type)
    {
    case Ray_ObjType::Static_objects:
        count = ince_o.Get_GameStatic_ObjectCount();
        for (int i = 0; i < count; i++)
        {
            Static_Object* obj = ince_o.Get_GameStatic_Object(i);

            if (raycast(Start, End, obj->GetModel(), hit, obj->GetTransform()))
            {
                return true;
            }
        }

        break;
    case Ray_ObjType::DynamicGimics:
        count = ince_o.Get_GameGimicCount();
        for (int i = 0; i < count; i++)
        {
            Gimic* obj = ince_o.Get_GameGimic(i);
            if (obj->GetBootFlag())continue;
            if (obj->Raycast(Start, End, hit))
                return true;
        }
        break;
    case Ray_ObjType::DaynamicObjects:
        count = ince_o.Get_GameObjCount();
        for (int i = 0; i < count; i++)
        {
            Object* obj = ince_o.Get_GameObject(i);
            if (obj->Get_Original_Objtype(0) == ObjType::cution || obj->Get_Original_Objtype(0) == ObjType::Super_cution)continue;
            if (raycast(Start, End, obj->GetModel(), hit, obj->GetTransform()))
            {
                return true;
            }
        }

        break;
    case Ray_ObjType::Stage:
        count = ince_st.GetStageCount();
        for (int i = 0; i < count; i++)
        {
            Stage* st = ince_st.GetStages(i);
            if (raycast(Start, End, st->GetModel(), hit, st->GetTransform()))
            {
                return true;
            }
        }

        break;
    }

    return false;

}
bool VMCFHT::raycast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, const Model* model, HitResult& result, DirectX::XMFLOAT4X4 WorldTransform_, DirectX::XMMATRIX rotation_ /*回転だけ適用するための行列 */, bool rotation_flag/*rotation_をworld行列に適用するかのフラグ */)
{
    DirectX::XMVECTOR WorldStart = DirectX::XMLoadFloat3(&start);
    DirectX::XMVECTOR WorldEnd = DirectX::XMLoadFloat3(&end);
    DirectX::XMVECTOR WorldRayVec = DirectX::XMVectorSubtract(WorldEnd, WorldStart);
    DirectX::XMVECTOR WorldRayLength = DirectX::XMVector3Length(WorldRayVec);

    //ワールド空間のレイの長さ
    DirectX::XMStoreFloat(&result.distance, WorldRayLength);
    bool hit = false;
    const Model* resource = model;
    for (const Model::mesh& mesh : model->meshes)
    {

        //レイをワールド空間からローカル空間へ変換

        DirectX::XMMATRIX WorldTransform = XMMatrixMultiply(XMLoadFloat4x4(&mesh.default_global_transform), XMLoadFloat4x4(&WorldTransform_));
        if (rotation_flag)
        {
            WorldTransform = XMMatrixMultiply(WorldTransform,rotation_);
        }
        XMFLOAT4X4 w;
        DirectX::XMStoreFloat4x4(&w, WorldTransform);
        DirectX::XMMATRIX InverseWorldTransform = DirectX::XMMatrixInverse(nullptr, WorldTransform);

        DirectX::XMVECTOR S = DirectX::XMVector3TransformCoord(WorldStart, InverseWorldTransform);
        DirectX::XMVECTOR E = DirectX::XMVector3TransformCoord(WorldEnd, InverseWorldTransform);
        DirectX::XMVECTOR SE = DirectX::XMVectorSubtract(E, S);
        DirectX::XMVECTOR V = DirectX::XMVector3Normalize(SE);
        DirectX::XMVECTOR Length = DirectX::XMVector3Length(SE);

        //レイの長さ
        float neart;
        DirectX::XMStoreFloat(&neart, Length);

        //三角形の（面）との交差判定 
        const std::vector<Model::vertex>& vertices = mesh.vertices;
        const std::vector<UINT>indices = mesh.indices;

        int materialIndex = -1;
        DirectX::XMVECTOR HitPosition;
        DirectX::XMVECTOR HitNormal;
        for (const Model::mesh::subset& subset : mesh.subsets)
        {

            for (UINT i = 0; i < subset.index_count; i += 3)
            {
                UINT index = subset.start_index_location + i;
                //三角形の頂点を抽出
                const Model::vertex& a = vertices.at(indices.at(index));
                const Model::vertex& b = vertices.at(indices.at(index + 1));
                const Model::vertex& c = vertices.at(indices.at(index + 2));

#if 0 // おすすめできない
                //  a,b,cの頂点ごとのボーンの行列を取得
                DirectX::XMMATRIX skin_matrix[3] = DirectX::XMMatrixIdentity();
                skin_matrix[0] += a.bone_weights[0] * bind_pose_matrixes[a.bone_indices[0]]
                    + a.bone_weights[1] * bind_pose_matrixes[a.bone_indices[1]]
                    + a.bone_weights[2] * bind_pose_matrixes[a.bone_indices[2]]
                    + a.bone_weights[3] * bind_pose_matrixes[a.bone_indices[3]];
                //  b
                skin_matrix[1] += a.bone_weights[0] * bind_pose_matrixes[a.bone_indices[0]]
                    + a.bone_weights[1] * bind_pose_matrixes[a.bone_indices[1]]
                    + a.bone_weights[2] * bind_pose_matrixes[a.bone_indices[2]]
                    + a.bone_weights[3] * bind_pose_matrixes[a.bone_indices[3]];
                //  c
                skin_matrix[2] += a.bone_weights[0] * bind_pose_matrixes[a.bone_indices[0]]
                    + a.bone_weights[1] * bind_pose_matrixes[a.bone_indices[1]]
                    + a.bone_weights[2] * bind_pose_matrixes[a.bone_indices[2]]
                    + a.bone_weights[3] * bind_pose_matrixes[a.bone_indices[3]];
                a.position = mul(float4(a.position.xyz, 1), skin_matrix[0]);
                b.position = mul(float4(b.position.xyz, 1), skin_matrix[1]);
                c.position = mul(float4(c.position.xyz, 1), skin_matrix[2]);
#endif

                //三角形の三辺のベクトルを算出
                DirectX::XMVECTOR AB = DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&b.position), DirectX::XMLoadFloat3(&a.position));
                DirectX::XMVECTOR BC = DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&c.position), DirectX::XMLoadFloat3(&b.position));
                DirectX::XMVECTOR CA = DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&a.position), DirectX::XMLoadFloat3(&c.position));
                //三角形の法線ベクトルを算出
                DirectX::XMVECTOR N = DirectX::XMVector3Cross(AB, BC);

                //内積の結果がプラスならば裏向き
                DirectX::XMVECTOR Dot = DirectX::XMVector3Dot(V, N);
                float d;
                DirectX::XMStoreFloat(&d, Dot);
                if (d >= 0) continue;

                //レイと平面の交点を算出
                DirectX::XMVECTOR SA = DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&a.position), S);
                DirectX::XMVECTOR X = DirectX::XMVectorDivide(DirectX::XMVector3Dot(N, SA), Dot);
                float x;
                DirectX::XMStoreFloat(&x, X);
                if (x<.0f || x>neart)continue;//交点までの距離が今までに計算した最近距離より
                //大きいときはスキップ
                DirectX::XMVECTOR P = DirectX::XMVectorAdd(S, DirectX::XMVectorScale(V, x));
                //交点が三角形の内側にあるか判定
                DirectX::XMVECTOR PA = DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&a.position), P);
                DirectX::XMVECTOR Cross1 = DirectX::XMVector3Cross(PA, AB);
                DirectX::XMVECTOR Dot1 = DirectX::XMVector3Dot(Cross1, N);
                float dot1;
                DirectX::XMStoreFloat(&dot1, Dot1);
                if (dot1 < 0.0f)continue;

                DirectX::XMVECTOR PB = DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&b.position), P);
                DirectX::XMVECTOR Cross2 = DirectX::XMVector3Cross(PB, BC);
                DirectX::XMVECTOR Dot2 = DirectX::XMVector3Dot(Cross2, N);
                float dot2;
                DirectX::XMStoreFloat(&dot2, Dot2);
                if (dot2 < 0.0f)continue;

                DirectX::XMVECTOR PC = DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&c.position), P);
                DirectX::XMVECTOR Cross3 = DirectX::XMVector3Cross(PC, CA);
                DirectX::XMVECTOR Dot3 = DirectX::XMVector3Dot(Cross3, N);
                float dot3;
                DirectX::XMStoreFloat(&dot3, Dot3);
                if (dot3 < 0.0f)continue;

                //最近距離を更新
                neart = x;
                //交点と法線を更新
                HitPosition = P;
                HitNormal = N;
                materialIndex = subset.material_unique_id;
            }
        }

        if (materialIndex >= 0)
        {
            //ローカル空間からワールド空間への変換
            DirectX::XMVECTOR WorldPosition = DirectX::XMVector3TransformCoord(HitPosition,
                WorldTransform);
            DirectX::XMVECTOR WorldCrossVec = DirectX::XMVectorSubtract(WorldPosition, WorldStart);
            DirectX::XMVECTOR WorldCrossLength = DirectX::XMVector3Length(WorldCrossVec);
            float distance;
            DirectX::XMStoreFloat(&distance, WorldCrossLength);
            //ヒット情報保存
            if (result.distance > distance)
            {
                DirectX::XMVECTOR WorldNormal = DirectX::XMVector3TransformNormal(HitNormal, WorldTransform);

                result.distance = distance;
                result.materialIndex = materialIndex;
                DirectX::XMStoreFloat3(&result.position, WorldPosition);
                DirectX::XMStoreFloat3(&result.normal, DirectX::XMVector3Normalize(WorldNormal));
                hit = true;
            }
        }
    }




    return hit;
}

bool VMCFHT::moveStage_raycast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, const Model* model, HitResult& result, vector<Model::mesh>& meshes)
{
    DirectX::XMVECTOR WorldStart = DirectX::XMLoadFloat3(&start);
    DirectX::XMVECTOR WorldEnd = DirectX::XMLoadFloat3(&end);
    DirectX::XMVECTOR WorldRayVec = DirectX::XMVectorSubtract(WorldEnd, WorldStart);
    DirectX::XMVECTOR WorldRayLength = DirectX::XMVector3Length(WorldRayVec);

    //ワールド空間のレイの長さ
    DirectX::XMStoreFloat(&result.distance, WorldRayLength);
    bool hit = false;
  
    for (const Model::mesh& mesh : meshes)
    {

        //レイをワールド空間からローカル空間へ変換

        DirectX::XMMATRIX WorldTransform = DirectX::XMLoadFloat4x4(&mesh.default_global_transform);

        XMFLOAT4X4 w;
        DirectX::XMStoreFloat4x4(&w, WorldTransform);
        DirectX::XMMATRIX InverseWorldTransform = DirectX::XMMatrixInverse(nullptr, WorldTransform);

        DirectX::XMVECTOR S = DirectX::XMVector3TransformCoord(WorldStart, InverseWorldTransform);
        DirectX::XMVECTOR E = DirectX::XMVector3TransformCoord(WorldEnd, InverseWorldTransform);
        DirectX::XMVECTOR SE = DirectX::XMVectorSubtract(E, S);
        DirectX::XMVECTOR V = DirectX::XMVector3Normalize(SE);
        DirectX::XMVECTOR Length = DirectX::XMVector3Length(SE);

        //レイの長さ
        float neart;
        DirectX::XMStoreFloat(&neart, Length);

        //三角形の（面）との交差判定 
        const std::vector<Model::vertex>& vertices = mesh.vertices;
        const std::vector<UINT>indices = mesh.indices;

        int materialIndex = -1;
        DirectX::XMVECTOR HitPosition;
        DirectX::XMVECTOR HitNormal;
        for (const Model::mesh::subset& subset : mesh.subsets)
        {

            for (UINT i = 0; i < subset.index_count; i += 3)
            {
                UINT index = subset.start_index_location + i;
                //三角形の頂点を抽出
                const Model::vertex& a = vertices.at(indices.at(index));
                const Model::vertex& b = vertices.at(indices.at(index + 1));
                const Model::vertex& c = vertices.at(indices.at(index + 2));



                //三角形の三辺のベクトルを算出
                DirectX::XMVECTOR AB = DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&b.position), DirectX::XMLoadFloat3(&a.position));
                DirectX::XMVECTOR BC = DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&c.position), DirectX::XMLoadFloat3(&b.position));
                DirectX::XMVECTOR CA = DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&a.position), DirectX::XMLoadFloat3(&c.position));
                //三角形の法線ベクトルを算出
                DirectX::XMVECTOR N = DirectX::XMVector3Cross(AB, BC);

                //内積の結果がプラスならば裏向き
                DirectX::XMVECTOR Dot = DirectX::XMVector3Dot(V, N);
                float d;
                DirectX::XMStoreFloat(&d, Dot);
                if (d >= 0) continue;

                //レイと平面の交点を算出
                DirectX::XMVECTOR SA = DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&a.position), S);
                DirectX::XMVECTOR X = DirectX::XMVectorDivide(DirectX::XMVector3Dot(N, SA), Dot);
                float x;
                DirectX::XMStoreFloat(&x, X);
                if (x<.0f || x>neart)continue;//交点までの距離が今までに計算した最近距離より
                //大きいときはスキップ
                DirectX::XMVECTOR P = DirectX::XMVectorAdd(S, DirectX::XMVectorScale(V, x));
                //交点が三角形の内側にあるか判定
                DirectX::XMVECTOR PA = DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&a.position), P);
                DirectX::XMVECTOR Cross1 = DirectX::XMVector3Cross(PA, AB);
                DirectX::XMVECTOR Dot1 = DirectX::XMVector3Dot(Cross1, N);
                float dot1;
                DirectX::XMStoreFloat(&dot1, Dot1);
                if (dot1 < 0.0f)continue;

                DirectX::XMVECTOR PB = DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&b.position), P);
                DirectX::XMVECTOR Cross2 = DirectX::XMVector3Cross(PB, BC);
                DirectX::XMVECTOR Dot2 = DirectX::XMVector3Dot(Cross2, N);
                float dot2;
                DirectX::XMStoreFloat(&dot2, Dot2);
                if (dot2 < 0.0f)continue;

                DirectX::XMVECTOR PC = DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&c.position), P);
                DirectX::XMVECTOR Cross3 = DirectX::XMVector3Cross(PC, CA);
                DirectX::XMVECTOR Dot3 = DirectX::XMVector3Dot(Cross3, N);
                float dot3;
                DirectX::XMStoreFloat(&dot3, Dot3);
                if (dot3 < 0.0f)continue;

                //最近距離を更新
                neart = x;
                //交点と法線を更新
                HitPosition = P;
                HitNormal = N;
                materialIndex = subset.material_unique_id;
            }
        }

        if (materialIndex >= 0)
        {
            //ローカル空間からワールド空間への変換
            DirectX::XMVECTOR WorldPosition = DirectX::XMVector3TransformCoord(HitPosition,
                WorldTransform);
            DirectX::XMVECTOR WorldCrossVec = DirectX::XMVectorSubtract(WorldPosition, WorldStart);
            DirectX::XMVECTOR WorldCrossLength = DirectX::XMVector3Length(WorldCrossVec);
            float distance;
            DirectX::XMStoreFloat(&distance, WorldCrossLength);
            //ヒット情報保存
            if (result.distance > distance)
            {
                DirectX::XMVECTOR WorldNormal = DirectX::XMVector3TransformNormal(HitNormal, WorldTransform);

                result.distance = distance;
                result.materialIndex = materialIndex;
                DirectX::XMStoreFloat3(&result.position, WorldPosition);
                DirectX::XMStoreFloat3(&result.normal, DirectX::XMVector3Normalize(WorldNormal));
                hit = true;
            }
        }
    }




    return hit;
   
}

