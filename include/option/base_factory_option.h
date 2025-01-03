#ifndef FACTORY_OPTION_H
#define FACTORY_OPTION_H

#include <string>
#include <concepts>
#include "payoff/base_payoff.h"
#include "option/base_option.h"
#include "option/parameter_object.h"
#include "market_data/market_data.h"

namespace OptionPricer {

    // Define a concept for an Option type
    template <typename T>
    concept OptionTypeConcept = std::is_base_of_v<Option, T>;

    // Define a concept for an Payoff type
    template <typename T>
    concept PayoffTypeConcept = std::is_base_of_v<Payoff, T>;

    template<OptionTypeConcept OptionT, PayoffTypeConcept PayoffT> class OptionFactory {
    public:
        virtual ~OptionFactory() = default;

        // Non-static create methods as we use virtual methods
        [[nodiscard]] std::shared_ptr<OptionT> createCallOption(const ParameterObject& params);
        [[nodiscard]] std::shared_ptr<OptionT> createPutOption(const ParameterObject& params);

    protected:
        // Template method pattern
        std::shared_ptr<OptionT> createOption(const ParameterObject& params, const PayoffType& type);

        // Can be overiden to add extra parameters
        virtual std::string invalidParams(const std::string& option_type) const;

        // To be overidden in concrete factories
        virtual std::unique_ptr<PayoffT> createSpecificPayoff(const ParameterObject& params,
                                                              const PayoffType& type) = 0;
        virtual std::shared_ptr<OptionT> createSpecificOption(const ParameterObject& params,
                                                              std::unique_ptr<PayoffT> payoff,
                                                              const std::shared_ptr<IMarketData>& marketData) = 0;
        virtual std::string getType(const PayoffType& type) const = 0;
    };


    template<OptionTypeConcept OptionT, PayoffTypeConcept PayoffT>
    std::shared_ptr<OptionT> OptionFactory<OptionT, PayoffT>::createCallOption(const ParameterObject &params) {
        return createOption(params, PayoffType::Call);
    }

    template<OptionTypeConcept OptionT, PayoffTypeConcept PayoffT>
    std::shared_ptr<OptionT> OptionFactory<OptionT, PayoffT>::createPutOption(const ParameterObject &params) {
        return createOption(params, PayoffType::Put);
    }

    template<OptionTypeConcept OptionT, PayoffTypeConcept PayoffT>
    std::shared_ptr<OptionT> OptionFactory<OptionT, PayoffT>::createOption(const ParameterObject &params,
                                                                           const PayoffType& type) {
        auto marketData = MarketData::getInstance();
        std::shared_ptr<OptionT> option = nullptr;

        try {
            auto payoff = createSpecificPayoff(params, type);
            option = createSpecificOption(params, std::move(payoff), marketData);
        } catch (const std::invalid_argument& e) {
            const std::string err = std::string(e.what()) + "\n" + invalidParams(getType(type));
            throw std::invalid_argument(err);
        }

        option->initialize();
        return option;
    }

    template<OptionTypeConcept OptionT, PayoffTypeConcept PayoffT>
    std::string OptionFactory<OptionT, PayoffT>::invalidParams(const std::string &option_type) const {
        return "Invalid parameters for " + option_type + "\n"
               "Expected parameters:\n"
               "  - ticker (string) for ticker symbol\n"
               "  - T (double) for maturity\n"
               "  - K (double) for strike\n";
    }

}

#endif //FACTORY_OPTION_H