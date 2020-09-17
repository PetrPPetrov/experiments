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

template<typename ...> struct type_list {};

template<typename T>
constexpr bool find_impl(const T*, const type_list<T>&)
{
    return true;
}

template<typename E, typename T>
constexpr bool find_impl(const E*, const type_list<T>&)
{
    return false;
}

template<typename T, typename ...RestTypes>
constexpr bool find_impl(const T*, const type_list<T, RestTypes...>&)
{
    return true;
}

template<typename E, typename T, typename ...RestTypes>
constexpr bool find_impl(const E* element, const type_list<T, RestTypes...>&)
{
    return find_impl<E>(element, type_list<RestTypes...>());
}

template<class E, class List>
constexpr bool find()
{
    return find_impl<E>(nullptr, List());
}

template<typename T>
void show_list_impl(const type_list<T>&)
{
    std::cout << typeid(T).name() << std::endl;
}

template<typename T, typename ...RestTypes>
void show_list_impl(const type_list<T, RestTypes...>&)
{
    std::cout << typeid(T).name() << std::endl;
    show_list_impl(type_list<RestTypes...>());
}

template<class List>
void show_list()
{
    return show_list_impl(List());
}

typedef type_list <
    int,
    double,
    char
> type_list_t;

struct Y
{
};

struct X
{
    virtual ~X() {}
};

struct I
{
    virtual ~I() {}
};

struct Z : public I
{
};

template<typename T>
void check()
{
    if constexpr (find<T, type_list_t>())
    {
        std::cout << typeid(T).name() << " is in list" << std::endl;
    }
    else
    {
        std::cout << typeid(T).name() << " is not in list" << std::endl;
    }
}

int main()
{
    std::cout << "type list:" << std::endl;
    show_list<type_list_t>();
    std::cout << std::endl;

    check<Z>();

    return EXIT_SUCCESS;
}
