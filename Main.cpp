// Copyright 2020 Petr Petrovich Petrov. All rights reserved.
// License: https://github.com/PetrPPetrov/experiments/blob/master/LICENSE

#include <cassert>
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <memory>
#include <vector>
#include <limits>
#include <iostream>
#include <typeinfo>

template<unsigned N>
constexpr const char* to_str()
{
    return "" N "";
}

template<unsigned R, unsigned C>
const char* description();

template<>
const char* description<1, 2>()
{
    return "matrix<1, 2>";
}

template<>
const char* description<1, 3>()
{
    return "matrix<1, 3>";
}

template<>
const char* description<3, 3>()
{
    return "matrix<3, 3>";
}

template<>
const char* description<4, 4>()
{
    return "matrix<3, 3>";
}

template<unsigned R, unsigned C>
struct matrix
{
    double data[R][C];
};

template<unsigned R, unsigned C>
void dump(matrix<R, C>& value)
{
    std::cout << description<R, C>() << std::endl;
}

int main()
{
    matrix<3, 3> a;
    dump(a);

    return EXIT_SUCCESS;
}
