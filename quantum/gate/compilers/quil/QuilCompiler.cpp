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
#include "GraphIR.hpp"
#include <regex>
#include "QasmToGraph.hpp"
#include "QuantumCircuit.hpp"
#include "QuilCompiler.hpp"
#include <boost/python.hpp>

namespace xacc {

namespace quantum {

void QuilCompiler::modifySource() {

	std::cout << "SRC:\n" << kernelSource << "\n";

	auto endFuncLine =  kernelSource.find_first_of("{");
	auto endFuncTotal = kernelSource.find_first_of("}");
	auto quilCode = kernelSource.substr(endFuncLine+1, (endFuncTotal - endFuncLine) - 1);

	for (auto it = typeToVarKernelArgs.begin(); it != typeToVarKernelArgs.end(); ++it) {
		std::cout << "HELLOWORLD: " << it->first << ", " << it->second << "\n";
	}

	quilCode = "import math\nfrom pyquil.quil import Program\nfrom pyquil.gates import *\n" + quilCode + "qasm = p.out()";

	kernelSource = quilCode;
}

std::shared_ptr<IR> QuilCompiler::compile() {

	using QuilGraphIR = GraphIR<QuantumCircuit>;

	using namespace boost::python;

	Py_Initialize();
	// Retrieve the main module.
	object main = import("__main__");
	object main_namespace = main.attr("__dict__");

	std::cout << "Code:\n" << kernelSource << "\n";

	try {
		exec(kernelSource.c_str(), main_namespace);
	} catch (boost::python::error_already_set e) {
		PyErr_Print();
	}

	std::string qasm = extract<std::string>(main_namespace["qasm"]);

	Py_Finalize();

	std::cout << "THE QASM STR: \n" << qasm << "\n";

	// Create a GraphIR instance from that graph
	auto graphIR = std::make_shared<QuilGraphIR>();

	// Return the IR.
	return graphIR;
}

} // end namespace quantum

} // end namespace xacc

// Required in CPP file to be discovered by factory pattern
REGISTER_QCIOBJECT_WITH_QCITYPE(xacc::quantum::QuilCompiler, "compiler",
		"quil");

