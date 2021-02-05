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
            readTagCompound(rootCompound);
            break;
        }
    }
}

nbtpp::NBT::~NBT() {
    this->in.close();
    for (auto it = getRootCompound().contentMap.begin(); it != getRootCompound().contentMap.end(); it++) {
        free(it->second.ptr);
    }
}

void nbtpp::NBT::readTagCompound(nbtpp::Compound& compound) {
    std::string* name = findTagName();

    nbtpp::Compound* mCompound;

    /**
     * Creating a new compound tag if the argument 'compound' is not in root;
     */
    if(isRoot){
        isRoot = false;
        mCompound = &rootCompound;
    }else{
        mCompound = std::make_unique<Compound>().get();
    }

    mCompound->setNameSize(name->size());
    if (name->size() != 0) {
        mCompound->setName(*name);
    }
    delete name;

    next(*mCompound);
}

void nbtpp::NBT::readTagSpecific(const char& payloadSize, nbtpp::Compound& compound) {

    std::string* name = findTagName();
    char payload[payloadSize];
    for (short i = payloadSize - 1; i >= 0; i--) {
        payload[i] = in.get();
    }

    unsigned int a = payloadSize;
    compound.addItem(*name, a, payload);
    delete name;
    next(compound);
}

std::string* nbtpp::NBT::findTagName() {
    short nameSize;
    for (char i = 1; i >= 0; i--) { // Data are stored as little endian in memory.
        *((char*) &nameSize + i) = in.get();
    }
    std::string* name = new std::string();
    if (nameSize != 0) {
        for (short i = 0; i < nameSize; i++) {
            name->push_back(in.get());
        }
    }

    return name;
}

void nbtpp::NBT::next(nbtpp::Compound& compound) {

    char nextId = in.get();

    if (nextId == End::type_id) {
        return;
    } else if (nextId == Byte::type_id) {
        readTagSpecific(1, compound);
    } else if (nextId == Short::type_id) {
        readTagSpecific(2, compound);
    } else if (nextId == Int::type_id) {
        readTagSpecific(4, compound);
    } else if (nextId == Long::type_id) {
        readTagSpecific(8, compound);
    } else if (nextId == Float::type_id) {
        readTagSpecific(4, compound);
    } else if (nextId == Double::type_id) {
        readTagSpecific(8, compound);
    } else if (nextId == ByteArray::type_id) {

    } else if (nextId == String::type_id) {

    } else if (nextId == List::type_id) {

    } else if (nextId == Compound::type_id) {
        readTagCompound(compound);
    } else if (nextId == IntArray::type_id) {

    } else if (nextId == LongArray::type_id) {

    }

}

nbtpp::Compound nbtpp::NBT::getRootCompound() const {
    return this->rootCompound;
}
