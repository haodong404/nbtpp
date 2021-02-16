//
// Created by Zachary on 2021/2/2.
//

#pragma once

#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <memory>
#include <map>

namespace nbtpp {

    class BaseTag {
    public:
        static size_t length;
        void toHex();

        static size_t getLength();

        /**
         * Set the length of data.
         * @param length
         */
        void setLength(size_t length);
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

        std::string name;

        const static char type_id = 0x0A;

        /**
         * Using a Content class to store the pointer and size of payload.
         */
        struct Content {
            char typeId;
            char* ptr; // The first byte pointer of payload
            unsigned int length; // The length of total data;

            Content(const char& typeId, char* ptr, unsigned int& length);

            /**
             * A copy constructor.
             * @param ano Another one.
             */
            Content(const Compound::Content& ano);
        };

        std::map<std::string, Content> itemMap;

        std::map<std::string, Compound*> internalCompound;

        Compound();

        Compound(Compound* ano);

        /**
         * Add item to content map, it will allocate memory.
         * @param name The name of one item.
         * @param length The length of the data.
         * @param payload The payload.
         * @return the target data's pointer.
         */
        char* addItem(std::string& name, const char& typeId, unsigned int& length, char* payload);


        /**
         * Find item by name, It just fits with ordinary tags.
         * E.g. Byte, Int, Short ...
         * @tparam Tag The target Tag.
         * @param name Target name.
         * @return The result.
         */
        template<typename Tag>
        Tag findItemByName(const char* name) {
            return Tag(itemMap.find(name)->second);
        }

        /**
         * Find the compound by name, it just fits with compound tag, DO NOT transfer others.
         * @tparam Tag The target Tag. It extends Compound.
         * @param name Target name.
         * @return The result.
         */
        template<typename Tag>
        Tag findCompoundByName(const char* name) {
            return Tag(internalCompound.find(name)->second);
        }

        /**
         * Find the list by name, it just fits with List Tag, DO NOT transfer others.
         * @tparam Tag The target Tag. It extends BaseList.
         * @param name Target name.
         * @return The result.
         */
        template<typename Tag>
        Tag findListByName(const char* name) {
            return findItemByName<Tag>(name);
        }

        const std::string& getName() const;

        void setName(const std::string& name);

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

        char payload;

        int toInteger();

        friend std::ostream& operator<<(std::ostream& out, nbtpp::Byte& byte) {
            return out << byte.payload;
        }

        static size_t size();
    };

    struct Short : public BaseTag {
        const static char type_id = 0x02;
        const static char payload_size = 2;

        Short(const short&);

        Short(const Compound::Content&);

        short payload;

        friend std::ostream& operator<<(std::ostream& out, nbtpp::Short& mShort) {
            return out << mShort.payload;
        }

        static size_t size();
    };

    struct Int : public BaseTag {
        const static char type_id = 0x03;
        const static char payload_size = 4;

        Int();

        Int(const int&);

        Int(const Compound::Content&);

        int payload;

        friend std::ostream& operator<<(std::ostream& out, nbtpp::Int& mInt) {
            return out << mInt.payload;
        }

        static size_t size();

    };

    struct Long : public BaseTag {
        const static char type_id = 0x04;
        const static char payload_size = 8;

        Long();

        Long(const long&);

        Long(const Compound::Content&);

        long payload;// Because the size of type long is different on different platforms.

        friend std::ostream& operator<<(std::ostream& out, nbtpp::Long& mLong) {
            return out << mLong.payload;
        }

        static size_t size();

    };

    struct Float : public BaseTag {
        const static char type_id = 0x05;
        const static char payload_size = 4;

        Float();

        Float(const float&);

        Float(const Compound::Content&);

        float payload;

        friend std::ostream& operator<<(std::ostream& out, nbtpp::Float& mFloat) {
            return out << mFloat.payload;
        }

        static size_t size();

    };

    struct Double : public BaseTag {
        const static char type_id = 0x06;
        const static char payload_size = 8;

        Double();

        Double(const double&);

        Double(const Compound::Content&);

        double payload;

        friend std::ostream& operator<<(std::ostream& out, nbtpp::Double& mDouble) {
            return out << mDouble.payload;
        }

        static size_t size();
    };

    struct ByteArray : public NonspecificIntTag {
        const static char type_id = 0x07;

        ByteArray();

        ByteArray(char*);

        ByteArray(const Compound::Content&);

        char* payload;

        static size_t size();

        friend std::ostream& operator<<(std::ostream& out, nbtpp::ByteArray& mByteArray) {
            out << "[";
            for (int i = 0; i < size(); i++) {
                out << mByteArray.payload[i];
                if (i != sizeof(payload) - 1) {
                    out << ", ";
                }
            }
            return out << "]";
        }

        char& operator[](const unsigned int& position);
    };

    struct String : public NonspecificShortTag {
        const static char type_id = 0x08;

        String();

        String(const std::string&);

        String(const char*);

        String(const Compound::Content&);

        std::string payload;

        friend std::ostream& operator<<(std::ostream& out, nbtpp::String& mString) {
            return out << mString.payload;
        }

        static size_t size();

        char& operator[](const unsigned int& position);
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

        friend std::ostream& operator<<(std::ostream& out, nbtpp::IntArray& mIntArray) {
            out << "[";
            for (int i = 0; i < size(); i++) {
                out << mIntArray.payload[i];
                if (i != size() - 1) {
                    out << ", ";
                }
            }
            return out << "]";
        }

        static size_t size();

        int& operator[](const unsigned int& position);
    };

    struct LongArray : public NonspecificIntTag {
        const static char type_id = 0x0C;

        LongArray();

        LongArray(long*);

        LongArray(char*);

        LongArray(const Compound::Content&);

        long* payload;

        friend std::ostream& operator<<(std::ostream& out, nbtpp::LongArray& mLongArray) {
            out << "[";
            for (int i = 0; i < size(); i++) {
                out << mLongArray.payload[i];
                if (i != size() - 1) {
                    out << ", ";
                }
            }
            return out << "]";
        }

        long& operator[](const unsigned int& position);

        static size_t size();
    };

}