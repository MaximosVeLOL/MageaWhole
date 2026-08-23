#pragma once

#include <magea/dev/tool.hpp>

#define DEFINE_TOOL(funcName, className) extern "C" __declspec(dllexport) Developer::Tool* funcName(){return new className();} 