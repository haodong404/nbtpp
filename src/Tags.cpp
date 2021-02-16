//
// Created by Zachary on 2021/2/2.
//
#include "Tags.h"

void nbtpp::BaseTag::toHex() {
    std::cout << "Hex" << std::endl;
}

size_t nbtpp::BaseTag::length = 0;
size_t nbtpp::BaseTag::getLength(){
    return length;
}

void nbtpp::BaseTag::setLength(size_t length) {
    BaseTag::length = length;
}


nbtpp::Compound::Content::Content(const char& typeId, char* ptr, unsigned int& length)
        : typeId(typeId), ptr(ptr), length(length) {}

nbtpp::Compound::Content::Content(const nbtpp::Compound::Content& ano)
        : typeId(ano.typeId), ptr(ano.ptr), length(ano.length) {}

char* nbtpp::Compound::addItem(std::string& name, const char& typeId, unsigned int& length, char* payload) {

    char* result = (char*) malloc(length);

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

const std::string& nbtpp::Compound::getName() const {
    return name;
}

void nbtpp::Compound::setName(const std::string& name) {
    Compound::name = name;
}

nbtpp::Byte::Byte(const char& payload) : payload(payload) {
    this->setLength(sizeof(payload));
}

nbtpp::Byte::Byte() {

}

nbtpp::Byte::Byte(const nbtpp::Compound::Content& content) {
    payload = *content.ptr;
    this->setLength(content.length);
}

int nbtpp::Byte::toInteger() {
    return payload;
}

size_t nbtpp::Byte::size() {
    return getLength();
}

nbtpp::Short::Short(const short& payload) : payload(payload) {
    this->setLength(sizeof(payload));
}

nbtpp::Short::Short(const nbtpp::Compound::Content& content) {
    payload = *(short*) content.ptr;
    this->setLength(content.length);
}

size_t nbtpp::Short::size() {
    return getLength();
}

nbtpp::Int::Int(const int& payload) : payload(payload) {
    this->setLength(sizeof(payload));
}

nbtpp::Int::Int() {

}

nbtpp::Int::Int(const nbtpp::Compound::Content& content) {
    payload = *(int*) content.ptr;
    this->setLength(content.length);
}

size_t nbtpp::Int::size(){
    return getLength();
}

nbtpp::Long::Long() {

}

nbtpp::Long::Long(const nbtpp::Compound::Content& content) {
    payload = *(long*) content.ptr;
    this->setLength(content.length);
}

nbtpp::Long::Long(const long& payload) : payload(payload) {
    this->setLength(sizeof(payload));
}

size_t nbtpp::Long::size() {
    return getLength();
}

nbtpp::Float::Float(const float& payload) : payload(payload) {
    this->setLength(sizeof(payload));
}

nbtpp::Float::Float(const nbtpp::Compound::Content& content) {
    payload = *(float*) content.ptr;
    this->setLength(content.length);
}

nbtpp::Float::Float() {

}

size_t nbtpp::Float::size() {
    return getLength();
}

nbtpp::Double::Double(const double& payload) : payload(payload) {
    this->setLength(sizeof(payload));
}

nbtpp::Double::Double(const nbtpp::Compound::Content& content) {
    payload = *(double*) content.ptr;
    this->setLength(content.length);
}

nbtpp::Double::Double() {

}

size_t nbtpp::Double::size() {
    return getLength();
}

nbtpp::String::String(const std::string& payload) : payload(payload) {
    this->setLength(sizeof(payload));
}

nbtpp::String::String() {

}

nbtpp::String::String(const char* string) : payload(string) {

}

nbtpp::String::String(const nbtpp::Compound::Content& content) {
    payload = content.ptr;
    this->setLength(content.length);
}

size_t nbtpp::String::size() {
    return getLength();
}

char& nbtpp::String::operator[](const unsigned int& position) {
    return this->payload[position];
}

nbtpp::ByteArray::ByteArray(const nbtpp::Compound::Content& content) {
    this->payload = content.ptr;
    this->setLength(content.length);
}

nbtpp::ByteArray::ByteArray(char* payload) : payload(payload) {
    this->setLength(sizeof(payload));
}

size_t nbtpp::ByteArray::size() {
    return getLength();
}

char& nbtpp::ByteArray::operator[](const unsigned int& position) {
    return *(this->payload + position);
}

nbtpp::IntArray::IntArray() {

}

nbtpp::IntArray::IntArray(int* payload) : payload(payload) {
    this->setLength(sizeof(payload));
}

nbtpp::IntArray::IntArray(const nbtpp::Compound::Content& content) {
    this->payload = (int*) content.ptr;
    this->setLength(content.length);
}

int& nbtpp::IntArray::operator[](const unsigned int& position) {
    return *(this->payload + position);
}

size_t nbtpp::IntArray::size() {
    return getLength() / 4;
}

nbtpp::LongArray::LongArray() {

}

nbtpp::LongArray::LongArray(long* payload) {
    this->setLength(sizeof(payload));

}

nbtpp::LongArray::LongArray(const nbtpp::Compound::Content& content) {
    this->payload = (long*) content.ptr;
}

nbtpp::LongArray::LongArray(char*) {

}

size_t nbtpp::LongArray::size() {
    return getLength() / 8;
}

long& nbtpp::LongArray::operator[](const unsigned int& position) {
    return *(this->payload + position);
}
