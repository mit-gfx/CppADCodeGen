#ifndef CPPAD_CODEGEN_ABS_OP_INCLUDED
#define CPPAD_CODEGEN_ABS_OP_INCLUDED

#include <cppad_codegen/local/ad_code_gen_name_provider.hpp>

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2011 Ciengis

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */


CPPAD_BEGIN_NAMESPACE
/*!
\file abs_op.hpp
Forward and reverse mode calculations for z = abs(x).
 */

/*!
Compute forward mode Taylor coefficient for result of op = AbsOp.

The C++ source code corresponding to this operation is
\verbatim
        z = abs(x)
\endverbatim

\copydetails forward_unary1_op
 */
template<class Base>
inline void forward_code_gen_abs_op(
std::ostream& s_out,
CodeGenNameProvider<Base>& n,
size_t d,
size_t i_z,
size_t i_x) {

    // check assumptions
    CPPAD_ASSERT_UNKNOWN(NumArg(AbsOp) == 1);
    CPPAD_ASSERT_UNKNOWN(NumRes(AbsOp) == 1);
    CPPAD_ASSERT_UNKNOWN(i_x < i_z);

    // Taylor coefficients corresponding to argument and result
    std::string sx_d = n.generateVarName(d, i_x);
    std::string sz_d = n.generateVarName(d, i_z);

    std::string aux;

    if (d > 0) {
        aux = n.tempBaseVarName();
        std::string sx_0 = n.generateVarName(0, i_x);

        // order that decides positive, negative or zero
        for (size_t j = 0; j <= d; j++) {
            std::string sx_j = n.generateVarName(j, i_x);
            if (j > 0) s_out << "} else ";
            s_out << "if (" << sx_j << " != " << n.zero() << ") {"
                    << aux << " = " << sx_j << n.endl();
        }
        s_out << "} else {"
                << aux << " = " << sx_0 << n.endl()
                << "}\n";
    } else {
        aux = sx_d;
    }

    s_out << "if(" << aux << " < " << n.zero() << ") "
            << sz_d << " = -" << sx_d << n.endl() <<
            "else if(" << aux << " > " << n.zero() << ")"
            << sz_d << " = " << sx_d << n.endl() <<
            "else "
            << sz_d << " = " << n.zero() << n.endl();
}

/*!
Compute zero order forward mode Taylor coefficient for result of op = AbsOp.

The C++ source code corresponding to this operation is
\verbatim
        z = abs(x)
\endverbatim

\copydetails forward_unary1_op_0
 */
template<class Base>
inline void forward_code_gen_abs_op_0(
std::ostream& s_out,
CodeGenNameProvider<Base>& n,
size_t i_z,
size_t i_x) {

    // check assumptions
    CPPAD_ASSERT_UNKNOWN(NumArg(AbsOp) == 1);
    CPPAD_ASSERT_UNKNOWN(NumRes(AbsOp) == 1);
    CPPAD_ASSERT_UNKNOWN(i_x < i_z);

    // Taylor coefficients corresponding to argument and result
    std::string sx = n.generateVarName(0, i_x);
    std::string sz = n.generateVarName(0, i_z);

    s_out << "if(" << sx << " < " << n.zero() << ") "
            << sz << " = -" << sx << n.endl() <<
            "else "
            << sz << " = " << sx << n.endl();
}

/*!
Compute reverse mode partial derivatives for result of op = AbsOp.

The C++ source code corresponding to this operation is
\verbatim
        z = abs(x)
\endverbatim

\copydetails reverse_unary1_op
 */
template<typename Base>
inline void reverse_code_gen_abs_op(
        std::ostream& s_out,
        CodeGenNameProvider<Base>& n,
        size_t d,
        size_t i_z,
        size_t i_x) {
    size_t j;

    // check assumptions
    CPPAD_ASSERT_UNKNOWN(NumArg(AbsOp) == 1);
    CPPAD_ASSERT_UNKNOWN(NumRes(AbsOp) == 1);
    CPPAD_ASSERT_UNKNOWN(i_x < i_z);

    std::string aux, kk;

    // order that decides positive, negative or zero
    aux = n.tempBaseVarName();
    kk = n.tempIntegerVarName();
    std::string sx_0 = n.generateVarName(0, i_x);

    // order that decides positive, negative or zero
    for (size_t j = 0; j <= d; j++) {
        std::string sx_j = n.generateVarName(j, i_x);
        if (j > 0) s_out << "} else ";
        s_out << "if (" << sx_j << " != " << n.zero() << ") {"
                << aux << " = " << sx_j << n.endl()
                << kk << " = " << j << n.endl();
    }
    s_out << "} else {"
            << aux << " = " << sx_0 << n.endl()
            << kk << " = 0" << n.endl()
            << "}\n";


    s_out << "if(" << aux << " > " << n.zero() << ") {";
    {
        for (j = 0; j <= d; j++) { // partial of z w.r.t y is +1
            std::string px_j = n.generatePartialName(j, i_x);
            std::string pz_j = n.generatePartialName(j, i_z);

            if (j > 0) s_out << "} else ";
            s_out << "if (" << j << " >= " << kk << ") {"
                    << px_j << " += " << pz_j << n.endl();
        }
        s_out << "}\n";
    }
    s_out << "} else if (" << aux << " < " << n.zero() << ") {";
    {
        for (j = 0; j <= d; j++) { // partial of z w.r.t y is -1
            std::string px_j = n.generatePartialName(j, i_x);
            std::string pz_j = n.generatePartialName(j, i_z);

            if (j > 0) s_out << "} else ";
            s_out << "if (" << j << " >= " << kk << ") {"
                    << px_j << " -= " << pz_j << n.endl();
        }
        s_out << "}\n";
    }
    s_out << "}\n";
}

CPPAD_END_NAMESPACE
#endif
