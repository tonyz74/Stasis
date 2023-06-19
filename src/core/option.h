#ifndef STASIS_CORE_OPTION_H
#define STASIS_CORE_OPTION_H

#include "types.h"
#include "asserts.h"

template<typename T>
class option {
public: 
    static option<T> some(T value) {
        option<T> self;
        self.exists = true;
        self.value = value;

        return self;
    }

    static option<T> none() {
        option<T> self;
        self.exists = false;

        return self;
    }

    bool is_some() const {
        return this->exists == true;
    }

    bool is_none() const {
        return this->exists == false;
    }

    T unwrap() {
        ASSERT(is_some(), "Tried to unwrap option::none!\n");
        return this->value;
    }

private:
    bool exists;
    T value; 
};


#endif // !STASIS_CORE_OPTION_H
