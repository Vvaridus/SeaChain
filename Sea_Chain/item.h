#pragma once
#include <memory>
#include <string>

class Item {
public:
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

	Item::Quality getQuality() const {	return _quality; };
	std::string getItemID() const { return _itemID; }
	int getID() const {	return _ID;	}

	Item::Item(std::string itemID, int id, Quality quality) : _itemID(itemID), _ID(id), _quality(quality) {}
	Item::~Item(){}

protected:
	Item::Quality _quality;
	std::string _itemID;
	int _ID;
};