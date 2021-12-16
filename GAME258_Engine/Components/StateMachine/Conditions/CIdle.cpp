#include "CIdle.h"

#include "../../../Game/Characters/CubePerson.h"

bool CIdle::test()
{
    return !distance(owner->position, SceneGraph::GetInstance()->GetGameObject("Player")->position) <= 19.0f;
}
