#include "spdlog/spdlog.h"
#include <iostream>

#include "bitmex.h"
#include "candle.h"
#include "printer.h"
#include "types/bitmex/trade.h"

void print_art() {
  spdlog::info("      _");
  spdlog::info("     ( )");
  spdlog::info("      H");
  spdlog::info("      H");
  spdlog::info("     _H_ ");
  spdlog::info("  .-'-.-'-.");
  spdlog::info(" /         \\");
  spdlog::info("|           |");
  spdlog::info("|   .-------'._");
  spdlog::info("|  / /  '.' '. \\                  BenderBot starting");
  spdlog::info("|  \\ \\ @   @ / /                  Kill all humans!");
  spdlog::info("|   '---------'        ");
  spdlog::info("|    _______|  ");
  spdlog::info("|  .'-+-+-+|  ");
  spdlog::info("|  '.-+-+-+|");
  spdlog::info("|    \"\"\"\"\"\" |");
  spdlog::info("'-.__   __.-'");
  spdlog::info("     \"\"\"");
}

void do_print(const bitmex::Trade& t) { spdlog::info(">" + t.String()); }

int main() {
  using namespace std::placeholders;
  print_art();
  bitmex::BitMexTap bitmex = bitmex::BitMexTap(false);
  CandleMaker candles = CandleMaker(5);

  auto t = bitmex::Trade();
  Printer<bitmex::Trade> printer("trade", true);
  Printer<Candle> candle_printer("candle", true);

  bitmex.trade_signals.connect(printer);
  bitmex.trade_signals.connect(std::bind(&CandleMaker::Receive, &candles, std::placeholders::_1));

  candles.candle_signals.connect(candle_printer);

  bitmex.Run();
}