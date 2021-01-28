#include "UnitInfo.h"
#include "imgui/imgui.h"

std::map<std::string, UnitTag> UnitInfo::TagMapping = {
	{std::string("Unit_"), Unit_},
	{std::string("Unit_Champion"), Unit_Champion},
	{std::string("Unit_Champion_Clone"), Unit_Champion_Clone},
	{std::string("Unit_IsolationNonImpacting"), Unit_IsolationNonImpacting},
	{std::string("Unit_KingPoro"), Unit_KingPoro},
	{std::string("Unit_Minion"), Unit_Minion},
	{std::string("Unit_Minion_Lane"), Unit_Minion_Lane},
	{std::string("Unit_Minion_Lane_Melee"), Unit_Minion_Lane_Melee},
	{std::string("Unit_Minion_Lane_Ranged"), Unit_Minion_Lane_Ranged},
	{std::string("Unit_Minion_Lane_Siege"), Unit_Minion_Lane_Siege},
	{std::string("Unit_Minion_Lane_Super"), Unit_Minion_Lane_Super},
	{std::string("Unit_Minion_Summon"), Unit_Minion_Summon},
	{std::string("Unit_Minion_SummonName_game_character_displayname_ZyraSeed"), Unit_Minion_SummonName_game_character_displayname_ZyraSeed},
	{std::string("Unit_Minion_Summon_Large"), Unit_Minion_Summon_Large},
	{std::string("Unit_Monster"), Unit_Monster},
	{std::string("Unit_Monster_Blue"), Unit_Monster_Blue},
	{std::string("Unit_Monster_Buff"), Unit_Monster_Buff},
	{std::string("Unit_Monster_Camp"), Unit_Monster_Camp},
	{std::string("Unit_Monster_Crab"), Unit_Monster_Crab},
	{std::string("Unit_Monster_Dragon"), Unit_Monster_Dragon},
	{std::string("Unit_Monster_Epic"), Unit_Monster_Epic},
	{std::string("Unit_Monster_Gromp"), Unit_Monster_Gromp},
	{std::string("Unit_Monster_Krug"), Unit_Monster_Krug},
	{std::string("Unit_Monster_Large"), Unit_Monster_Large},
	{std::string("Unit_Monster_Medium"), Unit_Monster_Medium},
	{std::string("Unit_Monster_Raptor"), Unit_Monster_Raptor},
	{std::string("Unit_Monster_Red"), Unit_Monster_Red},
	{std::string("Unit_Monster_Wolf"), Unit_Monster_Wolf},
	{std::string("Unit_Plant"), Unit_Plant},
	{std::string("Unit_Special"), Unit_Special},
	{std::string("Unit_Special_AzirR"), Unit_Special_AzirR},
	{std::string("Unit_Special_AzirW"), Unit_Special_AzirW},
	{std::string("Unit_Special_CorkiBomb"), Unit_Special_CorkiBomb},
	{std::string("Unit_Special_EpicMonsterIgnores"), Unit_Special_EpicMonsterIgnores},
	{std::string("Unit_Special_KPMinion"), Unit_Special_KPMinion},
	{std::string("Unit_Special_MonsterIgnores"), Unit_Special_MonsterIgnores},
	{std::string("Unit_Special_Peaceful"), Unit_Special_Peaceful},
	{std::string("Unit_Special_SyndraSphere"), Unit_Special_SyndraSphere},
	{std::string("Unit_Special_TeleportTarget"), Unit_Special_TeleportTarget},
	{std::string("Unit_Special_Trap"), Unit_Special_Trap},
	{std::string("Unit_Special_Tunnel"), Unit_Special_Tunnel},
	{std::string("Unit_Special_TurretIgnores"), Unit_Special_TurretIgnores},
	{std::string("Unit_Special_UntargetableBySpells"), Unit_Special_UntargetableBySpells},
	{std::string("Unit_Special_Void"), Unit_Special_Void},
	{std::string("Unit_Special_YorickW"), Unit_Special_YorickW},
	{std::string("Unit_Structure"), Unit_Structure},
	{std::string("Unit_Structure_Inhibitor"), Unit_Structure_Inhibitor},
	{std::string("Unit_Structure_Nexus"), Unit_Structure_Nexus},
	{std::string("Unit_Structure_Turret"), Unit_Structure_Turret},
	{std::string("Unit_Structure_Turret_Inhib"), Unit_Structure_Turret_Inhib},
	{std::string("Unit_Structure_Turret_Inner"), Unit_Structure_Turret_Inner},
	{std::string("Unit_Structure_Turret_Nexus"), Unit_Structure_Turret_Nexus},
	{std::string("Unit_Structure_Turret_Outer"), Unit_Structure_Turret_Outer},
	{std::string("Unit_Structure_Turret_Shrine"), Unit_Structure_Turret_Shrine},
	{std::string("Unit_Ward"), Unit_Ward},
};


void UnitInfo::ImGuiDraw()
{
	ImGui::DragFloat("Acquisition Radius",  &acquisitionRange);
	ImGui::DragFloat("Gameplay Radius",     &gameplayRadius);
	ImGui::DragFloat("Pathing Radius",      &pathRadius);
	ImGui::DragFloat("Selection Radius",    &selectionRadius);
	ImGui::DragFloat("Base Attack Range",   &baseAttackRange);
	ImGui::DragFloat("Base Movement Speed", &baseMovementSpeed);
	ImGui::DragFloat("Basic Attack Speed",  &basicAttackMissileSpeed);
	ImGui::DragFloat("Basic Attack Windup", &basicAttackWindup);
	ImGui::DragFloat("Attack Speed Ratio",  &attackSpeedRatio);
	ImGui::DragFloat("HP Bar Height",       &healthBarHeight);
	ImGui::Text("Tags");
	ImGui::TextColored(ImVec4(0.9f, 0.6f, 0.3f, 1.f), StringifyTags().c_str());
}

bool UnitInfo::HasTag(UnitTag tag)
{
	return tags.test(tag);
}

void UnitInfo::SetTag(std::string& str)
{
	tags.set(TagMapping[str]);
}

std::string UnitInfo::StringifyTags()
{
	//return tags.to_string();
	std::string result("");
	int i = 0;
	for (auto& pair : TagMapping) {
		if (tags.test(pair.second)) {
			if (i > 0)
				result.append(" | ");
			result.append(pair.first);
			i++;
		}
	}
	return result;
}
