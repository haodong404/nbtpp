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
         * @return A smart pointer of payload ptr array.
         */
        std::unique_ptr<unsigned char*> parsePayload(int& payloadLength, bool isNumber);

        /**
         * Delete internal Compounds.
         * @param compound It is in the last layer.
         */
        void deleteInternalCompounds(const Compound& compound);

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

        /**
         * Convert the data in the root compound into a class "Hex"
         * @param hex The reference of a Hex instance.
         * @param compound
         */
        void toHex(Hex& hex, Compound* compound);

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

        /**
         * Get the root compound that the NBT contains.
         * @return The pointer of the root compound.
         */
        Compound* getRootCompound() const;

        Edition getEdition() const;

        /**
         * Whether the item is a number.
         * @param typeId Type number.
         * @return A boolean.
         */
        bool isNumber(unsigned char& typeId);

        /**
         * It returns the total number of items in the root compound.
         * @return the count.
         */
        int count();

        /**
         * Get the class Hex from the data.
         * @return A Hex instance.
         */
        Hex toHex();

    };
}
