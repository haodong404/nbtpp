//
// Created by Zachary on 2021/2/2.
//

#include "Tags.h"

nbtpp::Hex nbtpp::BaseTag::toHex() {
    return Hex();
}

size_t nbtpp::BaseTag::getLength() const {
    return length;
}

void nbtpp::BaseTag::setLength(size_t length) {
    this->length = length;
}

void nbtpp::BaseTag::bind() {

}

const std::string& nbtpp::BaseTag::getName() const {
    return name;
}

void nbtpp::BaseTag::setName(const std::string& name) {
    BaseTag::name = name;
}

nbtpp::Edition nbtpp::BaseTag::getEdition() const {
    return edition;
}

void nbtpp::BaseTag::setEdition(nbtpp::Edition edition) {
    BaseTag::edition = edition;
}


nbtpp::Compound::Content::Content(const nbtpp::Edition& edition, const unsigned char& typeId, unsigned char* ptr,
                                  unsigned int& length)
        : edition(edition), typeId(typeId), ptr(ptr), length(length) {}

nbtpp::Compound::Content::Content(const nbtpp::Compound::Content& ano)
        : edition(ano.edition), typeId(ano.typeId), ptr(ano.ptr), length(ano.length) {}

unsigned char* nbtpp::Compound::addItem(std::string& name, const char& typeId, unsigned int& length, char* payload) {

    auto* result = (unsigned char*) malloc(length);

    if (result != nullptr) {
        for (int i = 0; i < length; i++) {
            result[i] = payload[i];
        }
        Content content(getEdition(), typeId, result, length);
        itemMap.insert(std::make_pair(name, content));
    }

    return result;
}

nbtpp::Compound::Compound() {}

nbtpp::Compound::Compound(nbtpp::Compound* ano) : itemMap(ano->itemMap), internalCompound(ano->internalCompound) {

}

size_t nbtpp::Compound::size() {
    return internalCompound.size() + itemMap.size();
}

nbtpp::Compound::Compound(const nbtpp::Compound::Content& content)
        : itemMap(((Compound*) content.ptr)->itemMap), internalCompound(((Compound*) content.ptr)->internalCompound) {}

void nbtpp::Compound::bind() {

}

nbtpp::Hex nbtpp::Compound::toHex() {
    return BaseTag::toHex();
}

nbtpp::Byte::Byte(const unsigned char& payload) : payload(payload) {
    this->setLength(sizeof(payload));
    *this->raw = payload;
}

nbtpp::Byte::Byte() {

}

nbtpp::Byte::Byte(const nbtpp::Compound::Content& content) {
    payload = *content.ptr;
    this->setLength(content.length);
    this->setEdition(content.edition);
    this->raw = content.ptr;
}

int nbtpp::Byte::toInteger() {
    return payload;
}

size_t nbtpp::Byte::size() {
    return getLength();
}

nbtpp::Hex nbtpp::Byte::toHex() {
    Hex hex(getEdition());
    hex.pushSpecific(BYTE, getName(), raw);
    return hex;
}

nbtpp::Short::Short(const short& payload) : payload(payload) {
    this->setLength(sizeof(payload));
}

nbtpp::Short::Short(const nbtpp::Compound::Content& content) {
    payload = *(short*) content.ptr;
    this->setLength(content.length);
    this->setEdition(content.edition);
}

size_t nbtpp::Short::size() {
    return getLength();
}

nbtpp::Hex nbtpp::Short::toHex() {
    Hex hex(getEdition());
    hex.pushSpecific(SHORT, getName(), raw);
    return hex;
}

nbtpp::Int::Int(const int& payload) : payload(payload) {
    this->setLength(sizeof(payload));
}

nbtpp::Int::Int() {

}

nbtpp::Int::Int(const nbtpp::Compound::Content& content) {
    payload = *(int*) content.ptr;
    this->setLength(content.length);
    this->setEdition(content.edition);
}

size_t nbtpp::Int::size() {
    return getLength();
}

nbtpp::Hex nbtpp::Int::toHex() {
    Hex hex(getEdition());
    hex.pushSpecific(INT, getName(), raw);
    return hex;
}

nbtpp::Long::Long() {

}

nbtpp::Long::Long(const nbtpp::Compound::Content& content) {
    payload = *(long*) content.ptr;
    this->setLength(content.length);
    this->setEdition(content.edition);
}

nbtpp::Long::Long(const long& payload) : payload(payload) {
    this->setLength(sizeof(payload));
}

size_t nbtpp::Long::size() {
    return getLength();
} 

nbtpp::Hex nbtpp::Long::toHex() {
    Hex hex(getEdition());
    hex.pushSpecific(LONG, getName(), raw);
    return hex;
}

nbtpp::Float::Float(const float& payload) : payload(payload) {
    this->setLength(sizeof(payload));
}

nbtpp::Float::Float(const nbtpp::Compound::Content& content) {
    payload = *(float*) content.ptr;
    this->setLength(content.length);
    this->setEdition(content.edition);
}

nbtpp::Float::Float() {

}

size_t nbtpp::Float::size() {
    return getLength();
}

nbtpp::Hex nbtpp::Float::toHex() {
    Hex hex(getEdition());
    hex.pushSpecific(FLOAT, getName(), raw);
    return hex;
}

nbtpp::Double::Double(const double& payload) : payload(payload) {
    this->setLength(sizeof(payload));
}

nbtpp::Double::Double(const nbtpp::Compound::Content& content) {
    payload = *(double*) content.ptr;
    this->setLength(content.length);
    this->setEdition(content.edition);
}

nbtpp::Double::Double() {

}

size_t nbtpp::Double::size() {
    return getLength();
}

nbtpp::Hex nbtpp::Double::toHex() {
    Hex hex(getEdition());
    hex.pushSpecific(DOUBLE, getName(), raw);
    return hex;
}

nbtpp::String::String(const std::string& payload) : payload(payload) {
    this->setLength(sizeof(payload));
}

nbtpp::String::String() {

}

nbtpp::String::String(const char* string) : payload(string) {

}

nbtpp::String::String(const nbtpp::Compound::Content& content) {
    payload = (char*) content.ptr;
    this->setLength(content.length);
    this->setEdition(content.edition);
}

size_t nbtpp::String::size() {
    return getLength();
}

char& nbtpp::String::operator[](const unsigned int& position) {
    return this->payload[position];
}

nbtpp::Hex nbtpp::String::toHex() {
    Hex hex(getEdition());
    hex.pushSpecific(STRING, getName(), raw);
    return hex;
}

nbtpp::ByteArray::ByteArray(const nbtpp::Compound::Content& content) {
    this->payload = content.ptr;
    this->setLength(content.length);
    this->setEdition(content.edition);
}

nbtpp::ByteArray::ByteArray(unsigned char* payload) : payload(payload) {
    this->setLength(sizeof(payload));
}

size_t nbtpp::ByteArray::size() {
    return getLength();
}

unsigned char& nbtpp::ByteArray::operator[](const unsigned int& position) {
    return *(this->payload + position);
}

nbtpp::Hex nbtpp::ByteArray::toHex() {
    Hex hex(getEdition());
    hex.pushSpecific(BYTE_ARRAY, getName(), raw);
    return hex;
}

nbtpp::IntArray::IntArray() {

}

nbtpp::IntArray::IntArray(int* payload) : payload(payload) {
    this->setLength(sizeof(payload));
}

nbtpp::IntArray::IntArray(const nbtpp::Compound::Content& content) {
    this->payload = (int*) content.ptr;
    this->setLength(content.length);
    this->setEdition(content.edition);
}

int& nbtpp::IntArray::operator[](const unsigned int& position) {
    return *(this->payload + position);
}

size_t nbtpp::IntArray::size() {
    return getLength() / 4;
}

nbtpp::Hex nbtpp::IntArray::toHex() {
    Hex hex(getEdition());
    hex.pushSpecific(INT_ARRAY, getName(), raw);
    return hex;
}

nbtpp::LongArray::LongArray() {

}

nbtpp::LongArray::LongArray(long* payload) {
    this->setLength(sizeof(payload));
}

nbtpp::LongArray::LongArray(const nbtpp::Compound::Content& content) {
    this->payload = (long*) content.ptr;
    this->setEdition(content.edition);
}

nbtpp::LongArray::LongArray(char*) {

}

size_t nbtpp::LongArray::size() {
    return getLength() / 8;
}

long& nbtpp::LongArray::operator[](const unsigned int& position) {
    return *(this->payload + position);
}

nbtpp::Hex nbtpp::LongArray::toHex() {
    Hex hex(getEdition());
    hex.pushSpecific(LONG_ARRAY, getName(), raw);
    return hex;
}

nbtpp::Hex nbtpp::End::toHex() {
    return BaseTag::toHex();
}
