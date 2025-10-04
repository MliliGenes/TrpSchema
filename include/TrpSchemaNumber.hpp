#pragma once

#include "TrpSchema.hpp"

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

