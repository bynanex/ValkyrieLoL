#pragma once
#include "MemoryReadable.h"
#include "Vector.h"
#include "ImGuiDrawable.h"
#include "imgui/imgui.h"
#include "Strings.h"
#include "Offsets.h"
#include "Memory.h"
#include "Logger.h"

#include <string>
#include <windows.h>

class GameObject : MemoryReadable, ImGuiDrawable {

public:
	     GameObject(std::string name);
	void ReadFromBaseAddress(int baseAddr);
	void ImGuiDraw();

public:
	std::string name;
	int         address;

	int         networkId;
	short       index;
	short       team;
	Vector3     pos;
	bool        isVisible;
	float       lastSeen;
};