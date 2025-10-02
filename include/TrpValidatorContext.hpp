#pragma once

#include <vector>
#include "../lib/TrpJson.hpp"

#ifndef TRPVALIDATORCONTEXT_HPP
#define TRPVALIDATORCONTEXT_HPP

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

struct ValidationError {
    std::string path;
    std::string msg;
    SchemaType expected;
    TrpJsonType actual;
};

typedef std::vector<ValidationError> TrpValidationError;
typedef std::vector<std::string> TrpValidationPath;

class TrpValidatorContext {
    private:
        TrpValidationError error;
        TrpValidationPath paths;
        size_t max_errors;

    public:
        TrpValidatorContext(size_t max_err = 100);
        ~TrpValidatorContext(void);

        void pushPath(const std::string _path);
        void popPath();

        void pushError(ValidationError _err);
};

#endif