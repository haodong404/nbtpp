//
// Created by Zachary on 2021/2/2.
//
#include "Tag.h"
#include "iostream"


void nbtpp::BaseTag::toHex() {
    std::cout << "Hex" << std::endl;
}


void nbtpp::BaseTag::bindName(std::string& name) {
    this->name = std::move(name);
}

short nbtpp::BaseTag::getNameSize() const {
    return name_size;
}

void nbtpp::BaseTag::setNameSize(const short& nameSize) {
    name_size = nameSize;
}

const std::string& nbtpp::BaseTag::getName() const {
    return name;
}

void nbtpp::BaseTag::setName(const std::string& m_name) {
    BaseTag::name = m_name;
}

nbtpp::Byte::Byte(const char& payload) : payload(payload) {

}

nbtpp::Short::Short(const short& payload) : payload(payload) {

}

nbtpp::Int::Int(const int& payload) : payload(payload) {

}

nbtpp::Long::Long(std::unique_ptr<char> payload) : payload(payload.get()) {

}

nbtpp::Float::Float(const float& payload) : payload(payload) {

}

nbtpp::Double::Double(const double& payload) : payload(payload) {

}

nbtpp::Compound::Content::Content(char* ptr, unsigned int& size) : ptr(ptr), size(size)  {}

char* nbtpp::Compound::addItem(std::string& name, unsigned int& size, char* payload) {

    size_t a = size;
    char* result = (char*) malloc(a);

    if(result != nullptr) {
        for (int i = 0; i < size; i++) {
            result[i] = payload[i];
        }
        Content content(result, size);
        contentMap.insert(std::make_pair(name, content));
    }

    return result;
}
