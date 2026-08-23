#ifndef NDEBUG
#pragma once


#include <magea/dev/tool.hpp>
#include <magea/dev/developer.hpp>

#include <mgui/handler.hpp>
#include <mgui/elements/all.hpp>
using namespace MGUI;

#include <magea/engine.hpp>

#include <api.hpp>

// -------------------
// Debugger -
// A tool made for viewing stdout and other misc debugging stuff
// -------------------

#include <magea/dev/commands.hpp>

namespace Developer {

	void mguiButtonConsoleGoOnClicked();

	class tDebugger : public Tool {
	public:
		Screen* mScreen = nullptr;

		void OnLoad() override {

			Console::Init();

			mScreen = new Screen();
			u8 window = mScreen->AddWidget(new wWindow({ 0, 0, 200, 200 }, Style::C_W_BACKGROUND, true, true));
			//std::function<void()> buttonRestartOnClicked([]() -> void { Engine::Restart(); }); // OK
			//std::function<void> yo = f;
			mScreen->AddWidget(new wTextInput({ 0, 180, 150, 20 }), "cText", window);
			mScreen->AddWidget(new wButton({ 150, 180, 50, 20 }, mguiButtonConsoleGoOnClicked, "Go"), nullptr, window);
			

		}

		void Update() override {
			mScreen->UpdateAndRender();
		}
	};

	void mguiButtonConsoleGoOnClicked() {
		tDebugger* d = GetTool<tDebugger>("debugger");
		char* command = d->mScreen->FindWidget("cText")->GetAs<wTextInput>()->uGetAsString();
		Console::ParseCommand(command);
	}
}

DEFINE_TOOL(DevToolDebugger, Developer::tDebugger)


#endif