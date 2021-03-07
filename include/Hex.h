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

    public:

        Hex();

        ~Hex();

        Hex(const Edition& edition);

        /**
         * Constructor that will insert default bytes.
         * @param edition The edition of the file.
         * @param data The bytes you want to insert.
         */
        Hex(const Edition& edition, unsigned char* data);

        /**
         * Add id and prefix of tag name.
         * @param id The tag id.
         * @param name The name of the tag.
         */
        void addIdAndNamePrefix(const unsigned char& id, const std::string& name);

        /**
         * Rewrite the operator "<<", which will return the byte stream.
         * @param out
         * @param hex
         * @return The byte stream.
         */
        friend std::ostream& operator<<(std::ostream& out, Hex& hex) {
            for (auto it = hex.bytesList.begin(); it != hex.bytesList.end(); it++) {
                out << *it;
            }
            return out;
        };

        /**
         * Insert bytes to the bytes list.
         * @param bytes
         */
        void insertBytes(const unsigned char* bytes);

        /**
         * Insert a byte to the bytes list.
         * @param byte
         */
        void insertByte(const unsigned char& byte);

        /**
         * Get the bytes as hex string.
         * @return
         */
        std::string toString();

        /**
         * Push the specific tag to the bytes list.
         * @param id
         * @param name
         * @param payload
         */
        void pushSpecific(const unsigned char& id, const std::string& name, const unsigned char* payload);

        /**
         * Push the short nonespecific tag to the bytes list.
         * @param id
         * @param name
         * @param payload
         */
        void pushShortNonspecific(const unsigned char& id, const std::string& name, const unsigned char* payload);

        /**
         * Push the int nonspecific tag to the bytes list.
         * @param id
         * @param name
         * @param payload
         */
        void pushIntNonspecific(const unsigned char& id, const std::string& name, const unsigned char* payload);

        /**
         * Push a tag by id.
         * @param id
         * @param name
         * @param paylaod
         */
        void pushById(const unsigned char& id, const std::string& name, const unsigned char* paylaod);

        Edition getEdition() const;

        void setEdition(Edition edition);
    };
}