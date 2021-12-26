#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <tuple>
#include <fstream>
#include <istream>
#include <sstream>
#include <utility>
#include <iomanip>
#include <functional>

using namespace std;

struct Person {

    string m_firstname, m_secondname, m_familyname;
public:
    Person() {}
    Person(string firstname, string secondname, string familyname)
        : m_firstname(firstname), m_secondname(secondname), m_familyname(familyname)
    {}
    friend std::ostream& operator << (std::ostream& os, const Person& p);
    friend bool operator<(const Person& p1, const Person& p2);
    friend bool operator==(const Person& p1, const Person& p2);
};

bool operator==(const Person& p1, const Person& p2)
{
    return tie(p1.m_firstname, p1.m_secondname, p1.m_familyname) ==
        tie(p2.m_firstname, p2.m_secondname, p2.m_familyname);
}

bool operator<(const Person& p1, const Person& p2)
{
    return tie(p1.m_firstname, p1.m_secondname, p1.m_familyname) <
        tie(p2.m_firstname, p2.m_secondname, p2.m_familyname);
}

std::ostream& operator << (std::ostream& os, const Person& p)
{
    return os << p.m_firstname << " " << p.m_secondname << " " << p.m_familyname;
}

struct PhoneNumber {

    string m_countrycode, m_citycode, m_number, m_addnumber;
public:
    PhoneNumber() {}
    PhoneNumber(string countrycode, string citycode, string number, string addnumber)
        : m_countrycode(countrycode), m_citycode(citycode), m_number(number), m_addnumber(addnumber)
    {}
    friend std::ostream& operator << (std::ostream& os, const PhoneNumber& ph);
    friend bool operator<(const PhoneNumber& ph1, const PhoneNumber& ph2);
    friend bool operator==(const PhoneNumber& ph1, const PhoneNumber& ph2);
};

bool operator==(const PhoneNumber& ph1, const PhoneNumber& ph2)
{
    return tie(ph1.m_countrycode, ph1.m_citycode, ph1.m_number, ph1.m_addnumber) ==
        tie(ph2.m_countrycode, ph2.m_citycode, ph2.m_number, ph2.m_addnumber);
}

bool operator<(const PhoneNumber& ph1, const PhoneNumber& ph2)
{
    return tie(ph1.m_countrycode, ph1.m_citycode, ph1.m_number, ph1.m_addnumber) <
        tie(ph2.m_countrycode, ph2.m_citycode, ph2.m_number, ph2.m_addnumber);
}

std::ostream& operator << (std::ostream& os, const PhoneNumber& ph) {
    return os << "+" << ph.m_countrycode << "(" << ph.m_citycode << ")" << ph.m_number << " " << ph.m_addnumber;
}

class PhoneBook {
private:
    vector<pair<Person, PhoneNumber>> m_data;
public:
    PhoneBook(ifstream& file) {
        while (!file.eof()) {
            string name, name1, name2, country, city, num, add;
            file >> name1 >> name >> name2 >> country >> city >> num >> add;
            if (name2 == "-") name2 = " ";
            if (add == "-") add = " ";
            m_data.push_back(make_pair(Person(name1, name, name2), PhoneNumber(country, city, num, add)));
        }
    }

    friend void operator << (std::ostream& os, const PhoneBook& phb) {
        for (size_t x = 0; x < phb.m_data.size(); ++x) {
            cout << phb.m_data[x].first << "\t" << phb.m_data[x].second << endl;
        }
    }

    void SortByName() {
        std::sort(m_data.begin(), m_data.end(),
            [&](const pair<Person, PhoneNumber>& L, const pair<Person, PhoneNumber> R) {
                return L.first < R.first; });
    }

    void SortByPhone() {
        std::sort(m_data.begin(), m_data.end(),
            [&](const pair<Person, PhoneNumber>& L, const pair<Person, PhoneNumber> R) {
                return L.second < R.second; });
    }

    pair<string, PhoneNumber> GetPhoneNumber(string surname) {
        int line = 0;
        PhoneNumber number;
        for (size_t x = 0; x < m_data.size(); ++x) {
            if (m_data[x].first.m_secondname == surname) {
                ++line;
                if (line == 1)
                    number = m_data[x].second;
            }
        }
        if (line == 1) return pair<string, PhoneNumber>("", number);
        if (line == 0) return pair<string, PhoneNumber>("not found", number);
        if (line > 1) return pair<string, PhoneNumber>("more then 1", number);
    }

    void ChangePhoneNumber(const Person& person, const PhoneNumber& phone) {
        for (size_t x = 0; x < m_data.size(); ++x) {
            if (person == m_data[x].first) m_data[x].second = phone;
        }
    }
};



int main() {

    ifstream file("d:\\PhoneBook.txt"); 
    PhoneBook book(file);
    cout << book;

    cout << "------SortByPhone-------" << endl;
    book.SortByPhone();
    cout << book;

    cout << "------SortByName--------" << endl;
    book.SortByName();
    cout << book;

    cout << "-----GetPhoneNumber-----" << endl;
   
    auto print_phone_number = [&book](const string& surname) {
        cout << surname << "\t";
        auto answer = book.GetPhoneNumber(surname);
        if (get<0>(answer).empty())
            cout << get<1>(answer);
        else
            cout << get<0>(answer);
        cout << endl; };

    return 0;
};