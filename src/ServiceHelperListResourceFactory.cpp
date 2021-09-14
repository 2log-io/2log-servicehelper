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


#include "ServiceHelperListResourceFactory.h"
#include "Server/Authentication/AuthentificationService.h"

//#define LOG_MAPPINGS_DESCRIPTOR QStringLiteral("logger/mappings")

ServiceHelperListResourceFactory::ServiceHelperListResourceFactory(QObject *parent) :IResourceFactory(parent)
{

}

ServiceHelperListResourceFactory::~ServiceHelperListResourceFactory()
{

}

QString ServiceHelperListResourceFactory::getResourceID(QString descriptor, QString token) const
{
    iIdentityPtr user = AuthenticationService::instance()->validateToken(token);
    if(user.isNull() || !(user->isAuthorizedTo(IS_ADMIN) || user->isAuthorizedTo(SERVICE)))
        return "";

    return descriptor;
}

QString ServiceHelperListResourceFactory::getResourceType() const
{
    return "synclist";
}

QString ServiceHelperListResourceFactory::getDescriptorPrefix() const
{
    return "2log/controller";
}


resourcePtr ServiceHelperListResourceFactory::createResource(QString token, QString descriptor, QObject *parent)
{
    Q_UNUSED(parent)

    iIdentityPtr user = AuthenticationService::instance()->validateToken(token);
    if( user.isNull() || !(user->isAuthorizedTo(IS_ADMIN) || user->isAuthorizedTo(SERVICE)))
        return nullptr;

     IListResourceStorage* storage = new ListResourceFileSystemStorage(descriptor, nullptr);
     auto ptr = resourcePtr(new ServiceHelperListResource(storage));
     return ptr;
}
