#pragma once
#include <memory>
#include <string>

class Item {
public:
	// Store the different qualities, will dictate textures etc
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


	// return the quality of the item
	Item::Quality getQuality() const {	return _quality; };
	// return the string id of the item
	std::string getItemID() const { return _itemID; }
	// return the numeric id
	int getID() const {	return _ID;	}

	Item::Item(std::string itemID, int id, Quality quality) : _itemID(itemID), _ID(id), _quality(quality) {}
	Item::~Item() {	}

protected:
	Item::Quality _quality;
	std::string _itemID;
	int _ID;
};