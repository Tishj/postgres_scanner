//===----------------------------------------------------------------------===//
//                         DuckDB
//
// postgres_connection.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "postgres_utils.hpp"
#include "postgres_result.hpp"

namespace duckdb {
class PostgresBinaryWriter;
struct PostgresBinaryReader;
class PostgresSchemaEntry;
class PostgresStatement;
class PostgresResult;
struct IndexInfo;

struct OwnedPostgresConnection {
	explicit OwnedPostgresConnection(PGconn *conn = nullptr) : connection(conn) {}
	~OwnedPostgresConnection() {
		if (!connection) {
			return;
		}
		PQfinish(connection);
		connection = nullptr;
	}

	PGconn *connection;
};

class PostgresConnection {
public:
	explicit PostgresConnection(shared_ptr<OwnedPostgresConnection> connection = nullptr);
	~PostgresConnection();
	// disable copy constructors
	PostgresConnection(const PostgresConnection &other) = delete;
	PostgresConnection &operator=(const PostgresConnection &) = delete;
	//! enable move constructors
	PostgresConnection(PostgresConnection &&other) noexcept;
	PostgresConnection &operator=(PostgresConnection &&) noexcept;

public:
	static PostgresConnection Open(const string &connection_string);
	bool TryPrepare(const string &query, PostgresStatement &result, string &error);
	PostgresStatement Prepare(const string &query);
	void Execute(const string &query);
	unique_ptr<PostgresResult> TryQuery(const string &query);
	unique_ptr<PostgresResult> Query(const string &query);

	vector<string> GetEntries(string entry_type);
	void GetViewInfo(const string &view_name, string &sql);
	void GetIndexInfo(const string &index_name, string &sql, string &table_name);
	//! Gets the max row id of a table, returns false if the table does not have a
	//! rowid column
	bool GetMaxRowId(const string &table_name, idx_t &row_id);
	bool ColumnExists(const string &table_name, const string &column_name);
	vector<IndexInfo> GetIndexInfo(const string &table_name);

	void BeginCopyTo(const string &schema_name, const string &table_name, const vector<string> &column_names);
	void CopyData(data_ptr_t buffer, idx_t size);
	void CopyData(PostgresBinaryWriter &writer);
	void CopyChunk(DataChunk &chunk);
	void FinishCopyTo();

	void BeginCopyFrom(PostgresBinaryReader &reader, const string &query);

	bool IsOpen();
	void Close();

	shared_ptr<OwnedPostgresConnection> GetConnection() {
		return connection;
	}
	string GetDSN() {
		return dsn;
	}

	PGconn *GetConn() {
		if (!connection || !connection->connection) {
			throw InternalException("PostgresConnection::GetConn - no connection available");
		}
		return connection->connection;
	}

private:
	shared_ptr<OwnedPostgresConnection> connection;
	string dsn;
};

} // namespace duckdb
