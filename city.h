//
// Created by mayfl on 27/11/2018.
//

#ifndef UNTITLED1_CITY_H
#define UNTITLED1_CITY_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include "citizen.h"
#include "set.h"
#include "uniqueOrderedList/uniqueOrderedList.h"


typedef struct city_t* City;


/** compare the cities id.
 * return values:
 * difference between the citied id.
 * 0 if this is the same city**/
int citiesCompare (City first, City second);

/** copy city to target city struct.
* return values:
* 	NULL if a NULL was sent or a memory allocation failed.
* 	A city containing the same elements.
 * **/
City copyCity (City copied_city);

/**
* Deallocates an existing city. Clears all elements by using the
* stored free function.
*/
void  freeCity (City city);

/**
*
* Creates a new citizen. This function receives the city's details.
*
* return values:
* 	NULL - name is NULL or allocations failed.
* 	A new city in case of success.
 */
City createCity (int id, const char* name);

/**
 * this function gets a city and returns a copy of it's citizen set.
 * returns NULL in case of error or NULL argument.
 */
Set getCitizenSet (City city);

/**
 *
 * this function gets a city and returns the id of the city
 * -1 (INVALID_INPUT) in case of NULL argumant
 */
int getCityId (City city);
/**
 *
 * this function gets an id and a city and search the citizen in the city.
 * returns the citizen struct.
 * it returns NULL id there is no citizen with this id in the city.
 */
Citizen getCitizenById (int CitizenId, City city);
/**
 * this function add a citizen to a city's set of citizens.
 * @param city - the city to add the citizen to.
 * @param citizen - the citizen to insert to the citizen set
 * * @return
* 	SET_NULL_ARGUMENT if a NULL was sent as set
* 	SET_OUT_OF_MEMORY if an allocation failed (Meaning the function for copying
* 	an element failed)
*  SET_ITEM_ALREADY_EXISTS if an equal citizen already exists in the set
* 	SET_SUCCESS the citizen has been inserted successfully
 */
SetResult updateCitizensSet(City city ,Citizen  citizen);


/**
 * this function add a candidate to a city's set of candidates.
 * @param city - the city to add the candidates to.
 * @param candidate - the candidate to insert to the candidates set
 * * @return
* 	SET_NULL_ARGUMENT if a NULL was sent as set
* 	SET_OUT_OF_MEMORY if an allocation failed (Meaning the function for copying
* 	an element failed)
*  SET_ITEM_ALREADY_EXISTS if an equal candidate already exists in the set
* 	SET_SUCCESS the candidate has been inserted successfully
 */
SetResult updateCandidateSet (City city ,Citizen  candidate);

/**
 *this function remove a candidate from a city's set of candidate's
 * @param city - the city to remove the candidate from.
 * @param candidateId - the id of the candidate that will be removed from the
 * set.
 * * @return
* 	SET_NULL_ARGUMENT if a NULL was sent as set
* 	SET_ITEM_DOES_NOT_EXIST if the candidate doesn't exist in the set
* 	SET_SUCCESS if the candidate was successfully removed.
 */
SetResult removeFromCandidateSet (City city, Citizen candidate);

/**
 *
 * @param
 * the current city of the citizen.
 * @param citizen
 * the citizen that leave the city.
 * @return
 * SET_SUCCESS - if the citizen's set updated successfully
 * SET_NULL_ARG - if a NULL is sent to the function
 * SET_ITEM_DOES_NOT_EXIST if we try to remove citizen than don't belong to the
 * city
 */
SetResult removeFromCitizensSet (City city ,Citizen  citizen);

/**
 * this function gets a city and returns a copy of it's candidates set.
 * returns NULL in case of error or NULL argument.
 */
Set getCandidateSet (City city);

/**
 * getCityName gets a city and a string pointer and copy the city name into
 * the target string.
 */
void  getCityName (City city, char** name_copy);
/**
 * this functions gets city and candidateId, and removes to all the citizens
 * in this city the preference to this candidate (id exist).
 */
void removePriorityToAllCitizens (City city, int candidateId);
/**
 * this function gets a city,citizen,candidate Id and priority, and adds to
 * the citizen's list of preferences the new preference to the candidate
 * return:
    CITIZEN_NULL_ARG - if NULL argumants sent
    CITIZEN_ALREADY_SUPPORT_CANDIDATE - if there is a preference to this
    candidate
    CITIZEN_PRIORITY_EXIST - if there is already priority in the list of
    preferences of the citizen
    CITIZEN_MEMORY_ERROR - if the adding to the list of preference failed
    becouse of memory problems
    CITIZEN_SUCCESS - in case of successful adding

 */

CitizenResult addPriorityForCandidateFromTheCity (City city, Citizen citizen, int
candidateId, int priority);

/**
 * this function gets a city, citizen, and candidate is and clear the
 * preference for the candidate, if the citizen prefered before.
 * @return
 *  UNIQUE_ORDERED_LIST_NULL_ARGUMENT - one of the arguments passed is NULL
 * UNIQUE_ORDERED_LIST_ITEM_DOES_NOT_EXIST - there is no priority which is equal
 * 											 to the passed element
 * UNIQUE_ORDERED_LIST_SUCCESS - in case of successful removal.
 */
UniqueOrderedListResult removePriorityForCandidateFromTheCity (City city, Citizen citizen,
                                                               int candidateId);
/**
 * this function gets a city and a citizen and clear all his preferences.
 */
void clearAllPrioritiesToCitizenFromTheCity (City city,Citizen citizen);
/**
 *
 * this function gets a citizen and a city and check if the citizen is a
 * belong to that city (check if part of the citizen set).
 * @return
 * true if the citizen is belong to the city
 * false otherwise (in case of NULL argumant or if the citizen is not part of
 * the city).
 * */
bool isCitizenInThisCity (City city, Citizen citizen);
/**
 *
 * this function gets a citizen and a city and check if the citizen is a
 * candidate in that city.
 * @return
 * true if the citizen is a candidate in the city
 * false otherwise (in case of NULL argumant or if the citizen is not
 * candidate).
 * */
bool isCandidateInThisCity (City city, Citizen candidate);

#endif //UNTITLED1_CITY_H
