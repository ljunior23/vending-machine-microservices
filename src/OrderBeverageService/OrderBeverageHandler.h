#ifndef VENDING_MACHINE_MICROSERVICES_ORDERBEVERAGEHANDLER_H
#define VENDING_MACHINE_MICROSERVICES_ORDERBEVERAGEHANDLER_H

#include <iostream>
#include <string>
#include <regex>
#include <future>

#include "../../gen-cpp/OrderBeverageService.h"
#include "../../gen-cpp/WeatherService.h"
#include "../../gen-cpp/BeveragePreferenceService.h"
#include "../ClientPool.h"
#include "../ThriftClient.h"
#include "../logger.h"

namespace vending_machine{

class OrderBeverageServiceHandler : public OrderBeverageServiceIf {
 public:
  OrderBeverageServiceHandler(
      ClientPool<ThriftClient<WeatherServiceClient>> *,
      ClientPool<ThriftClient<BeveragePreferenceServiceClient>> *);
  ~OrderBeverageServiceHandler() override = default;

  void PlaceOrder(std::string& _return, const int64_t city) override;

 private:
  ClientPool<ThriftClient<WeatherServiceClient>> *_weather_client_pool;
  ClientPool<ThriftClient<BeveragePreferenceServiceClient>> *_beverage_preference_client_pool;
};

OrderBeverageServiceHandler::OrderBeverageServiceHandler(
    ClientPool<ThriftClient<WeatherServiceClient>> *weather_client_pool,
    ClientPool<ThriftClient<BeveragePreferenceServiceClient>> *beverage_preference_client_pool) {

  _weather_client_pool = weather_client_pool;
  _beverage_preference_client_pool = beverage_preference_client_pool;
}

void OrderBeverageServiceHandler::PlaceOrder(std::string& _return, const int64_t city) {
  // Log the order
  LOG(info) << "Received PlaceOrder for city: " << city;

  // 1. Get the weather service client pool
  auto weather_client_wrapper = _weather_client_pool->Pop();
  if (!weather_client_wrapper) {
    ServiceException se;
    se.errorCode = ErrorCode::SE_THRIFT_CONN_ERROR;
    se.message = "Failed to connect to weather-service";
    LOG(error) << se.message;
    throw se;
  }

  // 2. call the weather service
  WeatherType::type weather_type;
  try {
    auto weather_client = weather_client_wrapper->GetClient();
    weather_type = weather_client->GetWeather(city);
    LOG(info) << "Weather type for city " << city << ": " 
              << (weather_type == WeatherType::WARM ? "WARM" : "COLD");
    _weather_client_pool->Push(weather_client_wrapper);
  } catch (...) {
    _weather_client_pool->Remove(weather_client_wrapper);
    ServiceException se;
    se.errorCode = ErrorCode::SE_THRIFT_CONN_ERROR;
    se.message = "Failed to connect to weather-service";
    throw se;
  }

  // 3. Determine beverage type based on the weather
  BeverageType::type beverage_type;
  if(weather_type == WeatherType::WARM) {
    beverage_type = BeverageType::COLD;
  } else {
    beverage_type = BeverageType::HOT;
  }

  // 4. Get the beverage preference service client pool
  auto beverage_client_wrapper = _beverage_preference_client_pool->Pop();
  if(!beverage_client_wrapper) {
    ServiceException se;
    se.errorCode = ErrorCode::SE_THRIFT_CONN_ERROR;
    se.message = "Failed to connect to beverage preference service";
    throw se;
  }
  
  try {
    auto beverage_client = beverage_client_wrapper->GetClient();

    // call getBeverage
    std::string beverage_name;
    beverage_client->getBeverage(beverage_name, beverage_type);
    _return = beverage_name;
    
    LOG(info) << "Selected beverage: " << _return;
    _beverage_preference_client_pool->Push(beverage_client_wrapper);
  } catch (...) {
    _beverage_preference_client_pool->Remove(beverage_client_wrapper);
    ServiceException se;
    se.errorCode = ErrorCode::SE_THRIFT_CONN_ERROR;
    se.message = "Failed to connect to beverage preference service";
    throw se;
  }
}

} // namespace vending_machine

#endif // VENDING_MACHINE_MICROSERVICES_ORDERBEVERAGEHANDLER_H
