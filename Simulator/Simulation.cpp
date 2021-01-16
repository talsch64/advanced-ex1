#include <iostream>
#include "Simulation.h"

Simulation::Simulation() : requestsFileParser(std::make_unique<RequestsFileParser>()),
                           registrar(std::make_unique<Registrar>()) {
}

void Simulation::addGisFactory(std::function<std::unique_ptr<AbstractGIS>()> gisFactory) {
    std::unique_ptr<GISContainer> gisContainer = std::make_unique<GISContainer>(gisFactory, nextName,
                                                                                registrar->getMapFilePath());
    gisContainers.push_back(std::move(gisContainer));
}

void Simulation::addNavigationFactory(
        std::function<std::unique_ptr<AbstractNavigation>(const NavigationGIS &)> navigationFactory) {
    std::unique_ptr<NavigationContainer> navigationContainer = std::make_unique<NavigationContainer>(navigationFactory,
                                                                                                     nextName);
    navigationContainers.push_back(std::move(navigationContainer));
}

void Simulation::setNextName(std::string name) {
    nextName = name;
}

std::unique_ptr<GISContainer> &Simulation::getGISContainer(int index) {
    return gisContainers[index];
}

std::unique_ptr<NavigationContainer> &Simulation::getNavigationContainer(int index) {
    return navigationContainers[index];
}

const NavigationRequest &Simulation::getNavigationRequest(int index) {
    return requests[index];
}

void Simulation::startSimulation() {
    requests = requestsFileParser->parse(registrar->getNavigationRequestsPath());
    taskManager = std::make_unique<NavigationTasksManager>(gisContainers.size(), navigationContainers.size(),
                                                           requests.size());
    resultsAnalyzer = std::make_unique<ResultsAnalyzer>(gisContainers.size(), navigationContainers.size(),
                                                        requests.size(), registrar->getOutputPath());
    results = std::make_unique<std::unique_ptr<TaskResult>[]>(
            gisContainers.size() * navigationContainers.size() * requests.size());
    threads = std::make_unique<std::thread[]>(registrar->getNumThreads());

    //TODO: case of 1 thread
    for (int i = 0; i < registrar->getNumThreads(); i++) {
        threads[i] = std::thread(&Simulation::navigationThread, this);
    }

    for (int i = 0; i < registrar->getNumThreads(); i++) {
        threads[i].join();
    }

    resultsAnalyzer->analyze();
    resultsAnalyzer->writeResultsToFile();
}

void Simulation::navigationThread() {
    bool cond = true;
    while (cond) {
        std::unique_ptr<NavigationTask> task;
        taskMutex.lock();
        cond = taskManager->hasTask();
        if (cond) {
            task = taskManager->getNextTask();
        }
        taskMutex.unlock();
        if (cond) {
            std::unique_ptr<TaskResult> result = executeTask(*task);
            setResult(task->getGisIndex(), task->getNavigationIndex(), task->getRequestIndex(), std::move(result));
            taskManager->discardTask(*task);
        }

    }
}

std::unique_ptr<TaskResult> &Simulation::getResult(int gisIndex, int navigationIndex, int requestIndex) {
    int index = gisIndex * navigationContainers.size() * requests.size() +
                navigationIndex * requests.size() +
                requestIndex;
    return results[index];
}

void Simulation::setResult(int gisIndex, int navigationIndex, int requestIndex, std::unique_ptr<TaskResult> result) {
    getResult(gisIndex, navigationIndex, requestIndex) = std::move(result);
}

std::unique_ptr<TaskResult> Simulation::executeTask(const NavigationTask &task) {
    auto &navigation = task.getNavigation();
    auto req = task.getRequest();
    std::unique_ptr<TaskResult> result = std::make_unique<TaskResult>();
    result->setRoutes(navigation->getRoutes(req.getFrom(), req.getTo()));
    auto &routes = result->getRoutes();
    bool validRoutes = routes->isValid();
    if (validRoutes) {
        auto &shortestDistanceRoute = routes->shortestDistance();
        auto &shortestTimeRoute = routes->shortestTime();
        auto &start = shortestTimeRoute.getWayStartPoint();
        auto &end = shortestTimeRoute.getWayEndPoint();
        result->setShortestDistanceValid(task.getValidator()->validateRoute(start, end, shortestDistanceRoute));
        result->setShortestTimeValid(task.getValidator()->validateRoute(start, end, shortestTimeRoute));
    }
    result->setGisUsageCount(task.getNavigationGis()->getUsageCounter());
    return result;
}

void Simulation::clear() {
    gisContainers.clear();
    navigationContainers.clear();
    results.reset();
}

const std::unique_ptr<Registrar> &Simulation::getRegistrar() const {
    return registrar;
}


