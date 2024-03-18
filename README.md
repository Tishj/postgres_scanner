# DuckDB Postgres extension

The Postgres extension allows DuckDB to directly read and write data from a running Postgres database instance. The data can be queried directly from the underlying Postgres database. Data can be loaded from Postgres tables into DuckDB tables, or vice versa.

## Reading Data from Postgres

To make a Postgres database accessible to DuckDB use the `ATTACH` command:

```sql
ATTACH 'dbname=postgresscanner' AS postgres_db (TYPE postgres);
```

The `ATTACH` command takes as input a [`libpq` connection string](https://www.postgresql.org/docs/current/libpq-connect.html#LIBPQ-CONNSTRING) - which is a set of `key=value` pairs separated by spaces. Below are some example connection strings and commonly used parameters. A full list of available parameters can be found [in the Postgres documentation](https://www.postgresql.org/docs/current/libpq-connect.html#LIBPQ-PARAMKEYWORDS).

```
dbname=postgresscanner
host=localhost port=5432 dbname=mydb connect_timeout=10
```

|   Name   |             Description              |    Default     |
|----------|--------------------------------------|----------------|
| host     | Name of host to connect to           | localhost      |
| hostaddr | Host IP address                      | localhost      |
| port     | Port Number                          | 5432           |
| user     | Postgres User Name                   | [OS user name] |
| password | Postgres Password                    |                |
| dbname   | Database Name                        | [user]         |
| passfile | Name of file passwords are stored in | ~/.pgpass      |


The tables in the file can be read as if they were normal DuckDB tables, but the underlying data is read directly from Postgres at query time.

```sql
D SHOW ALL TABLES;
┌───────────────────────────────────────┐
│                 name                  │
│                varchar                │
├───────────────────────────────────────┤
│ uuids                                 │
└───────────────────────────────────────┘
D SELECT * FROM postgres_db.uuids;
┌──────────────────────────────────────┐
│                  u                   │
│                 uuid                 │
├──────────────────────────────────────┤
│ 6d3d2541-710b-4bde-b3af-4711738636bf │
│ NULL                                 │
│ 00000000-0000-0000-0000-000000000001 │
│ ffffffff-ffff-ffff-ffff-ffffffffffff │
└──────────────────────────────────────┘
```

For more information on how to use the connector, refer to the [Postgres documentation on the website](https://duckdb.org/docs/extensions/postgres).

## Building & Loading the Extension

The DuckDB submodule must be initialized prior to building.

```bash
git submodule init
git pull --recurse-submodules
```

To build, type 
```
make
```

To run, run the bundled `duckdb` shell:
```
 ./build/release/duckdb -unsigned  # allow unsigned extensions
```

Then, load the Postgres extension like so:
```SQL
LOAD 'build/release/extension/postgres_scanner/postgres_scanner.duckdb_extension';
```
