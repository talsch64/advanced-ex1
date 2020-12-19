

#ifndef GIS_CPP_ASTAR_H
#define GIS_CPP_ASTAR_H

#include <queue>
#include <unordered_map>
#include <functional>
#include <memory>
#include "../GISNamedTypes.h"
#include "../entities/Way.h"
#include "../NavigationGIS.h"
#include "../CoordinatesMath.h"
#include "Route.h"

#define MAX_SPEED 130.0


/**
 * @brief AStar class implements the A* algorithm.
 * Allows to find the shortest Route from origin point to destination point By distance and by time.
 * We implemented the algorithm such that the initial Node already has a "kilometrage" of the startWay.
 * If the first way is bidirectional then we initialize two node.
 * Once one of the Junctions of the final way is reached (if the way is bidirectional), we retract the route that led to the way.
 * If the final way is unidirectional, the algorithm retracts the Route after finding the "from" junction of the final way.

 */
class AStar {
    const NavigationGIS &navigationGIS;
    const Coordinates origin;
    const Coordinates destination;
    const Way &startWay;
    const Way &finalWay;

public:
    /**
     *
     * @param navigationGis
     * @param origin
     * @param destination
     * @param startWay the way which the origin point is located on.
     * @param finalWay the way which the destination point is located on.
     */
    AStar(const NavigationGIS &navigationGis, const Coordinates &origin, const Coordinates &destination,
          const Way &startWay, const Way &finalWay);

    Route shortestByDistance();

    Route shortestByTime();

private:
    using Edge = std::optional<std::pair<EntityId, Direction>>;

    /**
     * class Node represents a node in the A* algorithm
     */
    class Node {
        Coordinates coordinates;
        EntityId junctionId;
        Meters distanceSoFar;
        Minutes timeSoFar;
        double costSoFar;
        // priority(n) = cost(n) + h(n) where h is the heuristic function
        double priority;
        Edge prevEdgeWay;
        std::shared_ptr<Node> prevNode;

    public:
        Node(const Coordinates &coordinates, const EntityId &junctionId, const Meters &distanceSoFar,
             const Minutes &timeSoFar, double costSoFar,
             double priority, const Edge &prevEdgeWay, std::shared_ptr<Node> prevNode);


        const Edge &getPrevEdgeWay() const;

        double getCostSoFar() const;

        const Coordinates &getCoordinates() const;

        double getPriority() const;

        const EntityId &getJunctionId() const;

        const Meters &getDistanceSoFar() const;

        const Minutes &getTimeSoFar() const;

        const std::shared_ptr<Node> &getPrevNode() const;


    };

    /**
     * @brief heuristic by distance
     * @param start start coordinates
     * @return the aerial distance between the coordinates and the destination of the search
     */
    static double distanceHeuristic(const Coordinates &coordinates, const Coordinates &destination);

    /**
     * @brief heuristic by Time
     * @param coordinates start coordinates
     * @return the time it takes to get start coordinates to the destination coordinates in aerial distance with max speed (130 kmh)
     */
    static double timeHeuristic(const Coordinates &coordinates, const Coordinates &destination);

    /**
     * @brief calculate the cost of the way by distance in meters
     * @param way
     * @return the length of the way
     */
    static double costByDistance(const Way &way);

    /**
     * @brief calculates the cost of the way by time in minutes
     * @param way
     * @return the time to cross the way in minutes
     */
    static double costByTime(const Way &way);

    /**
     * @brief comparator function for storing Nodes in the priority_queue of A* algorithm.
     * order of comparison: lowest priority -> shortest distance -> shortest time
     * @param node1
     * @param node2
     * @return true if node1 "larger" then node2, else false
     */
    static bool compareByDistance(std::shared_ptr<Node> node1, std::shared_ptr<Node> node2);

    /**
     * @brief comparator function for storing Nodes in the priority_queue of A* algorithm.
     * order of comparison: lowest priority -> shortest time -> shortest distance
     * @param node1
     * @param node2
     * @return true if node1 "larger" then node2, else false
     */
    static bool compareByTime(std::shared_ptr<Node> node1, std::shared_ptr<Node> node2);

    Route searchShortestRoute(double (*heuristicFunc)(const Coordinates &start, const Coordinates &target),
                              double (*costFunc)(const Way &way),
                              bool (*comparator)(std::shared_ptr<Node> node1, std::shared_ptr<Node> node2));

    /**
     *
     * @param node final Node of the search
     * @return all the ways of the Route by order
     */
    std::vector<std::pair<EntityId, Direction>> restoreShortestRoute(std::shared_ptr<Node> node);

private:
    /**
     * @brief calculates the aerial distance between the given coordinates and the end of the way
     * @param way
     * @param coordinates
     * @param front if true then calculate the distance from the "from" junction, else from "to" junction
     * @return the aerial distance between the given coordinates and the end of the way
     */
    Meters distanceFromWaysEnd(const Way &way, Coordinates coordinates, bool front);

    /**
     * @brief calculate the time it take to get from the edge of the Way to the given coordinates in MAX_SPEED
     * @param way
     * @param coordinates
     * @param front if true then calculate the time from the "from" junction, else from "to" junction
     * @return the time it take to get from the edge of the Way to the given coordinates in MAX_SPEED
     */
    Minutes timeFromWaysEnd(const Way &way, Coordinates coordinates, bool front);


    /**
     * @brief creates the neighbor node to the current node
     * @param currNode
     * @param wayId the way's id that connects the current node and the neighbor
     * @param heuristicFunc
     * @param costFunc
     * @return
     */
    std::shared_ptr<Node> createNeighbor(std::shared_ptr<Node> currNode, EntityId wayId,
                                         double (*heuristicFunc)(const Coordinates &start, const Coordinates &end),
                                         double (*costFunc)(const Way &));

    /**
     * @brief initializes the first Node according to the given direction.
     * @param heuristicFunc
     * @param costFunc
     * @param direction if direction is A_to_B initialize node that represents "to" junction, else "from" junction.
     * @return
     */
    std::shared_ptr<Node> createInitialNode(double (*heuristicFunc)(const Coordinates &start, const Coordinates &end),
                                            double (*costFunc)(const Way &), Direction direction);


};

#endif //GIS_CPP_ASTAR_H