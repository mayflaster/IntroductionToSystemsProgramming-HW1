//
// Created by mayfl on 27/11/2018.
//

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "citizen.h"

#define INVALID_INPUT -1


struct citizen_t {
    char* name;
    int id;
    int years_of_education;
    int age;
    int city_id;
    UniqueOrderedList listOfPreferences;
    int votes;
};

/**
 *
 * @param citizen - the citizen that his preference list is being checked
 * @param priority - the priority that the function checks if exist.
 * @return
 * true if the citizen already has this priority in his preferences list.
 * false otherwise.
 */
static  bool isPriorityExist (Citizen citizen, int priority);



int citizenCompare (Citizen first, Citizen second){

    if (!first || !second){
        return INVALID_INPUT;
    }
    int difference = first->id - second->id;
    return difference;
}

Citizen copyCitizen (Citizen copied_citizen){
    if (!copied_citizen){
        return NULL;
    }
    Citizen new_citizen = createCitizen(copied_citizen->id,
                                        copied_citizen->name,copied_citizen->age,
                                        copied_citizen->years_of_education,copied_citizen->city_id);
    uniqueOrderedListDestroy(new_citizen->listOfPreferences);//***********
    new_citizen->listOfPreferences = uniqueOrderedListCopy(copied_citizen->listOfPreferences);
    if (!new_citizen->listOfPreferences){
        free(new_citizen);
        return NULL;
    }
    return new_citizen;
}

void  freeCitizen (Citizen citizen){
    if (!citizen) {
        return;
    }
    free(citizen->name);
    uniqueOrderedListDestroy(citizen->listOfPreferences);
    free (citizen);
}

Citizen createCitizen (int id,const char* name, int age, int
years_of_education,int city_id) {
    if (!name) return NULL;
    Citizen new_citizen = malloc(sizeof(*new_citizen));
    if (!new_citizen){
        return NULL;
    }
    new_citizen->listOfPreferences = uniqueOrderedListCreate((copyElements) &copyPreferences,
                                                             (freeElements) &freePreferences,
                                                             (elementsEquals) &candidateIdEquals, (elementGreaterThan) &priorityGreaterThan);
    if (!new_citizen->listOfPreferences) {
        free(new_citizen);
        return NULL;
    }
    new_citizen->id = id;
    new_citizen->name = malloc(strlen(name)+1);
    if (!new_citizen->name){
        uniqueOrderedListDestroy(new_citizen->listOfPreferences);
        free(new_citizen);
        return NULL;
    }
    strcpy(new_citizen->name,name);
    new_citizen->years_of_education = years_of_education;
    new_citizen->age = age;
    new_citizen->city_id = city_id;
    new_citizen->votes=0;
    return new_citizen;
}

int getCitizenId (Citizen citizen){
    if (!citizen){
        return INVALID_INPUT;
    }
    int id = citizen->id;
    return id;
}


void getCitizenName (Citizen citizen,char ** name_copy){
    if(!citizen||!name_copy) return;
    int len = strlen(citizen->name)+1;
    *name_copy = malloc(sizeof(char)*len);
    strcpy (*name_copy, citizen->name);
}

int getCitizenCity (Citizen citizen){
    if(!citizen) {
        return INVALID_INPUT;
    }
    int city = citizen->city_id;
    return city;
}


int getCitizenAge (Citizen citizen){
    if(!citizen) {
        return INVALID_INPUT;
    }
    int age = citizen->age;
    return age;
}

int getCitizenEducation (Citizen citizen){
    if(!citizen) {
        return INVALID_INPUT;
    }
    int education = citizen->years_of_education;
    return  education;
}

void changeCitizenCity (Citizen citizen, int cityId){
    if (!citizen){
        return;
    }
    citizen->city_id=cityId;
}


CitizenResult addPriority (Citizen citizen, int candidateId, int priority){
    if (!citizen){
        return CITIZEN_NULL_ARG;
    }
    bool priority_exist = isPriorityExist(citizen,priority);
    Preference preference  = createPreference(candidateId, priority);
    UniqueOrderedListResult insert_result = uniqueOrderedListInsert
            (citizen->listOfPreferences, preference);
    freePreferences(preference);
    if (insert_result ==UNIQUE_ORDERED_LIST_ITEM_ALREADY_EXISTS ){
        return  CITIZEN_ALREADY_SUPPORT_CANDIDATE;
    }
    if (priority_exist){
        removePriority(citizen,candidateId);
        return CITIZEN_PRIORITY_EXIST;
    }
    else if (insert_result ==UNIQUE_ORDERED_LIST_NULL_ARGUMENT){
        return CITIZEN_NULL_ARG;
    }
    else if (insert_result == UNIQUE_ORDERED_LIST_OUT_OF_MEMORY){
        return CITIZEN_MEMORY_ERROR;
    }
    return CITIZEN_SUCCESS;

}



 static bool isPriorityExist (Citizen citizen, int priority){
    if (!citizen){
        return false;
    }
    for (Preference iterator = uniqueOrderedListGetLowest(citizen->listOfPreferences)
            ; iterator; iterator = uniqueOrderedListGetNext
            (citizen->listOfPreferences)){
        if (getPriority(iterator) == priority){
            return true;
        }
    }
    return false;
}


void clearPriorities (Citizen citizen){
    if(!citizen || !citizen->listOfPreferences){
        return;
    }
    uniqueOrderedListClear(citizen->listOfPreferences);
}


int getFavoriteCandidate (Citizen citizen){
    if (!citizen){
        return INVALID_INPUT;
    }
    Preference best_preference = uniqueOrderedListGetLowest
            (citizen->listOfPreferences);
    if (!best_preference){
        return INVALID_INPUT;
    }
    int favorite_candidate_id = getFavorite(best_preference);
    return favorite_candidate_id;
}


void updateVotes (Citizen candidate ,int contribution){
    if (!candidate){
        return;
    }
    candidate->votes+=contribution;
}


int getCandidateVotes (Citizen candidate){
    if (!candidate){
        return INVALID_INPUT;
    }
    int votes = candidate->votes;
    return votes;
}



UniqueOrderedListResult removePriority (Citizen citizen, int candidate_id){
    if (!citizen){
        return UNIQUE_ORDERED_LIST_NULL_ARGUMENT;
    }
    return uniqueOrderedListRemove
            (citizen->listOfPreferences,&candidate_id);
}




bool compareByNameAndId (Citizen first, Citizen second){
    if (!first ||!second){
        return false;
    }
    int difference = strcmp(first->name, second->name);
    if (difference < 0){
        return false;
    }
    else if (difference > 0){
        return true;
    }
    else if (first->id < second->id){
        return true;
    }
    return false;
}
