/**
 * ogon - Free Remote Desktop Services
 * Qt Greeter - Low-Res Greeter Window
 *
 * Copyright (c) 2013-2018 Thincast Technologies GmbH
 *
 * Authors:
 * David Fort <contact@hardening-consulting.com>
 * Martin Haimberger <martin.haimberger@thincast.com>
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

#ifndef __QOGON_LOWRES_GREETER_H__
#define __QOGON_LOWRES_GREETER_H__

#include <QObject>

#include "abstract_greeter.h"
#include "ui_small_greeter.h"

struct QOgonInterface;
class AbstractGreeter;

/** @brief */
class LowResGreeterWindow : public QMainWindow, public GreeterUi, private Ui_lowResWindow {
	Q_OBJECT

public:
	/**
	 *
	 * @param controller
	 * @param user
	 * @param domain
	 */
	LowResGreeterWindow(AbstractGreeter *controller, const QString &user, const QString &domain);

	/** dtor */
	virtual ~LowResGreeterWindow();

	/** @overload GreeterUi
	 * @{ */
	virtual void setStatus(const QString &toSet, bool visible);
	virtual QLineEdit *getPasswordWidget();
	virtual void setupEffects();
	virtual void setCurrentLanguage(const QString &lang);
	virtual void show();
	virtual void hide();
	virtual void close();
	virtual void onConnectionEstablished();
	virtual void onConnectionLost();
	virtual void onLoginSuccessful();
	virtual void onLoginFailed();
	/** @} */

public slots:
	void on_loginButton_clicked();
	void on_cancelButton_clicked();

protected:
	void handleAuthResponse(const QByteArray &reply);
	void handleEndResponse(const QByteArray &reply);
	virtual void changeEvent(QEvent *e);
	virtual void keyPressEvent(QKeyEvent* event);

	AbstractGreeter *mGreeter;
};

#endif //__QOGON_NICE_GREETER_H__
