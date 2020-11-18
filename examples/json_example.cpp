#include <string>
#include <fstream>
#include <iostream>

#include "nlohmann/json.hpp"

void read_json_from_file(const std::string& file) {
    std::ifstream in(file);

    // deserialize from stream
    nlohmann::json js;
    in >> js;

    // add a player uid=3
    js["player"].push_back( {
                                    {"uid", 3},
                                    {"name", "p3"},
                                    {"sex", "男"},
                                    {"level", 20},

                                    {"objs", { {{"id", 300}, {"desc", "权杖"}}, {{"id", 400}, {"desc", "大剑"}} }},

                                    {"city", "杭州"}
                            });

    // add a player uid=4
    js["player"].push_back({
                                   {"uid", 4},
                                   {"name", "p4"},
                                   {"sex", "女"},
                                   {"level", 30},

                                   {"objs", { {{"id", 400}, {"desc", "梳子"}},
                                                   {{"id", 401}, {"desc", "镜子"}},
                                                   {{"id", 402}, {"desc", "鞋子"}} }},

                                   {"city", "深圳"}
                           });

    nlohmann::json player = nlohmann::json::parse("{\"uid\": 5, \"name\": \"p5\", \"sex\": \"男\", \"level\": 7, "
                                                  "\"objs\": {}, \"city\": \"广州\"}");


    js["player"].push_back(player);

    // output
    std::cout << std::setw(4) << js;
}

int main() {
    read_json_from_file("data/json.json");

    return 0;
}
