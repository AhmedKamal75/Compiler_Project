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
    if (isOperator(operatorChar)) {
        return priorities[operatorChar];
    }
    return -1;
}

bool Constants::isOperator(char c) {
    return priorities.find(c) != priorities.end();
}
