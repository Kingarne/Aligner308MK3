// $Id: Database.h,v 1.2 2009/12/08 19:14:22 riar Exp $
//
// Copyright (C) Shill Reglerteknik AB 2002

#pragma once

/** @brief A static class encapsulating the DB session and source.
 *
 *  Database is class 
 */
class Database
{
private:
	Database( void ) ;
	virtual ~Database( void ) ;
public:
   static HRESULT Open( void ) ;
   static HRESULT StartTransaction( void ) ;
   static HRESULT Commit( void ) ;
   static void Close( void ) ;
   static void DisplayError( HRESULT hResult, const TCHAR *pFileName = NULL, int lineNumber = 0 ) ;
public:
  static CDataSource sm_source ;
  static CSession sm_session ;

    
} ;

/** @brief Template to open a DB table.
 *
 *  
 */
template <class Table>
HRESULT OpenTable( Table &table, LPCWSTR pSqlCommand = NULL )
{
  return table.OpenRowset( Database::sm_session , pSqlCommand ) ;
}

/** @brief Declare a simple database varible, e.g., a double or an int.
 *
 *  DECL_DB_V expands to a declaration of a DB variable where there is no need
 *  for a 'length' attribute in the db_column declaration. Use SET_DB_V to set
 *  the variable.
 *
 *  @param ordial - The ordial of the corresponding column in the query.
 *  @param type   - The type of varible, always a simple type, e.g., an int or a char.
 *  @param name   - The name of the variable, m_<name> can be used to access the varible. 
 */
#define DECL_DB_V(ordial,type,name) \
  [ db_column(ordial, status=m_status_##name) ] type m_##name ; DBSTATUS m_status_##name ;

/** @brief Declare a database varible with length, e.g., a string.
 *
 *  DECL_DB_L expands to a declaration of a DB variable where there is need
 *  for a 'length' attribute in the db_column declaration. Use SET_DB_L to set
 *  the variable.
 *
 *  @param ordial - The ordial of the corresponding column in the query.
 *  @param type   - The type of varible, always a simple type, e.g., an int or a char.
 *  @param name   - The name of the variable, m_<name> can be used to access the varible.
 *  @param size   - The maximum size of the varibale, the variable is declared as <type> m_<name> [<size>].
 */
#define DECL_DB_L(ordial,type,name,size ) \
  [ db_column(ordial, status=m_status_##name, length=m_length_##name) ] type m_##name [size] ; DBSTATUS m_status_##name ; DBLENGTH m_length_##name ;

/** @brief Declare a simple input database variable, e.g., a double or an int.
 *
 *  PARM_DB_V expands to a declaration of an input DB variable where there is no
 *  need for a 'length' attribute in the db_column declaration. Use SET_DB_V to set
 *  the variable.
 *
 *  @param ordial - The ordial of the corresponding column in the query.
 *  @param type   - The type of varible, always a simple type, e.g., an int or a char.
 *  @param name   - The name of the variable, m_<name> can be used to access the varible. 
 */
#define PARM_DB_V(ordial,type,name) \
  [ db_param(ordial, status=m_status_##name) ] type m_##name ; DBSTATUS m_status_##name ;

/** @brief Declare an input database varible with length, e.g., a string.
 *
 *  DECL_DB_L expands to a declaration of an input DB variable where there is need
 *  for a 'length' attribute in the db_column declaration. Use SET_DB_L to set
 *  the variable.
 *
 *  @param ordial - The ordial of the corresponding column in the query.
 *  @param type   - The type of varible, always a simple type, e.g., an int or a char.
 *  @param name   - The name of the variable, m_<name> can be used to access the varible.
 *  @param size   - The maximum size of the varibale, the variable is declared as <type> m_<name> [<size>].
 */
#define PARM_DB_L(ordial,type,name,size ) \
  [ db_param(ordial, status=m_status_##name, length=m_length_##name) ] type m_##name [size] ; DBSTATUS m_status_##name ; DBLENGTH m_length_##name ;

/** @brief Set the value of a DB string, i.e., a char array.
 *
 *  SET_DB_STRING sets the value of a variable and sets corresponding status
 *  and length fields of the DB table.
 *
 *  @param table - The name of the table, an instance of a class declared with db_command.
 *  @param name  - The name of the variable to set (without a m_ prefix).
 *  @param value - A pointer to character data that must be zero terminated.
 */
#define SET_DB_STRING(table,name,value) \
  do { \
      table.m_length_##name = (DBLENGTH)_tcslen( value ) ; \
      _tcscpy( table.m_##name, value ) ; \
      table.m_status_##name = DBSTATUS_S_OK ; \
  } while (0) ;
//#define SET_DB_STRING(table,name,value) \
//  do { \
//      table.m_length_##name = (DBLENGTH)_tcslen( value ) ; \
//      if (0 != table.m_length_##name) { \
//        _tcscpy( table.m_##name, value ) ; \
//        table.m_status_##name = DBSTATUS_S_OK ; \
//      } else { \
//        table.m_status_##name = DBSTATUS_S_ISNULL ; \
//      } \
//  } while (0) ;

#define IGNORE_DB(table,name) \
  table.m_status_##name = DBSTATUS_S_IGNORE ;

#define SET_DB_VALUE(table,name,value) \
  do { \
    if (HUGE_VAL != value) { \
      table.m_##name = value ; \
      table.m_status_##name = DBSTATUS_S_OK ; \
    } else { \
      table.m_status_##name = DBSTATUS_S_ISNULL ; \
    } \
  } while (0) ;

#define SET_DB_VALUE_NOCHECK(table,name,value) \
  do { \
      table.m_##name = value ; \
      table.m_status_##name = DBSTATUS_S_OK ; \
	} while (0) ;
