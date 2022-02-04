#include <iomanip>
#include <iostream>
#include <unordered_set> // find : complexity = constant
#include <utility>
#include <map> 
#include <unordered_map> // operator[] : complexity = constant

using namespace std;

class ReadingManager
{
public:
    void Read(int user_id, int page_count)
    {
        if (user_to_page.count(user_id) == 0)  // проверяем, вносили ли данные для данного юзера, count возвращает 0 или 1
        {
            user_to_page[user_id] = page_count;
            page_to_users[page_count].insert(user_id);
        }
        else
        {
            int previous_page = user_to_page[user_id];
            user_to_page[user_id] = page_count;

            page_to_users[previous_page].erase(user_id);
            page_to_users[page_count].insert(user_id);
        }
    }

    double Cheer(int user_id) 
    {
        auto count = user_to_page.count(user_id); // проверяем, вносили ли уже данные для данного пользователя или нет

        if (count == 0) // // если для данного пользователя пока не было ни одного события READ
        {
            return 0;
        }
        
        if (user_to_page.size() == 1 && count == 1) // если этот пользователь на данный момент единственный
        {
            return 1;
        }

        int user_page = user_to_page[user_id]; // find возвращает итератор

        auto position = page_to_users.find(user_page); // находим группу пользователей, каждый из которой прочитал данного количество страниц

        if (position == page_to_users.begin()) // если самое маленькое количество прочитанных страниц именно у этой группы
        {
            return 0;
        }

        position--;

        double result = 0;

        while (position != page_to_users.begin()) // пробегаем по всем ключам, соответствующим меньшему количеству страниц
        {
            result += position->second.size();
            position--;
        } 

        result += position->second.size();

        return result / (user_to_page.size() - 1); // -1, так как не учитываем самого юзера
    }

private:
    unordered_map<int, int> user_to_page;
    map<int, unordered_set<int>> page_to_users;
};


int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    ReadingManager manager;

    int query_count;
    cin >> query_count;
    for (int query_id = 0; query_id < query_count; ++query_id) {
        string query_type;
        cin >> query_type;
        int user_id;
        cin >> user_id;

        if (query_type == "READ") {
            int page_count;
            cin >> page_count;
            manager.Read(user_id, page_count);
        }
        else if (query_type == "CHEER") {
            cout << setprecision(6) << manager.Cheer(user_id) << "\n";
        }
    }
}

