#pragma once
#include"chain.h"
class debugobj :public BaseChain
{
public:
    ~debugobj()override {
       
        
        if (comp_)
        {

            delete comp_;
            comp_ = nullptr;
        }
    }
    void Update(float elapsedTime)override {};
    void Gui()override {};
};
