#ifndef BYTEBAC_SCANNER_H_INCLUDED
#define BYTEBAC_SCANNER_H_INCLUDED

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "Token.h"

namespace BlueCat {
    class Scanner {
    public:
        void Scan(const char* source);
        Token ScanToken();
    protected:
        const char* start;
        const char* current;
        size_t currentLine = 1;

        char Advance();
        char Peek(int8_t offset = 0);
        bool Match(char expected);
        Token::Type MatchEither(char a, char b, Token::Type aType, Token::Type bType, Token::Type defaultType);
        void SkipWhitespace();

        bool IsAtEnd() const;
        static bool IsDigit(char c);
        static bool IsBinaryDigit(char c);
        static bool IsAlpha(char c);
        Token MakeToken(Token::Type type); 
        Token ErrorToken(const char* message);
        
        Token String();
        Token RawString();
        Token Byte();
        Token Increment();
        Token Decrement();
        Token Number();
        Token Identifier();
        Token::Type IdentifierType();
        Token::Type CheckKeyword(size_t startOffset, const char* rest, Token::Type type) const;
    };
}

#endif //BYTEBAC_SCANNER_H_INCLUDED
