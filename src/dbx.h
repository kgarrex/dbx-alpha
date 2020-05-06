#ifndef DBX_H
#define DBX_H

#if !defined(__MSACCESS__) && !defined(__SQLSERVER__) && !defined(__MYSQL__) && !defined(__SQLITE__) && !defined(__POSTGRESQL__)
#error Error! Must define a database to use
#endif

#ifdef _MSC_VER
	#define WIN32_LEAN_AND_MEAN
	#include <windows.h>
#endif

#if defined(__SQLSERVER__) || defined(__MSACCESS__)
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
#define DBX_FLAG_PREPARED		0x02

#define ODBCCONN_FLAG_TRUSTED 0x01

//Maximum size of the server host name
#define DBX_MAX_HOSTNAME 63+1

//Maximum size of the column name
#define DBX_MAX_COLUMN_NAME 64

typedef int(*callback_t)(void *, int, void *);

#if defined(__SQLSERVER__) || defined(__MSACCESS__)
typedef SQL_TIMESTAMP_STRUCT dbdatetime_t;
typedef SQLCHAR							dbchar_t;
typedef SQLCHAR							dbvarchar_t;
typedef SQLINTEGER					 dbint32_t;
typedef SQLSMALLINT					dbint16_t;
typedef SQLCHAR							dbint8_t;

enum DbxAdapter {
	DbxAdapter_SqlServer,
	DbxAdapter_MySql,	//MySQL ODBC 5.2 ANSI Driver
	DbxAdapter_MSAccess
};

struct odbc_conn_info{
	enum DbxAdapter adapter;	 
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
	const struct dbx_field_desc *fld_desc;
};

struct dbx_data_desc {
	const struct dbx_field_desc *field_descriptor;
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

typedef struct _dbxconn {
	#if defined(__SQLSERVER__) || defined(__MSACCESS__)
	struct {
		SQLHANDLE hdbc;
	struct odbc_conn_info info;
	} odbc;

	#elif defined(__MYSQL__)
	struct {
		MYSQL *dbc;
	} mysql;

	#elif defined(__SQLITE__)
	struct {
		sqlite3 *dbc;
	} sqlite;
	#endif
	char *host;
	char *defdb;
	short port;

	int thread_id;

	char hostbuf[64];
	char dbbuf[64];
} *dbxconn_t;

typedef struct _dbxcmd {
	#if defined(__SQLSERVER__) || defined(__MSACCESS__)
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
	dbxconn_t conn;
	int flags;
	char *query_string;
	short query_length;
	int num_rows;

	void *databuf;
	int data_count;

	callback_t callback;
	void *passback;

	struct dbx_query_desc *query_desc;

	struct {
		void *data;
	};

	char query_stringbuf[256];
} *dbxcmd_t;


enum OdbcProtocol{
	OdbcProtocol_TcpIp,
	OdbcProtocol_NamedPipe,
	OdbcProtocol_SharedMemory
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


enum DbxProp
{
	DbxProp_Host,
	/**< The host name of the db server to make a connetion to. */

	DbxProp_Port,
	/**< The port used when connecting to a db server. */
	
	DbxProp_Database,
	/**< The default database name or database file used */

	DbxProp_ConnTimeout,
	/**< The timeout in seconds for a connection to be successful */

	DbxProp_QueryTimeout,
	DbxProp_QueryString,
	DbxProp_MaxRowCount,

	DbxProp_Callback,
	/**< The callback called on each row of a resultset */

    DbxProp_UserData,
	/**< The data passed to the callback */
 
	DbxProp_QueryDescriptor,
};

enum dbx_connection_property {
	DbxConnProp_Host,
	DbxConnProp_Port,
	DbxConnProp_Database,
	DbxConnProp_Timeout,
};

enum dbx_command_property {
	DbxCmdProp_QueryString,
	DbxCmdProp_QueryDescriptor,
	DbxCmdProp_MaxRowCount,
	DbxCmdProp_QueryTimeout,
	DbxCmdProp_Descriptor,
	DbxCmdProp_Callback,
};

enum db_error {
	DbErr
};


int dbx_connection_new(dbxconn_t *conn);

int dbx_command_new(dbxconn_t conn, void **cmd);

int dbx_connection_delete(dbxconn_t conn);

int dbx_command_delete(void *cmd);

int dbx_connect(void *conn);

void dbx_disconnect(void *conn);

int dbx_command_executecb(void *cmd);

int dbx_command_execute(void *cmd, dbint32_t *num_rows);

int dbx_execute(void *cmd);

int dbxSetProp(void *cmd, int prop, ...);

#endif
