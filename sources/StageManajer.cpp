#include "StageManager.h"

void StageManager::Update(float elapsedTime)
{
    const int stage_count = Stages.size();
    for (int i = 0; i < stage_count; i++)
    {
        Stages[i]->Update(elapsedTime);
    }
}

void StageManager::Render(ID3D11DeviceContext* dc)
{
    const int stage_count = Stages.size();
    for (int i = 0; i < stage_count; i++)
    {
        Stages[i]->Render(dc);
    }
}

void StageManager::Clear()
{
    Stages.clear();
}
