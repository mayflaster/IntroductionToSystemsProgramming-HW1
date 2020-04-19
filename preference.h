//
// Created by mayfl on 05/12/2018.
//

#ifndef HW1B_15_PREFERENCE_H
#define HW1B_15_PREFERENCE_H


#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>


typedef struct prefernce_t* Preference;

Preference copyPreferences(Preference preference);

void freePreferences(Preference preference);

bool candidateIdEquals(Preference d1, Preference d2);

Preference createPreference(int candidateId,int priority);

int getPriority (Preference preference);


int getFavorite (Preference preference);

bool priorityGreaterThan(Preference d1, Preference d2);


#endif //HW1B_15_PREFERENCE_H
