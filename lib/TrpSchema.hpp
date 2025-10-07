#ifndef TRPSCHEMA_CONSOLIDATED_HPP
#define TRPSCHEMA_CONSOLIDATED_HPP

#include "TrpJson.hpp"
#include <vector>
#include <string>
#include <map>
#include <sstream>
#include <set>

// ============================================================================
// Forward Declarations
// ============================================================================

class TrpSchema;
class TrpValidatorContext;

// ============================================================================
// Type Definitions and Enums
// ============================================================================

enum SchemaType
{
    SCHEMA_STRING,
    SCHEMA_NUMBER,
    SCHEMA_BOOLEAN,
    SCHEMA_OBJECT,
    SCHEMA_ARRAY,
    SCHEMA_NULL,
    SCHEMA_ANY
};

typedef SchemaType TrpSchemaType;
typedef std::vector<TrpSchema*> SchemaVec;

struct ValidationError {
    std::string path;
    std::string msg;
    SchemaType expected;
    TrpJsonType actual;
};

typedef std::vector<ValidationError> TrpValidationError;
typedef std::vector<std::string> TrpValidationPath;

// ============================================================================
// Utility Functions
// ============================================================================

std::string tokenTypeToString(TrpType type);

// ============================================================================
// TrpValidatorContext
// ============================================================================

class TrpValidatorContext {
    private:
        TrpValidationError errors;
        TrpValidationPath paths;

    public:
        TrpValidatorContext( void );

        void pushPath(const std::string _path);
        void popPath();

        void pushError(ValidationError _err);
        std::string getCurrentPath( void );

        const TrpValidationError& getErrors( void ) const ;
        bool  printErrors( void ) const;
};

// ============================================================================
// TrpSchema Base Class
// ============================================================================

class TrpSchema
{
    public:
        virtual ~TrpSchema( void ) {};
        virtual bool validate(ITrpJsonValue* value, TrpValidatorContext& ctx) const = 0;
        virtual SchemaType getType() const = 0;
};

// ============================================================================
// TrpSchemaString
// ============================================================================

class TrpSchemaString : public TrpSchema
{
    private:
        bool has_min, has_max;
        size_t min_len, max_len;

    public:
        TrpSchemaString();

        TrpSchemaString& min( size_t _min_len );
        TrpSchemaString& max( size_t _max_len );

        bool validate(ITrpJsonValue* value, TrpValidatorContext& ctx) const;
        SchemaType getType() const { return SCHEMA_STRING; }
};

// ============================================================================
// TrpSchemaNumber
// ============================================================================

class TrpSchemaNumber : public TrpSchema {
    private:
        bool has_min, has_max;
        size_t min_value, max_value;
        
    public:
        TrpSchemaNumber();

        TrpSchemaNumber& min( size_t _min_value );
        TrpSchemaNumber& max( size_t _max_value );

        bool validate(ITrpJsonValue* value, TrpValidatorContext& ctx) const;
        SchemaType getType() const { return SCHEMA_NUMBER; }
};

// ============================================================================
// TrpSchemaBool
// ============================================================================

class TrpSchemaBool : public TrpSchema {
    public:
        bool validate( ITrpJsonValue* value, TrpValidatorContext& ctx ) const;
        SchemaType getType( void ) const { return SCHEMA_BOOLEAN; }
};

// ============================================================================
// TrpSchemaNull
// ============================================================================

class TrpSchemaNull : public TrpSchema
{
    public:
        bool validate(ITrpJsonValue* value, TrpValidatorContext& ctx) const;
        SchemaType getType() const { return SCHEMA_NULL; }
};

// ============================================================================
// TrpSchemaArray
// ============================================================================

class TrpSchemaArray : public TrpSchema {
    private:
        TrpSchema* _item;
        SchemaVec _tuple;

        bool _uniq;
        bool has_max;
        bool has_min;
        size_t max_items, min_items;

    public:
        TrpSchemaArray();

        TrpSchemaArray& item( TrpSchema* _schema );
        TrpSchemaArray& tuple( SchemaVec& _schema );

        TrpSchemaArray& min(size_t min);
        TrpSchemaArray& max(size_t max);
        TrpSchemaArray& uniq(bool uniq);

        bool validate(ITrpJsonValue* value, TrpValidatorContext& ctx) const;
        SchemaType getType() const { return SCHEMA_ARRAY; }
};

// ============================================================================
// TrpSchemaObject
// ============================================================================

class TrpSchemaObject : public TrpSchema
{
    private:
        std::vector<std::string> required_entries;
        std::map<std::string, TrpSchema*> properties;
        bool has_min, has_max;
        size_t min_items, max_items;
        
    public:
        TrpSchemaObject();

        TrpSchemaObject& property( std::string key, TrpSchema* schema );
        TrpSchemaObject& required( std::string required );
        TrpSchemaObject& min( size_t min_value);
        TrpSchemaObject& max( size_t max_value);

        bool validate( ITrpJsonValue* value, TrpValidatorContext& ctx ) const;
        SchemaType getType() const { return SCHEMA_OBJECT; }
};

// ============================================================================
// TrpSchemaFactory
// ============================================================================

class TrpSchemaFactory {
    private:
        std::vector<TrpSchema*> _managedSchemas;

    public:
        ~TrpSchemaFactory();

        TrpSchemaString& string();
        TrpSchemaNumber& number(); 
        TrpSchemaBool& boolean();
        TrpSchemaObject& object();
        TrpSchemaArray& array();
        TrpSchemaNull& null();
};

#endif // TRPSCHEMA_CONSOLIDATED_HPP
