#!/bin/zsh

echo "================================"
echo "===== del previouse build ====="
echo "==============================\n"

rm -rf build/

echo "build folder has been deleted\n"

echo "================================"
echo "========== build ============="
echo "==============================\n"

cmake -S . -B build
cmake --build build

echo "build successfully\n"

echo "\n=============================="
echo "========== execution ========="
echo "==============================\n"

./build/CryptoArb