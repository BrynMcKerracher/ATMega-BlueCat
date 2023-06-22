#include <gtest/gtest.h>
#include "../../BlueCat.h"

namespace Tests {
    class FPBytes : public ::testing::Test {
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

    TEST_F(FPBytes, PrintBasicByte) {
        vm.Interpret(R"( print 0b00001101; )");
        EXPECT_STREQ(buffer.str().c_str(), "0b00001101 (13)");
    }

    TEST_F(FPBytes, PrintStoredByte) {
        vm.Interpret(R"( var a = 0b00011101; print a; )");
        EXPECT_STREQ(buffer.str().c_str(), "0b00011101 (29)");
    }

    TEST_F(FPBytes, PrintStoredBytesXOR) {
        vm.Interpret(R"( var a = 0b00011101; var b = 0b00011000; print a ^ b; )");
        EXPECT_STREQ(buffer.str().c_str(), "0b00000101 (5)");
    }

    TEST_F(FPBytes, PrintStoredBytesOR) {
        vm.Interpret(R"( var a = 0b00011101; var b = 0b10000001; print a | b; )");
        EXPECT_STREQ(buffer.str().c_str(), "0b10011101 (157)");
    }

    TEST_F(FPBytes, PrintStoredBytesAND) {
        vm.Interpret(R"( var a = 0b00011101; var b = 0b10000101; print a & b; )");
        EXPECT_STREQ(buffer.str().c_str(), "0b00000101 (5)");
    }

    TEST_F(FPBytes, PrintStoredBytesLSHIFT) {
        vm.Interpret(R"( var a = 0b00011101; var b = 0b00000011; print a << b; )");
        EXPECT_STREQ(buffer.str().c_str(), "0b11101000 (232)");
    }

    TEST_F(FPBytes, PrintStoredBytesRSHIFT) {
        vm.Interpret(R"( var a = 0b00011101; var b = 0b00000011; print a >> b; )");
        EXPECT_STREQ(buffer.str().c_str(), "0b00000011 (3)");
    }

    TEST_F(FPBytes, PrintStoredByteNumberLSHIFT) {
        vm.Interpret(R"( var a = 0b00011101; var b = 1.5; print a << b; )");
        EXPECT_STREQ(buffer.str().c_str(), "0b00111010 (58)");
    }

    TEST_F(FPBytes, PrintStoredByteNumberRSHIFT) {
        vm.Interpret(R"( var a = 0b01011101; var b = 2.545465; print a >> b; )");
        EXPECT_STREQ(buffer.str().c_str(), "0b00010111 (23)");
    }
}
