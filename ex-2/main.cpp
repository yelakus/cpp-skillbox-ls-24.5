#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <ctime>
#include <iomanip>
#include <limits>

struct person
{
    std::string name;
    std::string birthday;
};

std::string get_input()
{
    std::string input;
    std::getline(std::cin, input);
    return input;
}

bool is_date_valid(int year, int month, int day) {
    if (year < 1900 || month < 1 || month > 12) return false;

    std::vector<int> days_in_month = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) days_in_month[1] = 29; // високосный год

    return day >= 1 && day <= days_in_month[month - 1];
}

bool is_birthday_input_correct(std::string birthday) {
    std::stringstream ss(birthday);
    std::string item;
    std::vector<int> date_parts;

    while (std::getline(ss, item, '/')) {
        try {
            date_parts.push_back(std::stoi(item));
        } catch (...) {
            return false; // не удалось преобразовать строку в число
        }
    }

    if (date_parts.size() != 3) return false; // не больше 3х элементов

    int year = date_parts[0];
    int month = date_parts[1];
    int day = date_parts[2];

    if (!is_date_valid(year, month, day)) return false;

    // проверяю, чтобы др не оказался в будущем
    std::time_t now = std::time(nullptr);
    std::tm* local_time = std::localtime(&now);

    int current_year = local_time->tm_year + 1900;
    int current_month = local_time->tm_mon + 1;
    int current_day = local_time->tm_mday;

    if (year > current_year) return false;
    if (year == current_year && month > current_month) return false;
    if (year == current_year && month == current_month && day > current_day) return false;

    return true;
}

void find_nearest_birthday(const std::vector<person>& list_of_persons) {
    if (list_of_persons.empty()) {
        std::cout << "No bithdays" << std::endl;
        return;
    }

    std::time_t now = std::time(nullptr);
    std::tm* local_time = std::localtime(&now);

    int current_month = local_time->tm_mon + 1;
    int current_day = local_time->tm_mday;

    std::vector<std::string> todays_birthdays;
    std::pair<int, person> nearest_birthday = {366, {}};

    for (const auto& person : list_of_persons) {
        std::stringstream ss(person.birthday);
        int year, month, day;
        char delimiter;

        ss >> year >> delimiter >> month >> delimiter >> day;

        std::tm birthday_tm = *local_time;
        birthday_tm.tm_mon = month - 1;
        birthday_tm.tm_mday = day;

        std::time_t birthday_time = std::mktime(&birthday_tm);
        if (birthday_time == -1) continue;

        if (month == current_month && day == current_day) {
            todays_birthdays.push_back(person.name);
            continue;
        }

        if (birthday_time < now) {
            // если др уже прошел в этом году, считаем дни до следующего др в следующем году
            birthday_tm.tm_year += 1;
            birthday_time = std::mktime(&birthday_tm);
        }

        int days_until_birthday = std::difftime(birthday_time, now) / (60 * 60 * 24);
        if (days_until_birthday < nearest_birthday.first) {
            nearest_birthday = {days_until_birthday, person};
        }
    }

    if (!todays_birthdays.empty()) {
        std::cout << "Today's birthdays:" << std::endl;
        for (const auto& name : todays_birthdays) {
            std::cout << "* " << name << std::endl;
        }
    }

    if (nearest_birthday.first != std::numeric_limits<int>::max()) {
        std::cout << "The nearest birthday is in " << nearest_birthday.first << " days: " << nearest_birthday.second.name << " (" << nearest_birthday.second.birthday << ")" << std::endl;
    }
}

int main(){
    person current_person;
    std::vector<person> list_of_persons;
    std::cout << "Enter information about your friends:" << std::endl;

    while(true){
        std::cout << "Enter the person\'s name(enter \"end\" to finish):" << std::endl;
        std::string name = get_input();
        if (name == "end") break;

        std::cout << "Enter " + name + "\'s birthday(year/month/day):" << std::endl;
        std::string birthday = get_input();
        if (birthday == "end") break;
        if (!is_birthday_input_correct(birthday)) {
            std::cout << "Incorrect input. Enter birthday in \"year/month/day\" format! Try again." << std::endl;
            continue;
        } 

        current_person.name = name;
        current_person.birthday = birthday;
        list_of_persons.push_back(current_person);
    }

    /*for(auto i : list_of_persons){
        std::cout << i.name << std::endl;
        std::cout << i.birthday << std::endl;
    }*/

   find_nearest_birthday(list_of_persons);
}