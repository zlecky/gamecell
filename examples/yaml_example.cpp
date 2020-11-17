#include <iostream>
#include <string>
#include <vector>
#include <fstream>

#include "yaml-cpp/yaml.h"

using namespace std;

namespace {
    void load_from_yaml(const std::string& file) {
        YAML::Node node = YAML::LoadFile(file);
        std::cout << node << std::endl;
    }

    struct Obj {
        uint32_t id = 0;
        std::string desc;
    };

    struct Player {
        uint32_t uid = 0;
        std::string name;
        std::string sex;
        uint32_t  lv = 0;
        std::vector<Obj> objs;
        std::string city;
    };

    void dump_player(const std::vector<Player>& players) {
        for (const auto& item : players) {
            std::cout << "uid:" << item.uid << std::endl;
            std::cout << "name:" << item.name << std::endl;
            std::cout << "sex:" << item.sex << std::endl;
            std::cout << "level:" << item.lv << std::endl;

            for (const auto& obj : item.objs) {
                std::cout << "id:" << obj.id << std::endl;
                std::cout << "desc:" << obj.desc << std::endl;
            }

            std::cout << "city:" << item.city << std::endl;

            std::cout << std::endl;
        }
    }

    std::vector<Player> read_player_from_yaml(const std::string& file) {
        YAML::Node root = YAML::LoadFile(file);
        if (!root["player"].IsDefined()) {
           std::cout << "\"player\" node is not defined" << std::endl;
           return std::vector<Player>();
        }

        if (!root["player"].IsSequence()) {
            std::cout << "\"player\" node is not sequence" << std::endl;
            return std::vector<Player>();
        }

        std::vector<Player> players;
        for (const auto& node : root["player"]) {
            if (!node.IsMap()) {
                std::cout << root << " is not map" << std::endl;
                continue;
            }

            Player player;
            auto uid = node["uid"];
            if (uid.IsDefined() && uid.IsScalar()) {
                player.uid = uid.as<uint32_t>();
            }

            auto name = node["name"];
            if (name.IsDefined() && name.IsScalar()) {
                player.name = name.as<std::string>();
            }

            auto sex = node["sex"];
            if (sex.IsDefined() && sex.IsScalar()) {
                player.sex = sex.as<std::string>();
            }

            auto level = node["level"];
            if (level.IsDefined() && level.IsScalar()) {
                player.lv = level.as<uint32_t>();
            }

            auto objs = node["objs"];
            if (objs.IsDefined() && objs.IsSequence()) {
                for (const auto& item : objs) {
                    Obj obj;

                    auto id = item["id"];
                    if (id.IsDefined() && id.IsScalar()) {
                        obj.id = id.as<uint32_t>();
                    }

                    auto desc = item["desc"];
                    if (desc.IsDefined() && desc.IsScalar()) {
                        obj.desc = desc.as<std::string>();
                    }

                    player.objs.push_back(obj);
                }
            }

            auto city = node["city"];
            if (city.IsDefined() && city.IsScalar()) {
                player.city = city.as<std::string>();
            }

            players.push_back(player);
        }

        return players;
    }

    void write_player_to_yaml(const std::vector<Player>& players) {
        YAML::Node root;

        for (const auto& player : players) {
            YAML::Node p;
            p["uid"]   = player.uid;
            p["name"]  = player.name;
            p["sex"]   = player.sex;
            p["level"] = player.lv;
            p["city"]  = player.city;

            for (const auto& obj : player.objs) {
                YAML::Node o;
                o["id"]   = obj.id;
                o["desc"] = obj.desc;

                p["objs"].push_back(o);
            }

            root.push_back(p);
        }

        std::cout << root << std::endl;

        std::ofstream out("data/yaml-cpp-bak.yaml");
        out << root;
    }
}

int main(int argc,char** argv) {
    std::string file("data/yaml-cpp.yaml");

    load_from_yaml(file);

    auto players = read_player_from_yaml(file);

    dump_player(players);

    write_player_to_yaml(players);

    return 0;
}
