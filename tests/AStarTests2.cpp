#include "gtest/gtest.h"
#include "GISMock.h"
#include "../navigation/Route.h"
#include "../NavigationGIS.h"
#include "../navigation/Navigation.h"

void assertRoute(const std::vector<std::pair<EntityId, Direction>> &expected, const Route &actual) {
    auto routeIterator = actual.getWays().begin();
    for (auto &wayPair : expected) {
        ASSERT_EQ(wayPair.first, routeIterator->first);
        ASSERT_EQ(wayPair.second, routeIterator->second);
        routeIterator++;
    }
}

TEST(AStar, BasicPathTest) {
    GIS gis;
    gis.loadMapFile("astar.json");
    NavigationGIS navigationGis(gis);
    Navigation navigation(navigationGis);
    std::vector<std::pair<EntityId, Direction>> expectedDistanceRoute = {
            std::make_pair(EntityId("way1"), Direction::A_to_B),
            std::make_pair(EntityId("way2"), Direction::A_to_B),
            std::make_pair(EntityId("way5"), Direction::A_to_B),
            std::make_pair(EntityId("way6"), Direction::A_to_B)
    };
    Coordinates startCoord(Longitude(22.57676323126267),
                           Latitude(-30.64052973777345));
    Coordinates endCoord(Longitude(22.273619),
                         Latitude(-30.497604));

    Routes routes = navigation.getRoutes(startCoord, endCoord);
    const Route &shortestDistance = routes.shortestDistance();
    assertRoute(expectedDistanceRoute, shortestDistance);
}