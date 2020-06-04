#include "spdlog/spdlog.h"
#include <iostream>

#include "bitmex.h"
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

  auto t = bitmex::Trade();
  Printer<bitmex::Trade> printer("trade", true);
  bitmex.trade_signals.connect(printer);
  bitmex.Run();
}