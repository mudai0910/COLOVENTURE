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

	// key_code�ɊY������L�[�̏�Ԃ��擾
	bool GetKey(int key_code) const;
	// key_code�ɊY������L�[���u�����ꂽ�v�����f����
	bool GetKeyDown(int key_code) const;
	// key_code�ɊY������L�[���u�����ꂽ�v�����f����
	bool GetKeyUp(int key_code) const;

private:
	// �O�t���[���̃L�[�{�[�h�̉�����
	char prev_state[256];
	// ���t���[���̃L�[�{�[�h�̉�����
	char current_state[256];
};
