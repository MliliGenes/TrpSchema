#include "../include/TrpSchemaObject.hpp"

TrpSchemaObject::TrpSchemaObject() : has_min(false), has_max(false) {}

TrpSchemaObject& TrpSchemaObject::min( size_t min_value) {
    if ( !has_min ) has_min = true;

    min_items = min_value;
    return *this;
}

TrpSchemaObject& TrpSchemaObject::max( size_t max_value) {
    if ( !has_max ) has_max = true;

    max_items = max_value;
    return *this;
}

TrpSchemaObject& TrpSchemaObject::property( std::string key, TrpSchema* schema ) {
    std::map<std::string, TrpSchema*>::iterator it = properties.find(key);

    if (it != properties.end()) return *this;

    properties.insert(std::pair(key, schema));
    return *this;
}


TrpSchemaObject& TrpSchemaObject::required( std::string required ) {
    std::map<std::string, TrpSchema*>::iterator it = properties.find(required);

    if (it == properties.end()) return *this;

    required_entries.push_back( required );
    return *this;
}


bool TrpSchemaObject::validate(ITrpJsonValue* value, TrpValidatorContext& ctx) const {
    bool got_errors = false;

    if (!value || value->getType() != TRP_OBJECT ) {
         ValidationError err;

        err.path = ctx.getCurrentPath();
        err.expected = SCHEMA_OBJECT;
        err.actual = value ? value->getType() : TRP_ERROR;
        err.msg = "Expected object, found " + tokenTypeToString(err.actual);

        ctx.pushError(err);
        return false;
    }

    TrpJsonObject* obj = static_cast<TrpJsonObject*>(value);

    if (has_min && obj->size() < min_items) {
        ValidationError err;

        err.path = ctx.getCurrentPath();
        err.msg = "Object must have at least " + std::to_string(min_items) + " properties, but has " + std::to_string(obj->size());

        ctx.pushError(err);
        if ( !got_errors ) got_errors = true;
    }

    if (has_max && obj->size() > max_items) {
        ValidationError err;

        err.path = ctx.getCurrentPath();
        err.msg = "Object must have at most " + std::to_string(max_items) + " properties, but has " + std::to_string(obj->size());

        ctx.pushError(err);
        if ( !got_errors ) got_errors = true;
    }

    if ( required_entries.size() ) {
        for (size_t i = 0; i < required_entries.size(); i++) {
            if (!obj->find(required_entries[i])) {
                ValidationError err;

                err.path = ctx.getCurrentPath();
                err.msg = "Required property '" + required_entries[i] + "' is missing";
                
                ctx.pushError(err);
                if ( !got_errors ) got_errors = true;
            }
        }
    }

    std::map<std::string, TrpSchema*>::const_iterator it;
    for (it = properties.begin(); it != properties.end(); it ++) {
        ctx.pushPath("." + it->first);
        ITrpJsonValue* prop = obj->find(it->first);
        if (prop) {
            if (!it->second->validate(prop, ctx)) {
                if ( !got_errors ) got_errors = true;
            }
        }
        ctx.popPath();
    }

    if ( got_errors ) return false;
    return true;
}