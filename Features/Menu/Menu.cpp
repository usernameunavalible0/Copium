
#include "Menu.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx9.h"
#include <d3d9.h>
#include "../Vars.h"

const char* BoxesType[] = { "Defualt", "Outline" };

void CMenu::DrawMenuInterface()
{
	auto& style = ImGui::GetStyle();
	ImGuiIO& io = ImGui::GetIO();

	ImGui::SetNextWindowPos(ImVec2{ 100, 100 }, 0);
	ImGui::SetNextWindowSize(ImVec2{ 450, 280 }, 0);
	style.Alpha = 0.9f;

	ImGui::Begin("Copium-TF2", &Vars::Menu::Enabled, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
	{
		ImGui::BeginChild(("first penis"), ImVec2(400, 260), true);
		{
			ImGui::Separator();
			ImGui::Text(("ESP"));
			ImGui::Separator();
			ImGui::Checkbox("Box", &Vars::ESP::Players::Box);
			ImGui::SameLine(150);
			ImGui::Checkbox(("HP"), &Vars::ESP::Players::HealthBar);
			ImGui::SameLine(300);
			ImGui::Separator();
			ImGui::Text(("ESP Filter"));
			ImGui::Checkbox("Render on teammates", &Vars::ESP::Players::IgnoreTeam);
			ImGui::Separator();
			ImGui::Text(("ESP Options"));
			ImGui::Separator();
			ImGui::Text(("Removals"));
			ImGui::Separator();
			ImGui::Checkbox("Remove scope", &Vars::Visual::RemoveScope.m_Var);
			ImGui::Separator();
			ImGui::Text(("Miscellaneous"));
			ImGui::Separator();
			ImGui::Checkbox("BHOP", &Vars::Misc::Bunnyhop.m_Var);
			ImGui::SameLine(300);
			ImGui::Checkbox("Auto-strafe", &Vars::Misc::AutoStrafe.m_Var);
			ImGui::Separator();

		}
		ImGui::EndChild();
	}
	ImGui::End(); //End main window
}
