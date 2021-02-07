//
// Created by Zachary on 2021/2/2.
//
#pragma once

#include "Tag.h"
#include "fstream"
#include <stack>

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
        Compound rootCompound; // It contains the root compound
        bool isRoot = true ;
        std::stack<Compound*> compoundsStack; // The top compound is what the current compound used.

        /**
         * Parsing the tag name.
         * @return A smart pointer of string.
         */
        std::unique_ptr<std::string> parseTagName();

        /**
         * Parsing the prefix of payload size.
         * @param lengthOfPrefix Some are 2 bytes, some are 4 bytes.
         * @return A smart pointer of payload size.
         */
        std::unique_ptr<int> parsePayloadSizePrefix(const int& lengthOfPrefix);

        /**
         * Parsing the payload.
         * @param payloadSize payload size, it's always from the function 'parsePayloadSizePrefix(const int& lengthOf Prefix)'
         * @return A smart pointer of payload bytes array.
         */
        std::unique_ptr<char*> parsePayload(int& payloadSize);

        void deleteInternalCompounds(const Compound& compound, std::map<std::string, Compound*>::iterator& it);

        /**
         * Reading the compound tag.
         */
        void readTagCompound();

        /**
         * Reading the regular tags;
         * @param lengthOfPrefix The length of payload prefix, some are 2 bytes, and some are 4 bytes.
         * @param payloadSize The length of payload.
         */
        void readTagStandard(const short& lengthOfPrefix, const int& payloadSize);

        /**
         * Reading the list tag.
         */
        void readTagList();

        /**
         * The judgment statement of recursion.
         */
        void next();
    public:

        /**
         * Starting to parse a NBT file.
         * @param filePath File path.
         * @param edi edition.
         */
        NBT(const char* filePath, Edition edi);

        virtual ~NBT();

        /**
         * Get the tag size by tag name.
         * @param id Tag's ID.
         * @return A smart pointer of size.
         */
        std::unique_ptr<int> getTagSizeById(char& id);

        Compound getRootCompound() const ;
    };
}