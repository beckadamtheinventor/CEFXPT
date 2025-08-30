
#define FIXEDPOINT_NO_STDIO
#include "../../../fxpt/FixedPoint.hpp"
#include <cstdio>
#include <cstring>


char* dbgout = (char*)0xFB0000;

void printFixedPoint(FixedPoint f, char** addr) {
    *addr += f.tostring(*addr);
}

inline int putstrtobuf(char* addr, const char* str) {
    int len = strlen(str);
    memcpy(addr, str, len);
    return len;
}

int main() {
    FixedPoint a = FixedPoint::fromstring("8.5");
    FixedPoint b = FixedPoint::fromstring("1.75");
    FixedPoint aaddb = a + b;
    FixedPoint amulb = a * b;
    FixedPoint adivb = a / b;
    FixedPoint amodb = a % b;
    FixedPoint asquared = a.pow2();
    FixedPoint acubed = a.pow3();
    FixedPoint asqrt = a.sqrt();
    FixedPoint apow4 = a.powi(4);
    FixedPoint afloor = a.floor();
    FixedPoint around = a.round();
    FixedPoint aceil = a.ceil();

    char* addr = dbgout;
    // addr += sprintf(addr, "a (bytes): 0x%06X\n", *(int*)&a);
    // addr += sprintf(addr, "b (bytes): 0x%06X\n", *(int*)&b);
    printFixedPoint(a, &addr);
    *addr++ = '+';
    printFixedPoint(b, &addr);
    *addr++ = '=';
    printFixedPoint(aaddb, &addr);
    *addr++ = '\n';

    printFixedPoint(a, &addr);
    *addr++ = '*';
    printFixedPoint(b, &addr);
    *addr++ = '=';
    printFixedPoint(amulb, &addr);
    *addr++ = '\n';

    printFixedPoint(a, &addr);
    *addr++ = '/';
    printFixedPoint(b, &addr);
    *addr++ = '=';
    printFixedPoint(adivb, &addr);
    *addr++ = '\n';

    printFixedPoint(a, &addr);
    *addr++ = '%';
    printFixedPoint(b, &addr);
    *addr++ = '=';
    printFixedPoint(amodb, &addr);
    *addr++ = '\n';

    printFixedPoint(a, &addr);
    *addr++ = '*';
    *addr++ = '*';
    *addr++ = '2';
    *addr++ = '=';
    printFixedPoint(asquared, &addr);
    *addr++ = '\n';

    printFixedPoint(a, &addr);
    *addr++ = '*';
    *addr++ = '*';
    *addr++ = '3';
    *addr++ = '=';
    printFixedPoint(acubed, &addr);
    *addr++ = '\n';

    printFixedPoint(a, &addr);
    *addr++ = '*';
    *addr++ = '*';
    *addr++ = '4';
    *addr++ = '=';
    printFixedPoint(apow4, &addr);
    *addr++ = '\n';

    addr += putstrtobuf(addr, "sqrt ");
    printFixedPoint(a, &addr);
    *addr++ = '=';
    printFixedPoint(asqrt, &addr);
    *addr++ = '\n';

    addr += putstrtobuf(addr, "floor ");
    printFixedPoint(a, &addr);
    *addr++ = '=';
    printFixedPoint(afloor, &addr);
    *addr++ = '\n';

    addr += putstrtobuf(addr, "round ");
    printFixedPoint(a, &addr);
    *addr++ = '=';
    printFixedPoint(around, &addr);
    *addr++ = '\n';

    addr += putstrtobuf(addr, "ceil ");
    printFixedPoint(a, &addr);
    *addr++ = '=';
    printFixedPoint(aceil, &addr);
    *addr++ = '\n';

    *addr++ = 0;

    return 0;
}