#include "ScriptCreation.h"
#include "imgui.h"
#include <fstream>
#include "ImGUIManager.h"
#include <iostream>

PEditor::ScriptCreation* PEditor::ScriptCreation::instance = nullptr;

PEditor::ScriptCreation::ScriptCreation() : Window("", None)
{
	imGuiManager = ImGUIManager::getInstance();

	dropDownSelection = new ScriptCreationUtilities::ScriptDropdownSelection(this);

	xpos = ypos = 0;
	scrollx = scrolly = 0;

	instance = this;
}

PEditor::ScriptCreation::~ScriptCreation()
{
	instance = nullptr;
	delete dropDownSelection;
}

void PEditor::ScriptCreation::AddNode(ScriptCreationUtilities::ScriptNode* node)
{
	node->SetID(nodes.size());
	nodes.push_back(node);
}

void PEditor::ScriptCreation::GetScrollPosition(int* x, int* y)
{
	if (x != nullptr)
		*x = instance->xpos + instance->scrollx;

	if (y != nullptr)
		*y = instance->ypos + instance->scrolly;
}

bool PEditor::ScriptCreation::ScrolledThisFrame()
{
	return instance->scrolled;
}

float offset = 0;

void PEditor::ScriptCreation::render()
{
	ImGui::OpenPopup("Create script");
	ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	if (ImGui::BeginPopup("Create script", ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBringToFrontOnFocus))
	{
		//ImGui::SetWindowFocus();



		if (ImGui::InputText("Name the script", nameBuffer, sizeof(nameBuffer)))
		{
		}

		auto mouseScroll = ImGui::GetMouseDragDelta(2);

		scrollx = mouseScroll.x;
		scrolly = mouseScroll.y;

		if (scrollx != 0 || scrolly != 0)
			scrolled = true;

		if (ImGui::IsMouseReleased(2)) {

			xpos += scrollx;
			ypos += scrolly;

			scrollx = scrolly = 0;
			scrolled = true;
		}
		
		ScriptCreationUtilities::Grid::SetOffset(xpos + scrollx, ypos + scrolly);
		ScriptCreationUtilities::Grid::SetColor(100, 100, 100, 255);
		ScriptCreationUtilities::Grid::Draw();

		for (auto node : nodes) {

			node->Render();
		}

		if (!ImGui::IsMouseDown(0))
			ScriptCreationUtilities::ScriptMethod::currentlySelected = nullptr;



		ImGui::SetCursorPos(ImVec2(0, 700));

		if (ImGui::Button("Save script"))
		{
			std::string filename = std::string(nameBuffer) + ".script";
			std::string filePath = "scripts/" + filename;

			std::ofstream outputFile(filePath);
			if (outputFile.is_open())
			{
				outputFile.close();
			}

			ClearScript();

			ImGUIManager::getInstance()->creatingScript(false);
			ImGui::CloseCurrentPopup();
		}

		ImGui::SameLine();

		if (ImGui::Button("Close"))
		{
			ClearScript();
			ImGUIManager::getInstance()->creatingScript(false);
			ImGui::CloseCurrentPopup();
		}

		dropDownSelection->Render();


		scrolled = false;

		ImGui::EndPopup();
	}


}


void PEditor::ScriptCreation::RenderBox(const std::string& name, ImVec2 position, ImVec2 size)
{

	ImGui::SetCursorPos(position);

	if (ImGui::BeginChild(name.c_str(), size, true, ImGuiWindowFlags_None))
	{

		ImGui::Text("Child Window Content Here");
	}
	ImGui::EndChild();

}

void PEditor::ScriptCreation::ClearScript()
{
	nameBuffer[0] = '\0';

	for (auto node : nodes) {
		delete node;
	}
	nodes.clear();
}

void PEditor::ScriptCreation::setName(std::string name)
{
	strcpy_s(nameBuffer, name.c_str());
}
