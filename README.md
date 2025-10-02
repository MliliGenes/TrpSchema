# TrpSchema

> High-Performance JSON Schema Validator with Zod-like API for C++98

TrpSchema is a comprehensive JSON schema validation library built on top of the TrpJSON parser, providing enterprise-grade performance with modern API design principles while maintaining C++98 compatibility for maximum portability.

## 🚀 Key Features

- **🔥 High Performance**: Parse once with TrpJSON, validate multiple times using AST reuse
- **🎯 Zod-like API**: Intuitive, fluent interface for schema construction
- **📍 Precise Error Reporting**: Path-based error tracking with detailed validation messages
- **⚡ Configurable Strategies**: Fail-fast or comprehensive validation modes
- **🛡️ Memory Safe**: RAII-based memory management with AutoPointer templates
- **🔧 C++98 Compatible**: Maximum portability across legacy and modern systems

## 📦 Quick Start

```cpp
#include "TrpJson.hpp"
#include "TrpSchema.hpp"

// Define a user profile schema
AutoPointer<ISchema> userSchema(
    SchemaBuilder::object()
        ->property("id", SchemaBuilder::number()->min(1))
        ->property("name", SchemaBuilder::string()->minLength(1)->maxLength(50))
        ->property("email", SchemaBuilder::string()->minLength(5))
        ->property("age", SchemaBuilder::optional(
            SchemaBuilder::number()->min(13)->max(120)))
        ->required("id")
        ->required("name")
        ->required("email")
);

// Validate JSON data
TrpJsonParser parser("user.json");
if (parser.parse()) {
    ValidationContext ctx;
    bool isValid = userSchema->validate(parser.getAST(), ctx);
    
    if (!isValid) {
        const std::vector<ValidationError>& errors = ctx.getErrors();
        // Handle validation errors...
    }
}
```

## 🏗️ Schema Types

### String Validation

```cpp
SchemaBuilder::string()
    ->minLength(1)
    ->maxLength(100)
```

### Number Validation

```cpp
SchemaBuilder::number()
    ->min(0)
    ->max(1000)
    ->positive()
```

### Object Validation

```cpp
SchemaBuilder::object()
    ->property("name", SchemaBuilder::string())
    ->property("count", SchemaBuilder::number()->min(0))
    ->required("name")
    ->additionalProperties(false)
```

### Array Validation

```cpp
SchemaBuilder::array()
    ->items(SchemaBuilder::string())
    ->minItems(1)
    ->maxItems(10)
    ->uniqueItems(true)
```

## 🎯 Advanced Features

- **Nested Validation**: Complex object and array structures
- **Optional Fields**: Flexible schema definitions with `SchemaBuilder::optional()`
- **Custom Constraints**: Extensible validation rules
- **Performance Modes**: Fail-fast vs comprehensive error collection
- **Path Tracking**: Precise error location reporting (e.g., `user.address.zipcode`)

## 🔧 Integration

TrpSchema seamlessly integrates with the TrpJSON parser, leveraging its `ITrpJsonValue` AST for optimal performance:

```cpp
// Parse once, validate many times
TrpJsonParser parser("config.json");
AutoPointer<ITrpJsonValue> ast(parser.release());

// Multiple validations on same AST
ValidationContext ctx1, ctx2;
schema1->validate(ast.get(), ctx1);
schema2->validate(ast.get(), ctx2);
```

## 📊 Performance

- **Memory Efficient**: RAII design with automatic cleanup
- **Fast Validation**: Type caching and early termination optimizations
- **Scalable**: Configurable error collection limits
- **Zero-Copy**: Direct AST validation without data transformation

## 🛠️ Build Requirements

- C++98 compatible compiler
- TrpJSON library (libtrpjson.a)
- Standard library support

## � Example Usage

### User Profile Validation

```cpp
#include "TrpJson.hpp"
#include "TrpSchema.hpp"

int main() {
    // Create user profile schema
    AutoPointer<ISchema> userSchema(
        SchemaBuilder::object()
            ->property("id", SchemaBuilder::number()->min(1))
            ->property("name", SchemaBuilder::string()->minLength(1)->maxLength(50))
            ->property("email", SchemaBuilder::string()->minLength(5))
            ->property("age", SchemaBuilder::optional(
                SchemaBuilder::number()->min(13)->max(120)))
            ->property("preferences", 
                SchemaBuilder::object()
                    ->property("theme", SchemaBuilder::string())
                    ->property("notifications", SchemaBuilder::boolean())
                    ->additionalProperties(false))
            ->required("id")
            ->required("name")
            ->required("email")
    );
    
    // Parse and validate JSON
    TrpJsonParser parser("user.json");
    if (parser.parse()) {
        ValidationContext ctx;
        AutoPointer<ITrpJsonValue> ast(parser.release());
        
        if (userSchema->validate(ast.get(), ctx)) {
            std::cout << "✅ Validation passed!" << std::endl;
        } else {
            std::cout << "❌ Validation failed:" << std::endl;
            const std::vector<ValidationError>& errors = ctx.getErrors();
            for (size_t i = 0; i < errors.size(); ++i) {
                std::cout << "  " << errors[i].path << ": " 
                          << errors[i].message << std::endl;
            }
        }
    }
    
    return 0;
}
```

### Configuration File Validation

```cpp
// Server configuration validation
AutoPointer<ISchema> configSchema(
    SchemaBuilder::object()
        ->property("server",
            SchemaBuilder::object()
                ->property("host", SchemaBuilder::string())
                ->property("port", SchemaBuilder::number()->min(1)->max(65535))
                ->property("ssl", SchemaBuilder::boolean())
                ->required("host")
                ->required("port"))
        ->property("database",
            SchemaBuilder::object()
                ->property("url", SchemaBuilder::string())
                ->property("pool_size", SchemaBuilder::number()->min(1)->max(100))
                ->required("url"))
        ->required("server")
        ->required("database")
);

// Use the schema
TrpJsonParser parser("config.json");
if (parser.parse()) {
    ValidationContext ctx;
    AutoPointer<ITrpJsonValue> ast(parser.release());
    
    if (configSchema->validate(ast.get(), ctx)) {
        std::cout << "✅ Configuration is valid!" << std::endl;
    }
}
```

## 🏛️ Architecture

TrpSchema leverages the TrpJSON parser's AST capabilities for optimal performance:

- **AST Reuse**: Parse once, validate multiple times
- **Type Safety**: Compile-time schema construction with runtime validation
- **Memory Efficiency**: RAII-based memory management with AutoPointer
- **Error Collection**: Detailed path-based error reporting
- **Fail-Fast Options**: Configurable validation strategies

## 🤝 Contributing

Contributions are welcome! This library emphasizes:

- **Performance**: Optimized for high-throughput scenarios
- **Usability**: Intuitive Zod-like API design
- **Reliability**: Comprehensive error reporting
- **Portability**: C++98 compatibility for maximum deployment flexibility

## 📄 License

[Add your license here]

## 🔗 Related Projects

- **TrpJSON**: The underlying high-performance JSON parser
- Built with C++98 compatibility for legacy system support

---

Built with ❤️ for enterprise-grade JSON validation in C++98
