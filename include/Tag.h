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
    class End : public BaseTag {
    public:
        const static char type_id = 0x00;
        const static char payload_size = 0;
    };

    class Byte : public BaseTag {
    public:
        const static char type_id = 0x01;
        const static char payload_size = 1;

        Byte(const char& payload);

    private:
        char payload;
    };


    class Short : public BaseTag {
    public:
        const static char type_id = 0x02;
        const static char payload_size = 2;

        Short(const short&);

    private:
        short payload;
    };

    class Int : public BaseTag {
    public:
        const static char type_id = 0x03;
        const static char payload_size = 4;

        Int(const int&);

    private:
        int payload;
    };

    class Long : public BaseTag {
    public:
        const static char type_id = 0x04;
        const static char payload_size = 8;

        Long(std::unique_ptr<char>);

    private:
        std::unique_ptr<char> payload = std::make_unique<char>(); // Because the size of type long is different on different platforms.

    };

    class Float : public BaseTag {
    public:
        const static char type_id = 0x05;
        const static char payload_size = 4;

        Float(const float&);

    private:
        float payload;
    };

    class Double : public BaseTag {
    public:
        const static char type_id = 0x06;
        const static char payload_size = 8;

        Double(const double&);

    private:
        double payload;
    };

    class ByteArray : public NonspecificIntTag {
    public:
        const static char type_id = 0x07;

        ByteArray(const std::list<char>&);

    private:
        std::list<char> payload;
    };

    class String : public NonspecificShortTag {
    public:
        const static char type_id = 0x08;

        String(const std::string&);

    private:
        std::string payload;
    };

    class List : public NonspecificShortTag {
    public:
        const static char type_id = 0x09;
    private:

    };

    class Compound : public NonspecificShortTag {
    public:
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

        char* addItem(std::string& name, unsigned int& size, char* payload);

    };

    class IntArray : public NonspecificIntTag {
    public:
        const static char type_id = 0x0B;
    };

    class LongArray : public NonspecificIntTag {
    public:
        const static char type_id = 0x0C;
    };

}