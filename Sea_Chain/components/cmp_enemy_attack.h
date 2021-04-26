#pragma once

#include "cmp_actor_movement.h"
#include "../scenes/scene_combat.h"

class EnemyAttackComponent : public Component {
protected:
	// Kinda relies on scene_combat.h to be constructed first.
	// This could possibly cause issues.
	CombatScene::attackType _humanPreviousAttack = CombatScene::attackType::None;
	CombatScene::attackType _enemyPreviousAttack = CombatScene::attackType::None;

	// Replace with pointer at some point.
	float _humanHP;
	float _humanMaxHP;

	// enemy attack stats
	int _minDamage = 5;
	int _maxDamage = 50;
	int _crit = 20;


public:
	void update(double dt) override;

	void render() override {}

	explicit EnemyAttackComponent(Entity* p);

	EnemyAttackComponent() = delete;

	// get a random amount of damage between min and max damage
	int getDamage();  

	// set _humanPreviousAttack
	void setHumanAttack(CombatScene::attackType attack);
	// set _humanHP
	void setHumanHealth(float hp);
	// set _humanMaxHealth
	void setHumanMaxHealth(float hp);
	// set _enemyPreviousAttack
	void setEnemyAttack(CombatScene::attackType attack);
	// get _enemyPreviousAttack
	CombatScene::attackType getEnemyAttack();
	// get the crit chance
	int getCritChance();

	// Decide what attack to do.
	CombatScene::attackType decideAttack();
};
