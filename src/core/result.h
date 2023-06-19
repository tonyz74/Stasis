#ifndef STASIS_CORE_RESULT_H
#define STASIS_CORE_RESULT_H

#include "log.h"
#include "asserts.h"
#include "types.h"

template<typename T, typename E>
class result {
public:
    static result<T, E> ok(T x) {
        return (result) {
            .tag = 0,
            .inner = x
        };
    }

    static result<T, E> err(E err) {
        return (result) {
            .tag = 1,
            .inner = err
        };
    }

    bool is_ok() const {
        return tag == 0; 
    }

    bool is_err() const {
        return tag == 1;
    }

    T unwrap() const {
        if (is_err()) {
            PANIC("[core] Tried to unwrap an error (or invalid)!"); 
        } 

        return inner;
    }

    E get_err() const {
        if (is_ok()) {
            PANIC("[core] Tried to get error of an OK (or invalid)!");
        }

        return inner;
    }

private:
    i8 tag = -1;
    union {
        T ok;
        E payload;
    } inner;

};

#endif // !STASIS_CORE_RESULT_H
