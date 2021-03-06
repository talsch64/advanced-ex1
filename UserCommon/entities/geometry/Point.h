//
// Created by Tal on 19/11/2020.
//

#ifndef EX1_POINT_H
#define EX1_POINT_H
#include "Geometry.h"
#include "../../../Common/GISNamedTypes.h"


class Point: public Geometry {
    Coordinates coordinates;
public:
    const Coordinates &getCoordinates() const;

    Point(const Coordinates &coordinates);

    rapidjson::Value toJson(rapidjson::Document::AllocatorType &allocator) const override;

    std::vector<Coordinates> getGridCells(const Grid *grid) const override;

    Coordinates getClosestPoint(const Coordinates &coord) const override;


    bool isInCircle(const TopologicalSearch *topologicalSearch, const Coordinates &center, Meters radius) const override;
};
#endif //EX1_POINT_H
