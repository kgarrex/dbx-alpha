
#include <dbx.h>

struct test_case {
  char *desc;
  int input_type;
  char *input;
};

enum AllSexTableEnum {
  Sex_AuthSex
};

enum AllStateTableEnum {
  State_AuthState
};

enum AllSubjectTableEnum {
  Subj_AuthSubject
};

enum AuthorTableEnum {
  Auth_AuthId,
  Auth_LastName,
  Auth_FirstName,
  Auth_Phone,
  Auth_City,
  Auth_State,
  Auth_Zip,
  Auth_Sex,
  Auth_Salary,
  Auth_Subject,
};

static const struct dbx_field_desc AuthorTableFields[] = {
  {"au_id"},
  {"au_lname"},
  {"au_fname"},
  {"au_phone"},
  {"au_city"},
  {"au_state"},
  {"au_zip"},
  {"au_sex"},
  {"au_salary"},
  {"au_subject"},
};

enum BookAuthorTableEnum {
  BookAuth_AuthId,
  BookAuth_TitleId,
  BookAuth_Order,
  BookAuth_Royalty,
};

enum BooksTableEnum {
  Books_TitleId,
  Books_Title,
  Books_Type,
  Books_PubId,
  Books_Price,
  Books_Advance,
  Books_Royalty,
  Books_YTDSales,
  Books_Notes,
  Books_PubDate,
};

enum EmployeesTableEnum {
  Emp_EmpId,
  Emp_FirstName,
  Emp_MiddleInitial,
  Emp_LastName,
  Emp_JobId,
  Emp_JobLevel,
  Emp_PubId,
  Emp_HireDate,
};

enum JobsTableEnum {
  Job_JobId,
  Job_Description,
  Job_MinLevel,
  Job_MaxLevel,
};

enum PublishersTableEnum {
  Pub_PubId,
  Pub_Name,
  Pub_City,
  Pub_State,
  Pub_Country,
};

void add_test_case(const char *desc)
{
  
}

int main(int argc, char *argv[])
{
  struct odbc_conn_info coninfo;
  dbxconn_t conn;
  dbxcmd_t cmd;

  //Test MS Access connection
  coninfo.adapter = DbxAdapter_MSAccess;

  dbx_connection_new(&conn);
  //dbx_connection_setprop(conn, DbxConnProp_OdbcDriver, DbxAdapter_MSAccess); 

  char *db = "C:\\Dev\\code\\dbx-alpha\\test\\Access2010DB\\Books2010.accdb";
  char *db2 = ".\\test\\Access2010DB\\Books2010.accdb";


  dbxSetProp(cmd, DbxProp_Database, db2);
  //dbxConnSetProp(conn, DbxConnProp_Database, db2);
  

  dbxConnect(cmd);

  dbx_command_new(conn, &cmd);
  //dbx_command_setprop(cmd

  
  return 1;
}
