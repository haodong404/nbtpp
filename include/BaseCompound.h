//
// Created by Zachary on 2021/2/7.
//

#pragma once

#include "Tags.h"
#include "nbtpp.h"
#include <iostream>

namespace nbtpp {

    class BaseCompound : public Compound {

    private:

        std::stack<BaseCompound*> compoundStack;

        nbtpp::NBT* nbt = nullptr;

    public:

        BaseCompound(BaseCompound* ano);

        BaseCompound(nbtpp::NBT* nbt);

        BaseCompound(const char* filePath, const nbtpp::Edition& edition);

        virtual ~BaseCompound();

        const nbtpp::NBT* getNbt() const;
    };
}
