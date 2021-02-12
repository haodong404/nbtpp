//
// Created by Zachary on 2021/2/2.
//
#include "Tags.h"

void nbtpp::BaseTag::toHex() {
    std::cout << "Hex" << std::endl;
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

nbtpp::Compound::Content::Content(const char& typeId, char* ptr, unsigned int& length)
        : typeId(typeId), ptr(ptr), length(length) {}

nbtpp::Compound::Content::Content(const nbtpp::Compound::Content& ano)
        : typeId(ano.typeId), ptr(ano.ptr), length(ano.length) {}

char* nbtpp::Compound::addItem(std::string& name, const char& typeId, unsigned int& length, char* payload) {

    size_t a = length;
    char* result = (char*) malloc(a);

    if (result != nullptr) {
        for (int i = 0; i < length; i++) {
            result[i] = payload[i];
        }
        Content content(typeId, result, length);
        itemMap.insert(std::make_pair(name, content));
    }

    return result;
}

nbtpp::Compound::Compound() {}

nbtpp::Compound::Compound(nbtpp::Compound* ano) : itemMap(ano->itemMap), internalCompound(ano->internalCompound) {

}

void nbtpp::BaseTag::setName(const std::string& m_name) {
    BaseTag::name = m_name;
}

nbtpp::Byte::Byte(const char& payload) : payload(payload) {

}

nbtpp::Byte::Byte() {

}

nbtpp::Byte::Byte(const nbtpp::Compound::Content& content) {
    payload = *content.ptr;
}

int nbtpp::Byte::toInteger() {
    return payload;
}

nbtpp::Short::Short(const short& payload) : payload(payload) {

}

nbtpp::Short::Short(const nbtpp::Compound::Content& content) {
    payload = *(short*) content.ptr;
}

nbtpp::Int::Int(const int& payload) : payload(payload) {

}

nbtpp::Int::Int() {

}

nbtpp::Int::Int(const nbtpp::Compound::Content& content) {
    payload = *(int*) content.ptr;
}

nbtpp::Long::Long() {

}

nbtpp::Long::Long(const nbtpp::Compound::Content& content) {
    payload = *(long*)content.ptr;
}

nbtpp::Long::Long(const long& payload) : payload(payload) {

}

nbtpp::Float::Float(const float& payload) : payload(payload) {

}

nbtpp::Float::Float(const nbtpp::Compound::Content& content) {
    payload = *(float*) content.ptr;
}

nbtpp::Float::Float() {

}

nbtpp::Double::Double(const double& payload) : payload(payload) {

}

nbtpp::Double::Double(const nbtpp::Compound::Content& content) {
    payload = *(double*) content.ptr;
}

nbtpp::String::String(const std::string& payload) : payload(payload) {

}

nbtpp::String::String() {

}

nbtpp::String::String(const char* string) : payload(string) {

}

nbtpp::String::String(const nbtpp::Compound::Content& content) {
    payload = content.ptr;
}

nbtpp::ByteArray::ByteArray(const nbtpp::Compound::Content& content) {
    this->payload = content.ptr;
}

nbtpp::ByteArray::ByteArray(char* payload) : payload(payload) {

}

nbtpp::IntArray::IntArray() {

}

nbtpp::IntArray::IntArray(int* payload) : payload(payload) {
}

nbtpp::IntArray::IntArray(const nbtpp::Compound::Content& content) {
    int result[content.length / 4];
    for (int i = 0; i < content.length; i += 4) {
        result[i / 4] = content.ptr[i / 4];
    }
    this->payload = result;
}

nbtpp::LongArray::LongArray() {

}

nbtpp::LongArray::LongArray(long*) {

}

nbtpp::LongArray::LongArray(const nbtpp::Compound::Content& content) {
    this->payload = content.ptr;
}

nbtpp::LongArray::LongArray(char*) {

}
