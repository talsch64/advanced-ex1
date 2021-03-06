#ifndef EX1_CIRCLE_H
#define EX1_CIRCLE_H

#include "Geometry.h"
#include <rapidjson/document.h>

class Grid;

class Circle : public Geometry {
    Meters radius;
    Coordinates coordinates;
public:
    Circle(const Coordinates &coordinates, const Meters &radius);

    const Meters &getRadius() const {
        return radius;
    }

    rapidjson::Value toJson(rapidjson::Document::AllocatorType &allocator) const override;

    Coordinates getCenter() const;

    std::vector<Coordinates> getGridCells(const Grid *grid) const override;

    bool isInCircle(const TopologicalSearch *topologicalSearch, const Coordinates &searchCenter, Meters searchRadius) const override;

    Coordinates getClosestPoint(const Coordinates &coordinates) const override;

};

#endif //EX1_CIRCLE_H
