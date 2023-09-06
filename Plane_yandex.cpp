#include <iostream>
#include <vector>
#include <map>
#include <cmath>
#include <algorithm>

enum {row_place_number = 7, left_window = 0, left_aisle = 2, right_aisle = 4, right_window = 6, wrong_place = 3};


class Request {
private:
    int people_number;
    std::string side;
    std::string desire;
public:
    Request() = default;

    int get_people_number() const {
        return people_number;
    }

    const std::string& get_side() const {
        return side;
    }

    const std::string& get_desire() const {
        return desire;
    }

    friend std::istream& operator>>(std::istream& in, Request& request) {
        in >> request.people_number >> request.side >> request.desire;
        return in;
    }
};


class Plane {
private:
    class Place {
    private:
        char status;
    public:
        Place() : status('.') {}

        explicit Place(char status) : status(status) {}

        bool is_free() const {
            return status == '.';
        }

        void choose_place() {
            status = 'X';
        }

        char get_status() const {
            return status;
        }

        void close_for_choice() {
            status = '#';
        }

    };

    std::vector<std::vector<Place>> v;
    int rows_number;
    const std::map<std::string, int> first_place_map;

    bool row_is_suitable(const std::vector<Place>& row, const Request& request) const {

        std::string key_first_place = request.get_side() + '_' + request.get_desire();
        int first_place_index = first_place_map.at(key_first_place);
        int sign = pow(-1, first_place_map.at(key_first_place) / 2);

        for (int i = 0; i < request.get_people_number(); ++i) {
            if (!row[first_place_index + i * sign].is_free()) {
                return false;
            }
        }
        return true;

    }

    std::vector<std::string> get_chosen_places(int row_index, const Request& request) {

        std::string key = request.get_side() + '_' + request.get_desire();
        int first_place_index = first_place_map.at(key);
        int direction = pow(-1, first_place_index / 2);

        std::vector<std::string> chosen_places;
        chosen_places.resize(request.get_people_number());


        for (int i = 0; i < request.get_people_number(); ++i) {
            int place_index = first_place_index + i * direction;

            v[row_index][place_index].choose_place();

            chosen_places[i] += char('1' + row_index);
            chosen_places[i] += char('A' + place_index - 1 * (place_index > 3));
        }

        std::sort(chosen_places.begin(), chosen_places.end());

        return chosen_places;
    }


public:

    Plane(int rows_number) : rows_number(rows_number), first_place_map{{"left_window", left_window},
                                                                       {"left_aisle", left_aisle},
                                                                       {"right_window", right_window},
                                                                       {"right_aisle", right_aisle}} {
        v.reserve(rows_number);
    }

    void push_row(std::string&& new_row) {
        v.push_back(std::vector<Place>());
        v.back().reserve(row_place_number);
        for (char tmp_status : new_row) {
            v.back().push_back(Place(tmp_status));
        }
    }

    void process_request(const Request& request) {
        bool success = false;
        for (int i = 0; i < rows_number; ++i) {
            if (row_is_suitable(v[i], request)) {
                std::vector<std::string> chosen_places = get_chosen_places(i, request);
                success = true;
                show_successful_info(chosen_places);
                restyle_chosen_places(chosen_places);
                break;
            }
        }

        if (!success) {
            show_unsuccessful_info();
        }
    }

    void restyle_chosen_places(const std::vector<std::string>& chosen_places) {
        for (const auto& place : chosen_places) {
            int row_index = int(place[0]) - int('1');
            int place_index = int(place[1]) - int('A') + 1 * (place[1] > 'C');
            v[row_index][place_index].close_for_choice();
        }
    }
    /*void restyle_chosen_places(const std::vector<std::string>& chosen_places) {
        for (auto& row : v) {
            for (auto& place : row) {
                if (place.get_status() == 'X') {
                    place.close_for_choice();
                }
            }
        }
    }*/

    void show_successful_info(const std::vector<std::string>& places) const {
        std::cout << "Passengers can take seats:";
        for (const auto& place : places) {
            std::cout << ' ';
            std::cout << place;
        }
        std::cout << '\n';
        print();
    }

    void show_unsuccessful_info() const {
        std::cout << "Cannot fulfill passengers requirements" << '\n';
    }


    void print() const {
        for (const auto& row : v) {
            for (auto place : row) {
                std::cout << place.get_status();
            }
            std::cout << '\n';
        }
    }

    ~Plane() = default;

};


int main() {
    int rows_number;
    std::cin >> rows_number;
    Plane plane(rows_number);


    for (int i = 0; i < rows_number; ++i) {
        std::string tmp_row;
        std::cin >> tmp_row;
        plane.push_row(std::move(tmp_row));
    }

    int request_number;
    std::cin >> request_number;
    for (int i = 0; i < request_number; ++i) {
        Request tmp_request;
        std::cin >> tmp_request;

        plane.process_request(tmp_request);
    }


    return 0;
}
