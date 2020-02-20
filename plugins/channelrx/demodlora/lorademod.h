///////////////////////////////////////////////////////////////////////////////////
// Copyright (C) 2012 maintech GmbH, Otto-Hahn-Str. 15, 97204 Hoechberg, Germany //
// (C) 2015 John Greb                                                            //
// (C) 2020 Edouard Griffiths, F4EXB                                             //
//                                                                               //
// This program is free software; you can redistribute it and/or modify          //
// it under the terms of the GNU General Public License as published by          //
// the Free Software Foundation as version 3 of the License, or                  //
// (at your option) any later version.                                           //
//                                                                               //
// This program is distributed in the hope that it will be useful,               //
// but WITHOUT ANY WARRANTY; without even the implied warranty of                //
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the                  //
// GNU General Public License V3 for more details.                               //
//                                                                               //
// You should have received a copy of the GNU General Public License             //
// along with this program. If not, see <http://www.gnu.org/licenses/>.          //
///////////////////////////////////////////////////////////////////////////////////

#ifndef INCLUDE_LORADEMOD_H
#define INCLUDE_LORADEMOD_H

#include <vector>

#include "dsp/basebandsamplesink.h"
#include "channel/channelapi.h"
#include "util/message.h"

#include "lorademodbaseband.h"
#include "lorademoddecoder.h"

class DeviceAPI;
class QThread;

class LoRaDemod : public BasebandSampleSink, public ChannelAPI {
public:
    class MsgConfigureLoRaDemod : public Message {
        MESSAGE_CLASS_DECLARATION

    public:
        const LoRaDemodSettings& getSettings() const { return m_settings; }
        bool getForce() const { return m_force; }

        static MsgConfigureLoRaDemod* create(const LoRaDemodSettings& settings, bool force)
        {
            return new MsgConfigureLoRaDemod(settings, force);
        }

    private:
        LoRaDemodSettings m_settings;
        bool m_force;

        MsgConfigureLoRaDemod(const LoRaDemodSettings& settings, bool force) :
            Message(),
            m_settings(settings),
            m_force(force)
        { }
    };

    class MsgReportDecodeBytes : public Message {
        MESSAGE_CLASS_DECLARATION

    public:
        const QByteArray& getBytes() const { return m_bytes; }
        unsigned int getSyncWord() const { return m_syncWord; }
        float getSingalDb() const { return m_signalDb; }
        float getNoiseDb() const { return m_noiseDb; }
        unsigned int getPacketSize() const { return m_packetSize; }
        unsigned int getNbParityBits() const { return m_nbParityBits; }
        unsigned int getNbSymbols() const { return m_nbSymbols; }
        unsigned int getNbCodewords() const { return m_nbCodewords; }
        bool getHasCRC() const { return m_hasCRC; }
        int getHeaderParityStatus() const { return m_headerParityStatus; }
        bool getHeaderCRCStatus() const { return m_headerCRCStatus; }
        int getPayloadParityStatus() const { return m_payloadParityStatus; }
        bool getPayloadCRCStatus() const { return m_payloadCRCStatus; }

        static MsgReportDecodeBytes* create(const QByteArray& bytes) {
            return new MsgReportDecodeBytes(bytes);
        }
        void setSyncWord(unsigned int syncWord) {
            m_syncWord = syncWord;
        }
        void setSignalDb(float db) {
            m_signalDb = db;
        }
        void setNoiseDb(float db) {
            m_noiseDb = db;
        }
        void setPacketSize(unsigned int packetSize) {
            m_packetSize = packetSize;
        }
        void setNbParityBits(unsigned int nbParityBits) {
            m_nbParityBits = nbParityBits;
        }
        void setNbSymbols(unsigned int nbSymbols) {
            m_nbSymbols = nbSymbols;
        }
        void setNbCodewords(unsigned int nbCodewords) {
            m_nbCodewords = nbCodewords;
        }
        void setHasCRC(bool hasCRC) {
            m_hasCRC = hasCRC;
        }
        void setHeaderParityStatus(int headerParityStatus) {
            m_headerParityStatus = headerParityStatus;
        }
        void setHeaderCRCStatus(bool headerCRCStatus) {
            m_headerCRCStatus = headerCRCStatus;
        }
        void setPayloadParityStatus(int payloadParityStatus) {
            m_payloadParityStatus = payloadParityStatus;
        }
        void setPayloadCRCStatus(bool payloadCRCStatus) {
            m_payloadCRCStatus = payloadCRCStatus;
        }

    private:
        QByteArray m_bytes;
        unsigned int m_syncWord;
        float m_signalDb;
        float m_noiseDb;
        unsigned int m_packetSize;
        unsigned int m_nbParityBits;
        unsigned int m_nbSymbols;
        unsigned int m_nbCodewords;
        bool m_hasCRC;
        int m_headerParityStatus;
        bool m_headerCRCStatus;
        int m_payloadParityStatus;
        bool m_payloadCRCStatus;

        MsgReportDecodeBytes(const QByteArray& bytes) :
            Message(),
            m_bytes(bytes),
            m_syncWord(0),
            m_signalDb(0.0),
            m_noiseDb(0.0),
            m_packetSize(0),
            m_nbParityBits(0),
            m_nbSymbols(0),
            m_nbCodewords(0),
            m_hasCRC(false),
            m_headerParityStatus(false),
            m_headerCRCStatus(false),
            m_payloadParityStatus(false),
            m_payloadCRCStatus(false)
        { }
    };

    class MsgReportDecodeString : public Message {
        MESSAGE_CLASS_DECLARATION

    public:
        const QString& getString() const { return m_str; }
        unsigned int getSyncWord() const { return m_syncWord; }
        float getSingalDb() const { return m_signalDb; }
        float getNoiseDb() const { return m_noiseDb; }

        static MsgReportDecodeString* create(const QString& str)
        {
            return new MsgReportDecodeString(str);
        }
        void setSyncWord(unsigned int syncWord) {
            m_syncWord = syncWord;
        }
        void setSignalDb(float db) {
            m_signalDb = db;
        }
        void setNoiseDb(float db) {
            m_noiseDb = db;
        }

    private:
        QString m_str;
        unsigned int m_syncWord;
        float m_signalDb;
        float m_noiseDb;

        MsgReportDecodeString(const QString& str) :
            Message(),
            m_str(str),
            m_syncWord(0),
            m_signalDb(0.0),
            m_noiseDb(0.0)
        { }
    };

	LoRaDemod(DeviceAPI* deviceAPI);
	virtual ~LoRaDemod();
	virtual void destroy() { delete this; }
	void setSpectrumSink(BasebandSampleSink* sampleSink) { m_basebandSink->setSpectrumSink(sampleSink); }

	virtual void feed(const SampleVector::const_iterator& begin, const SampleVector::const_iterator& end, bool pO);
	virtual void start();
	virtual void stop();
	virtual bool handleMessage(const Message& cmd);

    virtual void getIdentifier(QString& id) { id = objectName(); }
    virtual void getTitle(QString& title) { title = m_settings.m_title; }
    virtual qint64 getCenterFrequency() const { return 0; }

    virtual QByteArray serialize() const;
    virtual bool deserialize(const QByteArray& data);

    virtual int getNbSinkStreams() const { return 1; }
    virtual int getNbSourceStreams() const { return 0; }

    virtual qint64 getStreamCenterFrequency(int streamIndex, bool sinkElseSource) const
    {
        (void) streamIndex;
        (void) sinkElseSource;
        return 0;
    }

    bool getDemodActive() const;

    static const QString m_channelIdURI;
    static const QString m_channelId;

private:
	DeviceAPI *m_deviceAPI;
    QThread *m_thread;
    LoRaDemodBaseband* m_basebandSink;
    LoRaDemodDecoder m_decoder;
    LoRaDemodSettings m_settings;
    int m_basebandSampleRate; //!< stored from device message used when starting baseband sink

    void applySettings(const LoRaDemodSettings& settings, bool force = false);
};

#endif // INCLUDE_LORADEMOD_H
