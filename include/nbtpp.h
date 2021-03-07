//
// Created by Zachary on 2021/2/2.
//
#pragma once

#include <stack>
#include "istream"
#include "Edition.h"
#include "Tags.h"

namespace nbtpp {

    class NBT {
    private:
        std::istream* in;
        Compound* rootCompound; // It contains the root compound
        bool isRoot = true;
        bool isInList = false;
        nbtpp::Edition edition;

        std::stack<Compound*> compoundsStack; // The top compound is what the current compound is used.
        /**
         * Parsing the tag name.
         * @return A smart pointer of string.
         */
        std::unique_ptr<std::string> parseTagName();

        /**
         * Parsing the prefix of payload length.
         * @param typeId type id
         * @return A smart pointer of payload length.
         */
        std::unique_ptr<int> parsePayloadLengthPrefix(const unsigned char& typeId);

        /**
         * Parsing the payload.
         * @param payloadLength it's always from the function 'parsePayloadLengthPrefix(const int& lengthOf Prefix)'
         * @param isNumber Whether the tag is a number.
         * @return A smart pointer of payload bytes array.
         */
        std::unique_ptr<char*> parsePayload(int& payloadLength, bool isNumber);

        /**
         * Delete internal Compounds.
         * @param compound It is in the last layer.
         * @param it The iterator of compound.
         */
        void deleteInternalCompounds(const Compound& compound, std::map<std::string, Compound*>::iterator& it);

        /**
         * Reading the compound tag.
         * @param isInlist Whether the current item is in a list.
         * @return A pointer to a compound.
         */
        Compound* readTagCompound(const bool& isInlist);

        /**
         * Reading the regular tags;
         * @param typeId Type id.
         * @param isInlist Whether the current item is in a list.
         * @return Return the pointer of a item if it's in a list.
         */
        char* readTagStandard(unsigned char& typeId, const bool& isInlist);

        /**
         * Reading the list tag.
         * @param Whether the list is root.
         */
        void readTagList(bool isRoot);

        /**
         * The judgment statement of recursion.
         */
        void next();

        void toHex(Hex& hex, Compound* compound, std::map<std::string, Compound*>::iterator& it);

    public:

        NBT() : in(nullptr){

        }

        /**
         * Starting to parse a NBT file.
         * @param filePath File path.
         * @param edi Game edition.
         */
        NBT(std::istream* in, const Edition& edi);

        virtual ~NBT();

        /**
         * Get the tag size by tag name.
         * @param id Tag's ID.
         * @return A smart pointer of size.
         */
        std::unique_ptr<int> getTagSizeById(const unsigned char& id);

        Compound* getRootCompound() const;

        Edition getEdition() const;

        /**
         * Whether the item is a number.
         * @param typeId Type number.
         * @return A boolean.
         */
        bool isNumber(unsigned char& typeId);

        int count();

        Hex toHex();

    };
}
