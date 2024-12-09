#include "Action.h"
#include "SelectionPolicy.h"
#include <string>
#include <vector>
#include "Facility.h"
#include "Settlement.h"
#include <sstream>
#include <iostream>
#include "Simulation.h"
using namespace std;
extern Simulation* backup;
enum class SettlementType;
enum class FacilityCategory;

// BaseAction
BaseAction::BaseAction() : errorMsg(), status(){}

ActionStatus BaseAction::getStatus() const{
    return status;
}


void BaseAction::complete(){
    status = ActionStatus::COMPLETED;
}
void BaseAction::error(string errorMsg){
    status = ActionStatus::ERROR;
    this->errorMsg = errorMsg;
    cout << "Error: " << errorMsg << endl;
}

const string &BaseAction::getErrorMsg() const{
    return errorMsg;
}

string BaseAction::statusToString() const{
    if (status == ActionStatus::COMPLETED){
        return "COMPLETED";
    }
    else{
        return "ERROR: " + errorMsg;
    }
}

// SimulateStep
SimulateStep::SimulateStep(const int numOfSteps) : numOfSteps(numOfSteps){}

void SimulateStep::act(Simulation &simulation) {
   for (int i = 0; i < numOfSteps; ++i) {
        simulation.step();  
    }
    complete();
}

const string SimulateStep::toString() const {
    return "Step " + to_string(numOfSteps) + statusToString();
}

SimulateStep *SimulateStep::clone() const {
    return new SimulateStep(*this);
}

AddPlan::AddPlan(const string &settlementName, const string &selectionPolicy) :
settlementName(settlementName), selectionPolicy(selectionPolicy) {}

void AddPlan::act(Simulation &simulation){
    
    if (!simulation.isSettlementExists(settlementName)) {
        error("Cannot create this plan: Settlement does not exist");
        return;
    }
    
    Settlement* settlement = &simulation.getSettlement(settlementName);
    SelectionPolicy* policy = nullptr;
    // Switch-case to handle different selection policies
    if (selectionPolicy == "nve") {
        policy = new NaiveSelection(); 
    } else if (selectionPolicy == "bal") {
        policy = new BalancedSelection(0, 0, 0); 
    } else if (selectionPolicy == "eco") {
        policy = new EconomySelection(); 
    } else if (selectionPolicy == "env") {
        policy = new SustainabilitySelection(); 
    } else {
        error("Cannot create this plan: Invalid selection policy");
    
    }
    
    simulation.addPlan(*settlement, policy);
    delete policy;
    policy = nullptr;
    complete();
}

const string AddPlan::toString() const {
    return "Plan " + settlementName + " " + selectionPolicy + " " + BaseAction::statusToString();
}

AddPlan *AddPlan::clone() const {
    return new AddPlan(*this);
}

AddSettlement::AddSettlement(const string &settlementName,SettlementType settlementType)
    : settlementName(settlementName),
    settlementType(settlementType) {}

void AddSettlement::act(Simulation &simulation) {
    Settlement* toAdd = new Settlement(settlementName, settlementType);
    if(simulation.addSettlement(toAdd)){
        complete();
    }
    else{
        delete toAdd;
        toAdd = nullptr;
        error("Settlement already exists");
    }
}

AddSettlement *AddSettlement::clone() const {
    return new AddSettlement(*this);
}

const string AddSettlement::toString() const {
    return "Settlement " + settlementName + " " + to_string(static_cast<int>(settlementType)) + " " + statusToString();
}

//AddFacility
AddFacility::AddFacility(const string &facilityName, const FacilityCategory facilityCategory, const int price, const int lifeQualityScore, const int economyScore, const int environmentScore)
    :facilityName(facilityName), 
    facilityCategory(facilityCategory), 
    price(price), 
    lifeQualityScore(lifeQualityScore), 
    economyScore(economyScore), 
    environmentScore(environmentScore) {}

void AddFacility::act(Simulation &simulation) {
    const FacilityType& newFacility = FacilityType(facilityName, facilityCategory, price, lifeQualityScore, economyScore, environmentScore);
    if(simulation.addFacility(newFacility)){
        complete();
    }
    else{
        error("Facility already exists");
    }
}

AddFacility *AddFacility::clone() const {
    return new AddFacility(*this);
}

const string AddFacility::toString() const {
    return "Facility " + facilityName + " " + to_string(static_cast<int>(facilityCategory))
           + " " + to_string(price) + " " + to_string(lifeQualityScore)
           + " " + to_string(economyScore) + " " + to_string(environmentScore) + " " + BaseAction::statusToString();
}

//PrintPlanStatus
PrintPlanStatus::PrintPlanStatus(int planId): planId(planId){}

void PrintPlanStatus::act(Simulation &simulation) {
    
    if (planId >= simulation.getPlanCounter()) {
        error("Plan doesn't exist");
    
    } else {
        Plan& plan = simulation.getPlan(planId);
        cout << plan.toString() << endl;
        complete();
    }
}

PrintPlanStatus *PrintPlanStatus::clone() const {
    return new PrintPlanStatus(*this);
}

const string PrintPlanStatus::toString() const {
    return "PlanStatus: " + to_string(planId) + statusToString();
}


//ChangePlanPolicy
ChangePlanPolicy::ChangePlanPolicy(const int planId, const string &newPolicy) :  planId(planId), newPolicy(newPolicy) {}

void ChangePlanPolicy::act(Simulation &simulation) {
    if(planId >= simulation.getPlanCounter()){
        error("Plan doesn't exist");
        return;
    }
    
    Plan& plan = simulation.getPlan(planId);
    string prev = plan.getSelectionPolicy()->toString();
    if (prev == newPolicy) {
        error("Cannot change selection policy: Same as current policy");
        return;
    }

    SelectionPolicy *newSelectionPolicy = nullptr;
    if (newPolicy == "nve") {
        newSelectionPolicy = new NaiveSelection();
    } else if (newPolicy == "bal") {
        newSelectionPolicy = new BalancedSelection(
            plan.getlifeQualityScore(),
            plan.getEconomyScore(),
            plan.getEnvironmentScore());
    } else if (newPolicy == "eco") {
        newSelectionPolicy = new EconomySelection();
    } else if (newPolicy == "env") {
        newSelectionPolicy = new SustainabilitySelection();
    } else {
        error("Invalid selection policy");
        return;
    }

    plan.setSelectionPolicy(newSelectionPolicy);
    cout << "Plan ID: " << planId << endl;
    cout << "Previous Policy: " << prev << endl;
    cout << "New Policy: " << newPolicy << endl;

    complete();
}

ChangePlanPolicy *ChangePlanPolicy::clone() const {
    return new ChangePlanPolicy(*this);
}

const string ChangePlanPolicy::toString() const {
    return "ChangePlanPolicy " + to_string(planId) + " " + newPolicy + BaseAction::statusToString();
}


//PrintActionsLog
PrintActionsLog::PrintActionsLog(){}

void PrintActionsLog::act(Simulation &simulation) {
    for(BaseAction* action : simulation.getActionsLog()){
        cout << action->toString() << endl;
    }
    complete();
    
}

PrintActionsLog *PrintActionsLog::clone() const {
    return new PrintActionsLog(*this);
}

const string PrintActionsLog::toString() const {
    return "PrintActionsLog " + statusToString();
}


//Close
Close::Close(){}

void Close::act(Simulation &simulation) {
    simulation.close();
    complete();
}

Close *Close::clone() const {
    return new Close(*this);
}

const string Close::toString() const {
    return "close " + statusToString();
}


//BackupSimulation
BackupSimulation::BackupSimulation(){
}

void BackupSimulation::act(Simulation &simulation) {
    if (backup == nullptr)
    {
        backup = new Simulation(simulation);
    }
    else{
        delete backup;
        *backup = simulation;
    }
    complete();
}

BackupSimulation *BackupSimulation::clone()const {
    return new BackupSimulation();
}

const string BackupSimulation::toString() const {
    return "backup " +  statusToString();
}

//RestoreSimulation
RestoreSimulation::RestoreSimulation(){}

void RestoreSimulation::act(Simulation &simulation){
    if (backup == nullptr){
        error("no backup available");
    }
    else {
        simulation = *backup;
        complete();
    }

}

RestoreSimulation *RestoreSimulation::clone() const{
    return new RestoreSimulation(*this);
}

const string RestoreSimulation::toString() const {
    return "restore " + statusToString();
}