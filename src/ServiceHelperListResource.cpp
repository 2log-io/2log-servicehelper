/*   2log.io
 *   Copyright (C) 2021 - 2log.io | mail@2log.io,  mail@friedemann-metzger.de
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU Affero General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU Affero General Public License for more details.
 *
 *   You should have received a copy of the GNU Affero General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#include "ServiceHelperListResource.h"
#include <QUuid>
#include <QDateTime>
#include "Server/Devices/DeviceManager.h"

ServiceHelperListResource::ServiceHelperListResource(IListResourceStorage *storage, QObject *parent) : ListResource(storage,parent)
{
    setAllowUserAccess(false);
    connect(DeviceManager::instance(), &DeviceManager::deviceRegistered, this, &ServiceHelperListResource::deviceRegistered);
}

IResource::ModificationResult ServiceHelperListResource::appendItem(QVariant data, QString token)
{
    iIdentityPtr user = AuthenticationService::instance()->validateToken(token);
    QVariantMap map = data.toMap();
    QString displayName = map["name"].toString();
    if(displayName.isEmpty())
    {
        ModificationResult result;
        result.error =  INVALID_PARAMETERS;
        return result;
    }

    if(user.isNull() || !(user->isAuthorizedTo(IS_ADMIN)||user->isAuthorizedTo(SERVICE)))
    {
        ModificationResult result;
        result.error =  PERMISSION_DENIED;
        return result;
    }

    QString uid = map.value("deviceID", "").toString();
    if(uid.isEmpty())
    {
        uid = displayName.toLower().trimmed().replace(" ","_")+"_"+QString::number(QDateTime::currentDateTime().toSecsSinceEpoch());
        uid = uid.replace("ß", "ss");
        uid = uid.replace("ä", "ae");
        uid = uid.replace("ö", "oe");
        uid = uid.replace("ü", "ue");
    }

    QVariantMap toInsert;
    toInsert["name"] = displayName;
    toInsert["deviceID"]  =  uid;
    _newDevices[uid] = token;
    auto result = ListResource::appendItem(toInsert, user);
    return result;
}

IResource::ModificationResult ServiceHelperListResource::removeItem(QString uuid, QString token, int index)
{
    iIdentityPtr user = AuthenticationService::instance()->validateToken(token);
    if(user.isNull() || !user->isAuthorizedTo(IS_ADMIN))
    {
        ModificationResult result;
        result.error =  PERMISSION_DENIED;
        return result;
    }

    QVariantMap item = getItem(index, uuid).toMap();
    DeviceManager::instance()->setDeviceMapping(token, item["data"].toMap()["deviceID"].toString(), "");

    auto result = ListResource::removeItem(index, user, uuid);
    return result;
}

bool ServiceHelperListResource::deleteItem(QString token, QString deviceID)
{
    auto tempListData = getListData();
    QListIterator<QVariant> it(tempListData);
    int idx = -1;
    while(it.hasNext())
    {
       idx ++;
       QVariantMap map = it.next().toMap();
       if(map.value("data").toMap().value("deviceID").toString() == deviceID)
       {
           DeviceManager::instance()->setDeviceMapping(token, deviceID, "");
           ListResource::removeItem(idx, AuthenticationService::instance()->validateToken(token), map.value("uuid").toString());
           return true;
       }
    }

    return false;
}

void ServiceHelperListResource::deviceRegistered(QString uuid)
{
    if(_newDevices.contains(uuid))
    {
        DeviceManager::instance()->setDeviceMapping(_newDevices[uuid], uuid, uuid);
        _newDevices.remove(uuid);
    }
}

