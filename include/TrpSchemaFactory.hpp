#pragma once

#include "TrpSchemaArray.hpp"
#include "TrpSchemaBool.hpp"
#include "TrpSchemaNull.hpp"
#include "TrpSchemaNumber.hpp"
#include "TrpSchemaObject.hpp"
#include "TrpSchemaString.hpp"
#include <vector>

class TrpSchemaFactory {
    private:
        std::vector<TrpSchema*> _managedSchemas;

    public:
        ~TrpSchemaFactory();

        TrpSchemaString& string();
        TrpSchemaNumber& number(); 
        TrpSchemaBool& boolean();
        TrpSchemaObject& object();
        TrpSchemaArray& array();
        TrpSchemaNull& null();
};