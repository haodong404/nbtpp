//
// Created by Zachary on 2021/2/21.
//

#include <Hex.h>

nbtpp::Hex::Hex() {

}

nbtpp::Hex::Hex(const Edition& edition, unsigned char* data)
        : edition(edition) {
    for (int i = 0; i < sizeof data; i++) {
        bytesList.emplace_back(data[i]);
    }
}

nbtpp::Hex::Hex(const Edition& edition)
        : edition(edition) {
}

std::string nbtpp::Hex::toString() {
    std::string str;
    const char hexes[] = "0123456789ABCDEF";
    str.reserve(bytesList.size());
    for (auto it = bytesList.begin(); it != bytesList.end(); it++) {
        str.push_back(hexes[*it / 16]);
        str.push_back(hexes[*it % 16]);
        str.push_back(0x20);
    }
    return str;
}


void nbtpp::Hex::insertBytes(nbtpp::Payload&& payload) {
    for (int i = 0; i < payload.size; i++) {
        bytesList.emplace_back(payload.ptr[i]);
    }
}

void nbtpp::Hex::insertIntPayloadPrefix(nbtpp::Payload&& payload) {
    int payloadSize = payload.size;

    if (getEdition() == BEDROCK) {
        for (int i = 0; i < 4; ++i) {
            insertByte(*((char*) &payloadSize + i));
        }
    } else {
        for (int i = 3; i >= 0; --i) {
            insertByte(*((char*) &payloadSize + i));
        }
    }
}

void nbtpp::Hex::insertByte(const unsigned char& byte) {
    bytesList.emplace_back(byte);
}

void nbtpp::Hex::insertBytesInOppositeOrder(nbtpp::Payload&& payload) {
    for (int i = payload.size - 1; i >= 0; --i) {
        bytesList.emplace_back(payload.ptr[i]);
    }
}

void nbtpp::Hex::pushSpecific(const unsigned char& id, const std::string& name, Payload&& payload) {
    addIdAndNamePrefix(id, name);
    if (getEdition() == JAVA) {
        insertBytesInOppositeOrder(std::move(payload));
    } else {
        insertBytes(std::move(payload));
    }
}

void nbtpp::Hex::pushShortNonspecific(const unsigned char& id, const std::string& name, Payload&& payload) {
    addIdAndNamePrefix(id, name);
    auto payloadSize = (unsigned short) payload.size;
    std::cout << name << " Payload size : " << payloadSize << std::endl;
    if (getEdition() == JAVA) {
        insertByte(*(((char*) &payloadSize) + 1));
        insertByte(*((char*) &payloadSize));
        insertBytes(std::move(payload));
    } else {
        insertByte(*(char*) &payloadSize);
        insertByte(*((char*) &payloadSize) + 1);
        insertBytesInOppositeOrder(std::move(payload));
    }
}

void nbtpp::Hex::pushIntNonspecific(const unsigned char& id, const std::string& name, Payload&& payload) {
    addIdAndNamePrefix(id, name);
    insertIntPayloadPrefix(std::move(payload));
    if (getEdition() == BEDROCK) {
        insertBytes(std::move(payload));
    } else {
        insertBytesInOppositeOrder(std::move(payload));
    }
}

nbtpp::Edition nbtpp::Hex::getEdition() const {
    return edition;
}

void nbtpp::Hex::setEdition(nbtpp::Edition edition) {
    Hex::edition = edition;
}

void nbtpp::Hex::addIdAndNamePrefix(const unsigned char& id, const std::string& name) {
    insertByte(id);
    short nameSize = name.size();
    if (getEdition() == BEDROCK) {
        insertByte(*(char*) &nameSize);
        insertByte(*((char*) &nameSize + 1));
        if (!name.empty()) {
            insertBytes(Payload((unsigned char*) name.c_str(), nameSize));
        }
    } else {
        insertByte(*((char*) &nameSize + 1));
        insertByte(*(char*) &nameSize);
        if (!name.empty()) {
            insertBytes(Payload((unsigned char*) name.c_str(), nameSize));
        }
    }
}

void nbtpp::Hex::pushList(const std::string& name, nbtpp::Payload&& payload) {
    addIdAndNamePrefix(LIST, name);
}

void nbtpp::Hex::pushById(const unsigned char& id, const std::string& name, Payload&& payload) {
    if (id == END || id == BYTE || id == SHORT || id == INT || id == LONG || id == FLOAT || id == DOUBLE) {
        pushSpecific(id, name, std::move(payload));
    } else if (id == BYTE_ARRAY || id == INT_ARRAY || id == LONG_ARRAY) {
        pushIntNonspecific(id, name, std::move(payload));
    } else if (id == STRING) {
        pushShortNonspecific(id, name, std::move(payload));
    } else if (id == LIST) {
        pushList(name, std::move(payload));
    }
}

nbtpp::Hex::~Hex() {

}
