#ifndef TRPJSON_HPP
#define TRPJSON_HPP

// =============================================================================
// ANSI COLOR CODES FOR TERMINAL OUTPUT
// =============================================================================

#define RESET          "\033[0m"
#define STRING_COLOR   "\033[31m"      // Red for strings
#define NUMBER_COLOR   "\033[33m"      // Yellow for numbers  
#define BOOL_COLOR     "\033[32m"      // Green for true/false
#define NULL_COLOR     "\033[35m"      // Magenta for null
#define KEY_COLOR      "\033[94m"      // Bright blue for keys
#define BRACE_COLOR    "\033[36m"      // Cyan for {} []
#define PUNCT_COLOR    "\033[37m"      // White for punctuation

// =============================================================================
// TrpJSON - Header File for Static Library Linking
// Consolidated declarations from include/**/*.hpp files
// C++98 Compatible - Link with libtrpjson.a
// =============================================================================

// Standard Library Includes
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <string>
#include <cstddef>
#include <sstream>
#include <cstdlib>

// =============================================================================
// CORE TYPE DEFINITIONS (from core/TrpJsonType.hpp)
// =============================================================================

enum TrpType {
    TRP_NULL,
    TRP_BOOL,
    TRP_NUMBER,
    TRP_STRING,
    TRP_ARRAY,
    TRP_OBJECT,
    TRP_ERROR
};

typedef enum TrpType TrpJsonType;

// =============================================================================
// TOKEN TYPE DEFINITIONS (from core/TrpJsonLexer.hpp)
// =============================================================================

enum TrpTokenType {
    T_BRACE_OPEN,      // {
    T_BRACE_CLOSE,     // }
    T_BRACKET_OPEN,    // [
    T_BRACKET_CLOSE,   // ]
    T_COLON,           // :
    T_COMMA,           // ,
    T_STRING,          // "string"
    T_NUMBER,          // 123, 123.45
    T_TRUE,            // true
    T_FALSE,           // false
    T_NULL,            // null
    T_END_OF_FILE,     // EOF
    T_ERROR            // Error token
};

// Token structure
struct token {
    TrpTokenType type;
    std::string value;
    size_t line;  // 0-based line number
    size_t col;   // 0-based column number
};

// Type definitions for containers
typedef std::string::iterator stringIterator;
typedef std::map<std::string, class ITrpJsonValue*> JsonObjectMap;
typedef std::pair<std::string, class ITrpJsonValue*> JsonObjectEntry;
typedef std::vector<class ITrpJsonValue*> JsonArrayVector;

// =============================================================================
// AUTOPOINTER TEMPLATE (from core/TrpAutoPointer.hpp)
// =============================================================================

template <typename T>
class AutoPointer {
private:
    T* ptr;

    // Disable copy constructor and copy assignment for C++98 safety
    AutoPointer(const AutoPointer& _other);
    AutoPointer& operator=(const AutoPointer& _other);

public:
    // Constructor
    explicit AutoPointer(T* _ptr = NULL) : ptr(_ptr) {}

    // Destructor - automatic cleanup
    ~AutoPointer() {
        delete ptr;
    }

    // Check if pointer is null
    bool isNULL() const {
        return (ptr == NULL);
    }

    // Release ownership and return pointer
    T* release() {
        if (!isNULL()) {
            T* tmp = ptr;
            ptr = NULL;
            return tmp;
        }
        return NULL;
    }

    // Get raw pointer without transferring ownership
    T* get() const {
        return ptr;
    }

    // Reset pointer with new value
    void reset(T* _ptr) {
        if (!isNULL()) {
            delete ptr;
        }
        ptr = _ptr;
    }

    // Dereference operator
    T& operator*() const {
        return *ptr;
    }

    // Arrow operator
    T* operator->() const {
        return ptr;
    }
};

// =============================================================================
// BASE JSON VALUE INTERFACE (from core/TrpJsonValue.hpp)
// =============================================================================

class ITrpJsonValue {
public:
    ITrpJsonValue();
    virtual ~ITrpJsonValue();
    virtual TrpJsonType getType() const = 0;
};

// =============================================================================
// JSON VALUE CLASSES (from values/*.hpp)
// =============================================================================

// JSON Object Class
class TrpJsonObject : public ITrpJsonValue {
private:
    JsonObjectMap m_members;

public:
    TrpJsonObject();
    ~TrpJsonObject();
    TrpJsonType getType() const;
    void add(std::string key, ITrpJsonValue* value);
    ITrpJsonValue* find(std::string key);
    JsonObjectMap::const_iterator begin() const;
    JsonObjectMap::const_iterator end() const;
    size_t size() const;
};

// JSON Array Class
class TrpJsonArray : public ITrpJsonValue {
private:
    JsonArrayVector m_elements;

public:
    TrpJsonArray();
    ~TrpJsonArray();
    TrpJsonType getType() const;
    void add(ITrpJsonValue* value);
    ITrpJsonValue* at(size_t index);
    size_t size() const;
};

// JSON String Class
class TrpJsonString : public ITrpJsonValue {
private:
    std::string m_value;

public:
    explicit TrpJsonString(const std::string& value);
    ~TrpJsonString();
    TrpJsonType getType() const;
    const std::string& getValue() const;
};

// JSON Number Class
class TrpJsonNumber : public ITrpJsonValue {
private:
    double m_value;

public:
    explicit TrpJsonNumber(double value);
    ~TrpJsonNumber();
    TrpJsonType getType() const;
    double getValue() const;
};

// JSON Boolean Class
class TrpJsonBool : public ITrpJsonValue {
private:
    bool m_value;

public:
    explicit TrpJsonBool(bool value);
    ~TrpJsonBool();
    TrpJsonType getType() const;
    bool getValue() const;
};

// JSON Null Class
class TrpJsonNull : public ITrpJsonValue {
public:
    TrpJsonNull();
    ~TrpJsonNull();
    TrpJsonType getType() const;
};

// =============================================================================
// LEXER CLASS (from core/TrpJsonLexer.hpp)
// =============================================================================

class TrpJsonLexer {
private:
    std::ifstream json_file;
    std::string file_name;
    bool has_next_line;
    std::string current_line;
    std::string next_line;
    size_t line;
    size_t col;
    stringIterator current;
    stringIterator line_end;

    void skipWhitespace();
    char peekChar() const;
    char getChar();
    void advanceLexer();
    void pushBackLexer();
    token readString();
    token readNumber();
    token readLiteral();
    token createErrorToken(const std::string& message);
    bool loadNextLineIfNeeded();
    bool isAtEndOfLine() const;
    bool isAtEnd() const;

public:
    TrpJsonLexer(std::string file_name);
    ~TrpJsonLexer();
    token getNextToken();
    bool isOpen() const;
    const std::string getFileName() const;
    void reset();
};

// =============================================================================
// PARSER CLASS (from parser/TrpJsonParser.hpp)
// =============================================================================

class TrpJsonParser {
private:
    TrpJsonLexer* lexer;
    ITrpJsonValue* head;
    bool parsed;
    token last_err;

    ITrpJsonValue* parseArray(token& current_token);
    ITrpJsonValue* parseObject(token& current_token);
    ITrpJsonValue* parseString(token& current_token);
    ITrpJsonValue* parseNumber(token& current_token);
    ITrpJsonValue* parseLiteral(token& current_token);
    ITrpJsonValue* parseValue(token& current_token);

    // Disable copy constructor and assignment
    TrpJsonParser(const TrpJsonParser& other);
    TrpJsonParser& operator=(const TrpJsonParser& other);

public:
    TrpJsonParser();
    TrpJsonParser(const std::string _file_name);
    ~TrpJsonParser();
    
    void resetLexer(TrpJsonLexer* new_lexer);
    void setLexer(TrpJsonLexer* _lexer);
    bool parse();
    ITrpJsonValue* getAST() const;
    ITrpJsonValue* release();
    bool isParsed() const;
    const token& getLastError() const;
    void lastError(token t);
    void clearAST();
    void reset();
    std::string astToString() const;
    void prettyPrint() const;
};

#endif // TRPJSON_HPP

