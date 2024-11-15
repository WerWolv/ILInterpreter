#pragma once

#include <filesystem>
#include <map>
#include <vector>

#include <ili/types.hpp>
#include <ili/data_structures.hpp>
#include <ili/tables.hpp>

namespace ili {

    class Executable {
    public:
        Executable() = default;
        explicit Executable(const std::filesystem::path &path);
        explicit Executable(std::span<const u8> data);

        [[nodiscard]] const Section* getVirtualSection(u64 rva) const;

        [[nodiscard]] table::Token getEntrypointMethodToken() const;

        [[nodiscard]] std::span<const u8> getUserStringBytes(u32 index) const;
        [[nodiscard]] std::span<const u8> getBlobBytes(u32 index) const;
        [[nodiscard]] std::span<const u8> getSectionBytes(const Section& section, u64 rva, std::size_t size) const;

        [[nodiscard]] std::string_view getString(table::StringIndex index) const;
        [[nodiscard]] std::string getUserString(table::UserStringIndex index) const;

        struct QualifiedName {
            std::string_view assemblyName;
            std::string_view namespaceName;
            std::string_view typeName;
            std::string_view methodName;

            [[nodiscard]] operator std::string() const {
                return fmt::format("[{}]{}.{}::{}", assemblyName, namespaceName, typeName, methodName);
            }

            bool operator<(const QualifiedName& other) const {
                return assemblyName < other.assemblyName &&
                       namespaceName < other.namespaceName &&
                       typeName < other.typeName &&
                       methodName < other.methodName;
            }
        };

        const table::TypeDef* getTypeDefOfMethod(const table::MethodDef *methodToFind) const;
        const table::ClassLayout* getClassLayoutOfType(const table::TypeDef *typeDef) const;

        [[nodiscard]] QualifiedName getQualifiedMethodName(table::Token methodToken) const;
        [[nodiscard]] const std::map<QualifiedName, const table::MethodDef*>& getMethods() const {
            return m_methods;
        }

        [[nodiscard]] u64 getStackSize() const;

        [[nodiscard]] u16 findTypeTokenForMethodToken(table::Token methodToken);

        template<table::TableType T>
        const T* getTableEntry(table::Token token) const {
            if (token.getId() != T::ID) [[unlikely]]
                return nullptr;

            const auto id = token.getId();
            const auto index = token.getIndex() - 1;

            if (id > m_streams.tilde.tableData.size()) [[unlikely]]
                return nullptr;

            const auto &table = m_streams.tilde.tableData[token.getId()];
            if (index > table.size()) [[unlikely]]
                return nullptr;

            return reinterpret_cast<T*>(table[index].data());
        }

    private:
        void parse();
        void parseHeaders();
        void parseSections();
        void parseStreamHeaders();
        void parseStreams();
        void parseMethods();

    private:
        std::vector<u8> m_data;
        u8 *m_parsePointer = nullptr;

        DOSHeader *m_dosHeader = nullptr;
        COFFHeader *m_coffHeader = nullptr;
        std::span<DataDirectory> m_directories;
        std::vector<Section> m_sections;
        OptionalHeader *m_optionalHeader = nullptr;
        CRLRuntimeHeader *m_crlRuntimeHeader = nullptr;
        Metadata m_metadata;
        u8 *m_metadataPointer;
        std::vector<StreamHeader> m_streamHeaders;
        std::map<QualifiedName, const table::MethodDef*> m_methods;
        std::array<u64, 64> m_tableRowCounts;

        struct {
            struct {
                std::vector<std::vector<std::span<u8>>> tableData;
            } tilde;
            struct {
                std::span<const u8> data;
                bool largeIndices;
            } string, userString, guid, blob;
        } m_streams;
    };

}
