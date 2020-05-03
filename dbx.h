#pragma once

#if !defined(__MYSQL__) && !defined(__SQLITE__) && !defined(__POSTGRESQL__)
#define __ODBC__
#endif

#if defined(__ODBC__)
#include <sql.h>
#include <sqlext.h>
#include <sqltypes.h>
#include <odbcinst.h>

#elif defined(__MYSQL__)
#include <mysql.h>

#elif defined(__SQLITE__)
#endif


#define ARRAY_SIZE(a) (sizeof(a)/sizeof(a[1]))

#define DBX_FLAG_STORED_PROC 0x01
#define DBX_FLAG_PREPARED    0x02

#define ODBCCONN_FLAG_TRUSTED 0x01

//Maximum size of the server host name
#define DBX_MAX_HOSTNAME 63+1

//Maximum size of the column name
#define DBX_MAX_COLUMN_NAME 64

typedef int(*callback_t)(void *, int, void *);

#ifdef __ODBC__
typedef SQL_TIMESTAMP_STRUCT dbdatetime_t;
typedef SQLCHAR              dbchar_t;
typedef SQLCHAR              dbvarchar_t;
typedef SQLINTEGER           dbint32_t;
typedef SQLSMALLINT          dbint16_t;
typedef SQLCHAR              dbint8_t;
#endif

enum dbx_command_type {
  SqlCmdStoredProc,
  SqlCmdText,
  SqlCmdTable
};

//need null and default values
struct dbx_field_desc {
  const char *name;
  const int type;
  const int size;
  const short offset;
  const short seq;
};

struct dbx_table_desc {
  const char *name;
  const char *schema;
  const int num_cols;
  const struct db_field_desc *fld_desc;
};

struct dbx_data_desc {
  const struct db_field_desc *field_descriptor;
  int data_offset;
  int length_offset;
};

struct dbx_query_desc {
  const char *sql;
  const int flags;
  const int row_size;
  const int num_fld;
  struct dbx_data_desc *data_desc;
};

typedef struct _dbconn {
#ifdef __ODBC__
  struct {
    SQLHANDLE hdbc;
  } odbc;

#elif __MYSQL__
  struct {
    MYSQL *dbc;
  } mysql;

#elif __SQLITE__
  struct {
    sqlite3 *dbc;
  } sqlite;
#endif
  char *host;
  char *defdb;
  short port;

  char hostbuf[64];
  char dbbuf[64];
} *dbconn;

typedef struct _dbcommand {
#ifdef __ODBC__
  struct {
    SQLHANDLE hstmt, ipd, ird;
  } odbc;

#elif defined(__MYSQL__)
  struct {

  } mysql;

#elif defined(__SQLITE__)
  struct {
    sqlite3_stmt *stmt;
  } sqlite;
#endif
  dbconn conn;
  int flags;
  char *query_string;
  short query_length;
  int num_rows;

  void *databuf;
  int data_count;

  callback_t callback;
  void *passback;

  struct db_query_desc *query_desc;

  struct {
    void *data;
  };

  char query_stringbuf[256];
} *dbcommand;


enum OdbcDriver {
  OdbcDriver_SqlServer,
  OdbcDriver_MySql,  //MySQL ODBC 5.2 ANSI Driver
  OdbcDriver_MSAccess
};

enum OdbcProtocol{
  OdbcProtocol_TcpIp,
  OdbcProtocol_NamedPipe,
  OdbcProtocol_SharedMemory
};

struct odbc_conn_info{
  enum OdbcDriver driver;   
  char *uid;
  char *pwd;
  int flags;
  union {
    struct {
      char *server_name;
      char *database; 
      enum OdbcProtocol protocol;
      short port;
    } sql_server;
    struct {
      char *dbq;
    } ms_access; 
  };
};

enum sql_types {
  SqlTypeError,
  SqlTypeChar,
  SqlTypeSmallint,
  SqlTypeTinyint,
  SqlTypeVarchar,
  SqlTypeUInt64,
  SqlTypeSInt64,
  SqlTypeUInt32,
  SqlTypeSInt32,
  SqlTypeUInt16,
  SqlTypeSInt16,
  SqlTypeUInt8,
  SqlTypeSInt8,
  SqlTypeBinary,
  SqlTypeFloat,
  SqlTypeDouble,
  SqlTypeDateTime
};

enum db_connection_property {
  DbConnPropHost,
  DbConnPropPort,
  DbConnPropDefaultDb,
  DbConnPropTimeout,
};

enum db_command_property {
  DbCmdPropQueryString,
  DbCmdPropQueryDescriptor,
  DbCmdPropMaxRowCount,
  DbCmdPropQueryTimeout,
  DbCmdPropDescriptor,
  DbCmdPropCallback,
};

enum db_error {
  DbErr
};


int db_connection_new(dbconn *conn);
int db_command_new(dbconn conn, dbcommand *cmd);
int db_connection_delete(dbconn conn);
int db_command_delete(dbcommand cmd);
int db_connect(dbconn conn);
void db_disconnect(dbconn conn);
int db_command_executecb(dbcommand cmd);
int db_command_execute(dbcommand cmd, dbint32_t *num_rows);
int db_connection_setprop(dbconn conn, enum db_connection_property prop, ...);
int db_command_setprop(dbcommand cmd, enum db_command_property prop, ...);
