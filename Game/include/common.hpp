#pragma once

//Since we are always going to use other libraries, but other libraries don't use us,
//We don't have to have dllimport stuff.
#define EXPORT extern "C" __declspec(dllexport)

#define DEFINE_OBJECT(name, className) EXPORT Object* name(){return new className();}
