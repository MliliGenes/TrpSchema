#pragma once

#include "TrpSchema.hpp"

class TrpSchemaString : public TrpSchema
{
    private:
        bool has_min, has_max;
        size_t min_len, max_len;

    public:
        TrpSchemaString();

        // good for chaining
        TrpSchemaString& min( size_t _min_len );
        TrpSchemaString& max( size_t _max_len );

        bool validate(ITrpJsonValue* value, TrpValidatorContext& ctx) const;
        SchemaType getType() const { return SCHEMA_STRING; }
};



