//
// Created by Zachary on 2021/2/7.
//

#include "BaseCompound.h"


void nbtpp::BaseCompound::bindName() {

}

const nbtpp::NBT* nbtpp::BaseCompound::getNbt() const {
    return nbt;
}

nbtpp::BaseCompound::BaseCompound(nbtpp::NBT* nbt) : nbt(nbt) {
    bindName();
}

nbtpp::BaseCompound::BaseCompound(const char* filePath, const nbtpp::Edition& edition) {
    if (nbt == nullptr) {
        nbt = new NBT(filePath, edition);
    }
    bindName();

}

nbtpp::BaseCompound::~BaseCompound() {
    delete nbt;
}
