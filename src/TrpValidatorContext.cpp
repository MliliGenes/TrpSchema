#include "../include/TrpValidatorContext.hpp"

TrpValidatorContext::TrpValidatorContext( size_t _max_errors ) : max_errors(_max_errors) {}

void TrpValidatorContext::pushPath( const std::string _path ) {
    if ( _path.empty() ) return;
    paths.push_back( _path );
}

void TrpValidatorContext::popPath( void ) {
    if ( paths.empty() ) return;
    paths.pop_back();
}

void TrpValidatorContext::pushError( ValidationError err ) {
    errors.push_back( err );
}

std::string TrpValidatorContext::getCurrentPath( void ) {
    std::string full_path;

    for (TrpValidationPath::iterator it = paths.begin(); it != paths.end(); it ++) {
        full_path += *it;
    }

    return full_path;
}