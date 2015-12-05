// (c) Copyright 2015 Josh Wright
#ifndef CONTAINERS_DYNAMIC_ARRAY_H
#define CONTAINERS_DYNAMIC_ARRAY_H

#include <cstring>
#include <algorithm>
#include <stdexcept>

namespace containers {

    template<typename T>
    class dynamic_array {


        std::size_t _capacity;
        std::size_t _size;

        T *_data;

        void grow(size_t new_size) {
            /*does not change size, only capacity*/
            _capacity = new_size;
            /*growing always fills with zeros*/
            T *new_data = new T[_capacity];
            /*don't bother filling them because they aren't accessible without using data()*/
            std::copy_n(_data, _size, new_data);
            /*free the previous array*/
            delete[] _data;
            _data = new_data;
        };

    public:

        dynamic_array(size_t size = 0, T initial_value = T()) : _capacity(size), _size(size),
                                                                _data(new T[_capacity]) {
            std::fill_n(_data, _capacity, initial_value);
        }

        ~dynamic_array() { delete[] _data; }

        const size_t size() const { return _size; };

        const size_t capacity() const { return _capacity; };

        T *begin() { return _data; };

        const T *begin() const { return _data; };

        T *end() { return _data + _size; };

        const T *end() const { return _data + _size; };

        const T *data() const { return _data; };

        T *data() { return _data; };

        T &operator[](size_t idx) {
            if (idx < 0 || idx >= _size) {
                throw std::out_of_range("Out of range");
            }
            return _data[idx];
        };

        const T &operator[](size_t idx) const {
            if (idx < 0 || idx >= _size) {
                throw std::out_of_range("Out of range");
            }
            return _data[idx];
        };

        void append(T element) {
            if (_size == _capacity) {
                grow(_size + 1);
            }
            /*this is safe because there is extra padding space after _size*/
            _data[_size] = element;
            _size++;
        }

        template<typename F>
        dynamic_array<T> &operator*=(const F &lhs) {
            std::transform(begin(), end(), begin(), [&lhs](T &n) { return n * lhs; });
            return *this;
        };

        template<typename F>
        dynamic_array<T> &operator/=(const F &lhs) {
            std::transform(begin(), end(), begin(), [&lhs](T &n) { return n / lhs; });
            return *this;
        };

        template<typename F>
        dynamic_array<T> &operator+=(const F &lhs) {
            std::transform(begin(), end(), begin(), [&lhs](T &n) { return n + lhs; });
            return *this;
        };

        template<typename F>
        dynamic_array<T> &operator-=(const F &lhs) {
            std::transform(begin(), end(), begin(), [&lhs](T &n) { return n - lhs; });
            return *this;
        };

        /*TODO: copy constructor*/
        /*TODO: move constructor*/
        /*TODO: copy-assignment constructor*/

    };

}
#endif //CONTAINERS_DYNAMIC_ARRAY_H