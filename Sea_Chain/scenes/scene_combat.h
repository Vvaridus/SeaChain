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
	void escape(const double& dt, bool& changingScene);
	void updateHealthBars(const double& dt);
	void attack(AttackData ad, std::string beingAttacked);
	AttackData getAttackStats(attackType type, std::string attacker);
	void updateLog(std::string attacking, std::string defending, AttackData ad);
	void createButtons();
	void nullify();
};

class AttackData {
public:
	CombatScene::attackType attack;
	int damage;
	bool parrySuccess;
	bool critSuccess;
	AttackData() = default;
	AttackData(CombatScene::attackType move, int dam, bool parry, bool crit) : attack(move), damage(dam), parrySuccess(parry), critSuccess(crit) {}
};

