//
// Created by Zachary on 2021/2/2.
//
#include "nbtpp.h"
#include <iostream>

nbtpp::NBT::NBT(const char* filePath, const nbtpp::Edition& edi) : in(std::ifstream(filePath)), edition(edi) {
    if (in.is_open()) {
    }
    while (!in.eof()) {
        char typeId = in.get();
        if (typeId == nbtpp::Compound::type_id) {
            rootCompound = new Compound();
            compoundsStack.push(rootCompound);
            readTagCompound(false);
            break;
        } else if (typeId == nbtpp::List::type_id) {
            readTagList(true);
        }
    }
}

nbtpp::NBT::~NBT() {
    this->in.close();
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
        compoundsStack.push(newCompound);
        compound->internalCompound.insert(std::make_pair(*name, newCompound));
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

char* nbtpp::NBT::readTagStandard(char& typeId, const bool& isInList) {

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
    char tagId = in.get();
    auto tagSize = getTagSizeById(tagId);

    int payloadLength = *parsePayloadLengthPrefix(0x09);

    char* listPtr = nullptr;
    if (tagId == Compound::type_id) {
        isRoot = false; // Set the "isRoot" to false to created new compound pointer.
        auto* items = new BaseList<Compound*>(payloadLength);
        for (int i = 0; i < payloadLength; i++) {

            items->push_back(readTagCompound(true));
        }
        listPtr = (char*) items;
    } else {
        auto* items = new BaseList<Compound::Content>(payloadLength);
        int *a = new int(415);

        for (int i = 0; i < payloadLength; i++) {
            char* payload = readTagStandard(tagId, true);
            unsigned int length = sizeof(payload);
            items->emplace_back(tagId, payload, length);
        }
        listPtr = (char*) items;
    }

    if (isRoot) {

    } else {
        Compound* compound = compoundsStack.top();
        unsigned int totalLength = sizeof(listPtr);
        compound->itemMap.insert(std::make_pair(*tagName, Compound::Content(tagId, listPtr, totalLength)));
    }
    this->isInList = false;
    next();
}

std::unique_ptr<std::string> nbtpp::NBT::parseTagName() {
 
    short nameSize;
    if (getEdition() == nbtpp::Edition::BEDROCK) {
        for (char i = 0; i < 2; i++) {
            *((char*) &nameSize + i) = in.get();
        }
    } else {
        for (char i = 1; i >= 0; i--) { // Numbers are stored as little endian in memory.
            *((char*) &nameSize + i) = in.get();
        }
    }

    auto name = std::make_unique<std::string>();
    if (nameSize != 0) {
        for (short i = 0; i < nameSize; i++) {
            name->push_back(in.get());
        }
    }
    return std::move(name);
}

std::unique_ptr<int> nbtpp::NBT::parsePayloadLengthPrefix(const char& typeId) {
    auto result = std::make_unique<int>();
    int lengthOfPrefix = 0;

    if (typeId == nbtpp::String::type_id) {
        lengthOfPrefix = 2;
    } else if (typeId == nbtpp::ByteArray::type_id ||
               typeId == nbtpp::IntArray::type_id ||
               typeId == nbtpp::LongArray::type_id ||
               typeId == nbtpp::List::type_id) {

        lengthOfPrefix = 4;
    } else {
        result = getTagSizeById(typeId);
        return result;
    }


    if (getEdition() == nbtpp::Edition::BEDROCK) {
        for (int i = 0; i < lengthOfPrefix; i++) {
            *((char*) result.get() + i) = in.get();
        }
    } else {
        for (int i = lengthOfPrefix - 1; i >= 0; i--) {
            *((char*) result.get() + i) = in.get();
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
            (*result)[i] = in.get();
        }
    } else {
        for (short i = 0; i < payloadLength; i++) {
            (*result)[i] = in.get();
        }
    }
    return std::move(result);
}

void nbtpp::NBT::next() {

    char nextId = in.get();

    if (in.eof()) {
        return;
    }
    if (nextId == End::type_id) {
        compoundsStack.pop();
        if (isInList) {
            return;
        }
        next();
    } else if (nextId == List::type_id) {
        readTagList(false);
    } else if (nextId == Compound::type_id) {
        readTagCompound(false);
    } else {
        readTagStandard(nextId, false);
    }
}

std::unique_ptr<int> nbtpp::NBT::getTagSizeById(const char& id) {

    auto result = std::make_unique<int>();
    if (id == nbtpp::Byte::type_id) {
        *result = 1;
    } else if (id == nbtpp::Int::type_id) {
        *result = 4;
    } else if (id == nbtpp::Short::type_id) {
        *result = 2;
    } else if (id == nbtpp::Double::type_id) {
        *result = 8;
    } else if (id == nbtpp::Float::type_id) {
        *result = 4;
    } else if (id == nbtpp::Long::type_id) {
        *result = 8;
    } else if (id == nbtpp::End::type_id) {
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

bool nbtpp::NBT::isNumber(char& typeId) {
    return typeId != nbtpp::Byte::type_id ||
           typeId != nbtpp::String::type_id ||
           typeId != nbtpp::Compound::type_id ||
           typeId != nbtpp::ByteArray::type_id ||
           typeId != nbtpp::End::type_id;
}
