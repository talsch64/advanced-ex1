#ifndef GIS_CPP_RANDTESTUTILS_H
#define GIS_CPP_RANDTESTUTILS_H

#include "../GIS/GIS.h"
#include "Bound.h"
#include "../UserCommon/entities/Entity.h"
#include "../UserCommon/entities/Junction.h"
#include "../UserCommon/entities/POI.h"
#include "../UserCommon/entities/Way.h"
#include "mocks/GISMock.h"
#include "../Navigation/Routes.h"
#include "mocks/RouteMock.h"
#include "../Common/NavigationGIS.h"
#include "NavigationMock.h"

class RandTestUtils {
public:
    static int randInt(int min, int max);

    static double randDouble(double min, double max);

    static bool randBool();

    static Coordinates randCoord(Bound bound);

    static Coordinates randCoord();

    static Bound randBound();

    static std::vector<Junction *>
    generateJunctions(GISMock &gis, IdGenerator &idGenerator, int n, Bound bound);

    static void
    generateWay(GISMock &gis, const EntityId &id, const std::vector<Coordinates> &curves, Junction &from,
                Junction &to, const TrafficDirection &dir, int speedLimit, std::optional<Meters> length);

    static void
    generateWays(GISMock &gis, IdGenerator &idGenerator, int n, Bound bound,
                 std::vector<Junction *> junctions);

    /**
     * Call this in the test AFTER the regular A*, because it adds fake ways and junctions which may mess up the next searches
     * @param gis
     * @param idGenerator
     * @param bestTimeRoute
     * @param bestDistanceRoute
     * @param start
     * @param end
     * @return
     */
    static std::unique_ptr<Routes>
    getBestRoutes(NavigationMock &navigation, GISMock &gis, IdGenerator &idGenerator, const Coordinates &start,
                  const Coordinates &end);

    static void getBestRoutesDFS(NavigationGIS &navGis, RouteMock *bestTimeRoute, RouteMock *bestDistanceRoute,
                                 std::vector<std::pair<EntityId, Direction>> ways, const EntityId &start,
                                 const EntityId &end,
                                 const EntityId &current, Meters currentLength, Minutes currentTime);
};


#endif //GIS_CPP_RANDTESTUTILS_H
