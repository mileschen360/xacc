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
#ifndef XACC_COMPILER_INSTRUCTION_HPP_
#define XACC_COMPILER_INSTRUCTION_HPP_
#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include "Utils.hpp"

namespace xacc {

/**
 * This is the base class for all Instruction
 * Visitor implementations. If you want to create
 * a visitor, you must inherit from this class.
 */
class BaseInstructionVisitor {
public:
	virtual ~BaseInstructionVisitor() {}
};

/**
 * Implementations of BaseInstructionVisitor must
 * inherit from this template class in order to specify
 * the actual Instruction subclasses they would like to
 * visit.
 */
template<class T>
class InstructionVisitor {
public:

	/**
	 * The visit method to be called by
	 * specialized Instructions
	 * @param T The instruction instances of type T
	 */
	virtual void visit(T&) = 0;
	virtual ~InstructionVisitor() {}
};

/**
 * All Instruction specializations that need to be visited by concrete
 * InstructionVisitors must invoke the DEFINE_VISITABLE macro in the
 * public scope of their class definition.
 */
class BaseInstructionVisitable {
public:
	virtual void accept(std::shared_ptr<BaseInstructionVisitor> visitor) = 0;
	virtual ~BaseInstructionVisitable() {}
protected:
	template<class T>
	static void acceptImpl(T& visited, std::shared_ptr<BaseInstructionVisitor> visitor) {
		auto castedVisitor = std::dynamic_pointer_cast<InstructionVisitor<T>>(visitor);
		if (castedVisitor) {
			castedVisitor->visit(visited);
			return;
		}
	}

#define DEFINE_VISITABLE() \
            virtual void accept(std::shared_ptr<BaseInstructionVisitor> v) \
                { acceptImpl(*this, v); }

};

/**
 * The Instruction interface is the base for all executable
 * Accelerator instructions in the XACC specification. It provides
 * an abstraction to describe the instructions unique name and the
 * accelerator bits it operates on.
 */
class Instruction : public BaseInstructionVisitable {

public:

	/**
	 * Return the unique name of this Instruction
	 * @return
	 */
	virtual const std::string getName() = 0;

	/**
	 * Return this instruction definition as a string.
	 *
	 * @return
	 */
	virtual const std::string toString(const std::string& bufferVarName) = 0;

	/**
	 * Return the accelerator bits that this instruction
	 * operates on.
	 *
	 * @return
	 */
	virtual const std::vector<int> bits() = 0;

	/**
	 * Return true if this Instruction contains other
	 * instructions as part of its definition.
	 * @return
	 */
	virtual bool isComposite() { return false; }

	/**
	 * Return if this instruction is enabled, ie,
	 * should be executed.
	 *
	 * @return
	 */
	virtual bool isEnabled() { return true; }

	/**
	 * Disable this instruction.
	 *
	 */
	virtual void disable() {}

	/**
	 * Enable this instruction
	 */
	virtual void enable() {}

	/**
	 * The destructor
	 */
	virtual ~Instruction() {
	}
};

}
#endif
