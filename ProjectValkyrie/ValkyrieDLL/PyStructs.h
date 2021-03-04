#pragma once
#include <boost/python.hpp>
#include <boost/python/suite/indexing/map_indexing_suite.hpp>

#include "GameObject.h"
#include "GameUnit.h"
#include "GameChampion.h"
#include "GameMissile.h"
#include "GameSpell.h"
#include "GameBuff.h"
#include "GameItemSlot.h"

#include "SpellInfo.h"
#include "ItemInfo.h"
#include "UnitInfo.h"
#include "SpellCast.h"

#include "Color.h"

#include "PyExecutionContext.h"
#include "GameState.h"

using namespace boost::python;

BOOST_PYTHON_MODULE(valkyrie) {
	
	class_<GameBuff>("Buff", "Contains data related to a buff on a champion")
		.def_readonly("name",                &GameBuff::name,        "Name of the buff")
		.def_readonly("time_begin",          &GameBuff::startTime,   "When the buff was received in game time")
		.def_readonly("time_end",            &GameBuff::endTime,     "When the buff will end in game time")
		;

	class_<UnitInfo>("UnitStatic",         "Static data loaded at runtime for an unit")
		.def_readonly("hp_bar_height",       &UnitInfo::healthBarHeight,     "The height at which the health bar starts")
		.def_readonly("movement_speed",      &UnitInfo::baseMovementSpeed,   "Base movement speed")
		.def_readonly("base_atk_range",      &UnitInfo::baseAttackRange,     "Base attack range")
		.def_readonly("base_atk_speed",      &UnitInfo::baseAttackSpeed,     "Base attack speed")
		.def_readonly("atk_speed_ratio",     &UnitInfo::attackSpeedRatio,    "Attack speed ratio")
											 
		.def_readonly("acquisition_radius",  &UnitInfo::acquisitionRange,    "Acquisition radius. See league wiki for more info")
		.def_readonly("selection_radius",    &UnitInfo::selectionRadius,     "Selection radius. See league wiki for more info")
		.def_readonly("pathing_radius",      &UnitInfo::pathRadius,          "Pathing radius. See league wiki for more info")
		.def_readonly("gameplay_radius",     &UnitInfo::gameplayRadius,      "Gameplay radius. See league wiki for more info")
											 
		.def_readonly("basic_atk",           &UnitInfo::GetBasicAttack,      "Default basic attack on the unit")
		.def_readonly("basic_atk_windup",    &UnitInfo::basicAttackWindup,   "Basic attack windup. See league wiki for more info")
		.def_readonly("basic_atk_cast_time", &UnitInfo::basicAttackCastTime, "Basic attack cast time. See league wiki for more info")
		;

	class_<GameItemSlot>("ItemSlot", "Represents an item slot")
		.def_readonly("charges",             &GameItemSlot::charges,          "Charges of item (example: corruption potion charges, number of red potions etc)")
		.def_readonly("item",                &GameItemSlot::GetItemPy,        "Static data of the item. If there is no item on the slot then this is None otherwise it is an ItemStatic instance")
		.def_readonly("active",              &GameItemSlot::GetActivePy,      "Spell active of the item. None if item doesnt have active")
		;

	class_<ItemInfo>("ItemStatic",        "Static data loaded at runtime for an item")
		.def_readonly("id",                &ItemInfo::id,                    "Item internal id")
		.def_readonly("cost",              &ItemInfo::cost,                  "Cost of item in gold coins")
		.def_readonly("mov_speed",         &ItemInfo::movementSpeed,         "Flat movement speed given by item")
		.def_readonly("health",            &ItemInfo::health,                "Flat health given by item")
		.def_readonly("crit",              &ItemInfo::crit,                  "Crit chance given by item")
		.def_readonly("ap",                &ItemInfo::abilityPower,          "Flat ability power given by item")
		.def_readonly("mana",              &ItemInfo::mana,                  "Flat mana given by item")
		.def_readonly("armor",             &ItemInfo::armour,                "Flat armour given by item")
		.def_readonly("magic_res",         &ItemInfo::magicResist,           "Flat magic resist given by item")
		.def_readonly("phys_dmg",          &ItemInfo::physicalDamage,        "Flat physical damage given by item")
		.def_readonly("atk_speed",         &ItemInfo::attackSpeed,           "Attack speed percent given by item")
		.def_readonly("life_steal",        &ItemInfo::lifeSteal,             "Life steal percent given by item")
		.def_readonly("hp_regen",          &ItemInfo::hpRegen,               "Health regen percent given by item")
		.def_readonly("mov_speed_percent", &ItemInfo::movementSpeedPercent,  "Movement speed percent given by item")
		;

	class_<SpellInfo>("SpellStatic",      "Static data loaded at runtime for a spell")
		.def_readonly("icon",              &SpellInfo::icon,                 "Icon name of the spell in lowercase")
		.def_readonly("cast_time",         &SpellInfo::castTime,             "Cast time of spell")
		.def_readonly("cast_range",        &SpellInfo::castRange,            "Cast range of the spell (e.g Ezreal Q length)")
		.def_readonly("cast_radius",       &SpellInfo::castRadius,           "Cast radius of the spell (e.g Ziggs R area of effect)")
		.def_readonly("width",             &SpellInfo::width,                "Width of the spell")
		.def_readonly("height",            &SpellInfo::height,               "The height at which the spell starts. Use this for drawing purposes")
		.def_readonly("speed",             &SpellInfo::speed,                "Speed of the spell. Used mostly by missile spells")
		.def_readonly("travel_time",       &SpellInfo::travelTime,           "Flat travel time in seconds. Use this if not 0 instead of calculating travel time using speed")
		.def_readonly("name",              &SpellInfo::name,                 "Name of the spell in lower case")
		;

	class_<GameObject>("Obj",             "Represents the base of a ingame object. Most ingame objects derive from this.")
		.def_readonly("name",              &GameObject::name,                "Name of the object in lower case")
		.def_readonly("index",             &GameObject::index,               "Index of the object. This is not a unique id, you use this for targeted spells to find who the target is")
		.def_readonly("net_id",            &GameObject::networkId,           "Network id. This is a unique id of the object")
		.def_readonly("team",              &GameObject::team,                "Team of the object: possible values 100, 200 and 300 for jungle side. Use ally_to/enemy_to functions instead of this")
		.def_readonly("pos",               &GameObject::pos,                 "Position of the object")
		.def_readonly("visible",           &GameObject::isVisible,           "Object visibility")
		.def_readonly("last_seen",         &GameObject::lastSeen,            "Timestamp in game time for when the object was last visible")
																             
		.def("ally_to",                    &GameObject::IsAllyTo,            "Checks if two objects are allied")
		.def("enemy_to",                   &GameObject::IsEnemyTo,           "Checks if two objects are enemies")
		.def("__eq__",                     &GameObject::EqualsTo,            "Checks if two object are identical by checking their network id")
		;

	class_<GameSpell>("Spell",            "Represents a spell in game. In case of spells from items make sure level > 0 before casting it.")
		.def_readonly("name",              &GameSpell::name,                 "Name of the spell in lower case")
		.def_readonly("lvl",               &GameSpell::lvl,                  "Level of the spell. Has value 0 when spell is not learned")
		.def_readonly("ready_at",          &GameSpell::readyAt,              "Timestamp in game time for when the cooldown of the spell ends")
		.def_readonly("value",             &GameSpell::value,                "Value of spells. Holds the value of the spell for summoner spells like ignite/smite it holds the damage. For champion spells this value is usually 0")
		.def_readonly("cd",                &GameSpell::GetRemainingCooldown, "The remaining cooldown of the spell. Internally it is calculated using ready_at")

		.def_readonly("static",            &GameSpell::GetStaticData,        "Gets static information loaded at runtime about the spell. Can be None but normally shouldn't. If you find a object for which this is null please contact a dev")
		;

	class_<SpellCast>("SpellCast",         "Has data about a spell cast.")
		.def_readonly("start_pos",         &SpellCast::start,                "Start position of the spell. Usually the position of the caster")
		.def_readonly("end_pos",           &SpellCast::end,                  "End position of the spell. Use this to get the direction of the spell.")
		.def_readonly("src_index",         &SpellCast::srcIndex,             "Index of the object who is casting")
		.def_readonly("dest_index",        &SpellCast::destIndex,            "If casting a targeted spell this holds the index of the target object")
		.def_readonly("time_begin",        &SpellCast::timeBegin,            "Start timestamp in game time of the casting")
		.def_readonly("cast_time",         &SpellCast::castTime,             "Total cast time")
		.def_readonly("static",            &SpellCast::GetStaticData,        "Static data of the spell being cast. Can be None but normally shouldn't. If you find a object for which this is null please contact a dev")
		;

	class_<GameMissile, bases<GameObject>>("MissileObj")
		.def_readonly("spell",             &GameMissile::GetSpell,           "The spell of the missile. A spell can have multiple missiles. So this is not necessarily the spell that was cast.")
		;

	class_<GameUnit, bases<GameObject>>("UnitObj", "Represents a base unit object")
		.def_readonly("dead",              &GameUnit::isDead,                "Used to check if unit is dead")
		.def_readonly("targetable",        &GameUnit::targetable,            "Used to check if unit is targetable")
		.def_readonly("invulnerable",      &GameUnit::invulnerable,          "Used to check if unit is invulnerable")
		.def_readonly("mana",              &GameUnit::mana,                  "Current mana of the unit")
		.def_readonly("health",            &GameUnit::health,                "Current health of the unit")
		.def_readonly("max_health",        &GameUnit::maxHealth,             "Max health of the unit")
		.def_readonly("armor",             &GameUnit::armor,                 "Armour of the unit")
		.def_readonly("magic_res",         &GameUnit::magicRes,              "Magic resist of the unit")
		.def_readonly("base_atk",          &GameUnit::baseAtk,               "Base physical damage of the unit")
		.def_readonly("bonus_atk",         &GameUnit::bonusAtk,              "Bonus physical damage of the unit (from items, buffs etc)")
		.def_readonly("move_speed",        &GameUnit::moveSpeed,             "Movement speed of the unit")
		.def_readonly("lvl",               &GameUnit::lvl,                   "Level of the unit")
		.def_readonly("expiry",            &GameUnit::expiry,                "Expiration duration in seconds. Used for units like wards")
		.def_readonly("crit",              &GameUnit::crit,                  "Crit chance of the unit as a decimal")
		.def_readonly("crit_multi",        &GameUnit::critMulti,             "Crit multi of the unit as a decimal")
		.def_readonly("ap",                &GameUnit::abilityPower,          "Flat ability power of the unit")
		.def_readonly("atk_speed_multi",   &GameUnit::atkSpeedMulti,         "Attack speed multiplier of the unit as a decimal. Multiply this with base attack speed to get current attack speed")
		.def_readonly("atk_range",         &GameUnit::attackRange,           "Attack range of the unit")
		.def_readonly("atk_speed",         &GameUnit::GetAttackSpeed,        "Calculates the attack speed of the unit")

		.def_readonly("curr_casting",      &GameUnit::GetCastingSpell,       "Currently casting spell by the unit")
		.def_readonly("static",            &GameUnit::GetStaticData,         "Static data loaded at runtime of the unit. Can be None but normally shouldn't. If you find a object for which this is null please contact a dev")
		.def_readonly("is_ranged",         &GameUnit::IsRanged,              "True if unit is ranged")

		.def("has_tags",                   &GameUnit::HasTags,               "Checks if the unit has unit tags (see Unit class)")

		;

	class_<GameChampion, bases<GameUnit>>("ChampionObj", "Represents a champion object")
		.def("has_buff",                   &GameChampion::HasBuff,           "Check if champion has buff. The buff name is case sensitive")
		.def_readonly("buffs",             &GameChampion::BuffsToPy,         "List of all the buffs on the champion. Currently buffs are only read for the player champion due to performance reasons.")
		.def_readonly("spells",            &GameChampion::SpellsToPy,        "List of all the champion spells. Remarks: First 4 spells are Q,W,E,R. Next two are D,F.The next 6 are item spells. Use Context.cast_spell to cast them. Only enemies and local player have item actives read for performance reasons")
		.def_readonly("item_slots",        &GameChampion::ItemsToPy,         "List of inventory slots. If an item is on the slot then the value is an Item object otherwise None. Only local player and enemies have items read for performance reasons")
		.def_readonly("hpbar_pos",         &GameChampion::GetHpBarPosition,  "Height position of the HP bar of the champion")
		.def_readonly("recalling",         &GameChampion::recalling,         "True if champion is recalling")
		;

	class_<GameTurret, bases<GameUnit>>("TurretObj", "Represents a turret object")
		;

	class_<GameMinion, bases<GameUnit>>("MinionObj", "Represents a minion object")
		.def_readonly("hpbar_pos",         &GameMinion::GetHpBarPosition,    "Height position of the HP bar of the minion")
		;
	
	class_<GameJungle, bases<GameUnit>>("JungleMobObj", "Represents a jungle mob object")
		;

	class_<PyImGui>("UI", "Used to draw imgui backed UIs. Each method is more or less the equivalent of the original imgui method. Check imgui documentation for more info")
		.def("begin",                    &PyImGui::Begin)
		.def("begin",                    &PyImGui::BeginWithFlags)
		.def("end",                      &PyImGui::End)
						                 
		.def("button",                   &PyImGui::Button)
		.def("button",                   &PyImGui::ColorButton)
		.def("colorpick",                &PyImGui::ColorPicker)
		.def("checkbox",                 &PyImGui::Checkbox)
		.def("text",                     &PyImGui::Text)
		.def("text",                     &PyImGui::TextColored)
		.def("labeltext",                &PyImGui::LabelText)
		.def("labeltext",                &PyImGui::LabelTextColored)
		.def("separator",                &PyImGui::Separator)
		.def("dragint",                  &PyImGui::DragInt,   PyImGui::DragIntOverloads())
		.def("dragfloat",                &PyImGui::DragFloat, PyImGui::DragFloatOverloads())
		.def("keyselect",                &PyImGui::KeySelect)
		.def("sliderfloat",              &PyImGui::SliderFloat)
		.def("sliderint",                &PyImGui::SliderInt)
		.def("sliderenum",               &PyImGui::SliderEnum)
		.def("progressbar",              &PyImGui::ProgressBar)
		.def("image",                    &PyImGui::Image,     PyImGui::ImageOverloads())

		.def("header",                   &PyImGui::CollapsingHeader)
		.def("treenode",                 &PyImGui::TreeNode)
		.def("treepop",                  &PyImGui::TreePop)
		.def("beginmenu",                &PyImGui::BeginMenu)
		.def("endmenu",                  &PyImGui::EndMenu)
		.def("opennext",                 &PyImGui::SetNextItemOpen)
		.def("openpopup",                &PyImGui::OpenPopup)
		.def("beginpopup",               &PyImGui::BeginPopup)
		.def("endpopup",                 &PyImGui::EndPopup)
		.def("beginmodal",               &PyImGui::BeginPopupModal)
		.def("closepopup",               &PyImGui::CloseCurrentPopup)
		.def("selectable",               &PyImGui::Selectable)

		.def("begintbl",                 &PyImGui::BeginTable)
		.def("endtbl",                   &PyImGui::EndTable)
		.def("tblnextrow",               &PyImGui::TableNextRow)
		.def("tblcolumn",                &PyImGui::TableSetColumn)
		.def("inputtext",                &PyImGui::InputText)
						                 
		.def("sameline",                 &PyImGui::SameLine)
		.def("begingroup",               &PyImGui::BeginGroup)
		.def("endgroup",                 &PyImGui::EndGroup)
						                 
		.def("listbox",                  &PyImGui::ListBox)
		.def("combo",                    &PyImGui::Combo)
		.def("demo",                     &PyImGui::Demo, "Shows the famous imgui demo")

		.def("pushid",                   &PyImGui::PushId)
		.def("popid",                    &PyImGui::PopId)
		;

	class_<PyExecutionContext>("Context", "Contains everything necessarry to create scripts. From utility functions to game data")
		.def("info",                     &PyExecutionContext::LogInfo,           "Logs an info message to the console & file log")
		.def("warn",                     &PyExecutionContext::LogWarning,        "Logs a warning message to the console & file log")
		.def("error",                    &PyExecutionContext::LogError,          "Logs a error message to the console & file log. The file log will be flushed. Use this in absolute worse cases")
		.def_readonly("ui",              &PyExecutionContext::GetImGuiInterface, "UI interface for drawing menus based on imgui")
		.def_readonly("cfg",             &PyExecutionContext::GetConfig,         "The script config interface. Used to load/save settings")
		.def_readonly("time",            &PyExecutionContext::time,              "Current game duration in seconds")
		.def_readonly("ping",            &PyExecutionContext::ping,              "Current ping of the game")

		.def_readonly("hovered",         &PyExecutionContext::hovered,           "Gets the game object under the mouse")
		.def_readonly("player",          &PyExecutionContext::player,            "The champion used by the local player. In replays this will be a random champion")
		.def_readonly("champs",          &PyExecutionContext::GetChampions,      "Get champions from game")
		.def_readonly("turrets",         &PyExecutionContext::GetTurrets,        "Get turrets from game")
		.def_readonly("missiles",        &PyExecutionContext::GetMissiles,       "Get missile spells from game")
		.def_readonly("minions",         &PyExecutionContext::GetMinions,        "Get minions from game")
		.def_readonly("jungle",          &PyExecutionContext::GetJungle,         "Get jungle monsters from game")
		.def_readonly("others",          &PyExecutionContext::GetOthers,         "Get other uncategorized objects")

		.def("attack",                   &PyExecutionContext::AttackUnit,        "Makes the player attack the given unit")
		.def("move",                     &PyExecutionContext::MoveToLocation,    "Moves the player to the given location")
		.def("move",                     &PyExecutionContext::MoveToMouse,       "Moves the player to where the mouse cursor is")

		.def("is_held",                  &PyExecutionContext::IsKeyDown,         "Checks if key is held down")
		.def("was_pressed",              &PyExecutionContext::WasKeyPressed,     "Checks if key was pressed")

		.def("cast_spell",               &PyExecutionContext::CastSpell,         "Casts a spell on a location. This function will check if spell is castable automatically. It doesnt check for item charge availability.")
		.def("get_spell",                &PyExecutionContext::GetSpellInfo,      "Gets static spell info. Argument must be lower case")

		.def("is_at_spawn",              &PyExecutionContext::IsInFountain,      "Checks if the object is in the fountain of his team")
		.def("is_on_screen",             &PyExecutionContext::IsScreenPointOnScreen, PyExecutionContext::IsScreenPointOnScreenOverloads())
		.def("is_on_screen",             &PyExecutionContext::IsWorldPointOnScreen,  PyExecutionContext::IsWorldPointOnScreenOverloads())
		.def("w2s",                      &PyExecutionContext::World2Screen,      "Converts a world space point to screen space")
		.def("w2m",                      &PyExecutionContext::World2Minimap,     "Converts a world space point to minimap space")
		.def("d2m",                      &PyExecutionContext::DistanceOnMinimap, "Converts a distance value from world space to minimap space")
										    
		.def("line",                     &PyExecutionContext::DrawLine)
		.def("line",                     &PyExecutionContext::DrawLineWorld)
		.def("circle",                   &PyExecutionContext::DrawCircle)
		.def("circle_fill",              &PyExecutionContext::DrawCircleFilled)
		.def("circle",                   &PyExecutionContext::DrawCircleWorld)
		.def("circle_fill",              &PyExecutionContext::DrawCircleWorldFilled)
		.def("text",                     &PyExecutionContext::DrawTxt)
		.def("rect",                     &PyExecutionContext::DrawRect,             PyExecutionContext::DrawRectOverloads())
		.def("rect_fill",                &PyExecutionContext::DrawRectFilled,       PyExecutionContext::DrawRectFilledOverloads())
		.def("rect",                     &PyExecutionContext::DrawRectWorld)
		.def("triangle",                 &PyExecutionContext::DrawTriangleWorld)
		.def("triangle_fill",            &PyExecutionContext::DrawTriangleWorldFilled)
		.def("image",                    &PyExecutionContext::DrawImage)
		.def("image",                    &PyExecutionContext::DrawImageRounded)
		.def("pill",                     &PyExecutionContext::Pill,             "Draws a pill with a text under the player. Pill positions are automatically managed each frame so that they dont overlap.")
		;

	class_<ConfigSet>("Config", "Interface for saving & loading script configurations")
		.def("set_int",                  &ConfigSet::SetInt,    "Saves an integer to the config file")
		.def("set_bool",                 &ConfigSet::SetBool,   "Saves a bool to the config file")
		.def("set_float",                &ConfigSet::SetFloat,  "Saves a float to the config file")
		.def("set_str",                  &ConfigSet::SetStr,    "Saves a string to the config file")
		.def("get_int",                  &ConfigSet::GetInt,    "Gets an integer from the config file")
		.def("get_bool",                 &ConfigSet::GetBool,	"Gets a bool from the config file")
		.def("get_float",                &ConfigSet::GetFloat,	"Gets a float from the config file")
		.def("get_str",                  &ConfigSet::GetStr,	"Gets a string from the config file")
		;

	class_<ImVec4>("Col", init<float, float, float, float>())
		.def_readonly("Black",           &Color::BLACK)
		.def_readonly("White",           &Color::WHITE)
		.def_readonly("Red",             &Color::RED)
		.def_readonly("DarkRed",         &Color::DARK_RED)
		.def_readonly("Green",           &Color::GREEN)
		.def_readonly("DarkGreen",       &Color::DARK_GREEN)
		.def_readonly("Yellow",          &Color::YELLOW)
		.def_readonly("DarkYellow",      &Color::DARK_YELLOW)
		.def_readonly("Cyan",            &Color::CYAN)
		.def_readonly("Purple",          &Color::PURPLE)
		.def_readonly("Gray",            &Color::GRAY)
		.def_readonly("Orange",          &Color::ORANGE)
		.def_readonly("Blue",            &Color::BLUE)
		.def_readonly("Brown",           &Color::BROWN)

		.def_readwrite("r",              &ImVec4::x,  "Red value of color")
		.def_readwrite("g",              &ImVec4::y,  "Green value of color")
		.def_readwrite("b",              &ImVec4::z,  "Blue value of color")
		.def_readwrite("a",              &ImVec4::w,  "Alpha value of color")
		;

	class_<Vector4>("Vec4", init<float, float, float, float>())
		.def_readwrite("x",              &Vector4::x)
		.def_readwrite("y",              &Vector4::y)
		.def_readwrite("z",              &Vector4::z)
		.def_readwrite("w",              &Vector4::w)
		.def("length",                   &Vector4::length)
		.def("normalize",                &Vector4::normalize)
		.def("distance",                 &Vector4::distance)
		.def("__mul__",                  &Vector4::scale)
		.def("__mul__",                  &Vector4::vscale)
		.def("__add__",                  &Vector4::add)
		.def("__sub__",                  &Vector4::sub)
		.def("clone",                    &Vector4::clone)
		;

	class_<Vector3>("Vec3", init<float, float, float>())
		.def_readwrite("x",              &Vector3::x)
		.def_readwrite("y",              &Vector3::y)
		.def_readwrite("z",              &Vector3::z)
		.def("length",                   &Vector3::length)
		.def("normalize",                &Vector3::normalize)
		.def("distance",                 &Vector3::distance)
		.def("__mul__",                  &Vector3::scale)
		.def("__mul__",                  &Vector3::vscale)
		.def("rotate_x",                 &Vector3::rotate_x, "Rotates the vector along the x axis")
		.def("rotate_y",                 &Vector3::rotate_y, "Rotates the vector along the y axis")
		.def("rotate_z",                 &Vector3::rotate_z, "Rotates the vector along the z axis")
		.def("__add__",                  &Vector3::add)
		.def("__sub__",                  &Vector3::sub)
		.def("clone",                    &Vector3::clone)
		;

	class_<Vector2>("Vec2", init<float, float>())
		.def_readwrite("x",              &Vector2::x)
		.def_readwrite("y",              &Vector2::y)
		.def("length",                   &Vector2::length)
		.def("normalize",                &Vector2::normalize)
		.def("distance",                 &Vector2::distance)
		.def("__mul__",                  &Vector2::scale)
		.def("__mul__",                  &Vector2::vscale)
		.def("__add__",                  &Vector2::add)
		.def("__sub__",                  &Vector2::sub)
		.def("clone",                    &Vector2::clone)
		;

	enum_<ImGuiWindowFlags>("WindowFlag", "Represents imgui window flags. These support bitwise operations so WindowFlag.NoResize | WindowFlag.NoMove would yield a flag with both")
		.value("None",                      ImGuiWindowFlags_None)
		.value("NoTitleBar",                ImGuiWindowFlags_NoTitleBar)
		.value("NoResize",                  ImGuiWindowFlags_NoResize)
		.value("NoMove",                    ImGuiWindowFlags_NoMove)
		.value("NoScrollbar",               ImGuiWindowFlags_NoScrollbar)
		.value("NoScrollWithMouse",         ImGuiWindowFlags_NoScrollWithMouse)
		.value("NoCollapse",                ImGuiWindowFlags_NoCollapse)
		.value("AlwaysAutoResize",          ImGuiWindowFlags_AlwaysAutoResize)
		.value("NoBackground",              ImGuiWindowFlags_NoBackground)
		.value("NoSavedSettings",           ImGuiWindowFlags_NoSavedSettings)
		.value("NoMouseInputs",             ImGuiWindowFlags_NoMouseInputs)
		.value("MenuBar",                   ImGuiWindowFlags_MenuBar)
		.value("HorizontalScrollbar",       ImGuiWindowFlags_HorizontalScrollbar)
		.value("NoFocusOnAppearing",        ImGuiWindowFlags_NoFocusOnAppearing)
		.value("NoBringToFrontOnFocus",     ImGuiWindowFlags_NoBringToFrontOnFocus)
		.value("AlwaysVerticalScrollbar",   ImGuiWindowFlags_AlwaysVerticalScrollbar)
		.value("AlwaysHorizontalScrollbar", ImGuiWindowFlags_AlwaysHorizontalScrollbar)
		.value("AlwaysUseWindowPadding",    ImGuiWindowFlags_AlwaysUseWindowPadding)
		.value("NoNavInputs",               ImGuiWindowFlags_NoNavInputs)
		.value("NoNavFocus",                ImGuiWindowFlags_NoNavFocus)
		.value("UnsavedDocument",           ImGuiWindowFlags_UnsavedDocument)
		.value("NoNav",                     ImGuiWindowFlags_NoNav)
		.value("NoDecoration",              ImGuiWindowFlags_NoDecoration)
		.value("NoInputs",                  ImGuiWindowFlags_NoInputs)
		;
	
	enum_<UnitTag>("Unit", "Riot unit tags extracted from the game files. These are not compatible with bitwise operations so writing things like Unit.Monster | Unit.Plant will not yield a tag that has both of those.")
		.value("Champion",                    UnitTag::Unit_Champion)
		.value("ChampionClone",               UnitTag::Unit_Champion_Clone)
		.value("IsolationNonImpacting",       UnitTag::Unit_IsolationNonImpacting)
		.value("KingPoro",                    UnitTag::Unit_KingPoro)
		.value("Minion",                      UnitTag::Unit_Minion)
		.value("MinionLane",                  UnitTag::Unit_Minion_Lane)
		.value("MinionLaneMelee",             UnitTag::Unit_Minion_Lane_Melee)
		.value("MinionLaneRanged",            UnitTag::Unit_Minion_Lane_Ranged)
		.value("MinionLaneSiege",             UnitTag::Unit_Minion_Lane_Siege)
		.value("MinionLaneSuper",             UnitTag::Unit_Minion_Lane_Super)
		.value("MinionSummon",                UnitTag::Unit_Minion_Summon)
		.value("MinionSummon_Large",          UnitTag::Unit_Minion_Summon_Large)
		.value("Monster",                     UnitTag::Unit_Monster)
		.value("MonsterBlue",                 UnitTag::Unit_Monster_Blue)
		.value("MonsterBuff",                 UnitTag::Unit_Monster_Buff)
		.value("MonsterCamp",                 UnitTag::Unit_Monster_Camp)
		.value("MonsterCrab",                 UnitTag::Unit_Monster_Crab)
		.value("MonsterDragon",               UnitTag::Unit_Monster_Dragon)
		.value("MonsterEpic",                 UnitTag::Unit_Monster_Epic)
		.value("MonsterGromp",                UnitTag::Unit_Monster_Gromp)
		.value("MonsterKrug",                 UnitTag::Unit_Monster_Krug)
		.value("MonsterLarge",                UnitTag::Unit_Monster_Large)
		.value("MonsterMedium",               UnitTag::Unit_Monster_Medium)
		.value("MonsterRaptor",               UnitTag::Unit_Monster_Raptor)
		.value("MonsterRed",                  UnitTag::Unit_Monster_Red)
		.value("MonsterWolf",                 UnitTag::Unit_Monster_Wolf)
		.value("Plant",                       UnitTag::Unit_Plant)
		.value("Special",                     UnitTag::Unit_Special)
		.value("SpecialAzirR",                UnitTag::Unit_Special_AzirR)
		.value("SpecialAzirW",                UnitTag::Unit_Special_AzirW)
		.value("SpecialCorkiBomb",            UnitTag::Unit_Special_CorkiBomb)
		.value("SpecialEpicMonsterIgnores",   UnitTag::Unit_Special_EpicMonsterIgnores)
		.value("SpecialKPMinion",             UnitTag::Unit_Special_KPMinion)
		.value("SpecialMonsterIgnores",       UnitTag::Unit_Special_MonsterIgnores)
		.value("SpecialPeaceful",             UnitTag::Unit_Special_Peaceful)
		.value("SpecialSyndraSphere",         UnitTag::Unit_Special_SyndraSphere)
		.value("SpecialTeleportTarget",       UnitTag::Unit_Special_TeleportTarget)
		.value("SpecialTrap",                 UnitTag::Unit_Special_Trap)
		.value("SpecialTunnel",               UnitTag::Unit_Special_Tunnel)
		.value("SpecialTurretIgnores",        UnitTag::Unit_Special_TurretIgnores)
		.value("SpecialUntargetableBySpells", UnitTag::Unit_Special_UntargetableBySpells)
		.value("SpecialVoid",                 UnitTag::Unit_Special_Void)
		.value("SpecialYorickW",              UnitTag::Unit_Special_YorickW)
		.value("Structure",                   UnitTag::Unit_Structure)
		.value("StructureInhibitor",          UnitTag::Unit_Structure_Inhibitor)
		.value("StructureNexus",              UnitTag::Unit_Structure_Nexus)
		.value("StructureTurret",             UnitTag::Unit_Structure_Turret)
		.value("StructureTurretInhib",        UnitTag::Unit_Structure_Turret_Inhib)
		.value("StructureTurretInner",        UnitTag::Unit_Structure_Turret_Inner)
		.value("StructureTurretNexus",        UnitTag::Unit_Structure_Turret_Nexus)
		.value("StructureTurretOuter",        UnitTag::Unit_Structure_Turret_Outer)
		.value("StructureTurretShrine",       UnitTag::Unit_Structure_Turret_Shrine)
		.value("Ward",                        UnitTag::Unit_Ward)
		;
}