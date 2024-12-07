
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
using std string;
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
        istringstream iss(line);
        string command;
        iss >> command;

        if (command == "settlement") {
            string name;
            int type;
            iss >> name >> type;
            addSettlement(new Settlement(name, static_cast<SettlementType>(type)));

        } else if (command == "facility") {
            string name;
            int category, price, lifeQuality, economy, environment;
            iss >> name >> category >> price >> lifeQuality >> economy >> environment;
            addFacility(FacilityType(name, static_cast<FacilityCategory>(category), price, lifeQuality, economy, environment));

        } else if (command == "plan") {
            string settlementName, policyName;
            iss >> settlementName >> policyName;
            Settlement &settlement = getSettlement(settlementName);
            SelectionPolicy *policy = createSelectionPolicy(policyName);
            addPlan(settlement, policy);
            planCounter++;
            delete policy;
            policy = nullptr;
        }
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
    : isRunning(other.isRunning), planCounter(other.planCounter) {
    for (const auto &action : other.actionsLog) {
        actionsLog.push_back(action->clone());
    }

    for (const auto &plan : other.plans) {
        plans.push_back(new Plan(*plan));
    }

    for (const auto &settlement : other.settlements) {
        settlements.push_back(new Settlement(*settlement));
    }

    for (const auto &facility : other.facilitiesOptions) {
        facilitiesOptions.push_back(new Facility(*facility));
    }
}

//destructor
Simulation::~Simulation() {
    for (auto &action : actionsLog) {
        delete action;
    }
    actionsLog.clear();

    plans.clear();

    for (auto &settlement : settlements) {
        delete settlement;
    }
    settlements.clear();
    
}

//copy assignment operator
Simulation &Simulation::operator=(const Simulation &other) {
    if (this != &other) {
        for (auto &action : actionsLog) {
            delete action;
        }
        actionsLog.clear();

        for (auto &plan : plans) {
            delete plan;
        }
        plans.clear();

        for (auto &settlement : settlements) {
            delete settlement;
        }
        settlements.clear();

        for (auto &facility : facilitiesOptions) {
            delete facility;
        }
        facilitiesOptions.clear();

        isRunning = other.isRunning;
        planCounter = other.planCounter;

        for (const auto &action : other.actionsLog) {
            actionsLog.push_back(action->clone());
        }

        for (const auto &plan : other.plans) {
            plans.push_back(new Plan(*plan));
        }

        for (const auto &settlement : other.settlements) {
            settlements.push_back(new Settlement(*settlement));
        }

        for (const auto &facility : other.facilitiesOptions) {
            facilitiesOptions.push_back(new Facility(*facility));
        }
    }

    return *this;
}

//move constructor
Simulation::Simulation(Simulation &&other) 
    : isRunning(other.isRunning), planCounter(other.planCounter) {
    for (auto &action : other.actionsLog) {
        actionsLog.push_back(action);
    }
    other.actionsLog.clear();

    for (auto &plan : other.plans) {
        plans.push_back(plan);
    }
    other.plans.clear();

    for (auto &settlement : other.settlements) {
        settlements.push_back(settlement);
    }
    other.settlements.clear();

    for (auto &facility : other.facilitiesOptions) {
        facilitiesOptions.push_back(facility);
    }
    other.facilitiesOptions.clear();
}

//move assignment operator
Simulation &Simulation::operator=(Simulation &&other) {
    if (this != &other) {
        for (auto &action : actionsLog) {
            delete action;
        }
        actionsLog.clear();

        for (auto &plan : plans) {
            delete plan;
        }
        plans.clear();

        for (auto &settlement : settlements) {
            delete settlement;
        }
        settlements.clear();

        for (auto &facility : facilitiesOptions) {
            delete facility;
        }
        facilitiesOptions.clear();

        isRunning = other.isRunning;
        planCounter = other.planCounter;

        for (auto &action : other.actionsLog) {
            actionsLog.push_back(action);
        }
        other.actionsLog.clear();

        for (auto &plan : other.plans) {
            plans.push_back(plan);
        }
        other.plans.clear();

        for (auto &settlement : other.settlements) {
            settlements.push_back(settlement);
        }
        other.settlements.clear();

        for (auto &facility : other.facilitiesOptions) {
            facilitiesOptions.push_back(facility);
        }
        other.facilitiesOptions.clear();
    }

    return *this;
}


//start the simulation
void Simulation::start() {
    open();
    cout<< "The simulation has started" << endl; 
    while (isRunning)
    {
        string input;   
        getline(cin, input);
        std::vector<std::string> arguments = Auxiliary::parseArguments(input);
       
        if (arguments[0] == "step") {
            addAction(new SimulateStep(stoi(arguments[1]))->act(*this));
        }

        if (arguments[0] == "plan"){
            addAction(new AddPlan(arguments[1], arguments[2])->act(*this));
        }

        if (arguments[0] == "settlement") {
            addAction(new AddSettlement(arguments[1], stoi(arguments[2]))->act(*this));
        }
        
        if (arguments[0] == "facility") {
            addAction(new AddFacility(arguments[1], stoi(arguments[2]), stoi(arguments[3]), stoi(arguments[4]), stoi(arguments[5]), stoi(arguments[6]))->act(*this));
        }
        
        if (arguments[0] == "planStatus") {
           addAction(new PrintStatus(arguments[1])->act(*this));
        }

        if (arguments[0] == "changePolicy") {
            addAction(new ChangePlanPolicy(stoi(arguments[1]), arguments[2])->act(*this));
        }

        if (arguments[0] == "log") {
            addAction(new PrintActionsLog()->act(*this));
            
        }

        if (arguments[0] == "backup") {
            addAction(new BackupSimulation()->act(*this));
        }

        if (arguments[0] == "restore") {
            addAction(new RestoreSimulation()->act(*this));
        }

        if (arguments[0] == "close") {
            ;
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
    facilitiesOptions.push_back(facility);
    return true;
}

bool isSettlementExists(const string &settlementName){
    for (const auto &settlement : settlements) {
        if (settlement->getName() == settlementName) {
            return true;
        }
    }
    return false;
}

Settlement &getSettlement(const string &settlementName){
    for (const auto &settlement : settlements) {
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