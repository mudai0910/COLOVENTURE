#include "InputManager.h"
#include <DxLib.h>


void InputManager::Initialize()
{
	memset(current_state, 0, sizeof(current_state));
	//current_state‚ğƒRƒs[
	memcpy(prev_state, current_state, sizeof(current_state));
}


void InputManager::Update()
{
	memcpy(prev_state, current_state, sizeof(current_state));

	GetHitKeyStateAll(current_state);
}

bool InputManager::GetKey(int key_code) const
{
	return current_state[key_code];
}

bool InputManager::GetKeyDown(int key_code) const
{
	return !prev_state[key_code] && current_state[key_code];
}

bool InputManager::GetKeyUp(int key_code) const
{
	return prev_state[key_code] && !current_state[key_code];
}
