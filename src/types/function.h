#pragma once

#include "../varType.h"

// Type that represents a function.
class VarTypeFunction : public VarType
{
private:

    // Prefetched LLVM type.
    llvm::Type* funcType = nullptr;

public:

    // Return type.
    std::unique_ptr<VarType> returnType;

    // Parameter types.
    std::vector<std::unique_ptr<VarType>> parameterTypes;

    // Create a function type.
    // returnType: What type the function returns.
    // parameterTypes: Types of each parameter.
    VarTypeFunction(std::unique_ptr<VarType> returnType, std::vector<std::unique_ptr<VarType>> parameterTypes) : returnType(std::move(returnType)), parameterTypes(std::move(parameterTypes)) {}

    // Virtual functions. See base class for details.
    virtual std::unique_ptr<VarType> Copy() override;
    virtual llvm::Type* GetLLVMType(llvm::LLVMContext& ctx) override;
    virtual bool Equals(VarType* other) override;

};
