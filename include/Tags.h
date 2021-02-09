//
// Created by Zachary on 2021/2/2.
//

#pragma once

#include <string>
#include <list>
#include <memory>
#include <map>

namespace nbtpp {

    enum TagSize : char {
        INT = 4,
        BYTE = 1,
        SHORT = 2,
        LONG = 8,
        DOUBLE = 8,
        FLOAT = 4
    };


    class BaseTag {
        short name_size;
        std::string name;
    public:
        void toHex();

        short getNameSize() const;

        void setNameSize(const short& nameSize);

        const std::string& getName() const;

        void setName(const std::string& m_name);

        void bindName(std::string& name);

    };

    /**
     * The class is a base class for nonspecific payload size that the prefix is a unsigned short.
     * e.g. String, List, Compound ...
     */
    class NonspecificShortTag : public BaseTag {
        unsigned short payload_size;

    };


    /**
     * The class is a base class for nonspecific payload size that the prefix is a signed integer.
     * e.g. ByteArray, IntArray ...
     */
    class NonspecificIntTag : public BaseTag {
        int payload_size;

    };

    /**
     * The Tag_End class
     */
    struct End : public BaseTag {
        const static char type_id = 0x00;
        const static char payload_size = 0;
    };

    struct Byte : public BaseTag {

        const static char type_id = 0x01;
        const static char payload_size = 1;

        Byte(const char& payload);

        char payload;
    };


    struct Short : public BaseTag {
        const static char type_id = 0x02;
        const static char payload_size = 2;

        Short(const short&);

        short payload;
    };

    struct Int : public BaseTag {
        const static char type_id = 0x03;
        const static char payload_size = 4;

        Int();
        Int(const int&);

        int payload;
    };

    struct Long : public BaseTag {
        const static char type_id = 0x04;
        const static char payload_size = 8;

        Long(std::unique_ptr<char>);

        std::unique_ptr<char> payload = std::make_unique<char>(); // Because the size of type long is different on different platforms.

    };

    struct Float : public BaseTag {
        const static char type_id = 0x05;
        const static char payload_size = 4;

        Float(const float&);

        float payload;
    };

    struct Double : public BaseTag {
        const static char type_id = 0x06;
        const static char payload_size = 8;

        Double(const double&);

        double payload;
    };

    struct ByteArray : public NonspecificIntTag {
        const static char type_id = 0x07;

        ByteArray(const std::list<char>&);

        std::list<char> payload;
    };

    struct String : public NonspecificShortTag {
        const static char type_id = 0x08;

        String(const std::string&);

        std::string payload;
    };

    struct List : public NonspecificShortTag {
        const static char type_id = 0x09;

    };

    struct Compound : public NonspecificShortTag {

        const static char type_id = 0x0A;

        /**
         * Using a Content class to store the pointer and size of payload.
         */
        struct Content {
            // The first byte pointer of payload
            char* ptr;
            unsigned int size;

            Content(char* ptr, unsigned int& size);

        };

        std::map<std::string, Content> contentMap;

        std::map<std::string, Compound*> internalCompound;

        /**
         * Add item to content map, it will allocate memory.
         * @param name The name of one item.
         * @param size payload size
         * @param payload payload bytes array.
         * @return
         */
        char* addItem(std::string& name, unsigned int& size, char* payload);

    };

    struct IntArray : public NonspecificIntTag {
        const static char type_id = 0x0B;
    };

    struct LongArray : public NonspecificIntTag {
        const static char type_id = 0x0C;
    };
}