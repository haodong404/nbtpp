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

        nbtpp::NBT* nbt;

    public:

        template<class Tag>
        Tag findItemByName(const char* name) {
            Tag tag;
            tag = *nbt->getRootCompound()->internalCompound.find("Data")->second->contentMap.find(name)->second.ptr;
//std::cout << nbt->getRootCompound()->internalCompound.size();
            return tag ;
        }

        virtual void bindName();

        BaseCompound(nbtpp::NBT* nbt);

        BaseCompound(const char* filePath, const nbtpp::Edition& edition);

        virtual ~BaseCompound();

        const nbtpp::NBT* getNbt() const;
    };
}
