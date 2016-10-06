/**
*   \file MySQL.h
*   \author Thiebaud Vannier
*   \brief MySQL
*   \version 1.0
*/

#ifndef ___MYSQL__H_
#define ___MYSQL__H_

/** \brief
 *
 * \param addr char*
 * \param login char*
 * \param password char*
 * \param bdd char*
 * \param request char*
 * \return int 0=OK, 1=bad adress, login or password, 2=bad request
 *
 */
int MySQLRequest(char* addr, char* login, char* password, char* bdd, char* request);




#endif
