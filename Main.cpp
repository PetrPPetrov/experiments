// Copyright 2020 Petr Petrovich Petrov. All rights reserved.
// License: https://github.com/PetrPPetrov/experiments/blob/master/LICENSE

#include <cstdlib>
#include <iostream>
#include <vector>

static void dump(const std::vector<int>& data)
{
    std::cout << "dump" << std::endl;
    for (auto element : data)
    {
        std::cout << element << std::endl;
    }
}

class A
{
    std::vector<int> a;

public:
    A(std::vector<int>&& a_) : a(std::move(a_))
    {
    }

    void dump() const
    {
        ::dump(a);
    }
};

int main()
{
    std::vector<int> c;
    c.push_back(5);
    c.push_back(7);
    c.push_back(9);
    dump(c);

    A a(std::move(c));
    dump(c);
    a.dump();
    return EXIT_SUCCESS;
}
