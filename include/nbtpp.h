//
// Created by Zachary on 2021/2/2.
//
#pragma once

#include "Tag.h"
#include "fstream"

namespace nbtpp {

    /**
     * Different editions use different endian;
     */
    enum Edition {
        BEDROCK,
        JAVA
    };

    class NBT {

    private:
        std::ifstream in;
        Compound rootCompound;
        bool isRoot = true;

        std::string* findTagName();

        void readTagCompound(nbtpp::Compound& compound);

        void readTagSpecific(const char& payloadSize, nbtpp::Compound& compound);

        void next(nbtpp::Compound& compound);

    public:
        NBT(const char* filePath, Edition edi);

        virtual ~NBT();

        Compound getRootCompound() const ;
    };
}