#include "cmp_enemy_attack.h"
#include <random>

using namespace std;
using namespace sf;

void EnemyAttackComponent::update(double dt) {

}

EnemyAttackComponent::EnemyAttackComponent(Entity* p) : Component(p) {}

int EnemyAttackComponent::getDamage()
{
	std::random_device dev;
	std::default_random_engine engine(dev());
	std::uniform_real_distribution<float> damage(_minDamage, _maxDamage);

	return damage(engine); // return a random amount of damage
}

void EnemyAttackComponent::setHumanAttack(CombatScene::attackType attack)
{
	_humanPreviousAttack = attack;
}

void EnemyAttackComponent::setEnemyAttack(CombatScene::attackType attack)
{
	_enemyPreviousAttack = attack;
}

CombatScene::attackType EnemyAttackComponent::getEnemyAttack()
{
	return _enemyPreviousAttack;
}

void EnemyAttackComponent::setHumanHealth(float hp)
{
	_humanHP = hp;
}
void EnemyAttackComponent::setHumanMaxHealth(float hp)
{
	_humanMaxHP = hp;
}

int EnemyAttackComponent::getCritChance() {
	return _crit;
}

CombatScene::attackType EnemyAttackComponent::decideAttack() {
	random_device rd;
	default_random_engine engine(rd());
	uniform_int_distribution<int> chance(0, 100);

	// if the player just parried, 80% chance the enemy will parry too as long as the AI didn't parry before
	if (_humanPreviousAttack == CombatScene::attackType::Parry && chance(engine) <= 80 && _enemyPreviousAttack != CombatScene::attackType::Parry)
	{
		return CombatScene::attackType::Parry;
	}
	// if the player is less than 10% health quick attack.
	else if (_humanHP <= (0.1 * _humanMaxHP) && chance(engine) <= 80)
	{
		return CombatScene::attackType::Quick;
	}
	// if the player is between 60% and 100% heavy attack.
	else if (_humanHP >= (0.6 * _humanMaxHP) && _humanHP <= (0.8 * _humanMaxHP) && chance(engine) <= 80)
	{
		return CombatScene::attackType::Heavy;
	}
	// if none of these attacks trigger just normal attack
	else
	{
		return CombatScene::attackType::Normal;
	}
}
