#ifndef RMANAGER_H
#define RMANAGER_H

#include "robject.h"
#include "txter.h"

class RManager
{
public:
    RManager();

    //find the RObject* of app_name, return RObject*
    RObject* find(QString app_name);
    //add a new map between of app_name and RObject to m_r_map
    void add_r(QString app_name, RObject* r_object);
    //kill all RObject
    void kill_all_r_object();

private:
    QMap<QString,RObject*> m_r_map;     //the map between app_name and RObject
    Txter* m_txter;                     //write and read app.txt
};

#endif // RMANAGER_H
