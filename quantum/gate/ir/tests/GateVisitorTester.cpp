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
#define BOOST_TEST_MODULE FateVisitorTester

#include <boost/test/included/unit_test.hpp>
#include "Hadamard.hpp"
#include "CNOT.hpp"

using namespace xacc;

using namespace xacc::quantum;

class TestVisitor:
		public BaseInstructionVisitor, // ALWAYS REQUIRED
		public InstructionVisitor<CNOT>,
		public InstructionVisitor<Hadamard> {
public:

	virtual void visit(CNOT& cnot) {
		std::cout << "HELLO WORLD CNOT\n";
	}

    virtual void visit(Hadamard& h) {
		std::cout << "HELLO WORLD HADAMARD\n";
	}

};

BOOST_AUTO_TEST_CASE(checkVisit) {
	auto v = std::make_shared<TestVisitor>();
	std::shared_ptr<GateInstruction> c = std::make_shared<CNOT>(0,1);
	std::shared_ptr<GateInstruction> h = std::make_shared<Hadamard>(0);
	c->accept(v);
	h->accept(v);
}
