#pragma once

#include "TrpSchema.hpp"

class TrpSchemaNull : public TrpSchema
{
    public:
        bool validate(ITrpJsonValue* value, TrpValidatorContext& ctx) const;
        SchemaType getType() const { return SCHEMA_NULL; }
};



