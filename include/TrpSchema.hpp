#pragma once

#include "TrpValidatorContext.hpp"
#include <sstream>
#include <string>
#include <set>

#ifndef TRPSCHEMA_HPP
#define TRPSCHEMA_HPP

class TrpSchema
{
    public:
        virtual ~TrpSchema( void ) {};
        virtual bool validate(ITrpJsonValue* value, TrpValidatorContext& ctx) const = 0;
        virtual SchemaType getType() const = 0;
};

#endif