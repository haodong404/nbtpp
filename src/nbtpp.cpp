//
// Created by Zachary on 2021/2/2.
//
#include "nbtpp.h"
#include <iostream>

nbtpp::NBT::NBT(const char* filePath, nbtpp::Edition edi) : in(std::ifstream(filePath)) {
    if (in.is_open()) {
    }
    while (!in.eof()) {
        if (in.get() == nbtpp::Compound::type_id) {
            compoundsStack.push(&rootCompound);
            readTagCompound();
            break;
        }
    }
}

nbtpp::NBT::~NBT() {
    this->in.close();

    // Releasing the memory
    for (auto it = getRootCompound().contentMap.begin(); it != getRootCompound().contentMap.end(); it++) {
        free(it->second.ptr);
    }

    auto it = getRootCompound().internalCompound.begin();
    deleteInternalCompounds(getRootCompound(), it);

}

void nbtpp::NBT::deleteInternalCompounds(const Compound& compound, std::map<std::string, Compound*>::iterator& it) {
    for (auto i = compound.contentMap.begin(); i != compound.contentMap.end(); i++) {
        free(i->second.ptr);
        std::cout << "DELETE ITEM !!" << std::endl;

    }

    if (it != compound.internalCompound.end()) {
        auto i  = it->second->internalCompound.begin();
        deleteInternalCompounds(*it->second, i);
        delete it->second;
        std::cout << "DELETE COMPOUND !!" << std::endl;
        it++;
    }
}


void nbtpp::NBT::readTagCompound() {
    Compound* compound = compoundsStack.top();

    auto name = parseTagName();
    if (!isRoot) {
        Compound* newCompound = new Compound();
        compoundsStack.push(newCompound);
        compound->internalCompound.insert(std::make_pair(*name, newCompound));
        next();
    } else {
        isRoot = false;
        compound->setName(*name);
        next();
    }
}

void nbtpp::NBT::readTagStandard(const short& lengthOfPrefix, const int& payloadSize) {
    Compound* compound = compoundsStack.top();

    auto name = parseTagName();

    int mPayloadSize = payloadSize;

    if (lengthOfPrefix == 4 ||
        payloadSize == 99) { // Get the length if the tag is String, Using "payloadSize == 99" to distinguish.
        mPayloadSize = *parsePayloadSizePrefix(lengthOfPrefix);
    }

    auto payload = parsePayload(mPayloadSize);

    compound->addItem(*name, *(unsigned int*) &mPayloadSize, *payload);

    next();
}

void nbtpp::NBT::readTagList() {
    auto tagName = parseTagName();
    char tagId = in.get();
    auto tagSize = getTagSizeById(tagId);
    int payloadSize = *parsePayloadSizePrefix(4);
    auto payload = parsePayload(payloadSize);
    Compound* compound = compoundsStack.top();

    compound->addItem(*tagName, *(unsigned int*) &payloadSize, *payload);

    next();
}

std::unique_ptr<std::string> nbtpp::NBT::parseTagName() {

    short nameSize;
    for (char i = 1; i >= 0; i--) { // Data are stored as little endian in memory.
        *((char*) &nameSize + i) = in.get();
    }
    auto name = std::make_unique<std::string>();
    if (nameSize != 0) {
        for (short i = 0; i < nameSize; i++) {
            name->push_back(in.get());
        }
    }
    return std::move(name);
}

std::unique_ptr<int> nbtpp::NBT::parsePayloadSizePrefix(const int& lengthOfPrefix) {
    auto result = std::make_unique<int>();
    for (int i = lengthOfPrefix - 1; i >= 0; i--) {
        *((char*) result.get() + i) = in.get();
    }
    return std::move(result);
}

std::unique_ptr<char*> nbtpp::NBT::parsePayload(int& payloadSize) {
    auto result = std::make_unique<char*>(new char[payloadSize]);
    for (short i = 0; i < payloadSize; i++) {
        (*result)[i] = in.get();
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
        next();
    } else if (nextId == Byte::type_id) {
        readTagStandard(2, 1);
    } else if (nextId == Short::type_id) {
        readTagStandard(2, 2);
    } else if (nextId == Int::type_id) {
        readTagStandard(2, 4);
    } else if (nextId == Long::type_id) {
        readTagStandard(2, 8);
    } else if (nextId == Float::type_id) {
        readTagStandard(2, 4);
    } else if (nextId == Double::type_id) {
        readTagStandard(2, 8);
    } else if (nextId == ByteArray::type_id) {
        readTagStandard(4, 0);
    } else if (nextId == String::type_id) {
        readTagStandard(2, 99);
    } else if (nextId == List::type_id) {
        readTagList();
    } else if (nextId == Compound::type_id) {
        readTagCompound();
    } else if (nextId == IntArray::type_id) {
        readTagStandard(4, 0);
    } else if (nextId == LongArray::type_id) {
        readTagStandard(4, 0);
    }

}

nbtpp::Compound nbtpp::NBT::getRootCompound() const {
    return this->rootCompound;
}

std::unique_ptr<int> nbtpp::NBT::getTagSizeById(char& id) {

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
    }

    return std::move(result);
}
