#pragma once

#include "TrpSchema.hpp"

class TrpSchemaBool : public TrpSchema {
    public:
        bool validate( ITrpJsonValue* value, TrpValidatorContext& ctx ) const;
        TrpSchemaType getType( void ) const { return SCHEMA_OBJECT; }
};
