#include"Player.h"
Player::Player(ID3D11Device*device)
{
    model = make_unique<Model>(device, filename, true);
    old_attribute_state.push_back(Obj_attribute::null);
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