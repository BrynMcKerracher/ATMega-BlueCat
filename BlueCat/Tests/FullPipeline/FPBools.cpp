#include <gtest/gtest.h>
#include "../../BlueCat.h"

namespace Tests {
    class FPBools : public ::testing::Test {
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

    TEST_F(FPBools, PrintBoolLiteral) {
        vm.Interpret(R"( print true; )");
        EXPECT_STREQ(buffer.str().c_str(), "true");
    }

    TEST_F(FPBools, PrintBoolStored) {
        vm.Interpret(R"( var a = false; print a; )");
        EXPECT_STREQ(buffer.str().c_str(), "false");
    }

    TEST_F(FPBools, PrintNotBoolStored) {
        vm.Interpret(R"( var a = false; print !a; )");
        EXPECT_STREQ(buffer.str().c_str(), "true");
    }

    TEST_F(FPBools, PrintBoolsStoredAND) {
        vm.Interpret(R"( var a = false; var b = true; print a and b; )");
        EXPECT_STREQ(buffer.str().c_str(), "false");
    }

    TEST_F(FPBools, PrintBoolsStoredOR) {
        vm.Interpret(R"( var a = false; var b = true; print a or b; )");
        EXPECT_STREQ(buffer.str().c_str(), "true");
    }
}
