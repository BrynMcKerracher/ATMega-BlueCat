#ifndef BYTEBAC_COMPILER_H_INCLUDED
#define BYTEBAC_COMPILER_H_INCLUDED

#include "../Common.h"
#include "../Token.h"
#include "../Parser.h"
#include "../Segment.h"
#include "../Scanner.h"
#include "../Object.h"
#include "../ObjectString.h"
#include "../Local.h"
#include "../Memory.h"
#include "../Stack.h"

namespace BlueCat {
    class Compiler;
    typedef void (Compiler::*ParseFn)(bool canAssign);
  
    class Compiler {
    public:
        Compiler();
        bool Compile(const char* source, Segment& segment);
        Segment* CurrentSegment() const;

    protected:
        enum Precedence {
            None,
            Assignment,
            Or,
            And,
            BitOr,
            BitAnd,
            Equality,
            Comparison,
            Term,
            Factor,
            UnaryPrecedence,
            Call,
            Primary
        };

        struct ParseRule {
            ParseFn prefix;
            ParseFn infix;
            Precedence precedence;
        };
    
        Parser parser;
        Scanner scanner;
        Segment* currentSegment = nullptr;
        Stack<Local> locals;
        Board::SizeType scopeDepth = 0;

        ParseRule parseRules[Token::Type::NumberOfTokens];

        void Advance();
        void ParseExpression();
        void ParseDeclaration();
        void ParseVarDeclaration();
        void ParseStatement();
        void ParsePrintStatement();
        void ParseExpressionStatement();
        void ParseForStatement();
        void ParseIfStatement();
        void ParsePrefixIncrement(bool canAssign);
        void ParseWhileStatement();
        void ParseAnd(bool canAssign);
        void ParseOr(bool canAssign);
        void ParseGrouping(bool canAssign);
        void ParseUnary(bool canAssign);
        void ParseBinary(bool canAssign);
        void ParseLiteral(bool canAssign);
        void ParseString(bool canAssign);
        void ParseRawString(bool canAssign);
        void ParseByte(bool canAssign);
        void ParseVariable(bool canAssign);
        void ParseNamedVariable(const Token& name, bool canAssign);
        void ParseBlock();
        ParseRule* GetRule(Token::Type type);

        bool Match(Token::Type type);
        bool Check(Token::Type type) const;
        
        void Consume(Token::Type type, const char* message);
        void ErrorAtCurrent(const char* message);
        void Error(const char* message);
        void ErrorAt(const Token& token, const char* message);
        void EndCompiler();
        void BeginScope();
        void EndScope();
        void Synchronise();

        void Number(bool canAssign);

        void EmitOp(Op op);
        void EmitLoop(Board::AddressOffset loopStart);
        Board::AddressOffset EmitJump(Op op);
        void PatchJump(Board::AddressOffset offset);
        void EmitConstant(const Value& value);
        Board::SizeType MakeConstant(const Value& value);

        void ParsePrecedence(Precedence precedence);
        Board::SizeType ParseVariable(const char* errorMessage);
        void MarkLocalInitialised();
        Board::SizeType IdentifierConstant(const Token& name);
        static bool IdentifiersEqual(const Token* a, const Token* b) ;
        int16_t ResolveLocal(const Token* name);
        void AddLocal(const Token& name);
        void DeclareVariable();
        void DefineVariable(size_t globalIndex);
        void WriteAddress(Board::Address address);
        void WriteShortAddress(Board::ShortAddress address);
        void WriteAddressOffset(Board::AddressOffset offset, Board::AddressOffset value);
        void WriteAddressOffset(Board::AddressOffset value);
    };
}

#endif //BYTEBAC_COMPILER_H_INCLUDED
