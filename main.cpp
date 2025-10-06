#include "include/TrpSchemaArray.hpp"
#include "include/TrpSchemaString.hpp"
#include "include/TrpSchemaNumber.hpp"
#include "include/TrpSchemaNull.hpp"
#include "include/TrpSchemaBool.hpp"

#include "lib/TrpJson.hpp"


int main (int ac, char ** av) {
    if (ac != 2) return 1;
    TrpJsonParser parser(av[1]);

    if (!parser.parse()) {
        std::cerr << "bad trip" << std::endl;
    }

    parser.prettyPrint();

    TrpSchemaArray arr;

    TrpSchemaString str;
    TrpSchemaNumber nbr;
    TrpSchemaBool bol;
    TrpSchemaNull null;

    SchemaVec vec;

    vec.push_back(&str.max(50).min(5));
    vec.push_back(&nbr.max(60).min(0));
    vec.push_back(&bol);
    vec.push_back(&null);

    arr.tuple(vec).uniq(true);
    TrpValidatorContext ctx;

    if (!arr.validate(parser.getAST(), ctx)) {
        ctx.printErrors();
    } else {
        std::cout << "good trip" << std::endl;
    }
}  