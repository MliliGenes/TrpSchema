#include "../include/TrpSchemaObject.hpp"

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

}


bool TrpSchemaObject::validate(ITrpJsonValue* value, TrpValidatorContext& ctx) const {
    return true;
}