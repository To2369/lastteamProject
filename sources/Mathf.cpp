#include"Mathf.h"
#include<stdlib.h>

//üŒ`•âŠ®
float Mathf::Leap(float a, float b, float t)
{
    return a * (1.0f - t) + (b * t);
}