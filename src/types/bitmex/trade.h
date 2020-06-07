#pragma once

#include <cstdint>
#include <string>

namespace bitmex {
struct Trade {
  std::string timestamp;
  std::string symbol;
  std::string side;
  int64_t size;
  double price;
  std::string tickDirection;
  std::string trdMatchID;
  int64_t grossValue;
  double homeNotional;
  int64_t foreignNotional;

  std::string String() const {
    std::string output;
    output = "<bitmex::Trade timestamp=" + timestamp + " symbol=" + symbol + " side=" + side +
             " size=" + std::to_string(size) + " price=" + std::to_string(price) +
             " tickDirection=" + tickDirection + " trdMatchID=" + trdMatchID +
             " grossValue=" + std::to_string(grossValue) +
             " homeNotional=" + std::to_string(homeNotional) +
             " foreignNotional=" + std::to_string(foreignNotional) + ">";
    return output;
  }
};
} // namespace bitmex