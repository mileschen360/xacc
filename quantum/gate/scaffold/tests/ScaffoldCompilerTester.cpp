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
 *   * Neither the name of the <organization> nor the
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

BOOST_AUTO_TEST_CASE(checkScaffoldCompiler) {
	auto compiler = qci::common::AbstractFactory::createAndCast<xacc::ICompiler>("compiler", "scaffold");
	BOOST_VERIFY(compiler);

	const std::string src("__qpu__ teleport () {\n"
						"   qbit qs[3];\n"
						"   cbit cs[2];\n"
						"   H(qs[1]);\n"
						"   CNOT(qs[1],qs[2]);\n"
						"   CNOT(qs[0], qs[1]);\n"
						"   H(qs[0]);\n"
						"   cs[0] = MeasZ(qs[0]);\n"
						"   cs[1] = MeasZ(qs[1]);\n"
//						"   if(cs[0]) {\n"
//						"      X(qs[2]);\n"
//						"   }\n"
//						"   if(cs[1]) {\n"
//						"      Z(qs[2]);\n"
//						"   }\n"
						"}\n");

	compiler->compile(src);
}