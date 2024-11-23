#include "stdafx.h"
#include "Random.h"

Random* Random::instance = nullptr;
Random* gRandom = Random::getInstance();

Random* Random::getInstance()
{
    if (!instance)
        instance = new Random();

    return instance;
}