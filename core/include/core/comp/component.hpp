#ifndef __COMPONENT_H__
#define __COMPONENT_H__

//A component is something that can be used when needed, and unused when not.

#define COMPONENT_DEFINE_START(delcName, name, used) \
	constexpr bool delcName = used;	\
	namespace name {

#define COMPONENT_DEFINE_END }

//Every component has atleast one of these functions

//We shouldn't use an unsigned char for the Init() function, as a u8 is returned for only one thing, but it's still a good measure!
#define COMPONENT_INCLUDE_INIT bool Init()

#define COMPONENT_INCLUDE_UPDATE void Update()

#define COMPONENT_INCLUDE_UNLOAD void Unload()


#define COMPONENT_DEFINE_INIT bool Init()
#define COMPONENT_DEFINE_UPDATE void Update()
#define COMPONENT_DEFINE_UNLOAD void Unload()

//Let's use them!
#define COMPONENT_USE_DENY(delcName) delcName = false
#define COMPONENT_USE_DECLARE(delcName) declName = true


#define COMPONENT_USE_CHECK(delcName) if constexpr(delcName)

//Basic component calling stuff
#define COMPONENT_CALL_INIT(delcName, name) if constexpr(delcName) name::Init()
#define COMPONENT_CALL_INIT_AND_CHECK(delcName, name, code) \
	if constexpr(delcName) { \
		if(!name::Init()) [[unlikely]] code \
	}
#define COMPONENT_CALL_UPDATE(delcName, name) if constexpr(delcName) name::Update() 
#define COMPONENT_CALL_UNLOAD(delcName, name) if constexpr(delcName) name::Unload() 

#endif