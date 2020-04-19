//
// Created by mayfl on 05/12/2018.
//

#include "preference.h"
#define INVALID_INPUT -1

struct prefernce_t{
    int candidateId;
    int priority;
};

int getFavorite (Preference preference){
    if (!preference){
      return INVALID_INPUT;
    }
    int favorite = preference->candidateId;
    return favorite;
}

Preference copyPreferences(Preference d){
    Preference new_data_of_preferences = malloc(sizeof(*new_data_of_preferences));
    if(!new_data_of_preferences){
        return NULL;
    }
    *new_data_of_preferences = *((Preference)d);
    return new_data_of_preferences;
}

void freePreferences(Preference d){
    if (!d){
        return;
    }
    free(d);
}

bool candidateIdEquals(Preference d1, Preference d2){
    return d1->candidateId == d2->candidateId;
}

bool priorityGreaterThan(Preference d1, Preference d2){
    return d1->priority > d2->priority;
}

Preference createPreference(int candidateId,int priority){
    Preference preference= malloc(sizeof(*preference));
    if(!preference) {
        return NULL;
    }
    preference->priority = priority;
    preference->candidateId =candidateId;
    return preference;

}


int getPriority (Preference preference){
    if (!preference){
        return INVALID_INPUT;
    }
    int priority = preference->priority;
    return priority;
}