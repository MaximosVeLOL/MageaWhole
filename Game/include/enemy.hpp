#pragma once

#include <magea/object.hpp>


class oEnemy : public Object {
public:
	void PreCache() override;
	void OnTick() override;
};