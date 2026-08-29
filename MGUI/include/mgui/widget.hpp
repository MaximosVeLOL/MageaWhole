#ifndef __MGUI_WIDGET_H__
#define __MGUI_WIDGET_H__

#include <mgui/common.hpp>
#include <core/rect.hpp>
#include <core/comp/input.hpp>
#include <mgui/style.hpp>

namespace MGUI {

	class Screen;


	class MGUI_API Widget {
	private:
		Screen* mParent = nullptr;
		u8 mID = 0; //Our index in the screen, IDK why we need this
	protected:
		Alignment mAlignment = { A_TOP, A_LEFT };
		u8 mWidgetParent = PARENT_NONE; //The parent of the widget we are attached to.

		bool uGetMouseHoverBefore();
		bool uGetMouseHoverCurrent();

		void uDrawMRect();

		s16 uDrawText(vector pOffset, const char* pFormat, ...);

		void uDrawRect(RRect pOffset, Render::Color);
		void uDrawFillRect(RRect pOffset, GUIStyle pStyle, bool pOutline = false);

		RRect mRect{ 0 };

		bool mVisible = true;
		bool mDisabled = false;

		//Widget settings
		bool m_sDoHoveringColors = true;

	public:

		Widget* uFindWidget(const char* pName);

		template<typename T>
		inline T* GetAs() { return reinterpret_cast<T*>(this); }

		//Editable things
		GUIStyle mBackground{false, 1}; //Widget background color ID!

		virtual void PreCache(){}

		//When the mouse hovers over the widget
		virtual void OnMouseHover(){}
		//When the mouse is still hovering over the widget
		virtual void OnMouseHovering(){}
		//When the mouse stops hovering over the widget
		virtual void OnMouseEndHover(){}

		//These all occur when the mouse is over

		//When the mouse clicks it
		virtual void OnMouseClickStart(){}
		//When the mouse click is held
		virtual void OnMouseClickHeld(){}
		//When the mouse click ends
		virtual void OnMouseClickEnd(){}

		//When the mouse clicks somewhere else to deselect the widget
		virtual void OnMouseDeselect() {}

		//Update every frame
		virtual void Tick() {}

		//When the widget gets destroyed
		virtual void CleanUp() {}

		virtual void OnAlign(RRect pOffset) {
			mRect.x + pOffset.x;
			mRect.y + pOffset.y;
		}

		void Align(RRect pOffset) {
			OnAlign(pOffset);
		}

		void Render() {
			if (!mVisible) return;
			OnRender();
		}

		virtual void OnRender() {}

		Widget() {}
		Widget(RRect pRect) : mRect(pRect) {}

		~Widget() {
			if (mBackground.isTexture) {
				Style::FlushTextureByID((Style::TextureID)mBackground.id);
			}
			CleanUp();
			
		}

		friend class Screen;

	};

}

#endif