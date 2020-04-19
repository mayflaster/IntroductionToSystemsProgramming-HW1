//
// Created by mayfl on 27/11/2018.
//

#ifndef UNTITLED1_CITIZEN_H
#define UNTITLED1_CITIZEN_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "uniqueOrderedList/uniqueOrderedList.h"
#include "preference.h"



typedef struct citizen_t *        citizen;

typedef enum {
    CITIZEN_MEMORY_ERROR, CITIZEN_SUCCESS,CITIZEN_ALREADY_SUPPORT_CANDIDATE,
    CITIZEN_NULL_ARG,CITIZEN_PRIORITY_EXIST
} CitizenResult;


/** compare the cities id.
 * return values:
 * difference between the cities id.
 * 0 if this is the same citizen**/
int citizenCompare (Citizen first, Citizen second);


/** copy city to target city struct.
* return values:
* 	NULL if a NULL was sent or a memory allocation failed.
* 	A citizen containing the same elements.
 * **/
Citizen copyCitizen (Citizen copied_citizen);

/**
* Deallocates an existing citizen. Clears all elements by using the
* stored free function.
*/
void  freeCitizen (Citizen citizen);

/**
 * Allocates a new Citizen.
*
* Creates a new citizen. This function receives the citizen's details
 * and put it in the new citizen.
*
* return values:
* 	NULL - name is NULL or allocations failed.
* 	A new citizen in case of success.
 */
Citizen createCitizen (int id, const char* name, int age, int
yaers_of_education, int city_id);

/**
 * getCitizenId gets Citizen and returns the citizen id
 * return values:
 *  NULL - Citizen is NULL
 * 	the citizen's id in case of success.
 */
int getCitizenId (Citizen citizen);

/**
 * getCitizenName gets Citizen and a pointer to a string.
 * It copies the Citizen's name and place it in the pointer to the string.
 */

void getCitizenName (Citizen citizen,char ** name_copy);

/**
 * getCitizenCity gets Citizen and returns the city id
 * return values:
 *  NULL - Citizen is NULL
 * 	the city's id in case of success.
 */

int getCitizenCity (Citizen citizen);

/**
 * getCitizenAge gets Citizen and returns Citizen's age
 * return values:
 *  NULL - Citizen is NULL
 * 	the Citizen's age in case of success.
 */

int getCitizenAge (Citizen citizen);

/**
 * getCitizenEducation gets Citizen and returns Citizen's number years of education.
 * return values:
 *  NULL - Citizen is NULL
 * 	the Citizen's age in case of success.
 */

int getCitizenEducation (Citizen citizen);

/**
 * this function gets a citizen and change his city id.
 */
void changeCitizenCity (Citizen citizen, int cityId);

/**
 * this function add a preferences to citizen's list of preference
 *  * return:
    CITIZEN_NULL_ARG - if NULL argumants sent
    CITIZEN_ALREADY_SUPPORT_CANDIDATE - if there is a preference to this
    candidate
    CITIZEN_PRIORITY_EXIST - if there is already priority in the list of
    preferences of the citizen
    CITIZEN_MEMORY_ERROR - if the adding to the list of preference failed
    becouse of memory problems
    CITIZEN_SUCCESS - in case of successful adding

 */
CitizenResult addPriority (Citizen citizen, int candidateId, int priority);


/**
 * removePriority get Citizen and candidate Id
 * It removes from the citizen's listOfPreferences the priority for the candidate that has the same candidateId.
 * it returns:
 * UNIQUE_ORDERED_LIST_NULL_ARGUMENT- if citizen is NULL
 * UNIQUE_ORDERED_LIST_ITEM_DOES_NOT_EXIST - if citizen didn't support the candiddate
 * UNIQUE_ORDERED_LIST_SUCCESS- if the removed succeed
 */
UniqueOrderedListResult removePriority (Citizen citizen, int candidate_id);

/**
 * removes all the preferences from the citizen's list of preferences.
 * @param citizen to remove all his priorities.
 */
void clearPriorities (Citizen citizen);

/**
 *
 * @param citizen - to check his list of preferences
 * @return
 * id of most prefered candidate.
 * -1 (INVALID_INPUT) in case of NULL argument was sent to the function.
 */
int getFavoriteCandidate (Citizen citizen);


/**
 *
 * @param candidate - to add votes.
 * @param contribution - number of votes that being added
 * the function update the votes number the candidate got and
 * save the sum of them..
 */
void updateVotes (Citizen citizen ,int contribution);

/**
 * this functions get a candidate and check's how many votes he got's from
 * the citizen's in his city.
 * @param candidate
 * @return
 * the number of votes the candidate got.
 * -1 (INVALID_INPUT) in case of NULL argument was sent to the function.
 */
int getCandidateVotes (Citizen candidate);
/**
 *
 * @param first citizen to compare
 * @param second citizen to compare
 * @return
 * true if first citizen name is bigger lexicography than second
 * false if second citizen is bigger than first
 * in case the names are equal, if first's id is bigger return false.
 */
bool compareByNameAndId (Citizen first, Citizen second);



#endif //UNTITLED1_CITIZEN_H