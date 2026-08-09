#ifndef __COMPONENT_H__
#define __COMPONENT_H__


//A component is something that can be used when needed, and unused when not.

#define COMPONENT_DEFINE_START(delcName, name) \
	static bool delcName = true;	\
	namespace name {

#define COMPONENT_DEFINE_END }

//Every component has atleast one of these functions
#define COMPONENT_INCLUDE_INIT bool Init()

#define COMPONENT_INCLUDE_UPDATE void Update()

#define COMPONENT_INCLUDE_UNLOAD void Unload()

//Let's use them!
#define COMPONENT_USE_DENY(delcName) delcName = false
#define COMPONENT_USE_DECLARE(delcName) declName = true


#define COMPONENT_USE_CHECK(delcName) if(delcName)

//Basic component calling stuff
#define COMPONENT_CALL_INIT(delcName, name) if(delcName) name::Init()
#define COMPONENT_CALL_INIT_AND_CHECK(delcName, name, code) \
	if(delcName) { [[unlikely]]\
		if(!name::Init()) code \
	}
#define COMPONENT_CALL_UPDATE(delcName, name) if(delcName) name::Update() 
#define COMPONENT_CALL_UNLOAD(delcName, name) if(delcName) name::Unload() 

#endif