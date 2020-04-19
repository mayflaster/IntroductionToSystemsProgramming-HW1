//
// Created by User on 27/11/2018.
//

//
// Created by mayfl on 27/11/2018.
//
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "mtm_elections.h"
#include "set.h"
#include "city.h"
#include "citizen.h"
#include "mtm_print.h"

#define MIN_AGE_TO_VOTE 17
#define DEFAULT_VOTE_WEIGHT 1
#define MIN_CANDIDATE_AGE 21


struct mtm_elections_t {
    Set cities;
};

/**
 * @param mtmElections
 * @param cityId - the city which we want to add the citizen to
 * @param citizen - the citizen that we want to add to the city
 * @return
 * MTM_ELECTIONS_NULL_ARGUMENT - if NULL argument sent
 * MTM_ELECTIONS_SUCCESS - if the adding succeed
 * MTM_ELECTIONS_CITIZEN_ALREADY_EXISTS - if this citizen alredy member in
 *in the city
 * MTM_ELECTIONS_CITY_DOES_NOT_EXIST - if there is no city with this id in
 * the system
 */
static MtmElectionsResult addToCitizenSet (MtmElections mtmElections, int
cityId, Citizen citizen);

/**
 * get's an id and return the struct of the citizen.
 * @param mtmElections - the elections system
 * @param citizenId - the id of the citizen that we look for
 * @return
 * a copy of the citizen
 * NULL if there is no citizen with this id in the system
 *
 */
static Citizen findCitizenById (MtmElections mtmElections, int citizenId);
/**
 *
 * gets the elections system and a citizen and checks if he is a candidate in
 * one of the cities in it
 * true if the citizen is candidate
 * false otherwise
 */
static bool isCandidate (MtmElections mtmElections, Citizen citizen);
/**
 *
    gets the elections system and the id of the city.
 * @return
 * the city struct with this city id
 * NULL if there is no city with this id in the system
 */
static City getCityById (MtmElections mtmElections, int cityId);

/**
 *  this function calculate the mayor of city with criteria the sent to her.
 * @param mtmElections - the elections system
 * @param city - the city where we want to find the mayor
 * @param mayor - pointer to the id of the new mayor.
 * @param filename - the name of the file where the details about the winner
 * will be printed
 * @param rank - the function the calculate the contrubutions of every citizen
 * @param auxilaryData - sent to the rank function
 * @return
 * MTM_ELECTIONS_NO_CANDIDATES_IN_CITY - in case of city without candidate
 * MTM_ELECTIONS_SUCCESS - in case of successful calculating
 */
static MtmElectionsResult mtmElectionsMayorOfCityByRank(
        MtmElections mtmElections, City city, int* mayor,
        const char* filename,RankFunc rank, void *auxilaryData);

/**
 * function for unique ordered list of the winners
 * @param first mayor
 * @param second  mayor
 * @return
 * true if the two winners are equals
 * false otherwise
 */
static bool winnerEquals (Citizen first, Citizen Second);

/**
 *  gets the elections system, the id of the mayor, and filname and send to
 *  the printing functions all the needed details about the mayor.
 *  returns:
 *   MTM_ELECTIONS_SUCCESS - in case of successful opening of the file
 *   MTM_ELECTIONS_FILE_ERROR - in case of error in opening the file
 */
static MtmElectionsResult printWinner (MtmElections mtmElections,int mayor_id,
        const char* filename);

/**
 * gets 2 pointers and always return false
 */
static bool allwaysFalse (int* first, int* second);

/**
 * gets 2 pointers and always return true
 */
static bool allwaysTrue (int* first, int* second);

/**
 * deallocates the number pointer
 */
static void freeFunc (int* number);
/**
 * gets an pointer and copy it.
 * return a copy of the pointer or NULL in case of null argumant sent
 */
static  int* copyFunc (int* number);





MtmElections mtmElectionsCreate(){
    MtmElections mtm_elections_system;
    mtm_elections_system = malloc(sizeof(*mtm_elections_system));
    if (!mtm_elections_system){
        return  NULL;
    }
    mtm_elections_system->cities = setCreate((copySetElements)&copyCity,
                                             (freeSetElements)&freeCity,
                                             (compareSetElements)
                                                     &citiesCompare);
    if (!mtm_elections_system->cities){
        free(mtm_elections_system);
        return NULL;
    }
    return mtm_elections_system;
}

void mtmElectionsDestroy(MtmElections mtmElections){
    if (!mtmElections){
        return;
    }
    setDestroy(mtmElections->cities);
    free(mtmElections);
}


MtmElectionsResult mtmElectionsAddCity(MtmElections mtmElections, const char*
cityName, int cityId){
    if (!mtmElections || !cityName){
        return MTM_ELECTIONS_NULL_ARGUMENT;
    }
    if (cityId < 0){
        return  MTM_ELECTIONS_ILLEGAL_ID;
    }
    City new_city = createCity (cityId,cityName);
    if (new_city == NULL){
        return MTM_ELECTIONS_MEMORY_ERROR;
    }
    SetResult set_add_result = setAdd(mtmElections->cities, new_city);
    freeCity(new_city);
    if (set_add_result==SET_SUCCESS){
        return MTM_ELECTIONS_SUCCESS;
    }
    else if (set_add_result == SET_OUT_OF_MEMORY){
        return MTM_ELECTIONS_MEMORY_ERROR;
    }
    else if (set_add_result ==SET_ITEM_ALREADY_EXISTS){
        return MTM_ELECTIONS_CITY_ALREADY_EXISTS;
    }
    return MTM_ELECTIONS_NULL_ARGUMENT;

}


MtmElectionsResult mtmElectionsAddCitizen(MtmElections mtmElections, const char* citizenName,
                                          int citizenId, int citizenAge, int
                                          yearsOfEducation, int cityId) {
    if (!mtmElections || !citizenName) {
        return MTM_ELECTIONS_NULL_ARGUMENT;
    }
    if (citizenId < 0 || cityId < 0) {
        return MTM_ELECTIONS_ILLEGAL_ID;
    }
    if (citizenAge <= 0) {
        return MTM_ELECTIONS_ILLEGAL_AGE;
    }
    if (yearsOfEducation < 0) {
        return MTM_ELECTIONS_ILLEGAL_NUMBER_OF_YEARS;
    }
    Citizen new_citizen = createCitizen( citizenId,citizenName, citizenAge,
                                         yearsOfEducation, cityId);
    if (!new_citizen) {
        return MTM_ELECTIONS_MEMORY_ERROR;
    }
    SET_FOREACH(City, curr_city, mtmElections->cities) {
        Set citizenSet = getCitizenSet(curr_city);
        if (setIsIn(citizenSet, new_citizen)){
            freeCitizen(new_citizen);
            setDestroy(citizenSet);
            return MTM_ELECTIONS_CITIZEN_ALREADY_EXISTS;
        }
        setDestroy(citizenSet);
    }
    return addToCitizenSet(mtmElections,cityId, new_citizen);

}








MtmElectionsResult MtmElectionsCitizenGetName(MtmElections mtmElections, int
citizenId, char** name){
    if (!mtmElections||!name){
        return MTM_ELECTIONS_NULL_ARGUMENT;
    }
    if (citizenId<0){
        return  MTM_ELECTIONS_ILLEGAL_ID;
    }
    char* name_copy = NULL;
    Citizen citizen = findCitizenById(mtmElections,citizenId);
    if (!citizen){
        return MTM_ELECTIONS_CITIZEN_DOES_NOT_EXIST;
    }
    getCitizenName (citizen,&name_copy);
    *name = name_copy;
    freeCitizen(citizen);
    return MTM_ELECTIONS_SUCCESS;

}



MtmElectionsResult MtmElectionsCitizenGetCity(MtmElections mtmElections, int
citizenId, int* cityId){
    if (!mtmElections || !cityId){
        return MTM_ELECTIONS_NULL_ARGUMENT;
    }
    if (citizenId<0){
        return  MTM_ELECTIONS_ILLEGAL_ID;
    }
    Citizen citizen = findCitizenById(mtmElections,citizenId);
    if (!citizen){
        return MTM_ELECTIONS_CITIZEN_DOES_NOT_EXIST;
    }
    *cityId = getCitizenCity(citizen);
    freeCitizen(citizen);
    return MTM_ELECTIONS_SUCCESS;
}

MtmElectionsResult MtmElectionsCitizenGetAge(MtmElections mtmElections, int citizenId, int* age){
    if (!mtmElections||!age){
        return MTM_ELECTIONS_NULL_ARGUMENT;
    }
    if (citizenId<0){
        return  MTM_ELECTIONS_ILLEGAL_ID;
    }
    Citizen citizen = findCitizenById(mtmElections,citizenId);
    if (!citizen){
        return MTM_ELECTIONS_CITIZEN_DOES_NOT_EXIST;
    }
    *age = getCitizenAge(citizen);
    freeCitizen(citizen);
    return MTM_ELECTIONS_SUCCESS;
}


MtmElectionsResult MtmElectionsCitizenGetEducation(MtmElections mtmElections, int citizenId, int* yearsOfEducation){
    if (!mtmElections||!yearsOfEducation){
        return MTM_ELECTIONS_NULL_ARGUMENT;
    }
    if (citizenId<0){
        return  MTM_ELECTIONS_ILLEGAL_ID;
    }
    Citizen citizen = findCitizenById(mtmElections,citizenId);
    if (!citizen){
        return MTM_ELECTIONS_CITIZEN_DOES_NOT_EXIST;
    }
    *yearsOfEducation = getCitizenEducation(citizen);
    freeCitizen(citizen);
    return MTM_ELECTIONS_SUCCESS;
}

MtmElectionsResult mtmElectionsAddCandidate(MtmElections mtmElections, int
candidateId, int cityId){
    if (!mtmElections){
        return MTM_ELECTIONS_NULL_ARGUMENT;
    }
    if (candidateId<0 || cityId<0){
        return MTM_ELECTIONS_ILLEGAL_ID;
    }
    City city = getCityById(mtmElections,cityId);
    if (!city){
        return MTM_ELECTIONS_CITY_DOES_NOT_EXIST;
    }
    Citizen candidate = findCitizenById(mtmElections,candidateId);
    if (!candidate) return MTM_ELECTIONS_CITIZEN_DOES_NOT_EXIST;
    if (getCitizenCity(candidate)!= cityId && !isCitizenInThisCity(city, candidate)) {
        freeCitizen(candidate);
        return MTM_ELECTIONS_CITIZEN_DOES_NOT_EXIST;
    }
    if (getCitizenAge(candidate)< MIN_CANDIDATE_AGE){
        freeCitizen(candidate);
        return MTM_ELECTIONS_AGE_NOT_APPROPRIATE;
    }
    clearAllPrioritiesToCitizenFromTheCity(city,candidate);
    SetResult update_result = updateCandidateSet(city, candidate);
    mtmElectionsSupportCandidate(mtmElections,candidateId, candidateId,0);//candidate supports himself
    freeCitizen(candidate); //need this becouse we insert copy of it to
    // candidate set
    if (update_result ==SET_ITEM_ALREADY_EXISTS){
        return MTM_ELECTIONS_CANDIDATE_ALREADY_EXISTS;
    }
    if (update_result == SET_SUCCESS) {
        return MTM_ELECTIONS_SUCCESS;
    }
    else if(update_result == SET_NULL_ARGUMENT) {
        return MTM_ELECTIONS_NULL_ARGUMENT;
    }
    return MTM_ELECTIONS_MEMORY_ERROR;
}




/**
 * This function gets mtmElection, candidate ID and city ID.
 * It will withdraw the candidate's candidacy in a city by getting his and the city Ids.
 * It will return:
 * MTM_ELECTIONS_SUCCESS- if the withdraw succeed
 * MTM_ELECTIONS_NULL_ARGUMENT- if mtm election point to NULL
 * MTM_ELECTIONS_ILLEGAL_ID - if city id or candidate id are negative numbers
 * MTM_ELECTIONS_CITY_DOES_NOT_EXIST- If city id does not exist in matmElection.
 * MTM_ELECTIONS_CANDIDATE_DOES_NOT_EXIST- If candidate id does not exist in matmElection on the city.
.
 */

MtmElectionsResult mtmElectionsWithdrawCandidate(MtmElections mtmElections,
                                                 int candidateId, int cityId){
    if (!mtmElections){
        return MTM_ELECTIONS_NULL_ARGUMENT;
    }
    if (candidateId <0 || cityId <0){
        return MTM_ELECTIONS_ILLEGAL_ID;
    }
    City candidate_city = getCityById(mtmElections, cityId);
    if (candidate_city==NULL){
        return MTM_ELECTIONS_CITY_DOES_NOT_EXIST;
    }
    Citizen candidate = findCitizenById(mtmElections,candidateId);
    if (!candidate){
        return MTM_ELECTIONS_CANDIDATE_DOES_NOT_EXIST;
    }
    SetResult withdraw_result = removeFromCandidateSet(candidate_city,
                                                       candidate);
    freeCitizen(candidate);
    if (withdraw_result == SET_NULL_ARGUMENT){
        return MTM_ELECTIONS_NULL_ARGUMENT;
    }
    if (withdraw_result==SET_ITEM_DOES_NOT_EXIST) {
        return MTM_ELECTIONS_CANDIDATE_DOES_NOT_EXIST;
    }
    removePriorityToAllCitizens (candidate_city, candidateId);
    return MTM_ELECTIONS_SUCCESS;
}


MtmElectionsResult mtmElectionsChangeAddress(MtmElections mtmElections, int
citizenId, int cityId) {
    if (!mtmElections) {
        return MTM_ELECTIONS_NULL_ARGUMENT;
    }
    if (citizenId < 0 || cityId < 0) {
        return MTM_ELECTIONS_ILLEGAL_ID;
    }
    Citizen citizen = findCitizenById(mtmElections,citizenId);
    if (!citizen){
        return MTM_ELECTIONS_CITIZEN_DOES_NOT_EXIST;
    }
    if (getCitizenCity(citizen) == cityId){
        freeCitizen(citizen);
        return MTM_ELECTIONS_SUCCESS;
    }
    City oldCity = getCityById(mtmElections,getCitizenCity(citizen));
    if (isCandidate(mtmElections,citizen)){
        mtmElectionsWithdrawCandidate(mtmElections,citizenId,getCitizenCity
                (citizen));
    }
    City newCity = getCityById(mtmElections,cityId);
    if (!newCity){
        freeCitizen(citizen);
        return MTM_ELECTIONS_CITY_DOES_NOT_EXIST;
    }
    removeFromCitizensSet(oldCity,citizen);
    return addToCitizenSet(mtmElections,cityId,citizen);
}


static MtmElectionsResult addToCitizenSet(MtmElections mtmElections, int cityId,
                                   Citizen citizen) {
    if (!mtmElections || !citizen){ //this is extra check - maybe we can
        // delete it
        return MTM_ELECTIONS_NULL_ARGUMENT;
    }
    SET_FOREACH(City, currnt_city, mtmElections->cities) {
        if (getCityId(currnt_city) == cityId) {
            changeCitizenCity(citizen, cityId);
            SetResult update_result = updateCitizensSet(currnt_city,
                                                        citizen);
            freeCitizen(citizen);
            if (update_result == SET_SUCCESS) {
                return MTM_ELECTIONS_SUCCESS;
            }
            if (update_result == SET_OUT_OF_MEMORY) {
                return MTM_ELECTIONS_MEMORY_ERROR;
            }
            else if (update_result == SET_NULL_ARGUMENT) {
                return MTM_ELECTIONS_NULL_ARGUMENT;
            }
            else if (update_result == SET_ITEM_ALREADY_EXISTS) {
                return MTM_ELECTIONS_CITIZEN_ALREADY_EXISTS;
            }
        }
    }
    freeCitizen(citizen);
    return MTM_ELECTIONS_CITY_DOES_NOT_EXIST;
}



/**
 * we have to make this function shorter!
 * @param mtmElections
 * @param citizenId
 * @param candidateId
 * @param priority
 * @return
 */
MtmElectionsResult mtmElectionsSupportCandidate(MtmElections mtmElections, int citizenId, int candidateId, int priority){
    if (!mtmElections) return MTM_ELECTIONS_NULL_ARGUMENT;
    if (citizenId<0 || candidateId <0){
        return MTM_ELECTIONS_ILLEGAL_ID;
    }
    if (priority<0) return MTM_ELECTIONS_ILLEGAL_PRIORITY;
    Citizen citizen = findCitizenById(mtmElections,citizenId);
    if (!citizen){
        return MTM_ELECTIONS_CITIZEN_DOES_NOT_EXIST;
    }
    Citizen candidate = findCitizenById(mtmElections,candidateId);
    if (!candidate){
        freeCitizen(citizen);
        return MTM_ELECTIONS_CANDIDATE_DOES_NOT_EXIST;
    }
    if (!isCandidate(mtmElections,candidate)){
        freeCitizen(candidate);
        freeCitizen(citizen);
        return MTM_ELECTIONS_CANDIDATE_DOES_NOT_EXIST;
    }
    if (getCitizenCity(candidate) != getCitizenCity(citizen)){
        freeCitizen(citizen);
        freeCitizen(candidate);
        return MTM_ELECTIONS_NOT_SAME_CITY;
    }
    int cityId = getCitizenCity(citizen);
    City city = getCityById(mtmElections,cityId);
    CitizenResult result_of_add_support =
            addPriorityForCandidateFromTheCity(city, citizen,candidateId, priority);
    if (result_of_add_support==CITIZEN_ALREADY_SUPPORT_CANDIDATE) {
        freeCitizen(citizen);
        freeCitizen(candidate);
        return MTM_ELECTIONS_ALREADY_SUPPORTED;
    }
    if (isCandidate(mtmElections,citizen)){
        if (citizenId!=candidateId){
            mtmElectionsCancelSupport(mtmElections,citizenId,candidateId);
            freeCitizen(citizen);
            freeCitizen(candidate);
            return MTM_ELECTIONS_CAN_NOT_SUPPORT;
        }
    }
    freeCitizen(citizen);
    freeCitizen(candidate);
    if (result_of_add_support == CITIZEN_PRIORITY_EXIST){
        return MTM_ELECTIONS_PRIORITY_EXISTS;
    }
    return MTM_ELECTIONS_SUCCESS;
}





/**
 *
 * @param mtmElections
 * @param citizenId
 * @return
 * copy of citizen struct
 * NULL id the citizen doesn't exist is the system
 */
static Citizen findCitizenById (MtmElections mtmElections, int citizenId){

    Citizen citizen = NULL;
    SET_FOREACH(City,current_city, mtmElections->cities){
         citizen = getCitizenById(citizenId,current_city);
        if (citizen != NULL){
            return citizen;
        }
    }
    return NULL; // if citizen not exist

}


static bool isCandidate (MtmElections mtmElections, Citizen citizen){
    if (!mtmElections || !citizen){
        return false;
    }
    SET_FOREACH(City,current_city, mtmElections->cities){
        if (isCandidateInThisCity(current_city,citizen)){
            return true;
        }
    }
    return false;

}



MtmElectionsResult mtmElectionsCancelSupport(MtmElections mtmElections, int
citizenId, int candidateId) {
    if (!mtmElections) {
        return MTM_ELECTIONS_NULL_ARGUMENT;
    }
    if (citizenId < 0 || candidateId < 0) {
        return MTM_ELECTIONS_ILLEGAL_ID;
    }
    Citizen citizen = findCitizenById(mtmElections, citizenId);
    if (!citizen) return MTM_ELECTIONS_CITIZEN_DOES_NOT_EXIST;
    int city_id = getCitizenCity(citizen);
    City city = getCityById(mtmElections,city_id);
    Citizen candidate = findCitizenById(mtmElections, candidateId);
    if (!isCandidate(mtmElections,candidate))
    {
        freeCitizen(candidate);
        freeCitizen(citizen);
        return MTM_ELECTIONS_CANDIDATE_DOES_NOT_EXIST;
    }
    freeCitizen(candidate);
    if (citizenId == candidateId) {
        freeCitizen(citizen);
        return MTM_ELECTIONS_MUST_SUPPORT;
    }
    UniqueOrderedListResult remove_result = removePriorityForCandidateFromTheCity(city, citizen,
                                                                                  candidateId);
    freeCitizen(citizen);
    if (remove_result == UNIQUE_ORDERED_LIST_ITEM_DOES_NOT_EXIST)  {
        return MTM_ELECTIONS_NOT_SUPPORTED;
    }
    return MTM_ELECTIONS_SUCCESS;
}



MtmElectionsResult mtmElectionsMayorOfCity(MtmElections mtmElections, int
cityId, int* mayor, const char* filename){
    if (!mtmElections || !mayor || !filename){
        return MTM_ELECTIONS_NULL_ARGUMENT;
    }
    if (cityId < 0){
        return  MTM_ELECTIONS_ILLEGAL_ID;
    }
    City city = getCityById(mtmElections,cityId);
    if (!city){
        return MTM_ELECTIONS_CITY_DOES_NOT_EXIST;
    }
    if (mtmElectionsMayorOfCityByRank(mtmElections,city,mayor,filename,NULL,
                                      NULL)==MTM_ELECTIONS_NO_CANDIDATES_IN_CITY){
        return MTM_ELECTIONS_NO_CANDIDATES_IN_CITY;
    }
    return printWinner(mtmElections,*mayor,filename);
}




/**
 *
 * @param mtmElections
 * @param cityId
 * @return
 * the city from the cities set
 * NULL if the city does not exist
 */
static City getCityById (MtmElections mtmElections, int cityId){
    SET_FOREACH(City ,current_city,mtmElections->cities){
        if (getCityId(current_city)==cityId){
            return current_city;
        }
    }
    return NULL;
}




int mtmElectionsRankByAge(MtmElections mtmElections, int citizen, void* pAge){
    if (!mtmElections || !pAge || citizen<0||*(int*)(pAge)==0){
        return 0;
    }
    int citizen_age;
    if( MtmElectionsCitizenGetAge(mtmElections, citizen, &citizen_age)==MTM_ELECTIONS_SUCCESS) {
        return (citizen_age/(*(int*)pAge));
    }
    return 0;
}




UniqueOrderedList mtmElectionsPerformElections(MtmElections mtmElections,
                                               RankFunc rank, void* auxilaryData, const char* filename){
    if (!mtmElections || (!auxilaryData) || !filename){
        return NULL;
    }
    UniqueOrderedList  winnerList = uniqueOrderedListCreate((copyElements)
            &copyCitizen, (freeElements)& freeCitizen, (elementsEquals)
            &winnerEquals, (elementGreaterThan) &compareByNameAndId);
    if (!winnerList){
        return NULL;
    }
    SET_FOREACH(City,current_city,mtmElections->cities){
        int mayor_id=0;
        mtmElectionsMayorOfCityByRank(mtmElections,current_city,&mayor_id,
                                      filename,rank,auxilaryData);
        Citizen mayor = NULL;
        mayor=  findCitizenById(mtmElections,mayor_id);
        if (!mayor) continue;
        uniqueOrderedListInsert(winnerList,mayor);
        freeCitizen(mayor);
    }
    UniqueOrderedList winnerIdList = uniqueOrderedListCreate((copyElements)
            &copyFunc,(freeElements) &freeFunc , (elementsEquals)
            &allwaysFalse, (elementGreaterThan)&allwaysTrue);
    if (!winnerIdList){
        uniqueOrderedListDestroy(winnerList);
        return NULL;
    }
    if (uniqueOrderedListSize(winnerList)==0){
        uniqueOrderedListDestroy(winnerList);
        return winnerIdList;
    }
    for (Citizen current_citizen = uniqueOrderedListGetLowest(winnerList);
         current_citizen; current_citizen=uniqueOrderedListGetNext(winnerList)){
        int citizen_id = getCitizenId(current_citizen);
        uniqueOrderedListInsert(winnerIdList,&citizen_id);
        printWinner(mtmElections,citizen_id,filename);
    }
    uniqueOrderedListDestroy(winnerList);
    return winnerIdList;
}






static MtmElectionsResult mtmElectionsMayorOfCityByRank
(MtmElections mtmElections, City city, int* mayor,
        const char* filename,RankFunc rank, void *auxilaryData){
    Set candidateSet = getCandidateSet(city);
    if (!setGetSize(candidateSet)){
        setDestroy(candidateSet); //**************
        return MTM_ELECTIONS_NO_CANDIDATES_IN_CITY;
    }
    Set citizenSet = getCitizenSet(city);
    SET_FOREACH(Citizen, current_citizen, citizenSet){
        if (getCitizenAge(current_citizen)<MIN_AGE_TO_VOTE && rank==NULL) {
            continue;
        }
        int favorite_citizen_id = getFavoriteCandidate (current_citizen);
        if (favorite_citizen_id < 0) continue;
        Citizen favorite = findCitizenById(mtmElections,favorite_citizen_id);
        if (!favorite) continue;
        SET_FOREACH(Citizen, current_candidate, candidateSet){
            if (!citizenCompare(favorite,current_candidate)){
                int vote_weight;
                if (!rank){
                    vote_weight=DEFAULT_VOTE_WEIGHT;
                }
                else{
                    vote_weight=rank(mtmElections,getCitizenId
                            (current_citizen),auxilaryData);
                }
                updateVotes(current_candidate,vote_weight);
            }
        }
        freeCitizen(favorite);
    }
    Citizen winner = setGetFirst(candidateSet);
    SET_FOREACH(Citizen,current_candidate,candidateSet){
        if (getCandidateVotes(winner)<getCandidateVotes(current_candidate)){
            winner=current_candidate;
        }
        else if (getCandidateVotes(winner)==getCandidateVotes
                (current_candidate)){
            if (compareByNameAndId(winner,current_candidate)){
                winner = current_candidate;
            }
        }
    }
    *mayor = getCitizenId(winner);
    setDestroy(citizenSet);
    setDestroy(candidateSet);
    return MTM_ELECTIONS_SUCCESS;
}


static MtmElectionsResult printWinner (MtmElections mtmElections,int mayor_id,
        const char* filename){
    Citizen mayor = findCitizenById(mtmElections,mayor_id);
    int mayor_age = getCitizenAge(mayor);
    int mayor_education = getCitizenEducation(mayor);
    char* name=NULL;
    MtmElectionsCitizenGetName(mtmElections,mayor_id,&name);
    int city_id = getCitizenCity(mayor);
    City city = getCityById(mtmElections,city_id);
    char* city_name=NULL;
    getCityName(city,&city_name);
    FILE* stream = fopen(filename, "a");
    if (!stream){
        mtmPrintFileError(filename,stream);
        free(name);
        free(city_name);
        freeCitizen(mayor);
        return MTM_ELECTIONS_FILE_ERROR;
    }
    mtmPrintMayorDetails(mayor_id,mayor_age,mayor_education, name,
                         city_name,city_id,stream);
    free(name);
    free(city_name);
    freeCitizen(mayor);
    fclose(stream);
    return MTM_ELECTIONS_SUCCESS;
}


/** unique ordered list functions **/

static bool allwaysFalse (int* first, int* second){
    if(*first==*second) return true;
    return false;
}

static bool allwaysTrue (int* first, int* second){
    if(*first==*second) return true;
    return true;
}

static bool winnerEquals (Citizen first, Citizen second){
    if (!first || !second){
        return false;
    }
    if (!citizenCompare(first,second)){
        return true;
    }
    else {
        return false;
    }
}


static int* copyFunc (int* number){
    int* new_number=malloc(sizeof(*new_number));
    if(!new_number) return NULL;
    *new_number= *number;
    return new_number;

}




static void freeFunc (int* number){
    free(number);
}


