/* --------------------------------------------------------------------------
 *  CppADCodeGen: C++ Algorithmic Differentiation with Source Code Generation:
 *    Copyright (C) 2013 Ciengis
 *
 *  CppADCodeGen is distributed under multiple licenses:
 *
 *   - Common Public License Version 1.0 (CPL1), and
 *   - GNU General Public License Version 2 (GPL2).
 *
 * CPL1 terms and conditions can be found in the file "epl-v10.txt", while
 * terms and conditions for the GPL2 can be found in the file "gpl2.txt".
 * ----------------------------------------------------------------------------
 * Author: Joao Leal
 */
#include "CppADCGPatternTest.hpp"

typedef double Base;
typedef CppAD::CG<Base> CGD;
typedef CppAD::AD<CGD> ADCGD;

using namespace CppAD;

std::vector<ADCGD> modelCommonTmp2(std::vector<ADCGD>& x, size_t repeat) {
    size_t m = 2;
    size_t n = 2;
    size_t m2 = repeat * m;

    // dependent variable vector 
    std::vector<ADCGD> y(m2);

    ADCGD tmp1 = x[1] * sin(x[1] + 4);
    ADCGD tmp2 = 2.5 * x[1];
    for (size_t i = 0; i < repeat; i++) {
        y[i * m] = tmp1 * cos(x[i * n]) / tmp2;
        y[i * m + 1] = x[i * n + 1] * log(x[i * n]);
    }

    return y;
}

TEST_F(CppADCGPatternTest, CommonTmp2) {
    using namespace CppAD;
    size_t m = 2;
    size_t n = 2;
    size_t mExtra = 0; // equations not in loops

    bool jacobian = true;
    bool hessian = false;

    testPatternDetection(modelCommonTmp2, m, n, 6);

    testLibCreation(modelCommonTmp2, m, n, 6, mExtra, "modelCommonTmp2", jacobian, hessian);
}

std::vector<ADCGD> model0(std::vector<ADCGD>& x, size_t repeat) {
    size_t m = 2;
    size_t n = 2;
    size_t m2 = repeat * m;

    // dependent variable vector 
    std::vector<ADCGD> y(m2);

    for (size_t i = 0; i < repeat; i++) {
        y[i * m] = cos(x[i * n]);
        y[i * m + 1] = x[i * n + 1] * x[i * n];
    }

    return y;
}

TEST_F(CppADCGPatternTest, DependentPatternMatcherDetached) {
    using namespace CppAD;
    size_t m = 2;
    size_t n = 2;
    size_t mExtra = 0; // equations not in loops

    bool jacobian = true;
    bool hessian = true;

    testPatternDetection(model0, m, n, 6);

    testLibCreation(model0, m, n, 6, mExtra, "model0", jacobian, hessian);
}

std::vector<ADCGD> model1(std::vector<ADCGD>& x, size_t repeat) {
    size_t m = 2;
    size_t n = 2;
    size_t m2 = repeat * m;

    // dependent variable vector 
    std::vector<ADCGD> y(m2);

    for (size_t i = 0; i < repeat; i++) {
        y[i * m] = cos(x[i * n]) + x[1] * x[2];
        y[i * m + 1] = x[i * n + 1] * x[i * n];
    }

    return y;
}

TEST_F(CppADCGPatternTest, DependentPatternMatcher) {
    using namespace CppAD;
    size_t m = 2;
    size_t n = 2;
    size_t mExtra = 0; // equations not in loops

    bool jacobian = false;
    bool hessian = false;

    testPatternDetection(model1, m, n, 6);

    testLibCreation(model1, m, n, 6, mExtra, "model1", jacobian, hessian);
}

std::vector<ADCGD> model2(std::vector<ADCGD>& x, size_t repeat) {
    size_t m = 1;
    size_t n = 2;

    // dependent variable vector 
    std::vector<ADCGD> y(repeat * m);

    ADCGD ax0;
    ADCGD ax1;
    for (size_t i = 0; i < repeat; i++) {
        if (i < repeat / 2) {
            ax0 = x[i * n];
            ax1 = x[i * n + 1];
        }
        y[i * m] = ax0 + ax0 * ax1;
    }

    return y;
}

TEST_F(CppADCGPatternTest, model2) {
    using namespace CppAD;

    size_t m = 1;
    size_t n = 2;
    size_t mExtra = 0; // equations not in loops

    testPatternDetection(model2, m, n, 6);

    testLibCreation(model2, m, n, 6, mExtra, "model2");
}

std::vector<ADCGD> model3(std::vector<ADCGD>& x, size_t repeat) {
    size_t m = 1;
    size_t n = 2;

    // dependent variable vector 
    std::vector<ADCGD> y(repeat * m);

    ADCGD ax1;
    for (size_t i = 0; i < repeat; i++) {
        if (i < repeat / 2) {
            ax1 = x[i * n + 1];
        }
        y[i * m] = x[0] + x[0] * ax1 * ax1;
    }

    return y;
}

TEST_F(CppADCGPatternTest, model3) {
    using namespace CppAD;

    size_t m = 1;
    size_t n = 2;
    size_t mExtra = 0; // equations not in loops

    testPatternDetection(model3, m, n, 6);

    testLibCreation(model3, m, n, 6, mExtra, "model3");
}

std::vector<ADCGD> model4Eq(std::vector<ADCGD>& x, size_t repeat) {
    size_t m = 4;
    size_t n = 4;
    size_t m2 = repeat * m;

    assert(x.size() == n * repeat);

    // dependent variable vector 
    std::vector<ADCGD> y(m2);

    for (size_t i = 0; i < repeat; i++) {
        y[i * m] = cos(x[i * n]) + 3 * x[1] * log(x[2]);
        y[i * m + 1] = x[i * n + 1] * x[i * n];
        y[i * m + 2] = x[i * n + 1] * x[i * n + 2];
        y[i * m + 3] = 5;
    }

    return y;
}

TEST_F(CppADCGPatternTest, Matcher4Eq) {
    using namespace CppAD;
    size_t m = 4;
    size_t n = 4;
    size_t mExtra = 0; // equations not in loops

    bool jacobian = true;
    bool hessian = false;

    testPatternDetection(model4Eq, m, n, 6);

    testLibCreation(model4Eq, m, n, 6, mExtra, "model4Eq", jacobian, hessian);
}

std::vector<ADCGD> modelCommonTmp(std::vector<ADCGD>& x, size_t repeat) {
    size_t m = 2;
    size_t n = 2;
    size_t m2 = repeat * m;

    // dependent variable vector 
    std::vector<ADCGD> y(m2);

    ADCGD tmp = x[1] * x[2];
    for (size_t i = 0; i < repeat; i++) {
        y[i * m] = cos(x[i * n]) + tmp;
        y[i * m + 1] = x[i * n + 1] * x[i * n] + tmp;
    }

    return y;
}

TEST_F(CppADCGPatternTest, CommonTmp) {
    using namespace CppAD;
    size_t m = 2;
    size_t n = 2;
    size_t mExtra = 0; // equations not in loops

    bool jacobian = false;
    bool hessian = false;

    testPatternDetection(modelCommonTmp, m, n, 6);

    testLibCreation(modelCommonTmp, m, n, 6, mExtra, "modelCommonTmp", jacobian, hessian);
}

std::vector<ADCGD> model4(std::vector<ADCGD>& x, size_t repeat) {
    size_t m = 2;
    size_t n = 2;
    size_t m2 = repeat * m;

    // dependent variable vector 
    std::vector<ADCGD> y(m2);

    for (size_t i = 0; i < repeat; i++) {
        ADCGD tmp = x[1] * x[i];
        y[i * m] = cos(x[i * n]) + tmp;
        y[i * m + 1] = x[i * n + 1] * x[i * n] + tmp;
    }

    return y;
}

TEST_F(CppADCGPatternTest, IndexedTmp) {
    using namespace CppAD;

    size_t m = 2;
    size_t n = 2;
    size_t mExtra = 0; // equations not in loops

    bool jacobian = false;
    bool hessian = false;

    testPatternDetection(model4, m, n, 6);

    testLibCreation(model4, m, n, 6, mExtra, "indexedTmp", jacobian, hessian);
}

std::vector<ADCGD> model5(std::vector<ADCGD>& x, size_t repeat) {
    size_t m = 2;
    size_t n = 2;
    size_t m2 = repeat * m;

    // dependent variable vector 
    std::vector<ADCGD> y(m2);

    for (size_t i = 0; i < repeat; i++) {
        ADCGD tmp = x[1] * x[i];
        y[i * m] = cos(x[i * n]) + tmp;

        if (i == 1) {
            for (size_t i2 = 0; i2 < repeat; i2++) {
                y[i2 * m + 1] = x[i2 * n + 1] * x[i2 * n] + tmp;
            }
        }
    }

    return y;
}

TEST_F(CppADCGPatternTest, DependentPatternMatcher5) {
    using namespace CppAD;

    size_t m = 2;
    size_t n = 2;
    size_t mExtra = 0; // equations not in loops

    bool jacobian = false;
    bool hessian = false;

    testPatternDetection(model5, m, n, 6, 2);

    testLibCreation(model5, m, n, 6, mExtra, "model5", jacobian, hessian);
}

std::vector<ADCGD> modelAtomic(std::vector<ADCGD>& x, size_t repeat, const std::vector<CGAbstractAtomicFun<double>*>& atoms) {
    size_t m = 2;
    size_t n = 2;
    size_t m2 = repeat * m;

    CGAbstractAtomicFun<Base>& atomic0 = *atoms[0];

    // dependent variable vector 
    std::vector<ADCGD> y(m2), ax(2), ay(1);

    for (size_t i = 0; i < repeat; i++) {
        y[i * m] = cos(x[i * n]);

        ax[0] = x[i * n];
        ax[1] = x[i * n + 1];
        atomic0(ax, ay);
        y[i * m + 1] = ay[0];
    }

    return y;
}

void atomicFunction(const std::vector<AD<double> >& x, std::vector<AD<double> >& y) {
    y[0] = x[1] * x[0];
}

TEST_F(CppADCGPatternTest, Atomic) {
    using namespace CppAD;

    size_t m = 2;
    size_t n = 2;

    // create atomic function
    std::vector<AD<double> > y(1), x(2);
    checkpoint<double> atomicfun("atomicFunc", atomicFunction, x, y);
    std::vector<atomic_base<double>*> atomics(1);
    atomics[0] = &atomicfun;


    testPatternDetectionWithAtomics(modelAtomic, atomics, m, n, 6);

    testLibCreationWithAtomics(modelAtomic, atomics, m, n, 6, "modelAtomic");
}

std::vector<ADCGD> modelAtomic2(std::vector<ADCGD>& x, size_t repeat, const std::vector<CGAbstractAtomicFun<double>*>& atoms) {
    size_t m = 1;
    size_t n = 2;
    size_t m2 = repeat * m;

    CGAbstractAtomicFun<Base>& atomic0 = *atoms[0];

    // dependent variable vector 
    std::vector<ADCGD> y(m2), ax(2), ay(1);

    for (size_t i = 0; i < repeat; i++) {
        ax[0] = x[i * n];
        if (i < repeat / 2) {
            ax[1] = x[i * n + 1];
        }
        atomic0(ax, ay);
        y[i * m] = ay[0];
    }

    return y;
}

TEST_F(CppADCGPatternTest, Atomic2) {
    using namespace CppAD;

    size_t m = 1;
    size_t n = 2;

    // create atomic function
    std::vector<AD<double> > y(1), x(2);
    checkpoint<double> atomicfun("atomicFunc2", atomicFunction, x, y);
    std::vector<atomic_base<double>*> atomics(1);
    atomics[0] = &atomicfun;


    testPatternDetectionWithAtomics(modelAtomic2, atomics, m, n, 6);

    testLibCreationWithAtomics(modelAtomic2, atomics, m, n, 6, "modelAtomic2");
}

std::vector<ADCGD> modelExtraFunc(std::vector<ADCGD>& x, size_t repeat) {
    size_t m = 2;
    size_t n = 2;
    size_t m2 = repeat * m;

    // dependent variable vector 
    std::vector<ADCGD> y(m2 + 1);

    for (size_t i = 0; i < repeat; i++) {
        y[i * m] = cos(x[i * n]);
        y[i * m + 1] = 10 * x[i * n + 1] * x[i * n];
    }

    y.back() = log(x[0] * x[1]) + 3 * x[3];

    return y;
}

TEST_F(CppADCGPatternTest, modelExtraFunc) {
    using namespace CppAD;

    size_t m = 2;
    size_t n = 2;
    size_t mExtra = 1; // equations not in loops

    bool jacobian = true;
    bool hessian = true;

    testPatternDetection(modelExtraFunc, m, n, 6);

    testLibCreation(modelExtraFunc, m, n, 6, mExtra, "modelExtraFunc", jacobian, hessian);
}