#include "ObjectQuery.h"
#include "Valkyrie.h"

ObjectQuery::ObjectQuery()
{
}

void ObjectQuery::Update(const GameState * state)
{
	this->state = state;
	MakePyObjects(QueryKey::QKEY_MINION,  state->minions);
	MakePyObjects(QueryKey::QKEY_TURRET,  state->turrets);
	MakePyObjects(QueryKey::QKEY_JUNGLE,  state->jungle);
	MakePyObjects(QueryKey::QKEY_MISSILE, state->missiles);
	MakePyObjects(QueryKey::QKEY_CHAMP,   state->champions);
	MakePyObjects(QueryKey::QKEY_OTHERS,  state->others);
}

void ObjectQuery::NewQuery(QueryKey key)
{
	qkey = key;
	conditions.clear();
}

list ObjectQuery::GetResultsPy()
{
	switch (qkey) {
	case QKEY_MINION:
		return MakePyList(pyObjects[QKEY_MINION], state->minions);
	case QKEY_TURRET:
		return MakePyList(pyObjects[QKEY_TURRET], state->turrets);
	case QKEY_JUNGLE:
		return MakePyList(pyObjects[QKEY_JUNGLE], state->jungle);
	case QKEY_MISSILE:
		return MakePyList(pyObjects[QKEY_MISSILE], state->missiles);
	case QKEY_CHAMP:
		return MakePyList(pyObjects[QKEY_CHAMP], state->champions);
	case QKEY_OTHERS:
		return MakePyList(pyObjects[QKEY_OTHERS], state->others);
	default:
		return list();
	}
}

ObjectQuery* ObjectQuery::AllyTo(const GameObject & obj)
{
	teamCondition.team      = obj.team;
	teamCondition.mustEqual = true;
	conditions.push_back(&teamCondition);

	return this;
}

ObjectQuery * ObjectQuery::EnemyTo(const GameObject & obj)
{
	teamCondition.team      = obj.team;
	teamCondition.mustEqual = false;
	conditions.push_back(&teamCondition);

	return this;
}

ObjectQuery * ObjectQuery::NearObj(const GameObject & obj, float distance)
{
	nearbyPointCondition.point = obj.pos;
	nearbyPointCondition.distance = distance;
	conditions.push_back(&nearbyPointCondition);
	return this;
}

ObjectQuery * ObjectQuery::NearPoint(const Vector3 & pt, float distance)
{
	nearbyPointCondition.point = pt;
	nearbyPointCondition.distance = distance;
	conditions.push_back(&nearbyPointCondition);

	return this;
}

ObjectQuery * ObjectQuery::Targetable()
{
	if (qkey == QKEY_MISSILE)
		throw new QueryException("Can't query Targetable for missiles");
	targetableCondition.targetable = true;
	conditions.push_back(&targetableCondition);
	return this;
}

ObjectQuery * ObjectQuery::Untargetable()
{
	if (qkey == QKEY_MISSILE)
		throw new QueryException("Can't query Untargetable for missiles");
	targetableCondition.targetable = false;
	conditions.push_back(&targetableCondition);
	return this;
}

ObjectQuery * ObjectQuery::Visible()
{
	visibilityCondition.visible = true;
	conditions.push_back(&visibilityCondition);
	return this;
}

ObjectQuery * ObjectQuery::Invisible()
{
	visibilityCondition.visible = false;
	conditions.push_back(&visibilityCondition);
	return this;
}

ObjectQuery * ObjectQuery::Alive()
{
	if (qkey == QKEY_MISSILE)
		throw new QueryException("Can't query IsNotDead for missiles");
	deathCondition.death = false;
	conditions.push_back(&deathCondition);
	return this;
}

ObjectQuery * ObjectQuery::Dead()
{
	if (qkey == QKEY_MISSILE)
		throw new QueryException("Can't query IsDead for missiles");
	deathCondition.death = true;
	conditions.push_back(&deathCondition);
	return this;
}

ObjectQuery * ObjectQuery::IsClone()
{
	if (qkey != QKEY_CHAMP)
		throw new QueryException("Can't query IsClone for non champions");
	cloneCondition.clone = true;
	conditions.push_back(&cloneCondition);
	return this;
}

ObjectQuery * ObjectQuery::IsNotClone()
{
	if(qkey != QKEY_CHAMP)
		throw new QueryException("Can't query IsNotClone for non champions");
	cloneCondition.clone = false;
	conditions.push_back(&cloneCondition);
	return this;
}

ObjectQuery * ObjectQuery::IsCasting()
{
	if (qkey == QKEY_MISSILE)
		throw new QueryException("Can't query IsCasting for missiles");

	castingCondition.casting = true;
	conditions.push_back(&castingCondition);
	return this;
}

ObjectQuery* ObjectQuery::OnScreen()
{
	onScreenCondition.renderer = &state->renderer;
	conditions.push_back(&onScreenCondition);

	return this;
}

bool QConditionTeam::Check(const GameObject * obj)
{
	if (mustEqual)
		return obj->team == team;
	return obj->team != team;
}

bool QConditionOnScreen::Check(const GameObject * obj)
{
	return renderer->IsWorldPointOnScreen(obj->pos);
}

bool QConditionNearbyPoint::Check(const GameObject * obj)
{
	return obj->pos.distance(point) < distance;
}

bool QConditionTargetable::Check(const GameObject * obj)
{
	const GameUnit* unit = (const GameUnit*)obj;
	return targetable == (unit->isVisible && !unit->isDead && unit->targetable && !unit->invulnerable);
}

bool QConditionDeath::Check(const GameObject * obj)
{
	const GameUnit* unit = (const GameUnit*)obj;
	return unit->isDead == death;
}

bool QConditionClone::Check(const GameObject * obj)
{
	const GameChampion* unit = (const GameChampion*)(obj);
	return unit->IsClone() == clone;
}

bool QConditionVisibility::Check(const GameObject * obj)
{
	return obj->isVisible == visible;
}

bool QConditionCasting::Check(const GameObject * obj)
{
	const GameUnit* unit = (const GameUnit*)obj;
	return (unit->isCasting && unit->castingSpell.RemainingCastTime() > 0.f) == casting;
}