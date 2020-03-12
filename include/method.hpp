#pragma once

#include "types.hpp"
#include "context.hpp"
#include "tables.hpp"

namespace ili  {


    class Method {
    public:
        Method(Context &ctx, u32 methodToken);
        ~Method();
        VariableBase* run();

    private:
        Context &m_ctx;
        table_method_def_t *m_methodDef;

        u8 *m_programCounter;

        VariableBase *m_localVariable[0xFF] = { nullptr };
        VariableBase *m_returnVariable = nullptr;


        // General Operations

        template<typename T>
        T getNext();

        // Instruction Implementations

        void stloc(u8 id);
        void ldloc(u8 id);
        template<typename T>
        void ldc(Type type, T num);
    };
}

