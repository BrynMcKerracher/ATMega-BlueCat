#include <gtest/gtest.h>
#include "../../BlueCat.h"

namespace Tests {
    class FPStrings : public ::testing::Test {
    protected:
        BlueCat::VirtualMachine vm;
        std::streambuf *sbuf = nullptr;
        std::stringstream buffer;

        void SetUp() override {
            sbuf = std::cout.rdbuf();
            std::cout.rdbuf(buffer.rdbuf());
        }

        void TearDown() override {
            std::cout.rdbuf(sbuf);
        }
    };


    TEST_F(FPStrings, BasicPrint) {
        vm.Interpret(R"( print "test"; )");
        EXPECT_STREQ(buffer.str().c_str(), "test");
    }

    TEST_F(FPStrings, PrintEscapedCharacters) {
        vm.Interpret(R"( print "\t\n"; )");
        EXPECT_STREQ(buffer.str().c_str(), "\t\n");
    }

    TEST_F(FPStrings, PrintRawStringEscapedChars) {
        vm.Interpret(R""( print R"(\t\n)"; )"");
        EXPECT_STREQ(buffer.str().c_str(), "\\t\\n");
        EXPECT_STRNE(buffer.str().c_str(), "\t\n");
    }

    TEST_F(FPStrings, PrintMultilineString) {
        vm.Interpret(R""( print "




"; )"");
        EXPECT_STREQ(buffer.str().c_str(), "\n\n\n\n\n");
    }

    TEST_F(FPStrings, PrintMultilineRawString) {
        vm.Interpret(R""( print R"(




)"; )"");
        EXPECT_STREQ(buffer.str().c_str(), "\n\n\n\n\n");
    }

    TEST_F(FPStrings, PrintStoredString) {
        vm.Interpret(R"( var string = "Hello there!\n"; print string; )");
        EXPECT_STREQ(buffer.str().c_str(), "Hello there!\n");
        EXPECT_STRNE(buffer.str().c_str(), "Hello there!\\n");
    }

    TEST_F(FPStrings, PrintStoredRawString) {
        vm.Interpret(R""( var string = R"(Hello there!\n)"; print string; )"");
        EXPECT_STREQ(buffer.str().c_str(), "Hello there!\\n");
        EXPECT_STRNE(buffer.str().c_str(), "Hello there!\n");
    }
}