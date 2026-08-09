#pragma once
#include "size_generator.hpp"

class mosaic_generator : public size_generator {
public:
    bool can_generate(provider *) override;
    area_size random_size(provider *, area_size) override;
};