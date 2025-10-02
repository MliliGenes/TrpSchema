#include "../include/TrpSchemaString.hpp"


TrpSchemaString::TrpSchemaString( void ) {

}


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