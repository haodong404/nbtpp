//
// Created by Zachary on 2021/2/2.
//

#pragma once

#include <string>
#include <list>
#include <memory>
#include <map>

namespace nbtpp {

    class BaseTag {
        short name_size;
        std::string name;

    public:
        void toHex();

        short getNameSize() const;

        void setNameSize(const short& nameSize);

        const std::string& getName() const;

        void setName(const std::string& m_name);
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

    struct Compound : public NonspecificShortTag {

        const static char type_id = 0x0A;

        /**
         * Using a Content class to store the pointer and size of payload.
         */
        struct Content {
            // The first byte pointer of payload
            char* ptr;
            unsigned int length; // The length of total data;
            unsigned int unitSize; // The number of unis, for example: [2, 4, 5], if it's a int array, the unitSize is 3, length is 12;

            Content(char* ptr, unsigned int& length, unsigned int& unitSize);

        };

        std::map<std::string, Content> contentMap;

        std::map<std::string, Compound*> internalCompound;

        /**
         * Add item to content map, it will allocate memory.
         * @param name The name of one item.
         * @param length payload length
         * @param payload payload bytes array.
         * @return
         */
        char* addItem(std::string& name, unsigned int& length, unsigned int& unitSize, char* payload);

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

        Byte();

        Byte(const char& payload);

        Byte(const Compound::Content&);

        int toInteger();

        char payload;
    };

    struct Short : public BaseTag {
        const static char type_id = 0x02;
        const static char payload_size = 2;

        Short(const short&);

        Short(const Compound::Content&);

        short payload;
    };

    struct Int : public BaseTag {
        const static char type_id = 0x03;
        const static char payload_size = 4;

        Int();

        Int(const int&);

        Int(const Compound::Content&);

        int payload;
    };

    struct Long : public BaseTag {
        const static char type_id = 0x04;
        const static char payload_size = 8;

        Long();

        Long(const long&);

        Long(char*);

        Long(const Compound::Content&);

        std::unique_ptr<char*> payload = std::make_unique<char*>(
                new char[8]); // Because the size of type long is different on different platforms.

    };

    struct Float : public BaseTag {
        const static char type_id = 0x05;
        const static char payload_size = 4;

        Float();

        Float(const float&);

        Float(const Compound::Content&);

        float payload;
    };

    struct Double : public BaseTag {
        const static char type_id = 0x06;
        const static char payload_size = 8;

        Double();

        Double(const double&);

        Double(const Compound::Content&);

        double payload;
    };

    struct ByteArray : public NonspecificIntTag {
        const static char type_id = 0x07;

        ByteArray();

        ByteArray(char*);

        ByteArray(const Compound::Content&);

        char* payload;

        size_t size() {
            return sizeof(payload);
        }
    };

    struct String : public NonspecificShortTag {
        const static char type_id = 0x08;

        String();

        String(const std::string&);

        String(const char*);

        String(const Compound::Content&);

        std::string payload;
    };

    struct List : public NonspecificShortTag {
        const static char type_id = 0x09;

    };

    struct IntArray : public NonspecificIntTag {
        const static char type_id = 0x0B;

        IntArray();

        IntArray(int*);

        IntArray(const Compound::Content&);

        int* payload;
    };

    struct LongArray : public NonspecificIntTag {
        const static char type_id = 0x0C;

        LongArray();

        LongArray(long*);

        LongArray(char**);

        LongArray(const Compound::Content&);

        char** payload;
    };
}