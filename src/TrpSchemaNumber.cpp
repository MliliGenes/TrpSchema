#include "../include/TrpSchemaNumber.hpp"

TrpSchemaNumber::TrpSchemaNumber() : has_max(false), has_min(false) {}

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

bool TrpSchemaNumber::validate(ITrpJsonValue* value, TrpValidatorContext& ctx) const {
    if ( !value || value->getType() != TRP_NUMBER ) {
        ValidationError err;

        err.path = ctx.getCurrentPath();
        err.msg = "Expected number";
        err.expected = SCHEMA_NUMBER;
        err.actual = value->getType();

        ctx.pushError( err );
        return false;
    }

    TrpJsonNumber* nbr = static_cast<TrpJsonNumber*>(value);

    if ( has_max && nbr->getValue() > max_value ) {
        ValidationError err;

        err.path = ctx.getCurrentPath();
        err.msg = "Number exceeds maximum value";
        err.expected = SCHEMA_NUMBER;
        err.actual = value->getType();

        ctx.pushError( err );
        return false;
    }

    if ( has_min && nbr->getValue() < min_value ) {
        ValidationError err;

        err.path = ctx.getCurrentPath();
        err.msg = "Number is below minimum value";
        err.expected = SCHEMA_NUMBER;
        err.actual = value->getType();

        ctx.pushError( err );
        return false;
    }

    return true;
}