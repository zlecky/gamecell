#include <string>
#include <fstream>
#include <iostream>
#include <map>
#include <string>

#include "rapidxml/rapidxml.hpp"
#include "rapidxml/rapidxml_print.hpp"

namespace {
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

           // the following way is wrong
/*
           for (size_t i = 3; i <= 10; ++i) {
               auto item = doc.allocate_node(rapidxml::node_element, "item", nullptr);

               item->append_attribute(doc.allocate_attribute("id", doc.allocate_string(std::to_string(i).data())));

               srv->append_node(item);
           }
*/
       }

       // append node element "srv" to "config"
       root->append_node(srv);

       // write it into xml file.
       {
           std::string content;
           rapidxml::print(std::back_inserter(content), doc, 0);

           std::cout << content << std::endl;

           std::fstream out(file, std::ios::out);
           out << content;
       }
   }
}

int main() {
    create_xml("data/rapidxml.xml");

    return 0;
}
