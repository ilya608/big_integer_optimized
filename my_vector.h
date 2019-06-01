#ifndef BIGINT_VECTOR_H
#define BIGINT_VECTOR_H
#include <assert.h>

#include <vector>
#include <memory>
#include <algorithm>
#include <iostream>

template<typename T>

struct my_vector {
    my_vector();

    explicit my_vector(size_t);

    my_vector(size_t, T);

    my_vector(my_vector<T> const &);

    my_vector &operator=(my_vector<T> const &);

    size_t size() const;

    T back() const;

    T const &operator[](size_t) const;

    bool operator==(my_vector<T> const &) const;

    T &operator[](size_t);

    void pop_back();

    void push_back(T);

    void resize(size_t);

    ~my_vector();

    void swap(my_vector &);

public:
    bool is_big = false;
    size_t sz = 0;
    union {
        std::shared_ptr<std::vector<T>> big_obj;
        T small_obj;
    };

    void inv();
};

template<typename T>
my_vector<T>::my_vector() : my_vector(1, 0) {
    sz = 0;
    is_big = false;
}

//template<typename T>
//my_vector<T>::my_vector(size_t size) : my_vector(size, 0) {
//    sz = size;
//    if (size > 1) {
//        is_big = 1;
//        new()
//    }
//    // assert(0);
//}

template<typename T>
my_vector<T>::my_vector(size_t size, T elem) {
    sz = size;
    if (size == 1) {
        small_obj = elem;
    } else {
        new(&big_obj) std::shared_ptr<std::vector<T>>(new std::vector<T>(size, elem));
        is_big = 1;
    }
    if (is_big) {
        //std::cout << "SZ = " << sz << "4 BIG OBJ " << big_obj.get()->size() << '\n';
        assert(sz == big_obj.get()->size()) ;
    }
}

template<typename T>
my_vector<T>::my_vector(my_vector<T> const &other) {
    sz = other.sz;
    is_big = other.is_big;
    if (sz == 1) {
        small_obj = other.small_obj;
    } else {
        new(&big_obj) std::shared_ptr<std::vector<T>>(other.big_obj);
    }
    if (is_big) {
        //std::cout << "SZ = " << sz << " 5BIG OBJ " << big_obj.get()->size() << '\n';
        //std::cout << other.big_obj.get()->size() << '\n';
        assert(sz == big_obj.get()->size()) ;
    }
}

template<typename T>
size_t my_vector<T>::size() const {
    return sz;
}

template<typename T>
my_vector<T> &my_vector<T>::operator=(my_vector<T> const &other) {
    sz = other.sz;
    if (is_big) {
        if (other.is_big) {
            big_obj = other.big_obj;
            std::shared_ptr<std::vector<T>>(other.big_obj);
            is_big = 1;
        } else {
            big_obj.reset();
            small_obj = other.small_obj;
            is_big = 0;
        }
    } else {
        if (other.is_big) {
            new(&big_obj) std::shared_ptr<std::vector<T>>(other.big_obj);
            is_big = 1;
        } else {
            small_obj = other.small_obj;
            is_big = 0;
        }
    }
    if (is_big) {
        //std::cout << "SZ = " << sz << " 6 BIG OBJ " << big_obj.get()->size() << '\n';
        //std::cout << other.big_obj.get()->size() << '\n';
        assert(sz == big_obj.get()->size()) ;
    }
    return *this;
}

template<typename T>
T my_vector<T>::back() const {
    if (sz == 1) return small_obj;
    return big_obj.get()->back();
}

template<typename T>
T const &my_vector<T>::operator[](size_t pos) const {
    return (sz == 1 ? small_obj : (*big_obj.get())[pos]);
}

template<typename T>
bool my_vector<T>::operator==(my_vector<T> const &other) const {
    if (sz != other.sz) return false;
    if (sz == 1) return small_obj == other.small_obj;
    if (sz == 0) return 1;
    return *big_obj.get() == *other.big_obj.get();
}

template<typename T>
T &my_vector<T>::operator[](size_t i) {
    //std::cout << "lol " << '\n';
    //std::cout << sz << '\n';
    if (sz <= 1) return small_obj;
    inv();
    //std::cout << "ahahaha " << big_obj.get()->size() << '\n';
    //std::cout << "i = " << i << '\n';
    return (*big_obj.get())[i];
}

template<typename T>
void my_vector<T>::pop_back() {
    //std::cout << "sSZZZZ = " << sz << '\n';
    if (is_big) {
        //std::cout << "kekeke " << big_obj.get()->size() << '\n';
    }
    if (sz == 1) {
        sz--;
        if (is_big) {
            //std::cout << "SZ = " << sz << "8 BIG OBJ " << big_obj.get()->size() << '\n';
            assert(sz == big_obj.get()->size()) ;
        }
        return;
    }
    inv();
    if (sz == 2) {
        T tmp = (*big_obj.get())[0];
        inv();
        big_obj.reset();
        is_big = 0;
        small_obj = tmp;
        sz--;
        if (is_big) {
            //std::cout << "SZ = " << sz << "9 BIG OBJ " << big_obj.get()->size() << '\n';
            assert(sz == big_obj.get()->size()) ;
        }
        //std::cout << "SZZZZZZZZZZZZZZ = " << sz << '\n';
        return;
    }
    big_obj.get()->pop_back();
    sz--;
    if (is_big) {
        //std::cout << "SZ = " << sz << "10 BIG OBJ " << big_obj.get()->size() << '\n';
        assert(sz == big_obj.get()->size()) ;
    }
}

template<typename T>
void my_vector<T>::push_back(T x) {
    if (sz == 0) {
        small_obj = x;
    } else if (sz == 1) {
        // inv();
        new(&big_obj) std::shared_ptr<std::vector<T>>(new std::vector<T>(1, small_obj));
        big_obj.get()->push_back(x);
        is_big = 1;
    } else {
        inv();
        big_obj.get()->push_back(x);
        is_big = 1;
    }
    sz++;
    if (is_big) {
        //std::cout << "SZ = " << sz << "11 BIG OBJ " << big_obj.get()->size() << '\n';
        assert(sz == big_obj.get()->size()) ;
    }
}

template<typename T>
void my_vector<T>::resize(size_t size) {
    if (size == 0) {
        if (sz > 1) {
            inv();
            (*big_obj.get()).resize(size);
        }
        sz = 0;
        if (is_big) {
            //std::cout << "SZ = " << sz << " 1BIG OBJ " << big_obj.get()->size() << '\n';
            //std::cout << small_obj << '\n';
            assert(sz == big_obj.get()->size()) ;
        }
        return;
    }
    if (size > 1) {
        if (sz > 1) {
            inv();
            (*big_obj.get()).resize(size);
            is_big = 1;
        } else {
            my_vector tmp(size, 0);
            tmp[0] = small_obj;
            swap(tmp);
            is_big = 1;
        }
    }
    sz = size;
    if (is_big) {
        //std::cout << "SZ = " << sz << " 2BIG OBJ " << big_obj.get()->size() << '\n';
        assert(sz == big_obj.get()->size()) ;
    }
}

template<typename T>
void my_vector<T>::inv() {
    if (!big_obj.unique()) {
        big_obj.reset(new std::vector<T>(*big_obj.get()));
    }
    if (is_big) {
        ////std::cout << "SZ = " << sz << "3 BIG OBJ " << big_obj.get()->size() << '\n';
        assert(sz == big_obj.get()->size()) ;
    }

}

template<typename T>
my_vector<T>::~my_vector() {
    //std::cout << " en " << '\n';
    if (is_big) {
        //std::cout << is_big << '\n';
        //std::cout << small_obj << '\n';
        //  //std::cout << big_obj->size() << '\n';
        big_obj.reset();
        is_big = 0;
    }
    sz = 0;
    //std::cout << " fin " << '\n';

}

template<typename T>
void my_vector<T>::swap(my_vector<T> &other) {
    my_vector tmp(other);
    other = *this;
    *this = tmp;
}

#endif //BIGINT_VECTOR_H