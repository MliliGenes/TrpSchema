#pragma once

#include "TrpSchema.hpp"

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
