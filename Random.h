#pragma once

template<typename type> using integerType = typename std::enable_if<std::is_integral<type>::value, type>::type;
template<typename type> using floatingType = typename std::enable_if<std::is_floating_point<type>::value, type>::type;

class Random
{
    static Random* instance;
    std::random_device device;
    std::mt19937* generator = nullptr;

    Random() { generator = new std::mt19937(device()); }
    ~Random() { delete generator; delete instance; };

public:

    static Random* getInstance();

    template<typename type> integerType<type> GenerateNumber(type min, type max);
    template<typename type> floatingType<type> GenerateNumber(type min, type max);
};

template<typename type>
integerType<type> Random::GenerateNumber(type min, type max)
{
    std::uniform_int_distribution<type> dist(min, max);
    return dist(*generator);
}

template<typename type>
floatingType<type> Random::GenerateNumber(type min, type max)
{
    std::uniform_real_distribution<type> dist(min, max);
    return dist(*generator);
}