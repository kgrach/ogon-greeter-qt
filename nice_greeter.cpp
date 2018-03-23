/**
 * ogon - Free Remote Desktop Services
 * Qt Greeter - Hi-Res Greeter Window
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

#include <QtWidgets/QApplication>
#include <QGraphicsColorizeEffect>
#include <QPropertyAnimation>
#include <QStateMachine>
#include <QSignalTransition>
#include <QDateTime>
#include <qpa/qplatformnativeinterface.h>
#include <qogon/qogon.h>
#include <QKeyEvent>
#include <QtGui/QPainter>

#include "nice_greeter.h"


NiceGreeterWindow::NiceGreeterWindow(AbstractGreeter *controller, const QString &user, const QString &domain) :
	mTimer(new QTimer(this)),
	mGfxStateMachine(NULL),
	mGreeter(controller)
{
	mTimer->setObjectName("timer");

	setupUi(this);

	int i = 0;
	foreach(QString lang, controller->getLocales()) {
		languageSelector->addItem(lang.toUpper(), lang);

		// ugly style hack to customize QComboBox item
		languageSelector->setItemData(i, QColor(8, 30, 42), Qt::BackgroundRole);
		i++;
	}
	languageSelector->setCurrentIndex(0);

	QString userContent;
	if(domain.size())
		userContent += domain + '\\';
	if(user.size())
		userContent += user;
	loginEntry->setText(userContent);

	if(user.size())
		passwordEntry->setFocus();
	else
		loginEntry->setFocus();

	on_loginEntry_textChanged(userContent);
	statusLabel->hide();

	on_timer_timeout();

	mScaledHeight = 0;
	mScaledWidth = 0;
}

void NiceGreeterWindow::loadAndScaleImage()
{
	if (mScaledHeight == height() && mScaledWidth == width()) return;
	if (mGreeter->is16Bit()) {
		mScaledPixmap.load(":/images/forms/ogon_background_16.png");
	} else {
		mScaledPixmap.load(":/images/forms/ogon_background_24.png");
	}
	mScaledPixmap = mScaledPixmap.scaled(width(),height());
	mScaledHeight = height();
	mScaledWidth = width();
}


NiceGreeterWindow::~NiceGreeterWindow() {
	if (mTimer->isActive())
		mTimer->stop();

	delete mTimer;
	delete mGfxStateMachine;
}


void NiceGreeterWindow::setStatus(const QString &toSet, bool visible) {
	statusLabel->setVisible(visible);
	statusLabel->setText(toSet);
}

QLineEdit *NiceGreeterWindow::getPasswordWidget() {
	return passwordEntry;
}


void NiceGreeterWindow::setCurrentLanguage(const QString &lang) {
	int idx = mGreeter->getLocales().indexOf(lang, 0);
	if(idx > 0)
		languageSelector->setCurrentIndex(idx);
}

void NiceGreeterWindow::show() {
	showFullScreen();
}

void NiceGreeterWindow::hide() {
	QWidget::hide();
}

void NiceGreeterWindow::close() {
	QWidget::close();
}

void NiceGreeterWindow::onLoginSuccessful() {
	emit backToColors();
}

void NiceGreeterWindow::onLoginFailed() {
	this->passwordEntry->clear();
	this->passwordEntry->setFocus();
	emit backToColors();
}

void NiceGreeterWindow::setupEffects() {
	QColor q(QColor::Hsv);
	q.setHsv(0, 0, 0, 255);

	mGreyScaleGfx = new QGraphicsColorizeEffect();
	mGreyScaleGfx->setColor(q);
	mGreyScaleGfx->setStrength(0.0);
	this->setGraphicsEffect(mGreyScaleGfx);


	/**
	 * We build a simple stateMachine to handle the fadeTo [Grey | Color] effect
	 *
	 *           screenReady()         action()
	 *
	 * initialState ----> colorsState ---------> greyedState
	 *                        ^                       |
	 *                        |                       |
	 *                        +-----------------------+
	 *                              actionResult()
	 *
	 *  The only trick is that the transition between initialState and colorsState
	 *  is set to take longer (to see a nice fade-in at connection)
	 */
	mGfxStateMachine = new QStateMachine();

	QState *initialState = new QState(mGfxStateMachine);
	initialState->assignProperty(mGreyScaleGfx, "strength", 1.0);

	QState *colorsState = new QState(mGfxStateMachine);
	colorsState->assignProperty(mGreyScaleGfx, "strength", 0.0);

	QState *greyedState = new QState(mGfxStateMachine);
	greyedState->assignProperty(mGreyScaleGfx, "strength", 1.0);

	mGfxStateMachine->setInitialState(initialState);

	QSignalTransition *init2colors;
	QObject *qogon = mGreeter->getQOgon();

	if (qogon)
		init2colors = initialState->addTransition(qogon,	SIGNAL(ogonScreenReady()), colorsState);
	else
		init2colors = initialState->addTransition(this,	SIGNAL(greeterInitialized()), colorsState);

	QPropertyAnimation *initAnimation = new QPropertyAnimation(mGreyScaleGfx, "strength");
	initAnimation->setDuration(800);
	init2colors->addAnimation(initAnimation);

	QSignalTransition *color2grey = colorsState->addTransition(this, SIGNAL(fadeToGrey()), greyedState);
	color2grey->addAnimation(new QPropertyAnimation(mGreyScaleGfx, "strength"));

	QSignalTransition *grey2colors = greyedState->addTransition(this, SIGNAL(backToColors()), colorsState);
	grey2colors->addAnimation(new QPropertyAnimation(mGreyScaleGfx, "strength"));
	mGfxStateMachine->start();
}

void NiceGreeterWindow::onConnectionEstablished() {
	if (!mGreeter->getQOgon())
		emit this->greeterInitialized();
	on_timer_timeout();
	mTimer->start(1000);
}

void NiceGreeterWindow::onConnectionLost() {
	mTimer->stop();
}


void NiceGreeterWindow::changeEvent(QEvent *e) {
	if(e->type() == QEvent::LanguageChange) {
		//qDebug("language change");
		retranslateUi(this);
	} else {
		QMainWindow::changeEvent(e);
	}
}

void NiceGreeterWindow::keyPressEvent(QKeyEvent* event) {
	if (event->key() == Qt::Key_Escape) {
		mGreeter->cancelPressed();
		return;
	}
	QWidget::keyPressEvent(event);
}


void NiceGreeterWindow::on_loginButton_clicked() {
	emit fadeToGrey();

	mGreeter->loginPressed(loginEntry->text(), passwordEntry->text());
}

void NiceGreeterWindow::on_cancelButton_clicked() {
	mGreeter->cancelPressed();
}

void NiceGreeterWindow::on_timer_timeout() {
	QDateTime now = QDateTime::currentDateTime();
	dateLabel->setText( now.toString("dd MMM yyyy") );
	timeLabel->setText( now.toString("hh:mm") );
}

void NiceGreeterWindow::on_loginEntry_textChanged(const QString &newText) {
	QString login, domain;
	AbstractGreeter::extractLoginAndDomain(newText, &login, &domain);

	QString toSet;
	if(domain.size()) {
		toSet += "<b>" + tr("domain:") + "</b>"+ domain + " ";
		if(login.size())
			toSet += "<b>" + tr("user:") + "</b> " + login;
	}
	feedbackLabel->setText(toSet);
	feedbackLabel->setVisible(toSet.size() > 0);
}

void NiceGreeterWindow::on_languageSelector_currentIndexChanged(int newIndex) {
	QString newLocale = mGreeter->getLocales().at(newIndex);
	if(!mGreeter->getTranslator()->load("greeter_" + newLocale, TRANSLATIONS_DIR))
		qWarning("%s: an error occurred when load the new locale '%s'", __FUNCTION__,
				newLocale.toLatin1().data());
}

void NiceGreeterWindow::paintEvent(QPaintEvent *e) {
	QPainter painter(this);
	loadAndScaleImage();
	painter.drawPixmap(0, 0, mScaledPixmap);
}

