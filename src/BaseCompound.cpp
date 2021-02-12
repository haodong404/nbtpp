//
// Created by Zachary on 2021/2/7.
//

#include "BaseCompound.h"


const nbtpp::NBT* nbtpp::BaseCompound::getNbt() const {
    return nbt;
}

nbtpp::BaseCompound::BaseCompound(nbtpp::NBT* nbt) : nbt(nbt) {

}

nbtpp::BaseCompound::BaseCompound(const char* filePath, const nbtpp::Edition& edition) {
    if (nbt == nullptr) {
        nbt = new NBT(filePath, edition);
        this->itemMap = nbt->getRootCompound()->itemMap;
        this->internalCompound = nbt->getRootCompound()->internalCompound;
    }
}

nbtpp::BaseCompound::~BaseCompound() {
    delete nbt;
}

