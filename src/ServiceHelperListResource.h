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


#ifndef MACHINECONTROLLERLISTRESOURCE_H
#define MACHINECONTROLLERLISTRESOURCE_H

#include "Server/Resources/ListResource/ListResource.h"
#include "Storage/ListResourceFileSystemStorage.h"


#include <QObject>

class ServiceHelperListResource : public ListResource
{
    Q_OBJECT

public:
    explicit ServiceHelperListResource(IListResourceStorage *storage, QObject *parent = nullptr);
    bool deleteItem(QString token, QString deviceID);
    virtual ModificationResult appendItem(QVariant data, QString token) override;
    virtual ModificationResult removeItem(QString uuid, QString token, int index = -1) override;

signals:
private slots:
    void deviceRegistered(QString uuid);

private:
    QMap<QString, QString> _newDevices;

public slots:
};

#endif // MACHINECONTROLLERLISTRESOURCE_H
