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
#include "Server/Services/ServiceManager.h"
#include "ServiceHelperService.h"
#include "ServiceHelperListResourceFactory.h"
#include <QDebug>

_2logServiceHelperPlugin::_2logServiceHelperPlugin(QObject *parent) : IPlugin(parent)
{

}

bool _2logServiceHelperPlugin::init(QVariantMap parameters)
{
    Q_UNUSED(parameters)
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
