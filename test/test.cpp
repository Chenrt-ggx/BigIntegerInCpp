#include "test.h"

std::mt19937 generator(std::chrono::system_clock::now().time_since_epoch().count()) /* NOLINT */ ;

PyObject *pythonInit(const char *module) {
    Py_Initialize();
    PyRun_SimpleString("import sys");
    PyRun_SimpleString("sys.path.append('../test/')");
    return PyImport_ImportModule(module);
}

void complexTest() {
    std::cout << "Complex Mix Test: ";
    Complex lhs(-3, 4);
    Complex rhs(5, 12);
    assert(lhs.getReal() == -3), assert(lhs.getImag() == 4);
    assert(rhs.getReal() == 5), assert(rhs.getImag() == 12);
    assert(lhs.length() == 5), assert(rhs.length() == 13), assert(lhs != rhs);
    Complex zeroAll;
    assert(zeroAll.getReal() == 0), assert(zeroAll.getImag() == 0);
    Complex zeroImagAlpha(0);
    assert(zeroImagAlpha.getReal() == 0), assert(zeroImagAlpha.getImag() == 0);
    Complex zeroImagBeta(1);
    assert(zeroImagBeta.getReal() == 1), assert(zeroImagBeta.getImag() == 0);
    // ---------------------------
    Complex *pLeft = &lhs, *pRight = &lhs;
    Complex add = lhs + rhs;
    assert(&add != pLeft), assert(&add != pRight);
    assert(add - lhs == rhs), assert(add - rhs == lhs);
    Complex sub = lhs - rhs;
    assert(&sub != pLeft), assert(&sub != pRight);
    assert(sub + rhs == lhs);
    Complex mul = lhs * rhs;
    assert(&mul != pLeft), assert(&mul != pRight);
    assert(mul / lhs == rhs), assert(mul / rhs == lhs);
    Complex div = lhs / rhs;
    assert(&div != pLeft), assert(&div != pRight);
    assert(div * rhs == lhs);
    // ---------------------------
    lhs += rhs, assert(&lhs == pLeft), assert(lhs == add), lhs -= rhs;
    lhs -= rhs, assert(&lhs == pLeft), assert(lhs == sub), lhs += rhs;
    lhs *= rhs, assert(&lhs == pLeft), assert(lhs == mul), lhs /= rhs;
    lhs /= rhs, assert(&lhs == pLeft), assert(lhs == div), lhs *= rhs;
    // ---------------------------
    Complex complex;
    const char *real[] = {"-2", "-1.5", "-1", "-0.5", "0", "0.5", "1", "1.5", "2"};
    const char *imag[] = {"-2i", "-1.5i", "-i", "-0.5i", "0", "0.5i", "i", "1.5i", "2i"};
    for (int i = -4; i < 5; ++i) {
        complex.update(1.0 * i / 2, 0);
        assert(complex.toString() == std::string(real[i + 4]));
    }
    for (int i = -4; i < 5; ++i) {
        complex.update(0, 1.0 * i / 2);
        assert(complex.toString() == std::string(imag[i + 4]));
    }
    for (int i = -4; i < 5; ++i) {
        for (int j = -4; j < 5; ++j) {
            if (i != 0 && j != 0) {
                complex.update(1.0 * i / 2, 1.0 * j / 2);
                if (j > 0) {
                    assert(complex.toString() == std::string(real[i + 4]) + " + " + imag[j + 4]);
                } else {
                    assert(complex.toString() == std::string(real[i + 4]) + " - " + (imag[j + 4] + 1));
                }
            }
        }
    }
    std::cout << "Passed" << std::endl;
}

void constantTest() {
    std::cout << "Constant Mix Test: ";
    std::vector<int> lengths;
    const int maxLength = 10000;
    for (int i = 0; i < 10; ++i) {
        lengths.emplace_back(i + 1);
        lengths.emplace_back(i ? (int) (generator() % (maxLength / i)) : maxLength);
    }
    PyObject *module = pythonInit("constant");
    PyObject *eStd = PyObject_GetAttrString(module, "e_calculate");
    PyObject *piStd = PyObject_GetAttrString(module, "pi_calculate");
    PyObject *args = PyTuple_New(1);
    for (int i : lengths) {
        char *parse;
        PyObject *result;
        PyTuple_SetItem(args, 0, Py_BuildValue("i", i));
        result = PyObject_CallObject(piStd, args);
        PyArg_Parse(result, "s", &parse);
        assert(piCalculate(i) == std::string(parse));
        result = PyObject_CallObject(eStd, args);
        PyArg_Parse(result, "s", &parse);
        assert(eCalculate(i) == std::string(parse));
    }
    Py_Finalize();
    std::cout << "Passed" << std::endl;
}

void buildTestcases(std::vector<std::string> &testcases) {
    long long base[] = {0, BigInteger::mask, -BigInteger::mask, INT_MAX, INT_MIN, LONG_LONG_MAX};
    for (long long i : base) {
        for (int j = -2; j <= 2; ++j) {
            testcases.emplace_back(std::to_string(i + j));
        }
    }
    for (int _ = 0; _ < 10; ++_) {
        std::string str;
        for (int i = 0; i < 5; ++i) {
            long long value = generator();
            str += std::to_string(value < 0 ? -value : value);
        }
        testcases.emplace_back(str + std::to_string((generator() % 10 + 10) % 10));
        testcases.emplace_back(str + std::to_string((generator() % 10 + 10) % 10));
        testcases.emplace_back("-" + str + std::to_string((generator() % 10 + 10) % 10));
        testcases.emplace_back("-" + str + std::to_string((generator() % 10 + 10) % 10));
    }
}

void buildTest(std::vector<std::string> &testcases) {
    std::cout << "Big Integer Build Test: ";
    BigInteger buildNone;
    assert(buildNone.toString() == std::string("0"));
    BigInteger buildEmpty("");
    assert(buildEmpty.toString() == std::string("0"));
    int iValues[] = {-2, -1, 0, 1, 2, INT_MAX, INT_MIN, INT_MAX / 2, INT_MIN / 2};
    for (int i : iValues) {
        BigInteger build(i);
        assert(build.toString() == std::to_string(i));
    }
    unsigned int uiValues[] = {0, 1, 2, 0x3f3f3f3f, 0x7fffffff, 0xffffffff};
    for (unsigned int i : uiValues) {
        BigInteger build(i);
        assert(build.toString() == std::to_string(i));
    }
    long long llValues[] = {-2, -1, 0, 1, 2, INT_MAX, INT_MIN, INT_MAX / 2, INT_MIN / 2, LONG_LONG_MAX,
                            LONG_LONG_MIN, LONG_LONG_MAX / 2, LONG_LONG_MIN / 2};
    for (long long i : llValues) {
        BigInteger build(i);
        assert(build.toString() == std::to_string(i));
    }
    unsigned long long ullValues[] = {0, 1, 2, 0x3f3f3f3f, 0x7fffffff, 0xffffffff, 0x3f3f3f3f3f3f,
                                      0x7fffffffffff, 0xffffffffffff, 0x3f3f3f3f3f3f3f3f,
                                      0x7fffffffffffffff, 0xffffffffffffffff};
    for (unsigned long long i : ullValues) {
        BigInteger build(i);
        assert(build.toString() == std::to_string(i));
    }
    for (std::string &i : testcases) {
        BigInteger buildStr(i);
        assert(buildStr.toString() == i);
        BigInteger buildChr(i.c_str());
        assert(buildStr.toString() == i);
    }
    for (int _ = 0; _ < 3; ++_) {
        std::string str;
        for (int i = 0; i < 3; ++i) {
            long long value = generator();
            str += std::to_string(value < 0 ? -value : value);
        }
        std::vector<std::string> tests;
        for (int i = 0; i < 5; ++i) {
            std::string fix(i, '0');
            fix += str, tests.emplace_back(fix);
            tests.emplace_back(std::string("+") + fix);
            tests.emplace_back(std::string("-") + fix);
        }
        for (std::string &i : tests) {
            BigInteger buildStr(i);
            assert(buildStr.toString() == (i[0] == '-' ? "-" + str : str));
            BigInteger buildChr(i.c_str());
            assert(buildChr.toString() == (i[0] == '-' ? "-" + str : str));
        }
    }
    std::cout << "Passed" << std::endl;
}

void basicTest(std::vector<std::string> &testcases) {
    std::cout << "Big Integer Basic Test: ";
    PyObject *module = pythonInit("calculate");
    PyObject *add = PyObject_GetAttrString(module, "add");
    PyObject *sub = PyObject_GetAttrString(module, "sub");
    PyObject *args = PyTuple_New(2);
    for (std::string &i : testcases) {
        BigInteger bigInteger(i), *ptr = &bigInteger;
        BigInteger absTest = bigInteger.abs();
        BigInteger posTest = +bigInteger;
        BigInteger negTest = -bigInteger;
        assert(ptr != &absTest), assert(ptr != &posTest), assert(ptr != &negTest);
        if (i == std::string("0")) {
            assert(absTest.toString() == std::string("0"));
            assert(posTest.toString() == std::string("0"));
            assert(negTest.toString() == std::string("0"));
        } else if (i[0] == '-') {
            assert(absTest.toString() == i.substr(1));
            assert(posTest.toString() == i);
            assert(negTest.toString() == i.substr(1));
        } else {
            assert(absTest.toString() == i);
            assert(posTest.toString() == i);
            assert(negTest.toString() == std::string("-") + i);
        }
    }
    for (std::string &i : testcases) {
        char *parse;
        PyObject *result;
        PyTuple_SetItem(args, 0, Py_BuildValue("s", i.c_str()));
        PyTuple_SetItem(args, 1, Py_BuildValue("s", "1"));
        result = PyObject_CallObject(add, args);
        BigInteger incTest(i), *incPtr = &incTest;
        PyArg_Parse(result, "s", &parse);
        std::string incBefore = incTest.toString();
        BigInteger rightInc = incTest++;
        assert(&incTest == incPtr), assert(&rightInc != incPtr);
        assert(incTest.toString() == std::string(parse));
        assert(rightInc.toString() == incBefore);
        --incTest;
        BigInteger leftInc = ++incTest;
        assert(&incTest == incPtr), assert(&leftInc != incPtr);
        assert(incTest.toString() == std::string(parse));
        assert(leftInc.toString() == std::string(parse));
        result = PyObject_CallObject(sub, args);
        BigInteger decTest(i), *decPtr = &decTest;
        PyArg_Parse(result, "s", &parse);
        std::string decBefore = decTest.toString();
        BigInteger rightDec = decTest--;
        assert(&decTest == decPtr), assert(&rightDec != decPtr);
        assert(decTest.toString() == std::string(parse));
        assert(rightDec.toString() == decBefore);
        ++decTest;
        BigInteger leftDec = --decTest;
        assert(&decTest == decPtr), assert(&leftDec != decPtr);
        assert(decTest.toString() == std::string(parse));
        assert(leftDec.toString() == std::string(parse));
    }
    Py_Finalize();
    std::cout << "Passed" << std::endl;
}

void compareTest(std::vector<std::string> &testcases) {
    std::cout << "Big Integer Compare Test: ";
    PyObject *module = pythonInit("calculate");
    PyObject *eq = PyObject_GetAttrString(module, "eq");
    PyObject *ne = PyObject_GetAttrString(module, "ne");
    PyObject *gt = PyObject_GetAttrString(module, "gt");
    PyObject *ge = PyObject_GetAttrString(module, "ge");
    PyObject *lt = PyObject_GetAttrString(module, "lt");
    PyObject *le = PyObject_GetAttrString(module, "le");
    PyObject *args = PyTuple_New(2);
    for (std::string &i : testcases) {
        for (std::string &j : testcases) {
            int parse;
            PyObject *result;
            BigInteger lhs(i), rhs(j);
            PyTuple_SetItem(args, 0, Py_BuildValue("s", i.c_str()));
            PyTuple_SetItem(args, 1, Py_BuildValue("s", j.c_str()));
            result = PyObject_CallObject(eq, args);
            PyArg_Parse(result, "i", &parse);
            assert((lhs == rhs) == parse);
            result = PyObject_CallObject(ne, args);
            PyArg_Parse(result, "i", &parse);
            assert((lhs != rhs) == parse);
            result = PyObject_CallObject(gt, args);
            PyArg_Parse(result, "i", &parse);
            assert((lhs > rhs) == parse);
            result = PyObject_CallObject(ge, args);
            PyArg_Parse(result, "i", &parse);
            assert((lhs >= rhs) == parse);
            result = PyObject_CallObject(lt, args);
            PyArg_Parse(result, "i", &parse);
            assert((lhs < rhs) == parse);
            result = PyObject_CallObject(le, args);
            PyArg_Parse(result, "i", &parse);
            assert((lhs <= rhs) == parse);
        }
    }
    Py_Finalize();
    std::cout << "Passed" << std::endl;
}

void calculateTest(std::vector<std::string> &testcases) {
    std::cout << "Big Integer Calculate Test: ";
    PyObject *module = pythonInit("calculate");
    PyObject *add = PyObject_GetAttrString(module, "add");
    PyObject *sub = PyObject_GetAttrString(module, "sub");
    PyObject *args = PyTuple_New(2);
    for (std::string &i : testcases) {
        for (std::string &j : testcases) {
            char *parse;
            PyObject *result;
            BigInteger lhs(i), rhs(j);
            BigInteger *pLeft = &lhs, *pRight = &rhs;
            PyTuple_SetItem(args, 0, Py_BuildValue("s", i.c_str()));
            PyTuple_SetItem(args, 1, Py_BuildValue("s", j.c_str()));
            result = PyObject_CallObject(add, args);
            PyArg_Parse(result, "s", &parse);
            BigInteger addTest = lhs + rhs;
            assert(&addTest != pLeft), assert(&addTest != pRight);
            assert(addTest.toString() == std::string(parse));
            lhs += rhs, assert(&lhs == pLeft), assert(lhs.toString() == std::string(parse)), lhs -= rhs;
            result = PyObject_CallObject(sub, args);
            PyArg_Parse(result, "s", &parse);
            BigInteger subTest = lhs - rhs;
            assert(&subTest != pLeft), assert(&subTest != pRight);
            assert(subTest.toString() == std::string(parse));
            lhs -= rhs, assert(&lhs == pLeft), assert(lhs.toString() == std::string(parse)), lhs += rhs;
        }
    }
    Py_Finalize();
    std::cout << "Passed" << std::endl;
}

void mulDivTest(std::vector<std::string> &testcases) {
    std::cout << "Big Integer Mul Div Test: ";
    PyObject *module = pythonInit("calculate");
    PyObject *mul = PyObject_GetAttrString(module, "mul");
    PyObject *div = PyObject_GetAttrString(module, "div");
    PyObject *mod = PyObject_GetAttrString(module, "mod");
    PyObject *args = PyTuple_New(2);
    std::vector<int> mulMethods = {BigInteger::selectBrute, BigInteger::selectFFT, BigInteger::selectNTT};
    for (std::string &i : testcases) {
        for (std::string &j : testcases) {
            char *parse;
            PyObject *result;
            BigInteger *pLeft, *pRight;
            for (int k : mulMethods) {
                BigInteger lhs(i), rhs(j);
                BigInteger::setMulMethod(k);
                pLeft = &lhs, pRight = &rhs;
                PyTuple_SetItem(args, 0, Py_BuildValue("s", i.c_str()));
                PyTuple_SetItem(args, 1, Py_BuildValue("s", j.c_str()));
                result = PyObject_CallObject(mul, args);
                PyArg_Parse(result, "s", &parse);
                BigInteger mulTest = lhs * rhs;
                assert(&mulTest != pLeft), assert(&mulTest != pRight);
                assert(mulTest.toString() == std::string(parse));
                lhs *= rhs, assert(&lhs == pLeft), assert(lhs.toString() == std::string(parse));
            }
            if (j != std::string("0")) {
                PyTuple_SetItem(args, 0, Py_BuildValue("s", i.c_str()));
                PyTuple_SetItem(args, 1, Py_BuildValue("s", j.c_str()));
                BigInteger lhsDiv(i), rhsDiv(j);
                pLeft = &lhsDiv, pRight = &rhsDiv;
                result = PyObject_CallObject(div, args);
                PyArg_Parse(result, "s", &parse);
                BigInteger divTest = lhsDiv / rhsDiv;
                assert(&divTest != pLeft), assert(&divTest != pRight);
                assert(divTest.toString() == std::string(parse));
                lhsDiv /= rhsDiv, assert(&lhsDiv == pLeft), assert(lhsDiv.toString() == std::string(parse));
                BigInteger lhsMod(i), rhsMod(j);
                pLeft = &lhsMod, pRight = &rhsMod;
                result = PyObject_CallObject(mod, args);
                PyArg_Parse(result, "s", &parse);
                BigInteger modTest = lhsMod % rhsMod;
                assert(&modTest != pLeft), assert(&modTest != pRight);
                assert(modTest.toString() == std::string(parse));
                lhsMod %= rhsMod, assert(&lhsMod == pLeft), assert(lhsMod.toString() == std::string(parse));
                assert(BigInteger(i) == divTest * BigInteger(j) + modTest);
            }
        }
    }
    BigInteger::setMulMethod(BigInteger::autoSelect);
    for (std::string &i : testcases) {
        for (std::string &j : testcases) {
            BigInteger lhs(i);
            BigInteger rhs(j);
            BigInteger mulTest = lhs * rhs;
            if (i != std::string("0")) {
                assert(mulTest / lhs == rhs), assert(mulTest % lhs == BigInteger(0));
            }
            if (j != std::string("0")) {
                assert(mulTest / rhs == lhs), assert(mulTest % rhs == BigInteger(0));
            }
        }
    }
    Py_Finalize();
    std::cout << "Passed" << std::endl;
}

void bigIntegerTest() {
    std::vector<std::string> testcases;
    buildTestcases(testcases);
    buildTest(testcases);
    basicTest(testcases);
    compareTest(testcases);
    calculateTest(testcases);
    mulDivTest(testcases);
}
