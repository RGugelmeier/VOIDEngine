#include "CFollowPlayer.h"

#include "../../../Game/Characters/CubePerson.h"

//Check if the character is within 30 of the player.
bool CFollowPlayer::test()
{
	return distance(owner->position, SceneGraph::GetInstance()->GetGameObject("Player")->position) <= 19.0f;
}