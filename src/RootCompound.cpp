//
// Created by Zachary on 2021/2/7.
//

#include "RootCompound.h"


const nbtpp::NBT* nbtpp::RootCompound::getNbt() const {
    return nbt;
}

nbtpp::RootCompound::RootCompound(nbtpp::NBT* nbt) : nbt(nbt) {

}

nbtpp::RootCompound::RootCompound(const char* filePath, const nbtpp::Edition& edition) {
    if (nbt == nullptr) {
        nbt = new NBT(filePath, edition);
        this->itemMap = nbt->getRootCompound()->itemMap;
        this->internalCompound = nbt->getRootCompound()->internalCompound;
    }
}

nbtpp::RootCompound::~RootCompound() {
    delete nbt;
}