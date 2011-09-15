#include "llvm/state.hpp"

#include "llvm/method_info.hpp"
#include "llvm/jit_context.hpp"

#include "builtin/compiledmethod.hpp"
#include "object_utils.hpp"

namespace rubinius {
  llvm::AllocaInst* JITMethodInfo::create_alloca(const llvm::Type* type, llvm::Value* size,
                                           const llvm::Twine& name)
  {
    return new llvm::AllocaInst(type, size, name,
        function()->getEntryBlock().getTerminator());
  }

  JITMethodInfo::JITMethodInfo(jit::Context& ctx, CompiledMethod* cm, VMMethod* v,
                  JITMethodInfo* parent)
    : context_(ctx)
    , entry_(0)
    , call_frame_(0)
    , stack_(0)
    , args_(0)
    , previous_(0)
    , profiling_entry_(0)
    , parent_info_(parent)
    , creator_info_(0)
    , use_full_scope_(false)
    , inline_block_(0)
    , block_info_(0)
    , method_(&ctx.state()->roots())
    , return_pad_(0)
    , return_phi_(0)
    , self_class_(&ctx.state()->roots())
    , vmm(v)
    , is_block(false)
    , inline_return(0)
    , return_value(0)
    , inline_policy(0)
    , fin_block(0)
    , called_args(-1)
    , stack_args(0)
    , root(0)
  {
    method_.set(cm);
    self_class_.set(nil<Object>());
  }

  void JITMethodInfo::setup_return() {
    return_pad_ = llvm::BasicBlock::Create(context_.state()->ctx(), "return_pad", function());
    return_phi_ = llvm::PHINode::Create(
       context_.state()->ptr_type("Object"), "return_phi", return_pad_);
  }

  llvm::BasicBlock* JITMethodInfo::new_block(const char* name) {
    return llvm::BasicBlock::Create(context_.state()->ctx(), name, function());
  }
}
