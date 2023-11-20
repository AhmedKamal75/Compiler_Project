#include "Constants.h"


Constants::Constants() {
    priorities[ESCAPE] = 5;
    priorities[KLEENE_CLOSURE] = 4;
    priorities[POSITIVE_CLOSURE] = 4;
    priorities[RANGE] = 3;
    priorities[CONCATENATION] = 2;
    priorities[UNION] = 1;
    priorities[OPEN_PARENTHESIS] = 0;
    priorities[CLOSE_PARENTHESIS] = 0;
}

int Constants::priority(char operatorChar) {
    if (is_operator(operatorChar)) {
        return priorities[operatorChar];
    }
    return -1;
}

int Constants::priority(std::string operatorChar) {
    if (is_operator(operatorChar)) {
        return priorities[operatorChar.at(0)];
    }
    return -1;
}

bool Constants::is_operator(std::string c) {
    if (c.length() > 1) return false;

    return priorities.find(c.at(0)) != priorities.end();
}


bool Constants::is_operator(char c) {
    return priorities.find(c) != priorities.end();
}

bool Constants::is_operator(const std::string &str, const char &op) {
    if (str.length() > 1) return false;
    return str.at(0) == op;
}