#pragma once

#include "TrpSchema.hpp"
#include <sstream>
#include <string>

typedef std::vector<TrpSchema*> SchemaVec;

class TrpSchemaArray : public TrpSchema {
    private:
        TrpSchema* _items;
        SchemaVec _tuple;

        bool _uniq;
        bool has_max, has_min;
        size_t max_items, min_items;

    public:
        TrpSchemaArray();

        TrpSchemaArray& items( TrpSchema* _schema );
        TrpSchemaArray& tuple( SchemaVec _schema );

        TrpSchemaArray& min(size_t min);
        TrpSchemaArray& max(size_t max);
        TrpSchemaArray& uniq(bool uniq);

        bool validate(ITrpJsonValue* value, TrpValidatorContext& ctx) const;
        SchemaType getType() const { return SCHEMA_ARRAY; }
};
