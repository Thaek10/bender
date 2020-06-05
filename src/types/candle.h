#include <string>

struct Candle {
  Candle(double o, double h, double l, double c, double v) : open(o), high(h), low(l), close(c), volume(v) {}
  double open;
  double high;
  double low;
  double close;

  int volume;

  const std::string String() const {
    std::string output;
    output = "<Candle open=" + std::to_string(open) + " high=" + std::to_string(high) + " low=" + std::to_string(low) +
             " close=" + std::to_string(close) + ">";
    return output;
  }
};