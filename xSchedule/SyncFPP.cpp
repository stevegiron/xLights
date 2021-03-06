#include "SyncFPP.h"
#include "ScheduleOptions.h"
#include "events/ListenerManager.h"

#include <log4cpp/Category.hh>
#include "../xLights/UtilFunctions.h"
#include <wx/filename.h>
#include "Control.h"
#include "../xLights/outputs/IPOutput.h"

#define FPP_MEDIA_SYNC_INTERVAL_MS 500
#define FPP_SEQ_SYNC_INTERVAL_FRAMES 16
#define FPP_SEQ_SYNC_INTERVAL_INITIAL_FRAMES 4
#define FPP_SEQ_SYNC_INITIAL_NUMBER_OF_FRAMES 32

void SyncBroadcastFPP::SendSync(uint32_t frameMS, uint32_t stepLengthMS, uint32_t stepMS, uint32_t playlistMS, const std::string& fseq, const std::string& media, const std::string& step, const std::string& timeItem) const
{
    static std::string lastfseq = "";
    static std::string lastmedia = "";
    static size_t lastfseqmsec = 0;
    static size_t lastmediamsec = 0;

    if (fseq == "")
    {
        if (lastfseq != "")
        {
            SendFPPSync(lastfseq, 0xFFFFFFFF, 50);
        }

        if (lastmedia != "")
        {
            SendFPPSync(lastmedia, 0xFFFFFFFF, 50);
        }

        return;
    }

    bool dosendFSEQ = false;
    bool dosendMedia = false;

    if (stepMS == 0 || stepMS == 0xFFFFFFFF) {
        dosendFSEQ = true;
        dosendMedia = true;
    }

    wxFileName fn(fseq.c_str());
    if (fn.GetExt().Lower() == "fseq")
    {
        if (lastfseq != fseq)
        {
            if (lastfseq != "")
            {
                SendFPPSync(lastfseq, 0xFFFFFFFF, frameMS);
            }

            lastfseq = fseq;

            if (stepMS != 0)
            {
                SendFPPSync(fseq, 0, frameMS);
            }
        }

        if (!dosendFSEQ)
        {
            if (stepMS <= FPP_SEQ_SYNC_INITIAL_NUMBER_OF_FRAMES * frameMS)
            {
                // we are in the initial period
                if (stepMS - lastfseqmsec >= FPP_SEQ_SYNC_INTERVAL_INITIAL_FRAMES * frameMS)
                {
                    dosendFSEQ = true;
                }
            }
            else
            {
                if (stepMS - lastfseqmsec >= FPP_SEQ_SYNC_INTERVAL_FRAMES * frameMS)
                {
                    dosendFSEQ = true;
                }
            }
        }
    }

    if (media != "")
    {
        if (lastmedia != media)
        {
            if (lastmedia != "")
            {
                SendFPPSync(lastmedia, 0xFFFFFFFF, frameMS);
            }

            lastmedia = media;

            if (stepMS != 0)
            {
                SendFPPSync(media, 0, frameMS);
            }
        }

        if (!dosendMedia)
        {
            if (stepMS - lastmediamsec >= FPP_MEDIA_SYNC_INTERVAL_MS)
            {
                dosendMedia = true;
            }
        }
    }

    if (dosendFSEQ) {
        SendFPPSync(fseq, stepMS, frameMS);
        lastfseqmsec = stepMS;
    }
    if (dosendMedia) {
        SendFPPSync(media, stepMS, frameMS);
        lastmediamsec = stepMS;
    }

    if (stepMS == 0xFFFFFFFF)
    {
        lastfseq = "";
        lastfseqmsec = 0;
        lastmedia = "";
        lastmediamsec = 0;
    }

}

void SyncUnicastFPP::SendSync(uint32_t frameMS, uint32_t stepLengthMS, uint32_t stepMS, uint32_t playlistMS, const std::string& fseq, const std::string& media, const std::string& step, const std::string& timeItem) const
{
    static std::string lastfseq = "";
    static std::string lastmedia = "";
    static size_t lastfseqmsec = 0;
    static size_t lastmediamsec = 0;

    if (fseq == "")
    {
        if (lastfseq != "")
        {
            SendFPPSync(lastfseq, 0xFFFFFFFF, 50);
        }

        if (lastmedia != "")
        {
            SendFPPSync(lastmedia, 0xFFFFFFFF, 50);
        }

        return;
    }

    bool dosendFSEQ = false;
    bool dosendMedia = false;

    if (stepMS == 0 || stepMS == 0xFFFFFFFF) {
        dosendFSEQ = true;
        dosendMedia = true;
    }

    wxFileName fn(fseq.c_str());
    if (fn.GetExt().Lower() == "fseq")
    {
        if (lastfseq != fseq)
        {
            if (lastfseq != "")
            {
                SendFPPSync(lastfseq, 0xFFFFFFFF, frameMS);
            }

            lastfseq = fseq;

            if (stepMS != 0)
            {
                SendFPPSync(fseq, 0, frameMS);
            }
        }

        if (!dosendFSEQ)
        {
            if (stepMS <= FPP_SEQ_SYNC_INITIAL_NUMBER_OF_FRAMES * frameMS)
            {
                // we are in the initial period
                if (stepMS - lastfseqmsec >= FPP_SEQ_SYNC_INTERVAL_INITIAL_FRAMES * frameMS)
                {
                    dosendFSEQ = true;
                }
            }
            else
            {
                if (stepMS - lastfseqmsec >= FPP_SEQ_SYNC_INTERVAL_FRAMES * frameMS)
                {
                    dosendFSEQ = true;
                }
            }
        }
    }

    if (media != "")
    {
        if (lastmedia != media)
        {
            if (lastmedia != "")
            {
                SendFPPSync(lastmedia, 0xFFFFFFFF, frameMS);
            }

            lastmedia = media;

            if (stepMS != 0)
            {
                SendFPPSync(media, 0, frameMS);
            }
        }

        if (!dosendMedia)
        {
            if (stepMS - lastmediamsec >= FPP_MEDIA_SYNC_INTERVAL_MS)
            {
                dosendMedia = true;
            }
        }
    }

    if (dosendFSEQ) {
        SendFPPSync(fseq, stepMS, frameMS);
        lastfseqmsec = stepMS;
    }
    if (dosendMedia) {
        SendFPPSync(media, stepMS, frameMS);
        lastmediamsec = stepMS;
    }

    if (stepMS == 0xFFFFFFFF)
    {
        lastfseq = "";
        lastfseqmsec = 0;
        lastmedia = "";
        lastmediamsec = 0;
    }
}

void SyncBroadcastFPP::SendStop() const
{
    SendSync(50, 0, 0xFFFFFFFF, 0, "", "", "", "");
}

void SyncUnicastFPP::SendStop() const
{
    SendSync(50, 0, 0xFFFFFFFF, 0, "", "", "", "");
}

void SyncBroadcastFPP::SendFPPSync(const std::string& item, uint32_t stepMS, uint32_t frameMS) const
{
    int bufsize = sizeof(ControlPkt) + sizeof(SyncPkt) + item.size();

    std::vector<uint8_t> buffer(bufsize);

    ControlPkt* cp = reinterpret_cast<ControlPkt*>(&buffer[0]);
    strncpy(cp->fppd, "FPPD", 4);
    cp->pktType = CTRL_PKT_SYNC;
    cp->extraDataLen = bufsize - sizeof(ControlPkt);

    SyncPkt* sp = reinterpret_cast<SyncPkt*>(&buffer[0] + sizeof(ControlPkt));

    if (stepMS == 0)
    {
        sp->pktType = SYNC_PKT_START;
    }
    else if (stepMS == 0xFFFFFFFF)
    {
        sp->pktType = SYNC_PKT_STOP;
    }
    else
    {
        sp->pktType = SYNC_PKT_SYNC;
    }

    wxFileName fn(item);
    if (fn.GetExt().Lower() == "fseq")
    {
        sp->fileType = SYNC_FILE_SEQ;
        sp->frameNumber = stepMS / frameMS;
    }
    else
    {
        sp->fileType = SYNC_FILE_MEDIA;
        sp->frameNumber = 0;
    }

    if (sp->pktType == SYNC_PKT_SYNC)
    {
        sp->secondsElapsed = stepMS / 1000.0;
    }
    else
    {
        sp->frameNumber = 0;
        sp->secondsElapsed = 0;
    }

    strcpy(&sp->filename[0], fn.GetFullName().c_str());

    if (_fppBroadcastSocket != nullptr)
    {
        _fppBroadcastSocket->SendTo(_remoteAddr, &buffer[0], bufsize);
    }
}

void SyncUnicastFPP::SendFPPSync(const std::string& item, uint32_t stepMS, uint32_t frameMS) const
{
    uint8_t pktType;
    if (stepMS == 0)
    {
        pktType = SYNC_PKT_START;
    }
    else if (stepMS == 0xFFFFFFFF)
    {
        pktType = SYNC_PKT_STOP;
    }
    else
    {
        pktType = SYNC_PKT_SYNC;
    }

    uint8_t fileType;
    wxFileName fn(item);
    if (fn.GetExt().Lower() == "fseq")
    {
        fileType = SYNC_FILE_SEQ;
    }
    else
    {
        fileType = SYNC_FILE_MEDIA;
    }

    if (fileType == SYNC_FILE_SEQ)
    {
        for (auto it : _remotes)
        {
            SendUnicastSync(it, fn.GetFullName().ToStdString(), stepMS, frameMS, pktType);
        }
    }
}

SyncBroadcastFPP::SyncBroadcastFPP(SyncBroadcastFPP&& from) : SyncBase(from)
{
    _fppBroadcastSocket = from._fppBroadcastSocket;
    from._fppBroadcastSocket = nullptr; // this is a transfer of ownership
    _remoteAddr = from._remoteAddr;
}

SyncUnicastFPP::SyncUnicastFPP(SyncUnicastFPP&& from) : SyncBase(from)
{
    _fppUnicastSocket = from._fppUnicastSocket;
    from._fppUnicastSocket = nullptr; // this is a transfer of ownership
    _remotes = from._remotes;
}

SyncBroadcastFPP::~SyncBroadcastFPP()
{
    if (_fppBroadcastSocket != nullptr) {
        _fppBroadcastSocket->Close();
        delete _fppBroadcastSocket;
        _fppBroadcastSocket = nullptr;
    }
}

SyncUnicastFPP::~SyncUnicastFPP()
{
    if (_fppUnicastSocket != nullptr) {
        _fppUnicastSocket->Close();
        delete _fppUnicastSocket;
        _fppUnicastSocket = nullptr;
    }
}

SyncBroadcastFPP::SyncBroadcastFPP(SYNCMODE sm, REMOTEMODE rm, const ScheduleOptions& options, ListenerManager* listenerManager) : SyncBase(sm, rm)
{
    static log4cpp::Category &logger_base = log4cpp::Category::getInstance(std::string("log_base"));

    if (sm == SYNCMODE::FPPBROADCASTMASTER)
    {
        _remoteAddr.Hostname("255.255.255.255");
        _remoteAddr.Service(FPP_CTRL_PORT);

        wxIPV4address localaddr;
        if (IPOutput::GetLocalIP() == "")
        {
            localaddr.AnyAddress();
        }
        else
        {
            localaddr.Hostname(IPOutput::GetLocalIP());
        }

        _fppBroadcastSocket = new wxDatagramSocket(localaddr, wxSOCKET_NOWAIT | wxSOCKET_BROADCAST);
        if (_fppBroadcastSocket == nullptr)
        {
            logger_base.error("Error opening datagram for FPP Sync as master. %s", (const char *)localaddr.IPAddress().c_str());
        }
        else if (!_fppBroadcastSocket->IsOk())
        {
            logger_base.error("Error opening datagram for FPP Sync as master. %s OK : FALSE", (const char *)localaddr.IPAddress().c_str());
            delete _fppBroadcastSocket;
            _fppBroadcastSocket = nullptr;
        }
        else if (_fppBroadcastSocket->Error())
        {
            logger_base.error("Error opening datagram for FPP Sync as master. %d : %s", _fppBroadcastSocket->LastError(), (const char*)DecodeIPError(_fppBroadcastSocket->LastError()).c_str());
            delete _fppBroadcastSocket;
            _fppBroadcastSocket = nullptr;
        }
        else
        {
            logger_base.info("FPP Sync as master datagram opened successfully.");
        }
    }

    if (rm == REMOTEMODE::FPPBROADCASTSLAVE)
    {
        listenerManager->SetRemoteFPP();
    }
}

SyncUnicastFPP::SyncUnicastFPP(SYNCMODE sm, REMOTEMODE rm, const ScheduleOptions& options, ListenerManager* listenerManager) : SyncBase(sm, rm)
{
    static log4cpp::Category &logger_base = log4cpp::Category::getInstance(std::string("log_base"));

    if (sm == SYNCMODE::FPPUNICASTMASTER)
    {
        wxIPV4address localaddr;
        if (IPOutput::GetLocalIP() == "")
        {
            localaddr.AnyAddress();
        }
        else
        {
            localaddr.Hostname(IPOutput::GetLocalIP());
        }

        _remotes = options.GetFPPRemotes();
        if (_remotes.size() > 0)
        {
            _fppUnicastSocket = new wxDatagramSocket(localaddr, wxSOCKET_NOWAIT);
            if (_fppUnicastSocket == nullptr)
            {
                logger_base.error("Error opening unicast datagram for FPP Sync as master %s.", (const char *)localaddr.IPAddress().c_str());
            }
            else if (!_fppUnicastSocket->IsOk())
            {
                logger_base.error("Error opening unicast datagram for FPP Sync as master %s. OK : FALSE", (const char *)localaddr.IPAddress().c_str());
                delete _fppUnicastSocket;
                _fppUnicastSocket = nullptr;
            }
            else if (_fppUnicastSocket->Error())
            {
                logger_base.error("Error opening unicast datagram for FPP Sync as master. %d : %s %s", _fppUnicastSocket->LastError(), (const char*)DecodeIPError(_fppUnicastSocket->LastError()).c_str(), (const char *)localaddr.IPAddress().c_str());
                delete _fppUnicastSocket;
                _fppUnicastSocket = nullptr;
            }
            else
            {
                logger_base.info("FPP Sync as master unicast datagram opened successfully.");
            }
        }
    }

    if (rm == REMOTEMODE::FPPUNICASTSLAVE)
    {
        listenerManager->SetRemoteFPPUnicast();
    }
}

void SyncUnicastFPP::SendUnicastSync(const std::string& ip, const std::string& syncItem, size_t msec, size_t frameMS, int action) const
{
    static log4cpp::Category &logger_base = log4cpp::Category::getInstance(std::string("log_base"));
    wxIPV4address remoteAddr;
    remoteAddr.Hostname(ip);
    remoteAddr.Service(FPP_CTRL_CSV_PORT);

    std::string buffer;

    switch (action)
    {
    case SYNC_PKT_SYNC:
        buffer = wxString::Format("FPP,%d,%d,%d,%s,%d,%d\n", CTRL_PKT_SYNC, SYNC_FILE_SEQ, action, syncItem.c_str(), static_cast<int>(msec / 1000), static_cast<int>(msec) % 1000).ToStdString();
        //logger_base.debug("Sending remote sync unicast packet to %s.", (const char*)ip.c_str());
        break;
    case SYNC_PKT_STOP:
        buffer = wxString::Format("FPP,%d,%d,%d,%s\n", CTRL_PKT_SYNC, SYNC_FILE_SEQ, action, syncItem.c_str()).ToStdString();
        logger_base.debug("Sending remote stop unicast packet to %s : %s.", (const char*)ip.c_str(), (const char*)buffer.c_str());
        break;
    case SYNC_PKT_START:
        buffer = wxString::Format("FPP,%d,%d,%d,%s\n", CTRL_PKT_SYNC, SYNC_FILE_SEQ, action, syncItem.c_str()).ToStdString();
        logger_base.debug("Sending remote start unicast packet to %s : %s.", (const char*)ip.c_str(), (const char*)buffer.c_str());
        break;
    case CTRL_PKT_BLANK:
        buffer = wxString::Format("FPP,%d\n", CTRL_PKT_BLANK).ToStdString();
        break;
    default:
        break;
    }

    if (_fppUnicastSocket != nullptr)
    {
        _fppUnicastSocket->SendTo(remoteAddr, static_cast<const char*>(buffer.c_str()), buffer.size());
    }
}
