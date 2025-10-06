#include "../include/TrpSchemaArray.hpp"


TrpSchemaArray::TrpSchemaArray( void ) : _item(NULL), _uniq(false),
    has_max(false), has_min(false) {}

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

TrpSchemaArray& TrpSchemaArray::item( TrpSchema* _schema ) {
    if (!_schema) return *this;
    _item = _schema;
    return *this;
}

TrpSchemaArray& TrpSchemaArray::uniq(bool uniq) {
    _uniq = uniq;
    return *this;
}

TrpSchemaArray& TrpSchemaArray::tuple( SchemaVec& _schema_vec ) {
    if (_schema_vec.empty()) return *this;
    _tuple = _schema_vec;
    return *this;
}

static std::string intToString( int nbr ) {
    std::stringstream oss;
    oss << nbr;

    return oss.str();
}

bool TrpSchemaArray::validate(ITrpJsonValue* value, TrpValidatorContext& ctx) const {
    bool got_error = false;

    if ( !value || value->getType() != TRP_ARRAY ) {
        ValidationError err;

        err.expected = SCHEMA_ARRAY;
        err.actual = value ? value->getType() : TRP_NULL;
        err.msg = "Expected array, found " + tokenTypeToString(err.actual);
        err.path = ctx.getCurrentPath();

        ctx.pushError( err );
        return false;
    }

    TrpJsonArray* arr = static_cast<TrpJsonArray*>(value);
    if ( has_max && arr->size() > max_items ) {
        ValidationError err;

        err.path = ctx.getCurrentPath();
        std::stringstream ss;
        ss << "Array must contain at most " << max_items << " items, but got " << arr->size();
        err.msg = ss.str();
        err.expected = SCHEMA_ARRAY;
        err.actual = TRP_ARRAY;

        ctx.pushError(err);
        if ( !got_error ) got_error = true;
    }

    if ( has_min && arr->size() < min_items ) {
        ValidationError err;

        err.path = ctx.getCurrentPath();
        std::stringstream ss;
        ss << "Array must contain at least " << min_items << " items, but got " << arr->size();
        err.msg = ss.str();
        err.expected = SCHEMA_ARRAY;
        err.actual = TRP_ARRAY;

        ctx.pushError(err);
        if ( !got_error ) got_error = true;
    }

    if ( _item ) {
        for ( size_t i = 0; i < arr->size(); i++ ) {
            ctx.pushPath("[" + intToString(i) + "]");
            if ( !_item->validate( arr->at(i), ctx ) ) {
                if ( !got_error ) got_error = true;
            }
            ctx.popPath();
        }
    }

    if ( !_tuple.empty() ) {
        for ( size_t i = 0; i < _tuple.size() && i < arr->size(); i++ ) {
            ctx.pushPath("[" + intToString(i) + "]");
            if ( !_tuple[i]->validate(arr->at(i), ctx) ) {
                if ( !got_error ) got_error = true;
            }
            ctx.popPath();
        }
    }

    if ( _uniq ) {
        std::set<double> nbr_bucket;
        std::set<std::string> str_bucket;
        std::set<bool> bool_bucket;
        bool null_found = false;

        for ( size_t i = 0; i < arr->size(); i++ ) {
            ITrpJsonValue* element = arr->at(i);
            bool is_duplicate = false;

            switch (element->getType()) {
                case TRP_STRING:
                    if (!str_bucket.insert(static_cast<TrpJsonString*>(element)->getValue()).second) {
                        is_duplicate = true;
                    }
                    break;
                case TRP_NUMBER:
                    if (!nbr_bucket.insert(static_cast<TrpJsonNumber*>(element)->getValue()).second) {
                        is_duplicate = true;
                    }
                    break;
                case TRP_BOOL:
                    if (!bool_bucket.insert(static_cast<TrpJsonBool*>(element)->getValue()).second) {
                        is_duplicate = true;
                    }
                    break;
                case TRP_NULL:
                    if (null_found) is_duplicate = true;
                    else null_found = true;
                    break;
                default:
                    break;
            }

            if (is_duplicate) {
                ValidationError err;

                ctx.pushPath("[" + intToString(i) + "]");
                err.path = ctx.getCurrentPath();
                ctx.popPath();
                err.msg = "Duplicate item found in array, Items must be unique";
                err.expected = SCHEMA_ARRAY;
                err.actual = TRP_ARRAY;
        
                ctx.pushError(err);
                if ( !got_error ) got_error = true;
            }
        }
    }
    if ( got_error ) return false;
    return true;
}