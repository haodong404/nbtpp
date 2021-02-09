//
// Created by Zachary on 2021/2/2.
//
#include "nbtpp.h"
#include <iostream>

nbtpp::NBT::NBT(const char* filePath, const nbtpp::Edition& edi) : in(std::ifstream(filePath)), edition(edi) {
    if (in.is_open()) {
    }
    while (!in.eof()) {
        if (in.get() == nbtpp::Compound::type_id) {
            rootCompound = new Compound();
            compoundsStack.push(rootCompound);
            readTagCompound();
            break;
        } else if (in.get() == nbtpp::List::type_id) {

        }
    }
}

nbtpp::NBT::~NBT() {
    this->in.close();

    // Releasing the memory
    for (auto it = getRootCompound()->contentMap.begin(); it != getRootCompound()->contentMap.end(); it++) {
        free(it->second.ptr);
    }

    deleteInternalCompounds(*getRootCompound(), getRootCompound()->internalCompound.begin());

}

void nbtpp::NBT::deleteInternalCompounds(const Compound& compound, std::map<std::string, Compound*>::iterator it) {
    for (auto i = compound.contentMap.begin(); i != compound.contentMap.end(); i++) {
        free(i->second.ptr);
    }

    if (it != compound.internalCompound.end()) {
        auto i = it->second->internalCompound.begin();
        deleteInternalCompounds(*it->second, i);
        delete it->second;
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

void nbtpp::NBT::readTagStandard(const short& lengthOfPrefix, const int& payloadLength, bool isNumber) {
    Compound* compound = compoundsStack.top();

    auto name = parseTagName();

    int mPayloadLength = payloadLength;
    unsigned int unitSize = 1;

    if (lengthOfPrefix == 4 ||
        payloadLength == 99) { // Get the length if the tag is String, Using "payloadLength == 99" to distinguish.
        mPayloadLength = *parsePayloadLengthPrefix(lengthOfPrefix);
        unitSize = payloadLength;
    }

    auto payload = parsePayload(mPayloadLength, isNumber);

    compound->addItem(*name, *(unsigned int*) &mPayloadLength, unitSize, *payload);

    next();
}

void nbtpp::NBT::readTagList(bool isRoot) {
    auto tagName = parseTagName();
    char tagId = in.get();
    auto tagSize = getTagSizeById(tagId);
    int payloadSize = *parsePayloadLengthPrefix(4);
    int totalSize = payloadSize * (*tagSize);
    auto payload = parsePayload(totalSize, false);
    if (isRoot) {

    } else {
        Compound* compound = compoundsStack.top();
        compound->addItem(*tagName, *(unsigned int*) &payloadSize, reinterpret_cast<unsigned int&>(*tagSize), *payload );
    }

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

std::unique_ptr<int> nbtpp::NBT::parsePayloadLengthPrefix(const int& lengthOfPrefix) {
    auto result = std::make_unique<int>();
    if (getEdition() == nbtpp::Edition::BEDROCK) {
        for (int i = 0; i < lengthOfPrefix; i++) {
            *((char*) result.get() + i) = in.get();
        }
    } else {
        for (int i = lengthOfPrefix - 1; i >= 0; i--) {
            *((char*) result.get() + i) = in.get();
        }
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
        next();
    } else if (nextId == Byte::type_id) {
        readTagStandard(2, 1, false);
    } else if (nextId == Short::type_id) {
        readTagStandard(2, 2, true);
    } else if (nextId == Int::type_id) {
        readTagStandard(2, 4, true);
    } else if (nextId == Long::type_id) {
        readTagStandard(2, 8, true);
    } else if (nextId == Float::type_id) {
        readTagStandard(2, 4, true);
    } else if (nextId == Double::type_id) {
        readTagStandard(2, 8, true);
    } else if (nextId == ByteArray::type_id) {
        readTagStandard(4, 0, false);
    } else if (nextId == String::type_id) {
        readTagStandard(2, 99, false);
    } else if (nextId == List::type_id) {
        readTagList(false);
    } else if (nextId == Compound::type_id) {
        readTagCompound();
    } else if (nextId == IntArray::type_id) {
        readTagStandard(4, 4, false);
    } else if (nextId == LongArray::type_id) {
        readTagStandard(4, 4, false);
    }

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
