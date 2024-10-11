#include <iostream>
#include <cmath>
using namespace std;

int get_integer_value();


int daysNumber(int month, int year);

int convertToInt(string text, int ind_start, int ind_finish);

bool isDate(string text);

bool isDate(int day, int month, int year);



class Date {
private:
    int day;
    int month;
    int year;

    int daysInYear(int year);
    int localDifference(Date less, Date more);

public:
    Date(int a, int b, int c) : day(a), month(b), year(c)
    {
        if (isDate(day, month, year))
        {
            day = a;
            month = b;
            year = c;
        }
        else
        {
            year = 1970;
            month = 1;
            day = 1;
        }
    }

    Date(string text) : year(0), month(0), day(0)
    {
        if (isDate(text))
        {
            year += (int(text[0]) - int('0')) * 1000;
            year += (int(text[1]) - int('0')) * 100;
            year += (int(text[2]) - int('0')) * 10;
            year += (int(text[3]) - int('0')) * 1;

            month += (int(text[5]) - int('0')) * 10;
            month += (int(text[6]) - int('0')) * 1;

            day += (int(text[8]) - int('0')) * 10;
            day += (int(text[9]) - int('0')) * 1;
        }
        else
        {
            year = 1970;
            month = 1;
            day = 1;
        }
    }

    void addDays(int number);

    void subDays(int number);

    bool operator<(Date other);

    bool operator==(Date other);

    bool operator!=(Date other);

    bool operator<=(Date other);

    int Difference(Date other);

    bool isLeapYear();

    void Print();

    //void boolDifference(Date other);

};


int Date::daysInYear(int year) {
    if (year % 4 == 0) {
        return 366;
    }
    else {
        return 365;
    }
}

void Date::addDays(int number) {

    if (number < daysNumber(month, year) - day) {
        day += number;
    }
    else {
        number -= daysNumber(month, year) - day;
        //number = number - daysNumer(month, year) + day;
        month++;
        if (month == 13) {
            year++;
            month = 1;
        }

        while (number > daysNumber(month, year)) {
            number -= daysNumber(month, year);
            month++;
            if (month == 13) {
                year++;
                month = 1;
            }
        }

        day = number;
    }
}


void Date::subDays(int number) {

    if (number < day) {
        day -= number;
    }
    else
    {
        number -= day;
        month--;
        if (month == 0) {
            year--;
            month = 12;
        }
        day = daysNumber(month, year);

        while (number >= daysNumber(month, year)) {
            number -= daysNumber(month, year);
            month--;
            day = daysNumber(month, year);
            if (month == 0) {
                year--;
                month = 12;
            }
        }

        day -= number;
    }
}


bool Date::operator<(Date other) {
    if (year <= other.year) {
        if (year == other.year) {
            if (month <= other.month) {
                if (month == other.month) {
                    if (day < other.day) {
                        return true;
                    }
                    else {
                        return false;
                    }
                }
                else {
                    return true;
                }
            }
            else {
                return false;
            }
        }
        else {
            return true;
        }
    }
    else {
        return false;
    }
}

bool Date::operator==(Date other) {
    return (!(*this < other) && !(other < *this));
}


bool Date::operator!=(Date other) {
    return !(*this == other);
}

bool Date::operator<=(Date other)
{
    return (*this < other) || (*this == other);
}

int Date::localDifference(Date less, Date more) {
    int answer = 0;
    Date tmp = less;
    while (tmp != more) {
        if (tmp.year == more.year) {
            if (tmp.month == more.month) {
                answer += abs(more.day - tmp.day);
                tmp.day = more.day;
            }
            else {
                while (tmp.month != more.month) {
                    if (tmp.month < more.month) {
                        answer += daysNumber(tmp.month, tmp.year);
                        ++tmp.month;
                    }
                    else {
                        --tmp.month;
                        answer -= daysNumber(tmp.month, tmp.year);
                    }
                }
            }
        }
        else {
            while (tmp.year != more.year) {
                answer += 365;
                if (tmp.isLeapYear() && tmp <= (Date(29, 2, tmp.year))) {
                    answer += 1;
                }
                ++tmp.year;
                if (tmp.isLeapYear() && (Date(29, 2, tmp.year)) <= tmp) {
                    answer += 1;
                }
            }
        }
    }
    return answer;
}


int Date::Difference(Date other) {
    if (*this < other) {
        return localDifference(*this, other);
    }
    else {
        return localDifference(other, *this);
    }
}


bool Date::isLeapYear() {
    return year % 4 == 0;
}


void Date::Print() {
    cout << year << '/' << month << '/' << day << endl;
}

/*void Date::boolDifference(Date other) {
    if (*this < other) {
        cout << "первая дата меньше" << endl;
    }
    else if (*this == other) {
        cout << "даты равны" << endl;
    }
    else if (other < *this) {
        cout << "первая дата больше" << endl;
    }
}*/


int get_integer_value()
{
    int number;
    while (!(cin >> number) || cin.peek() != '\n')
    {
        cin.clear();
        while (cin.get() != '\n') {};
        cout << "Введено неверное значение. Повторите попытку: ";
    }
    return number;
}


int daysNumber(int month, int year) {
    if (month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12) {
        return 31;
    }
    else if (month == 4 || month == 6 || month == 9 || month == 11) {
        return 30;
    }
    else if (month == 2 && year % 4 == 0) {
        return 29;
    }
    else if (month == 2 && year % 4 != 0) {
        return 28;
    }
}


bool isDate(int day, int month, int year) {
    if (!(year > 999 && year < 10000)) {
        return false;
    }
    if (!(month > 0 && month < 13)) {
        return false;
    }
    if (!(day > 0 && day <= daysNumber(month, year))) {
        return false;
    }
    return true;
}


int convertToInt(string text, int ind_start, int ind_finish) {
    int value = 0;
    for (int i = ind_start; i < ind_finish; ++i) {
        value += (int(text[i]) - int('0')) * pow(10, ind_finish - i - 1);
    }
    return value;
}


bool isDate(string text) {
    int day, month, year;
    day = month = year = 0;

    for (int i = 0; i < 4; ++i) {
        if (!(int(text[i]) >= 48 && int(text[i]) <= 57)) {
            return false;
        }
    }
    year = convertToInt(text, 0, 4);


    if (text[4] != '/') {
        return false;
    }
    for (int i = 5; i < 7; ++i) {
        if (!(int(text[i]) >= 48 && int(text[i]) <= 57)) {
            return false;
        }
    }
    month = convertToInt(text, 5, 7);

    if (text[7] != '/') {
        return false;
    }
    for (int i = 8; i < 10; ++i) {
        if (!(int(text[i]) >= 48 && int(text[i]) <= 57)) {
            return false;
        }
    }
    day = convertToInt(text, 8, 10);

    return isDate(day, month, year);
}



int main() {
    setlocale(LC_ALL, "RUS");
    cout << "Лабораторная работа №2. Класс дата, в формате год, месяц, день" << endl;
    Link:
    cout << "Для начала введите дату ввиде строки вида: год/месяц/день: ";



    string text;
    cin >> text;
    while (!isDate(text)) {
        cout << "Введите дату еще раз: " << endl;
        cin >> text;
    }
    Date date_1(text);

    cout << "Теперь введите день, месяц, год по отдельности: " << endl;
    int day = 1, month = 1, year = 2000;
    cout << "Введите год: ";
    year = get_integer_value();
    while (!isDate(day, month, year)) {
        cout << "Год введен неверно. Попробуйте снова: " << endl;
        year = get_integer_value();
        if (isDate(day, month, year)) break;
    }
    cout << "Введите месяц: ";
    month = get_integer_value();
    while (!isDate(day, month, year)) {
        cout << "Месяц введен неверно. Попробуйте снова: " << endl;
        month = get_integer_value();
        if (isDate(day, month, year)) break;
    }
    cout << "Введите день: ";
    day = get_integer_value();
    while (!isDate(day, month, year)) {
        cout << "День введен неверно. Попробуйте снова: ";
        day = get_integer_value();
        if (isDate(day, month, year)) break;
    }

    Date date_2(day, month, year);

    while (true) {
        cout << "Введите название команды из меню" << endl;
        cout << "1 - Добавление к дате указанного пользователем количества дней" << endl;
        cout << "2 - Вычитание из даты указанного пользователем количества дней" << endl;
        cout << "3 - Вычисление разницы в днях между двумя датами" << endl;
        cout << "4 - определение високосного года" << endl;
        cout << "5 - сравнение двух дат" << endl;
        cout << "6 - переопределить данные" << endl;
        cout << "7 - выход" << endl;
        int command;
        command = get_integer_value();

        if (command == 6) {
            goto Link;
        }


        else if (command == 1) {
            int value;
            cout << "Количество дней: ";
            value = get_integer_value();
            date_1.addDays(value);
            date_1.Print();
        }

        else if (command == 2) {
            int value;
            cout << "Количество дней: ";
            value = get_integer_value();
            date_1.subDays(value);
            date_1.Print();
        }

        else if (command == 3) {
            cout << date_1.Difference(date_2) << endl;
        }

        else if (command == 5)
        {
            // date_1.boolDifference(date_2);
            if (date_1 < date_2) {
                cout << "первая дата меньше" << endl;
            }
            else if (date_1 == date_2) {
                cout << "даты равны" << endl;
            }
            else if (date_2 < date_1) {
                cout << "первая дата больше" << endl;
            }
        }

        else if (command == 4) {
            if (date_1.isLeapYear())
            {
                cout << "Введенный год является високосным" << endl;
            }
            else
            {
                cout << "Введенный год не является високосным" << endl;
            }
        }
        else if (command == 7)
        {
            break;
        }
        else cout << "Введена несуществующая команда." << endl;
    }

    return 0;
}
