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
	virtual Item::Quality getQuality() const {	return _quality; };
	// return the string id of the item
	virtual std::string getItemID() const { return _itemID; }
	// return the numeric id
	Item() { }
	Item::Item(std::string itemID,  Quality quality) : _itemID(itemID),  _quality(quality) {}
	virtual Item::~Item() {	}

protected:
	Item::Quality _quality;
	std::string _itemID;
};