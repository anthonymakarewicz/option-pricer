#include <ostream>
#include <iostream>
#include "market_data/market_data.h"

// Static member initialization to avoid undefined behaviour
std::shared_ptr<MarketData> MarketData::instance_ = nullptr;
std::mutex MarketData::mutex_;

// MarketData implementation
/** @brief
 * Here we cannot use make_shared because constructor is private
 * and the call to the constructor is made outside of the class.
 * Whereas here using new we are explicitly calling the constructor
 * within the scope of the class.
 *
 * The mutex ensures that only one thread can access the creation of
 * the singleton instance at a time so as to prevent race conditions.
 *
 */
std::shared_ptr<MarketData> MarketData::getInstance() {
    std::lock_guard<std::mutex> lock(mutex_); // Ensure thread safety
    if (!instance_) {
        instance_ = std::shared_ptr<MarketData>(new MarketData());
        std::cout << "MakertData instance created" << "\n";
    }
    return instance_;
}

MarketData::~MarketData() { std::cout << "MarketData object properly destroyed!"; }

// Initlize risk-free interest with some default value
MarketData::MarketData() : r_(0.05) {}

void MarketData::addObserver(const std::shared_ptr<MarketDataObserver>& observer) {
    observers_.push_back(observer);
    std::cout << "Observer added. Total observers: " << observers_.size() << std::endl;
}

void MarketData::removeObserver() {
    // Erase_if introduced in C++20 that replace the erase-remove idiom
    std::erase_if(observers_, [](const std::weak_ptr<MarketDataObserver>& obs) {
        return obs.expired();
    });
    std::cout << "Expired observer removed. Total remaining observers: " << observers_.size() << "\n";
}

/**
 * @brief This method will notify only specific observers with ticker
 * as ticker symbol. First, this function checks wether the weak_ptr can access
 * the shared_ptr of MarketData observer it observes. If obs actually poiints to an existing
 * shared_ptr, then it safely casts the shared_ptr<MarketData> as a shared_ptr<Option> before
 * checking wether its ticker matches the one speicified as paramter.
 *
 * As the MarketData class serves only an interface that the Option class must implement
 * and it has no other subclasses, then it is reasonable to assume that the casting will
 * succeed. In fact, the observers_ being a std::vector<std::weak_ptr<MarketDataObserver>>
 * and as MarketDataObserver is an asbtarct class, at run time observers_ will actually
 * poin to Option objects.
 *if (auto opt = std::dynamic_pointer_cast<Option>(obs))
 */
void MarketData::notifyObserver(const std::string& id) const {
    for (const auto& observer : observers_) {
        if (auto obs = observer.lock()) {
            if (obs->getID() == id) {
                obs->update();
            }
        }
    }
}

// Overloaded method that notify all observers
void MarketData::notifyObservers() const {
    for (const auto& observer : observers_) {
        if (auto obs = observer.lock()) {
            obs->update();
        }
    }
}

void MarketData::updateStockPrice(const std::string& ticker, double S) {
    // If statement with initializer C++17
    if (auto it = stockDataMap_.find(ticker); it != stockDataMap_.end()) {
        it->second->setPrice(S);
        notifyObserver(ticker);
    } else {
        throw std::invalid_argument(ticker + " is no present in the MarketData object");
    }
}

void MarketData::updateStockSigma(const std::string& ticker, double sigma) {
    if (auto it = stockDataMap_.find(ticker); it != stockDataMap_.end()) {
        it->second->setSigma(sigma);
        notifyObserver(ticker);
    } else {
        throw std::invalid_argument(ticker + " is no present in the MarketData object");
    }
}

void MarketData::updateStockCoupon(const std::string& ticker, std::optional<double> c) {
    if (auto it = stockDataMap_.find(ticker); it != stockDataMap_.end()) {
        it->second->setCoupon(c);
        notifyObserver(ticker);
    } else {
        throw std::invalid_argument(ticker + " is no present in the MarketData object.");
    }
}

std::shared_ptr<StockData> MarketData::getStockData(const std::string &ticker) const {
    if (auto it = stockDataMap_.find(ticker); it != stockDataMap_.end()) {
        return it->second;
    }
    throw std::invalid_argument(ticker + " Stock data not found.");
}

double MarketData::getR() const {
    return r_;
}

void MarketData::setR(const double& r) {
    r_ = r;
    notifyObservers();
}