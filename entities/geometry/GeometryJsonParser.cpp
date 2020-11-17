
#include <memory>
#include "GeometryJsonParser.h"


std::unique_ptr<Geometry> GeometryJsonParser::parseGeometry(rapidjson::Value &doc) {
    if (!doc.HasMember("geometry") || !doc["geometry"].IsObject()) {
        throw std::runtime_error("JSON entity doesn't contain geometry");
    }
    doc = doc["geometry"];
    if (!doc.HasMember("type") || !doc["type"].IsString()) {
        throw std::runtime_error("Geomtry JSON doesn't contain type");
    }
    std::string type = doc["type"].GetString();
    if (type == "Circle") {
        return parseCircle(doc);
//    } else if (type == "Polygon") {
//        return parsePolygon(doc);
    } else {
        throw std::runtime_error("Unsupported geometry type");
    }
}

std::unique_ptr<Circle> GeometryJsonParser::parseCircle(rapidjson::Value &doc) {
    std::vector<Coordinates> coordinates = parseCoordinates(doc);
    if (!doc.HasMember("radius") || !doc["radius"].IsNumber()) {
        throw std::runtime_error("Circle JSON doesn't contain radius");
    }
    Meters radius(doc["radius"].GetDouble());
    std::unique_ptr<Circle> circle(new Circle(coordinates, radius));
    return circle;
}

std::unique_ptr<Polygon> parsePolygon(rapidjson::Value &value) {
    return nullptr;
}


std::vector<Coordinates> GeometryJsonParser::parseCoordinates(rapidjson::Value &doc) {
    std::vector<Coordinates > coordinates;
    if (!doc.HasMember("coordinates") || !doc["coordinates"].IsArray())  {
        throw std::runtime_error("Invalid coordinates in JSON");
    }
    coordinates.push_back(CoordinatesParser::parse(doc["coordinates"]));
    return coordinates;
}