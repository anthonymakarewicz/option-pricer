#include <iostream>
#include <option/single_path/european_option.h>
#include "market_data/market_data.h"
#include <payoff/single_strike/payoff_vanilla.h>
#include <option/single_path/factory_european_option.h>
#include "option/single_path/factory_double_digital_option.h"
#include <boost/random/sobol.hpp>
#include <iomanip>
#include <vector>
#include <fstream>
#include <filesystem>
#include <option/single_path/factory_digital_option.h>
#include "option/path_dependent/factory_asian_option.h"
#include "option/path_dependent/factory_barrier_option.h"
#include "option/path_dependent/factory_lookback_option.h"
#include "option/path_dependent/factory_american_option.h"

#include "solver/monte_carlo/mc_solver.h"
#include "solver/monte_carlo/builder/mc_builder_american.h"
#include "random/distribution/standard_normal_distribution.h"
#include "random/number_generator/sobol_quasi_random_generator.h"
#include <Eigen/Dense>
#include <solver/monte_carlo/regression/ridge.h>
#include "solver/monte_carlo/basis_function/legendre.h"

using namespace OptionPricer;

int main() {
    std::string ticker = "AAPL";
    double T = 1.0;
    double K = 100.0;
    double S = 100.0;
    double sigma = 0.2;
    double r = 0.05;
    double B = 120;
    double dim = 52;
    auto marketData = MarketData::getInstance();
    marketData->addStock(ticker, S, sigma);
    marketData->setR(r);

    ParameterObject params;
    params.setParameter("ticker", "AAPL");
    params.setParameter("T", T);
    params.setParameter("K", K);
    params.setParameter("B", B);
    params.setParameter("direction", BarrierDirection::Up);

    auto normal = std::make_shared<StandardNormalDistribution>();
    auto generator = std::make_shared<SobolGenerator>(normal, dim);
    auto brownianMotion = std::make_shared<GeometricBrownianMotionModel>(ticker, marketData);

    AmericanOptionFactory factory;
    auto call = factory.createCallOption(params);

    AmericanMCBuilder builder;
    auto americanPricer = builder.setOption(call).setSteps(25).build();

    MCSolver mcSolver;
    mcSolver.setN(100000);
    mcSolver.setPricer(std::move(americanPricer));

    std::cout << mcSolver.solve() << "\n";

    auto ridgeStrat = std::make_shared<RidgeRegression>(0);
    auto americanPricer2 = builder.setRegressionStrategy(ridgeStrat).build();

    mcSolver.setPricer(std::move(americanPricer2));
    std::cout << mcSolver.solve() << "\n";

    auto legendreStrat = std::make_shared<LegendreBasisFunction>(5);
    auto americanPricer3 = builder.setBasisFunctionStrategy(legendreStrat).build();

    mcSolver.setPricer(std::move(americanPricer3));
    std::cout << mcSolver.solve() << "\n";

    return 0;

    /*
    AmericanOptionFactory factory;
    FloatingStrikeLookbackOptionFactory factory2;
    KnockOutBarrierOptionFactory factory3;
    auto call = factory.createCallOption(params);
    auto call2 = factory2.createCallOption(params);
    auto call3 = factory3.createCallOption(params);

    auto mcStrategy = std::make_unique<AmericanMCStrategy>(call, brownianMotion, generator, marketData, dim);
    auto mcStrategy2 = std::make_unique<FloatingStrikeLookbackMCStrategy>(call2, brownianMotion, generator, marketData, dim);
    auto mcStrategy3 = std::make_unique<KnockOutMCStrategy>(call3, brownianMotion, generator, marketData, dim);


    MCSolver mcSolver;
    mcSolver.setN(100000);
    mcSolver.setStrategy(std::move(mcStrategy));

    std::cout << mcSolver.solve() << "\n";

    mcSolver.setStrategy(std::move(mcStrategy2));
    std::cout << mcSolver.solve() << "\n";

    mcSolver.setStrategy(std::move(mcStrategy3));
    std::cout << mcSolver.solve() << "\n";


    //std::vector<unsigned long> Ns = {100, 1000, 10000, 100000, 1000000, 10000000};
    */


    /*
    std::cout << "Crude MC Call:" << std::endl;
    for (const auto& N : Ns) {
        mcSolver.setN(N);
        auto mcStrategy = std::make_unique<SinglePathMCStrategy>(call, brownianMotion, generator, marketData);
        mcSolver.setStrategy(std::move(mcStrategy));
        std::cout << "N = " << N << ", Price: " << mcSolver.solve() << "\n";
    }

    auto put = factory.createPutOption(params);

    std::cout << "Crude MC Put:" << std::endl;
    for (const auto& N : Ns) {
        mcSolver.setN(N);
        auto mcStrategy = std::make_unique<SinglePathMCStrategy>(put, brownianMotion, generator, marketData);
        mcSolver.setStrategy(std::move(mcStrategy));
        std::cout << "N = " << N << ", Price: " << mcSolver.solve() << "\n";
    }

    auto generator2 = std::make_shared<SobolGenerator>(normal);

    std::cout << "Quasi MC Sobol:" << std::endl;
    for (const auto& N : Ns) {
        mcSolver.setN(N);
        auto mcStrategy = std::make_unique<SinglePathMCStrategy>(call, brownianMotion, generator2, marketData);
        mcSolver.setStrategy(std::move(mcStrategy));
        std::cout << "N = " << N << ", Price: " << mcSolver.solve() << "\n";
    }

    auto generator3 = std::make_shared<FaureGenerator>(normal);

    std::cout << "Quasi MC Faure:" << std::endl;
    for (const auto& N : Ns) {
        mcSolver.setN(N);
        auto mcStrategy = std::make_unique<SinglePathMCStrategy>(call, brownianMotion, generator3, marketData);
        mcSolver.setStrategy(std::move(mcStrategy));
        std::cout << "N = " << N << ", Price: " << mcSolver.solve() << "\n";
    }

    return 0;

    */

    /*
auto mcStrategy = std::make_unique<SinglePathMCStrategy>(eurCall, brownianMotion, generator, marketData);
mcSolver.setStrategy(std::move(mcStrategy));
std::cout << "Crude: " << mcSolver.solve() << "\n";

auto generator2 = std::make_shared<FaureGenerator>(normal);
mcStrategy = std::make_unique<SinglePathMCStrategy>(eurCall, brownianMotion, generator2, marketData);
mcSolver.setStrategy(std::move(mcStrategy));
std::cout << "Faure: " << mcSolver.solve() << "\n";
*/

    /*
    auto marketData = MarketData::getInstance();
    marketData->addStock("AAPL", 120.0, 0.2);

    ParameterObject params;
    params.setParameter("ticker", "AAPL");
    params.setParameter("T", 1.0);
    params.setParameter("K1", 100.0);
    params.setParameter("K2", 120.0);

    DoubleDigitalOptionFactory factory;
    try {
        auto option = factory.createCallOption(params);
    } catch (const std::exception& e) {
        std::cerr << e.what();
    }
    std::cout << "ded";

    */


}