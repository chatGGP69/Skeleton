#include "Action.h"
#include <string>
#include <vector>
#include "Simulation.h"
enum class SettlementType;
enum class FacilityCategory;


BaseAction::BaseAction() : status(ActionStatus::COMPLETED), errorMsg(""){}

ActionStatus BaseAction::getStatus() const{
    return status;
}

virtual void act(Simulation& simulation)=0;
virtual const string toString() const=0;
virtual BaseAction* clone() const = 0;
virtual ~BaseAction() = default;

void BaseAction::complete(){
    status = ActionStatus::COMPLETED;
}
void BaseAction::error(string errorMsg){
    status = ActionStatus::ERROR;
    this->errorMsg = errorMsg;
    std::cout << "Error: " << errorMsg << std::endl;
}
const string &getErrorMsg() const{
    return errorMsg;
}

SimulateStep::SimulateStep(const int numOfSteps) : numOfSteps(numOfSteps){}

void act(Simulation &simulation) override{
   for (int i = 0; i < numOfSteps; ++i) {
        simulation.step();  
    }
    complete();
    //simulation.addAction(this);
}

const string toString() const override{
    return "Step " + to_string(numOfSteps) + status.toString();
}

SimulateStep *clone() const override{
    return new SimulateStep(numOfSteps);
}

AddPlan::AddPlan(const string &settlementName, const string &selectionPolicy) :
SettlementName(SettlementName), SelectionPolicy(SelectionPolicy) {}

void AddPlan::act(Simulation &simulation) override{
    
    if (!simulation.isSettlemnetExists) {
        error("Cannot create this plan: Settlement does not exist");
        return;
    }
    Settlement* settlement = simulation.getSettlement(settlementName);
    SelectionPolicy* policy = nullptr;
    // Switch-case to handle different selection policies
    if (selectionPolicy == "nve") {
        policy = new NaiveSelectionPolicy(); 
    } else if (selectionPolicy == "bal") {
        policy = new BalancedSelectionPolicy(plan->getlifeQualityScore(), plan->getEconomyScore(), plan->getEnvironmentScore()); 
    } else if (selectionPolicy == "eco") {
        policy = new EconomySelectionPolicy(); 
    } else if (selectionPolicy == "env") {
        policy = new SustainabilitySelectionPolicy(); 
    } else {
        error("Cannot create this plan: Invalid selection policy");
        delete policy;
        //simulation.addAction(this);
        return;
    }
    //simulation.addPlan(settlement, policy); 
    delete policy;

    complete();
    //simulation.addAction(this);
}

const string AddPlan::toString() const override{
    return "Plan " + settlementName + " " + selectionPolicy + status.toString();
}

AddPlan *clone() const override {
    return new AddPlan(this);
}

AddSettlement::AddSettlement(const string &settlementName,SettlementType settlementType):
settlementName(settlementName), settlementType(settlementType) {}

void AddSettlement::act(Simulation &simulation) override{
    // if (simulation.isSettlementExists(settlementName)) {
      //  error("Settlement already exists");
       // return;
       // simulation.addAction(this);
    //}
    // Add the settlement the simulation
    Settlement* toAdd = new Settlement(settlementName, settlementType) // who handles
    if(simulation.addSettlement(toAdd)){
        complete();
    }
    else{
        error("Settlement already exists");
    }
    //simulation.addAction(this);
}

AddSettlement *clone() const override{
    new AddSettlement(this);
}

const string AddSettlement::toString() const override{
    return "Settlement " + settlementName + " " + SettlementType + " " status.toString;
}

AddFacility::AddFacility(const string &facilityName, const FacilityCategory facilityCategory, const int price, const int lifeQualityScore, const int economyScore, const int environmentScore):
facilityName(facilityName), facilityCategory(facilityCategory), price(price), lifeQualityScore(lifeQualityScore), economyScore(economyScore), environmentScore(environmentScore) {}

void AddFacility::act(Simulation &simulation) override{
    Facility newFacility(facilityName, facilityCategory, price, lifeQualityScore, economyScore, environmentScore);
    if(simulation.addFacility(newFacility)){
        complete();
    }
    else{
        error("Facility already exists")
    }
    //simulation.addAction(this);
}

AddFacility *clone() const override{
    new AddFacility(this);
}

const string AddFacility::toString() const override{
    return "Facility " + facilityName + " " + to_string(static_cast<int>(facilityCategory))
           + " " + to_string(price) + " " + to_string(lifeQualityScore)
           + " " + to_string(economyScore) + " " + to_string(environmentScore) + " " + status.toString();
}

PrintPlanStatus::PrintPlanStatus(int planId): planId(planId){}

void PrintPlanStatus::act(Simulation &simulation) override{
    Plan& plan = Simulation.getPlan(planId);
    if (!plan) {
        error("Plan doesn't exist");
        //simulation.addAction(this);
        return;
    }
    cout << plan->toString() << endl;
    complete();
    //simulation.addAction(this);
}

PrintPlanStatus *clone() const override{
    return new PrintPlanStatus(this);
}

const string PrintPlanStatus::toString() const override{
    return "PlanStatus: " + to_string(planId) + status.toString();
}

ChangePlanPolicy::ChangePlanPolicy(const int planId, const string &newPolicy) :  planId(planId), newPolicy(newPolicy) {}

void ChangePlanPolicy::act(Simulation &simulation) override{
    Plan& plan = Simulation.getPlan(planId);
    if (!plan) {
        error("Plan doesn't exist");
        return;
        //simulation.addAction(this);
    }
    String prev = plan->getSelectionPolicy()->toString();
    if (prev == newPolicy) {
        error("Cannot change selection policy: Same as current policy");
        //simulation.addAction(this);
        return;
    }
    SelectionPolicy* newSP = nullptr;
     // Switch-case to handle different selection policies
    if (newPolicy == "nve") {
        newSP = new NaiveSelectionPolicy(); 
    } else if (newPolicy == "bal") {
        newSP = new BalancedSelectionPolicy(plan->getlifeQualityScore(), plan->getEconomyScore(), plan->getEnvironmentScore()); 
    } else if (newPolicy == "eco") {
        newSP = new EconomySelectionPolicy(); 
    } else if (newPolicy == "env") {
        newSP = new SustainabilitySelectionPolicy(); 
    } else {
        error("Invalid selection policy");
        delete newSP;
        return;
    }
    plan->setSelectionPolicy(newPS);
    cout << "planID: " << planId << endl;
    cout << "previousPolicy: " << prev << endl;
    cout << "newPolicy: " << newPolicy << endl;

    complete();
    delete newSP;
    //simulation.addAction(this);
}

ChangePlanPolicy *clone() const override{
    return new ChangePlanPolicy(this);
}

const string ChangePlanPolicy::toString() const override{
     return "ChangePlanPolicy " + to_string(planId) + " " + newPolicy + status.toString();
}

PrintActionsLog::PrintActionsLog(){}

void PrintActionsLog::act(Simulation &simulation) override{
    for( action in simulation.getActionsLog()){
        cout << action.toString() << planId << endl;
    }
    complete();
    //simulation.addAction(this);
}

PrintActionsLog *clone() const override{
    return new PrintActionsLog(this);
}

const string PrintActionsLog::toString() const override;

Close::Close(){}

void Close::act(Simulation &simulation) override{
    simulation.close();

    complete();
    //simulation.addAction(this);
}

Close *clone() const override;

const string Close::toString() const override;

BackupSimulation::BackupSimulation(){}

void BackupSimulation::act(Simulation &simulation) override{
    extern Simulation* backup = new Simulation(simulation);
    //simulation.backup();
    complete();
    //simulation.addAction(this);
}

BackupSimulation *clone() const override{
    new BackupSimulation(this);
}

const string BackupSimulation::toString() const override{
    return "backup " +  status.toString();
}

RestoreSimulation::RestoreSimulation(){}

void RestoreSimulation::act(Simulation &simulation) override{
    if (extern backup == nullptr){
        error("no backup available");
    }
    else {
        Simulation = &backup;
    }
    //simulation.restore();
    complete();
    //simulation.addAction(this);
}

RestoreSimulation *clone() const override{
    new RestoreSimulation(this);
}

const string RestoreSimulation::toString() const override{
    return "restore " + status.toString();
}