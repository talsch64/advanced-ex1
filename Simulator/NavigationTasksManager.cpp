#include "NavigationTasksManager.h"
#include "Simulation.h"

NavigationTasksManager::NavigationTasksManager(int gisAmount, int navigationsAmount, int requestsAmount) : gisAmount(
        gisAmount), navigationsAmount(navigationsAmount), requestsAmount(requestsAmount) {}

std::unique_ptr<NavigationTask> NavigationTasksManager::getNextTask() {
    Simulation &sim = Simulation::getInstance();
    auto &gisContainer = sim.getGISContainer(nextGisIndex);
    auto &navigationContainer = sim.getNavigationContainer(nextNavigationIndex);
    auto &navigationReq = sim.getNavigationRequest(nextRequestIndex);
    std::unique_ptr<NavigationTask> task = createTask(gisContainer, navigationContainer,
                                                      navigationReq, nextGisIndex,
                                                      nextNavigationIndex, nextRequestIndex);
    gisContainer->incrementUsageCount();
    if (nextRequestIndex == requestsAmount - 1) {
        nextRequestIndex = 0;
        if (nextNavigationIndex == navigationsAmount - 1) {
            nextNavigationIndex = 0;
            gisContainer->enableCloseMap();
            nextGisIndex++;
        } else {
            nextNavigationIndex++;
        }
    } else {
        nextRequestIndex++;
    }
    return task;
}

bool NavigationTasksManager::hasTask() const {
    return nextGisIndex < gisAmount;
}

void NavigationTasksManager::discardTask(NavigationTask &task) {
    auto &gisContainer = task.getGisContainer();
    gisContainer.decrementUsageCount();
}

std::unique_ptr<NavigationTask> NavigationTasksManager::createTask(std::unique_ptr<GISContainer> &gisContainer,
                                                                   std::unique_ptr<NavigationContainer> &navigationContainer,
                                                                   const NavigationRequest &request, int gisIndex,
                                                                   int navigationIndex, int requestIndex) {
    return std::make_unique<NavigationTask>(gisContainer, navigationContainer,
                                            request, gisIndex,
                                            navigationIndex, requestIndex);
}
