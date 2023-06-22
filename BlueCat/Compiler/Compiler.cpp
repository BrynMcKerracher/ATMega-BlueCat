#include "../Common.h"
#include "../ObjectString.h"
#include "Compiler.h"
#include <iostream>

BlueCat::Compiler::Compiler() {
    parseRules[Token::Type::LeftParen]            = {&Compiler::ParseGrouping,        nullptr,                Precedence::None};
    parseRules[Token::Type::RightParen]           = {nullptr,                         nullptr,                Precedence::None};
    parseRules[Token::Type::LeftBrace]            = {nullptr,                         nullptr,                Precedence::None};
    parseRules[Token::Type::RightBrace]           = {nullptr,                         nullptr,                Precedence::None};
    parseRules[Token::Type::Comma]                = {nullptr,                         nullptr,                Precedence::None};
    parseRules[Token::Type::Dot]                  = {nullptr,                         nullptr,                Precedence::None};
    parseRules[Token::Type::Minus]                = {&Compiler::ParseUnary,           &Compiler::ParseBinary, Precedence::Term};
    parseRules[Token::Type::Plus]                 = {nullptr,                         &Compiler::ParseBinary, Precedence::Term};
    parseRules[Token::Type::Remainder]            = {nullptr,                         &Compiler::ParseBinary, Precedence::Factor};
    parseRules[Token::Type::BitwiseNot]           = {&Compiler::ParseUnary,           nullptr,                Precedence::UnaryPrecedence};
    parseRules[Token::Type::BitwiseAnd]           = {nullptr,                         &Compiler::ParseBinary, Precedence::BitAnd};
    parseRules[Token::Type::BitwiseOr]            = {nullptr,                         &Compiler::ParseBinary, Precedence::BitOr};
    parseRules[Token::Type::BitwiseShiftLeft]     = {nullptr,                         &Compiler::ParseBinary, Precedence::BitAnd};
    parseRules[Token::Type::BitwiseShiftRight]    = {nullptr,                         &Compiler::ParseBinary, Precedence::BitAnd};
    parseRules[Token::Type::BitwiseXOR]           = {nullptr,                         &Compiler::ParseBinary, Precedence::BitOr};
    parseRules[Token::Type::Semicolon]            = {nullptr,                         nullptr,                Precedence::None};
    parseRules[Token::Type::Slash]                = {nullptr,                         &Compiler::ParseBinary, Precedence::Factor};
    parseRules[Token::Type::Star]                 = {nullptr,                         &Compiler::ParseBinary, Precedence::Factor};
    parseRules[Token::Type::Bang]                 = {&Compiler::ParseUnary,           nullptr,                Precedence::UnaryPrecedence};
    parseRules[Token::Type::BangEqual]            = {nullptr,                         &Compiler::ParseBinary, Precedence::None};
    parseRules[Token::Type::Equal]                = {nullptr,                         &Compiler::ParseBinary, Precedence::None};
    parseRules[Token::Type::EqualEqual]           = {nullptr,                         &Compiler::ParseBinary, Precedence::Equality};
    parseRules[Token::Type::Greater]              = {nullptr,                         &Compiler::ParseBinary, Precedence::Comparison};
    parseRules[Token::Type::GreaterEqual]         = {nullptr,                         &Compiler::ParseBinary, Precedence::Comparison};
    parseRules[Token::Type::Less]                 = {nullptr,                         &Compiler::ParseBinary, Precedence::Comparison};
    parseRules[Token::Type::LessEqual]            = {nullptr,                         &Compiler::ParseBinary, Precedence::Comparison};
    parseRules[Token::Type::Increment]            = {&Compiler::ParsePrefixIncrement, nullptr,                Precedence::Factor};
    parseRules[Token::Type::Decrement]            = {&Compiler::ParsePrefixIncrement, nullptr,                Precedence::Factor};
    parseRules[Token::Type::RawString]            = {&Compiler::ParseRawString,       nullptr,                Precedence::None};
    parseRules[Token::Type::Byte]                 = {&Compiler::ParseByte,            nullptr,                Precedence::None};
    parseRules[Token::Type::Exponent]             = {nullptr,                         &Compiler::ParseBinary, Precedence::Factor};
    parseRules[Token::Type::Identifier]           = {&Compiler::ParseVariable,        nullptr,                Precedence::None};
    parseRules[Token::Type::String]               = {&Compiler::ParseString,          nullptr,                Precedence::None};
    parseRules[Token::Type::Number]               = {&Compiler::Number,               nullptr,                Precedence::None};
    parseRules[Token::Type::And]                  = {nullptr,                         &Compiler::ParseAnd,    Precedence::And};
    parseRules[Token::Type::Class]                = {nullptr,                         nullptr,                Precedence::None};
    parseRules[Token::Type::Else]                 = {nullptr,                         nullptr,                Precedence::None};
    parseRules[Token::Type::False]                = {&Compiler::ParseLiteral,         nullptr,                Precedence::None};
    parseRules[Token::Type::For]                  = {nullptr,                         nullptr,                Precedence::None};
    parseRules[Token::Type::Fun]                  = {nullptr,                         nullptr,                Precedence::None};
    parseRules[Token::Type::If]                   = {nullptr,                         nullptr,                Precedence::None};
    parseRules[Token::Type::Null]                 = {&Compiler::ParseLiteral,         nullptr,                Precedence::None};
    parseRules[Token::Type::Or]                   = {nullptr,                         &Compiler::ParseOr,     Precedence::Or};
    parseRules[Token::Type::Print]                = {nullptr,                         nullptr,                Precedence::None};
    parseRules[Token::Type::Return]               = {nullptr,                         nullptr,                Precedence::None};
    parseRules[Token::Type::Super]                = {nullptr,                         nullptr,                Precedence::None};
    parseRules[Token::Type::This]                 = {nullptr,                         nullptr,                Precedence::None};
    parseRules[Token::Type::True]                 = {&Compiler::ParseLiteral,         nullptr,                Precedence::None};
    parseRules[Token::Type::Var]                  = {nullptr,                         nullptr,                Precedence::None};
    parseRules[Token::Type::While]                = {nullptr,                         nullptr,                Precedence::None};
    parseRules[Token::Type::Error]                = {nullptr,                         nullptr,                Precedence::None};
    parseRules[Token::Type::EndOfFile]            = {nullptr,                         nullptr,                Precedence::None};
}

bool BlueCat::Compiler::Compile(const char* source, Segment& segment) {
    parser.hadError = false;
    parser.panicMode = false;
    currentSegment = &segment;

    scanner.Scan(source);
  
    Advance();

    while (!Match(Token::Type::EndOfFile)) {
        ParseDeclaration();
    }
    
    EndCompiler();
    return !parser.hadError;
}

void BlueCat::Compiler::Advance() {
    parser.previous = parser.current;
    while (true) {
        parser.current = scanner.ScanToken();

        if (parser.current.type != Token::Type::Error) break;

        ErrorAtCurrent(parser.current.start);
    }
}

void BlueCat::Compiler::ParseExpression() {
    ParsePrecedence(Precedence::Assignment);
}

void BlueCat::Compiler::ParseDeclaration() {
    if (Match(Token::Var)) {
        ParseVarDeclaration();
    }
    else {
        ParseStatement();
    }

    if (parser.panicMode) Synchronise();
}

void BlueCat::Compiler::ParseVarDeclaration() {
    size_t globalIndex = ParseVariable("Invalid variable name: Variable names must contain only alphanumeric characters or underscores.");
    if (Match(Token::Type::Equal)) {
        ParseExpression();
    }
    else {
        EmitOp(Op::Nil);
    }
    Consume(Token::Type::Semicolon, "Missing semicolon: A semicolon ';' was expected after this variable declaration but was not found.");
    DefineVariable(globalIndex);
}

void BlueCat::Compiler::ParseStatement() {
    if (Match(Token::Type::Print)) {
        ParsePrintStatement();
    }
    else if (Match(Token::Type::For)) {
        ParseForStatement();
    }
    else if (Match(Token::Type::If)) {
        ParseIfStatement();
    }
    else if (Match(Token::Type::While)) {
        ParseWhileStatement();
    }
    else if (Match(Token::Type::LeftBrace)) { //Scope statement
        BeginScope();
        ParseBlock();
        EndScope();
    }
    else {
        ParseExpressionStatement();
    }
}

void BlueCat::Compiler::ParsePrintStatement() {
    ParseExpression();
    Consume(Token::Type::Semicolon, "Missing semicolon: A semicolon ';' was expected but was not found.");
    EmitOp(Op::Print);
}

void BlueCat::Compiler::ParseExpressionStatement() {
    ParseExpression();
    Consume(Token::Type::Semicolon, "Missing semicolon: A semicolon ';' was expected after this expression but was not found.");
    EmitOp(Op::Pop);
}

void BlueCat::Compiler::ParseForStatement() {
    BeginScope();
    Consume(Token::Type::LeftParen, "Missing bracket: Expect '(' after 'for'.");
    if (Match(Token::Type::Semicolon)) {
        //No initialiser
    }
    else if (Match(Token::Type::Var)) {
        ParseVarDeclaration();
    }
    else {
        ParseExpressionStatement();
    }

    Board::AddressOffset loopStart = CurrentSegment()->InstructionCount();
    Board::AddressOffset exitJump = -1;
    if (!Match(Token::Type::Semicolon)) {
        ParseExpression();
        Consume(Token::Type::Semicolon, "Missing semicolon: Expect an ';' after loop condition.");

        exitJump = EmitJump(Op::JumpIfFalse);
        EmitOp(Op::Pop); //Pop the condition
    }
     if (!Match(Token::Type::RightParen)) {
         Board::AddressOffset bodyJump = EmitJump(Op::Jump);
         Board::AddressOffset incrementStart = CurrentSegment()->InstructionCount();
         ParseExpression();
         EmitOp(Op::Pop);
         Consume(Token::Type::RightParen, "Expect ')' after 'for' clauses.");

         EmitLoop(loopStart);
         loopStart = incrementStart;
         PatchJump(bodyJump);
     }

    ParseStatement();
    EmitLoop(loopStart);

    if (exitJump != -1) { //We only do this if an exit clause exists
        PatchJump(exitJump);
        EmitOp(Op::Pop);
    }

    EndScope();
}

void BlueCat::Compiler::ParseIfStatement() {
    Consume(Token::Type::LeftParen, "Missing bracket: Expect '(' after 'if'.");
    ParseExpression();
    Consume(Token::Type::RightParen, "Missing bracket: Expect ')' after 'if' condition.");

    Board::AddressOffset thenJump = EmitJump(Op::JumpIfFalse);
    EmitOp(Op::Pop);
    ParseStatement();

    Board::AddressOffset elseJump = EmitJump(Op::Jump);

    PatchJump(thenJump);
    EmitOp(Op::Pop);

    if (Match(Token::Type::Else)) {
        ParseStatement();
    }
    PatchJump(elseJump);
}

void BlueCat::Compiler::ParsePrefixIncrement(bool canAssign) {
    Op incrementType;
    if (parser.previous.type == Token::Type::Increment) { //Check if we're incrementing or decrementing.
        incrementType = Op::Increment;
    }
    else {
        incrementType = Op::Decrement;
    }

    Consume(Token::Type::Identifier, "Expect a variable name to follow an increment operation.");
    Token name = parser.previous;
    Op getOp, setOp;
    int16_t varIndex = ResolveLocal(&name);
    if (varIndex != -1) {
        getOp = Op::GetLocal;
        setOp = Op::SetLocal;
    }
    else {
        varIndex = IdentifierConstant(name);
        getOp = Op::GetGlobal;
        setOp = Op::SetGlobal;
    }
    EmitOp(getOp);
    WriteAddress(varIndex);
    EmitOp(incrementType);
    EmitOp(setOp);
    WriteAddress(varIndex);
}

void BlueCat::Compiler::ParseWhileStatement() {
    Board::AddressOffset loopStart = currentSegment->InstructionCount();
    Consume(Token::Type::LeftParen, "Missing bracket: Expecting '(' after 'while'.'");
    ParseExpression();
    Consume(Token::Type::RightParen, "Missing bracket: Expecting ')' after while expression.");

    Board::AddressOffset exitJump = EmitJump(Op::JumpIfFalse);
    EmitOp(Op::Pop);
    ParseStatement();
    EmitLoop(loopStart);
    PatchJump(exitJump);
    EmitOp(Op::Pop);
}

void BlueCat::Compiler::Synchronise() {
    parser.panicMode = false;

    while (parser.current.type != Token::Type::EndOfFile) {
        if (parser.previous.type == Token::Type::Semicolon) return; //We sync when we start a new statement.
        switch (parser.current.type) {
            case Token::Type::Class:
            case Token::Type::Fun:
            case Token::Type::Var:
            case Token::Type::For:
            case Token::Type::If:
            case Token::Type::While:
            case Token::Type::Print:
            case Token::Type::Return:
                return;

            default: ; //Do nothing
        }
    }
    Advance();
}

void BlueCat::Compiler::ParseAnd(bool canAssign) {
    Board::AddressOffset endJump = EmitJump(Op::JumpIfFalse);
    EmitOp(Op::Pop);
    ParsePrecedence(Precedence::And);
    PatchJump(endJump);
}

void BlueCat::Compiler::ParseOr(bool canAssign) {
    Board::AddressOffset elseJump = EmitJump(Op::JumpIfFalse);
    Board::AddressOffset endJump = EmitJump(Op::Jump);

    PatchJump(elseJump);
    EmitOp(Op::Pop);

    ParsePrecedence(Precedence::Or);
    PatchJump(endJump);
}

void BlueCat::Compiler::ParseGrouping(bool canAssign) {
    ParseExpression();
    Consume(Token::Type::RightParen, "Unmatched bracket: A closing bracket ')' was expected after this expression but was not found.");
}

void BlueCat::Compiler::ParseUnary(bool canAssign) {
    Token::Type operatorType = parser.previous.type;

    //Compile operand
    ParsePrecedence(Precedence::UnaryPrecedence);

    //Emit the operator instruction
    switch (operatorType) {
        case Token::Type::Bang:       EmitOp(Op::Not);        break;
        case Token::Type::Minus:      EmitOp(Op::Negate);     break;
        case Token::Type::Increment:  EmitOp(Op::Increment);  break;
        case Token::Type::Decrement:  EmitOp(Op::Decrement);  break;
        case Token::Type::BitwiseNot: EmitOp(Op::BitwiseNot); break;
        default: return; //Unreachable
    }
}

void BlueCat::Compiler::ParseBinary(bool canAssign) {
    Token::Type operatorType = parser.previous.type;
    ParseRule* rule = GetRule(operatorType);
    ParsePrecedence((Precedence)(rule->precedence + 1));
    switch (operatorType) {
        case Token::Type::BangEqual: {
            EmitOp(Op::Equal);
            EmitOp(Op::Not);      
            break;
        }
        case Token::Type::EqualEqual:   EmitOp(Op::Equal);      break;
        case Token::Type::Exponent:     EmitOp(Op::Exponent);   break;
        case Token::Type::Greater:      EmitOp(Op::Greater);    break;
        case Token::Type::GreaterEqual: {
            EmitOp(Op::Less);
            EmitOp(Op::Not);
            break;
        }
        case Token::Type::Less:         EmitOp(Op::Less);       break;
        case Token::Type::LessEqual: {
            EmitOp(Op::Greater);
            EmitOp(Op::Not);
            break;
        }
        case Token::Type::Minus:             EmitOp(Op::Subtract);          break;
        case Token::Type::Plus:              EmitOp(Op::Add);               break;
        case Token::Type::Remainder:         EmitOp(Op::Remainder);         break;
        case Token::Type::Star:              EmitOp(Op::Multiply);          break;
        case Token::Type::Slash:             EmitOp(Op::Divide);            break;
        case Token::Type::BitwiseAnd:        EmitOp(Op::BitwiseAnd);        break;
        case Token::Type::BitwiseOr:         EmitOp(Op::BitwiseOr);         break;
        case Token::Type::BitwiseShiftLeft:  EmitOp(Op::BitwiseShiftLeft);  break;
        case Token::Type::BitwiseShiftRight: EmitOp(Op::BitwiseShiftRight); break;
        case Token::Type::BitwiseXOR:        EmitOp(Op::BitwiseXOR);        break;

        default: return; //Unreachable;
    }
}

void BlueCat::Compiler::ParseLiteral(bool canAssign) {
    switch (parser.previous.type) {
        case Token::Type::False: EmitOp(Op::False); break;
        case Token::Type::Null:   EmitOp(Op::Nil);   break;
        case Token::Type::True:  EmitOp(Op::True);  break;
        default: return; //Unreachable
    }
}

void BlueCat::Compiler::ParseString(bool canAssign) {
    //+1 To skip the double quote at the start of the string
    //-2 So we don't include the double quotes at the start and end of the string
    EmitConstant(Value(ObjectString::CreateTranslatedCopyOfCStr(parser.previous.start + 1, parser.previous.length - 2)));
}

void BlueCat::Compiler::ParseRawString(bool canAssign) {
    //+3 for R"( at start of string
    //-5 to account for the two double quotes, brackets, and the R
    EmitConstant(Value(ObjectString::CreateFromCopyOfCStr(parser.previous.start + 3, parser.previous.length - 5)));
}

void BlueCat::Compiler::ParseByte(bool canAssign) {
    Value::byte_t byte = 0;
    for (uint8_t i = 0; i < 8 * sizeof (Value::byte_t); ++i) {
        //+2 to skip the '0b' identifier at the start of the lexeme.
        if (*(parser.previous.start + i + 2) == '1') {
            byte |= 1 << (8 * sizeof (Value::byte_t) - 1 - i);
        }
    }
    EmitConstant(Value(byte));
}

void BlueCat::Compiler::ParseVariable(bool canAssign) {
    ParseNamedVariable(parser.previous, canAssign);
}

void BlueCat::Compiler::ParseNamedVariable(const Token& name, bool canAssign) {
    Op getOp, setOp;
    int16_t varIndex = ResolveLocal(&name);
    if (varIndex != -1) {
        getOp = Op::GetLocal;
        setOp = Op::SetLocal;
    } 
    else {
        varIndex = IdentifierConstant(name);
        getOp = Op::GetGlobal;
        setOp = Op::SetGlobal;
    }

    if (canAssign && Match(Token::Type::Equal)) {
        ParseExpression();
        EmitOp(setOp);
        WriteAddress(varIndex);
    }
    else {
        EmitOp(getOp);
        WriteAddress(varIndex);
    }
}

void BlueCat::Compiler::ParseBlock() {
    while (!Check(Token::Type::RightBrace) && !Check(Token::Type::EndOfFile)) {
        ParseDeclaration();
    }
    Consume(Token::Type::RightBrace, "Unmatched Brace: An '}' was expected but not found.");
}

BlueCat::Compiler::ParseRule* BlueCat::Compiler::GetRule(Token::Type type) {
    return &parseRules[type];
}

bool BlueCat::Compiler::Match(Token::Type type) {
    if (!Check(type)) return false;
    Advance();
    return true;
}

bool BlueCat::Compiler::Check(Token::Type type) const {
    return parser.current.type == type; 
}

void BlueCat::Compiler::Consume(Token::Type type, const char* message) {
    if (parser.current.type == type) {
        Advance();
        return;
    }
    ErrorAtCurrent(message);
}

BlueCat::Segment* BlueCat::Compiler::CurrentSegment() const {
    return currentSegment;
}

void BlueCat::Compiler::ErrorAtCurrent(const char* message) {
    ErrorAt(parser.current, message);
}

void BlueCat::Compiler::Error(const char* message) {
    ErrorAt(parser.previous, message);
}

void BlueCat::Compiler::ErrorAt(const Token& token, const char* message) {
    if (parser.panicMode) return;
    parser.panicMode = true;
    
    std::cout << "Error";
    if (token.type == BlueCat::Token::Type::EndOfFile) {
        std::cout << " at end of file!";
    }
    else if (token.type == Token::Type::Error) {
        //Nothing
    }
    else {
        char errorPoint[token.length + 1];
        strncpy(errorPoint, token.start, token.length);
        errorPoint[token.length] = '\0';

        std::cout << " at '" << errorPoint << "' on line " << token.line;
    }
    std::cout << ": " << message << "\n";
    parser.hadError = true; 
}

void BlueCat::Compiler::Number(bool canAssign) {
    EmitConstant(Value(strtod(parser.previous.start, nullptr)));
}

void BlueCat::Compiler::EndCompiler() {
    EmitOp(Op::Return);
}

void BlueCat::Compiler::BeginScope() {
    scopeDepth++;
}

void BlueCat::Compiler::EndScope() {
    scopeDepth--;
    Board::ShortAddress numPops = 0;
    while (locals.Count() > 0 && locals.PeekBack().depth > scopeDepth) {
        locals.PopNoAlloc();
        ++numPops;
    }
    if (numPops > Board::OpsPerAddress) { //Only use PopN if it creates fewer instructions
        EmitOp(Op::PopN);
        WriteShortAddress(numPops);
    }
    else {
        for (uint8_t i = 0; i < numPops; ++i) { //Otherwise we create a sequence of Pops instead
            EmitOp(Op::Pop);
        }
    }
}

void BlueCat::Compiler::EmitOp(Op op) {
    currentSegment->AddOp(op);
}

void BlueCat::Compiler::EmitLoop(BlueCat::Board::AddressOffset loopStart) {
    EmitOp(Op::Loop);
    Board::AddressOffset offset = CurrentSegment()->InstructionCount() - loopStart + Board::OpsPerAddressOffset;
    WriteAddressOffset(offset);
}

BlueCat::Board::AddressOffset BlueCat::Compiler::EmitJump(Op op) {
    EmitOp(op);
    WriteAddress(0); //Placeholder address
    return currentSegment->InstructionCount() - Board::OpsPerAddress;
}

void BlueCat::Compiler::PatchJump(Board::AddressOffset offset) {
    //-Board::OpsPerAddress to adjust for the bytecode for the jump offset itself.
    Board::AddressOffset jump = currentSegment->InstructionCount() - offset - Board::OpsPerAddress;
    WriteAddressOffset(offset, jump);
}

void BlueCat::Compiler::EmitConstant(const Value& value) {
    EmitOp(Op::Constant);
    WriteAddress(MakeConstant(value));
}

BlueCat::Board::SizeType BlueCat::Compiler::MakeConstant(const Value& value) {
    return currentSegment->AddConstant(value);
}

void BlueCat::Compiler::ParsePrecedence(Precedence precedence) {
    Advance();
    ParseFn prefixRule = GetRule(parser.previous.type)->prefix;
    if (prefixRule == nullptr) {
        Error("ParseExpression Expected: An expression was expected here but not found.");
        return;
    }

    bool canAssign = precedence <= Precedence::Assignment;
    (*this.*prefixRule)(canAssign);

    while (precedence <= GetRule(parser.current.type)->precedence) {
        Advance();
        ParseFn infixRule = GetRule(parser.previous.type)->infix;
        (*this.*infixRule)(canAssign);
    }

    if (canAssign && Match(Token::Type::Equal)) {
        Error("Invalid assignment target: Must assign to a variable!");
    }
}

BlueCat::Board::SizeType BlueCat::Compiler::ParseVariable(const char* errorMessage) {
    Consume(Token::Identifier, errorMessage);

    DeclareVariable();
    if (scopeDepth > 0) return 0;
    
    return IdentifierConstant(parser.previous);
}

void BlueCat::Compiler::MarkLocalInitialised() {
    locals.PeekBack().depth = scopeDepth;
}

BlueCat::Board::SizeType BlueCat::Compiler::IdentifierConstant(const Token& name) {
    return MakeConstant(Value(ObjectString::CreateFromCopyOfCStr(name.start, name.length)));
}

bool BlueCat::Compiler::IdentifiersEqual(const Token* a, const Token* b) {
    return a->length == b->length && memcmp(a->start, b->start, a->length) == 0;
}

int16_t BlueCat::Compiler::ResolveLocal(const Token* name) {
    for (int16_t i = locals.Count() - 1; i >= 0; --i) {
        Local* local = &locals[i];
        if (IdentifiersEqual(name, &local->name)) {
            if (local->depth == -1) {
                Error("Self-Initialised Local: Can't initialise a local variable with itself!");
            }
            return i;
        }
    }
    return -1;
}

void BlueCat::Compiler::AddLocal(const Token& name) {
    locals.Push(Local());
    Local* local = &locals.PeekBack();
    local->name = name;
    local->depth = -1;
}

void BlueCat::Compiler::DeclareVariable() {
    if (scopeDepth == 0) return;

    for (int16_t i = locals.Count() - 1; i >= 0; --i) {
        Local* local = &locals[i];
        if (local->depth != -1 && local->depth < scopeDepth) {
            break;
        }
        if (IdentifiersEqual(&parser.previous, &local->name)) {
            Error("ParseVariable Redefinition: There is already a variable with this name in scope.");
        }
    }
    AddLocal(parser.previous);
}

void BlueCat::Compiler::DefineVariable(size_t globalIndex) {
    if (scopeDepth > 0) {
        MarkLocalInitialised();
        return;
    }
  
    EmitOp(Op::DefineGlobal);
    WriteAddress(globalIndex);
}

void BlueCat::Compiler::WriteAddress(Board::Address address) {
    for (uint8_t i = 0; i < BlueCat::Board::OpsPerAddress; ++i) {
        EmitOp((Op)(address >> (8 * sizeof(Op) * i)));
    }
}

void BlueCat::Compiler::WriteShortAddress(Board::ShortAddress address) {
    for (uint8_t i = 0; i < BlueCat::Board::OpsPerShortAddress; ++i) {
        EmitOp((Op)(address >> (8 * sizeof(Op) * i)));
    }
}

void BlueCat::Compiler::WriteAddressOffset(Board::AddressOffset offset, Board::AddressOffset value) {
    for (uint8_t i = 0; i < BlueCat::Board::OpsPerAddressOffset; ++i) {
        currentSegment->instructions[offset + i] = (Op)(value >> (8 * sizeof(Op) * i));
    }
}

void BlueCat::Compiler::WriteAddressOffset(Board::AddressOffset value) {
    for (uint8_t i = 0; i < BlueCat::Board::OpsPerAddressOffset; ++i) {
        EmitOp((Op)(value >> (8 * sizeof(Op) * i)));
    }
}