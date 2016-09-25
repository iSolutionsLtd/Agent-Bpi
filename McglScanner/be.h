#ifndef _BE_CLS
#define _BE_CLS

#include <arpa/inet.h> // for ntohs() etc.
#include <stdint.h>
#include <time.h>

class be_uint16_t {
public:
        be_uint16_t() : be_val_(0) {
        }
        // Transparently cast from uint16_t
        be_uint16_t(const uint16_t &val) : be_val_(htons(val)) {
        }
        // Transparently cast to uint16_t
        operator uint16_t() const {
                return (be_val_);
        }
private:
        uint16_t be_val_;
} __attribute__((packed));


class be_uint32_t {
public:
        be_uint32_t() : be_val_(0) {
        }
        // Transparently cast from uint32_t
        be_uint32_t(const uint32_t &val) : be_val_(htonl(val)) {
        }
        // Transparently cast to uint32_t
        operator uint32_t() const {
                return ntohl(be_val_);
        }
private:
        uint32_t be_val_;
} __attribute__((packed));
#endif