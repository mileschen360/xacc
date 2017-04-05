
/***********************************************************************************
 * Copyright (c) 2016, UT-Battelle
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the name of the xacc nor the
 *     names of its contributors may be used to endorse or promote products
 *     derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Contributors:
 *   Initial API and implementation - Alex McCaskey
 *
 **********************************************************************************/
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE ScaffoldCompilerTester

#include <boost/test/included/unit_test.hpp>
#include "AbstractFactory.hpp"
#include "Compiler.hpp"
#include "QasmToGraph.hpp"
#include "GraphIR.hpp"
#include "QuilCompiler.hpp"

using namespace qci::common;
using namespace xacc::quantum;

BOOST_AUTO_TEST_CASE(checkSimpleCompile) {
	using GraphType = QuantumCircuit;

	auto compiler = qci::common::AbstractFactory::createAndCast<xacc::ICompiler>("compiler", "quil");
	BOOST_VERIFY(compiler);

	const std::string src("__qpu__ bellstate (qbit qubits) {\n"
						"p = Program()\n"
						"p.inst(H(0))\n"
						"p.inst(CNOT(0, 1))\n"
						"}\n");

	auto ir = compiler->compile(src);
//	BOOST_VERIFY(ir);


}

BOOST_AUTO_TEST_CASE(checkComplex_QFT) {
	using GraphType = QuantumCircuit;

	const std::string src("__qpu__ qftKernel(qbit qreg) {\n"
		"import math\n"
		"def bit_reversal(qubits):\n"
		"    p = Program()\n"
		"    n = len(qubits)\n"
		"    for i in xrange(n / 2):\n"
		"        p.inst(SWAP(qubits[i], qubits[-i - 1]))\n"
		"    return p\n"
		"def _core_qft(qubits, coeff):\n"
		"    q = qubits[0]\n"
		"    qs = qubits[1:]\n"
		"    if 1 == len(qubits):\n"
		"        return [H(q)]\n"
		"    else:\n"
		"        n = 1 + len(qs)\n"
		"        cR = []\n"
		"        for idx, i in enumerate(xrange(n - 1, 0, -1)):\n"
		"            q_idx = qs[idx]\n"
		"            angle = math.pi / 2 ** (n - i)\n"
		"            cR.append(CPHASE(coeff * angle)(q, q_idx))\n"
		"        return _core_qft(qs, coeff) + list(reversed(cR)) + [H(q)]\n"
		"def qft(qubits):\n"
		"    p = Program().inst(_core_qft(qubits, 1))\n"
		"    return p + bit_reversal(qubits)\n"
		"p = qft([0,1,2,3,4,5,6])\n"
		"}\n");

	auto compiler = qci::common::AbstractFactory::createAndCast<xacc::ICompiler>("compiler", "quil");
	BOOST_VERIFY(compiler);
	auto ir = compiler->compile(src);

}

BOOST_AUTO_TEST_CASE(checkCodeWithArgument) {
	using GraphType = QuantumCircuit;

}
