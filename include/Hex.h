//
// Created by Zachary on 2021/2/21.
//

#include "iostream"
#include "Edition.h"
#include "TagID.h"
#include <list>

#pragma once

namespace nbtpp {
    class Hex {
        unsigned short position = 0;
        std::list<unsigned char> bytesList;
        Edition edition = JAVA;


        void pushBytes(const unsigned char*);

        void addIdAndNamePrefix(const unsigned char& id, const std::string& name);

    public:
        Hex();

        ~Hex();

        Hex(const Edition& edition);

        Hex(const Edition& edition, unsigned char* data);

        std::ostream& operator<<(std::ostream&);


        const char* toString();

        void pushSpecific(const unsigned char& id, const std::string& name, const unsigned char*);

        void pushShortNonspecific(const unsigned char& id, const std::string& name, const unsigned char*);

        void pushIntNonspecific(const unsigned char& id, const std::string& name, const unsigned char*);

        void pushById(const unsigned char& id, const std::string& name, const unsigned char* paylaod);

        Edition getEdition() const;

        void setEdition(Edition edition);
    };
}