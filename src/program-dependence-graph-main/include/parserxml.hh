#ifndef _PARSER_XML_H_
#define _PARSER_XML_H_
#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <pugixml.hpp>
using namespace std;

// 定义结构体用于存储信息
struct StmtInfo {
    string dir;
    string functionName;
    string stmt;
};

class XMLParser {
private:
    vector<StmtInfo> stmts;
    map<string, vector<string>> dirMap;
    map<string, vector<string>> functionMap;
    bool loadXML(const string& filePath);
    void classifyByFunction();
    void classifyByDirectory();
public:
    // 构造函数
    XMLParser(const string& filePath) {
        if (loadXML(filePath)) {
            classifyByFunction();
            classifyByDirectory();
        }
    }
    map<string, vector<string>> &getfunMap() { return functionMap; }
    map<string, vector<string>> &getdirMap() { return dirMap; }

};

#endif