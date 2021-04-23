#include "cmp_inventory.h"
#include <engine.h>

using namespace std;
using namespace sf;

void InventoryComponent::update(double dt) {

}

InventoryComponent::InventoryComponent(Entity* p)
    : Component(p) {
    itemCount = 0;
    nullify();
}

void InventoryComponent::nullify() {
    for (size_t i = 0; i < capacity; i++) {
        itemArray[i] = nullptr;
    }
}

const int& InventoryComponent::size() const {
    return itemCount;
}

const int& InventoryComponent::maxSize() const {
    return capacity;
}

void InventoryComponent::clear() {
    for (size_t i = 0; i < itemCount; i++) {
        delete[] itemArray[i].get();
    }
    
    nullify();
}

const bool InventoryComponent::add(std::unique_ptr<Item> item) {
    if (itemCount < capacity) {
        itemArray[itemCount++] = move(item);

        return true;
    }

    return false;
}

const bool InventoryComponent::remove(const unsigned index) {
    if (this->itemCount > 0) {
        if (index < 0 || index >= capacity)
            return false;

        delete itemArray[index].get();
        itemArray[index] = nullptr;
        --itemCount;

        return true;
    }

    return false;
}

const int InventoryComponent::getUsing()
{
    return itemUsing;
}

const void InventoryComponent::setUsing(int item)
{
    itemUsing = item;
}

std::unique_ptr<Item> InventoryComponent::find(std::string id)
{
    for (size_t i = 0; i < capacity; i++) {
        if (itemArray[i]->getItemID() == id)
            return move(itemArray[i]);
    }

    return nullptr;
}

std::unique_ptr<Item> InventoryComponent::find(int id)
{
    for (size_t i = 0; i < capacity; i++) {
        if (itemArray[i]->getID() == id)
            return move(itemArray[i]);
    }

    return nullptr;
}
