#pragma once

#ifndef TRPJSON_HPP
#define TRPJSON_HPP

// =============================================================================
// TrpJSON - Complete Header-Only JSON Parser Library
// Consolidated from include/**/*.hpp files
// C++98 Compatible - No external dependencies
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
    ITrpJsonValue() {}
    virtual ~ITrpJsonValue() {}
    virtual TrpJsonType getType() const = 0;
};

// =============================================================================
// JSON VALUE CLASSES (from values/*.hpp)
// =============================================================================

// JSON Object Class (from values/TrpJsonObject.hpp)
class TrpJsonObject : public ITrpJsonValue {
private:
    JsonObjectMap m_members;

public:
    TrpJsonObject() {}
    
    ~TrpJsonObject() {
        for (JsonObjectMap::iterator it = m_members.begin(); it != m_members.end(); ++it) {
            delete it->second;
        }
    }

    TrpJsonType getType() const {
        return TRP_OBJECT;
    }

    void add(std::string key, ITrpJsonValue* value) {
        m_members[key] = value;
    }

    ITrpJsonValue* find(std::string key) {
        JsonObjectMap::iterator it = m_members.find(key);
        return (it != m_members.end()) ? it->second : NULL;
    }

    JsonObjectMap::const_iterator begin() const {
        return m_members.begin();
    }

    JsonObjectMap::const_iterator end() const {
        return m_members.end();
    }

    size_t size() const {
        return m_members.size();
    }
};

// JSON Array Class (from values/TrpJsonArray.hpp)
class TrpJsonArray : public ITrpJsonValue {
private:
    JsonArrayVector m_elements;

public:
    TrpJsonArray() {}
    
    ~TrpJsonArray() {
        for (size_t i = 0; i < m_elements.size(); ++i) {
            delete m_elements[i];
        }
    }

    TrpJsonType getType() const {
        return TRP_ARRAY;
    }

    void add(ITrpJsonValue* value) {
        m_elements.push_back(value);
    }

    ITrpJsonValue* at(size_t index) {
        return (index < m_elements.size()) ? m_elements[index] : NULL;
    }

    size_t size() const {
        return m_elements.size();
    }
};

// JSON String Class (from values/TrpJsonString.hpp)
class TrpJsonString : public ITrpJsonValue {
private:
    std::string m_value;

public:
    explicit TrpJsonString(const std::string& value) : m_value(value) {}
    
    ~TrpJsonString() {}

    TrpJsonType getType() const {
        return TRP_STRING;
    }

    const std::string& getValue() const {
        return m_value;
    }
};

// JSON Number Class (from values/TrpJsonNumber.hpp)
class TrpJsonNumber : public ITrpJsonValue {
private:
    double m_value;

public:
    explicit TrpJsonNumber(double value) : m_value(value) {}
    
    ~TrpJsonNumber() {}

    TrpJsonType getType() const {
        return TRP_NUMBER;
    }

    double getValue() const {
        return m_value;
    }
};

// JSON Boolean Class (from values/TrpJsonBool.hpp)
class TrpJsonBool : public ITrpJsonValue {
private:
    bool m_value;

public:
    explicit TrpJsonBool(bool value) : m_value(value) {}
    
    ~TrpJsonBool() {}

    TrpJsonType getType() const {
        return TRP_BOOL;
    }

    bool getValue() const {
        return m_value;
    }
};

// JSON Null Class (from values/TrpJsonNull.hpp)
class TrpJsonNull : public ITrpJsonValue {
public:
    TrpJsonNull() {}
    
    ~TrpJsonNull() {}

    TrpJsonType getType() const {
        return TRP_NULL;
    }
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

    void skipWhitespace() {
        while (current != line_end && (*current == ' ' || *current == '\t' || *current == '\r')) {
            ++current;
            ++col;
        }
    }

    char peekChar() const {
        return (current != line_end) ? *current : '\0';
    }

    char getChar() {
        if (current != line_end) {
            char c = *current;
            ++current;
            ++col;
            return c;
        }
        return '\0';
    }

    void advanceLexer() {
        if (current != line_end) {
            ++current;
            ++col;
        }
    }

    void pushBackLexer() {
        if (current != current_line.begin()) {
            --current;
            --col;
        }
    }

    token readString() {
        token t;
        t.type = T_STRING;
        t.line = line;
        t.col = col;
        
        advanceLexer(); // Skip opening quote
        
        while (current != line_end && *current != '"') {
            if (*current == '\\') {
                advanceLexer();
                if (current == line_end) break;
                
                char escaped = *current;
                switch (escaped) {
                    case '"': t.value += '"'; break;
                    case '\\': t.value += '\\'; break;
                    case '/': t.value += '/'; break;
                    case 'b': t.value += '\b'; break;
                    case 'f': t.value += '\f'; break;
                    case 'n': t.value += '\n'; break;
                    case 'r': t.value += '\r'; break;
                    case 't': t.value += '\t'; break;
                    default: t.value += escaped; break;
                }
            } else {
                t.value += *current;
            }
            advanceLexer();
        }
        
        if (current != line_end && *current == '"') {
            advanceLexer(); // Skip closing quote
        }
        
        return t;
    }

    token readNumber() {
        token t;
        t.type = T_NUMBER;
        t.line = line;
        t.col = col;
        
        while (current != line_end && 
               ((*current >= '0' && *current <= '9') || *current == '.' || *current == '-' || *current == '+' || *current == 'e' || *current == 'E')) {
            t.value += *current;
            advanceLexer();
        }
        
        return t;
    }

    token readLiteral() {
        token t;
        t.line = line;
        t.col = col;
        
        std::string literal;
        while (current != line_end && 
               ((*current >= 'a' && *current <= 'z') || (*current >= 'A' && *current <= 'Z'))) {
            literal += *current;
            advanceLexer();
        }
        
        if (literal == "true") {
            t.type = T_TRUE;
            t.value = "true";
        } else if (literal == "false") {
            t.type = T_FALSE;
            t.value = "false";
        } else if (literal == "null") {
            t.type = T_NULL;
            t.value = "null";
        } else {
            t.type = T_ERROR;
            t.value = "Unknown literal: " + literal;
        }
        
        return t;
    }

    token createErrorToken(const std::string& message) {
        token t;
        t.type = T_ERROR;
        t.value = message;
        t.line = line;
        t.col = col;
        return t;
    }

    bool loadNextLineIfNeeded() {
        if (current == line_end) {
            if (has_next_line) {
                current_line = next_line;
                current = current_line.begin();
                line_end = current_line.end();
                ++line;
                col = 0;
                
                if (std::getline(json_file, next_line)) {
                    has_next_line = true;
                } else {
                    has_next_line = false;
                }
                return true;
            }
            return false;
        }
        return true;
    }

    bool isAtEndOfLine() const {
        return current == line_end;
    }

    bool isAtEnd() const {
        return isAtEndOfLine() && !has_next_line;
    }

public:
    explicit TrpJsonLexer(const std::string& filename) 
        : file_name(filename), has_next_line(false), line(0), col(0) {
        json_file.open(filename.c_str());
        
        if (json_file.is_open()) {
            if (std::getline(json_file, current_line)) {
                current = current_line.begin();
                line_end = current_line.end();
                
                if (std::getline(json_file, next_line)) {
                    has_next_line = true;
                }
            }
        }
    }

    ~TrpJsonLexer() {
        if (json_file.is_open()) {
            json_file.close();
        }
    }

    token getNextToken() {
        if (!loadNextLineIfNeeded()) {
            token t;
            t.type = T_END_OF_FILE;
            t.line = line;
            t.col = col;
            return t;
        }

        skipWhitespace();

        if (isAtEndOfLine()) {
            return getNextToken(); // Try next line
        }

        token t;
        t.line = line;
        t.col = col;

        char c = peekChar();
        switch (c) {
            case '{': t.type = T_BRACE_OPEN; t.value = "{"; advanceLexer(); break;
            case '}': t.type = T_BRACE_CLOSE; t.value = "}"; advanceLexer(); break;
            case '[': t.type = T_BRACKET_OPEN; t.value = "["; advanceLexer(); break;
            case ']': t.type = T_BRACKET_CLOSE; t.value = "]"; advanceLexer(); break;
            case ':': t.type = T_COLON; t.value = ":"; advanceLexer(); break;
            case ',': t.type = T_COMMA; t.value = ","; advanceLexer(); break;
            case '"': return readString();
            case 't': case 'f': case 'n': return readLiteral();
            default:
                if ((c >= '0' && c <= '9') || c == '-') {
                    return readNumber();
                } else {
                    return createErrorToken("Unexpected character");
                }
        }

        return t;
    }

    bool isOpen() const {
        return json_file.is_open();
    }

    std::string getFileName() const {
        return file_name;
    }

    void reset() {
        if (json_file.is_open()) {
            json_file.clear();
            json_file.seekg(0, std::ios::beg);
            line = 0;
            col = 0;
            
            if (std::getline(json_file, current_line)) {
                current = current_line.begin();
                line_end = current_line.end();
                
                if (std::getline(json_file, next_line)) {
                    has_next_line = true;
                } else {
                    has_next_line = false;
                }
            }
        }
    }
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

    ITrpJsonValue* parseArray(token& current_token) {
        if (current_token.type != T_BRACKET_OPEN) return NULL;

        AutoPointer<TrpJsonArray> arr_ptr(new TrpJsonArray());
        token t = lexer->getNextToken();
        
        if (t.type == T_BRACKET_CLOSE) return arr_ptr.release();

        while (true) {
            ITrpJsonValue* tmp_value = parseValue(t);
            if (!tmp_value) return NULL;
            
            arr_ptr->add(tmp_value);
            
            t = lexer->getNextToken();
            if (t.type == T_BRACKET_CLOSE) {
                break;
            } else if (t.type == T_COMMA) {
                t = lexer->getNextToken();
                continue;
            } else {
                return NULL;
            }
        }

        return arr_ptr.release();
    }

    ITrpJsonValue* parseObject(token& current_token) {
        if (current_token.type != T_BRACE_OPEN) return NULL;

        AutoPointer<TrpJsonObject> obj_ptr(new TrpJsonObject());
        token t = lexer->getNextToken();
        
        if (t.type == T_BRACE_CLOSE) return obj_ptr.release();

        while (true) {
            if (t.type != T_STRING) return NULL;
            
            std::string key = t.value;
            
            t = lexer->getNextToken();
            if (t.type != T_COLON) return NULL;
            
            t = lexer->getNextToken();
            ITrpJsonValue* tmp_value = parseValue(t);
            if (!tmp_value) return NULL;
            
            obj_ptr->add(key, tmp_value);
            
            t = lexer->getNextToken();
            if (t.type == T_BRACE_CLOSE) {
                break;
            } else if (t.type == T_COMMA) {
                t = lexer->getNextToken();
                continue;
            } else {
                return NULL;
            }
        }

        return obj_ptr.release();
    }

    ITrpJsonValue* parseString(token& current_token) {
        if (current_token.type != T_STRING) return NULL;
        return new TrpJsonString(current_token.value);
    }

    ITrpJsonValue* parseNumber(token& current_token) {
        if (current_token.type != T_NUMBER) return NULL;
        double value = std::atof(current_token.value.c_str());
        return new TrpJsonNumber(value);
    }

    ITrpJsonValue* parseLiteral(token& current_token) {
        switch (current_token.type) {
            case T_TRUE:
                return new TrpJsonBool(true);
            case T_FALSE:
                return new TrpJsonBool(false);
            case T_NULL:
                return new TrpJsonNull();
            default:
                return NULL;
        }
    }

    ITrpJsonValue* parseValue(token& current_token) {
        switch (current_token.type) {
            case T_BRACE_OPEN:
                return parseObject(current_token);
            case T_BRACKET_OPEN:
                return parseArray(current_token);
            case T_STRING:
                return parseString(current_token);
            case T_NUMBER:
                return parseNumber(current_token);
            case T_TRUE:
            case T_FALSE:
            case T_NULL:
                return parseLiteral(current_token);
            default:
                lastError(current_token);
                return NULL;
        }
    }

    // Disable copy constructor and assignment
    TrpJsonParser(const TrpJsonParser& other);
    TrpJsonParser& operator=(const TrpJsonParser& other);

public:
    TrpJsonParser() : lexer(NULL), head(NULL), parsed(false) {
        last_err.type = T_ERROR;
    }

    explicit TrpJsonParser(const std::string& file_name) : head(NULL), parsed(false) {
        lexer = new TrpJsonLexer(file_name);
        last_err.type = T_ERROR;
    }

    ~TrpJsonParser() {
        clearAST();
        delete lexer;
    }

    void resetLexer(TrpJsonLexer* new_lexer) {
        if (!new_lexer || !new_lexer->isOpen()) return;
        delete lexer;
        lexer = new_lexer;
        reset();
    }

    void setLexer(TrpJsonLexer* _lexer) {
        resetLexer(_lexer);
    }

    bool parse() {
        if (!lexer || !lexer->isOpen()) return false;
        
        clearAST();
        
        token t = lexer->getNextToken();
        head = parseValue(t);
        parsed = (head != NULL);
        
        return parsed;
    }

    ITrpJsonValue* getAST() const {
        return head;
    }

    ITrpJsonValue* release() {
        ITrpJsonValue* tmp = head;
        head = NULL;
        parsed = false;
        return tmp;
    }

    bool isParsed() const {
        return parsed;
    }

    const token& getLastError() const {
        return last_err;
    }

    void lastError(token t) {
        if (t.type != T_ERROR) t.value = "Expected token";
        last_err = t;
        
        if (lexer) {
            std::cerr << lexer->getFileName() << ":" << t.line << ":" << t.col 
                      << " Error: " << t.value << std::endl;
        }
    }

    void clearAST() {
        delete head;
        head = NULL;
        parsed = false;
    }

    void reset() {
        clearAST();
        parsed = false;
        last_err.type = T_ERROR;
        last_err.value.clear();
        last_err.col = 0;
        last_err.line = 0;
    }

    std::string astToString() const {
        if (!head) return "";
        return valueToString(head, 0);
    }

    void prettyPrint() const {
        if (head) {
            std::cout << astToString() << std::endl;
        }
    }

private:
    std::string createTabIndent(int level) const {
        return std::string(level, '\t');
    }

    std::string valueToString(ITrpJsonValue* value, int indentLevel) const {
        if (!value) return "null";

        switch (value->getType()) {
            case TRP_OBJECT:
                return objectToString(static_cast<TrpJsonObject*>(value), indentLevel);
            case TRP_ARRAY:
                return arrayToString(static_cast<TrpJsonArray*>(value), indentLevel);
            case TRP_STRING:
                return "\"" + static_cast<TrpJsonString*>(value)->getValue() + "\"";
            case TRP_NUMBER: {
                std::ostringstream oss;
                oss << static_cast<TrpJsonNumber*>(value)->getValue();
                return oss.str();
            }
            case TRP_BOOL:
                return static_cast<TrpJsonBool*>(value)->getValue() ? "true" : "false";
            case TRP_NULL:
                return "null";
            default:
                return "unknown";
        }
    }

    std::string objectToString(TrpJsonObject* obj, int indentLevel) const {
        std::ostringstream oss;
        std::string indent = createTabIndent(indentLevel);
        std::string nextIndent = createTabIndent(indentLevel + 1);

        oss << "{\n";

        JsonObjectMap::const_iterator it = obj->begin();
        for (size_t i = 0; i < obj->size(); ++i) {
            oss << nextIndent << "\"" << it->first << "\": ";
            oss << valueToString(it->second, indentLevel + 1);
            
            if (i < obj->size() - 1) {
                oss << ",";
            }
            oss << "\n";
            ++it;
        }

        oss << indent << "}";
        return oss.str();
    }

    std::string arrayToString(TrpJsonArray* arr, int indentLevel) const {
        std::ostringstream oss;
        std::string indent = createTabIndent(indentLevel);
        std::string nextIndent = createTabIndent(indentLevel + 1);

        oss << "[\n";

        for (size_t i = 0; i < arr->size(); ++i) {
            oss << nextIndent << valueToString(arr->at(i), indentLevel + 1);
            
            if (i < arr->size() - 1) {
                oss << ",";
            }
            oss << "\n";
        }

        oss << indent << "]";
        return oss.str();
    }
};

// =============================================================================
// CONVENIENCE FUNCTIONS
// =============================================================================

// Quick parse function for string input
inline ITrpJsonValue* parseJsonString(const std::string& jsonStr) {
    // Create temporary file
    std::ofstream temp("temp_json.tmp");
    temp << jsonStr;
    temp.close();
    
    // Parse from file
    TrpJsonParser parser("temp_json.tmp");
    if (parser.parse()) {
        return parser.release();
    }
    
    return NULL;
}

// Quick parse function for file input
inline ITrpJsonValue* parseJsonFile(const std::string& filename) {
    TrpJsonParser parser(filename);
    if (parser.parse()) {
        return parser.release();
    }
    
    return NULL;
}

#endif // TRPJSON_HPP