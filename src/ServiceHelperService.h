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


#ifndef MACHINECONTROLLINGSERVICE_H
#define MACHINECONTROLLINGSERVICE_H

#include <QObject>
#include <QVariant>
#include "Server/Services/IService.h"

class ServiceHelperService : public IService
{

public:
    ServiceHelperService(QObject* parent = nullptr);
    QString         getServiceName() override;
    QStringList     getServiceCalls() override;
    bool            call(QString call, QString token, QString cbID, QVariant argument = QVariant()) override;


private:
    QVariant syncCalls(QString call, QString token, QVariant argument);
};

#endif // MACHINECONTROLLINGSERVICE_H
