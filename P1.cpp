
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cctype>
#include <cstdlib>

using namespace std;

enum TokenType {
    IDENTIFIER,
    NUMBER,
    OPERATOR,
    KEYWORD,
    SEPARATOR,
    EOFtk
};


struct Token {
    TokenType group;
    string instance;
    int lineNumber;
};

string sourceCode;
int position = 0;
int currentLine = 1;

string tokenTypeToString(TokenType type) {
    switch (type) {
        case IDENTIFIER: 
            return "Identifier";
        case NUMBER:     
            return "Number";
        case OPERATOR:   
            return "Operator";
        case KEYWORD:    
            return "Keyword";
        case SEPARATOR:  
            return "Separator";
        case EOFtk:      
            return "EOFtk";
        default:         
            return "Unknown";
    }
}

void lexicalError(string message) {
    cout << "LEXICAL ERROR: " << message << endl;
    exit(1);
}

bool isKeyword(const string& word) {
    vector<string> keywords = {"if", "else", "while", "return", "int", "float", "void"};

    for (const string& keyword : keywords) {
        if (word == keyword)
            return true;
    }

    return false;
}

void initializeScanner(const string& filename) {
    ifstream inputFile(filename);

    if (!inputFile.is_open())
        lexicalError("Unable to open input file: " + filename);

    sourceCode.assign(istreambuf_iterator<char>(inputFile), istreambuf_iterator<char>());

    position = 0;
    currentLine = 1;
    inputFile.close();
}

Token scanner() {

    while (position < sourceCode.length() && isspace(static_cast<unsigned char>(sourceCode[position]))) {
        if (sourceCode[position] == '\n')
            currentLine++;

        position++;
    }

    if (position >= sourceCode.length())
        return {EOFtk, "", currentLine};

    int tokenLine = currentLine;
    char currentChar = sourceCode[position];

    if (isalpha(static_cast<unsigned char>(currentChar)) || currentChar == '_') {
        string word = "";

        while (position < sourceCode.length() && (isalnum(static_cast<unsigned char>(sourceCode[position])) || sourceCode[position] == '_')) {
            word += sourceCode[position];
            position++;
        }

        if (isKeyword(word))
            return {KEYWORD, word, tokenLine};

        return {IDENTIFIER, word, tokenLine};
    }

    if (isdigit(static_cast<unsigned char>(currentChar))) {
        string number = "";

        while (position < sourceCode.length() && isdigit(static_cast<unsigned char>(sourceCode[position]))) {
            number += sourceCode[position];
            position++;
        }

        return {NUMBER, number, tokenLine};
    }


    if (string("+-*/%=<>!").find(currentChar) != string::npos) {
        string op(1, currentChar);
        position++;
        return {OPERATOR, op, tokenLine};
    }

    if (string("(){}[],;").find(currentChar) != string::npos) {
        string separator(1, currentChar);
        position++;
        return {SEPARATOR, separator, tokenLine};
    }

    lexicalError("Invalid character '" + string(1, currentChar) + "' at line " + to_string(currentLine));
    return {EOFtk, "", currentLine};
}

void testScanner() {
    Token token = scanner();

    while (true) {

        cout << token.lineNumber << " " << tokenTypeToString(token.group);

        if (!token.instance.empty())
            cout << " " << token.instance;

        cout << endl;

        if (token.group == EOFtk)
            break;

        token = scanner();
    }
}


int main(int argc, char* argv[]) {
    if (argc != 2)
        lexicalError("Invalid invocation. Usage: P1 [file]");

    initializeScanner(argv[1]);
    testScanner();

    return 0;
}
