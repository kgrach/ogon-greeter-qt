/**
 * ogon - Free Remote Desktop Services
 * Qt Greeter - Lo-Res Greeter Window
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

#include <qpa/qplatformnativeinterface.h>
#include <qogon/qogon.h>
#include <QKeyEvent>

#include "lowres_greeter.h"
#include "abstract_greeter.h"


LowResGreeterWindow::LowResGreeterWindow(AbstractGreeter *controller, const QString &user,
										const QString &domain) :
	mGreeter(controller)
{
	setupUi(this);

	QString userContent;
	if(domain.size())
		userContent += domain + '\\';
	if(user.size()) {
		userContent += user;
		passwordEntry->setFocus();
	} else
		loginEntry->setFocus();

	loginEntry->setText(userContent);
	statusLabel->setVisible(false);
}

LowResGreeterWindow::~LowResGreeterWindow() {
}


void LowResGreeterWindow::setupEffects() {
}


void LowResGreeterWindow::setStatus(const QString &toSet, bool visible) {
	statusLabel->setVisible(visible);
	statusLabel->setText(toSet);
}

QLineEdit *LowResGreeterWindow::getPasswordWidget() {
	return passwordEntry;
}

void LowResGreeterWindow::setCurrentLanguage(const QString & lang) {
	Q_UNUSED(lang);
}

void LowResGreeterWindow::show() {
	showFullScreen();
}

void LowResGreeterWindow::hide() {
	QWidget::hide();
}

void LowResGreeterWindow::close() {
	QWidget::close();
}


void LowResGreeterWindow::onConnectionEstablished() {
}

void LowResGreeterWindow::onConnectionLost() {
}

void LowResGreeterWindow::onLoginSuccessful() {
}

void LowResGreeterWindow::onLoginFailed() {
	this->passwordEntry->clear();
	this->passwordEntry->setFocus();
}


void LowResGreeterWindow::changeEvent(QEvent *e) {
	if(e->type() == QEvent::LanguageChange) {
		//qDebug("language change");
		retranslateUi(this);
	} else {
		QMainWindow::changeEvent(e);
	}
}

void LowResGreeterWindow::keyPressEvent(QKeyEvent* event) {
	if (event->key() == Qt::Key_Escape) {
		mGreeter->cancelPressed();
		return;
	}
	QWidget::keyPressEvent(event);
}

void LowResGreeterWindow::on_loginButton_clicked() {
	mGreeter->loginPressed(loginEntry->text(), passwordEntry->text());
}

void LowResGreeterWindow::on_cancelButton_clicked() {
	mGreeter->cancelPressed();
}
