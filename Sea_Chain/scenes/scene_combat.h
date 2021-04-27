#pragma once

#include "engine.h"

class AttackData;

class CombatScene : public Scene {
public:


	void Load() override;

	void UnLoad() override;

	void Update(const double& dt) override;

	void Render() override;

	int randomNumber(int min, int max);


	enum attackType {
		None,
		Quick,
		Normal,
		Heavy,
		Parry
	};

protected:
	void updateHealthBars(const double& dt);
	void attack(AttackData ad, std::string beingAttacked);
	AttackData getAttackStats(attackType type, std::string attacker);

	void createButtons();
	void nullify();
};

class AttackData {
public:
	CombatScene::attackType attack;
	int damage;
	bool parryChance;
	bool critChance;
	AttackData() = default;
	AttackData(CombatScene::attackType move, int dam, bool parry, bool crit) : attack(move), damage(dam), parryChance(parry), critChance(crit) {}
};

