#pragma once

#include <engine/object.hpp>

class oPlayer : public Object {
public:
	void PreCache() override;
	void OnTick() override;
};