#include "random/number_generator/base_generator.h"

namespace OptionPricer {

    NumberGenerarator::NumberGenerarator(std::shared_ptr<Distribution> dist)
    : dist_(std::move(dist)) {
        std::random_device rd;
        seed_ = rd();
    }

    NumberGenerarator::NumberGenerarator(std::shared_ptr<Distribution> dist, const unsigned int &seed)
    : dist_(std::move(dist)), seed_(seed) {
        if (seed_ < 0) {
            throw std::invalid_argument("seed parameetr must be a positve integer");
        }
    }

    NumberGenerarator::~NumberGenerarator() = default;

}