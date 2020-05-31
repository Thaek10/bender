# Bender is a trading bot

![Bender CI](https://github.com/Thaek10/bender/workflows/Bender%20CI/badge.svg)

Bender supports trading crypto-currency on Bitmex. It is written in C++ and low-latency. It has a unique message-passing architecture.

Status: it doesn't actually trade yet.

## Building

We use the conan package manager to handle dependencies.
```bash
cd build
conan install .. --build missing
cmake ..
make
./bin/bender
```
