/*
 * mtm_elections.h
 *
 *  Created on: Oct 24, 2018
 *      Author: ameeraam
 */

#ifndef MTM_ELECTIONS_H_
#define MTM_ELECTIONS_H_

#include "uniqueOrderedList/uniqueOrderedList.h"


typedef enum mtmElectionsResult_t{
    MTM_ELECTIONS_MEMORY_ERROR,
    MTM_ELECTIONS_NULL_ARGUMENT,
    MTM_ELECTIONS_ILLEGAL_ID,
    MTM_ELECTIONS_ILLEGAL_AGE,
    MTM_ELECTIONS_ILLEGAL_PRIORITY,
    MTM_ELECTIONS_ILLEGAL_NUMBER_OF_YEARS,
    MTM_ELECTIONS_CITIZEN_ALREADY_EXISTS,
    MTM_ELECTIONS_CITIZEN_DOES_NOT_EXIST,
    MTM_ELECTIONS_CANDIDATE_ALREADY_EXISTS,
    MTM_ELECTIONS_CANDIDATE_DOES_NOT_EXIST,
    MTM_ELECTIONS_CITY_ALREADY_EXISTS,
    MTM_ELECTIONS_CITY_DOES_NOT_EXIST,
    MTM_ELECTIONS_NOT_SAME_CITY,
    MTM_ELECTIONS_ALREADY_SUPPORTED,
    MTM_ELECTIONS_NOT_SUPPORTED,
    MTM_ELECTIONS_CAN_NOT_SUPPORT,
    MTM_ELECTIONS_MUST_SUPPORT,
    MTM_ELECTIONS_AGE_NOT_APPROPRIATE,
    MTM_ELECTIONS_PRIORITY_EXISTS,
    MTM_ELECTIONS_NO_CANDIDATES_IN_CITY,
    MTM_ELECTIONS_FILE_ERROR,
    MTM_ELECTIONS_SUCCESS
}MtmElectionsResult;

typedef struct mtm_elections_t* MtmElections;

typedef int (*RankFunc)(MtmElections, int, void*);

MtmElections mtmElectionsCreate();

void mtmElectionsDestroy(MtmElections mtmElections);

MtmElectionsResult mtmElectionsAddCity(MtmElections mtmElections, const char* cityName, int cityId);

MtmElectionsResult mtmElectionsAddCitizen(MtmElections mtmElections, const char* citizenName,
                                          int citizenId, int citizenAge, int yearsOfEducation, int cityId);

MtmElectionsResult MtmElectionsCitizenGetName(MtmElections mtmElections, int citizenId, char** name);

MtmElectionsResult MtmElectionsCitizenGetCity(MtmElections mtmElections, int citizenId, int* cityId);

MtmElectionsResult MtmElectionsCitizenGetAge(MtmElections mtmElections, int citizenId, int* age);

MtmElectionsResult MtmElectionsCitizenGetEducation(MtmElections mtmElections, int citizenId, int* yearsOfEducation);

MtmElectionsResult mtmElectionsAddCandidate(MtmElections mtmElections, int candidateId, int cityId);


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

MtmElectionsResult mtmElectionsWithdrawCandidate(MtmElections mtmElections, int candidateId, int cityId);

MtmElectionsResult mtmElectionsSupportCandidate(MtmElections mtmElections, int citizenId, int candidateId, int priority);
/**
 * mtmElectionsCancelSupport get mtmElections, citizen Id, candidate Id.
 * It will cancel the support of the citizen in the candidate that has the same candidate Id.
 * return values:
 * MTM_ELECTIONS_NULL_ARGUMENT- if mtmElections is a null argument
 * MTM_ELECTIONS_ILLEGAL_ID - if citizen Id or candidate Id are negative numbers
 * MTM_ELECTIONS_CITIZEN_DOES_NOT_EXIST- if citizen not exist
 * MTM_ELECTIONS_CANDIDATE_DOES_NOT_EXIST- if candidate not exist.
 * MTM_ELECTIONS_MUST_SUPPORT- if the citizen is the candidate.
 *CITIZEN_NOT_SUPPORT_CANDIDATE- if the citizen didn't support the candidate
 * MTM_ELECTIONS_SUCCESS- if succeed.
 */

MtmElectionsResult mtmElectionsCancelSupport(MtmElections mtmElections, int citizenId, int candidateId);
/**
 *
 * @param mtmElections
 * @param citizenId
 * @param cityId
 * @return
 */
MtmElectionsResult mtmElectionsChangeAddress(MtmElections mtmElections, int citizenId, int cityId);


/**
 * This function get mtmElections struct, citizenId and a Pointer Page that point to minimal age.
 * mtmElectionsRankByAge calculate the contribution of each citizen to his favorite candidate by the
 * formula citizen Contribution = ROUND_DOWN(age/minimal age)
 * return the contribution if succeed
 * return 0 if there is an error (Id is ILLEGAL or if CITIZEN_DOES_NOT_EXIST, Parameters are not proper )
 *
 */
int mtmElectionsRankByAge(MtmElections mtmElections, int citizen, void* pAge);
/**
 *
 * @param mtmElections
 * @param rank
 * @param auxilaryData
 * @param filename
 * @return
 */
UniqueOrderedList mtmElectionsPerformElections(MtmElections mtmElections, RankFunc rank, void* auxilaryData, const char* filename);
/**
 *
 * @param mtmElections
 * @param cityId
 * @param mayor
 * @param filename
 * @return
 */
MtmElectionsResult mtmElectionsMayorOfCity(MtmElections mtmElections, int cityId, int* mayor, const char* filename);




#endif /* MTM_ELECTIONS_H_ */
