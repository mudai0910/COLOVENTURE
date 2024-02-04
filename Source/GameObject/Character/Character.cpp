#include "Character.h"

Character::Character()
	: GameObject()
{
	movement_state=EMovementState::FALLING;
	animation_state=EAnimationState::IDLE;
	is_flip_sprite = false;
	HP = 50;
}

Character::~Character()
{
	Finalize();
}

void Character::Initialize()
{
	__super::Initialize();
}

void Character::Update(float delta_seconds)
{
	__super::Update(delta_seconds);
}

void Character::Draw(const Vector2D& camera_offset)
{
	__super::Draw(camera_offset);
}

void Character::Finalize()
{
	__super::Finalize();
}
void Character::Respawn(Vector2D respawn_point)
{
	ShiftAnimationState(EAnimationState::WALK);
	__super::Respawn(respawn_point);
}

void Character::ShiftMovementState(EMovementState new_movement_state)
{
	if (movement_state == new_movement_state)
	{
		return;
	}
	
	OnLeaveMovementState(movement_state);

	movement_state = new_movement_state;

	OnEnterMovementState(movement_state);
}

void Character::ShiftAnimationState(EAnimationState new_animation_state)
{
	if (animation_state == new_animation_state)
	{
		return;
	}

	OnLeaveAnimationState(animation_state);

	animation_state = new_animation_state;

	OnEnterAnimationState(animation_state);
}

void Character::ApplyDamage(int amount)
{
	//HP‚ª0ˆÈ‰º‚Ìê‡‚Í0‚Ì‚Ü‚Ü
	HP = (HP > 0) ? HP - amount : 0;
}


void Character::OnDestroy()
{
	ShiftAnimationState(EAnimationState::IDLE);
	__super::OnDestroy();
}