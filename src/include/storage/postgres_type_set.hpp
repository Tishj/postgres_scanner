//===----------------------------------------------------------------------===//
//                         DuckDB
//
// storage/postgres_type_set.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "storage/postgres_catalog_set.hpp"
#include "storage/postgres_type_entry.hpp"

namespace duckdb {
struct CreateTableInfo;
class PostgresResult;
class PostgresSchemaEntry;
struct PGTypeInfo;

class PostgresTypeSet : public PostgresInSchemaSet {
public:
	explicit PostgresTypeSet(PostgresSchemaEntry &schema, unique_ptr<PostgresResultSlice> enum_result = nullptr,
	                         unique_ptr<PostgresResultSlice> composite_type_result = nullptr);

public:
	optional_ptr<CatalogEntry> CreateType(ClientContext &context, CreateTypeInfo &info);

	static string GetInitializeEnumsQuery(PostgresVersion version);
	static string GetInitializeCompositesQuery();

protected:
	bool HasInternalDependencies() const override {
		// composite types can refer to other types
		return true;
	}
	void LoadEntries(ClientContext &context) override;

	void CreateEnum(PostgresResult &result, idx_t start_row, idx_t end_row);
	void CreateCompositeType(PostgresTransaction &transaction, PostgresResult &result, idx_t start_row, idx_t end_row);

	void InitializeEnums(PostgresResultSlice &enums);
	void InitializeCompositeTypes(PostgresTransaction &transaction, PostgresResultSlice &composite_types);

protected:
	unique_ptr<PostgresResultSlice> enum_result;
	unique_ptr<PostgresResultSlice> composite_type_result;
};

} // namespace duckdb
