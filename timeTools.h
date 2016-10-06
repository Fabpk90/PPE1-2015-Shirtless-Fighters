#ifndef TIMETOOLS_H_INCLUDED
#define TIMETOOLS_H_INCLUDED

#include "../graphic.h"

typedef struct myTime
{

    float startTime = 0;
    float endTime = 0;

    float deltaTime = 1;


    void Init()
    {
        startTime = Time();
        endTime = Time();
    }

    void update()
    {
        endTime = Time();

        deltaTime = endTime - startTime;

        startTime = Time();
        endTime = Time();
    }

} myTime;



#endif // TIMETOOLS_H_INCLUDED

