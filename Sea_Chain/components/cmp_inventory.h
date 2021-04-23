#pragma once
#include <ecm.h>
#include "../item.h"
#include <string>	

class InventoryComponent : public Component {
private:
	std::vector<std::unique_ptr<Item>> itemArray;
	int itemCount;
	int capacity = 16; // default capacity
	void nullify();
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

	const bool add(std::unique_ptr<Item> item);
	const bool remove(const unsigned index);

	const int getUsing();
	const void setUsing(int item);

	std::unique_ptr<Item> find(std::string id);
	std::unique_ptr<Item> find(int id);
};
