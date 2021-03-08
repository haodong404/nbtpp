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

void nbtpp::Hex::insertByte(const unsigned char& byte) {
    bytesList.emplace_back(byte);
}

void nbtpp::Hex::pushSpecific(const unsigned char& id, const std::string& name, Payload&& payload) {
    addIdAndNamePrefix(id, name);
    if (getEdition() == JAVA) {
        for (int i = 0; i < payload.size; --i) {
            insertByte(payload.ptr[i]);
        }
    } else {
        for (int i = payload.size - 1; i >= 0; --i) {
            insertByte(payload.ptr[i]);
        }
    }
}

void nbtpp::Hex::pushShortNonspecific(const unsigned char& id, const std::string& name, Payload&& payload) {
    addIdAndNamePrefix(id, name);
    unsigned short&& payloadSize = (unsigned short) payload.size;
    if (getEdition() == JAVA) {
        insertByte(*(char*) &payloadSize);
        insertByte(*(char*) (&payloadSize + 1));
        for (int i = 0; i < payloadSize; ++i) {
            insertByte(payload.ptr[i]);
        }
    } else {
        insertByte(*(char*) (&payloadSize + 1));
        insertByte(*(char*) &payloadSize);
        for (int i = payloadSize - 1; i >= 0; --i) {
            insertByte(payload.ptr[i]);
        }
    }
}

void nbtpp::Hex::pushIntNonspecific(const unsigned char& id, const std::string& name, Payload&& payload) {
    addIdAndNamePrefix(id, name);
    int payloadSize = payload.size;

    if (getEdition() == BEDROCK) {
        for (int i = 0; i < 4; ++i) {
            insertByte(*((char*)&payloadSize + i));
        }
        for (int i = 0; i < payloadSize; ++i) {
            insertByte(payload.ptr[i]);
        }
    } else {
        for (int i = 3; i >= 0; --i) {
            insertByte(*((char*)&payloadSize + i));
        }

        for (int i = 0; i < payloadSize; ++i) {
            insertByte(payload.ptr[i]);
        }
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
        insertByte(*((char*)&nameSize + 1));
        if (!name.empty()) {
            insertBytes(Payload((unsigned char*) name.c_str(), nameSize));
        }
    } else {
        insertByte(*((char*)&nameSize + 1));
        insertByte(*(char*) &nameSize);
        if (!name.empty()) {
            insertBytes(Payload((unsigned char*) name.c_str(), nameSize));
        }
    }
}

void nbtpp::Hex::pushById(const unsigned char& id, const std::string& name, Payload&& payload) {
    if (id == END || id == BYTE || id == SHORT || id == INT || id == LONG || id == FLOAT || id == DOUBLE) {
        pushSpecific(id, name, std::move(payload));
    } else if (id == BYTE_ARRAY || id == INT_ARRAY || id == LONG_ARRAY) {
        pushIntNonspecific(id, name, std::move(payload));
    } else if (id == STRING || id == LIST) {
        pushShortNonspecific(id, name, std::move(payload));
    }
}

nbtpp::Hex::~Hex() {

}
