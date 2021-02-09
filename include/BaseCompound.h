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

        template<typename Tag>
        Tag findItemByName(const char* name) {
            if (Tag::type_id == nbtpp::String::type_id) {
                Tag tag1(getNbt()->getRootCompound()->contentMap.find(name)->second);
                std::cout << "String" << std::endl;
                return tag1;
            } else {
                Tag tag1(getNbt()->getRootCompound()->contentMap.find(name)->second);
                std::cout << "Not String" << std::endl;
                return tag1;
            }
        }


        BaseCompound(nbtpp::NBT* nbt);

        BaseCompound(const char* filePath, const nbtpp::Edition& edition);

        virtual ~BaseCompound();

        const nbtpp::NBT* getNbt() const;
    };
}
