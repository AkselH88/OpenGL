#pragma once

#include <string>

namespace ShaderSource
{
    std::string Vertex(int joints = 0);

    std::string Fragment();

    std::string SkeletenVs();

    std::string SkeletenFs();

    std::string BasicVs();

    std::string BasicFs();
}