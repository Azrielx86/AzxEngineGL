//
// Created by tohka on 9/10/25.
//

#ifndef OPENGLPLAYGROUND_UTILS_H
#define OPENGLPLAYGROUND_UTILS_H

#include <string>

// struct is from "https://www.cppstories.com/2021/heterogeneous-access-cpp20/"
struct string_hash
{
    using is_transparent = void;

    [[nodiscard]] size_t operator()(const char *txt) const
    {
        return std::hash<std::string_view>{}(txt);
    }

    // ReSharper disable once CppParameterMayBeConst
    [[nodiscard]] size_t operator()(std::string_view txt) const
    {
        return std::hash<std::string_view>{}(txt);
    }

    [[nodiscard]] size_t operator()(const std::string &txt) const
    {
        return std::hash<std::string>{}(txt);
    }
};


#endif // OPENGLPLAYGROUND_UTILS_H
