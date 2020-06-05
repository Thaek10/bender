#include "candle.h"

using namespace std::chrono;

CandleMaker::CandleMaker(int seconds) : seconds_(seconds) {}

void CandleMaker::Receive(const bitmex::Trade& trade) {
  steady_clock::time_point now = steady_clock::now();
  duration<double> span = duration_cast<duration<double>>(now - start_of_period);

  if (span.count() > seconds_) {
    Candle c = Candle(open, high, low, close, volume);
    candle_signals(c);
    open = high = low = close = volume = 0;
  }
  if (!open) {
    open = trade.price;
    start_of_period = now;
  }
  if (!high || high < trade.price) {
    high = trade.price;
  }
  if (low < trade.price) {
    low = trade.price;
  }
  volume += trade.size;
  close = trade.price;
}