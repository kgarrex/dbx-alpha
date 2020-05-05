
#ifdef _MSC_VER
	#pragma comment(lib, "advapi32.lib")
	#if defined(__SQLSERVER__) || defined(__MSACCESS__)
		#pragma comment(lib, "odbc32.lib")
	#endif
#endif


#include <stdio.h>
#include <stdlib.h>
#include <dbx.h>

//define globals
#if defined(__MSACCESS__) || defined(__SQLSERVER__)
SQLHANDLE odbc_henv;
#endif

typedef void * dbxobj_t;


#if defined(__MSACCESS__) || defined(__SQLSERVER__)
int db_get_odbc_type(enum sql_types type)
{
	switch (type) {
		case SqlTypeChar:
			return SQL_C_CHAR << 16 | SQL_CHAR;
		case SqlTypeVarchar:
			return SQL_C_CHAR << 16 | SQL_VARCHAR;
		case SqlTypeUInt64:
			return SQL_C_UBIGINT << 16 | SQL_BIGINT;
		case SqlTypeSInt64:
			return SQL_C_SBIGINT << 16 | SQL_BIGINT;
		case SqlTypeSInt32:
			return SQL_C_SLONG << 16 | SQL_INTEGER;
		case SqlTypeUInt32:
			return SQL_C_ULONG << 16 | SQL_INTEGER;
		case SqlTypeSmallint:
			return SQL_C_SHORT << 16 | SQL_SMALLINT;
		case SqlTypeSInt16:
			return SQL_C_SSHORT << 16 | SQL_SMALLINT;
		case SqlTypeUInt16:
			return SQL_C_USHORT << 16 | SQL_SMALLINT;
		case SqlTypeTinyint:
			return SQL_C_TINYINT << 16 | SQL_TINYINT;
		case SqlTypeSInt8:
			return SQL_C_STINYINT << 16 | SQL_TINYINT;
		case SqlTypeUInt8:
			return SQL_C_UTINYINT << 16 | SQL_TINYINT;
		case SqlTypeBinary:
			return SQL_C_BINARY << 16 | SQL_BINARY;
		case SqlTypeFloat:
			return SQL_C_FLOAT << 16 | SQL_FLOAT;
		case SqlTypeDouble:
			return SQL_C_DOUBLE << 16 | SQL_DOUBLE;
		case SqlTypeDateTime:
			return SQL_C_DATE << 16 | SQL_DATETIME;
		default:
			return 0;
	}
}
#endif

#if defined(__MSACCESS__) || defined(__SQLSERVER__)
SQLSMALLINT make_odbc_connection_string
(char buf[256], int bufsize, struct odbc_conn_info *info)
{
	char namebuf[64];
	char *ptr = buf;
	int len;

	memcpy(ptr, "DRIVER=", 7);
	ptr += 7;

	if(info->adapter == DbxAdapter_SqlServer)
	{
		memcpy(ptr, "{SQL Server}", 10);
		ptr += 10;
		memcpy(ptr, ";SERVER=", 8);
		ptr += 8;
		switch (info->sql_server.protocol) {

			case OdbcProtocol_TcpIp:
			memcpy(ptr, "tcp:", 4);
			ptr += 4;
			break;

			case OdbcProtocol_NamedPipe:
			memcpy(ptr, "np:", 3);
			ptr += 3;
			break;

			case OdbcProtocol_SharedMemory:
			memcpy(ptr, "lpc:", 4);
			ptr += 4;
			break;
		}
		len = strlen(info->sql_server.server_name);
		memcpy(ptr, info->sql_server.server_name, len);
		ptr += len;
		memcpy(ptr, ",1433", 5);
		ptr += 5;

		if (info->flags & ODBCCONN_FLAG_TRUSTED) {
			len = 64;
			GetUserNameA(namebuf, &len);
			memcpy(ptr, ";UID=", 5);
			ptr += 5;
			memcpy(ptr, namebuf, --len);
			ptr += len;
			memcpy(ptr, ";Trusted_Connection=Yes", 23);
			ptr += 23;
		}
		else {}

		len = 64;
		GetComputerNameExA(ComputerNameNetBIOS, namebuf, &len);

		memcpy(ptr, ";WSID=", 6);
		ptr += 6;
		memcpy(ptr, namebuf, len);
		ptr += len;

		memcpy(ptr, ";DATABASE=", 10);
		ptr += 10;
		len = strlen(info->sql_server.database);
		memcpy(ptr, info->sql_server.database, len);
		ptr += len;
	}
	else if(info->adapter == DbxAdapter_MSAccess)
	{
		memcpy(ptr, "{Microsoft Access Driver (*.mdb, *.accdb)}", 42);
		ptr += 42;
		memcpy(ptr, ";Dbq=", 5);
		ptr += 5;
		len = strlen(info->ms_access.dbq);
		memcpy(ptr, info->ms_access.dbq, len);
		ptr += len;

		memcpy(ptr, ";UID=", 5);
		ptr += 5;
		memcpy(ptr, ";Pwd=", 5);
		ptr += len;
	}

	*ptr = '\0';
	return ptr - buf;
}
#endif

int dbx_command_error(dbxobj_t obj, char *funcname)
{
	//if(obj->type != 'C') return 0;
	dbxcmd_t cmd = obj;

	#if defined(__MSACCESS__) || defined(__SQLSERVER__)
	SQLRETURN rtn;
	char sqlstate[6];
	SQLINTEGER error;
	char msg[64];
	SQLSMALLINT len;
	rtn = SQLGetDiagRec(SQL_HANDLE_STMT, cmd->odbc.hstmt, 1, sqlstate, &error, msg, 64, &len);
	printf("Error (%.*s): %s\n", 5, sqlstate, funcname);
	#endif

	return 1;
}

int dbxConnect(dbxconn_t conn)
{
	char buf[256];

	#if defined(__MSACCESS__) || defined(__SQLSERVER__)
	SQLRETURN rtn;
	SQLSMALLINT len;
	len = make_odbc_connection_string(buf, 256, &conn->odbc.info);
	printf("InString: %s\n", buf);
	rtn = SQLDriverConnect(conn->odbc.hdbc, 0, buf, len, buf, 256, &len, SQL_DRIVER_NOPROMPT);
	if (rtn != SQL_SUCCESS && rtn != SQL_SUCCESS_WITH_INFO) {
		printf("Error (%d): SQLDriverConnect \n", rtn);
		return 0;
	}

	#elif defined(__MYSQL__)
	mysql_real_connect(mysql, conn->host, user, pwd, conn->defdb, conn->port, 0, 0);

	#elif defined(__SQLITE__)
	sqlite3_open_v2(conn->host, &conn->mysql.dbc, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, 0); //should include vfs object

	#elif defined(__POSTGRESQL__)
	PQconnectdbParams();

	#endif

	//printf("OutString: %s\n", buf);
	return 1;
}

void dbxDisconnect(dbxconn_t conn)
{
	#if defined(__MSACCESS__) || defined(__SQLSERVER__)
	SQLFreeHandle(SQL_HANDLE_DBC, conn->odbc.hdbc);
	SQLFreeHandle(SQL_HANDLE_ENV, odbc_henv);
	#endif
}

void dbx_bindparams(dbxobj_t cmd)
{
	int num_param = 0;

	for (int i = 0; i < num_param; i++)
	{
		//SQLBindParameter(cmd->odbc.hstmt, i, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 0, buf, bufsize, );
	}
}

int dbxConnSetProp(dbxconn_t conn, enum dbx_connection_property prop, ...)
{
	va_list args;
	void *ptr;
	int len;

	va_start(args, prop);
	switch (prop) {

		case DbxConnProp_Host:
			ptr = va_arg(args, char *);
			len = strlen((char*)ptr);
			memcpy(conn->host, ptr, len);
			conn->host[len] = '\0';
			break;

		case DbxConnProp_Port: {
			unsigned short port = va_arg(args, unsigned short);
			conn->port = port;
		}
		break;

		case DbxConnProp_Database: {
			ptr = va_arg(args, char *);
			len = strlen((char*)ptr);
			memcpy(conn->defdb, ptr, len);
			conn->defdb[len] = '\0';
			conn->odbc.info.ms_access.dbq = conn->defdb;
			#if defined(__MSACCESS__) || defined(__SQLSERVER__)
			SQLSetConnectAttr(conn->odbc.hdbc, SQL_ATTR_CURRENT_CATALOG, (SQLPOINTER)conn->defdb, len);
			#elif defined(__MYSQL__)
			mysql_select_db(mysql, conn->defdb);
			#endif
		}
		break;

		case DbxConnProp_Timeout: {
			int seconds;
			seconds = va_arg(args, int);
			#if defined(__MSACCESS__) || defined(__SQLSERVER__)
			SQLSetConnectAttr(conn->odbc.hdbc, SQL_ATTR_CONNECTION_TIMEOUT, (SQLPOINTER)seconds, 0);
			#elif defined(__MYSQL__)
			mysql_options(mysql, MYSQL_OPT_CONNECT_TIMEOUT, &seconds);
			#endif
		}
		break;

	}
	va_end(args);
	return 1;
}

int dbxCmdSetProp(dbxobj_t obj, enum dbx_command_property prop, ...)
{
	va_list args;
	void *ptr;
	int n;
	dbxcmd_t cmd = obj;

	va_start(args, prop);
	switch (prop)
	{
		case DbxCmdProp_QueryString: {
			ptr = va_arg(args, char *);
			if (!ptr) return 0;
			cmd->query_string = cmd->query_stringbuf;
			n = strlen((char*)ptr);
			memcpy(cmd->query_string, ptr, n);
			cmd->query_string[n] = '\0';
		} break;

		case DbxCmdProp_MaxRowCount: {
			n = va_arg(args, int);
			#if defined(__MSACCESS__) || defined(__SQLSERVER__)
			SQLSetStmtAttr(cmd->odbc.hstmt, SQL_ATTR_MAX_ROWS, (SQLPOINTER)&n, sizeof(n));
			#endif
		} break;

		case DbxCmdProp_QueryTimeout: {
			int seconds = va_arg(args, int);
			#if defined(__MSACCESS__) || defined(__SQLSERVER__)
			SQLSetStmtAttr(cmd->odbc.hstmt, SQL_ATTR_QUERY_TIMEOUT, (SQLPOINTER)seconds, 0);
			#elif defined(__MYSQL__)
			//mysql_options(cmd->mysql, MYSQL_OPT_READ_TIMEOUT, 0);
			//mysql_options(cmd->mysql, MYSQL_OPT_WRITE_TIMEOUT, 0);
			#endif
		} break;

		case DbxCmdProp_Descriptor: {
			void *desc = va_arg(args, void*);
			void *buf = va_arg(args, void *);
			int bufsize = va_arg(args, int);
			cmd->query_desc = desc;
			cmd->databuf = buf;
			cmd->data_count = bufsize;

			cmd->query_string = (char*)cmd->query_desc->sql;
		} break;

		case DbxCmdProp_Callback: {
			callback_t cb = va_arg(args, void *);
			void *pb = va_arg(args, void *);
			cmd->callback = cb;
			cmd->passback = pb;
		}

	}
	va_end(args);
	return 1;
}

int dbx_prepare(dbxobj_t obj)
{
	dbxcmd_t cmd = obj;
	if (!cmd->query_string) {
		return 0;
	}

	#if defined(__MSACCESS__) || defined(__SQLSERVER__)
	SQLRETURN rtn;
	rtn = SQLPrepare(cmd->odbc.hstmt, cmd->query_string, SQL_NTS);
	if (rtn != SQL_SUCCESS && rtn != SQL_SUCCESS_WITH_INFO) {
		dbx_command_error(cmd, "SQLPrepare");
		return 0;
	}

	#elif defined(__MYSQL__)
	mysql_stmt_init();

	#elif defined(__SQLITE__)
	int rtn;
	//-1 for nul-terminated, otherwise, pass length of query + 1 for null char
	rtn = sqlite3_prepare_v2(cmd->conn->mysql.dbc, cmd->query_string, -1, &cmd->mysql.stmt, 0);
	if (rtn != SQLITE_OK) {
		//error
	}

	#endif

	cmd->flags |= DBX_FLAG_PREPARED;
	return 1;
}

int dbx_connection_new(dbxconn_t *pconn)
{
	#if defined(__MSACCESS__) || defined(__SQLSERVER__)
	SQLRETURN rtn;
	if (!odbc_henv) {
		rtn = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &odbc_henv);
		if (rtn != SQL_SUCCESS) {
			printf("Error (%d): SQLAllocHandle Env\n", rtn);
			return 0;
		}
		SQLSetEnvAttr(odbc_henv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0);
	}

	*pconn = malloc(sizeof(struct _dbxconn));

	if (!*pconn) {
		printf("Error: No memory\n");
		return 0;
	}

	rtn = SQLAllocHandle(SQL_HANDLE_DBC, odbc_henv, &(*pconn)->odbc.hdbc);
	if (rtn != SQL_SUCCESS) {
		printf("Error (%d): SQLAllocHandle Dbc\n", rtn);
		return 0;
	}
	#endif

	(*pconn)->host = (*pconn)->hostbuf; 
	(*pconn)->defdb = (*pconn)->dbbuf;

	return 1;
}

int dbx_command_new(dbxconn_t conn, dbxobj_t *pobj)
{
	dbxcmd_t cmd;

	*pobj = malloc(sizeof(struct _dbxcmd));
	if (!*pobj) {
		printf("Error: No memory\n");
		return 0;
	}
	cmd = *pobj;

	#if defined(__MSACCESS__) || defined(__SQLSERVER__)
	SQLRETURN rtn;
	rtn = SQLAllocHandle(SQL_HANDLE_STMT, conn->odbc.hdbc, cmd->odbc.hstmt);
	if (rtn != SQL_SUCCESS) {
		printf("Error (%d): SQLAllocHandle Stmt\n", rtn);
		return 0;
	}

	//get the number of rows after SQLFetch or SQLFetchScroll
	rtn = SQLSetStmtAttr(cmd->odbc.hstmt, SQL_ATTR_ROWS_FETCHED_PTR, (SQLPOINTER)cmd->num_rows, 0);
	if (rtn != SQL_SUCCESS) {
		printf("Error (%d): SQLSetStmtAttr\n", rtn);
		return 0;
	}
	
	rtn = SQLSetStmtAttr(cmd->odbc.hstmt, SQL_ATTR_CURSOR_SCROLLABLE, (SQLPOINTER)SQL_SCROLLABLE, 0);
	if (rtn != SQL_SUCCESS) {
		dbx_command_error(cmd, "SQLSetStmtAttr scroll\n");
		return 0;
	}

	//get the implementation row and parameter descriptors
	rtn = SQLGetStmtAttr(cmd->odbc.hstmt, SQL_ATTR_IMP_PARAM_DESC, (SQLPOINTER)cmd->odbc.ipd, SQL_IS_POINTER, 0);
	if (rtn != SQL_SUCCESS);
	rtn = SQLGetStmtAttr(cmd->odbc.hstmt, SQL_ATTR_IMP_ROW_DESC, (SQLPOINTER)cmd->odbc.ird, SQL_IS_POINTER, 0);
	if (rtn != SQL_SUCCESS);
	#elif defined(__MYSQL__)

	#elif defined(__SQLITE__)

	#elif defined(__POSTGRESQL__)

	#endif

	cmd->conn = conn;
	cmd->query_string = 0;
	cmd->num_rows = 0;
	cmd->callback = 0;
	cmd->passback = 0;
	return 1;
}

int dbx_getprop(dbxobj_t cmd, int prop, ...)
{
	return 0;
}

int dbx_command_delete(dbxobj_t cmd)
{
	free(cmd);
	return 1;
}

int dbx_connection_delete(dbxconn_t conn)
{
	free(conn);
	return 1;
}

void compare_column(dbxcmd_t cmd, struct dbx_field_desc *fd, int col)
{
	#if defined(__MSACCESS__) || defined(__SQLSERVER__)
	SQLRETURN rtn;
	char buf[DBX_MAX_COLUMN_NAME];
	SQLSMALLINT len = 0;
	rtn = SQLColAttribute(cmd->odbc.hstmt, col, SQL_DESC_BASE_COLUMN_NAME, (SQLPOINTER)buf, DBX_MAX_COLUMN_NAME, &len, 0);
	if (rtn != SQL_SUCCESS && rtn != SQL_SUCCESS_WITH_INFO)
	{
		dbx_command_error(cmd, "SQLColAttribute");
		return;
	}

	if (strcmp(buf, fd->name)) { //string doesn't match, 
		return;
	}
	printf("Column Name: %s\n", buf);
	#endif
}

int _bind_columns(dbxcmd_t cmd)
{
	struct dbx_query_desc *qry_desc = cmd->query_desc;
	struct dbx_data_desc *dat_desc;
	struct dbx_field_desc *fld_desc;

	#if defined(__MSACCESS__) || defined(__SQLSERVER__)
	SQLRETURN rtn;
	void *len_ptr, *data_ptr;

	for (int i = 0; i < qry_desc->num_fld; i++)
	{
		dat_desc = &qry_desc->data_desc[i];
		fld_desc = dat_desc->field_descriptor;
		data_ptr = (char *)cmd->databuf + dat_desc->data_offset;
		len_ptr = dat_desc->length_offset ? (char *)cmd->databuf + dat_desc->length_offset : 0;
		rtn = SQLBindCol(cmd->odbc.hstmt, i + 1, db_get_odbc_type(fld_desc->type) >> 16, (SQLPOINTER)data_ptr, fld_desc->size, (SQLLEN*)len_ptr);
		if (rtn != SQL_SUCCESS && rtn != SQL_SUCCESS_WITH_INFO) {
			dbx_command_error(cmd, "SQLBindCol");
			return 0;
		}
	}
	#endif

	return 1;
}

int _fetch_rows(dbxcmd_t cmd)
{
	struct dbx_query_desc *qry_desc = cmd->query_desc;

	#if defined(__MSACCESS__) || defined(__SQLSERVER__)
	SQLRETURN rtn;
	SQLSMALLINT col_count;

	if (cmd->flags & DBX_FLAG_PREPARED) {
		printf("SQLExecute\n");
		rtn = SQLExecute(cmd->odbc.hstmt);
		if (rtn != SQL_SUCCESS) {
			printf("Error (%d): SQLExecute\n", rtn);
			return 0;
		}
	}
	else {
		printf("SQLExecDirect\n");
		rtn = SQLExecDirect(cmd->odbc.hstmt, cmd->query_string, strlen(cmd->query_string));
		if (rtn != SQL_SUCCESS) {
			printf("Error (%d): SQLExecDirect\n", rtn);
			return 0;
		}
	}
	
	rtn = SQLNumResultCols(cmd->odbc.hstmt, &col_count);
	if (rtn != SQL_SUCCESS) {
		printf("Error (%d): SQLNumResultCols\n", rtn);
		return 0;
	}
	if (col_count != qry_desc->num_fld) {
		printf("Invalid column count\n");
		return 0;
	}

	rtn = SQLSetStmtAttr(cmd->odbc.hstmt, SQL_ATTR_ROW_ARRAY_SIZE, (SQLPOINTER)cmd->data_count, 0);
	if (rtn != SQL_SUCCESS) {
		dbx_command_error(cmd, "SQLSetStmtAttr");
		return 0;
	}

	rtn = SQLSetStmtAttr(cmd->odbc.hstmt, SQL_ATTR_ROW_BIND_TYPE, (SQLPOINTER)qry_desc->row_size, 0);
	if (rtn != SQL_SUCCESS) {
		dbx_command_error(cmd, "SQLSetStmtAttr");
		return 0;
	}
	#elif defined(__MYSQL__)
	#endif

	return 1;
}



/********
 *
 *******/

int dbx_command_executecb(dbxobj_t obj)
{
	dbxcmd_t cmd = obj;
	if (!cmd->callback) return 0; //error: no callback set

	_fetch_rows(cmd);

	_bind_columns(cmd);

	#if defined(__MSACCESS__) || defined(__SQLSERVER__)
	SQLRETURN rtn;

	do {
		rtn = SQLFetch(cmd->odbc.hstmt);
		if (rtn == SQL_NO_DATA) break;
		if (rtn != SQL_SUCCESS) {
			if (rtn == SQL_ERROR || rtn == SQL_INVALID_HANDLE) {
				dbx_command_error(cmd, "SQLFetchScroll");
				return 0;
			}
			else if (rtn == SQL_SUCCESS_WITH_INFO) {
				printf("Success with info\n");
			}
		}
		cmd->callback(cmd->databuf, cmd->num_rows, cmd->passback);
	} while (1);

	#elif defined(__MYSQL__)
	#elif defined(__SQLITE__)
	#elif defined(__POSTGRESQL__)
	#endif

	return 1;
}



/*************************************//*
 * @brief execute a command against a database
 *
 *
 *****************/

int dbx_command_execute(dbxobj_t obj, dbint32_t *num_rows)
{
	dbxcmd_t cmd = obj;
	if (!cmd->query_string) {
		printf("Query string not set\n");
		return 0;
	}
	_fetch_rows(cmd);

	_bind_columns(cmd);

	#if defined(__MSACCESS__) || defined(__SQLSERVER__)
	SQLRETURN rtn;
	rtn = SQLFetch(cmd->odbc.hstmt);
	if (rtn == SQL_NO_DATA) {
		*num_rows = 0;
		return 0; //no data
	}
	if (rtn != SQL_SUCCESS) {
		if (rtn == SQL_ERROR || rtn == SQL_INVALID_HANDLE) {
			dbx_command_error(cmd, "SQLFetchScroll");
			return 0;
		}
		else if (rtn == SQL_SUCCESS_WITH_INFO) {
	printf("Success with info\n");
		}


		if (num_rows) *num_rows = cmd->num_rows;
		//if (cmd->num_rows > cmd->data_count); //return more results

	#elif defined(__MYSQL__)
		//mysql_query(cmd->mysql.stmt, cmd->query_string);
		//mysql_real_query(cmd->mysql.stmt, cmd->query_string, len);
	#elif defined(__SQLITE__)
		sqlite3_step(cmd->sqlite.stmt);
	#elif defined(__POSTGRESQL__)

	#endif

	}

	return 1;
}
