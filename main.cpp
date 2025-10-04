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
    TrpSchemaNumber nbr;

    str.max(50);
    arr.tuple(std::vector<TrpSchema*>{}).uniq(true);

    TrpValidatorContext ctx(100);


    if (!arr.validate(parser.getAST(), ctx)) {
        std::cerr << "bad trip" << std::endl;
    } else {
        std::cout << "good trip" << std::endl;
    }

}