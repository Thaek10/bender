#include "spdlog/spdlog.h"
#include <iostream>

#include "bitmex.h"

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

int main() {
  std::cout << "Hello World!";
  print_art();
  bitmex::BitMexTap bitmex = bitmex::BitMexTap(false);
  bitmex.Run();
}