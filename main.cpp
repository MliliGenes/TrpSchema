#include "lib/TrpSchema.hpp"

int main (int ac, char ** av) {
    if (ac != 2) return 1;
    TrpJsonParser parser(av[1]);

    if (!parser.parse()) {
        std::cerr << "bad trip: Failed to parse JSON file." << std::endl;
        return 1;
    }

    parser.prettyPrint();

    TrpSchemaFactory factory;

    TrpSchemaObject& rootSchema = factory.object()
        .property("arr", &factory.array()
                    .uniq(true)
                    .item(&factory
                        .number()
                        .min(5)
                        .max(10)
                    )
                );
    
    TrpValidatorContext ctx;
    if (!rootSchema.validate(parser.getAST(), ctx)) {
        std::cerr << "\n--- Validation Errors ---" << std::endl;
        ctx.printErrors();
        return 1;
    } else {
        std::cout << "\ngood trip: Configuration is valid!" << std::endl;
    }

    return 0;
}  