//
// Created by Zachary on 2021/2/2.
//
#pragma once

#include <stack>
#include "fstream"
#include "BaseList.h"

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
        Compound* rootCompound; // It contains the root compound
        bool isRoot = true;
        bool isInList = false;
        nbtpp::Edition edition;

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
        std::unique_ptr<int> parsePayloadLengthPrefix(const char& typeId);

        /**
         * Parsing the payload.
         * @param payloadLength payload size, it's always from the function 'parsePayloadLengthPrefix(const int& lengthOf Prefix)'
         * @return A smart pointer of payload bytes array.
         */
        std::unique_ptr<char*> parsePayload(int& payloadLength, bool isNumber);

        void deleteInternalCompounds(const Compound& compound, std::map<std::string, Compound*>::iterator it);

        /**
         * Reading the compound tag.
         */
        Compound* readTagCompound(const bool& isInlist);

        /**
         * Reading the regular tags;
         * @param lengthOfPrefix The length of payload prefix, some are 2 bytes, and some are 4 bytes.
         * @param payloadLength The length of payload.
         */
        char* readTagStandard(char& typeId, const bool& isInlist);

        /**
         * Reading the list tag.
         */
        void readTagList(bool isRoot);

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
        NBT(const char* filePath, const Edition& edi);

        virtual ~NBT();

        /**
         * Get the tag size by tag name.
         * @param id Tag's ID.
         * @return A smart pointer of size.
         */
        std::unique_ptr<int> getTagSizeById(const char& id);

        Compound* getRootCompound() const;

        Edition getEdition() const;

        bool isNumber(char& typeId);

    };
}
