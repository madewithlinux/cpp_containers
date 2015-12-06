// (c) Copyright 2015 Josh Wright
#pragma once


template<typename T>
struct functor_class {
    bool has_derivative;
    virtual T operator()(const T&) {}
    virtual T diff(const T&) {}
};
