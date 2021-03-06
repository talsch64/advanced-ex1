
#pragma once

#include "Routes.h"
#include "../Common/Restrictions.h"
#include "../Common/NavigationGIS.h"
#include "../Common/AbstractNavigation.h"

/**
 * @brief Navigation class.
 * Navigation class manages the navagation between two Corrdinates.
 * Supported operations:
 * Get the shortest routes by distance and time.
 *
 */
class Navigation : public AbstractNavigation {
    mutable std::unordered_map<EntityId, const AbstractWay &> waysMap;     // ways caching for lower gis usage
    mutable std::unordered_map<EntityId, std::vector<EntityId>> waysByJunctionMap;     // ways caching for lower gis usage

public:
    /**
     * @brief Construct a new Navigation object
     *
     * @param navigationGis - Note: you get a const ref of NavigationGIS
     */
    Navigation(const NavigationGIS &navigationGis);

    /**
     * @brief Get the Routes object
     *
     * @param start
     * @param end
     * @return Routes
     */
    std::unique_ptr<AbstractRoutes> getRoutes(const Coordinates &start, const Coordinates &end) const;

    /**
     * @brief Get the Routes object
     *        Note: bonus method - not mandatory
     *
     * @param start
     * @param end
     * @param restrictions - Restricted Ways
     * @return Routes
     */
    std::unique_ptr<AbstractRoutes>
    getRoutes(const Coordinates &start, const Coordinates &end, const Restrictions &restrictions) const;

    //must be protected instead of private because we have a mock class that exposes a method as public
protected:

    /**
     * @brief returns a non-restricted way that is not an highway (unless the distance from coord is less then 3 meters)
     * @param coord
     * @param restrictions
     * @return tuple contains coordinates on the way, reference to the way itself and the initial segment number.
     */
    std::tuple<Coordinates, const AbstractWay &, std::size_t>
    getClosestValidWay(const Coordinates &coord, const Restrictions &restrictions) const;
};
