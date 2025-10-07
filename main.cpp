#include "include/TrpSchemaArray.hpp"
#include "include/TrpSchemaObject.hpp"
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

    TrpSchemaObject obj;
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

    // --- Webserver Schema ---
    TrpSchemaObject webserverSchema;
    TrpSchemaString hostSchema;
    TrpSchemaNumber portSchema;
    TrpSchemaBool httpsSchema;
    TrpSchemaArray protocolsSchema;
    TrpSchemaString protocolStringSchema;
    TrpSchemaNumber timeoutSchema;

    protocolsSchema.item(&protocolStringSchema).uniq(true).min(1);

    webserverSchema.property("host", &hostSchema.min(1))
                   .property("port", &portSchema.min(1024).max(65535))
                   .property("enable_https", &httpsSchema)
                   .property("supported_protocols", &protocolsSchema)
                   .property("timeout", &timeoutSchema.min(0))
                   .required("host")
                   .required("port");
    // --- End Webserver Schema ---

    obj.property("array", &arr)
        .property("webserver", &webserverSchema);
    TrpValidatorContext ctx;

    if (!obj.validate(parser.getAST(), ctx)) {
        ctx.printErrors();
    } else {
        std::cout << "good trip" << std::endl;
    }
}  