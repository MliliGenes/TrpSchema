#include "../include/TrpSchemaNumber.hpp"

TrpSchemaNumber::TrpSchemaNumber() : has_min(false), has_max(false) {}

TrpSchemaNumber& TrpSchemaNumber::min( size_t _min_value ) {
    if ( !has_min ) has_min = true;

    min_value = _min_value;
    return *this;
}

TrpSchemaNumber& TrpSchemaNumber::max( size_t _max_value ) {
    if ( !has_max ) has_max = true;

    max_value = _max_value;
    return *this;
}

static std::string intToString( int nbr ) {
    std::stringstream oss;
    oss << nbr;

    return oss.str();
}

bool TrpSchemaNumber::validate(ITrpJsonValue* value, TrpValidatorContext& ctx) const {
    bool got_error = false;

    if ( !value || value->getType() != TRP_NUMBER ) {
        ValidationError err;

        err.expected = SCHEMA_NUMBER;
        err.actual = value ? value->getType() : TRP_ERROR;
        err.path = ctx.getCurrentPath();
        err.msg = "Expected number, found " + tokenTypeToString(err.actual);

        ctx.pushError( err );
        return false;
    }

    TrpJsonNumber* nbr = static_cast<TrpJsonNumber*>(value);

    if ( has_max && nbr->getValue() > max_value ) {
        ValidationError err;

        err.path = ctx.getCurrentPath();
        err.msg = "Number exceeds maximum value of " + intToString(max_value);
        err.expected = SCHEMA_NUMBER;
        err.actual = nbr->getType();

        ctx.pushError( err );
        if ( !got_error ) got_error = true;
    }

    if ( has_min && nbr->getValue() < min_value ) {
        ValidationError err;

        err.path = ctx.getCurrentPath();
        err.msg = "Number is below minimum value of " + intToString(min_value);
        err.expected = SCHEMA_NUMBER;
        err.actual = nbr->getType();

        ctx.pushError( err );
        if ( !got_error ) got_error = true;
    }

    if ( got_error ) return false;
    return true;
}