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


#ifndef MACHINECONTROLLINGLOGIC_H
#define MACHINECONTROLLINGLOGIC_H

#include <QObject>
#include <QVariant>



class ServiceHelperLogic : public QObject
{
    Q_OBJECT

public:
    explicit ServiceHelperLogic(QObject *parent = nullptr);
    static QVariantMap hookSwitch(QString shortID, QString deviceID, QString token, bool force);
    static QVariantMap hookDot(QString shortID, QString deviceID, QString token, bool force);
    static QVariantMap prepareDotHook(QString uuid, QString deviceID, QString token);
    static QVariantMap prepareSwitchHook(QString uuid, QString deviceID, QString token);
    static QVariantMap unhookDot(QString deviceID, QString token);
    static QVariantMap unhookSwitch(QString deviceID, QString token);

private:
    static QVariantMap hookDevice(QString shortID, QString deviceID, QString mappingSuffix, QString internalMappingProperty, QString token,  bool force);

signals:

public slots:
};

#endif // MACHINECONTROLLINGLOGIC_H
