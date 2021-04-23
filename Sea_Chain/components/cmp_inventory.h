#pragma once
#include <ecm.h>
#include "../item.h"
#include "../weapon.h"
#include <string>	

class InventoryComponent : public Component {
private:
	std::vector<Weapon> weaponItem;
	int itemCount;
	int capacity = 16; // default capacity
	int itemUsing;

public:
	void update(double dt) override;
	void render() override {}
	explicit InventoryComponent(Entity* p);
	InventoryComponent() = delete;

	const int& size() const;
	const int& maxSize() const;

	void clear();
	const bool empty() const;

	const bool addWeapon(Weapon item);
	const bool removeWeapon(const unsigned index);

	const int getUsing();
	const void setUsing(int item);

	Weapon& findWeapon(std::string id);
	Weapon& findWeapon(int index);
};
