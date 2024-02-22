#pragma once
#include"Debug_Stage.h"
class StageManager
{
public:
    StageManager() {};
    ~StageManager() {};


    static StageManager& incetance()
    {
        static StageManager ince;
        return ince;
    }
    void Update(float elapsedTime);
    void Render(ID3D11DeviceContext* dc);
    void Rigister(unique_ptr<DebugStage>stage) { Stages.push_back(move(stage)); }
    void Clear();
    /// <summary>
    /// 
    /// </summary>
    /// <param name="name">StageManagerの中にある GetNowStage()を引数に使ってください</param>
    /// <returns></returns>
    DebugStage* GetStages(StageName name) {
        int stagecount = Stages.size();
        for (int i = 0; i < stagecount; i++)
        {
            if (Stages[i]->GetNowStage() == name);
            {
                return Stages[i].get();
            }
        }
    }

    //ステージ選択したときにセットする
    void SetStage(StageName n) { s_name = n; }
    StageName GetNowStage() { return s_name; }
private:
    vector<unique_ptr<DebugStage>>Stages;
    StageName s_name = StageName::stage1;
};
