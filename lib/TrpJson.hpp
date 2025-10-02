#pragma once

// TrpJSON - Comprehensive Header with All Declarations
// This file contains all macros, enums, classes, templates and signatures
// from the include/**/*.hpp files consolidated into a single header

#ifndef TRPJSON_ALL_HPP
#define TRPJSON_ALL_HPP

// ============================================================================
// STANDARD LIBRARY INCLUDES
// ============================================================================
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <string>
#include <cstddef>
#include <stdexcept>
#include <cstdlib>

// ============================================================================
// MACROS AND DEFINES - Color codes for terminal output
// ============================================================================
#define RESET          "\033[0m"
#define STRING_COLOR   "\033[31m"      // Red for strings
#define NUMBER_COLOR   "\033[33m"      // Yellow for numbers  
#define BOOL_COLOR     "\033[32m"      // Green for true/false
#define NULL_COLOR     "\033[35m"      // Magenta for null
#define KEY_COLOR      "\033[94m"      // Bright blue for keys
#define BRACE_COLOR    "\033[36m"      // Cyan for {} []
#define PUNCT_COLOR    "\033[37m"      // White for punctuation

// Header guards from original files
#ifndef TRP_TOKEN_TYPE
#define TRP_TOKEN_TYPE
#endif

#ifndef TRPVALUE_HPP
#define TRPVALUE_HPP
#endif

#ifndef TRPJSONLEXER_HPP
#define TRPJSONLEXER_HPP
#endif

#ifndef AUTOPOINTER_HPP
#define AUTOPOINTER_HPP
#endif

#ifndef TRPJSONPARSER_HPP
#define TRPJSONPARSER_HPP
#endif

#ifndef TRPOBJECT_HPP
#define TRPOBJECT_HPP
#endif

#ifndef TRPJSONARRAY_HPP
#define TRPJSONARRAY_HPP
#endif

#ifndef TRPJSONSTRING_HPP
#define TRPJSONSTRING_HPP
#endif

#ifndef TRPJSONNUMBER_HPP
#define TRPJSONNUMBER_HPP
#endif

#ifndef TRPJSONBOOL_HPP
#define TRPJSONBOOL_HPP
#endif

#ifndef TRPJSONNULL_HPP
#define TRPJSONNULL_HPP
#endif

// ============================================================================
// ENUMERATIONS
// ============================================================================

// JSON value types enumeration
enum TrpType {
    TRP_NULL,
    TRP_BOOL,
    TRP_NUMBER,
    TRP_STRING,
    TRP_ARRAY,
    TRP_OBJECT
};

// Token types for lexer
enum TrpTokenType {
    T_BRACE_OPEN,
    T_BRACE_CLOSE,
    T_BRACKET_OPEN,
    T_BRACKET_CLOSE,
    T_COLON,
    T_COMMA,
    T_STRING,
    T_NUMBER,
    T_TRUE,
    T_FALSE,
    T_NULL,
    T_END_OF_FILE,
    T_ERROR
};

// ============================================================================
// TYPE DEFINITIONS
// ============================================================================
typedef enum TrpType TrpJsonType;
typedef std::string::iterator stringIterator;
typedef std::map<std::string, class ITrpJsonValue*> JsonObjectMap;
typedef std::pair<std::string, class ITrpJsonValue*> JsonObjectEntry;
typedef std::vector<class ITrpJsonValue*> JsonArrayVector;

// ============================================================================
// STRUCTURES
// ============================================================================

// Token structure for lexer
struct token {
    TrpTokenType type;
    std::string value;
    size_t line; // 0-based line number
    size_t col;  // 0-based column number
};

// ============================================================================
// FORWARD DECLARATIONS
// ============================================================================
class ITrpJsonValue;
class TrpJsonObject;
class TrpJsonArray;
class TrpJsonString;
class TrpJsonNumber;
class TrpJsonBool;
class TrpJsonNull;
class TrpJsonLexer;
class TrpJsonParser;

// ============================================================================
// TEMPLATE CLASSES
// ============================================================================

// RAII smart pointer template for C++98 compatibility
template <typename T>
class AutoPointer {
    private:
        T* ptr;

        // Disable copy constructor and copy assignment
        AutoPointer(const AutoPointer& _other);
        AutoPointer& operator=(const AutoPointer& _other);

    public:
        // Constructor
        AutoPointer(T* _ptr = NULL) : ptr(_ptr) {
            // RAII initialization
        }

        // Destructor - automatic cleanup
        ~AutoPointer(void) {
            delete ptr;
        }

        // Check if pointer is null
        bool isNULL(void) const {
            return (ptr == NULL);
        }

        // Release ownership and return pointer
        T* release(void) {
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
                ptr = _ptr;
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

// ============================================================================
// BASE INTERFACE CLASS
// ============================================================================

// Base interface for all JSON value types
class ITrpJsonValue {
    public:
        ITrpJsonValue(void) {}
        virtual ~ITrpJsonValue(void) = 0;
        virtual TrpJsonType getType(void) const = 0;
};

// ============================================================================
// LEXER CLASS DECLARATION
// ============================================================================

class TrpJsonLexer {
    private:
        // File data
        std::ifstream json_file;
        std::string file_name;

        // Line data
        bool has_next_line;
        std::string current_line;
        std::string next_line;

        // Coordinates for debugging and errors
        size_t line;
        size_t col;

        // Iterator for current position
        stringIterator current;
        stringIterator line_end;

        // Private methods
        void skipWhitespace();
        char peekChar() const;
        char getChar();
        void advanceLexer();
        void pushBackLexer();
        token readString();
        token readNumber();
        token readLiteral();
        token createErrorToken(const std::string &message);
        bool loadNextLineIfNeeded();
        bool isAtEndOfLine() const;
        bool isAtEnd() const;

    public:
        // Constructor
        TrpJsonLexer(std::string file_name);
        
        // Destructor
        ~TrpJsonLexer(void);

        // Public interface
        token getNextToken(void);
        bool isOpen(void);
        const std::string getFileName(void) const;
        void reset(void);
};

// ============================================================================
// VALUE TYPE CLASS DECLARATIONS
// ============================================================================

// JSON Object class
class TrpJsonObject : public ITrpJsonValue {
    private:
        JsonObjectMap m_members;

    public:
        TrpJsonObject(void);
        ~TrpJsonObject(void);
        TrpJsonType getType(void) const;
        void add(std::string key, ITrpJsonValue* value);
        ITrpJsonValue* find(std::string key);
        
        // Iterator support for serialization
        JsonObjectMap::const_iterator begin() const;
        JsonObjectMap::const_iterator end() const;
        size_t size() const;
};

// JSON Array class
class TrpJsonArray : public ITrpJsonValue {
    private:
        JsonArrayVector m_elements;

    public:
        TrpJsonArray(void);
        ~TrpJsonArray(void);
        TrpJsonType getType(void) const;
        void add(ITrpJsonValue* value);
        ITrpJsonValue* at(size_t index);
        size_t size(void) const;
};

// JSON String class
class TrpJsonString : public ITrpJsonValue {
    private:
        std::string m_value;
    
    public:
        TrpJsonString(std::string value) : m_value(value) {}
        ~TrpJsonString(void);
        TrpJsonType getType(void) const;
        const std::string& getValue(void) const;
};

// JSON Number class
class TrpJsonNumber : public ITrpJsonValue {
    private:
        double m_value;
    
    public:
        TrpJsonNumber(double value) : m_value(value) {}
        ~TrpJsonNumber(void);
        TrpJsonType getType(void) const;
        const double& getValue(void) const;
};

// JSON Boolean class
class TrpJsonBool : public ITrpJsonValue {
    private:
        bool m_value;
    
    public:
        TrpJsonBool(bool value) : m_value(value) {}
        ~TrpJsonBool(void);
        TrpJsonType getType(void) const;
        const bool& getValue(void) const;
};

// JSON Null class
class TrpJsonNull : public ITrpJsonValue {
    public:
        TrpJsonType getType(void) const;
};

// ============================================================================
// PARSER CLASS DECLARATION
// ============================================================================

class TrpJsonParser {
    private:
        TrpJsonLexer* lexer;
        ITrpJsonValue* head;
        bool parsed;
        token last_err;

        // Private parsing methods
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
        // Constructors
        TrpJsonParser(void);
        TrpJsonParser(const std::string _file_name);
        
        // Destructor
        ~TrpJsonParser(void);

        // Lexer management
        void resetLexer(TrpJsonLexer* new_lexer);
        void setLexer(TrpJsonLexer* _lexer);

        // Parsing operations
        bool parse(void);
        ITrpJsonValue* getAST(void) const;
        ITrpJsonValue* release(void);

        // State management
        bool isParsed(void) const;
        const token& getLastError(void) const;
        void lastError(void) const;
        void clearAST(void);
        void reset(void);

        // Output operations
        std::string astToString(void) const;
        void prettyPrint() const;
};

// ============================================================================
// GRAMMAR REFERENCE (from original lexer file)
// ============================================================================
// JSON Grammar:
// value  → object | array | string | number | true | false | null
// object → '{' (string ':' value (',' string ':' value)*)? '}'
// array  → '[' (value (',' value)*)? ']'

#endif // TRPJSON_ALL_HPP