#include <gtest/gtest.h>
#include "../../BlueCat.h"

namespace Tests {
    class FPNumbers : public ::testing::Test {
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

    TEST_F(FPNumbers, PrintBasicNumber) {
        vm.Interpret(R"( print 8; )");
        EXPECT_STREQ(buffer.str().c_str(), "8");
    }

    TEST_F(FPNumbers, PrintNumberSum) {
        vm.Interpret(R"( print 8 + 32; )");
        EXPECT_STREQ(buffer.str().c_str(), "40");
    }

    TEST_F(FPNumbers, PrintStoredSum) {
        vm.Interpret(R"( var a = 8; var b = 529; print a + b; )");
        EXPECT_STREQ(buffer.str().c_str(), "537");
    }

    TEST_F(FPNumbers, PrintDecimalSum) {
        vm.Interpret(R"( print 9.424442 + 13455423.2335; )");
        EXPECT_STREQ(buffer.str().c_str(), "13455432.657942");
    }

    TEST_F(FPNumbers, PrintStoredNegativesSum) {
        vm.Interpret(R"( var a = -7336.443; var b = -1345.354; print a + b; )");
        EXPECT_STREQ(buffer.str().c_str(), "-8681.797");
    }

    TEST_F(FPNumbers, PrintStoredMultProduct) {
        vm.Interpret(R"( var a = 34678; var b = 2367; print a * b; )");
        EXPECT_STREQ(buffer.str().c_str(), "82082826");
    }

    TEST_F(FPNumbers, PrintStoredDecimalMultProduct) {
        vm.Interpret(R"( var a = -9238.23; var b = 2367.17633; print a * b; )");
        EXPECT_STREQ(buffer.str().c_str(), "-21868519.3870959");
    }

    TEST_F(FPNumbers, PrintStoredDecimalDivProduct) {
        vm.Interpret(R"( var a = -67436.346; var b = 984587.2344; print a / b; )");
        EXPECT_STREQ(buffer.str().c_str(), "-0.06849199709672775");
    }

    TEST_F(FPNumbers, PrintStoredDecimalExponent) {
        vm.Interpret(R"( var a = 1.0334; var b = -2.12; print a ^^ b; )");
        EXPECT_STREQ(buffer.str().c_str(), "0.9327191010362162");
    }
}
