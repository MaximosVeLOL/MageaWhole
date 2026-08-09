#ifndef __COMPONENT_H__
#define __COMPONENT_H__


//A component is something that can be used when needed, and unused when not.

#define COMPONENT_DEFINE_START(name) namespace name {

#define COMPONENT_DEFINE_END }


#define COMPONENT_INCLUDE_INIT bool Init()

#define COMPONENT_INCLUDE_UPDATE void Update()

#define COMPONENT_INCLUDE_UNLOAD void Unload()


#define COMPONENT_USE_DECLARE(name) #define name

#define COMPONENT_IS_USED(name) #ifdef name

#endif