#include "Murasuke.h"
#include "DxLib.h"
#include "../../../../Manager/GraphicsManager/GraphicsManager.h"

Murasuke::Murasuke()
	:Enemy()
{
}

Murasuke::~Murasuke()
{
	Finalize();
}

void Murasuke::Initialize()
{
	__super::Initialize();
	
	graphics_handle_buf.resize((int)EAnimationState::DIED+1);

	graphics_handle_buf[(int)EAnimationState::IDLE].resize(ANIM_IDLE_TOTAL_FRAME_NUM);
	GraphicsManager::GetInstance().LoadDivGraph(_T("Resources/Images/murasuke_pp_wait.bmp"),
		ANIM_IDLE_TOTAL_FRAME_NUM, ANIM_IDLE_TOTAL_FRAME_NUM, 1, 40, 48,
		graphics_handle_buf[(int)EAnimationState::IDLE]);

	graphics_handle_buf[(int)EAnimationState::WALK].resize(ANIM_WALK_TOTAL_FRAME_NUM);
	GraphicsManager::GetInstance().LoadDivGraph(_T("Resources/Images/murasuke_pp_walk.bmp"),
		ANIM_WALK_TOTAL_FRAME_NUM, ANIM_WALK_TOTAL_FRAME_NUM, 1, 40, 48,
		graphics_handle_buf[(int)EAnimationState::WALK]);

	graphics_handle_buf[(int)EAnimationState::DAMAGE].resize(ANIM_DAMAGE_TOTAL_FRAME_NUM);
	GraphicsManager::GetInstance().LoadDivGraph(_T("Resources/Images/murasuke_pp_damage.bmp"),
		ANIM_DAMAGE_TOTAL_FRAME_NUM, ANIM_DAMAGE_TOTAL_FRAME_NUM, 1, 40, 48,
		graphics_handle_buf[(int)EAnimationState::DAMAGE]);

	graphics_handle_buf[(int)EAnimationState::DIED].resize(ANIM_DIED_TOTAL_FRAME_NUM);
	GraphicsManager::GetInstance().LoadDivGraph(_T("Resources/Images/enemy_death.bmp"),
		ANIM_DIED_TOTAL_FRAME_NUM, ANIM_DIED_TOTAL_FRAME_NUM, 1, 40, 40,
		graphics_handle_buf[(int)EAnimationState::DIED]);

}
