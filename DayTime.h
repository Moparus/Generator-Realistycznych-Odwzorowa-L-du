#pragma once
#include <iostream>
#include <string>

using namespace std;

class DayTime
{
private:
    int minutes = 0;
    int hours = 23;
public:
    DayTime() {}
    string getDayTime()
    {
        if(hours < 10 && minutes < 10) return "0" + to_string(hours) + ":" + "0"+to_string(minutes);
        if (hours < 10) return "0" + to_string(hours) + ":" + to_string(minutes);
        if (minutes < 10) return to_string(hours) + ":" + "0" + to_string(minutes);
        return to_string(hours) + ":" + to_string(minutes);
    }
    void setDayTime(int m)
    {
        minutes += m;
        while (minutes > 59)
        {
            minutes -= 60;
            hours++;
        }
        while (hours > 23)
        {
            hours -= 24;
        }
    }
    int getDarkness()
    {
        int sumMinutes = hours * 60 + minutes;
        if (sumMinutes > 1080) {
            return (int)(sumMinutes - 1080) * 0.41666;
        }
        if (sumMinutes < 360) {
            return (int)(150 - (sumMinutes * (0.41666)));
        }
        return 0;  
    }
};