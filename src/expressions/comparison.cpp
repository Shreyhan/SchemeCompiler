#include "comparison.h"

std::unique_ptr<VarType> ASTExpressionComparison::ReturnType(ASTFunction* func)
{
    return VarTypeSimple::BoolType.Copy();
}

bool ASTExpressionComparison::IsLValue(ASTFunction* func)
{
    return false;
}

llvm::Value* ASTExpressionComparison::Compile(llvm::Module& mod, llvm::IRBuilder<>& builder, ASTFunction* func)
{
    VarTypeSimple* returnType;
    if (!ASTExpression::CoerceTypes(ast, func, a1, a2, returnType)) // This will force our arguments to be the same type and outputs which one it is.
            throw std::runtime_error("ERROR: Can not coerce types in comparison expression! Are they all booleans, ints, and floats?");

    // Get values. Operations only work on R-Values.
    auto a1Val = a1->CompileRValue(mod, builder, func);
    auto a2Val = a2->CompileRValue(mod, builder, func);

    // Int type, do int comparisons.
    if (returnType->Equals(&VarTypeSimple::IntType))
    {
        switch (type)
        {
            case Equal: return builder.CreateCmp(llvm::CmpInst::ICMP_EQ, a1Val, a2Val);
            case NotEqual: return builder.CreateCmp(llvm::CmpInst::ICMP_NE, a1Val, a2Val);
            // Hm, some of these cases in these functions appear to be missing...
            case LessThan: return builder.CreateCmp(llvm::CmpInst::ICMP_SLT, a1Val, a2Val);
            case LessThanOrEqual: return builder.CreateCmp(llvm::CmpInst::ICMP_SLE, a1Val, a2Val);
            case GreaterThan: return builder.CreateCmp(llvm::CmpInst::ICMP_SGT, a1Val, a2Val);
            case GreaterThanOrEqual: return builder.CreateCmp(llvm::CmpInst::ICMP_SGE, a1Val, a2Val);
        }
    }

    // Float type, do float comparisons.
    if (returnType->Equals(&VarTypeSimple::RealType))
    {
        switch (type)
        {
            case Equal: return builder.CreateCmp(llvm::CmpInst::FCMP_OEQ, a1Val, a2Val); // Use ordered operations to not allow NANS.
            case NotEqual: return builder.CreateCmp(llvm::CmpInst::FCMP_ONE, a1Val, a2Val);
            case LessThan: return builder.CreateCmp(llvm::CmpInst::FCMP_OLT, a1Val, a2Val);
            case LessThanOrEqual: return builder.CreateCmp(llvm::CmpInst::FCMP_OLE, a1Val, a2Val);
            case GreaterThan: return builder.CreateCmp(llvm::CmpInst::FCMP_OGT, a1Val, a2Val);
            case GreaterThanOrEqual: return builder.CreateCmp(llvm::CmpInst::FCMP_OGE, a1Val, a2Val);

        }
    }

    // How did we get here?
    throw std::runtime_error("ERROR: Did not return value from comparison. Unsuccessful coercion of values or invalid comparison type!");
    
}

std::string ASTExpressionComparison::ToString(const std::string& prefix)
{
    std::string op = "";
    switch (type)
    {
        case Equal: op = "="; break;
        case NotEqual: op = "!="; break;
        case LessThan: op = "<"; break;
        case LessThanOrEqual: op = "<="; break;
        case GreaterThan: op = ">"; break;
        case GreaterThanOrEqual: op = ">="; break;
    }
    std::string ret = "(" + op + ")\n";
    ret += prefix + "├──" + a1->ToString(prefix + "│  ");
    ret += prefix + "└──" + a2->ToString(prefix + "   ");
    return ret;
}