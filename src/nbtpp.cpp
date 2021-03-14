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
    deleteInternalCompounds(*getRootCompound());
}

void nbtpp::NBT::deleteInternalCompounds(const Compound& compound) {
    for (const auto& i : compound.itemMap) {
        free(i.second.payload.ptr);
    }

    for (auto& it : compound.internalCompound) {
        deleteInternalCompounds(it.second);
        delete it.second;
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
//            compoundsStack.top()->internalCompound.insert(std::make_pair(*name, newCompound));
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
        compound->addItem(*name, typeId, {*payload, (unsigned long) *payloadLength});
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

    int payloadLength = *parsePayloadLengthPrefix(0x09);

    unsigned char* listPtr = nullptr;

    auto* items = new List<Compound::Content>(payloadLength);

    for (int i = 0; i < payloadLength; i++) {
        if (tagId == COMPOUND) {
            isRoot = false; // Set the "isRoot" to false to created new compound pointer.
            unsigned char* ptr = (unsigned char*) readTagCompound(true);
            nbtpp::Compound::Content content(getEdition(), tagId, {ptr,
                                                                   static_cast<unsigned long >(payloadLength)});
            items->emplace_back(content);
        } else {
            unsigned char* payload = (unsigned char*) (readTagStandard(tagId, true));
            nbtpp::Compound::Content content(getEdition(), tagId,
                                             {payload, static_cast<unsigned long >(sizeof(payload))});
            items->emplace_back(content);
        }
    }

    items->setContentItemTypeId(tagId);
    listPtr = (unsigned char*) items;

    if (isRoot) {

    } else {
        Compound* compound = compoundsStack.top();
        compound->itemMap.insert(
                std::make_pair(*tagName,
                               Compound::Content(getEdition(), LIST, {listPtr, (unsigned long) payloadLength})));
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
    if (typeId == BYTE_ARRAY ||
        typeId == INT_ARRAY ||
        typeId == LONG_ARRAY) {
        *result = *result * *getTagSizeById(typeId);
    }
    return std::move(result);
}

std::unique_ptr<unsigned char*> nbtpp::NBT::parsePayload(int& payloadLength, bool isNumber) {
    auto result = std::make_unique<unsigned char*>(new unsigned char[payloadLength]);
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
    if (id == TagID::BYTE || id == TagID::BYTE_ARRAY) {
        *result = 1;
    } else if (id == TagID::INT || id == TagID::INT_ARRAY) {
        *result = 4;
    } else if (id == TagID::SHORT) {
        *result = 2;
    } else if (id == TagID::DOUBLE) {
        *result = 8;
    } else if (id == TagID::FLOAT) {
        *result = 4;
    } else if (id == TagID::LONG || id == TagID::INT_ARRAY) {
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

nbtpp::Hex nbtpp::NBT::toHex() {
    Hex hex(getEdition());
    hex.addIdAndNamePrefix(COMPOUND, getRootCompound()->getName());
    toHex(hex, getRootCompound());
    hex.insertByte(END);
    return hex;
}

void nbtpp::NBT::toHex(Hex& hex, nbtpp::Compound* compound) {

    for (auto& i : compound->itemMap) {
        if (i.second.typeId == LIST) {
            hex.addIdAndNamePrefix(LIST, i.first);
            List<Compound::Content> contents = *(List<Compound::Content>*) i.second.payload.ptr;
            hex.insertByte(contents.getContentItemTypeId());
            hex.insertIntPayloadPrefix(std::move(i.second.payload));

            for (auto& j : contents) {
                if (j.typeId == COMPOUND) {
                    Compound* temp = (Compound*) j.payload.ptr;
                    toHex(hex, temp);
                    hex.insertByte(END);
                } else {
                    if (getEdition() == BEDROCK || j.typeId == STRING) {
                        hex.insertBytes(std::move(j.payload));
                    } else {
                        hex.insertBytesInOppositeOrder(std::move(j.payload));
                    }
                }
            }
        } else {
            hex.pushById(i.second.typeId, i.first, std::move(i.second.payload));
        }
    }

    for (auto& it : compound->internalCompound) {
        std::string temp = it.first; // There must be a string copy here, or an error will occur.
        hex.addIdAndNamePrefix(COMPOUND, temp);
        toHex(hex, it.second);
        hex.insertByte(END);
    }
}