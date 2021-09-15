#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lib/catch.hpp"
#include "lib/mstats-utils.h"

// Testers 1-5
TEST_CASE("tester1", "[weight=1][part=5][suite=week2]") {
  system("make -s");
  system("./mstats tests/testers_exe/tester1 evaluate");
  mstats_result * result = read_mstats_result("mstats_result.txt");
  system("rm mstats_result.txt");
  REQUIRE(result->status == 1);
}

TEST_CASE("tester1", "[weight=1][part=5][suite=week2]") {
  system("make -s");
  system("./mstats tests/testers_exe/tester2 evaluate");
  mstats_result * result = read_mstats_result("mstats_result.txt");
  system("rm mstats_result.txt");
  REQUIRE(result->status == 1);
}

TEST_CASE("tester1", "[weight=1][part=5][suite=week2]") {
  system("make -s");
  system("./mstats tests/testers_exe/tester3 evaluate");
  mstats_result * result = read_mstats_result("mstats_result.txt");
  system("rm mstats_result.txt");
  REQUIRE(result->status == 1);
}

TEST_CASE("tester1", "[weight=1][part=5][suite=week2]") {
  system("make -s");
  system("./mstats tests/testers_exe/tester4 evaluate");
  mstats_result * result = read_mstats_result("mstats_result.txt");
  system("rm mstats_result.txt");
  REQUIRE(result->status == 1);
}

TEST_CASE("tester1", "[weight=1][part=5][suite=week2]") {
  system("make -s");
  system("./mstats tests/testers_exe/tester5 evaluate");
  mstats_result * result = read_mstats_result("mstats_result.txt");
  system("rm mstats_result.txt");
  REQUIRE(result->status == 1);
}
