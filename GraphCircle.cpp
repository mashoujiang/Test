#include <unordered_map>
#include <iostream>

/**
 *  1000->2000->3000->1000
*/
std::unordered_map<std::string, std::string> relationship;

std::string findFather(const std::string &key)
{
    return key == relationship[key] ? key : findFather(relationship[key]);
}
void insertToGraph(const std::string &key, const std::string &value)
{
    std::string keyF = findFather(key);
    std::string valueF = findFather(value);
    if (keyF == valueF)
        return;
    relationship[valueF] = keyF;
}
bool isSameFather(const std::string &key, const std::string &value)
{
    std::string keyF = findFather(key);
    std::string valueF = findFather(value);
    return keyF == valueF;
}
void initRelationship(const std::unordered_map<std::string, std::string> &graph)
{
    for (auto &item : graph)
    {
        relationship[std::get<0>(item)] = std::get<0>(item);
        relationship[std::get<1>(item)] = std::get<1>(item);
    }
}
bool isCircle(const std::unordered_map<std::string, std::string> &graph)
{
    initRelationship(graph);
    for (auto &item : graph)
    {
        auto key = std::get<0>(item);
        auto value = std::get<1>(item);
        if (isSameFather(key, value))
        {
            return true;
        }
        else
        {
            insertToGraph(key, value);
        }
    }
}

int main()
{
    std::unordered_map<std::string, std::string> graph{{"1000", "2000"}, {"2000", "3000"}, {"3000", "4000"}};
    std::unordered_map<std::string, std::string> graph2{{"1000", "2000"}, {"2000", "3000"}, {"3000", "1000"}};
    std::unordered_map<std::string, std::string> graph3{{"1000", "2000"}, {"2000", "3000"}, {"3000", "2000"}};
    std::cout << "graph is circle? " << (isCircle(graph) ? "Yes" : "No") << std::endl;
    std::cout << "graph is circle? " << (isCircle(graph2) ? "Yes" : "No") << std::endl;
    std::cout << "graph is circle? " << (isCircle(graph3) ? "Yes" : "No") << std::endl;
}