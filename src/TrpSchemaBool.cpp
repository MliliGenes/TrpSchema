#include "../include/TrpSchemaBool.hpp"

bool TrpSchemaBool::validate( ITrpJsonValue* value, TrpValidatorContext& ctx ) const {
    if (!value || value->getType() != TRP_BOOL ) {
        ValidationError err;

        err.expected = SCHEMA_BOOLEAN;
        err.actual = value ? value->getType() : TRP_NULL;
        err.msg = "Expected boolean, found " + tokenTypeToString(err.actual);
        err.path = ctx.getCurrentPath();

        ctx.pushError( err );
        return false;
    }

    return true;
}