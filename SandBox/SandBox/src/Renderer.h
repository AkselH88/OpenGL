#pragma once

#include <GL/glew.h>

#include "Shader/Shader.h"

// start error
#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);

// end error

class Renderer
{
public:
    void Clear() const;
};