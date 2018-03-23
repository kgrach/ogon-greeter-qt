/**
 * ogon - Free Remote Desktop Services
 * Qt Greeter - Abstract Greeter Class
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

#ifndef __ABSTRACT_GREETER_H_
#define __ABSTRACT_GREETER_H_

#include <QObject>
#include <QStringList>
#include <QTranslator>

#define SBP_VERSION_MAJOR 1
#define SBP_VERSION_MINOR 0

struct QOgonInterface;
class QLineEdit;

/** @brief the interface that greeter UIs must implement */
struct GreeterUi {
	/** dtor */
	virtual ~GreeterUi() { }

	/** setup the graphical effects if any */
	virtual void setupEffects() = 0;

	/** called to set a status label
	 * @param toSet
	 * @param visible
	 */
	virtual void setStatus(const QString &toSet, bool visible) = 0;

	/** called to retrieve the password widget (mainly to set focus on it)
	 * @return
	 */
	virtual QLineEdit *getPasswordWidget() = 0;

	/**
	 * @param lang
	 */
	virtual void setCurrentLanguage(const QString &lang) = 0;

	/** shows the UI */
	virtual void show() = 0;

	/** hides the UI */
	virtual void hide() = 0;

	/** closes the UI */
	virtual void close() = 0;

	virtual void onConnectionEstablished() = 0;

	virtual void onConnectionLost() = 0;

	/** callback called when the login fails */
	virtual void onLoginSuccessful() = 0;

	/** callback called when the login fails */
	virtual void onLoginFailed() = 0;
};


/** @brief a class that implements the greeter logic */
class AbstractGreeter : public QObject {
	Q_OBJECT

public:
	AbstractGreeter(quint32 sessionId, const QString &user, const QString &domain, bool useEffects, bool use16Bit);
	virtual ~AbstractGreeter();

	QTranslator *getTranslator();

public:
	const QStringList &getLocales();
	QObject *getQOgon();
	void loginPressed(const QString &login, const QString &password);
	void cancelPressed();
	void show();
	bool is16Bit();

public:
	/** extract user and domain from a "domain\user" or "user@domain" string
	 * @param input input string
	 * @param user the target user string
	 * @param domain the target domain string
	 */
	static void extractLoginAndDomain(const QString &input, QString *user, QString *domain);

public slots:
	void onConnectionEstablished();
	void onConnectionLost();
	void onScreenReady();
	void onSbpReply(quint32 tag, quint32 sbpType, quint32 status, const QByteArray &reply);

signals:
	void greeterInitialized();

protected:

	void handleAuthResponse(const QByteArray &reply);
	void handleEndResponse(const QByteArray &reply);
	void handleVersionInfoResponse(const QByteArray &reply);

	QTranslator			mQtTranslator;
	QTranslator			mGreeterTranslator;
	quint32				mSessionId;
	QOgonInterface 	*mOgon;

	quint32				mAuthReqTag;
	quint32				mLogoffReqTag;
	quint32				mVersionInfoTag;
	bool				mUse16Bit;

	QStringList  mLocalizationFiles;
	GreeterUi *mNiceGreeter;
	GreeterUi *mLowResGreeter;
	GreeterUi *mUi;
};



#endif /* __ABSTRACT_GREETER_H_ */
