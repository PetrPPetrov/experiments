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
struct vector
{
    double data[N] = { 0 };
};

template<unsigned R, unsigned C>
struct matrix
{
    double data[R][C] = { 0 };
};

namespace T0
{
    template<unsigned N>
    void dump(const vector<N>& v)
    {
        for (unsigned i = 0; i < N; ++i)
        {
            std::cout << "element " << i << " " << v.data[i] << std::endl;
        }
    }

    template<unsigned R, unsigned C>
    void dump(const matrix<R, C>& m)
    {
        for (unsigned i = 0; i < R; ++i)
        {
            for (unsigned j = 0; j < C; ++j)
            {
                std::cout << "element " << i << " " << j << " " << m.data[i][j] << std::endl;
            }
        }
    }
}

namespace T1
{
    template<unsigned N>
    constexpr const char* get_number();

    template<>
    constexpr const char* get_number<0>()
    {
        return "0";
    }

    template<>
    constexpr const char* get_number<1>()
    {
        return "1";
    }

    template<>
    constexpr const char* get_number<2>()
    {
        return "2";
    }

    template<>
    constexpr const char* get_number<3>()
    {
        return "3";
    }

    template<unsigned N, unsigned Counter>
    struct dumper
    {
        static inline void dump(const vector<N>& v)
        {
            dumper<N, Counter - 1>::dump(v);
            std::cout << "element " << get_number<Counter>() << " " << v.data[Counter] << std::endl;
        }
    };

    template<unsigned N>
    struct dumper<N, 0>
    {
        static inline void dump(const vector<N>& v)
        {
            std::cout << "element 0 " << v.data[0] << std::endl;
        }
    };

    template<unsigned N>
    void dump(const vector<N>& v)
    {
        dumper<N, N-1>::dump(v);
    }

    template<unsigned R, unsigned C, unsigned I, unsigned J>
    struct matrix_dumper
    {
        static inline void dump(const matrix<R, C>& m)
        {
            matrix_dumper<R, C, I, J - 1>::dump(m);
            std::cout << "element " << get_number<I>() << " " << get_number<J>() << " " << m.data[I][J] << std::endl;
        }
    };

    template<unsigned R, unsigned C, unsigned I>
    struct matrix_dumper<R, C, I, 0>
    {
        static inline void dump(const matrix<R, C>& m)
        {
            matrix_dumper<R, C, I - 1, C - 1>::dump(m);
            std::cout << "element " << get_number<I>() << " 0 " << m.data[I][0] << std::endl;
        }
    };

    template<unsigned R, unsigned C>
    struct matrix_dumper<R, C, 0, 0>
    {
        static inline void dump(const matrix<R, C>& m)
        {
            std::cout << "element 0 0 " << m.data[0][0] << std::endl;
        }
    };

    template<unsigned R, unsigned C>
    void dump(const matrix<R, C>& m)
    {
        matrix_dumper<R, C, R - 1, C - 1>::dump(m);
    }
}

int main()
{
    using namespace T1;

    vector<3> v;
    v.data[1] = 1.0;
    v.data[2] = 3.14;
    matrix<3, 3> m;
    m.data[1][2] = 1.0;
    m.data[2][1] = 3.14;
    m.data[0][1] = 7.0;
    m.data[2][0] = -1.0;

    std::cout << "vector" << std::endl;
    dump(v);

    std::cout << "matrix" << std::endl;
    dump(m);

    return EXIT_SUCCESS;
}
