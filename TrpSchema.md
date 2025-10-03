# TrpSchema - High-Performance JSON Schema Validator

A C++98-compatible JSON schema validation library built on top of TrpJSON parser, providing Zod-like API with enterprise-grade performance.

## Overview

TrpSchema is a comprehensive JSON schema validation system that leverages the TrpJSON parser's AST generation capabilities to provide fast, memory-efficient validation with detailed error reporting. The library follows modern API design principles while maintaining C++98 compatibility for maximum portability.

## Architecture

### Core Design Principles

- **AST Reuse**: Parse once with TrpJSON, validate multiple times for optimal performance
- **Type Safety**: Compile-time schema type checking with runtime validation
- **Memory Efficiency**: Stack-allocated schemas with automatic RAII cleanup
- **Fail-Fast Options**: Configurable validation strategies for different performance needs
- **Detailed Errors**: Path-based error reporting with precise location information

## 1. Schema Representation

### Base Schema Interface

```cpp
class ISchema {
public:
    virtual ~ISchema() {}
    virtual bool validate(ITrpJsonValue* value, ValidationContext& ctx) const = 0;
    virtual SchemaType getType() const = 0;
};
```

### Schema Types

```cpp
enum SchemaType {
    SCHEMA_STRING,   // String validation with constraints
    SCHEMA_NUMBER,   // Numeric validation with ranges
    SCHEMA_BOOLEAN,  // Boolean type validation
    SCHEMA_OBJECT,   // Object structure validation
    SCHEMA_ARRAY,    // Array validation with item schemas
    SCHEMA_NULL,     // Null type validation
    SCHEMA_ANY       // Any type allowed
};
```

## 2. Validation Context

The validation context manages error collection and path tracking for performance-critical validation operations.

```cpp
class ValidationContext {
private:
    std::vector<std::string> error_path;
    std::vector<ValidationError> errors;
    bool fail_fast;
    size_t max_errors;

public:
    ValidationContext(bool fast_fail = false, size_t max_err = 100);
    
    // Path management for nested validation
    void pushPath(const std::string& segment);
    void popPath();
    
    // Error collection
    void addError(const std::string& message);
    
    // Performance control
    bool shouldContinue() const;
    const std::vector<ValidationError>& getErrors() const;
    std::string getCurrentPath() const;
};
```

### Validation Error Structure

```cpp
struct ValidationError {
    std::string path;        // JSON path to error (e.g., "user.address.zipcode")
    std::string message;     // Human-readable error description
    SchemaType expected;     // Expected schema type
    TrpJsonType actual;      // Actual JSON type found
};
```

## 3. String Schema

Provides comprehensive string validation with length constraints and built-in format validation.

```cpp
class StringSchema : public ISchema {
private:
    size_t min_length;
    size_t max_length;
    bool has_min, has_max;

public:
    StringSchema();
    
    // Fluent API for constraint specification
    StringSchema& minLength(size_t len);
    StringSchema& maxLength(size_t len);
    
    // Core validation method
    bool validate(ITrpJsonValue* value, ValidationContext& ctx) const;
    SchemaType getType() const { return SCHEMA_STRING; }
};
```

### Usage Example

```cpp
// Product name validation
StringSchema nameSchema = StringSchema()
    .minLength(1)
    .maxLength(100);

// Description validation
StringSchema descriptionSchema = StringSchema()
    .minLength(10)
    .maxLength(500);
```

## 4. Number Schema

Handles numeric validation with range constraints and boundary conditions.

```cpp
class NumberSchema : public ISchema {
private:
    double minimum, maximum;
    bool has_min, has_max;
    bool exclusive_min, exclusive_max;

public:
    NumberSchema();
    
    // Range validation methods
    NumberSchema& min(double val, bool exclusive = false);
    NumberSchema& max(double val, bool exclusive = false);
    
    // Convenience methods
    NumberSchema& positive() { return min(0, true); }
    NumberSchema& nonNegative() { return min(0, false); }
    
    bool validate(ITrpJsonValue* value, ValidationContext& ctx) const;
    SchemaType getType() const { return SCHEMA_NUMBER; }
};
```

### Usage Example

```cpp
// Age validation (0-120, inclusive)
NumberSchema ageSchema = NumberSchema()
    .min(0).max(120);

// Port number validation (1-65535)
NumberSchema portSchema = NumberSchema()
    .min(1).max(65535);
```

## 5. Object Schema

Comprehensive object validation with property schemas, required fields, and additional properties control.

```cpp
class ObjectSchema : public ISchema {
private:
    typedef std::map<std::string, ISchema*> PropertyMap;
    PropertyMap properties;
    std::vector<std::string> required_props;
    bool additional_properties_allowed;
    ISchema* additional_properties_schema;
    size_t min_properties, max_properties;
    bool has_min_props, has_max_props;

public:
    ObjectSchema();
    ~ObjectSchema();
    
    // Property definition
    ObjectSchema& property(const std::string& name, ISchema* schema);
    ObjectSchema& required(const std::string& name);
    
    // Additional properties control
    ObjectSchema& additionalProperties(bool allowed);
    ObjectSchema& additionalProperties(ISchema* schema);
    
    // Size constraints
    ObjectSchema& minProperties(size_t min);
    ObjectSchema& maxProperties(size_t max);
    
    bool validate(ITrpJsonValue* value, ValidationContext& ctx) const;
    SchemaType getType() const { return SCHEMA_OBJECT; }
};
```

### Validation Logic

```cpp
bool ObjectSchema::validate(ITrpJsonValue* value, ValidationContext& ctx) const {
    // Type checking
    if (value->getType() != TRP_OBJECT) {
        ctx.addError("Expected object, got " + typeToString(value->getType()));
        return false;
    }
    
    TrpJsonObject* obj = static_cast<TrpJsonObject*>(value);
    
    // Size validation
    if (has_min_props && obj->size() < min_properties) {
        ctx.addError("Object has too few properties");
        return false;
    }
    
    // Property validation
    bool valid = true;
    PropertyMap::const_iterator it;
    for (it = properties.begin(); it != properties.end(); ++it) {
        ctx.pushPath(it->first);
        
        ITrpJsonValue* prop = obj->find(it->first);
        if (prop) {
            if (!it->second->validate(prop, ctx)) {
                valid = false;
                if (!ctx.shouldContinue()) break;
            }
        }
        
        ctx.popPath();
    }
    
    // Required field validation
    for (size_t i = 0; i < required_props.size(); ++i) {
        if (!obj->find(required_props[i])) {
            ctx.addError("Required property '" + required_props[i] + "' is missing");
            valid = false;
            if (!ctx.shouldContinue()) break;
        }
    }
    
    return valid;
}
```

## 6. Array Schema

Provides flexible array validation with item schemas, tuple validation, and size constraints.

```cpp
class ArraySchema : public ISchema {
private:
    ISchema* item_schema;        // Schema for all items
    std::vector<ISchema*> tuple_schemas;  // Tuple validation schemas
    size_t min_items, max_items;
    bool unique_items;
    bool has_min, has_max;

public:
    ArraySchema();
    ~ArraySchema();
    
    // Item validation
    ArraySchema& items(ISchema* schema);
    ArraySchema& tuple(const std::vector<ISchema*>& schemas);
    
    // Size constraints
    ArraySchema& minItems(size_t min);
    ArraySchema& maxItems(size_t max);
    
    // Uniqueness constraint
    ArraySchema& uniqueItems(bool unique = true);
    
    bool validate(ITrpJsonValue* value, ValidationContext& ctx) const;
    SchemaType getType() const { return SCHEMA_ARRAY; }
};
```

### Array Validation Strategies

```cpp
bool ArraySchema::validate(ITrpJsonValue* value, ValidationContext& ctx) const {
    if (value->getType() != TRP_ARRAY) {
        ctx.addError("Expected array");
        return false;
    }
    
    TrpJsonArray* arr = static_cast<TrpJsonArray*>(value);
    
    // Size validation
    if (has_min && arr->size() < min_items) {
        ctx.addError("Array too short");
        return false;
    }
    
    bool valid = true;
    
    // Item validation strategies
    if (!tuple_schemas.empty()) {
        // Tuple validation - each position has specific schema
        for (size_t i = 0; i < tuple_schemas.size() && i < arr->size(); ++i) {
            ctx.pushPath("[" + toString(i) + "]");
            if (!tuple_schemas[i]->validate(arr->at(i), ctx)) {
                valid = false;
                if (!ctx.shouldContinue()) break;
            }
            ctx.popPath();
        }
    } else if (item_schema) {
        // Uniform validation - all items use same schema
        for (size_t i = 0; i < arr->size(); ++i) {
            ctx.pushPath("[" + toString(i) + "]");
            if (!item_schema->validate(arr->at(i), ctx)) {
                valid = false;
                if (!ctx.shouldContinue()) break;
            }
            ctx.popPath();
        }
    }
    
    return valid;
}
```

## Schema Builder (Zod-like API)

The Schema Builder provides a fluent, intuitive API for constructing complex validation schemas.

```cpp
class SchemaBuilder {
public:
    // Primary type factories
    static StringSchema string();
    static NumberSchema number(); 
    static BooleanSchema boolean();
    static ObjectSchema object();
    static ArraySchema array();
    static NullSchema null();
    static AnySchema any();
    
    // Utility schema constructors
    static ISchema* optional(ISchema* schema);
    static ISchema* oneOf(const std::vector<ISchema*>& schemas);
    static ISchema* allOf(const std::vector<ISchema*>& schemas);
};
```

### Implementation

```cpp
StringSchema SchemaBuilder::string() {
    return StringSchema();
}

NumberSchema SchemaBuilder::number() {
    return NumberSchema();
}

ObjectSchema SchemaBuilder::object() {
    return ObjectSchema();
}

// Optional wrapper schema (stack allocated)
template<typename T>
class OptionalSchema : public ISchema {
private:
    T inner_schema;
    
public:
    OptionalSchema(const T& schema) : inner_schema(schema) {}
    
    bool validate(ITrpJsonValue* value, ValidationContext& ctx) const {
        // Allow null/undefined values
        if (value->getType() == TRP_NULL) {
            return true;
        }
        return inner_schema.validate(value, ctx);
    }
    
    SchemaType getType() const { return inner_schema.getType(); }
};

template<typename T>
OptionalSchema<T> SchemaBuilder::optional(const T& schema) {
    return OptionalSchema<T>(schema);
}
```

## Complete Usage Examples

### User Profile Schema

```cpp
// User profile validation with basic constraints (stack allocated)
ObjectSchema profileSchema = SchemaBuilder::object()
    .property("id", SchemaBuilder::number().min(1))
    .property("name", 
        SchemaBuilder::string()
            .minLength(1)
            .maxLength(50))
    .property("contact",
        SchemaBuilder::string()
            .minLength(5)
            .maxLength(100))
    .property("age", 
        SchemaBuilder::optional(
            SchemaBuilder::number().min(13).max(120)))
    .property("preferences",
        SchemaBuilder::object()
            .property("theme", 
                SchemaBuilder::string())
            .property("notifications", 
                SchemaBuilder::boolean())
            .additionalProperties(false))
    .property("tags",
        SchemaBuilder::array()
            .items(SchemaBuilder::string())
            .maxItems(10)
            .uniqueItems(true))
    .required("id")
    .required("name")
    .required("contact")
    .additionalProperties(false);
```

### Configuration Schema

```cpp
// Server configuration validation (stack allocated)
ObjectSchema configSchema = SchemaBuilder::object()
    .property("server",
        SchemaBuilder::object()
            .property("host", SchemaBuilder::string())
            .property("port", SchemaBuilder::number().min(1).max(65535))
            .property("ssl", SchemaBuilder::boolean())
            .required("host")
            .required("port"))
    .property("database",
        SchemaBuilder::object()
            .property("url", SchemaBuilder::string())
            .property("pool_size", SchemaBuilder::number().min(1).max(100))
            .property("timeout", SchemaBuilder::number().min(1000))
            .required("url"))
    .property("routes",
        SchemaBuilder::array()
            .items(SchemaBuilder::object()
                .property("path", SchemaBuilder::string())
                    ->property("methods", SchemaBuilder::array()
                        ->items(SchemaBuilder::string()))
                    ->property("auth", SchemaBuilder::boolean())
                    ->required("path")))
        ->required("server")
        ->required("database")
);
```

### E-commerce Product Schema

```cpp
// Product data validation (stack allocated)
ObjectSchema productSchema = SchemaBuilder::object()
    .property("id", SchemaBuilder::string())
    .property("name", 
        SchemaBuilder::string().minLength(1).maxLength(200))
    .property("price", 
        SchemaBuilder::number().min(0, true))  // Exclusive minimum (> 0)
    .property("category",
        SchemaBuilder::string())
    .property("tags",
        SchemaBuilder::array()
            .items(SchemaBuilder::string())
            .uniqueItems(true))
    .property("variants",
        SchemaBuilder::array()
            .items(SchemaBuilder::object()
                .property("sku", SchemaBuilder::string())
                .property("size", SchemaBuilder::string())
                .property("color", SchemaBuilder::string())
                .property("price", SchemaBuilder::number().min(0, true))
                .required("sku")
                .required("price")))
    .property("availability",
        SchemaBuilder::object()
            .property("in_stock", SchemaBuilder::boolean())
            .property("quantity", SchemaBuilder::number().min(0))
            .required("in_stock"))
    .required("id")
    .required("name")
    .required("price")
    .required("category");
```

## Performance Features

### Memory Management

- **RAII Design**: Stack-allocated schemas with automatic cleanup
- **Memory Efficiency**: No heap allocation needed, pure stack-based objects
- **Pool Allocation**: Reusable validation contexts for high-throughput scenarios

### Validation Strategies

- **Fail-Fast Mode**: Stop validation on first error for maximum speed
- **Complete Mode**: Collect all validation errors for comprehensive reporting
- **Selective Mode**: Validate only specified properties for partial validation

### Optimization Techniques

- **Type Caching**: Pre-computed type checks for repeated validations
- **Path Optimization**: Efficient string building for nested error paths
- **Early Termination**: Skip expensive validations when possible

## Integration with TrpJSON

```cpp
#include "TrpJson.hpp"  // Your JSON parser
#include "TrpSchema.hpp" // Schema validator

// Complete validation workflow
bool validateJsonFile(const std::string& filename, const ISchema& schema) {
    // Parse with TrpJSON
    TrpJsonParser parser(filename);
    if (!parser.parse()) {
        std::cerr << "JSON parsing failed" << std::endl;
        return false;
    }
    
    // Validate with TrpSchema
    ValidationContext ctx(false, 100);  // Collect up to 100 errors
    AutoPointer<ITrpJsonValue> ast(parser.release());
    
    bool valid = schema.validate(ast.get(), ctx);
    
    if (!valid) {
        const std::vector<ValidationError>& errors = ctx.getErrors();
        for (size_t i = 0; i < errors.size(); ++i) {
            std::cerr << "Validation error at " << errors[i].path 
                      << ": " << errors[i].message << std::endl;
        }
    }
    
    return valid;
}
```

## Next Steps

This architecture provides the foundation for implementing a complete JSON schema validation system. The design emphasizes:

- **Performance**: Optimized for high-throughput validation scenarios
- **Usability**: Intuitive Zod-like API for schema construction
- **Reliability**: Comprehensive error reporting and robust validation logic
- **Portability**: C++98 compatibility for maximum deployment flexibility

The system is ready for extension with additional schema types, validation rules, and performance optimizations based on specific use case requirements.
