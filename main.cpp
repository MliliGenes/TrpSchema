#include "include/TrpSchemaArray.hpp"
#include "include/TrpSchemaString.hpp"
#include "include/TrpSchemaNumber.hpp"

#include "lib/TrpJson.hpp"


int main (int ac, char ** av) {
    if (ac != 2) return 1;
    TrpJsonParser parser(av[1]);

    parser.parse();

    parser.prettyPrint();

    TrpSchemaArray arr;
    TrpSchemaString str;

    str.min(10);
    arr.item(&str.min(2)).max(1).min(1).uniq(true);

    TrpValidatorContext ctx(100);


    if (!arr.validate(parser.getAST(), ctx)) {
        std::cerr << "bad trip" << std::endl;
    } else {
        std::cout << "good trip" << std::endl;
    }

}