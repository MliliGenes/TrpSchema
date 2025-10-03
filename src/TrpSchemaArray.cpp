#include "../include/TrpSchemaArray.hpp"


TrpSchemaArray::TrpSchemaArray( void ) : has_max(false), has_min(false), _items(NULL) {}

TrpSchemaArray& TrpSchemaArray::min(size_t min) {
    if (!has_min) has_min = true;
    min_items = min;
    return *this;
}

TrpSchemaArray& TrpSchemaArray::max(size_t max) {
    if (!has_max) has_max = true;
    max_items = max;
    return *this;
}

TrpSchemaArray& TrpSchemaArray::items( TrpSchema* _schema ) {
    if (!_schema) return *this;
    _items = _schema;
    return *this;
}

TrpSchemaArray& TrpSchemaArray::uniq(bool uniq) {
    _uniq = uniq;
    return *this;
}

TrpSchemaArray& TrpSchemaArray::tuple( SchemaVec _schema_vec ) {
    if (_schema_vec.empty()) return *this;
    _tuple = _schema_vec;
    return *this;
}

std::string intToString( int nbr ) {
    std::stringstream oss;
    oss << nbr;

    return oss.str();
}

bool TrpSchemaArray::validate(ITrpJsonValue* value, TrpValidatorContext& ctx) const {
    if ( !value || value->getType() != TRP_ARRAY ) {
        ValidationError err;

        err.path = ctx.getCurrentPath();
        err.msg = "Expected array";
        err.expected = SCHEMA_ARRAY;
        err.actual = value ? value->getType() : TRP_NULL;

        ctx.pushError( err );
        return false;
    }

    TrpJsonArray* arr = static_cast<TrpJsonArray*>(value);
    if ( has_max && arr->size() > max_items ) {
        ValidationError err;

        err.path = ctx.getCurrentPath();
        std::stringstream ss;
        ss << "Array must contain at least " << min_items << " items, but got " << arr->size();
        err.msg = ss.str();
        err.expected = SCHEMA_ARRAY;
        err.actual = TRP_ARRAY;

        ctx.pushError(err);
        return false;
    }

    if ( has_min && arr->size() < min_items ) {
        ValidationError err;

        err.path = ctx.getCurrentPath();
        std::stringstream ss;
        ss << "Array must contain at most " << max_items << " items, but got " << arr->size();
        err.msg = ss.str();
        err.expected = SCHEMA_ARRAY;
        err.actual = TRP_ARRAY;

        ctx.pushError(err);
        return false;
    }

    if ( _items ) {
        for ( int i = 0; i < arr->size(); i++ ) {
            ctx.pushPath("[" + intToString(i) + "]");
            if ( !_items->validate( arr->at(i), ctx ) ) {
                ctx.popPath();
                return false;
            }
            ctx.popPath();
        }
    }

    if ( !_tuple.empty() ) {
        for ( int i = 0; i < arr->size(); i++ ) {
            ctx.pushPath("[" + intToString(i) + "]");
            if ( !_tuple[i]->validate(arr->at(i), ctx) ) {
                ctx.popPath();
                return false;
            }
            ctx.popPath();
        }
    }

    if ( _uniq ) {
        
    }

    return true;
}