
#include "Simulation.h"
#include "Auxiliary.h"
#include "SelectionPolicy.h"
#include "Settlement.h"
#include "Facility.h"
#include "Plan.h"
#include "Action.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
using namespace std;

class BaseAction;
class SelectionPolicy;


//Constructor
Simulation::Simulation(const std::string &configFilePath) 
    : isRunning(false), planCounter(0) {
    std::ifstream configFile(configFilePath);
    if (!configFile.is_open()) {
        throw runtime_error("Could not open configuration file: " + configFilePath);
    }

    string line;
    while (getline(configFile, line)) {
        vector<string> arguments = Auxiliary::parseArguments(line);
        if(arguments.empty()){
            continue;
        }   
        if (arguments[0] == "settlement") {
            addSettlement(new Settlement(arguments[1], stoi(arguments[2])));
        } else if (arguments[0] == "facility") {
            addFacility(FacilityType(arguments[1], static_cast<FacilityCategory>(stoi(arguments[2])), stoi(arguments[3]), stoi(arguments[4]), stoi(arguments[5]), stoi(arguments[6])));
        } else if (arguments[0] == "plan") {
            Settlement &settlement = getSettlement(arguments[1]);
            SelectionPolicy *policy = createSelectionPolicy(arguments[2]);
            addPlan(settlement, policy);
            delete policy;
            policy = nullptr;
        }

    configFile.close();
}


SelectionPolicy* Simulation::createSelectionPolicy(const string &policyName) {
    if (policyName == "nve") return new NaiveSelection();
    if (policyName == "bal") return new BalancedSelection(0,0,0);
    if (policyName == "eco") return new EconomySelection();
    if (policyName == "env") return new SustainabilitySelection();
    return nullptr;
}


//copy constructor  
Simulation::Simulation(const Simulation &other) 
    : isRunning(other.isRunning),
    planCounter(other.planCounter),
    actionsLog(),
    settlements(), 
    plans() {
    for (const BaseAction* action : other.actionsLog) {
        actionsLog.push_back(action->clone());
    }

    facilitiesOptions.clear();
    for (const FacilityType& facility : other.facilitiesOptions){
        facilitiesOptions.push_back(facility);
    }

    for (const Settlement* settlement : other.settlements) {
        settlements.push_back(new Settlement(*settlement));
    }

    for (const Plan &plan : other.plans) {
        plans.push_back(Plan(plan));
    }
    for (const auto &plan : other.plans) {
        Plan newPlan = Plan(plan.getPlanID(), plan.getSettlement(), plan.getSelectionPolicy(), plan.getFacilities());
        newPlan.setLifeQualityScore(plan.getLifeQualityScore());    
        newPlan.setEconomyScore(plan.getEconomyScore());
        newPlan.setEnvironmentScore(plan.getEnvironmentScore());
        newPlan.setPlanStatus(plan.getPlanStatus());
        newPlan.setConstructionLimit(plan.getConstructionLimit());
        for (const auto &facility : plan.getFacilities()) {
            newPlan.addFacility(facility->clone());
        }
        for (const auto &facility : plan.getUnderConstruction()) {
            newPlan.addUnderConstruction(facility->clone());
        }
    }
}

//destructor
Simulation::~Simulation() {
    for (BaseAction* action : actionsLog) {
        delete action;
    }
    actionsLog.clear();

    for (Settlement* settlement : settlements) {
        delete settlement;
    }
    settlements.clear();
    
}

//copy assignment operator
Simulation &Simulation::operator=(const Simulation &other) {
    if (this != &other) {
        for (BaseAction* action : actionsLog) {
            delete action;
        }
        actionsLog.clear();

        for (Settlements* settlement : settlements) {
            delete settlement;
        }
        settlements.clear();

        facilitiesOptions.clear();
        for (const FacilityType& facility : other.facilitiesOptions){
                facilitiesOptions.push_back(move(facility));
        }

        plans.clear();


        isRunning = other.isRunning;
        planCounter = other.planCounter;

        for (const BaseAction* action : other.actionsLog) {
            actionsLog.push_back(action->clone());
        }

        for (const auto &plan : other.plans) {
            Plan newPlan = Plan(plan.getPlanID(), plan.getSettlement(), plan.getSelectionPolicy(), plan.getFacilities());
            newPlan.setLifeQualityScore(plan.getLifeQualityScore());
            newPlan.setEconomyScore(plan.getEconomyScore());
            newPlan.setEnvironmentScore(plan.getEnvironmentScore());
            newPlan.setPlanStatus(plan.getPlanStatus());
            newPlan.setConstructionLimit(plan.getConstructionLimit());
            for (const auto &facility : plan.getFacilities()) {
                newPlan.addFacility(facility->clone());
            }
            for (const auto &facility : plan.getUnderConstruction()) {
                newPlan.addUnderConstruction(facility->clone());
            }
        }

        for (const Settlement * settlement : other.settlements) {
            settlements.push_back(new Settlement(*settlement));
        }


    return *this;
}


//move constructor
Simulation::Simulation(Simulation &&other) noexcept
    : isRunning(other.isRunning), 
    planCounter(other.planCounter),
    actionLog(move(other.actionLog)),
    settlements(move(other.settlements)),
    facilitiesOptions(move(other.facilitiesOptions)){
        for (const Plan &plan : other.plans) {
            plans.push_back(Plan(plan));
        }

        other.isRunning = false;
        other.planCounter = 0;
        other.actionLog.clear();
        other.settlements.clear();
    }
}


//move assignment operator
Simulation &Simulation::operator=(Simulation &&other) {
    if (this != &other) {
        for (BaseAction* action : actionsLog) {
            delete action;
        }
        actionsLog.clear();

        for (Settlement* settlement : settlements) {
            delete settlement;
        }
        settlements.clear();

        isRunning = other.isRunning;
        planCounter = other.planCounter;
        settlements = other.settlements;
        actionsLog = other.actionsLog;
        facilitiesOptions = move(other.facilitiesOptions);
        plans = move(other.plans);
    }

    return *this;
}


//start the simulation
void Simulation::start() {
    open();
    cout<< "The simulation has started\n"; 
    while (isRunning)
    {
        string input;   
        getline(cin, input);
        vector<string> arguments = Auxiliary::parseArguments(input);
        if(arguments.empty()){
            continue;
        }
        else if (arguments[0] == "step") {
            addAction(new SimulateStep(stoi(arguments[1]))->act(*this));
        }

        else if (arguments[0] == "plan"){
            addAction(new AddPlan(arguments[1], arguments[2])->act(*this));
        }

        else if (arguments[0] == "settlement") {
            addAction(new AddSettlement(arguments[1], stoi(arguments[2]))->act(*this));
        }
        
        else if (arguments[0] == "facility") {
            addAction(new AddFacility(arguments[1], stoi(arguments[2]), stoi(arguments[3]), stoi(arguments[4]), stoi(arguments[5]), stoi(arguments[6]))->act(*this));
        }
        
        else if (arguments[0] == "planStatus") {
           addAction(new PrintStatus(arguments[1])->act(*this));
        }

        else if (arguments[0] == "changePolicy") {
            addAction(new ChangePlanPolicy(stoi(arguments[1]), arguments[2])->act(*this));
        }

        else if (arguments[0] == "log") {
            addAction(new PrintActionsLog()->act(*this));
            
        }

        else if (arguments[0] == "backup") {
            addAction(new BackupSimulation()->act(*this));
        }

        else if (arguments[0] == "restore") {
            addAction(new RestoreSimulation()->act(*this));
        }

        else if (arguments[0] == "close") {
            addAction(new CloseSimulation()->act(*this));
        }
    
        else {
            cout << "Invalid command\n";
        }
}

//add a plan to the simulation
void Simulation::addPlan(const Settlement &settlement, SelectionPolicy *selectionPolicy){
    Plan newPlan = Plan(planCounter, settlement, selectionPolicy, facilitiesOptions);
    plans.push_back(newPlan);
    planCounter++;  
}

// Add an action to the ActionLog
void Simulation::addAction(BaseAction *action){
    actionsLog.push_back(action);
}


bool Simulation::addSettlement(Settlement *settlement){
    if(isSettlementExists(settlement->getName())){
        return false;
    }
    else{
        settlements.push_back(settlement);
        return true;
    }
}

bool Simualtion::addFacility(FacilityType facility){
    for (const auto &facilityOption : facilitiesOptions) {
        if (facilityOption.getName() == facility.getName()) {
            return false;
        }
    }
    facilitiesOptions.push_back(move(facility));
    return true;
}

bool isSettlementExists(const string &settlementName){
    for (Settlement* settlement : settlements) {
        if (settlement->getName() == settlementName) {
            return true;
        }
    }
    return false;
}

Settlement &getSettlement(const string &settlementName){
    for (Setllemnt* settlement : settlements) {
        if (settlement->getName() == settlementName) {
            return *settlement;
        }
    }
}

Plan Simualtion::&getPlan(const int planID){
    for (const auto &plan : plans) {
        if (plan.getPlanID() == planID) {
            return plan;
        }
    }
}

int Simulation::getPlanCounter() const {
    return planCounter;
}

vector<BaseAction *> Simulation::getActionsLog() const {
    return actionsLog;
}

void Simulation::step(){
    for (auto &plan : plans) {
        plan.step();
    }
}

void Simulation::close() {
    isRunning = false;

    for(Plan plan : plans){
        plan.printStatus();
    }
}

void Simulation::open() {
    isRunning = true;
}

private:
    bool isRunning;
    int planCounter; // For assigning unique plan IDs
    vector<BaseAction *> actionsLog;
    vector<Plan> plans;
    vector<Settlement *> settlements;
    vector<FacilityType> facilitiesOptions;
};