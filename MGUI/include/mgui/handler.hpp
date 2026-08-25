#ifndef __MGUI_HANDLER_H__
#define __MGUI_HANDLER_H__

#include "common.hpp"

namespace MGUI {
	class Widget;

	//A screen is a part of 

	struct HoverState {
		bool cur : 4;
		bool prev : 4;
	};

	class MGUI_API Screen {
	public:
		Widget** mWidgets = nullptr;
		//Names for each widget id
		char** mWidgetNames = nullptr;
		HoverState* mHoverStates = nullptr;
		u8 mWidgetCount = 0;
		bool mMouseOverWidget = false;

		Screen();

		u8 AddWidget(Widget* pWidget, const char* pName = nullptr, u8 pWidgetParentID = PARENT_NONE);

		Widget* FindWidget(const char* pName);

		void UpdateAndRender();

		void UpdateAlignment(RRect pOffset);

		//Destroys all the widgets, and other neccessary information
		void Destroy();
	};

	MGUI_API void Init();

	MGUI_API void AddScreen(Screen* pScreen);

	MGUI_API void UpdateAndRender();

	MGUI_API Screen* GetScreen(u8 pIndex);

	MGUI_API void RemoveScreen(u8 pIndex);

	MGUI_API void PostCall();

	MGUI_API void SetPostCallEvent(void(*pEvent)());

	MGUI_API void UpdateAlignment(RRect pOffset);
}


#endif