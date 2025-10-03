#include "../include/TrpSchemaString.hpp"


TrpSchemaString::TrpSchemaString( void ) : has_max(false), has_min(false) {}

TrpSchemaString& TrpSchemaString::min( size_t _min_len ) {
    if (!has_min) has_min = false;
    min_len = _min_len;
    return *this;
}

TrpSchemaString& TrpSchemaString::max( size_t _max_len ) {
    if (!has_max) has_max = false;
    max_len = _max_len;
    return *this;
}

bool TrpSchemaString::validate(ITrpJsonValue* value, TrpValidatorContext& ctx) const {
    if ( !value || value->getType() != TRP_STRING ) {
        ValidationError err;

        err.path = ctx.getCurrentPath();
        err.msg = "Expected string";
        err.expected = SCHEMA_STRING;
        err.actual = value ? value->getType() : TRP_NULL;

        ctx.pushError( err );
        return false;
    }

    TrpJsonString* str = static_cast<TrpJsonString*>(value);
    if (has_max && str->getValue().size() > max_len) {
        ValidationError err;

        std::stringstream error;
        error << "String size should be at most " << max_len << "chars, but got " << str->getValue().size();
        err.path = ctx.getCurrentPath();
        err.msg = error.str();
        err.expected = SCHEMA_STRING;
        err.actual = value->getType();

        ctx.pushError( err );
        return false;
    }

    if (has_min && str->getValue().size() < min_len) {
        ValidationError err;

        std::stringstream error;
        error << "String size should be at least " << max_len << "chars, but got " << str->getValue().size();
        err.path = ctx.getCurrentPath();
        err.msg = error.str();
        err.expected = SCHEMA_STRING;
        err.actual = value->getType();

        ctx.pushError( err );
        return false;
    }

    return true;
}