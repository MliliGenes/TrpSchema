# TrpSchema

A JSON schema validation library for C++98 with a fluent, chainable API.

## Overview

TrpSchema is a JSON schema validation library built on top of [TrpJSON](https://github.com/MliliGenes/TrpJSON), providing runtime validation of JSON data against defined schemas. The library features a fluent API for schema construction and detailed path-based error reporting.

## Features

- **C++98 Compatible**: Works with legacy compilers and systems
- **Fluent API**: Chainable method calls for intuitive schema construction
- **Path-Based Error Reporting**: Precise error locations with JSON path tracking
- **Comprehensive Validation**: Support for all JSON types with constraints
- **Factory Pattern**: Clean schema creation through TrpSchemaFactory
- **Static Library**: Can be compiled as a static library for easy integration
- **Memory Management**: Automatic cleanup of factory-allocated schemas

## Prerequisites

TrpSchema requires the TrpJSON library to be installed on your system:

- **TrpJSON Repository**: [https://github.com/MliliGenes/TrpJSON](https://github.com/MliliGenes/TrpJSON)
- Follow the TrpJSON installation instructions to build and install `libtrpjson.a` before building TrpSchema

## Architecture

The library follows a modular design with schema types and validation context:

### Core Components

- **TrpSchema**: Base interface for all schema types
- **TrpSchemaFactory**: Factory class for creating schema instances
- **TrpValidatorContext**: Context for collecting validation errors with path tracking
- **ValidationError**: Structure containing error details (path, message, expected/actual types)

### Schema Types

- **TrpSchemaObject**: Validates JSON objects with property schemas and constraints
- **TrpSchemaArray**: Validates JSON arrays with item schemas and constraints
- **TrpSchemaString**: Validates JSON strings with length constraints
- **TrpSchemaNumber**: Validates JSON numbers with min/max constraints
- **TrpSchemaBool**: Validates JSON boolean values
- **TrpSchemaNull**: Validates JSON null values

## Quick Start

```cpp
#include "TrpSchema.hpp"

int main(int argc, char** argv) {
    if (argc != 2) return 1;
    
    // Parse JSON file
    TrpJsonParser parser(argv[1]);
    if (!parser.parse()) {
        std::cerr << "Failed to parse JSON file." << std::endl;
        return 1;
    }
    
    // Create schema using factory
    TrpSchemaFactory factory;
    
    TrpSchemaObject& rootSchema = factory.object()
        .property("arr", &factory.array()
            .uniq(true)
            .item(&factory.number()
                .min(5)
                .max(10)
            )
        );
    
    // Validate
    TrpValidatorContext ctx;
    if (!rootSchema.validate(parser.getAST(), ctx)) {
        std::cerr << "Validation Errors:" << std::endl;
        ctx.printErrors();
        return 1;
    }
    
    std::cout << "Validation successful!" << std::endl;
    return 0;
}
```

## API Reference

### TrpSchemaFactory

Factory for creating schema instances. The factory manages memory for all created schemas.

#### Methods
```cpp
TrpSchemaObject& object();    // Create object schema
TrpSchemaArray& array();      // Create array schema
TrpSchemaString& string();    // Create string schema
TrpSchemaNumber& number();    // Create number schema
TrpSchemaBool& boolean();     // Create boolean schema
TrpSchemaNull& null();        // Create null schema
```

**Note**: The factory's destructor automatically cleans up all allocated schemas.

### TrpSchemaString

Validates JSON string values with length constraints.

#### Methods
```cpp
TrpSchemaString& min(size_t min_len);    // Set minimum length
TrpSchemaString& max(size_t max_len);    // Set maximum length
bool validate(ITrpJsonValue* value, TrpValidatorContext& ctx) const;
```

#### Example
```cpp
factory.string()
    .min(1)
    .max(100)
```

### TrpSchemaNumber

Validates JSON number values with min/max constraints.

#### Methods
```cpp
TrpSchemaNumber& min(size_t min_value);    // Set minimum value
TrpSchemaNumber& max(size_t max_value);    // Set maximum value
bool validate(ITrpJsonValue* value, TrpValidatorContext& ctx) const;
```

#### Example
```cpp
factory.number()
    .min(0)
    .max(1000)
```

### TrpSchemaObject

Validates JSON object values with property schemas and constraints.

#### Methods
```cpp
TrpSchemaObject& property(std::string key, TrpSchema* schema);  // Define property
TrpSchemaObject& required(std::string key);                     // Mark as required
TrpSchemaObject& min(size_t min_items);                         // Min properties
TrpSchemaObject& max(size_t max_items);                         // Max properties
bool validate(ITrpJsonValue* value, TrpValidatorContext& ctx) const;
```

#### Example
```cpp
factory.object()
    .property("name", &factory.string().min(1))
    .property("age", &factory.number().min(0).max(150))
    .required("name")
    .min(1)
    .max(10)
```

### TrpSchemaArray

Validates JSON array values with item schemas and constraints.

#### Methods
```cpp
TrpSchemaArray& item(TrpSchema* schema);      // Schema for all items
TrpSchemaArray& tuple(SchemaVec& schemas);    // Fixed-length tuple validation
TrpSchemaArray& min(size_t min_items);        // Minimum array length
TrpSchemaArray& max(size_t max_items);        // Maximum array length
TrpSchemaArray& uniq(bool unique);            // Require unique items
bool validate(ITrpJsonValue* value, TrpValidatorContext& ctx) const;
```

#### Example
```cpp
factory.array()
    .item(&factory.string())
    .min(1)
    .max(10)
    .uniq(true)
```

### TrpSchemaBool

Validates JSON boolean values.

#### Methods
```cpp
bool validate(ITrpJsonValue* value, TrpValidatorContext& ctx) const;
```

### TrpSchemaNull

Validates JSON null values.

#### Methods
```cpp
bool validate(ITrpJsonValue* value, TrpValidatorContext& ctx) const;
```

### TrpValidatorContext

Context for collecting and reporting validation errors.

#### Methods
```cpp
void pushPath(const std::string path);           // Add to current path
void popPath();                                  // Remove from current path
void pushError(ValidationError err);             // Add validation error
std::string getCurrentPath();                    // Get current path
const TrpValidationError& getErrors() const;     // Get all errors
bool printErrors() const;                        // Print errors to stderr
```

### ValidationError

Structure containing error details.

```cpp
struct ValidationError {
    std::string path;           // JSON path (e.g., ".arr[0]")
    std::string msg;            // Error message
    SchemaType expected;        // Expected schema type
    TrpJsonType actual;         // Actual JSON type
};
```

## Schema Type Mapping

| Schema Type    | Enum Value      | Validates       |
|----------------|-----------------|-----------------|
| String         | SCHEMA_STRING   | TRP_STRING      |
| Number         | SCHEMA_NUMBER   | TRP_NUMBER      |
| Boolean        | SCHEMA_BOOLEAN  | TRP_BOOL        |
| Object         | SCHEMA_OBJECT   | TRP_OBJECT      |
| Array          | SCHEMA_ARRAY    | TRP_ARRAY       |
| Null           | SCHEMA_NULL     | TRP_NULL        |

## Usage Examples

### Basic String Validation

```cpp
TrpSchemaFactory factory;

TrpSchemaObject& schema = factory.object()
    .property("name", &factory.string().min(3).max(50));

TrpValidatorContext ctx;
if (!schema.validate(jsonData, ctx)) {
    ctx.printErrors();
}
```

### Nested Object Validation

```cpp
TrpSchemaFactory factory;

TrpSchemaObject& schema = factory.object()
    .property("user", &factory.object()
        .property("name", &factory.string().min(1))
        .property("age", &factory.number().min(0).max(150))
        .required("name")
    );

TrpValidatorContext ctx;
if (!schema.validate(jsonData, ctx)) {
    ctx.printErrors();
}
```

### Array with Item Validation

```cpp
TrpSchemaFactory factory;

TrpSchemaObject& schema = factory.object()
    .property("tags", &factory.array()
        .item(&factory.string().min(1))
        .min(1)
        .max(10)
        .uniq(true)
    );

TrpValidatorContext ctx;
if (!schema.validate(jsonData, ctx)) {
    ctx.printErrors();
}
```

### Tuple Validation (Fixed-Length Array)

```cpp
TrpSchemaFactory factory;

SchemaVec tupleSchemas;
tupleSchemas.push_back(&factory.string());
tupleSchemas.push_back(&factory.number());
tupleSchemas.push_back(&factory.boolean());

TrpSchemaObject& schema = factory.object()
    .property("tuple", &factory.array().tuple(tupleSchemas));

TrpValidatorContext ctx;
if (!schema.validate(jsonData, ctx)) {
    ctx.printErrors();
}
```

## Building

### Build Static Library

```bash
make lib
```

This creates `lib/libtrpschema.a` static library.

### Build Main Executable

```bash
make
```

### Clean Build Artifacts

```bash
make clean      # Remove object files
make fclean     # Remove all build artifacts including library
make re         # Clean rebuild
make lib-re     # Rebuild library only
```

## Installation

### System-Wide Installation

```bash
sudo make install
```

This installs:
- `libtrpschema.a` → `/usr/local/lib/`
- `TrpSchema.hpp` → `/usr/local/include/`
- `TrpJson.hpp` → `/usr/local/include/`

### Uninstall

```bash
sudo make uninstall
```

### Using the Library

After installation, link against TrpSchema in your projects:

```bash
# If installed system-wide
g++ -std=c++98 -o myapp myapp.cpp -ltrpschema -ltrpjson

# If using locally
g++ -std=c++98 -Iinclude -Ilib -o myapp myapp.cpp -Llib -ltrpschema -ltrpjson
```

## Complete Example

### User Profile Validation

```cpp
#include "TrpSchema.hpp"
#include <iostream>

int main() {
    // Parse JSON file
    TrpJsonParser parser("user.json");
    if (!parser.parse()) {
        std::cerr << "Failed to parse JSON" << std::endl;
        return 1;
    }
    
    // Create user profile schema using factory
    TrpSchemaFactory factory;
    
    TrpSchemaObject& userSchema = factory.object()
        .property("id", &factory.number().min(1))
        .property("name", &factory.string().min(1).max(50))
        .property("email", &factory.string().min(5))
        .property("age", &factory.number().min(13).max(120))
        .property("preferences", &factory.object()
            .property("theme", &factory.string())
            .property("notifications", &factory.boolean())
        )
        .required("id")
        .required("name")
        .required("email");
    
    // Validate
    TrpValidatorContext ctx;
    if (userSchema.validate(parser.getAST(), ctx)) {
        std::cout << "Validation passed!" << std::endl;
    } else {
        std::cout << "Validation failed:" << std::endl;
        ctx.printErrors();
    }
    
    return 0;
}
```

### Configuration File Validation

```cpp
#include "TrpSchema.hpp"
#include <iostream>

int main() {
    TrpJsonParser parser("config.json");
    if (!parser.parse()) {
        std::cerr << "Failed to parse JSON" << std::endl;
        return 1;
    }
    
    // Create configuration schema
    TrpSchemaFactory factory;
    
    TrpSchemaObject& configSchema = factory.object()
        .property("server", &factory.object()
            .property("host", &factory.string())
            .property("port", &factory.number().min(1).max(65535))
            .property("ssl", &factory.boolean())
            .required("host")
            .required("port")
        )
        .property("database", &factory.object()
            .property("url", &factory.string())
            .property("pool_size", &factory.number().min(1).max(100))
            .required("url")
        )
        .required("server")
        .required("database");
    
    // Validate
    TrpValidatorContext ctx;
    if (configSchema.validate(parser.getAST(), ctx)) {
        std::cout << "Configuration is valid!" << std::endl;
    } else {
        std::cout << "Configuration errors:" << std::endl;
        ctx.printErrors();
    }
    
    return 0;
}
```

## Error Handling

The library provides detailed error information with JSON paths:

```cpp
TrpValidatorContext ctx;
if (!schema.validate(jsonData, ctx)) {
    const TrpValidationError& errors = ctx.getErrors();
    for (size_t i = 0; i < errors.size(); ++i) {
        std::cout << errors[i].path << ": " 
                  << errors[i].msg << std::endl;
    }
}
```

Example error output:
```
.arr[0]: Number is below minimum value of 5
.arr[1]: Number is below minimum value of 5
.user.email: Required property 'email' is missing
.tags: Duplicate item found in array, Items must be unique
```

## Memory Management

The library uses RAII principles through the factory pattern:

- All schemas are allocated by `TrpSchemaFactory`
- The factory's destructor automatically cleans up all created schemas
- Use references to schemas (e.g., `TrpSchemaObject&`) returned by factory methods
- Never manually delete schema objects created by the factory

## Requirements

- C++98 compatible compiler
- TrpJSON library (libtrpjson.a)
- Standard C++ library (iostream, string, vector, map, sstream)
- Make (for building)

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

## Related Projects

- **TrpJSON**: The underlying JSON parser - [https://github.com/MliliGenes/TrpJSON](https://github.com/MliliGenes/TrpJSON)

---

Built for JSON validation in C++98
