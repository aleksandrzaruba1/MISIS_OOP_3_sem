# arbicrypto

### Introduction
arbicrypto is a C++ trading system that executes automatic long/short arbitrage
between multiple exchanges, targeting on a range of cryptocurrencies including
Bitcoin, Ethereum.

### How it works

Cyrptocurrencies markets are still new and inefficient. Multiple exchanges exist
in the world and the prices fluctuate - that is, some specific bid/ask prices of
a specific trading pair can be briefly different from one exchange to another.

arbicrypto is built to exploit these opportunities to automatically profit from
the temporary price difference while maintaining market-neutral, to mitigate
risks.


### How to build & run

Dependencies: <a href="https://www.openssl.org/source" target="_blank">OpenSSL</a>, <a href="http://curl.haxx.se" target="_blank">CURL</a>, <a href="http://www.sqlite.org" target="_blank">SQLite3</a>, <a href="https://cmake.org" target="_blank">CMake</a>

## Manualy 

```bash
    git clone https://github.com/aleksandrzaruba1/MISIS_OOP_3_sem.git
    cmake -S . -B build
    cmake --build build
    ./build/CryptoArb
```

## Automatically

```bash
    ./run.sh
```