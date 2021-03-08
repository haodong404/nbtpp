//
// Created by Zachary on 2021/2/21.
//

#include "iostream"
#include "Edition.h"
#include "TagID.h"
#include <list>
#include "payload.h"

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
         * Constructor that will insert default ptr.
         * @param edition The edition of the file.
         * @param data The ptr you want to insert.
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
         * Insert ptr to the ptr list.
         * @param bytes
         */
        void insertBytes(Payload&& payload);

        /**
         * Insert a byte to the ptr list.
         * @param byte
         */
        void insertByte(const unsigned char& byte);

        /**
         * Get the ptr as hex string.
         * @return
         */
        std::string toString();

        /**
         * Push the specific tag to the ptr list.
         * @param id
         * @param name
         * @param payload
         */
        void pushSpecific(const unsigned char& id, const std::string& name, Payload&& payload);

        /**
         * Push the short nonespecific tag to the ptr list.
         * @param id
         * @param name
         * @param payload
         */
        void pushShortNonspecific(const unsigned char& id, const std::string& name, Payload&& payload);

        /**
         * Push the int nonspecific tag to the ptr list.
         * @param id
         * @param name
         * @param payload
         */
        void pushIntNonspecific(const unsigned char& id, const std::string& name, Payload&& payload);

        /**
         * Push a tag by id.
         * @param id
         * @param name
         * @param paylaod
         */
        void pushById(const unsigned char& id, const std::string& name, Payload&& payload);

        Edition getEdition() const;

        void setEdition(Edition edition);
    };
}