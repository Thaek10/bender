# Bender is a trading bot

Bender supports trading crypto-currency on Bitmex. It is written in C++ and low-latency. It has a unique message-passing architecture.

## Building

We use the conan package manager to handle dependencies.
```bash
cd build
conan install .. --build missing
cmake
./bin/bender
```