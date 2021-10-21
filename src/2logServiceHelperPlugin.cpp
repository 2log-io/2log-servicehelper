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


#include "2logServiceHelperPlugin.h"
#include "Server/Resources/ResourceManager/ResourceManager.h"
#include "Server/Authentication/DefaultAuthenticator.h"
#include "Server/Authentication/User.h"
#include "Server/Services/ServiceManager.h"
#include "Services/FablabAuthenticator.h"
#include "ServiceHelperService.h"
#include "ServiceHelperListResourceFactory.h"
#include <QDebug>
#include <QUuid>
#include <QDir>

_2logServiceHelperPlugin::_2logServiceHelperPlugin(QObject *parent) : IPlugin(parent)
{

}

_2logServiceHelperPlugin::~_2logServiceHelperPlugin()
{
    _serviceProcess.terminate();
}

bool _2logServiceHelperPlugin::init(QVariantMap parameters)
{
    Q_UNUSED(parameters)

    QString path;
    #ifdef Q_OS_MACOS
        path = QDir::currentPath()+"/../../../services/2log.services.app/Contents/MacOS/2log.services";
    #else
        path =  QDir::currentPath()+"/services/2log.services";
    #endif


    QString user = "service";
    QString pass = QUuid::createUuid().toString();

    userPtr serviceUser =  DefaultAuthenticator::instance()->createUser(user, pass);
    serviceUser->setUserPermission("service",true,false);
    serviceUser->setUserPermission("lab.service", true, false);
    serviceUser->setUserPermission("lab.admin", true);
    FablabAuthenticator::instance()->addUser("service", iUserPtr(serviceUser));
    QProcess::execute("killall", {"2log.services"});
    _serviceProcess.start(path, {"-p", pass,"-u", user});
    ResourceManager::instance()->addResourceFactory(new ServiceHelperListResourceFactory(this));
    ServiceManager::instance()->registerService(new ServiceHelperService(this));
    return true;
}

bool _2logServiceHelperPlugin::shutdown()
{
    return false;
}

QString _2logServiceHelperPlugin::getPluginName()
{
    return "2log-servicehelper";
}
