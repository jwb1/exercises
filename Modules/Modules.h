/*
Build steps for GLI and GLM:
mkdir build
cd build
cmake .. -G "Visual Studio 15 2017 Win64"
*/
#pragma once
#if defined(_MSC_VER)
#pragma warning (push)
#pragma warning (disable : 4127) // conditional expression is constant
#pragma warning (disable : 4201) // nonstandard extension used : nameless struct / union
#pragma warning (disable : 4458) // declaration hides class member
#pragma warning (disable : 4701) // potentially uninitialized local variable used
#endif

// GLM
#define GLM_FORCE_SWIZZLE
#define GLM_FORCE_AVX
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_FORCE_RADIANS
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/type_precision.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/color_space.hpp>

// GLI
#include <gli/gli.hpp>

#if defined(_MSC_VER)
#pragma warning (pop)
#endif
