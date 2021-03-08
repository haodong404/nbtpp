//
// Created by Zachary on 2021/3/8.
//
#pragma once

namespace nbtpp {
    struct Payload {
        unsigned char* ptr;
        unsigned long size;

        Payload(unsigned char* bytes, unsigned long&& size) : ptr(bytes), size(size) {

        }
    };
}
