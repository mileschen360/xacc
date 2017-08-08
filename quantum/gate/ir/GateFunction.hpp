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
#ifndef QUANTUM_GATEQIR_QFUNCTION_HPP_
#define QUANTUM_GATEQIR_QFUNCTION_HPP_

#include "Registry.hpp"
#include "Function.hpp"
#include "Utils.hpp"

namespace xacc {
namespace quantum {

/**
 * The GateFunction is a QFunction for gate-model
 * quantum computing. It is composed of QInstructions that
 * are themselves derivations of the GateInstruction class.
 */
class GateFunction: public virtual Function {

protected:

	/**
	 * The name of this function
	 */
	std::string functionName;

	std::list<InstPtr> instructions;

	std::vector<InstructionParameter> parameters;

	/**
	 * Map of Instruction Index to ( Instruction's Runtime Parameter Index, Dependent Variable name)
	 */
	std::map<int, std::pair<int, std::string>> cachedVariableInstructions;

public:

	/**
	 * The constructor, takes the function unique id and its name.
	 *
	 * @param id
	 * @param name
	 */
	GateFunction(const std::string& name) :
			functionName(name), parameters(
					std::vector<InstructionParameter> { }) {
	}
	GateFunction(const std::string& name,
			std::vector<InstructionParameter> params) :
			functionName(name), parameters(params) {
	}

	virtual const int nInstructions() {
		return instructions.size();
	}

	virtual InstPtr getInstruction(const int idx) {
		if (instructions.size() > idx) {
			return *std::next(instructions.begin(), idx);
		} else {
			XACCError("Invalid instruction index.");
		}
	}

	virtual std::list<InstPtr> getInstructions() {
		return instructions;
	}

	virtual void removeInstruction(const int idx) {
		instructions.remove(getInstruction(idx));
	}

	/**
	 * Add an instruction to this quantum
	 * intermediate representation.
	 *
	 * @param instruction
	 */
	virtual void addInstruction(InstPtr instruction) {
		instructions.push_back(instruction);
	}

	/**
	 * Replace the given current quantum instruction
	 * with the new replacingInst quantum Instruction.
	 *
	 * @param currentInst
	 * @param replacingInst
	 */
	virtual void replaceInstruction(const int idx, InstPtr replacingInst) {
		std::replace(instructions.begin(), instructions.end(),
				getInstruction(idx), replacingInst);
	}

	virtual void insertInstruction(const int idx, InstPtr newInst) {
		auto iter = std::next(instructions.begin(), idx);
		instructions.insert(iter, newInst);
	}

	/**
	 * Return the name of this function
	 * @return
	 */
	virtual const std::string getName() {
		return functionName;
	}

	/**
	 * Return the qubits this function acts on.
	 * @return
	 */
	virtual const std::vector<int> bits() {
		return std::vector<int> { };
	}

	/**
	 * Return an assembly-like string representation for this function .
	 * @param bufferVarName
	 * @return
	 */
	virtual const std::string toString(const std::string& bufferVarName) {
		std::string retStr = "";
		for (auto i : instructions) {
			retStr += i->toString(bufferVarName) + "\n";
		}
		return retStr;
	}

	virtual InstructionParameter getParameter(const int idx) const {
		if (idx + 1 > parameters.size()) {
			XACCError("Invalid Parameter requested.");
		}

		return parameters[idx];
	}

	virtual void setParameter(const int idx, InstructionParameter& p) {
		if (idx + 1 > parameters.size()) {
			XACCError("Invalid Parameter requested.");
		}

		parameters[idx] = p;
	}

	virtual std::vector<InstructionParameter> getParameters() {
		return parameters;
	}

	virtual bool isParameterized() {
		return nParameters() > 0;
	}

	virtual const int nParameters() {
		return parameters.size();
	}


	virtual void evaluateVariableParameters(
			std::vector<InstructionParameter> runtimeParameters) {

		std::map<std::string, InstructionParameter> varToValMap;

		int i = 0;
		for (auto funcParam : parameters) {
			varToValMap.insert(
					std::make_pair(boost::get<std::string>(funcParam),
							runtimeParameters[i]));
			i++;
		}

		for (const auto& gateIdVarName : cachedVariableInstructions) {
			auto inst = getInstruction(gateIdVarName.first);
			auto varInstDependsOn = gateIdVarName.second.second;
			auto instParamIdx = gateIdVarName.second.first;

			int indexOfRuntimeParam, counter = 0;
			for (auto p : parameters) {
				if (boost::get<std::string>(p) == varInstDependsOn) {
					indexOfRuntimeParam = counter;
					break;
				}
				counter++;
			}

			inst->setParameter(instParamIdx, runtimeParameters[indexOfRuntimeParam]);
		}

		i = 0;
		for (auto inst : instructions) {
			if (inst->isComposite()) {
				std::dynamic_pointer_cast<Function>(inst)->evaluateVariableParameters(
						runtimeParameters);
			} else if (inst->isParameterized() && inst->getName() != "Measure") {

				for (int j = 0; j < inst->nParameters(); ++j) {
					auto instParam = inst->getParameter(j);

					if (instParam.which() == 3) {
						// This is a variable
						auto variable = boost::get<std::string>(instParam);

						auto runtimeParameter = varToValMap[variable];

						inst->setParameter(j, runtimeParameter);

						cachedVariableInstructions.insert(std::make_pair(i, std::make_pair(j, variable)));
					}
				}
			}
			i++;
		}
	}

	DEFINE_VISITABLE()

};

}
}

#endif
