#ifndef MC_SINGLE_PATH_H
#define MC_SINGLE_PATH_H

#include "solver/monte_carlo/base_mc.h"
#include "option/single_path/base_single_path_option.h"

namespace OptionPricer {

    class SinglePathMCPricer final: public MCPricer {
    public:
        SinglePathMCPricer(std::shared_ptr<SinglePathOption> option,
                           std::shared_ptr<IMarketData> marketData,
                           std::shared_ptr<StockModel> stockModel);

        ~SinglePathMCPricer() override = default;

        [[nodiscard]] double calculatePrice(const unsigned long& N) const override;

    private:
        std::shared_ptr<SinglePathOption> option_;
    };

}

#endif //MC_SINGLE_PATH_H
