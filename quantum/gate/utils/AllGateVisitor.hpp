/***********************************************************************************
 * Copyright (c) 2017, UT-Battelle
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
#ifndef QUANTUM_GATE_ALLGATEVISITOR_HPP_
#define QUANTUM_GATE_ALLGATEVISITOR_HPP_

#include "InstructionIterator.hpp"
#include "Hadamard.hpp"
#include "CNOT.hpp"
#include "X.hpp"
#include "Y.hpp"
#include "Z.hpp"
#include "ConditionalFunction.hpp"
#include "Rz.hpp"
#include "Rx.hpp"
#include "Ry.hpp"
#include "CPhase.hpp"
#include "Swap.hpp"
#include "Measure.hpp"

namespace xacc {
namespace quantum {

/**
 * FIXME write this
 */
class AllGateVisitor:
		public BaseInstructionVisitor,
		public InstructionVisitor<GateFunction>,
		public InstructionVisitor<Hadamard>,
		public InstructionVisitor<CNOT>,
		public InstructionVisitor<Rz>,
		public InstructionVisitor<Rx>,
		public InstructionVisitor<Ry>,
		public InstructionVisitor<ConditionalFunction>,
		public InstructionVisitor<X>,
		public InstructionVisitor<Y>,
		public InstructionVisitor<Z>,
		public InstructionVisitor<CPhase>,
		public InstructionVisitor<Swap>,
		public InstructionVisitor<Measure> {

};
}
}
#endif
