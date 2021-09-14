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


#include "ServiceHelperService.h"
#include "ServiceHelperLogic.h"
#include "Server/Devices/DeviceManager.h"
#include "Server/Resources/ResourceManager/ResourceManager.h"
#include "ServiceHelperListResource.h"

ServiceHelperService::ServiceHelperService(QObject* parent): IService (parent)
{

}

QString ServiceHelperService::getServiceName()
{
    return "machineControl";
}

QStringList ServiceHelperService::getServiceCalls()
{
    QStringList commands;
    commands << "hookSwitch" << "hookDot";
    return commands;
}

bool ServiceHelperService::call(QString call, QString token, QString cbID, QVariant argument)
{

    QVariant answer = syncCalls(call, token, argument);

    if(answer.isValid())
    {
        Q_EMIT response(cbID, answer);
        return true;
    }

    return false;
}

QVariant ServiceHelperService::syncCalls(QString call, QString token, QVariant argument)
{
    QVariantMap argMap = argument.toMap();
    QString deviceID = argMap["deviceID"].toString();
    QString shortID = argMap["shortID"].toString();
    bool force = argMap["force"].toBool();


    if(call == "newController")
    {
        QVariantMap returnVal;
        returnVal["success"] = false;
        QString type = argMap["type"].toString();
        if(type.isEmpty())
            return returnVal;

        resourcePtr resource = ResourceManager::instance()->getOrCreateResource("synclist", "2log/controller/"+type, token);
        if(resource.isNull())
            return returnVal;

        ServiceHelperListResource* resourcePtr = qobject_cast<ServiceHelperListResource*>(resource.data());
        QVariantMap data;

        data["name"]     = argMap["name"].toString();
        data["deviceID"] = deviceID;

        ServiceHelperListResource::ModificationResult result = resourcePtr->appendItem(data, token);

        if(resourcePtr && result.error == IResource::NO_ERROR)
        {
            returnVal["data"] = result.data;
            returnVal["success"] = true;
            return returnVal;
        }

        return returnVal;
    }


    if(call == "hookSwitch")
    {
        return ServiceHelperLogic::hookSwitch(shortID, deviceID, token, force);
    }

    if(call == "hookDot")
    {
        return ServiceHelperLogic::hookDot(shortID, deviceID, token, force);
    }


    if(call == "prepareDotMappingWighUUID")
    {
        QString uuid = argMap["uuid"].toString();
        return ServiceHelperLogic::prepareDotHook(uuid, deviceID, token);
    }


    if(call == "prepareSwitchMappingWighUUID")
    {
        QString uuid = argMap["uuid"].toString();
        return ServiceHelperLogic::prepareSwitchHook(uuid, deviceID, token);
    }

    if(call == "deleteController")
    {
        QVariantMap returnVal;
        returnVal["success"] = false;

        QString type = argMap["type"].toString();
        if(type.isEmpty())
            return returnVal;

        resourcePtr resource = ResourceManager::instance()->getOrCreateResource("synclist", "2log/controller/"+type, token);
        if(resource.isNull())
            return returnVal;

        ServiceHelperListResource* resourcePtr = qobject_cast<ServiceHelperListResource*>(resource.data());
        if(resourcePtr && resourcePtr->deleteItem(token, deviceID))
        {
            ServiceHelperLogic::unhookDot(deviceID, token);
            ServiceHelperLogic::unhookSwitch(deviceID, token);
            returnVal["success"] = true;
            return returnVal;
        }
    }

    //######################## DEPRECATED

    if(call == "deleteMachine") // DEPRECATED
    {
        QVariantMap returnVal;
        returnVal["success"] = false;

        resourcePtr resource = ResourceManager::instance()->getOrCreateResource("synclist", "2log/controller/machines", token);
        if(resource.isNull())
            return returnVal;

        ServiceHelperListResource* resourcePtr = qobject_cast<ServiceHelperListResource*>(resource.data());
        if(resourcePtr && resourcePtr->deleteItem(token, deviceID))
        {
            ServiceHelperLogic::unhookDot(deviceID, token);
            ServiceHelperLogic::unhookSwitch(deviceID, token);
            returnVal["success"] = true;
            return returnVal;
        }
    }

    if(call == "deleteSuction") // DEPRECATED
    {
        QVariantMap returnVal;
        returnVal["success"] = false;

        resourcePtr resource = ResourceManager::instance()->getOrCreateResource("synclist", "2log/controller/suctions", token);
        if(resource.isNull())
            return returnVal;

        ServiceHelperListResource* resourcePtr = qobject_cast<ServiceHelperListResource*>(resource.data());
        if(resourcePtr && resourcePtr->deleteItem(token, deviceID))
        {
            ServiceHelperLogic::unhookSwitch(deviceID, token);
            returnVal["success"] = true;
            return returnVal;
        }
    }

    if(call == "newMachine") // DEPRECATED
    {
        QVariantMap returnVal;
        returnVal["success"] = false;

        resourcePtr resource = ResourceManager::instance()->getOrCreateResource("synclist", "2log/controller/machines", token);
        if(resource.isNull())
            return returnVal;

        ServiceHelperListResource* resourcePtr = qobject_cast<ServiceHelperListResource*>(resource.data());
        QVariantMap data;

        data["name"]     = argMap["name"].toString();
        data["deviceID"] = deviceID;

        ServiceHelperListResource::ModificationResult result = resourcePtr->appendItem(data, token);

        if(resourcePtr && result.error == IResource::NO_ERROR)
        {
            returnVal["data"] = result.data;
            returnVal["success"] = true;
            return returnVal;
        }

        return returnVal;
    }


    if(call == "newSuction") // DEPRECATED
    {
        QVariantMap returnVal;
        returnVal["success"] = false;

        resourcePtr resource = ResourceManager::instance()->getOrCreateResource("synclist", "2log/controller/suctions", token);
        if(resource.isNull())
            return returnVal;

        ServiceHelperListResource* resourcePtr = qobject_cast<ServiceHelperListResource*>(resource.data());
        QVariantMap data;

        data["name"]     = argMap["name"].toString();
        data["deviceID"] = deviceID;

        ServiceHelperListResource::ModificationResult result = resourcePtr->appendItem(data, token);

        if(resourcePtr && result.error == IResource::NO_ERROR)
        {
            returnVal["data"] = result.data;
            returnVal["success"] = true;
            return returnVal;
        }

        return returnVal;
    }
    return QVariant();
}
