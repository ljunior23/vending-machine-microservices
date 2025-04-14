#ifndef VENDING_MACHINE_MICROSERVICES_BEVERAGEPREFERENCEHANDLER_H
#define VENDING_MACHINE_MICROSERVICES_BEVERAGEPREFERENCEHANDLER_H

#include <iostream>
#include <string>
#include <random>
#include <chrono>
#include <vector>

#include "../../gen-cpp/BeveragePreferenceService.h"
#include "../logger.h"

namespace vending_machine {

class BeveragePreferenceServiceHandler : public BeveragePreferenceServiceIf {
 public:

  BeveragePreferenceServiceHandler() = default;
  ~BeveragePreferenceServiceHandler() override = default;

  void getBeverage(std::string& _return, const BeverageType::type btype) override {
    LOG(info) << "getBeverage called with type: " << (btype == BeverageType::HOT ? "HOT" : "COLD");

    // Seed with a real random value
    std::mt19937 gen(std::chrono::high_resolution_clock::now().time_since_epoch().count());

    if (btype == BeverageType::HOT) {

      // Hot beverages
      std::vector<std::string> hot_beverages = {"cappuccino", "latte", "espresso"};
      std::uniform_int_distribution<> dis(0, hot_beverages.size() - 1);
      _return = hot_beverages[dis(gen)];
    } else {

      // Cold beverages
      std::vector<std::string> cold_beverages = {"lemonade", "ice tea", "soda"};
      std::uniform_int_distribution<> dis(0, cold_beverages.size() - 1);
      _return = cold_beverages[dis(gen)];
    }

    LOG(info) << "Selected beverage: " << _return;
  }
};

} // namespace vending_machine

#endif //VENDING_MACHINE_MICROSERVICES_BEVERAGEPREFERENCEHANDLER_H
