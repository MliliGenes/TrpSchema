#pragma once

#include "TrpSchema.hpp"

class TrpSchemaObject : public TrpSchema
{
    private:
        std::vector<std::string> required_entries;
        std::vector<std::string> optional_entries;
        std::map<std::string, TrpSchema*> properties;
    public:
        TrpSchemaObject& property( std::string key, TrpSchema* schema );
        TrpSchemaObject& required( std::string required );

        bool validate( ITrpJsonValue* value, TrpValidatorContext& ctx ) const;
        SchemaType getType() const { return SCHEMA_OBJECT; }
};
