#include <gtest/gtest.h>
#include "../../BlueCat.h"

namespace Tests {
    class FPControlFlows : public ::testing::Test {
    protected:
        BlueCat::VirtualMachine vm;
        std::streambuf *sbuf = nullptr;
        std::stringstream buffer;

        void SetUp() override {
            sbuf = std::cout.rdbuf();
            std::cout.rdbuf(buffer.rdbuf());
            std::cout.precision(16);
        }

        void TearDown() override {
            std::cout.rdbuf(sbuf);
        }
    };

    TEST_F(FPControlFlows, PrintForIndex) {
        vm.Interpret(R"(
            for (var i = 0; i < 20; i = i + 1) {
                print i;
            }
         )");
        EXPECT_STREQ(buffer.str().c_str(), "012345678910111213141516171819");
    }

    TEST_F(FPControlFlows, PrintWhileIndex) {
        vm.Interpret(R"(
            var i = 0;
            while (i < 20) {
                print i;
                i = i + 1;
            }
         )");
        EXPECT_STREQ(buffer.str().c_str(), "012345678910111213141516171819");
    }

    TEST_F(FPControlFlows, PrintConditionalValue) {
        vm.Interpret(R"(
            var bool = true;
            if (true) {
                print "true";
            }
         )");
        EXPECT_STREQ(buffer.str().c_str(), "true");
    }

    TEST_F(FPControlFlows, PrintElseValue) {
        vm.Interpret(R"(
            var bool = false;
            if (true) {
                print "true";
            }
            else {
                print "false";
            }
         )");
        EXPECT_STREQ(buffer.str().c_str(), "true");
    }

    TEST_F(FPControlFlows, PrintElseIfValue) {
        vm.Interpret(R"(
            var num = 10;
            if (num < 10) {
                print "false";
            }
            else if (num == 10) {
                print "true";
            }
            else {
                print "false";
            }
         )");
        EXPECT_STREQ(buffer.str().c_str(), "true");
    }
}