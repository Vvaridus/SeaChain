#include "cmp_inventory.h"
#include <engine.h>
#include <logger.h>

using namespace std;
using namespace sf;

void InventoryComponent::update(double dt) {

}

InventoryComponent::InventoryComponent(Entity* p)
    : Component(p) {
    itemCount = 0;
    itemUsing = -1;
    biscuits = 0;
}

const int& InventoryComponent::size() const {
    return itemCount;
}

const int& InventoryComponent::maxSize() const {
    return capacity;
}

void InventoryComponent::clear() {
    weaponItem.clear();
}

//const bool InventoryComponent::add(std::unique_ptr<Item> item) {
//    if (itemCount < capacity) {
//        itemArray.push_back(move(item));
//
//        return true;
//    }
//
//    return false;
//}

const bool InventoryComponent::addWeapon(Weapon item) {
    if (itemCount < capacity) {
        weaponItem.push_back(item);
        ++itemCount;

        Logger::addEvent(Logger::EventType::Inventory, Logger::Action::ItemAdded, "");
        return true;
    }

    return false;
}

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

const int InventoryComponent::getUsing()
{
    return itemUsing;
}

const void InventoryComponent::setUsing(int item)
{
    itemUsing = item;
}

const int InventoryComponent::getBiscuits() {
    return biscuits;
}

void InventoryComponent::setBiscuits(int amount) {
    Logger::addEvent(Logger::EventType::Inventory, Logger::Action::BiscuitsChanged, "");
    biscuits = amount;
}

Weapon& InventoryComponent::findWeapon(std::string id)
{
    for (size_t i = 0; i < capacity; i++) {
        if (weaponItem[i].getItemID() == id)
            return move(weaponItem[i]);
    }
}

Weapon& InventoryComponent::findWeapon(int index)
{
    for (size_t i = 0; i < capacity; i++) {
        if (i == index)
            return weaponItem[i];
    }
}
