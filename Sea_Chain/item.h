#pragma once
#include <memory>

class Item {
public:
	enum class ItemType {
		MeleeWeapon,
		Tool,
		Food,
		Material
	};

	enum class Quality {
		Wood,
		Stone,
		Tin,
		Lead,
		Copper,
		Bronze,
		Iron,
		Steel
	};

	Item::ItemType getType() const { return _type; };
	Item::Quality getQuality() const {	return _quality; };

	Item::Item(ItemType type, Quality quality) : _type(type), _quality(quality) {}
	Item::~Item(){}

protected:
	Item::ItemType _type;
	Item::Quality _quality;
};