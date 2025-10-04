#include "include/TrpSchemaArray.hpp"
#include "include/TrpSchemaString.hpp"
#include "include/TrpSchemaNumber.hpp"

#include "lib/TrpJson.hpp"


int main (int ac, char ** av) {
    if (ac != 2) return 1;
    TrpJsonParser parser(av[1]);

    if (!parser.parse()) {
        std::cerr << "zaba" << std::endl;
    }

    parser.prettyPrint();

    TrpSchemaArray arr;

    TrpSchemaString str;
    str.max(2);
    TrpSchemaNumber nbr;
    nbr.min(10);

    SchemaVec vec;

    vec.push_back(&str);
    vec.push_back(&nbr);

    arr.tuple(vec).uniq(true);

    TrpValidatorContext ctx;


    if (!arr.validate(parser.getAST(), ctx)) {
        ctx.printErrors();
    } else {
        std::cout << "good trip" << std::endl;
    }

}