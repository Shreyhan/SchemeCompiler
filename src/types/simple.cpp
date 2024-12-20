#include "simple.h"

VarTypeSimple VarTypeSimple::BoolType = VarTypeSimple(VarTypeSimpleEnumeration::Bool);
VarTypeSimple VarTypeSimple::IntType = VarTypeSimple(VarTypeSimpleEnumeration::Int);
VarTypeSimple VarTypeSimple::RealType = VarTypeSimple(VarTypeSimpleEnumeration::Real);
VarTypeSimple VarTypeSimple::StringType = VarTypeSimple(VarTypeSimpleEnumeration::String);

std::unique_ptr<VarType> VarTypeSimple::Copy()
{
    return std::make_unique<VarTypeSimple>(type);
}

llvm::Type* VarTypeSimple::GetLLVMType(llvm::LLVMContext& ctx)
{
    switch (type)
    {
        case VarTypeSimpleEnumeration::Bool: return (llvm::Type*)llvm::Type::getInt1Ty(ctx);
        case VarTypeSimpleEnumeration::Int: return (llvm::Type*)llvm::Type::getInt32Ty(ctx);
        case VarTypeSimpleEnumeration::Real: return llvm::Type::getDoubleTy(ctx);
        case VarTypeSimpleEnumeration::String: return (llvm::Type*)llvm::Type::getInt8PtrTy(ctx);
    }
    return nullptr;
}

bool VarTypeSimple::Equals(VarType* other)
{
    VarTypeSimple* casted = dynamic_cast<VarTypeSimple*>(other);
    if (casted) // This is null if the cast fails (meaning it's not really a simple type).
    {
        return casted->type == type;
    }
    else return false;
}
