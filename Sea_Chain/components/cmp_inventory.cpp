#include "cmp_inventory.h"
#include <engine.h>
#include <logger.h>

using namespace std;
using namespace sf;

void InventoryComponent::update(double dt) {

}

InventoryComponent::InventoryComponent(Entity* p)
    : Component(p) {
    // set the default values
    itemCount = 0;
    itemUsing = -1;
    biscuits = 0;
}

// Get the amount of current items
const int& InventoryComponent::size() const {
    return itemCount;
}

// get the max amount of items
const int& InventoryComponent::maxSize() const {
    return capacity;
}

// clear the inventory
void InventoryComponent::clear() {
    weaponItem.clear();
}

// Add a weapon to the inventory
const bool InventoryComponent::addWeapon(Weapon item) {
    if (itemCount < capacity) {
        weaponItem.push_back(item);
        ++itemCount;

        Logger::addEvent(Logger::EventType::Inventory, Logger::Action::ItemAdded, "");
        return true;
    }

    return false;
}

// Remove a weapon from the inventory
const bool InventoryComponent::removeWeapon(const unsigned index) {
    if (this->itemCount > 0) {
        if (index < 0 || index >= capacity)
            return false;

        weaponItem.erase(weaponItem.begin() + index);
        --itemCount;

        Logger::addEvent(Logger::EventType::Inventory, Logger::Action::ItemRemoved, "");
        return true;
    }

    return false;
}

// get the current index of the item being used
const int InventoryComponent::getUsing()
{
    return itemUsing;
}

// set the index of the current item being used
const void InventoryComponent::setUsing(int item)
{
    itemUsing = item;
}

// get the number of biscuits
const int InventoryComponent::getBiscuits() {
    return biscuits;
}

// set the number of biscuits 
void InventoryComponent::setBiscuits(int amount) {
    Logger::addEvent(Logger::EventType::Inventory, Logger::Action::BiscuitsChanged, "");
    biscuits = amount;
}

// Find a weapon by id
Weapon& InventoryComponent::findWeapon(std::string id)
{
    for (size_t i = 0; i < capacity; i++) {
        if (weaponItem[i].getItemID() == id)
            return move(weaponItem[i]);
    }
}

// find a weapon by index
Weapon& InventoryComponent::findWeapon(int index)
{
    for (size_t i = 0; i < capacity; i++) {
        if (i == index)
            return weaponItem[i];
    }
}
