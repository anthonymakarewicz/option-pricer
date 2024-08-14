#ifndef BASE_DISTRIBUTION_H
#define BASE_DISTRIBUTION_H

#include <random>

namespace OptionPricer {

    class Distribution {
    public:
        virtual ~Distribution();
        [[nodiscard]] virtual double inv_cdf(const double& quantile) const = 0;
        virtual double operator()(std::mt19937& gen) = 0;
    };

}

#endif //BASE_DISTRIBUTION_H