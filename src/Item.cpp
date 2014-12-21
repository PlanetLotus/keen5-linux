#include "Item.h"

Item::Item(ItemTypeEnum type, int value) {
    this->type = type;
    this->value = value;

    // Animations depend on item type
}

int Item::getValue() { return value; }
ItemTypeEnum Item::getType() { return type; }
