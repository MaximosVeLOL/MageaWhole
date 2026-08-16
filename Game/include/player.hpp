#pragma once

#include <magea/object.hpp>

class oPlayer : public Object {
public:
	void PreCache() override;
	void OnTick() override;
	void CleanUp() override;
};