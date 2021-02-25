//
// Created by Zachary on 2021/2/2.
//
#include "nbtpp.h"
#include <iostream>

nbtpp::NBT::NBT(std::istream* m_in, const nbtpp::Edition& edi) : in(m_in), edition(edi) {

    while (!in->eof()) {
        char typeId = this->in->get();
        if (typeId == nbtpp::TagID::COMPOUND) {
            rootCompound = new Compound();
            compoundsStack.push(rootCompound);
            readTagCompound(false);
            return;
        } else if (typeId == nbtpp::TagID::LIST) {
            readTagList(true);
            return;
        }
    }
    std::cout << "This file may not be a nbt file !!!" << std::endl;
}

nbtpp::NBT::~NBT() {
    // Releasing the memory
    deleteInternalCompounds(*getRootCompound(), getRootCompound()->internalCompound.begin());
}

void nbtpp::NBT::deleteInternalCompounds(const Compound& compound, std::map<std::string, Compound*>::iterator it) {
    for (auto i = compound.itemMap.begin(); i != compound.itemMap.end(); i++) {
        free(i->second.ptr);
    }

    if (it != compound.internalCompound.end()) {
        auto i = it->second->internalCompound.begin();
        deleteInternalCompounds(*it->second, i);
        delete it->second;
        it++;
    }
}


nbtpp::Compound* nbtpp::NBT::readTagCompound(const bool& isInList) {
    auto name = std::make_unique<std::string>();
    if (!isInList) {
        name = std::move(parseTagName());
    }
    Compound* compound = compoundsStack.top();

    if (!isRoot) {
        Compound* newCompound = new Compound();
        newCompound->setName(*name);
        compoundsStack.push(newCompound);
        if (!isInList) {
            compound->internalCompound.insert(std::make_pair(*name, newCompound));
        } else {
            compoundsStack.top()->internalCompound.insert(std::make_pair(*name, newCompound));
        }

        this->isInList = isInList;
        next();
        compound = newCompound;
    } else {
        isRoot = false;
        compound->setName(*name);
        next();
    }
    return compound;
}

char* nbtpp::NBT::readTagStandard(unsigned char& typeId, const bool& isInList) {

    Compound* compound = compoundsStack.top();

    auto name = std::make_unique<std::string>();
    if (!isInList) {
        name = parseTagName();
    }
    auto payloadLength = parsePayloadLengthPrefix(typeId);

    auto payload = parsePayload(*payloadLength, isNumber(typeId));
    if (!isInList) {
        compound->addItem(*name, typeId, *(unsigned int*) payloadLength.get(), *payload);
    } else {
        char* result = (char*) std::malloc(*payloadLength);
        for (int i = 0; i < *payloadLength; i++) {
            result[i] = (*payload)[i];
        }
        return result;
    }

    next();
    return nullptr;
}

void nbtpp::NBT::readTagList(bool isRoot) {

    auto tagName = parseTagName();
    unsigned char tagId = in->get();
    auto tagSize = getTagSizeById(tagId);

    int payloadLength = *parsePayloadLengthPrefix(0x09);

    char* listPtr = nullptr;

    auto* items = new List<Compound::Content>(payloadLength);

    for (int i = 0; i < payloadLength; i++) {
        if (tagId == COMPOUND) {
            isRoot = false; // Set the "isRoot" to false to created new compound pointer.
            unsigned int length = payloadLength;
            items->emplace_back(getEdition(),tagId, (char*) readTagCompound(true), length);
        } else {
            char* payload = readTagStandard(tagId, true);
            unsigned int length = sizeof(payload);
            items->emplace_back(getEdition(),tagId, payload, length);
        }
    }

    listPtr = (char*) items;

    if (isRoot) {

    } else {
        Compound* compound = compoundsStack.top();
        unsigned int totalLength = sizeof(listPtr);
        compound->itemMap.insert(std::make_pair(*tagName, Compound::Content(getEdition(),tagId, listPtr, totalLength)));
    }
    this->isInList = false;
    next();
}

std::unique_ptr<std::string> nbtpp::NBT::parseTagName() {

    short nameSize;
    if (getEdition() == nbtpp::Edition::BEDROCK) {
        for (char i = 0; i < 2; i++) {
            *((char*) &nameSize + i) = in->get();
        }
    } else {
        for (char i = 1; i >= 0; i--) { // Numbers are stored as little endian in memory.
            *((char*) &nameSize + i) = in->get();
        }
    }

    auto name = std::make_unique<std::string>();
    if (nameSize != 0) {
        for (short i = 0; i < nameSize; i++) {
            name->push_back(in->get());
        }
    }
    return std::move(name);
}

std::unique_ptr<int> nbtpp::NBT::parsePayloadLengthPrefix(const unsigned char& typeId) {
    auto result = std::make_unique<int>();
    int lengthOfPrefix = 0;

    if (typeId == nbtpp::TagID::STRING) {
        lengthOfPrefix = 2;
    } else if (typeId == nbtpp::TagID::BYTE_ARRAY ||
               typeId == nbtpp::TagID::INT_ARRAY ||
               typeId == nbtpp::TagID::LONG_ARRAY ||
               typeId == nbtpp::TagID::LIST) {

        lengthOfPrefix = 4;
    } else {
        result = getTagSizeById(typeId);
        return result;
    }


    if (getEdition() == nbtpp::Edition::BEDROCK) {
        for (int i = 0; i < lengthOfPrefix; i++) {
            *((char*) result.get() + i) = in->get();
        }
    } else {
        for (int i = lengthOfPrefix - 1; i >= 0; i--) {
            *((char*) result.get() + i) = in->get();
        }
    }
    if (typeId == nbtpp::ByteArray::type_id ||
        typeId == nbtpp::IntArray::type_id ||
        typeId == nbtpp::LongArray::type_id) {
        *result = *result * lengthOfPrefix;
    }
    return std::move(result);
}

std::unique_ptr<char*> nbtpp::NBT::parsePayload(int& payloadLength, bool isNumber) {
    auto result = std::make_unique<char*>(new char[payloadLength]);
    if (isNumber && getEdition() == nbtpp::Edition::JAVA) {
        for (short i = payloadLength - 1; i >= 0; i--) {
            (*result)[i] = in->get();
        }
    } else {
        for (short i = 0; i < payloadLength; i++) {
            (*result)[i] = in->get();
        }
    }
    return std::move(result);
}

void nbtpp::NBT::next() {

    unsigned char nextId = in->get();

    if (in->eof()) {
        return;
    }

    if (nextId == TagID::END) {
        compoundsStack.pop();
        if (isInList) {
            return;
        }
        next();
    } else if (nextId == TagID::LIST) {
        readTagList(false);
    } else if (nextId == TagID::COMPOUND) {
        readTagCompound(false);
    } else {
        readTagStandard(nextId, false);
    }
}

std::unique_ptr<int> nbtpp::NBT::getTagSizeById(const unsigned char& id) {

    auto result = std::make_unique<int>();
    if (id == TagID::BYTE) {
        *result = 1;
    } else if (id == TagID::INT) {
        *result = 4;
    } else if (id == TagID::SHORT) {
        *result = 2;
    } else if (id == TagID::DOUBLE) {
        *result = 8;
    } else if (id == TagID::FLOAT) {
        *result = 4;
    } else if (id == TagID::LONG) {
        *result = 8;
    } else if (id == TagID::END) {
        *result = 0;
    } else {
        return nullptr;
    }

    return std::move(result);
}

nbtpp::Compound* nbtpp::NBT::getRootCompound() const {
    return rootCompound;
}

nbtpp::Edition nbtpp::NBT::getEdition() const {
    return edition;
}

bool nbtpp::NBT::isNumber(unsigned char& typeId) {
    return typeId == nbtpp::TagID::INT ||
           typeId == nbtpp::TagID::SHORT ||
           typeId == nbtpp::TagID::DOUBLE ||
           typeId == nbtpp::TagID::FLOAT ||
           typeId == nbtpp::TagID::LONG ||
           typeId == nbtpp::TagID::INT_ARRAY ||
           typeId == nbtpp::TagID::LONG_ARRAY;
}

int nbtpp::NBT::count() {
    return getRootCompound()->itemMap.size() + getRootCompound()->internalCompound.size();
}
