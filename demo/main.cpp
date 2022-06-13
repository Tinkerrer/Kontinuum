//
// Created by arty on 12.06.2022.
//

#include <Student.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <nlohmann/json.hpp>
#include <boost/program_options.hpp>
#include <exception>
#include <regex>

using namespace boost::program_options;
using nlohmann::json;

//Функция для конвертации даты в число
size_t convert_date_to_number(const std::string &date) {
    std::regex data_fromat("[0-9]{2}.[0-9]{2}.[0-9]{4}");
    if (!std::regex_match(date, data_fromat)) {
        throw std::logic_error("wrong data format");
    }
    size_t equivalent_number = 0;
    size_t day = std::stoi(date.substr(0, 2));
    size_t month = std::stoi(date.substr(date.find('.') + 1, 2));
    size_t year = std::stoi(date.substr(date.rfind('.') + 1));

    equivalent_number += day;
    switch (month) {
        case 1:
        case 3:
        case 5:
        case 7:
        case 10:
        case 12:
            equivalent_number += month * 31;
            break;
        case 2:
            equivalent_number += month * 28;
            break;
        case 4:
        case 6:
        case 9:
        case 11:
            equivalent_number += month * 30;
            break;
    }
    equivalent_number += year * 365;
    return equivalent_number;
}

int main(int argc, char *argv[]) {
    try {
        //Команда запуска программы с параметрами на linux
        //./demo --path="path_to_file" --id=1(по умолчанию 0, означает всех) --week=18.10.2021(первый день недели)

        //Обработка параметров запуска программы
        options_description desc{"Options"};
        desc.add_options()("path", value<std::string>()->required(), "path to database")
                ("week", value<std::string>()->required(), "week to be calculated")
                ("id", value<size_t>()->default_value(0), "students id");

        variables_map vm;
        store(parse_command_line(argc, argv, desc), vm);
        notify(vm);

        //Открываем и читаем базу данных, все содержимое хранится в формате json
        std::string jsonPath(vm["path"].as<std::string>());
        json data;

        //Читаем и парсим фрагмент бд со студентами
        std::ifstream file{jsonPath + "/students.json"};
        if (!file) {
            throw std::runtime_error{"unable to open database fragment: " + jsonPath};
        }

        file >> data;
        if (!data.is_array()) {
            throw std::logic_error("\"students.json\" has wrong form");
        }
        file.close();

        std::map<size_t, Student> students;
        for (auto const &item: data) {
            //Либо выводятся все студенты, либо студент с определенным id
            if (vm["id"].as<size_t>() == 0 || vm["id"].as<size_t>() == item["id"]) {
                students.insert(make_pair(item["id"], Student(item["name"])));
            }
        }
        data.clear();

        //Читаем и парсим фрагмент бд с последними оценками за пробник
        file.open(jsonPath + "/subject_results.json");
        if (!file) {
            throw std::runtime_error{"unable to open database fragment: " + jsonPath};
        }

        file >> data;
        if (!data.is_array()) {
            throw std::logic_error("\"subject_results.json\" has wrong form");
        }
        file.close();

        for (auto const &item: data) {
            //Либо выводятся все студенты, либо студент с определенным id
            if (vm["id"].as<size_t>() == 0 || vm["id"].as<size_t>() == item["id"]) {
                students.at(item["id"]).add_subject_last_result(item["subject"], item["result"]);
            }
        }
        data.clear();

        //Читаем и парсим фрагмент бд с посещенными занятиями и результатами этих занятий
        file.open(jsonPath + "/lessons.json");
        if (!file) {
            throw std::runtime_error{"unable to open database fragment: " + jsonPath};
        }

        file >> data;
        if (!data.is_array()) {
            throw std::logic_error("\"lessons.json\" has wrong form");
        }
        file.close();

        for (auto const &item: data) {
            //Отсекаются занятия нерассматриваеымых студентов
            if (!(vm["id"].as<size_t>() == 0 || vm["id"].as<size_t>() == item["id"])) {
                continue;
            }
            //Отсекаются занятия, не попавшие в нужную неделю
            size_t week_equivalent_number = convert_date_to_number(vm["week"].as<std::string>());
            size_t lesson_date_equivalent_number = convert_date_to_number(item["date"]);
            if (!(lesson_date_equivalent_number >= week_equivalent_number
                  && lesson_date_equivalent_number < week_equivalent_number + 7)) {
                continue;
            }
            Lesson new_lesson;
            new_lesson.subject_name = item["subject"];
            new_lesson.date = item["date"];
            new_lesson.lesson_activity = item["activity"];
            new_lesson.homework_percent = item["homework"];
            students.at(item["id"]).add_lesson(new_lesson);
        }
        data.clear();

        //Расчет и вывод результатов
        //по факту, map students уже готовый обработанный контейнер, содержащий в себе всю
        //информацию о студентах для конкретной недели. Если данную программу прикручивать к чему-то,
        //достаточно просто передать этот контейнер
        for (const auto &student: students) {
            std::cout << student.second.get_name() << " : " << student.second.calculate_mark() << std::endl;
        }
    }
    catch (const std::logic_error &e) {
        std::cerr << std::endl << e.what() << std::endl;
        return 2;
    }
    catch (const std::runtime_error &e) {
        std::cerr << std::endl << e.what() << std::endl;
        return 3;
    }
}
