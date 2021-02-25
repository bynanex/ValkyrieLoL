#pragma once
#include "HKey.h"
#include "Vector.h"
#include "FakeMouse.h"
#include "GameObject.h"
#include "GameUnit.h"

#include <queue>
#include <chrono>
#include <windows.h>

using namespace std::chrono;

enum ClickType {
	CT_LEFT_CLICK,
	CT_RIGHT_CLICK
};

class IoStep {
	
public:
	void         Start() {};
	virtual bool Update() = 0;
};

class InputController {

public:

	bool    IsDown(HKey key);
	bool    WasPressed(HKey key, float lastMillis = 250.f);
	Vector2 GetMouseCursor();

	void UpdateIssuedOperations();
	void IssuePressKey(HKey key);
	void IssuePressKeyAt(HKey key, std::function<Vector2()> posGetter);
	void IssueClick(ClickType type);
	void IssueClickAt(ClickType type, std::function<Vector2()> posGetter);

	void IssueClickUnit(ClickType type, const GameUnit& unit);

	static int ImGuiKeySelect(const char* label, int key);

public:
	static const float                ScreenWidth;
	static const float                ScreenHeight;
	static const float                WidthRatio;
	static const float                HeightRatio;

private:
	IoStep*                           ioCurrent;
	std::queue<IoStep*>               ioQueue;

	duration<float, std::milli>       timeDiff;
	high_resolution_clock::time_point nowTime;
	high_resolution_clock::time_point lastTimePressed[300] = { high_resolution_clock::now() };
	bool                              pressed[300] = { 0 };
};

class IoDelay : public IoStep {

public:
	IoDelay(float delayMillis) {
		this->delayMillis = delayMillis;
	}

	void Start() {
		startTime = high_resolution_clock::now();
	}

	bool Update() {
		timeDiff = high_resolution_clock::now() - startTime;
		if (timeDiff.count() < delayMillis)
			return false;
		return true;
	}

	float                             delayMillis;
	duration<float, std::milli>       timeDiff;
	high_resolution_clock::time_point startTime;
};

class IoPressKey : public IoStep {

public:
	IoPressKey(HKey key) {
		this->key = key;
	}

	bool Update() {
		INPUT input = { 0 };
		input.type = INPUT_KEYBOARD;
		input.ki.wScan = key;
		input.ki.time = 0;
		input.ki.dwExtraInfo = 0;
		input.ki.wVk = 0;
		input.ki.dwFlags = KEYEVENTF_SCANCODE;
		SendInput(1, &input, sizeof(INPUT));

		return true;
	}

	HKey key;
};

class IoReleaseKey : public IoStep {

public:
	IoReleaseKey(HKey key) {
		this->key = key;
	}

	bool Update() {
		INPUT input = { 0 };
		input.type = INPUT_KEYBOARD;
		input.ki.wScan = key;
		input.ki.time = 0;
		input.ki.dwExtraInfo = 0;
		input.ki.wVk = 0;
		input.ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;
		SendInput(1, &input, sizeof(INPUT));

		return true;
	}

	HKey key;
};

class IoSpoofMouse : public IoStep {

public:
	IoSpoofMouse(Vector2 pos) {
		FakeMouse::FakePositionGetter = [pos] { return pos; };
	}

	IoSpoofMouse(std::function<Vector2()>& getter) {
		FakeMouse::FakePositionGetter = getter;
	}

	bool Update() {
		FakeMouse::Enabled = true;
		return true;
	}
};

class IoUnspoofMouse : public IoStep {

public:
	bool Update() {
		FakeMouse::Enabled = false;
		return true;
	}
};

class IoPressMouse : public IoStep {

public:
	IoPressMouse(ClickType type, std::function<bool()> condition = nullptr) {
		this->type = type;
		this->condition = condition;
	}

	bool Update() {
		if (condition && !condition())
			return true;

		INPUT input = { 0 };
		input.type = INPUT_MOUSE;
		input.mi.dwFlags = (type == ClickType::CT_LEFT_CLICK ? MOUSEEVENTF_LEFTDOWN : MOUSEEVENTF_RIGHTDOWN);
		SendInput(1, &input, sizeof(INPUT));

		return true;
	}

	ClickType type;
	std::function<bool()> condition;
};

class IoReleaseMouse : public IoStep {

public:
	IoReleaseMouse(ClickType type, std::function<bool()> condition = nullptr) {
		this->type = type;
		this->condition = condition;
	}

	bool Update() {
		if (condition && !condition())
			return true;

		INPUT input = { 0 };
		input.type = INPUT_MOUSE;
		input.mi.dwFlags = (type == ClickType::CT_LEFT_CLICK ? MOUSEEVENTF_LEFTUP : MOUSEEVENTF_RIGHTUP);
		SendInput(1, &input, sizeof(INPUT));

		return true;
	}

	ClickType type;
	std::function<bool()> condition;
};

class IoBlockInput : public IoStep {

public:

	bool Update() {
		BlockInput(TRUE);
		return true;
	}
};

class IoUnBlockInput : public IoStep {

public:

	bool Update() {
		BlockInput(FALSE);
		return true;
	}
};