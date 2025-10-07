#include "../include/TrpValidatorContext.hpp"

TrpValidatorContext::TrpValidatorContext( void ) {}

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

const TrpValidationError& TrpValidatorContext::getErrors( void ) const {
    return errors;
}

bool TrpValidatorContext::printErrors( void ) const {
    bool got_errors = !errors.empty();
    for (size_t i = 0; i < errors.size(); i++) {
        std::cerr
            << errors[i].path
            << ": "
            << errors[i].msg
            << std::endl;
    }
    return got_errors;
}