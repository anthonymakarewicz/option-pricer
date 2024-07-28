#ifndef MOCK_CLASSES_H
#define MOCK_CLASSES_H

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "../../../include/market_data/market_data.h"

namespace Mocks {
    // Minimal Mock Option class
    class Option final : public MarketDataObserver, public std::enable_shared_from_this<Option> {
    public:
        explicit Option(std::string ticker) : MarketDataObserver(std::move(ticker)), updated_(false) {}

        void update() override { updated_ = true; }
        bool wasUpdated() const { return updated_; }
        void resetUpdated() { updated_ = false; }

    private:
        bool updated_;
    };

    // Minimal Mock MarketData class
    class MockMarketData {
    public:
        virtual ~MockMarketData() = default;

        virtual std::shared_ptr<StockData> getStockData(const std::string& ticker) const = 0;
        virtual void addObserver(std::shared_ptr<MarketDataObserver> observer) = 0;
        virtual void removeObserver(std::shared_ptr<MarketDataObserver> observer) = 0;

        virtual double getR() const = 0;
    };

    // Mock class for MarketData using Google Mock
    class MarketData final : public MockMarketData {
    public:
        MOCK_METHOD(std::shared_ptr<StockData>, getStockData, (const std::string& ticker), (const, override));
        MOCK_METHOD(void, addObserver, (std::shared_ptr<MarketDataObserver> observer), (override));
        MOCK_METHOD(void, removeObserver, (std::shared_ptr<MarketDataObserver> observer), (override));
        MOCK_METHOD(double, getR, (), (const, override));
    };
}

#endif //MOCK_CLASSES_H