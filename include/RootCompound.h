//
// Created by Zachary on 2021/2/7.
//

#pragma once

#include "Tags.h"
#include "nbtpp.h"
#include <iostream>

namespace nbtpp {

    class RootCompound : public Compound {

    private:

        std::stack<RootCompound*> compoundStack;

        nbtpp::NBT* nbt = nullptr;

    public:

        RootCompound();

        RootCompound(nbtpp::NBT* nbt);

        RootCompound(const char* filePath, const nbtpp::Edition& edition);

        virtual ~RootCompound();

        nbtpp::NBT* getNbt() const;

        /**
         * Get the class Hex from the data.
         * @return A Hex instance.
         */
        Hex toHex();
    };
}
