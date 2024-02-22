#include "ground.h"

Ground::Ground(ID3D11Device* device)
{
	model = std::make_unique<Model>(device, filename);

	attribute_state = Obj_attribute::null;
	Scale.x = Scale.y = Scale.z = 10.0f;
	Position.y = -10.0f;
}

void Ground::Update(float elapsed_time)
{
	UpdateTransform();
}

void Ground::Render(ID3D11DeviceContext* dc)
{
	if (model)model->render(dc, Transform);
}
