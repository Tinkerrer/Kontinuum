//
// Created by arty on 12.06.2022.
//

#ifndef KONTINUUM_STUDENT_H
#define KONTINUUM_STUDENT_H

#include <Lesson.h>
#include <string>
#include <vector>
#include <map>

class Student {
private:
    //Имя студента
    std::string name;

    //Карта с названием предмета и результатом последнего пробника
    std::map<std::string, size_t> subject_last_result;

    //Вектор посещенный занятий
    std::vector<Lesson> lessons;
public:
    explicit Student(const std::string &new_student_name);

    Student(const Student &old_student) = default;

    //Добавление результата последнего пробника студента
    void add_subject_last_result(const std::string &subject_name, size_t result);

    //Добавление посещенного занятия в профиль студента
    void add_lesson(const Lesson &new_lesson);

    //Геттер для имени студента
    std::string get_name() const;

    //Функция расчета итогового балла
    float calculate_mark() const;
};

#endif //KONTINUUM_STUDENT_H
