#include"Player.h"
Player::Player(ID3D11Device*device)
{
    model = make_unique<Model>(device, filename, true);
    old_attribute_state = Obj_attribute::null;
    Scale.x = Scale.y = Scale.z = 100.0f;
}

Player::~Player()
{

}

void Player::Update(float elapsedTime)
{
    UpdateTransform();
}

void Player::Render(ID3D11DeviceContext* dc)
{
    model->render(dc, Transform);
}