//
// Created by arty on 12.06.2022.
//

#ifndef KONTINUUM_LESSON_H
#define KONTINUUM_LESSON_H

#include <string>

struct Lesson {
    //Название предмета
    std::string subject_name;

    //Дата предмета
    std::string date;

    //Активность на занятии
    size_t lesson_activity;

    //Процент выполнения дз
    size_t homework_percent;
};

#endif //KONTINUUM_LESSON_H
