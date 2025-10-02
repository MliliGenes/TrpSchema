#pragma once

#include "TrpValidatorContext.hpp"

#ifndef TRPSCHEMA_HPP
#define TRPSCHEMA_HPP

class TrpSchema
{
    public:
        virtual ~TrpSchema( void ) = 0;
        virtual bool validate(ITrpJsonValue* value, TrpValidatorContext& ctx) const = 0;
        virtual SchemaType getType() const = 0;
};

#endif