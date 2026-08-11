#ifndef __OBJECT_H__
#define __OBJECT_H__

#include <engine/common.hpp>
#include <engine/types.hpp>
//#include <SDL3/SDL.h>

struct Sprite;

struct Transform {
	vector position{ 0 };
	VectorTemplate<s8> velocity;
	SIZE_TYPE width = 0, height = 0;
	//DECIMAL_TYPE rotation = 0.0f;
	u8 xScale : 4, yScale : 4;

	bool Overlaps(Transform pOther);
	rect AsRect() { return { position.x, position.y, width, height }; }
	RRect AsRRect() { return { position.x, position.y, width, height }; }

};

struct VisualSettings {
	bool visible : 4;
	bool useSprite : 4;

	//A Render::texture_t
	union {
		void* texture = nullptr;
		Sprite* sprite;
	};
	void UpdateSprite() {
		return;
	}
	void LoadSprite(const char* pTexturePageName, const char* pSpriteName) {
		return;
	}
};
struct CollisionSettings {
	//Can we collide with other things?
	bool canCollide : 4;
	bool solid : 4;
	u8 mass = 0;
};

/*
//A simple object that only contains a position, and a tag. Meant for simple things
class Object {
public:
	Transform mTransform;
	VisualSettings mVisual;
};
*/

//An object meant for game stuff?
class Object {// : public Object {
private:

public:

	bool mActive = true;

	Transform mTransform = { 0, 0, 0, 0, 1, 1 };
	VisualSettings mVisual = {true, false, nullptr};


	u8 mTag = 0; //Any u8 value can be assigned to this, but it is you who needs to make an enum!

	bool uCompareTag(u8 pTag);

	void Tick();


	//Load all of your assets man!
	virtual void PreCache(){}
	
	//When the world ticks, you go too.
	virtual void OnTick(){}

	//Not sure if we need this, because deconstructors exist.
	virtual void CleanUp(){}

	//Whenever our rendering capabilities aren't enough
	virtual void Render(){}
};
//Show our object in the DLL
#define DEFINE_OBJECT(name, className) EXPORT Object* name(){return new className();}


#endif