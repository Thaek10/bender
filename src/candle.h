
#include "types/bitmex/trade.h"
#include "types/candle.h"
#include <boost/signals2/signal.hpp>
#include <chrono>

class CandleMaker {
public:
  CandleMaker(int seconds);

  void Receive(const bitmex::Trade& trade);

  boost::signals2::signal<void(const Candle&)> candle_signals;

private:
  int seconds_;

  double open;
  double high;
  double low;
  double close;
  int volume;

  std::chrono::steady_clock::time_point start_of_period;
};