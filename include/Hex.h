//
// Created by Zachary on 2021/2/21.
//

#include "iostream"
#include "Edition.h"

#pragma once

namespace nbtpp {
    class Hex {
        unsigned short position = 0;
        unsigned char* data;
        Edition edition = JAVA;

        void pushByte(const unsigned char&);

        void pushBytes(const unsigned char*);

        void addIdAndNamePrefix(const unsigned char& id, const  std::string& name);

    public:
        Hex();

        ~Hex();

        Hex(const Edition& edition);

        Hex(const Edition& edition, unsigned char* data);

        std::ostream& operator<<(std::ostream&);


        const char* toString();

        void pushSpecific(const unsigned char& id, const std::string& name, const unsigned char*);

        void pushShortNonspecific(const unsigned char& id, std::string& name, const unsigned char*);

        void pushIntNonspecific(const unsigned char& id, std::string& name, const unsigned char*);

        Edition getEdition() const;

        void setEdition(Edition edition);
    };
}