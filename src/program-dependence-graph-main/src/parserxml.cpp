#include "parserxml.hh"

bool XMLParser::loadXML(const string& filePath) {
        pugi::xml_document doc;
        if (!doc.load_file(filePath.c_str())) {
            cerr << "Failed to load XML file: " << filePath << endl;
            return false;
        }

        for (pugi::xml_node functionNode : doc.child("root").children("function")) {
            string functionName = functionNode.attribute("name").value();
            string dir = functionNode.attribute("dir").value();

            for (pugi::xml_node stmtNode : functionNode.children("stmt")) {
                string stmtDesc = stmtNode.attribute("dec").value();
                stmts.push_back({dir, functionName, stmtDesc});
            }
        }

        return true;
    }

// 按函数分类并输出结果
void XMLParser::classifyByFunction() {
    for (const auto& stmtInfo : stmts) {
        string fullStmt = stmtInfo.dir + " + " + stmtInfo.functionName + " + " + stmtInfo.stmt;
        functionMap[stmtInfo.functionName].push_back(fullStmt);
    }

    // cout << "Statements classified by function:" << endl;
    // for (const auto& funcPair : functionMap) {
    //     cout << "Function: " << funcPair.first << endl;
    //     for (const auto& stmt : funcPair.second) {
    //         cout << "  " << stmt << endl;
    //     }
    // }
}

 // 按目录分类并输出结果
void XMLParser::classifyByDirectory() {
    

    for (const auto& stmtInfo : stmts) {
        string fullStmt = stmtInfo.dir + " + " + stmtInfo.functionName + " + " + stmtInfo.stmt;
        dirMap[stmtInfo.dir].push_back(fullStmt);
    }

    // cout << "\nStatements classified by directory:" << endl;
    // for (const auto& dirPair : dirMap) {
    //     cout << "Directory: " << dirPair.first << endl;
    //     for (const auto& stmt : dirPair.second) {
    //         cout << "  " << stmt << endl;
    //     }
    // }
}