#pragma once

#include "my_header/log.h"

template<typename V, int n, bool v, int m>
struct Uniform {
    static constexpr auto uniform = nullptr;
};

#define TEMPLATE_UNIFORM(V, n, T, t) \
    template<> \
    struct Uniform<V, n, false, 0> { \
        const PFNGLUNIFORM##n##T##PROC uniform = glUniform##n##t; \
    };

#define TEMPLATE_UNIFORM_V(V, n, T, t) \
    template<> \
    struct Uniform<V, n, true, 0> { \
        const PFNGLUNIFORM##n##T##VPROC uniform = glUniform##n##t##v; \
    };

#define TEMPLATE_UNIFORM_V_MATRIX(V, n, T, t) \
    template<> \
    struct Uniform<V, n, true, n> { \
        PFNGLUNIFORMMATRIX##n##T##VPROC uniform = glUniformMatrix##n##t##v; \
    }; 

#define TEMPLATE_UNIFORM_V_MATRIX_X(V, n, T, t, m) \
    template<> \
    struct Uniform<V, n, true, m> { \
        PFNGLUNIFORMMATRIX##n##X##m##T##VPROC uniform = glUniformMatrix##n##x##m##t##v; \
    };


#define TEMPLATE_UNIFORM_(V, T, t) \
    TEMPLATE_UNIFORM(V, 1, T, t) \
    TEMPLATE_UNIFORM(V, 2, T, t) \
    TEMPLATE_UNIFORM(V, 3, T, t) \
    TEMPLATE_UNIFORM(V, 4, T, t) \
    TEMPLATE_UNIFORM_V(V, 1, T, t) \
    TEMPLATE_UNIFORM_V(V, 2, T, t) \
    TEMPLATE_UNIFORM_V(V, 3, T, t) \
    TEMPLATE_UNIFORM_V(V, 4, T, t)

#define TEMPLATE_UNIFORM_MATRIX_(V, T, t) \
    TEMPLATE_UNIFORM_V_MATRIX(V, 2, T, t) \
    TEMPLATE_UNIFORM_V_MATRIX(V, 3, T, t) \
    TEMPLATE_UNIFORM_V_MATRIX(V, 4, T, t) \
    TEMPLATE_UNIFORM_V_MATRIX_X(V, 2, T, t, 3) \
    TEMPLATE_UNIFORM_V_MATRIX_X(V, 2, T, t, 4) \
    TEMPLATE_UNIFORM_V_MATRIX_X(V, 3, T, t, 2) \
    TEMPLATE_UNIFORM_V_MATRIX_X(V, 3, T, t, 4) \
    TEMPLATE_UNIFORM_V_MATRIX_X(V, 4, T, t, 2) \
    TEMPLATE_UNIFORM_V_MATRIX_X(V, 4, T, t, 3) \

TEMPLATE_UNIFORM_(GLfloat, F, f)
TEMPLATE_UNIFORM_(GLint, I, i)
TEMPLATE_UNIFORM_(GLuint, UI, ui)
TEMPLATE_UNIFORM_(GLdouble, D, d)

TEMPLATE_UNIFORM_MATRIX_(GLfloat, F, f)
TEMPLATE_UNIFORM_MATRIX_(GLdouble, D, d)

#undef TEMPLATE_UNIFORM
#undef TEMPLATE_UNIFORM_V
#undef TEMPLATE_UNIFORM_V_MATRIX
#undef TEMPLATE_UNIFORM_V_MATRIX_X
#undef TEMPLATE_UNIFORM_
#undef TEMPLATE_UNIFORM_MATRIX_
