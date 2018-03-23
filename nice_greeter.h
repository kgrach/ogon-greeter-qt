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

#ifndef __QOGON_NICE_GREETER_H__
#define __QOGON_NICE_GREETER_H__

#include <QObject>
#include <QTimer>
#include <QTranslator>
#include <QMap>

#include "abstract_greeter.h"
#include "ui_nice_greeter.h"

struct QOgonInterface;
class QGraphicsColorizeEffect;
class QStateMachine;


/** @brief */
class NiceGreeterWindow : public QMainWindow, public GreeterUi, private Ui_MainWindow {
	Q_OBJECT

public:
	NiceGreeterWindow(AbstractGreeter *controller, const QString &user, const QString &domain);
	virtual ~NiceGreeterWindow();

	/** @overload GreeterUi
	 * @{ */
	virtual void setStatus(const QString &toSet, bool visible);
	virtual QLineEdit *getPasswordWidget();
	virtual void setupEffects();
	virtual void setCurrentLanguage(const QString &lang);
	virtual void show();
	virtual void hide();
	virtual void close();
	virtual void onLoginSuccessful();
	virtual void onLoginFailed();

	virtual void onConnectionEstablished();
	virtual void onConnectionLost();
/** @} */

public slots:

	/* UI slots */
	void on_loginButton_clicked();
	void on_cancelButton_clicked();
	void on_timer_timeout();
	void on_loginEntry_textChanged(const QString &newText);
	void on_languageSelector_currentIndexChanged(int newIndex);

signals:
	void greeterInitialized();
	void fadeToGrey();
	void backToColors();

protected:
	virtual void changeEvent(QEvent *e);
	virtual void keyPressEvent(QKeyEvent* event);
	virtual void paintEvent(QPaintEvent *e);
	void loadAndScaleImage();

	QTimer	  				*mTimer;
	QStateMachine			*mGfxStateMachine;
	QGraphicsColorizeEffect *mGreyScaleGfx;
	AbstractGreeter			*mGreeter;

	QPixmap					mScaledPixmap;
	int						mScaledHeight;
	int						mScaledWidth;
};

#endif //__QOGON_NICE_GREETER_H__
