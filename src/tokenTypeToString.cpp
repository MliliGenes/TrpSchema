#include "../include/tokenTypeToString.hpp"

std::string tokenTypeToString(TrpType type) {
    switch (type) {
        case TRP_ARRAY: return "Array";
        case TRP_OBJECT: return "Object";
        case TRP_STRING: return "String";
        case TRP_NUMBER: return "Number";
        case TRP_BOOL: return "Bool";
        case TRP_NULL: return "Null";
        default: return "UNKNOWN";
    }
}