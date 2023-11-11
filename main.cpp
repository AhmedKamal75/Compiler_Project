#include <iostream>
#include <set>
#include <map>
#include "phase_one/Automaton/State.h"
#include "phase_one/Automaton/Automaton.h"


int main() {
//    Automaton a("a","","");
//    std::cout << a.toString() << std::endl;
    State s1(1,false,"hello");
    State s2(1,true," world");

    State s3 = s1.copy();
    std::cout << s1.toStringFull();
    std::cout << s3.toStringFull();
    std::cout << (s1 == s3);
    return 0;
}
