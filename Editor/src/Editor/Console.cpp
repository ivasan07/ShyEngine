#include "Console.h"
#include "Game.h"
#include "Editor.h"
#include "imgui.h"

#include "CheckML.h"

namespace ShyEditor {


	Console::Console() : Window("Console", 0)
	{
		windowWidth = 700;
		windowHeight = 700;

		canBeDisplayedOnTop = true;

	}


	void Console::Behaviour()
	{
		bool newmessages = false;
		while (Game::PendingOutput()) {

			std::string str = Game::FlushOutput();

			if (str != "") {

				size_t divide = str.find_last_of(':') + 1;

				std::string principio = str.substr(0, divide);

				std::string fin = str.substr(divide);

				divide = principio.find(']') + 1;
				std::string fecha = principio.substr(0, divide);
				std::string tipo = principio.substr(divide);

				divide = tipo.find(':') + 1;
				std::string objeto = tipo.substr(0, divide);
				std::string script = tipo.substr(divide);


				newmessages = true;
				messages.push_back({ {51, 153, 255}, fecha + objeto, script, fin });
			}
		}


		if (ImGui::Button("Clear")) {

			messages.clear();
		}

		ImGui::BeginChild("Messages", ImGui::GetContentRegionAvail());


		int messageIdx = 0;
		for (auto& message : messages) {

			const int padding = 30;

			auto contentSize = ImGui::CalcTextSize(
				(message.time + message.type + message.message).c_str(), nullptr, false, windowWidth);

			ImGui::BeginChild(std::to_string(messageIdx).c_str(), ImVec2(windowWidth, contentSize.y));

			ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(message.color.r, message.color.g, message.color.b, 255));
			ImGui::TextWrapped(message.time.c_str());
			ImGui::SameLine();

			ImGui::TextWrapped(message.type.c_str());

			if (ImGui::IsMouseHoveringRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax()))
			{
				if (ImGui::IsMouseDoubleClicked(0))
				{
					Editor::GetInstance()->OpenScript(message.type.substr(1, message.type.size() - 2));
				}
			}

			ImGui::SameLine();
			ImGui::PopStyleColor();
			ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 255, 255, 255));
			ImGui::TextWrapped(message.message.c_str());
			ImGui::PopStyleColor();

			//ImGui::Separator();


			ImGui::EndChild();
			messageIdx++;
		}

		if (newmessages) {

			ImGui::SetScrollHereY(0);
		}

		ImGui::EndChild();
	}

	void Console::Print(const std::string& str)
	{
		Message mssg{};
		mssg.message = str;

		messages.push_back(mssg);
	}

}
