#include <Student.h>

#include <utility>

Student::Student(const std::string &new_student_name) : name(new_student_name) {}

void Student::add_subject_last_result(const std::string &subject_name, size_t result) {
    subject_last_result.insert(std::make_pair(subject_name, result));
}

void Student::add_lesson(const Lesson &new_lesson) {
    lessons.push_back(new_lesson);
}

std::string Student::get_name() const {
    return name;
}

float Student::calculate_mark() const {
    float calculated_mark = 0;
    if (lessons.empty()) {
        return 0;
    }
    for (auto &lesson: lessons) {
        float current_lesson_mark = 0;

        //Оценка домашнего задания
        if (lesson.homework_percent == 100) current_lesson_mark += 3;
        else {
            if (lesson.homework_percent >= 80) current_lesson_mark += 2;
            else {
                if (lesson.homework_percent >= 50) current_lesson_mark += 1;
                else {
                    if (lesson.homework_percent < 30) current_lesson_mark -= 0.5;
                }
            }
        }

        //Оценка активности на занятии
        if (lesson.lesson_activity >= 4) current_lesson_mark += 1;
        else {
            if (lesson.lesson_activity >= 2) current_lesson_mark += 0.5;
        }

        //Доп баллы за результат пробника и посещения занятия
        current_lesson_mark += 0.02f * subject_last_result.at(lesson.subject_name);

        calculated_mark += current_lesson_mark;
    }
    calculated_mark /= lessons.size();
    return calculated_mark;
}






