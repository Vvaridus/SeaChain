#pragma once

#include "engine.h"

class AttackData;

class CombatScene : public Scene {
public:


	void Load() override;

	void UnLoad() override;

	void Update(const double& dt) override;

	void Render() override;

	enum class attackType {
		None,
		Quick,
		Normal,
		Heavy,
		Parry
	};

protected:
	void attack(AttackData ad, std::string attacked);
	AttackData handlePlayerAttack(attackType attack);
	AttackData quickAttack();

	void createButtons();
	void nullify();
};

class AttackData {
public:
	CombatScene::attackType attack;
	int damage;
	float dmgModifier;
	float parryChance;
	bool critChance;
	AttackData() = default;
	AttackData(CombatScene::attackType move, int dam, float mod, float parry, bool crit) : attack(move), damage(dam), dmgModifier(mod), parryChance(parry), critChance(crit) {}
};

