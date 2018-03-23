/**
 * ogon - Free Remote Desktop Services
 * Qt Greeter Application
 *
 * Copyright (c) 2013-2018 Thincast Technologies GmbH
 *
 * Authors:
 * David Fort <contact@hardening-consulting.com>
 *
 * This file may be used under the terms of the GNU Affero General
 * Public License version 3 as published by the Free Software Foundation
 * and appearing in the file LICENSE-AGPL included in the distribution
 * of this file.
 *
 * Under the GNU Affero General Public License version 3 section 7 the
 * copyright holders grant the additional permissions set forth in the
 * ogon Core AGPL Exceptions version 1 as published by
 * Thincast Technologies GmbH.
 *
 * For more information see the file LICENSE in the distribution of this file.
 */

#include <QtWidgets/QApplication>
#include "abstract_greeter.h"
#include <unistd.h>


int main(int argc, char *argv[]) {
	QApplication app(argc, argv);

	bool ok;
	bool useEffects;
	QByteArray sessionIdStr = qgetenv("OGON_SID");
	quint32 sessionId = sessionIdStr.toUInt(&ok, 10);
	if (!ok) {
		qCritical("invalid OGON_SID environment variable '%s'", sessionIdStr.data());
		exit(1);
	}

	QString user = qgetenv("OGON_USER");
	QString domain = qgetenv("OGON_DOMAIN");
	QString bppstr = qgetenv("OGON_CONNECTION_BPP");
	int bpp = bppstr.toInt(&ok, 10);
	if (!ok) {
		qWarning("invalid OGON_CONNECTION_BPP environment variable '%s', using 32 bit!", bppstr.toLatin1().data());
		bpp = 32;
	}

	useEffects = !app.arguments().contains("--noeffects");

	AbstractGreeter greeter(sessionId, user, domain, useEffects, bpp < 25);
	app.installTranslator(greeter.getTranslator());
	greeter.show();
	return app.exec();
}
