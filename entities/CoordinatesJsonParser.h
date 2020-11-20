#ifndef EX1_COORDINATESJSONPARSER_H
#define EX1_COORDINATESJSONPARSER_H


#include <rapidjson/fwd.h>
#include <vector>
#include "../GISdefs.h"
#include <rapidjson/document.h>

class CoordinatesJsonParser {
public:
    Coordinates parse(rapidjson::Value &coordinates);
};

#endif //EX1_COORDINATESJSONPARSER_H