#include "../include/TrpSchemaFactory.hpp"

TrpSchemaFactory::~TrpSchemaFactory() {
    for (size_t i = 0; i < _managedSchemas.size(); ++i) {
        delete _managedSchemas[i];
    }
}

TrpSchemaString& TrpSchemaFactory::string() {
    TrpSchemaString* schema = new TrpSchemaString();
    _managedSchemas.push_back(schema);
    return *schema;
}

TrpSchemaNumber& TrpSchemaFactory::number() {
    TrpSchemaNumber* schema = new TrpSchemaNumber();
    _managedSchemas.push_back(schema);
    return *schema;
}

TrpSchemaBool& TrpSchemaFactory::boolean() {
    TrpSchemaBool* schema = new TrpSchemaBool();
    _managedSchemas.push_back(schema);
    return *schema;
}

TrpSchemaObject& TrpSchemaFactory::object() {
    TrpSchemaObject* schema = new TrpSchemaObject();
    _managedSchemas.push_back(schema);
    return *schema;
}

TrpSchemaArray& TrpSchemaFactory::array() {
    TrpSchemaArray* schema = new TrpSchemaArray();
    _managedSchemas.push_back(schema);
    return *schema;
}

TrpSchemaNull& TrpSchemaFactory::null() {
    TrpSchemaNull* schema = new TrpSchemaNull();
    _managedSchemas.push_back(schema);
    return *schema;
}
