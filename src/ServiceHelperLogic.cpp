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


#include "ServiceHelperLogic.h"
#include "Server/Devices/DeviceProperty.h"
#include "Server/Devices/DeviceManager.h"

ServiceHelperLogic::ServiceHelperLogic(QObject *parent) : QObject(parent)
{

}

QVariantMap ServiceHelperLogic::hookSwitch(QString shortID, QString deviceID, QString token, bool force)
{
    return hookDevice(shortID, deviceID,"switch","switchHook", token, force);
}


QVariantMap ServiceHelperLogic::hookDot(QString shortID, QString deviceID, QString token, bool force)
{
    return hookDevice(shortID, deviceID,"dot","readerHook", token, force);
}

QVariantMap ServiceHelperLogic::prepareDotHook(QString uuid, QString deviceID, QString token)
{
    QVariantMap result;
    result["errCode"] = 0;
    result["errString"] = "No Error";
    deviceHandlePtr handle = DeviceManager::instance()->getHandle(deviceID);
    if(handle.isNull())
    {
        result["errCode"] = -10;
        result["errString"] = "Unknown device ID";
        return  result;
    }

    QString mapping = deviceID+"/dot";
    DeviceProperty* prop = handle->property("readerHook");
    prop->setValue(mapping);
    Err::CloudError err = DeviceManager::instance()->prepareDeviceMapping(token, mapping, uuid);
    if(err != Err::NO_ERROR)
    {
        result["errCode"] = err;
        result["errString"] = "Error";
    }

    return result;
}

QVariantMap ServiceHelperLogic::prepareSwitchHook(QString uuid, QString deviceID, QString token)
{
    QVariantMap result;
    result["errCode"] = 0;
    result["errString"] = "No Error";
    deviceHandlePtr handle = DeviceManager::instance()->getHandle(deviceID);
    if(handle.isNull())
    {
        result["errCode"] = -10;
        result["errString"] = "Unknown device ID";
        return  result;
    }

    QString mapping = deviceID+"/switch";
    DeviceProperty* prop = handle->property("switchHook");
    prop->setValue(mapping);
    Err::CloudError err = DeviceManager::instance()->prepareDeviceMapping(token, mapping, uuid);
    if(err != Err::NO_ERROR)
    {
        result["errCode"] = err;
        result["errString"] = "Error";
    }

    return result;
}

QVariantMap ServiceHelperLogic::unhookDot(QString deviceID, QString token)
{
    QString mapping = deviceID+"/switch";
    auto err = DeviceManager::instance()->setDeviceMapping(token, mapping, "", true);

    QVariantMap result;
    if(err != Err::NO_ERROR)
    {
        result["errCode"] = err;
        result["errString"] = "Error";
    }

    return result;
}

QVariantMap ServiceHelperLogic::unhookSwitch(QString deviceID, QString token)
{
    QString mapping = deviceID+"/dot";
    auto err = DeviceManager::instance()->setDeviceMapping(token, mapping, "", true);

    QVariantMap result;
    if(err != Err::NO_ERROR)
    {
        result["errCode"] = err;
        result["errString"] = "Error";
    }
    return result;
}


QVariantMap ServiceHelperLogic::hookDevice(QString shortID, QString deviceID, QString mappingSuffix, QString internalMappingProperty, QString token, bool force)
{

    QVariantMap result;
    result["errCode"] = 0;
    result["errString"] = "No Error";
    deviceHandlePtr handle = DeviceManager::instance()->getHandle(deviceID);
    if(handle.isNull())
    {
        result["errCode"] = -10;
        result["errString"] = "Unknown device ID";
        return  result;
    }

    QString uuid = DeviceManager::instance()->getUuidForShortId(shortID);
    if(uuid.isEmpty())
    {
        result["errCode"] = -11;
        result["errString"] = "Device not found";
        return  result;
    }

    QString type = DeviceManager::instance()->getTypeForUuid(uuid);
    // check device type -> avoid hook a switch as a dot
    if(!type.toLower().contains(mappingSuffix.toLower()))
    {
        result["errCode"] = -12;
        result["errString"] = "Wrong device type";
        return  result;
    }


    QString mapping = deviceID+"/"+mappingSuffix;
    DeviceProperty* prop = handle->property(internalMappingProperty);
    prop->setValue(mapping);
    Err::CloudError err = DeviceManager::instance()->setDeviceMappingByShortId(token, mapping, shortID, force);
    if(err != Err::NO_ERROR)
    {
        result["errCode"] = err;
        result["errString"] = "Error";
    }

    return result;

}
