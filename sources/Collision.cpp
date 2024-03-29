#include"Collision.h"
#include<vector>

bool Collision::intersectRayAndModel(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, const std::unique_ptr<Model>& model, HitResult& result)
{
	// ワールド空間上でのレイの始点
	DirectX::XMVECTOR worldRayStartVec = DirectX::XMLoadFloat3(&start);
	// ワールド空間上でのレイの終点
	DirectX::XMVECTOR worldRayEndVec = DirectX::XMLoadFloat3(&end);
	// ワールド空間上でのレイの始点から終点までのベクトル
	DirectX::XMVECTOR worldRayVec = DirectX::XMVectorSubtract(worldRayEndVec, worldRayStartVec);
	// ワールド空間上でのレイの長さ
	DirectX::XMVECTOR worldRayLength = DirectX::XMVector3Length(worldRayVec);
	DirectX::XMStoreFloat(&result.distance, worldRayLength);

	//true...衝突した
	bool hit = false;
	for (const Model::mesh& mesh : model->meshes)
	{
		//レイをワールド空間からローカル空間に変換
		DirectX::XMMATRIX worldTransformMat = DirectX::XMLoadFloat4x4(&mesh.default_global_transform);
		DirectX::XMMATRIX inverseWorldTransformMat = DirectX::XMMatrixInverse(nullptr, worldTransformMat);
	
		// ローカル空間上でのレイの始点
		DirectX::XMVECTOR localRayStartVec = DirectX::XMVector3TransformCoord(worldRayStartVec, inverseWorldTransformMat);
		// ローカル空間上でのレイの終点
		DirectX::XMVECTOR localRayEndVec = DirectX::XMVector3TransformCoord(worldRayEndVec, inverseWorldTransformMat);
		// ローカル空間上でのレイの始点から終点までのベクトル
		DirectX::XMVECTOR localRayVec = DirectX::XMVectorSubtract(localRayEndVec, localRayStartVec);
		// ローカル空間上でのレイの方向ベクトル
		DirectX::XMVECTOR localRayDirectVec = DirectX::XMVector3Normalize(localRayVec);
		// ローカル空間上でのレイの長さ
		DirectX::XMVECTOR localRayLengthVec = DirectX::XMVector3Length(localRayVec);
		float localRayLength;
		DirectX::XMStoreFloat(&localRayLength, localRayLengthVec);
	
		//頂点データを取得
		const std::vector<Model::vertex>& vertices = mesh.vertices;
		const std::vector<UINT>& indices = mesh.indices;

		//候補となる情報
		int materialIndex = -1;
		DirectX::XMVECTOR hitPosition;
		DirectX::XMVECTOR hitNormal;

		for (const Model::mesh::subset& subset : mesh.subsets)
		{
			for (UINT i = 0; i < subset.index_count; i += 3)//i += 3)
			{
				//UINT index = subset.start_index_location + i;

				////三角形の頂点抽出
				//const Model::vertex& a = vertices.at(indices.at(index));
				//const Model::vertex& b = vertices.at(indices.at(index + 1));
				//const Model::vertex& c = vertices.at(indices.at(index + 2));

				//DirectX::XMVECTOR aVec = DirectX::XMLoadFloat3(&a.position);
				//DirectX::XMVECTOR bVec = DirectX::XMLoadFloat3(&b.position);
				//DirectX::XMVECTOR cVec = DirectX::XMLoadFloat3(&c.position);

				//DirectX::XMVECTOR abVec = DirectX::XMVectorSubtract(bVec, aVec);
				//DirectX::XMVECTOR bcVec = DirectX::XMVectorSubtract(cVec, bVec);
				//DirectX::XMVECTOR caVec = DirectX::XMVectorSubtract(aVec, cVec);
			
				////三角形の法線を計算
				//DirectX::XMVECTOR normalVec = DirectX::XMVector3Cross(abVec, bcVec);

				////レイの方向と面の方向をチェック(平面の表裏判定)
				//DirectX::XMVECTOR dotVec = DirectX::XMVector3Dot(localRayDirectVec, normalVec);
				//float dot;
				//DirectX::XMStoreFloat(&dot, dotVec);
				//if (dot >= 0)
				//	continue;

				////レイの始点から交点までの長さを計算
				//DirectX::XMVECTOR tmp = DirectX::XMVectorSubtract(aVec, localRayStartVec);
				//float length = DirectX::XMVectorGetX(DirectX::XMVector3Dot(tmp, normalVec)) / dot;

				////長さが 0 以下なら以降の処理は飛ばす
				//if (length < 0.0f)
				//	continue;

				////交点までの距離が今までに計算した最近距離より大きい時はスキップ
				//if (length > localRayLength)
				//	continue;

				////レイと平面の交点を計算
				//DirectX::XMVECTOR hitPosVec = DirectX::XMVectorAdd(localRayStartVec, DirectX::XMVectorScale(localRayDirectVec, length));
			
				////交点が三角形の内側にあるかどうかを判定

				////交点から点Aまでのベクトル
				//DirectX::XMVECTOR paVec = DirectX::XMVectorSubtract(aVec, hitPosVec);
				////交点から点Aまでのベクトルと点A点Bのベクトル外積
				//DirectX::XMVECTOR crossVec1 = DirectX::XMVector3Cross(paVec, abVec);
				////外積と法線の向きをチェック
				//DirectX::XMVECTOR dotVec1 = DirectX::XMVector3Dot(crossVec1, normalVec);
				//float dot1;
				//DirectX::XMStoreFloat(&dot1, dotVec1);
				//if (dot1 < 0.0f)
				//	continue;

				////交点から点Bまでのベクトル
				//DirectX::XMVECTOR pbVec = DirectX::XMVectorSubtract(bVec, hitPosVec);
				////交点から点Bまでのベクトル
				//DirectX::XMVECTOR crossVec2 = DirectX::XMVector3Cross(pbVec, bcVec);
				////外積と法線の向きチェック
				//DirectX::XMVECTOR dotVec2 = DirectX::XMVector3Dot(crossVec2, normalVec);
				//float dot2;
				//DirectX::XMStoreFloat(&dot2, dotVec2);
				//if (dot2 < 0.0f)
				//{
				//	continue;
				//}

				////交点から点Cまでのベクトル
				//DirectX::XMVECTOR pcVec = DirectX::XMVectorSubtract(cVec, hitPosVec);
				////交点から点Cまでのベクトルと点C点Aのベクトルの外積
				//DirectX::XMVECTOR crossVec3 = DirectX::XMVector3Cross(pcVec, caVec);
				////外積と法線の向きをチェック
				//DirectX::XMVECTOR dotVec3 = DirectX::XMVector3Dot(crossVec3, normalVec);
				//float dot3;
				//DirectX::XMStoreFloat(&dot3, dotVec3);
				//if (dot3 < 0.0f)
				//{
				//	continue;
				//}

				////最近距離を更新
				//localRayLength = length;

				////マテリアル番号を更新
				//materialIndex = subset.material_unique_id;

				////交点と法線を更新
				//hitPosition = hitPosVec;
				//hitNormal = normalVec;
			}
		}

		if (materialIndex >= 0)
		{
			//交点座標をローカル空間からワールド空間へ変換
			DirectX::XMVECTOR worldPositionVec = DirectX::XMVector3TransformCoord(hitPosition, worldTransformMat);
			//ワールド空間上でのレイの始点から交点までのベクトル
			DirectX::XMVECTOR worldVec = DirectX::XMVectorSubtract(worldPositionVec, worldRayStartVec);
			//ワールド空間上でのレイの視点から交点までの長さ
			DirectX::XMVECTOR worldLengthVec = DirectX::XMVector3Length(worldVec);
			float distance;
			DirectX::XMStoreFloat(&distance, worldLengthVec);

			//ヒット結果情報を保存
			if (result.distance > distance)
			{
				//ヒット時の面の法線をローカル空間からワールド空間へ変換
				DirectX::XMVECTOR worldNormal = DirectX::XMVector3TransformNormal(hitNormal, worldTransformMat);

				result.distance = distance;
				result.materialIndex = materialIndex;
				DirectX::XMStoreFloat3(&result.position, worldPositionVec);
				DirectX::XMStoreFloat3(&result.normal, DirectX::XMVector3Normalize(worldNormal));
				hit = true;
			}
		}
	}
	return hit;
}