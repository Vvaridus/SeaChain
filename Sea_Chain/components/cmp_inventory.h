#pragma once
#include <ecm.h>
#include "../item.h"

class InventoryComponent : public Component {
private:
	Item** itemArray;
	int itemCount;
	int capacity = 16; // default capacity
	void nullify();

public:
	void update(double dt) override;
	void render() override {}
	explicit InventoryComponent(Entity* p);
	InventoryComponent() = delete;

	const int& size() const;
	const int& maxSize() const;

	void clear();
	const bool empty() const;

	const bool add(Item* item);
	const bool remove(const unsigned index);
};
