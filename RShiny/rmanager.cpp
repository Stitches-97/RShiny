#include "rmanager.h"

RManager::RManager()
{
    m_txter = new Txter();
}

RObject* RManager::find(QString app_name)
{
    return m_r_map[app_name];
}

void RManager::add_r(QString app_name, RObject* r_object)
{
    m_r_map.insert(app_name,r_object);
}

void RManager::kill_all_r_object()
{
    QMap<QString,RObject*>::Iterator it = m_r_map.begin();
    while(it != m_r_map.end())
    {
        RObject* r_object = it.value();
        QString app_name = r_object->m_app_name;
        qint64 process_id = r_object->m_process_id;
        QString url = r_object->m_url;
        if(process_id <= 0)
        {
            it++;
            continue;
        }
        r_object->kill_r_object();
        QString out = "app_name: " + app_name + "  process_id: " + QString::number(process_id) + "  url: " + url + "  success close";
        qDebug()<<out;
        r_object->m_process_id = 0;
        r_object->m_url = "";
        m_r_map.insert(app_name,r_object);
        it++;
    }
}
