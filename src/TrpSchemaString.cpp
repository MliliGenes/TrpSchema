#include "../include/TrpSchemaString.hpp"


TrpSchemaString::TrpSchemaString( void ) : has_min(false), has_max(false) {}

TrpSchemaString& TrpSchemaString::min( size_t _min_len ) {
    has_min = true;
    min_len = _min_len;
    return *this;
}

TrpSchemaString& TrpSchemaString::max( size_t _max_len ) {
    has_max = true;
    max_len = _max_len;
    return *this;
}

bool TrpSchemaString::validate(ITrpJsonValue* value, TrpValidatorContext& ctx) const {
    bool got_error = false;

    if ( !value || value->getType() != TRP_STRING ) {
        ValidationError err;

        err.expected = SCHEMA_STRING;
        err.actual = value ? value->getType() : TRP_NULL;
        err.path = ctx.getCurrentPath();
        err.msg = "Expected string, found " + tokenTypeToString(err.actual);

        ctx.pushError( err );
        return false;
    }

    TrpJsonString* str = static_cast<TrpJsonString*>(value);
    
    if (has_max && str->getValue().size() > max_len) {
        ValidationError err;

        std::stringstream error;
        error << "String size should be at most " << max_len << " chars, but got " << str->getValue().size();
        err.path = ctx.getCurrentPath();
        err.msg = error.str();
        err.expected = SCHEMA_STRING;
        err.actual = value->getType();

        ctx.pushError( err );
        if ( !got_error ) got_error = true;
    }

    if (has_min && str->getValue().size() < min_len) {
        ValidationError err;

        std::stringstream error;
        error << "String size should be at least " << max_len << " chars, but got " << str->getValue().size();
        err.path = ctx.getCurrentPath();
        err.msg = error.str();
        err.expected = SCHEMA_STRING;
        err.actual = value->getType();

        ctx.pushError( err );
        if ( !got_error ) got_error = true;
    }

    if (got_error) return false;
    return true;
}