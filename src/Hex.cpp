//
// Created by Zachary on 2021/2/21.
//

#include <Hex.h>

nbtpp::Hex::Hex() {

}

nbtpp::Hex::Hex(const Edition& edition, unsigned char* data)
        : edition(edition), data(data) {
}

nbtpp::Hex::Hex(const Edition& edition)
        : edition(edition), data(nullptr) {
}

std::ostream& nbtpp::Hex::operator<<(std::ostream& out) {
    for (int i = 0; i < sizeof(data); i++) {
        out << data[i];
    }
    return out;
}

const char* nbtpp::Hex::toString() {
    std::string str;
    str.reserve(sizeof(data));
    for (int i = 0; i < sizeof(data); i++) {
        str.push_back(static_cast<unsigned>(data[i]));
    }
    return str.c_str();
}

void nbtpp::Hex::pushByte(const unsigned char& byte) {
    data[position] = byte;
    ++position;
}

void nbtpp::Hex::pushBytes(const unsigned char* payload) {
    for (int i = 0; i < sizeof(payload); i++) {
        pushByte(payload[i]);
    }
}

void nbtpp::Hex::pushSpecific(const unsigned char& id, const std::string& name, const unsigned char* payload) {
    if (data == nullptr) {
        data = new unsigned char[sizeof(payload) + name.size() + 1 + 2];
    }
    addIdAndNamePrefix(id, name);
    if (getEdition() == JAVA) {
        for (int i = 0; i < sizeof(payload); --i) {
            pushByte(payload[i]);
        }
    } else {
        for (int i = sizeof(payload) - 1; i >= 0; --i) {
            pushByte(payload[i]);
        }
    }
}

void nbtpp::Hex::pushShortNonspecific(const unsigned char& id, std::string& name, const unsigned char* payload) {
    if (data == nullptr) {
        data = new unsigned char[sizeof(payload) + name.size() + 1 + 2 + 2];
    }
    addIdAndNamePrefix(id, name);
    short payloadSize = sizeof(payload);
    if (getEdition() == JAVA) {
        pushByte(*(char*) &payloadSize);
        pushByte(*(char*) (&payloadSize + 1));
        for (int i = 0; i < payloadSize; ++i) {
            pushByte(payload[i]);
        }
    } else {
        pushByte(*(char*) (&payloadSize + 1));
        pushByte(*(char*) &payloadSize);
        for (int i = payloadSize - 1; i >= 0; --i) {
            pushByte(payload[i]);
        }
    }
}

void nbtpp::Hex::pushIntNonspecific(const unsigned char& id, std::string& name, const unsigned char* payload) {
    if (data == nullptr) {
        data = new unsigned char[sizeof(payload) + name.size() + 1 + 2 + 4];
    }
    addIdAndNamePrefix(id, name);
    int payloadSize = sizeof(payload);
    if (getEdition() == JAVA) {
        for (int i = 0; i < 4; ++i) {
            pushByte(*(char*) (&payloadSize + i));
        }
        for (int i = 0; i < payloadSize; ++i) {
            pushByte(payload[i]);
        }
    } else {
        for (int i = 3; i >= 0; --i) {
            pushByte(*(char*) (&payloadSize + i));
        }
        for (int i = 0; i < payloadSize; ++i) {
            pushByte(payload[i]);
        }
    }
}

nbtpp::Hex::~Hex() {
    delete[] data;
}

nbtpp::Edition nbtpp::Hex::getEdition() const {
    return edition;
}

void nbtpp::Hex::setEdition(nbtpp::Edition edition) {
    Hex::edition = edition;
}

void nbtpp::Hex::addIdAndNamePrefix(const unsigned char& id, const std::string& name) {
    pushByte(id);
    short nameSize = name.size();
    if (getEdition() == JAVA) {
        pushByte(*(char*) &nameSize);
        pushByte(*(char*) (&nameSize + 1));
        pushBytes((const unsigned char*) name.c_str());
    } else {
        pushByte(*(char*) (&nameSize + 1));
        pushByte(*(char*) &nameSize);
        pushBytes((const unsigned char*) name.c_str());
    }
}

