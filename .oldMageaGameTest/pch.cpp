// pch.cpp: source file corresponding to the pre-compiled header

#include "pch.h"
#include "oPlayer.hpp"

EXPORT bool Init() {
	return true;
}

EXPORT u8 GetExpectedEngineVersion() {
	return 0x00;
}

EXPORT void Unload() {

}

EXPORT EngineFunctions* GetEngineFunctions() {
	return &gEngine;
}


// When you are using pre-compiled headers, this source file is necessary for compilation to succeed.
