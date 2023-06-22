#include "Scanner.h"

void BlueCat::Scanner::Scan(const char* source) {
    current = start = source;
}

BlueCat::Token BlueCat::Scanner::ScanToken() {
    SkipWhitespace();
    start = current;

    if (IsAtEnd()) return MakeToken(Token::Type::EndOfFile);

    char c = Advance();

    if (c == 'R' && Peek() == '"') return RawString();
    if (c == '0' && Peek() == 'b') return Byte();
    if (IsAlpha(c)) return Identifier();
    if (IsDigit(c)) return Number();

    switch (c) {
        case '(': return MakeToken(Token::Type::LeftParen);
        case ')': return MakeToken(Token::Type::RightParen);
        case '{': return MakeToken(Token::Type::LeftBrace);
        case '}': return MakeToken(Token::Type::RightBrace);
        case ';': return MakeToken(Token::Type::Semicolon);
        case '.': return MakeToken(Token::Type::Dot);
        case '-': return Decrement();
        case '+': return Increment();
        case '/': return MakeToken(Token::Type::Slash);
        case '*': return MakeToken(Token::Type::Star);
        case '%': return MakeToken(Token::Type::Remainder);
        case '~': return MakeToken(Token::Type::BitwiseNot);
        case '|': return MakeToken(Token::Type::BitwiseOr);
        case '&': return MakeToken(Token::Type::BitwiseAnd);
        case '^': return MakeToken(Match('^') ? Token::Type::Exponent : Token::Type::BitwiseXOR);
        case '!': return MakeToken(Match('=') ? Token::Type::BangEqual : Token::Type::Bang);
        case '=': return MakeToken(Match('=') ? Token::Type::EqualEqual : Token::Type::Equal);
        case '<': return MakeToken(MatchEither('<', '=', Token::Type::BitwiseShiftLeft, Token::Type::LessEqual, Token::Type::Less));
        case '>': return MakeToken(MatchEither('>', '=', Token::Type::BitwiseShiftRight, Token::Type::GreaterEqual, Token::Type::Greater));
        case '"': return String();
        default: return ErrorToken("Unexpected character.");
    }
}

char BlueCat::Scanner::Advance() {
    return *current++;
}

char BlueCat::Scanner::Peek(int8_t offset) {
    if (IsAtEnd()) return '\0';
    return *(current + offset);
}

bool BlueCat::Scanner::Match(char expected) {
    if (IsAtEnd() or Peek() != expected) return false;
    Advance(); //Eat the char only if expected
    return true;
}

BlueCat::Token::Type BlueCat::Scanner::MatchEither(char a, char b, BlueCat::Token::Type aType, BlueCat::Token::Type bType, BlueCat::Token::Type defaultType) {
    if (IsAtEnd()) return defaultType;

    Token::Type returnType = defaultType;
    if (Peek() == a) {
        returnType = aType;
    }
    else if (Peek() == b) {
        returnType = bType;
    }
    Advance();
    return returnType;
}

void BlueCat::Scanner::SkipWhitespace() {
    while (true) {
        char c = Peek();
        switch(c) {
            case ' ':
            case '\r':
            case '\t':
                Advance();
                break;
            case '\n':
                currentLine++;
                Advance();
                break;
            case '/':
                if (Peek(1) == '/') {
                    while (Peek() != '\n' && !IsAtEnd()) Advance();
                }
                else {
                    return;
                }
                break;
            default: return;
        }
    }
}

bool BlueCat::Scanner::IsAtEnd() const {
    return *current == '\0';
}

bool BlueCat::Scanner::IsDigit(char c) {
    return c >= '0' && c <= '9';
}

bool BlueCat::Scanner::IsBinaryDigit(char c) {
    return c == '0' || c == '1';
}

bool BlueCat::Scanner::IsAlpha(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

BlueCat::Token BlueCat::Scanner::MakeToken(Token::Type type) {
    Token token;
    token.type = type;
    token.start = start;
    token.length = (size_t)(current - start);
    token.line = currentLine;
    return token;
}

BlueCat::Token BlueCat::Scanner::ErrorToken(const char* message) {
    Token token;
    token.type = Token::Type::Error;
    token.start = message;
    token.length = (size_t)strlen(message);
    token.line = currentLine;
    return token;
}

BlueCat::Token BlueCat::Scanner::String() {
    while (Peek() != '"' && !IsAtEnd()) {
        Advance();
    }

    if (IsAtEnd()) return ErrorToken("Unterminated string: Strings must end with a double-quote.");

    //The closing quote
    Advance();
    return MakeToken(Token::Type::String);
}

BlueCat::Token BlueCat::Scanner::RawString() {
    Advance(); //Eat the 'R'
    Advance(); //Eat the quote

    while (!(Peek() == ')' && Peek(1) == '"') && !IsAtEnd()) {
        Advance();
    }

    if (IsAtEnd()) return ErrorToken("Unterminated string: Raw strings must end with a close-bracket followed by a double-quote.");

    Advance(); //Eat the closing bracket
    Advance(); //Eat the closing quote

    return MakeToken(Token::Type::RawString);
}

BlueCat::Token BlueCat::Scanner::Byte() {
    Advance(); //Eat the '0'
    Advance(); //Eat the 'b'

    size_t charsRead = 1;
    while (!IsAtEnd() && IsBinaryDigit(Peek())) {
        Advance();
        ++charsRead;
    }
    if (IsAtEnd()) {
        return ErrorToken("Unterminated byte: A byte declaration must specify all 8 bits in the byte.");
    }
    if (charsRead != 8) {
        return ErrorToken("Mis-sized byte: A byte must specify exactly 8 bits.");
    }
    return MakeToken(Token::Type::Byte);
}

BlueCat::Token BlueCat::Scanner::Increment() {
    if (Peek(0) == '+') {
        Advance();
        return MakeToken(Token::Type::Increment);
    }
    return MakeToken(Token::Type::Plus);
}

BlueCat::Token BlueCat::Scanner::Decrement() {
    if (Peek(0) == '-') {
        Advance();
        return MakeToken(Token::Type::Decrement);
    }
    return MakeToken(Token::Type::Minus);
}

BlueCat::Token BlueCat::Scanner::Number() {
    while (IsDigit(Peek())) Advance();
    //Look for decimal part
    if (Peek() == '.' && IsDigit(Peek(1))) {
        //Consume the '.'
        Advance();

        while (IsDigit(Peek())) Advance();
    }
    return MakeToken(Token::Type::Number);
}

BlueCat::Token BlueCat::Scanner::Identifier() {
    while (IsAlpha(Peek()) || IsDigit(Peek())) Advance();
    return MakeToken(IdentifierType());
}

BlueCat::Token::Type BlueCat::Scanner::IdentifierType() {
    switch (*start) {
        case 'a': return CheckKeyword(1, "nd", Token::And);
        case 'c': return CheckKeyword(1, "lass", Token::Class);
        case 'e': return CheckKeyword(1, "lse", Token::Else);
        case 'f': {
            if (current - start > 1) {
                switch (*(start + 1)) {
                    case 'a': return CheckKeyword(2, "lse", Token::Type::False);
                    case 'o': return CheckKeyword(2, "r", Token::Type::For);
                    case 'u': return CheckKeyword(2, "n", Token::Type::Fun);
                }
            }
            break;
        }
        case 'i': return CheckKeyword(1, "f", Token::If);
        case 'n': return CheckKeyword(1, "ull", Token::Null);
        case 'o': return CheckKeyword(1, "r", Token::Or);
        case 'p': return CheckKeyword(1, "rint", Token::Print);
        case 'r': return CheckKeyword(1, "eturn", Token::Return);
        case 's': return CheckKeyword(1, "uper", Token::Super);
        case 't': {
            if (current - start > 1) {
                switch (*(start + 1)) {
                    case 'h': return CheckKeyword(2, "is", Token::Type::This);
                    case 'r': return CheckKeyword(2, "ue", Token::Type::True);
                }
            }
            break;
        }
        case 'v': return CheckKeyword(1, "ar", Token::Var);
        case 'w': return CheckKeyword(1, "hile", Token::While);
    }
    return Token::Type::Identifier;
}

BlueCat::Token::Type BlueCat::Scanner::CheckKeyword(size_t startOffset, const char* rest, Token::Type type) const {
    size_t length = strlen(rest);
    if (current - start == startOffset + length && memcmp(start + startOffset, rest, length) == 0) {
        return type;
    }
    return Token::Type::Identifier;
}
