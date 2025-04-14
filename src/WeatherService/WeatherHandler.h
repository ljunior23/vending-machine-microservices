#ifndef VENDING_MACHINE_MICROSERVICES_WEATHERHANDLER_H
#define VENDING_MACHINE_MICROSERVICES_WEATHERHANDLER_H

#include <iostream>
#include <string>

#include "../../gen-cpp/WeatherService.h"
#include "../logger.h"

namespace vending_machine {

class WeatherServiceHandler : public WeatherServiceIf {
 public:
  WeatherServiceHandler() = default;
  ~WeatherServiceHandler() override = default;

  WeatherType::type GetWeather(const int64_t city) override {
    LOG(info) << "GetWeather called with city: " << city;

    // Implement odd-even mechanism
    // If city ID is odd, return WARM
    // If city ID is even, return COLD
    if (city % 2 == 1) {
      LOG(info) << "City ID " << city << " is odd, returning WARM weather";
      return WeatherType::WARM;
    } else {
      LOG(info) << "City ID " << city << " is even, returning COLD weather";
      return WeatherType::COLD;
    }
  }
};

} // namespace vending_machine

#endif //VENDING_MACHINE_MICROSERVICES_WEATHERHANDLER_H
