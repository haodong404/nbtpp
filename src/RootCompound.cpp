//
// Created by Zachary on 2021/2/7.
//

#include <fstream>
#include "RootCompound.h"


const nbtpp::NBT* nbtpp::RootCompound::getNbt() const {
    return nbt;
}

nbtpp::RootCompound::RootCompound(nbtpp::NBT* nbt) : nbt(nbt) {

}

nbtpp::RootCompound::RootCompound(const char* filePath, const nbtpp::Edition& edition) {
    if (nbt == nullptr) {
        std::ifstream in;
        in.open(filePath);
        nbt = new NBT(&in, edition);
        this->itemMap = nbt->getRootCompound()->itemMap;
        this->internalCompound = nbt->getRootCompound()->internalCompound;
        in.close();
    }
}

nbtpp::RootCompound::~RootCompound() {
    delete nbt;
}

nbtpp::RootCompound::RootCompound() : Compound() {

}

nbtpp::Hex nbtpp::RootCompound::toHex() {
    Hex hex(getEdition());

    return hex;
}
