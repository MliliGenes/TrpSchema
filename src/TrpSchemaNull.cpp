#include "../include/TrpSchemaNull.hpp"

bool TrpSchemaNull::validate( ITrpJsonValue* value, TrpValidatorContext& ctx ) const {
    if (!value || value->getType() != TRP_NULL ) {
        ValidationError err;

        err.expected = SCHEMA_NULL;
        err.actual = value ? value->getType() : TRP_NULL;
        err.msg = "Expected null, found " + tokenTypeToString(err.actual);
        err.path = ctx.getCurrentPath();

        ctx.pushError( err );
        return false;
    }

    return true;
}