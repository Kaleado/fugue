//
// Created by kaleado on 5/02/19.
//

#include "DataItem.hpp"

Fugue::DataItem::DataItem(void* r, std::size_t l) : raw{r}, length{l} {}

template<>
void Fugue::DataItem::free<void>() {
    operator delete(raw, length);
    raw = nullptr;
    length = 0;
}