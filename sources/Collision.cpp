#include"Collision.h"
#include<vector>

bool Collision::intersectRayAndModel(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, const std::unique_ptr<Model>& model, HitResult& result)
{
	// ���[���h��ԏ�ł̃��C�̎n�_
	DirectX::XMVECTOR worldRayStartVec = DirectX::XMLoadFloat3(&start);
	// ���[���h��ԏ�ł̃��C�̏I�_
	DirectX::XMVECTOR worldRayEndVec = DirectX::XMLoadFloat3(&end);
	// ���[���h��ԏ�ł̃��C�̎n�_����I�_�܂ł̃x�N�g��
	DirectX::XMVECTOR worldRayVec = DirectX::XMVectorSubtract(worldRayEndVec, worldRayStartVec);
	// ���[���h��ԏ�ł̃��C�̒���
	DirectX::XMVECTOR worldRayLength = DirectX::XMVector3Length(worldRayVec);
	DirectX::XMStoreFloat(&result.distance, worldRayLength);

	//true...�Փ˂���
	bool hit = false;
	for (const Model::mesh& mesh : model->meshes)
	{
		//���C�����[���h��Ԃ��烍�[�J����Ԃɕϊ�
		DirectX::XMMATRIX worldTransformMat = DirectX::XMLoadFloat4x4(&mesh.default_global_transform);
		DirectX::XMMATRIX inverseWorldTransformMat = DirectX::XMMatrixInverse(nullptr, worldTransformMat);
	
		// ���[�J����ԏ�ł̃��C�̎n�_
		DirectX::XMVECTOR localRayStartVec = DirectX::XMVector3TransformCoord(worldRayStartVec, inverseWorldTransformMat);
		// ���[�J����ԏ�ł̃��C�̏I�_
		DirectX::XMVECTOR localRayEndVec = DirectX::XMVector3TransformCoord(worldRayEndVec, inverseWorldTransformMat);
		// ���[�J����ԏ�ł̃��C�̎n�_����I�_�܂ł̃x�N�g��
		DirectX::XMVECTOR localRayVec = DirectX::XMVectorSubtract(localRayEndVec, localRayStartVec);
		// ���[�J����ԏ�ł̃��C�̕����x�N�g��
		DirectX::XMVECTOR localRayDirectVec = DirectX::XMVector3Normalize(localRayVec);
		// ���[�J����ԏ�ł̃��C�̒���
		DirectX::XMVECTOR localRayLengthVec = DirectX::XMVector3Length(localRayVec);
		float localRayLength;
		DirectX::XMStoreFloat(&localRayLength, localRayLengthVec);
	
		//���_�f�[�^���擾
		const std::vector<Model::vertex>& vertices = mesh.vertices;
		const std::vector<UINT>& indices = mesh.indices;

		//���ƂȂ���
		int materialIndex = -1;
		DirectX::XMVECTOR hitPosition;
		DirectX::XMVECTOR hitNormal;

		for (const Model::mesh::subset& subset : mesh.subsets)
		{
			for (UINT i = 0; i < subset.index_count; i += 3)//i += 3)
			{
				//UINT index = subset.start_index_location + i;

				////�O�p�`�̒��_���o
				//const Model::vertex& a = vertices.at(indices.at(index));
				//const Model::vertex& b = vertices.at(indices.at(index + 1));
				//const Model::vertex& c = vertices.at(indices.at(index + 2));

				//DirectX::XMVECTOR aVec = DirectX::XMLoadFloat3(&a.position);
				//DirectX::XMVECTOR bVec = DirectX::XMLoadFloat3(&b.position);
				//DirectX::XMVECTOR cVec = DirectX::XMLoadFloat3(&c.position);

				//DirectX::XMVECTOR abVec = DirectX::XMVectorSubtract(bVec, aVec);
				//DirectX::XMVECTOR bcVec = DirectX::XMVectorSubtract(cVec, bVec);
				//DirectX::XMVECTOR caVec = DirectX::XMVectorSubtract(aVec, cVec);
			
				////�O�p�`�̖@�����v�Z
				//DirectX::XMVECTOR normalVec = DirectX::XMVector3Cross(abVec, bcVec);

				////���C�̕����Ɩʂ̕������`�F�b�N(���ʂ̕\������)
				//DirectX::XMVECTOR dotVec = DirectX::XMVector3Dot(localRayDirectVec, normalVec);
				//float dot;
				//DirectX::XMStoreFloat(&dot, dotVec);
				//if (dot >= 0)
				//	continue;

				////���C�̎n�_�����_�܂ł̒������v�Z
				//DirectX::XMVECTOR tmp = DirectX::XMVectorSubtract(aVec, localRayStartVec);
				//float length = DirectX::XMVectorGetX(DirectX::XMVector3Dot(tmp, normalVec)) / dot;

				////������ 0 �ȉ��Ȃ�ȍ~�̏����͔�΂�
				//if (length < 0.0f)
				//	continue;

				////��_�܂ł̋��������܂łɌv�Z�����ŋߋ������傫�����̓X�L�b�v
				//if (length > localRayLength)
				//	continue;

				////���C�ƕ��ʂ̌�_���v�Z
				//DirectX::XMVECTOR hitPosVec = DirectX::XMVectorAdd(localRayStartVec, DirectX::XMVectorScale(localRayDirectVec, length));
			
				////��_���O�p�`�̓����ɂ��邩�ǂ����𔻒�

				////��_����_A�܂ł̃x�N�g��
				//DirectX::XMVECTOR paVec = DirectX::XMVectorSubtract(aVec, hitPosVec);
				////��_����_A�܂ł̃x�N�g���Ɠ_A�_B�̃x�N�g���O��
				//DirectX::XMVECTOR crossVec1 = DirectX::XMVector3Cross(paVec, abVec);
				////�O�ςƖ@���̌������`�F�b�N
				//DirectX::XMVECTOR dotVec1 = DirectX::XMVector3Dot(crossVec1, normalVec);
				//float dot1;
				//DirectX::XMStoreFloat(&dot1, dotVec1);
				//if (dot1 < 0.0f)
				//	continue;

				////��_����_B�܂ł̃x�N�g��
				//DirectX::XMVECTOR pbVec = DirectX::XMVectorSubtract(bVec, hitPosVec);
				////��_����_B�܂ł̃x�N�g��
				//DirectX::XMVECTOR crossVec2 = DirectX::XMVector3Cross(pbVec, bcVec);
				////�O�ςƖ@���̌����`�F�b�N
				//DirectX::XMVECTOR dotVec2 = DirectX::XMVector3Dot(crossVec2, normalVec);
				//float dot2;
				//DirectX::XMStoreFloat(&dot2, dotVec2);
				//if (dot2 < 0.0f)
				//{
				//	continue;
				//}

				////��_����_C�܂ł̃x�N�g��
				//DirectX::XMVECTOR pcVec = DirectX::XMVectorSubtract(cVec, hitPosVec);
				////��_����_C�܂ł̃x�N�g���Ɠ_C�_A�̃x�N�g���̊O��
				//DirectX::XMVECTOR crossVec3 = DirectX::XMVector3Cross(pcVec, caVec);
				////�O�ςƖ@���̌������`�F�b�N
				//DirectX::XMVECTOR dotVec3 = DirectX::XMVector3Dot(crossVec3, normalVec);
				//float dot3;
				//DirectX::XMStoreFloat(&dot3, dotVec3);
				//if (dot3 < 0.0f)
				//{
				//	continue;
				//}

				////�ŋߋ������X�V
				//localRayLength = length;

				////�}�e���A���ԍ����X�V
				//materialIndex = subset.material_unique_id;

				////��_�Ɩ@�����X�V
				//hitPosition = hitPosVec;
				//hitNormal = normalVec;
			}
		}

		if (materialIndex >= 0)
		{
			//��_���W�����[�J����Ԃ��烏�[���h��Ԃ֕ϊ�
			DirectX::XMVECTOR worldPositionVec = DirectX::XMVector3TransformCoord(hitPosition, worldTransformMat);
			//���[���h��ԏ�ł̃��C�̎n�_�����_�܂ł̃x�N�g��
			DirectX::XMVECTOR worldVec = DirectX::XMVectorSubtract(worldPositionVec, worldRayStartVec);
			//���[���h��ԏ�ł̃��C�̎��_�����_�܂ł̒���
			DirectX::XMVECTOR worldLengthVec = DirectX::XMVector3Length(worldVec);
			float distance;
			DirectX::XMStoreFloat(&distance, worldLengthVec);

			//�q�b�g���ʏ���ۑ�
			if (result.distance > distance)
			{
				//�q�b�g���̖ʂ̖@�������[�J����Ԃ��烏�[���h��Ԃ֕ϊ�
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