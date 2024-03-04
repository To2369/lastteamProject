#pragma once
#include"object.h"
#include"model.h"
class Player:public Object
{
public:

	Player(ID3D11Device* device);
	~Player()override;

	//�X�V����
	void Update(float elapsedTime)override;

	//�`�揈��
	void Render(RenderContext* rc)override;

private:
	const char* filename = ".\\resources\\3Dmodel\\Cube\\Cube.fbx";
};