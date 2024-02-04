#pragma once

class InputManager
{
private:
	InputManager() = default;
	~InputManager() = default;

public:
	InputManager(const InputManager&) = delete;
	InputManager& operator=(const InputManager&) = delete;
	InputManager(InputManager&&) = delete;
	InputManager& operator=(InputManager&&) = delete;

	static InputManager& GetInstance() {
		static InputManager instance;
		return instance;
	}

	void Initialize();
	void Update();

	// key_codeに該当するキーの状態を取得
	bool GetKey(int key_code) const;
	// key_codeに該当するキーが「押された」か判断する
	bool GetKeyDown(int key_code) const;
	// key_codeに該当するキーが「離された」か判断する
	bool GetKeyUp(int key_code) const;

private:
	// 前フレームのキーボードの押下状況
	char prev_state[256];
	// 現フレームのキーボードの押下状況
	char current_state[256];
};
