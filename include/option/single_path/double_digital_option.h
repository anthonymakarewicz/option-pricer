#ifndef DOUBLE_DIGITAL_OPTION_H
#define DOUBLE_DIGITAL_OPTION_H

#include "base_single_path_option.h"

namespace OptionPricer {
    class DoubleDigitalOption final: public SinglePathOption {
    public:
        ~DoubleDigitalOption() override;
        [[nodiscard]] double calc_price() const override;

    private:
        DoubleDigitalOption(const std::string& ticker, std::unique_ptr<Payoff> payoff, const double& T,
            std::shared_ptr<IMarketData> marketData);

        friend class DoubleDigitalOptionFactory;
    };
}

#endif //DOUBLE_DIGITAL_OPTION_H
