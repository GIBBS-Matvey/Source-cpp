#include <iostream>
#include <string>
#include <vector>
#include <map>

using namespace std;

class PhoneBook {
private:
    map<string, map<string, vector<string>>> mainMap;

    bool isPhone(const string& text) {
        return isdigit(text[0]);
    }

    bool isEmail(const string& text) {
        auto iter = find(text.begin(), text.end(), '@');
        return iter != text.end();
    }

public:

    void AddPerson(const string& newName) {
        mainMap[newName];
    }

    void AddPhone(const string& name, const string& phone) {
        mainMap[name]["Phones"].push_back(phone);
    }

    void ReplaceEmail(const string& name, const string& email) {
        if (!mainMap[name]["Email"].empty()) {
            mainMap[name]["Email"][0] = email;
        } else {
            mainMap[name]["Email"].push_back(email);
        }
    }

    void ReplacePhone(const string& name, const string& oldPhone, const string& newPhone) {
        auto iter = find(mainMap[name]["Phones"].begin(), mainMap[name]["Phones"].end(), oldPhone);
        *iter = newPhone;
    }

    void DeletePerson(const string& name) {
        mainMap.erase(name);
    }

    void DeletePhone(const string& name, const string& phone) {
        auto iter = find(mainMap[name]["Phones"].begin(), mainMap[name]["Phones"].end(), phone);
        mainMap[name]["Phones"].erase(iter);
    }

    void PrintPerson(const string& name) {
        cout << "Name: " << name << endl;
        for (const auto& [key, v_data] : mainMap[name]) {
            cout << key << ": ";
            for (const auto& value : v_data) {
                if (value == v_data.back()) {
                    cout << value << endl;
                } else {
                    cout << value <<", ";
                }
            }
        }
    }

    void readCommand(const string& command) {

        if (command == "AddPerson") {
            string name, data;
            cin >> name >> data;
            AddPerson(name);

            if (isPhone(data) || isEmail(data)) {
                if (isPhone(data)) {
                    AddPhone(name, data);
                    string newData;
                    cin >> newData;
                    if (isEmail(newData)) {
                        ReplaceEmail(name, newData);
                    } else {
                        readCommand(newData);
                    }
                } else {
                    ReplaceEmail(name, data);
                    string newData;
                    cin >> newData;
                    if (isPhone(newData)) {
                        AddPhone(name, newData);
                    } else {
                        readCommand(newData);
                    }
                }
            } else {
                readCommand(data);
            }
        }

        if (command == "AddPhone") {
            string name, phone;
            cin >> name >> phone;
            AddPhone(name, phone);
        }

        if (command == "ReplaceEmail") {
            string name, email;
            cin >> name >> email;
            ReplaceEmail(name, email);
        }

        if (command == "ReplacePhone") {
            string name, oldPhone, newPhone;
            cin >> name >> oldPhone >> newPhone;
            ReplacePhone(name, oldPhone, newPhone);
        }

        if (command == "DeletePhone") {
            string name, phone;
            cin >> name >> phone;
            DeletePhone(name, phone);
        }

        if (command == "DeletePerson") {
            string name;
            DeletePerson(name);
        }

        if (command == "PrintPerson") {
            string name;
            cin >> name;
            PrintPerson(name);
        }

        cout << "DONE" << endl;
    }
};




int main() {

    PhoneBook phoneBook;

    while (true) {
        string command;
        cin >> command;
        if (command == "Finish") {
            break;
        }
        phoneBook.readCommand(command);
    }

    return 0;
}
