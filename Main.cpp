// Copyright 2020 Petr Petrovich Petrov. All rights reserved.
// License: https://github.com/PetrPPetrov/experiments/blob/master/LICENSE

#include <cassert>
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <memory>
#include <vector>
#include <limits>
#include <Windows.h>

namespace T0
{
    class Reader
    {
        std::vector<int>& data;
        size_t index = 0;

    public:
        Reader(std::vector<int>& data_) : data(data_) {}

        constexpr static bool isReader = true;
        void serialize(int& a)
        {
            a = data.at(index++);
        }
    };

    class Writer
    {
        std::vector<int>& data;

    public:
        Writer(std::vector<int>& data_) : data(data_) {}

        constexpr static bool isReader = false;
        void serialize(int a)
        {
            data.push_back(a);
        }
    };

    template<class Serializator>
    void serialize(Serializator& serializator, int& data)
    {
        serializator.serialize(data);
    }

    struct IFigure
    {
        virtual void dump() = 0;
        int code = -1;
    };

    struct Triangle : public IFigure
    {
        int a = -1;
        int b = -1;
        int c = -1;

        Triangle() { code = 1; }
        Triangle(int a_, int b_, int c_) : a(a_), b(b_), c(c_) { code = 1; }

        virtual void dump()
        {
            std::cout << "    triangle code: " << code << " a: " << a << " b: " << b << " c: " << c << std::endl;
        }
    };

    template<class Serializator>
    void serialize_triangle(Serializator& serializator, IFigure*& figure)
    {
        if constexpr (Serializator::isReader)
            figure = new Triangle();

        Triangle* triangle = static_cast<Triangle*>(figure);
        serialize(serializator, triangle->a);
        serialize(serializator, triangle->b);
        serialize(serializator, triangle->c);
    }

    struct Circle : public IFigure
    {
        int radius = -1;

        Circle() { code = 2;  }
        Circle(int radius_) : radius(radius_) { code = 2; }

        virtual void dump()
        {
            std::cout << "    circle code: " << code << " radius: " << radius << std::endl;
        }
    };

    template<class Serializator>
    void serialize_circle(Serializator& serializator, IFigure*& figure)
    {
        if constexpr (Serializator::isReader)
            figure = new Circle();

        Circle* circle = static_cast<Circle*>(figure);
        serialize(serializator, circle->radius);
    }

    template<class Serializator>
    void serialize(Serializator& serializator, IFigure*& figure)
    {
        int object_type = figure ? figure->code : 0;
        serialize(serializator, object_type);
        switch (object_type)
        {
        default:
        case 0:
            if constexpr (!Serializator::isReader)
            {
                figure = nullptr;
            }
            break;
        case 1:
            serialize_triangle(serializator, figure);
            break;
        case 2:
            serialize_circle(serializator, figure);
            break;
        }
    }

    struct Info
    {
        int info0 = -1;
        int info1 = -1;
    };

    template<class Serializator>
    void serialize(Serializator& serializator, Info& info)
    {
        serialize(serializator, info.info0);
        serialize(serializator, info.info1);
    }

    struct Node
    {
        int weight = -1;
        int height = -1;
        std::vector<IFigure*> figures;
        std::vector<Info> infos;

        virtual void dump()
        {
            std::cout << "node weight: " << weight << " height: " << height << " figure_count: " << figures.size() << std::endl;
            for (size_t i = 0; i < figures.size(); ++i)
            {
                IFigure* figure = figures[i];
                if (figure)
                {
                    figure->dump();
                }
            }
        }
    };

    template<class Serializator>
    void serialize(Serializator& serializator, Node& node)
    {
        serialize(serializator, node.weight);
        serialize(serializator, node.height);

        if constexpr (Serializator::isReader)
        {
            int figure_count = 0;
            serialize(serializator, figure_count);
            node.figures.resize(figure_count, nullptr);
            for (int i = 0; i < figure_count; ++i)
            {
                serialize(serializator, node.figures[i]);
            }
        }
        else
        {
            int figure_count = static_cast<int>(node.figures.size());
            serialize(serializator, figure_count);
            for (auto& figure : node.figures)
            {
                serialize(serializator, figure);
            }
        }

        if constexpr (Serializator::isReader)
        {
            int info_count = 0;
            serialize(serializator, info_count);
            node.infos.resize(info_count);
            for (int i = 0; i < info_count; ++i)
            {
                serialize(serializator, node.infos[i]);
            }
        }
        else
        {
            int info_count = static_cast<int>(node.infos.size());
            serialize(serializator, info_count);
            for (auto& info : node.infos)
            {
                serialize(serializator, info);
            }
        }
    }
}

namespace T
{
    class Reader
    {
        std::vector<int>& data;
        size_t index = 0;

    public:
        Reader(std::vector<int>& data_) : data(data_) {}
        int serialize(int a)
        {
            return data.at(index++);
        }
        int peek() const
        {
            return data.at(index);
        }
    };

    class Writer
    {
        std::vector<int>& data;

    public:
        Writer(std::vector<int>& data_) : data(data_) {}
        int serialize(int a)
        {
            data.push_back(a);
            return a;
        }
    };

    template<class Serializator, class Primitive>
    inline void serialize_primitive(Primitive& field, Serializator& serializator)
    {
        field = serializator.serialize(field);
    }

    struct IFigure
    {
        virtual void serialize(Reader& r) = 0;
        virtual void serialize(Writer& w) = 0;
        virtual void dump() = 0;
    };

    struct Triangle : public IFigure
    {
        int code = 1;
        int a = -1;
        int b = -1;
        int c = -1;

        Triangle() {}
        Triangle(int a_, int b_, int c_) : a(a_), b(b_), c(c_) {}

        template<class Serializator>
        void serialize(Serializator& serializator)
        {
            serialize_primitive(code, serializator);
            serialize_primitive(a, serializator);
            serialize_primitive(b, serializator);
            serialize_primitive(c, serializator);
        }

        virtual void serialize(Reader& r)
        {
            serialize<Reader>(r);
        }

        virtual void serialize(Writer& w)
        {
            serialize<Writer>(w);
        }

        virtual void dump()
        {
            std::cout << "    triangle code: " << code << " a: " << a << " b: " << b << " c: " << c << std::endl;
        }
    };

    struct Circle : public IFigure
    {
        int code = 2;
        int radius = -1;

        Circle() {}
        Circle(int radius_) : radius(radius_) {}

        template<class Serializator>
        void serialize(Serializator& serializator)
        {
            serialize_primitive(code, serializator);
            serialize_primitive(radius, serializator);
        }

        virtual void serialize(Reader& r)
        {
            serialize<Reader>(r);
        }

        virtual void serialize(Writer& w)
        {
            serialize<Writer>(w);
        }

        virtual void dump()
        {
            std::cout << "    circle code: " << code << " radius: " << radius << std::endl;
        }
    };

    template<class Serializator, class Struct>
    inline void serialize_struct(Struct* object, Serializator& serializator)
    {
        object->serialize(serializator);
    }

    template<class Object>
    inline void serialize_object(Object*& object, Writer& serializator)
    {
        if (!object)
        {
            int object_type = 0;
            serialize_primitive(object_type, serializator);
            return;
        }

        object->serialize(serializator);
    }

    template<class Object>
    inline void serialize_object(Object*& object, Reader& serializator)
    {
        int object_type = serializator.peek();

        IFigure* result = nullptr;
        switch(object_type)
        {
        default:
        case 0:
            break;
        case 1:
            result = new Triangle;
            break;
        case 2:
            result = new Circle;
            break;
        }

        if (result)
        {
            result->serialize(serializator);
        }
        object = result;
    }

    struct Info
    {
        int info0 = -1;
        int info1 = -1;

        template<class Serializator>
        void serialize(Serializator& serializator)
        {
            serialize_primitive(info0, serializator);
            serialize_primitive(info1, serializator);
        }
    };

    struct Node
    {
        int weight = -1;
        int height = -1;
        std::vector<IFigure*> figures;
        std::vector<Info> infos;

        template<class Serializator>
        void serialize(Serializator& serializator)
        {
            serialize_primitive(weight, serializator);
            serialize_primitive(height, serializator);
            int figure_count = static_cast<int>(figures.size());
            serialize_primitive(figure_count, serializator);
            figures.resize(figure_count, nullptr);
            for (int i = 0; i < figure_count; ++i)
            {
                serialize_object(figures[i], serializator);
            }
            int info_count = static_cast<int>(infos.size());
            serialize_primitive(info_count, serializator);
            infos.resize(info_count, Info());
            for (int i = 0; i < info_count; ++i)
            {
                serialize_struct(&infos[i], serializator);
            }
        }

        virtual void dump()
        {
            std::cout << "node weight: " << weight << " height: " << height << " figure_count: " << figures.size() << std::endl;
            for (size_t i = 0; i < figures.size(); ++i)
            {
                IFigure* figure = figures[i];
                if (figure)
                {
                    figure->dump();
                }
            }
        }
    };
}

namespace V
{
    class ISerializer
    {
    public:
        virtual int serialize(int a) = 0;
        virtual int peek() const = 0;
        virtual bool isReader() const = 0;
    };

    struct ISerializable
    {
        virtual void serialize(ISerializer& serializer) = 0;
    };

    class Reader : public ISerializer
    {
        std::vector<int>& data;
        size_t index = 0;

    public:
        Reader(std::vector<int>& data_) : data(data_) {}
        virtual int serialize(int a)
        {
            return data.at(index++);
        }
        virtual int peek() const
        {
            return data.at(index);
        }
        virtual bool isReader() const
        {
            return true;
        }
    };

    class Writer : public ISerializer
    {
        std::vector<int>& data;

    public:
        Writer(std::vector<int>& data_) : data(data_) {}
        virtual int serialize(int a)
        {
            data.push_back(a);
            return a;
        }
        virtual int peek() const
        {
            assert(false);
            return 0;
        }
        virtual bool isReader() const
        {
            return false;
        }
    };

    template<class Serializator, class Primitive>
    inline void serialize_primitive(Primitive& field, Serializator& serializator)
    {
        field = serializator.serialize(field);
    }

    struct IFigure : public ISerializable
    {
        virtual void dump() = 0;
    };

    struct Triangle : public IFigure
    {
        int code = 1;
        int a = -1;
        int b = -1;
        int c = -1;

        Triangle() {}
        Triangle(int a_, int b_, int c_) : a(a_), b(b_), c(c_) {}

        virtual void serialize(ISerializer& serializator)
        {
            serialize_primitive(code, serializator);
            serialize_primitive(a, serializator);
            serialize_primitive(b, serializator);
            serialize_primitive(c, serializator);
        }

        virtual void dump()
        {
            std::cout << "    triangle code: " << code << " a: " << a << " b: " << b << " c: " << c << std::endl;
        }
    };

    struct Circle : public IFigure
    {
        int code = 2;
        int radius = -1;

        Circle() {}
        Circle(int radius_) : radius(radius_) {}

        void serialize(ISerializer& serializator)
        {
            serialize_primitive(code, serializator);
            serialize_primitive(radius, serializator);
        }

        virtual void dump()
        {
            std::cout << "    circle code: " << code << " radius: " << radius << std::endl;
        }
    };

    inline void serialize_struct(ISerializable* object, ISerializer& serializator)
    {
        object->serialize(serializator);
    }

    inline void serialize_object(ISerializable*& object, ISerializer& serializator)
    {
        if (serializator.isReader())
        {
            int object_type = serializator.peek();

            IFigure* result = nullptr;
            switch (object_type)
            {
            default:
            case 0:
                break;
            case 1:
                result = new Triangle;
                break;
            case 2:
                result = new Circle;
                break;
            }

            if (result)
            {
                result->serialize(serializator);
            }
            object = result;
        }
        else
        {
            if (!object)
            {
                int object_type = 0;
                serialize_primitive(object_type, serializator);
                return;
            }

            object->serialize(serializator);
        }
    }

    struct Info : public ISerializable
    {
        int info0 = -1;
        int info1 = -1;

        virtual void serialize(ISerializer& serializator)
        {
            serialize_primitive(info0, serializator);
            serialize_primitive(info1, serializator);
        }
    };

    struct Node : public ISerializable
    {
        int weight = -1;
        int height = -1;
        std::vector<IFigure*> figures;
        std::vector<Info> infos;

        virtual void serialize(ISerializer& serializator)
        {
            serialize_primitive(weight, serializator);
            serialize_primitive(height, serializator);
            int figure_count = static_cast<int>(figures.size());
            serialize_primitive(figure_count, serializator);
            figures.resize(figure_count, nullptr);
            for (int i = 0; i < figure_count; ++i)
            {
                ISerializable* serializable = figures[i];
                serialize_object(serializable, serializator);
                figures[i] = reinterpret_cast<IFigure*>(serializable);
            }
            int info_count = static_cast<int>(infos.size());
            serialize_primitive(info_count, serializator);
            infos.resize(info_count, Info());
            for (int i = 0; i < info_count; ++i)
            {
                serialize_struct(&infos[i], serializator);
            }
        }

        virtual void dump()
        {
            std::cout << "node weight: " << weight << " height: " << height << " figure_count: " << figures.size() << std::endl;
            for (size_t i = 0; i < figures.size(); ++i)
            {
                IFigure* figure = figures[i];
                if (figure)
                {
                    figure->dump();
                }
            }
        }
    };
}


void dump(std::vector<int>& data)
{
    for (size_t i = 0; i < data.size(); ++i)
    {
        std::cout << std::fixed << std::setw(5) << data[i] << " ";
    }
    std::cout << std::endl;
}

int main()
{
    using namespace T0;

    const static size_t COUNT = 1024 * 1204 * 32;

    std::vector<int> data;
    data.reserve(COUNT * 8);
    Writer w(data);

    Node a;
    a.weight = 1024;
    a.height = 768;
    a.figures.reserve(COUNT + 10);
    a.figures.push_back(new Triangle(1, 2, 3));
    a.figures.push_back(new Circle(13));
    a.figures.push_back(new Triangle(6, 7, 8));
    for (size_t i = 0; i < COUNT; ++i)
    {
        a.figures.push_back(new Circle(static_cast<int>(i)));
    }
    a.infos.reserve(COUNT + 10);
    for (size_t i = 0; i < COUNT; ++i)
    {
        a.infos.push_back(Info());
    }
    std::cout << "created" << std::endl;
    //a.dump();
    DWORD t0 = GetTickCount();
    serialize(w, a);
    //a.serialize(w);
    DWORD t1 = GetTickCount();

    //dump(data);

    Reader r(data);
    Node b;

    DWORD t2 = GetTickCount();
    serialize(r, b);
    //b.serialize(r);
    DWORD t3 = GetTickCount();

    //b.dump();

    std::cout << "t0 = " << t0 << std::endl;
    std::cout << "t1 = " << t1 << std::endl;
    std::cout << "t2 = " << t2 << std::endl;
    std::cout << "t3 = " << t3 << std::endl;
    std::cout << "t1 - t0 = " << t1 - t0 << std::endl;
    std::cout << "t3 - t2 = " << t3 - t2 << std::endl;

    return EXIT_SUCCESS;
}
