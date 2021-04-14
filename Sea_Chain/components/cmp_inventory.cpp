#include "cmp_inventory.h"
#include <engine.h>

using namespace std;
using namespace sf;

void InventoryComponent::update(double dt) {

}

InventoryComponent::InventoryComponent(Entity* p)
    : Component(p) {
    itemCount = 0;
    itemArray = new Item * [capacity];
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
        delete[] itemArray[i];
    }
    
    nullify();
}

const bool InventoryComponent::add(Item* item) {
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

        delete itemArray[index];
        itemArray[index] = nullptr;
        --itemCount;

        return true;
    }

    return false;
}