#include <unordered_set>
#include "Grid.h"
#include "../entities/geometry/Geometry.h"
#include "../entities/geometry/CoordinatesMath.h"
#include "../entities/geometry/PointList.h"
#include "../entities/geometry/Point.h"
#include "../entities/geometry/Circle.h"

Grid::Grid() : topologicalSearch(std::make_unique<TopologicalSearch>()) {
}

CellEntities Grid::getEntitiesOnGrid(const Coordinates &coordinates) {
    GridCell grid_cell = truncateCoordinates(coordinates);
    auto pair = grid.find(grid_cell);
    if (pair == grid.end()) {
        return CellEntities();
    } else {
        return pair->second;
    }
}

std::vector<Grid::GridCell> Grid::setEntityOnGrid(const Entity &entity) {
    Geometry &entityGeometry = *(entity.getGeometry().get());
    std::vector<Grid::GridCell> cells = entityGeometry.getGridCells(this);
    EntityId id = entity.getId();
    for (const auto &cell: cells) {
        grid[cell].insertEntity(id);
    }
    return cells;
}

std::vector<Coordinates> Grid::getGeometryGridCells(const PointList &geometry) const {
    std::vector<Coordinates> points = geometry.getPoints();
    std::unordered_set<GridCell> cellSet;
    for (int i = 0; i < points.size() - 1; i++) {
        addIntervalsGridCells(points[i], points[i + 1], cellSet);
    }
    std::vector<GridCell> cells;
    cells.reserve(cellSet.size());
    for (const auto &cell: cellSet) {
        cells.push_back(cell);
    }
    return cells;
}

std::vector<Grid::GridCell> Grid::getGeometryGridCells(const Point &geometry) const {
    return std::vector<Grid::GridCell>{truncateCoordinates(geometry.getCoordinates())};
}

std::vector<Grid::GridCell> Grid::getGeometryGridCells(const Circle &geometry) const {
    return topologicalSearch->searchCircleInGrid(*this, geometry.getCenter(), geometry.getRadius());
}

void
Grid::addIntervalsGridCells(const Coordinates &coord1, const Coordinates &coord2,
                            std::unordered_set<GridCell> &cells) const {
    if (fabs(coord1.latitude() - coord2.latitude()) < Grid::precision &&
        fabs(coord1.latitude() - coord2.latitude()) < Grid::precision) {
        cells.insert(truncateCoordinates(coord1));
        cells.insert(truncateCoordinates(coord2));
        return;
    }
    Coordinates midPoint = CoordinatesMath::calculateMidpoint(coord1, coord2);
    addIntervalsGridCells(coord1, midPoint, cells);
    addIntervalsGridCells(midPoint, coord2, cells);
}

//TODO remove after testing
//std::vector<Grid::GridCell> Grid::setEntityOnGrid(const PointList &geometry, const EntityId &id) {
//    std::vector<GridCell> cells = getGeometryGridCells(geometry);
//    for(const auto& cell: cells) {
//        grid[cell].insertEntity(id);
//    }
//    return cells;
//}
