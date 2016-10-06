#include <winsock.h>
#include <MYSQL/mysql.h>

int MySQLRequest(char* addr, char* login, char* password, char* bdd, char* request)
{
    MYSQL mysql;
    //init de mysql
    mysql_init(&mysql);
    mysql_options(&mysql,MYSQL_READ_DEFAULT_GROUP,"option");
    //on ce connecte au serveur
    if(mysql_real_connect(&mysql,addr,login,password,bdd,0,NULL,0))
    {
        //on execute la requete
        if (mysql_query(&mysql, request))
        {
            mysql_close(&mysql);
            return 2;
        }
        //on ferme la base de donnée
        mysql_close(&mysql);
        return 0;
    }
    else
    {
        return 1;
    }
}
