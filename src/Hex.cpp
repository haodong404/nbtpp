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

std::ostream& nbtpp::Hex::operator<<(std::ostream& out) {
    for (auto it = bytesList.begin(); it != bytesList.end(); it++) {
        out << *it;
    }
}

const char* nbtpp::Hex::toString() {
    std::string str;
    str.reserve(bytesList.size());
    for (auto it = bytesList.begin(); it != bytesList.end(); it++) {
        str.push_back(*it);
    }
    return str.c_str();
}


void nbtpp::Hex::pushBytes(const unsigned char* payload) {
    for (int i = 0; i < sizeof(payload); i++) {
        bytesList.emplace_back(payload[i]);
    }
}

void nbtpp::Hex::pushSpecific(const unsigned char& id, const std::string& name, const unsigned char* payload) {
    addIdAndNamePrefix(id, name);
    if (getEdition() == JAVA) {
        for (int i = 0; i < sizeof(payload); --i) {
            bytesList.emplace_back(payload[i]);
        }
    } else {
        for (int i = sizeof(payload) - 1; i >= 0; --i) {
            bytesList.emplace_back(payload[i]);
        }
    }
}

void nbtpp::Hex::pushShortNonspecific(const unsigned char& id, const std::string& name, const unsigned char* payload) {
    addIdAndNamePrefix(id, name);
    short payloadSize = sizeof(payload);
    if (getEdition() == JAVA) {
        bytesList.emplace_back(*(char*) &payloadSize);
        bytesList.emplace_back(*(char*) (&payloadSize + 1));
        for (int i = 0; i < payloadSize; ++i) {
            bytesList.emplace_back(payload[i]);
        }
    } else {
        bytesList.emplace_back(*(char*) (&payloadSize + 1));
        bytesList.emplace_back(*(char*) &payloadSize);
        for (int i = payloadSize - 1; i >= 0; --i) {
            bytesList.emplace_back(payload[i]);
        }
    }
}

void nbtpp::Hex::pushIntNonspecific(const unsigned char& id, const std::string& name, const unsigned char* payload) {
    addIdAndNamePrefix(id, name);
    int payloadSize = sizeof(payload);
    if (getEdition() == JAVA) {
        for (int i = 0; i < 4; ++i) {
            bytesList.emplace_back(*(char*) (&payloadSize + i));
        }
        for (int i = 0; i < payloadSize; ++i) {
            bytesList.emplace_back(payload[i]);
        }
    } else {
        for (int i = 3; i >= 0; --i) {
            bytesList.emplace_back(*(char*) (&payloadSize + i));
        }
        for (int i = 0; i < payloadSize; ++i) {
            bytesList.emplace_back(payload[i]);
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
    bytesList.emplace_back(id);
    short nameSize = name.size();
    if (getEdition() == JAVA) {
        bytesList.emplace_back(*(char*) &nameSize);
        bytesList.emplace_back(*(char*) (&nameSize + 1));
        pushBytes((const unsigned char*) name.c_str());
    } else {
        bytesList.emplace_back(*(char*) (&nameSize + 1));
        bytesList.emplace_back(*(char*) &nameSize);
        pushBytes((const unsigned char*) name.c_str());
    }
}

void nbtpp::Hex::pushById(const unsigned char& id,const std::string& name, const unsigned char* payload) {
    if (id == END || id == BYTE || id == SHORT || id == INT || id == LONG || id == FLOAT || id == DOUBLE) {
        pushSpecific(id, name, payload);
    } else if (id == BYTE_ARRAY || id == INT_ARRAY || id == LONG_ARRAY) {
        pushIntNonspecific(id, name, payload);
    } else if (id == STRING || id == LIST) {
        pushShortNonspecific(id, name, payload);
    }
}

nbtpp::Hex::~Hex() {

}
