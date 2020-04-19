//
// Created by mayfl on 27/11/2018.
//

#include <stdlib.h>
#include "city.h"
#include <string.h>

#include "set.h"
Citizen findOriginalCitizen (City city, Citizen citizen);
#define MIN_CANDIDATE_AGE 21
#define INVALID_INPUT -1


struct city_t {
    int city_id;
    char* name;
    Set citizens;
    Set candidates;
};


City createCity (int id, const char* name){
    if ( !name || id<0 ){
        return NULL;
    }
    City new_city = malloc(sizeof(*new_city));
    if (!new_city){
        return NULL;
    }
    new_city->city_id=id;
    new_city->name = malloc(strlen(name)+ 1);
    if (!new_city->name){
        free(new_city);
        return NULL;
    }
    strcpy(new_city->name,name);
    new_city->candidates = setCreate((copySetElements)&copyCitizen,(freeSetElements)
                                             &freeCitizen, (compareSetElements)&citizenCompare);
    if (!new_city->candidates){
        free (new_city);
        return NULL;
    }
    new_city->citizens = setCreate((copySetElements)&copyCitizen,(freeSetElements)
            &freeCitizen, (compareSetElements)&citizenCompare);
    if(!new_city->citizens){
        setDestroy(new_city->candidates);
        free(new_city);
        return NULL;
    }
    return (new_city);

}

int citiesCompare (City first, City second){
    if (!first || !second){
        return INVALID_INPUT;
    }
    int difference = first-> city_id - second->city_id;
    return difference;
}

City copyCity (City copied_city){
    if (!copied_city){
        return NULL;
    }
    City new_city = createCity(copied_city->city_id,copied_city->name);
    if (!new_city){
        return NULL;
    }
    setDestroy(new_city->candidates);
    setDestroy(new_city->citizens);
    new_city->candidates = setCopy(copied_city->candidates);
    if (!new_city->candidates){
        free(new_city);
        return NULL;
    }
    new_city->citizens = setCopy(copied_city->citizens);
    if (!new_city->citizens){
        setDestroy(new_city->candidates);
        free(new_city);
        return NULL;
    }
    return new_city;
}

void  freeCity (City city){
    if (!city) {
        return;
    }
    free(city->name);
    setDestroy(city->citizens);
    setDestroy(city->candidates);
    free(city);
}




Set getCitizenSet (City city){
    if (!city || !city->citizens){
        return NULL;
    }
    Set citizenCopy = setCopy(city->citizens);
    return citizenCopy;
}

Set getCandidateSet (City city){
    if  (!city || !city->candidates){
        return NULL;
    }
    Set candidateCopy = setCopy(city->candidates);
    return  candidateCopy;
}

int getCityId (City city){
    if (!city){
        return INVALID_INPUT;
    }
    int id = city->city_id;
    return id;
}


SetResult updateCitizensSet(City city ,Citizen  citizen){
    if (!city){
        return SET_NULL_ARGUMENT;
    }
    return setAdd(city->citizens,citizen);
}



SetResult updateCandidateSet (City city ,Citizen  candidate){
    if (!city){
        return SET_NULL_ARGUMENT;
    }
    Citizen candidate_copy = copyCitizen(candidate);
    SetResult result= setAdd(city->candidates,candidate_copy);
    freeCitizen(candidate_copy);
    return result;
}




Citizen getCitizenById (int CitizenId, City city){
    if (!city){
        return NULL;
    }
    Citizen new_citizen = NULL;

    SET_FOREACH(Citizen, current_citizen, city->citizens){
        if(getCitizenId(current_citizen)== CitizenId){
            new_citizen = copyCitizen(current_citizen);
            if (new_citizen==NULL){
                return NULL;
            }
            else{
                return new_citizen;
            }
        }
    }
    return NULL;
}


SetResult removeFromCandidateSet (City city, Citizen candidate){
    if (!city ){
        return SET_NULL_ARGUMENT;
    }
    return setRemove(city->candidates,candidate);
}




SetResult removeFromCitizensSet (City city ,Citizen  citizen) {
    clearPriorities(citizen);
    return setRemove(city->citizens,citizen);

}





void  getCityName (City city, char** name){
    if (!city || !name){
        return ;
    }
    int len = strlen(city->name)+1;
    *name=malloc(sizeof(char)*len);
    strcpy (*name, city->name);

}




void removePriorityToAllCitizens (City city, int candidateId){
    if (!city){
        return;
    }
    SET_FOREACH(Citizen,current_citizen,city->citizens){
        removePriority(current_citizen,candidateId);
    }

}



CitizenResult addPriorityForCandidateFromTheCity (City city, Citizen citizen,
                                                  int candidateId, int priority){
    if (!citizen || !city){
        return CITIZEN_NULL_ARG;
    }
    Citizen original_citizen = findOriginalCitizen(city,citizen);
    CitizenResult add_result = addPriority(original_citizen,candidateId,priority);
    return add_result;
}

/**
 *
 * @param city
 * @param citizen
 * @return the citizen struct from city's set and not a copy of it.
 NULL if citizen doesn't exist.
 for internal use only! (not outside city.c)
 */
Citizen findOriginalCitizen (City city, Citizen citizen){
    SET_FOREACH(Citizen, current_citizen, city->citizens){
        if (!citizenCompare(citizen,current_citizen)){
            return current_citizen;
        }
    }
    return NULL;
}




UniqueOrderedListResult removePriorityForCandidateFromTheCity (City city, Citizen citizen,
                                                               int candidateId){
    if (!citizen || !city){
        return UNIQUE_ORDERED_LIST_NULL_ARGUMENT;
    }
    SET_FOREACH(Citizen,current_citizen,city->citizens){
        if (!citizenCompare(current_citizen,citizen)){
            return removePriority(current_citizen,candidateId);
        }
    }
    return UNIQUE_ORDERED_LIST_ITEM_DOES_NOT_EXIST; // Should never reach
    // here - becouse we already check that the citizen is in the city.

}


void clearAllPrioritiesToCitizenFromTheCity (City city,Citizen citizen){
    SET_FOREACH(Citizen,current_citizen, city->citizens){
        if (!citizenCompare(current_citizen,citizen)){
            clearPriorities(current_citizen);
        }
    }
}


bool isCitizenInThisCity (City city, Citizen citizen){
    if (!city || !citizen||!city->citizens){
        return false;
    }
    if (setIsIn(city->citizens,citizen)){
        return true;
    }
    return false;
}


bool isCandidateInThisCity (City city, Citizen candidate){
    if (!city || !candidate || !city->candidates){
        return false;
    }
    if (setIsIn(city->candidates,candidate)){
        return true;
    }
    return false;
}