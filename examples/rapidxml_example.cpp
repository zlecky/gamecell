#include <string>
#include <fstream>
#include <iostream>
#include <string>
#include <cmath>

#include "rapidxml/rapidxml.hpp"
#include "rapidxml/rapidxml_utils.hpp"
#include "rapidxml/rapidxml_print.hpp"

namespace {
    void write_doc_to_xml(const std::string& file, rapidxml::xml_document<>& doc) {
        // write it into xml file.
        std::string content;
        rapidxml::print(std::back_inserter(content), doc, 0);

        std::cout << content << std::endl;

        std::fstream out(file, std::ios::out | std::ios::trunc);
        out << content;
        out.close();
    }

    void create_xml(const std::string& file) {
        rapidxml::xml_document<> doc;

        // declaration node by "node_declaration".
        {
            auto declaration = doc.allocate_node(rapidxml::node_declaration);
            declaration->append_attribute(doc.allocate_attribute("version", "1.0"));
            declaration->append_attribute(doc.allocate_attribute("encoding", "utf-8"));
            doc.append_node(declaration);
        }

        // declaration node by "node_pi"
        {
            auto declaration = doc.allocate_node(rapidxml::node_pi, doc.allocate_string(R"(xml version="1.0" encoding="utf-8")"));
            doc.append_node(declaration);
        }

        // node comment
        {
            auto comment = doc.allocate_node(rapidxml::node_comment, nullptr, "RapidXml很好用哟！");
            doc.append_node(comment);
        }

        // node element called "root"
        auto root = doc.allocate_node(rapidxml::node_element, "config");
        doc.append_node(root);

        // node element called "srv"
        auto srv = doc.allocate_node(rapidxml::node_element, "srv");
        {
            // node comment of "srv"
            {
                auto comment = doc.allocate_node(rapidxml::node_comment, nullptr, "服务器配置");
                srv->append_node(comment);
            }

            // node element called "item" without value
            {
                auto item = doc.allocate_node(rapidxml::node_element, "item");
                item->append_attribute(doc.allocate_attribute("id", "1"));
                item->append_attribute(doc.allocate_attribute("ip", "127.0.0.1"));
                item->append_attribute(doc.allocate_attribute("port", "8081"));

                srv->append_node(item);
            }

            // node element called "item" with value
            {
                auto item = doc.allocate_node(rapidxml::node_element, "item", "2");
                item->append_attribute(doc.allocate_attribute("id", "2"));
                item->append_attribute(doc.allocate_attribute("ip", "127.0.0.1"));
                item->append_attribute(doc.allocate_attribute("port", "8082"));

                srv->append_node(item);
            }

            // the right way to append attribute in loops
            for (size_t i = 3; i <= 10; ++i) {
                auto item = doc.allocate_node(rapidxml::node_element, "item", nullptr);

                auto id_attr_val   = doc.allocate_string(std::to_string(i).data());
                auto port_attr_val = doc.allocate_string(std::to_string(i + 8080).data());

                item->append_attribute(doc.allocate_attribute("id", id_attr_val));
                item->append_attribute(doc.allocate_attribute("ip", "127.0.0.1"));
                item->append_attribute(doc.allocate_attribute("port", port_attr_val));

                srv->append_node(item);
            }

            // The following way is wrong, because the ownership of strings. As follows:
            // Nodes and attributes produced by RapidXml do not own their name and value strings.
            // They merely hold the pointers to them.
            // Care must be taken to ensure that lifetime of the string passed is at least as long as lifetime of the node/attribute.
            // The easiest way to achieve it is to allocate the string from memory_pool owned by the document.
            // Use memory_pool::allocate_string() function for this purpose.
            // It's done this way for speed, but this feels like an car crash waiting to happen.
/*
           for (size_t i = 11; i <= 12; ++i) {
               auto item = doc.allocate_node(rapidxml::node_element, "item", nullptr);

               item->append_attribute(doc.allocate_attribute("id", std::to_string(i).data()));
               item->append_attribute(doc.allocate_attribute("ip", "127.0.0.1"));
               item->append_attribute(doc.allocate_attribute("port", std::to_string(i + 8080).data()));

               srv->append_node(item);
           }
*/
        }

        // append node element "srv" to "config"
        root->append_node(srv);

        write_doc_to_xml(file, doc);
    }

    void traverse_doc(rapidxml::xml_document<>& doc) {
        // traverse doc, then print
        auto config = doc.first_node("config");
        for (auto srv = config->first_node("srv"); srv != nullptr; srv = srv->next_sibling()) {
            for (auto item = srv->first_node("item"); item != nullptr; item = item->next_sibling()) {
                std::cout << "<" << item->name() << " ";

                for (auto attr = item->first_attribute(); attr != nullptr; attr = attr->next_attribute()) {
                    std::cout << attr->name() << "=\"" << attr->value() << "\" ";
                }

                if (item->first_node() != nullptr) {
                    std::cout << ">" << item->value() << "</" << item->name() << ">";
                } else {
                    std::cout << "/>";
                }

                std::cout << std::endl;
            }
        }
    }

    void read_xml_by_file(const std::string& file) {
        rapidxml::file<> fd(file.data());

        rapidxml::xml_document<> doc;
        doc.parse<0>(fd.data());

        traverse_doc(doc);
    }

    void read_xml_by_stream(const std::string& file) {
        std::ifstream in(file, std::ios::in);

        char buf[2048] = {0};
        in.read(buf, 2048);

        rapidxml::xml_document<> doc;
        doc.parse<0>(buf);

        traverse_doc(doc);
    }

    void modify_xml_node_attr(const std::string& file) {
        rapidxml::file<> fd(file.data());

        rapidxml::xml_document<> doc;
        doc.parse<rapidxml::parse_no_data_nodes>(fd.data());

        auto root = doc.first_node();
        auto srv = root->first_node("srv");

        for (auto item = srv->first_node("item"); item != nullptr; item = item->next_sibling()) {
            auto id = item->first_attribute("id");
            if (id != nullptr) {
                if (2 == std::strtol(id->value(), nullptr, 10))
                {
                    item->value("Yeah, find it!");

                    auto ip = item->first_attribute("ip");
                    if (ip != nullptr) {
                        ip->value("192.168.1.1");
                    }
                }
            }
        }

        write_doc_to_xml(file, doc);
    }
}

int main() {
    //create_xml("data/rapidxml.xml");

    //read_xml_by_file("data/rapidxml.xml");

    //read_xml_by_stream("data/rapidxml.xml");

    modify_xml_node_attr("data/rapidxml.xml");

    return 0;
}
