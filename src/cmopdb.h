#pragma once

#include <windows.h>
#include "db.h"

/* UserProfiles Table */
#define UP_OPERATOR_ID_SIZE   20+1
#define UP_LOGIN_ID_SIZE      20+1
#define UP_OPERATOR_NAME_SIZE 50+1
#define UP_PASSWORD_SIZE      35+1

enum UserProfilesTableEnum {
	UserProfilesTable_OperatorIDEnum,
	UserProfilesTable_LoginIDEnum,
	UserProfilesTable_OperatorNameEnum,
	UserProfilesTable_SecurityGroupFlagEnum,
	UserProfilesTable_SecurityMask1Enum,
	UserProfilesTable_SecurityMask2Enum,
	UserProfilesTable_SecurityMask3Enum,
	UserProfilesTable_SecurityGroupMaskEnum,
	UserProfilesTable_PasswordEnum,
	UserProfilesTable_PrivateReportingIDEnum
};

struct UserProfiles {
	dbvarchar_t OperatorID[UP_OPERATOR_ID_SIZE];
	dbvarchar_t LoginID[UP_LOGIN_ID_SIZE];
	dbvarchar_t OperatorName[UP_OPERATOR_NAME_SIZE];
	dbvarchar_t SecurityGroupFlag;
	dbint32_t   SecurityMask1;
	dbint32_t   SecurityMask2;
	dbint32_t   SecurityMask3;
	dbint32_t   SecurityGroupMask;
	dbvarchar_t Password[UP_PASSWORD_SIZE];
	dbint32_t   PrivateReportingID;
};

static const struct db_field_desc UserProfilesFields[] = {
	{"OperatorID",         SqlTypeVarchar,  UP_OPERATOR_ID_SIZE,   offsetof(struct UserProfiles, OperatorID)         },
	{"LoginID",            SqlTypeVarchar,  UP_LOGIN_ID_SIZE,      offsetof(struct UserProfiles, LoginID)            },
	{"OperatorName",       SqlTypeVarchar,  UP_OPERATOR_NAME_SIZE, offsetof(struct UserProfiles, OperatorName)       },
	{"SecurityGroupFlag",  SqlTypeTinyint,  sizeof(char),          offsetof(struct UserProfiles, SecurityGroupFlag)  },
	{"SecurityMask1",      SqlTypeSInt32,   sizeof(int),           offsetof(struct UserProfiles, SecurityMask1)      },
	{"SecurityMask2",      SqlTypeSInt32,   sizeof(int),           offsetof(struct UserProfiles, SecurityMask2)      },
	{"SecurityMask3",      SqlTypeSInt32,   sizeof(int),           offsetof(struct UserProfiles, SecurityMask3)      },
	{"SecurityGroupMask",  SqlTypeSInt32,   sizeof(int),           offsetof(struct UserProfiles, SecurityGroupMask)  },
	{"Password",           SqlTypeVarchar,  UP_PASSWORD_SIZE,      offsetof(struct UserProfiles, Password)           },
	{"PrivateReportingID", SqlTypeSInt32,   sizeof(int),           offsetof(struct UserProfiles, PrivateReportingID) }
};
static struct db_table_desc UserProfilesTable = { "UserProfiles", "dbo", ARRAY_SIZE(UserProfilesFields), UserProfilesFields };

//extern const struct db_field_desc UserProfilesFields[];
//extern struct db_table_desc UserProfilesTable;

/* OrderMaster Table */
#define OM_ORDER_NUMBER_SIZE           20+1
#define OM_BATCHID_SIZE                20+1
#define OM_PATIENTID_SIZE              20+1
#define OM_PATIENTID_TRAILER_SIZE      5+1
#define OM_PATIENT_FIRST_NAME_SIZE     200+1
#define OM_PATIENT_MIDDLE_NAME_SIZE    48+1
#define OM_PATIENT_LAST_NAME_SIZE      200+1
#define OM_PATIENT_SUFFIX_SIZE         10+1
#define OM_PATIENT_ADDRESS1_SIZE       100+1
#define OM_PATIENT_ADDRESS2_SIZE       100+1
#define OM_PATIENT_ADDRESS3_SIZE       50+1
#define OM_PATIENT_ADDERSS4_SIZE       50+1
#define OM_PATIENT_CITY_SIZE           50+1
#define OM_PATIENT_STATE_SIZE          20+1
#define OM_PATIENT_ZIP_CODE_SIZE       15+1
#define OM_ENCODING_CHARACTERS_SIZE    10+1
#define OM_PATIENT_PHONE_NUMBER_SIZE   20+1
#define OM_LANGUAGE_CODE_SIZE          3+1
#define OM_PATIENT_ACCOUNT_NUMBER_SIZE 250+1

enum OrderMasterTableEnum {
	OrderMaster_OrderNumber,
	OrderMaster_BatchID,
	OrderMaster_MessageDateTime,
	OrderMaster_ReceivedDateTime,
	OrderMaster_PatientID,
	OrderMaster_PatientIDTrailer,
	OrderMaster_PatientFirstName,
	OrderMaster_PatientMiddleName,
	OrderMaster_PatientLastName,
	OrderMaster_PatientSuffix,
	OrderMaster_PatientAddress1,
	OrderMaster_PatientAddress2,
	OrderMaster_PatientAddress3,
	OrderMaster_PatientAddress4,
	OrderMaster_PatientCity,
	OrderMaster_PatientState,
	OrderMaster_PatientZipCode,
	OrderMaster_EncodingCharacters,
	OrderMaster_PatientPhoneNumber,
	OrderMaster_LanguageFlag,
	OrderMaster_LanguageCode,
	OrderMaster_PatientAccountNumber
};

struct OrderMaster {
	dbvarchar_t  OrderNumber[OM_ORDER_NUMBER_SIZE];
	dbvarchar_t  BatchID[OM_BATCHID_SIZE];
	dbdatetime_t MessageDateTime;
	dbdatetime_t ReceivedDateTime;
	dbvarchar_t  PatientID[OM_PATIENTID_SIZE];
	dbvarchar_t  PatientIDTrailer[OM_PATIENTID_TRAILER_SIZE];
	dbvarchar_t  PatientFirstName[OM_PATIENT_FIRST_NAME_SIZE];
	dbvarchar_t  PatientMiddleName[OM_PATIENT_MIDDLE_NAME_SIZE];
	dbvarchar_t  PatientLastName[OM_PATIENT_LAST_NAME_SIZE];
	dbvarchar_t  PatientSuffix[OM_PATIENT_SUFFIX_SIZE];
	dbvarchar_t  PatientAddress1[OM_PATIENT_ADDRESS1_SIZE];
	dbvarchar_t  PatientAddress2[OM_PATIENT_ADDRESS2_SIZE];
	dbvarchar_t  PatientAddress3[OM_PATIENT_ADDRESS3_SIZE];
	dbvarchar_t  PatientAddress4[OM_PATIENT_ADDRESS1_SIZE];
	dbvarchar_t  PatientCity[OM_PATIENT_CITY_SIZE];
	dbvarchar_t  PatientState[OM_PATIENT_STATE_SIZE];
	dbvarchar_t  PatientZipCode[OM_PATIENT_ZIP_CODE_SIZE];
	dbvarchar_t  EncodingCharacters[OM_ENCODING_CHARACTERS_SIZE];
	dbvarchar_t  PatientPhoneNumber[OM_PATIENT_PHONE_NUMBER_SIZE];
	dbint32_t    LanguageFlag;
	dbvarchar_t  LanguageCode[OM_LANGUAGE_CODE_SIZE];
	dbvarchar_t  PatientAccountNumber[OM_PATIENT_ACCOUNT_NUMBER_SIZE];
};

static const struct db_field_desc OrderMasterFields[] = {
	{"OrderNumber",          SqlTypeVarchar,  OM_ORDER_NUMBER_SIZE,           offsetof(struct OrderMaster, OrderNumber)          },
	{"BatchID",              SqlTypeVarchar,  OM_BATCHID_SIZE,                offsetof(struct OrderMaster, BatchID)              },
	{"MessageDateTime",      SqlTypeDateTime, sizeof(dbdatetime_t),           offsetof(struct OrderMaster, MessageDateTime)      },
	{"ReceivedDateTime",     SqlTypeDateTime, sizeof(dbdatetime_t),           offsetof(struct OrderMaster, ReceivedDateTime)     },
	{"PatientID",            SqlTypeVarchar,  OM_PATIENTID_SIZE,              offsetof(struct OrderMaster, PatientID)            },
	{"PatientIDTrailer",     SqlTypeVarchar,  OM_PATIENTID_TRAILER_SIZE,      offsetof(struct OrderMaster, PatientIDTrailer)     },
	{"PatientFirstName",     SqlTypeVarchar,  OM_PATIENT_FIRST_NAME_SIZE,     offsetof(struct OrderMaster, PatientFirstName)     },
	{"PatientMiddleName",    SqlTypeVarchar,  OM_PATIENT_MIDDLE_NAME_SIZE,    offsetof(struct OrderMaster, PatientMiddleName)    },
	{"PatientLastName",      SqlTypeVarchar,  OM_PATIENT_LAST_NAME_SIZE,      offsetof(struct OrderMaster, PatientLastName)      },
	{"PatientSuffix",        SqlTypeVarchar,  OM_PATIENT_SUFFIX_SIZE,         offsetof(struct OrderMaster, PatientSuffix)        },
	{"PatientAddress1",      SqlTypeVarchar,  OM_PATIENT_ADDRESS1_SIZE,       offsetof(struct OrderMaster, PatientAddress1)      },
	{"PatientAddress2",      SqlTypeVarchar,  OM_PATIENT_ADDRESS2_SIZE,       offsetof(struct OrderMaster, PatientAddress2)      },
	{"PatientAddress3",      SqlTypeVarchar,  OM_PATIENT_ADDRESS3_SIZE,       offsetof(struct OrderMaster, PatientAddress3)      },
	{"PatientAddress4",      SqlTypeVarchar,  OM_PATIENT_ADDERSS4_SIZE,       offsetof(struct OrderMaster, PatientAddress4)      },
	{"PatientCity",          SqlTypeVarchar,  OM_PATIENT_CITY_SIZE,           offsetof(struct OrderMaster, PatientCity)          },
	{"PatientState",         SqlTypeVarchar,  OM_PATIENT_STATE_SIZE,          offsetof(struct OrderMaster, PatientState)         },
	{"PatientZipCode",       SqlTypeVarchar,  OM_PATIENT_ZIP_CODE_SIZE,       offsetof(struct OrderMaster, PatientZipCode)       },
	{"EncodingCharacters",   SqlTypeVarchar,  OM_ENCODING_CHARACTERS_SIZE,    offsetof(struct OrderMaster, EncodingCharacters)   },
	{"PatientPhoneNumber",   SqlTypeVarchar,  OM_PATIENT_PHONE_NUMBER_SIZE,   offsetof(struct OrderMaster, PatientPhoneNumber)   },
	{"LanguageFlag",         SqlTypeSInt32,   sizeof(dbint32_t),              offsetof(struct OrderMaster, LanguageFlag)         },
	{"LanguageCode",         SqlTypeVarchar,  OM_LANGUAGE_CODE_SIZE,          offsetof(struct OrderMaster, LanguageCode)         },
	{"PatientAccountNumber", SqlTypeVarchar,  OM_PATIENT_ACCOUNT_NUMBER_SIZE, offsetof(struct OrderMaster, PatientAccountNumber) },
};

static struct db_table_desc OrderMasterTable = { "OrderMaster", "dbo", ARRAY_SIZE(OrderMasterFields), OrderMasterFields };