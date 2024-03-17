#include"Player.h"
Player::Player(ID3D11Device*device)
{
    model = make_unique<Model>(device, filename, true);
    initialaize_Set_attribute(ObjType::null, ObjType::null);
    Scale.x = Scale.y = Scale.z = 100.0f;
}

Player::~Player()
{

}

void Player::Update(float elapsedTime)
{
    UpdateTransform();
}

void Player::Render(RenderContext* rc)
{
    model->render(rc->deviceContext, Transform);
}