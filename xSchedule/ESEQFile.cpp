#include "ESEQFile.h"
#include <wx/file.h>
#include <wx/filename.h>

ESEQFile::ESEQFile()
{
    _channelsPerFrame = 0;
    _filename = "";
    _frames = 0;
    _fh = nullptr;
    _frameBuffer = nullptr;
    _frame0Offset = 0;
    _currentFrame = 0;
    _ok = false;
}

ESEQFile::ESEQFile(const std::string& filename)
{
    _channelsPerFrame = 0;
    _filename = filename;
    _frames = 0;
    _fh = nullptr;
    _frameBuffer = nullptr;
    _frame0Offset = 0;
    _currentFrame = 0;
    _ok = true;
    Load(filename);
}

ESEQFile::~ESEQFile()
{
    Close();
}


void ESEQFile::Close()
{
    if (_fh != nullptr)
    {
        _fh->Close();
        delete _fh;
        _fh = nullptr;
    }

    if (_frameBuffer != nullptr)
    {
        free(_frameBuffer);
        _frameBuffer = nullptr;
    }

    _ok = false;
}

int ESEQFile::ReadInt16(wxFile* fh)
{
    wxByte lsb;
    fh->Read(&lsb, sizeof(lsb));
    wxByte msb;
    fh->Read(&msb, sizeof(msb));
    return (((int)msb) << 8) + lsb;
}

int ESEQFile::ReadInt32(wxFile* fh)
{
    wxByte lsb;
    fh->Read(&lsb, sizeof(lsb));
    wxByte lsb1;
    fh->Read(&lsb1, sizeof(lsb1));
    wxByte lsb2;
    fh->Read(&lsb2, sizeof(lsb2));
    wxByte msb;
    fh->Read(&msb, sizeof(msb));
    return (((int)msb) << 24) + (((int)lsb2) << 16) + (((int)lsb1) << 8) + lsb;
}

std::list<std::string> ESEQFile::GetBlendModes()
{
    std::list<std::string> res;

    res.push_back("Overwrite");
    res.push_back("Overwrite if black");
    res.push_back("Mask out if not black");
    res.push_back("Mask out if black");
    res.push_back("Average");
    res.push_back("Maximum");

    return res;
}

void ESEQFile::Load(const std::string& filename)
{
    Close();

    _fh = new wxFile(filename);

    if (_fh->IsOpened())
    {
        char tag[5];
        memset(tag, 0x00, sizeof(tag));
        _fh->Read(tag, sizeof(tag)-1);
        if (std::string(tag) == "ESEQ")
        {
            _frame0Offset = 20;
            ReadInt32(_fh); // models count ... should be 1
            _channelsPerFrame = ReadInt32(_fh);
            _offset = ReadInt32(_fh);
            ReadInt32(_fh); // model size
            //_frames = ReadInt32(_fh);
            _currentFrame = 0;
            _frameBuffer = (wxByte*)malloc(_channelsPerFrame);

            wxFileName fn(filename);
            _frames = (size_t)(fn.GetSize().ToULong() - _frame0Offset) / _channelsPerFrame;
        }
        else
        {
            Close();
        }
    }
    else
    {
        Close();
    }
}

void ESEQFile::ReadData(wxByte* buffer, size_t buffersize, size_t frame, APPLYMETHOD applyMethod)
{
    if (frame >= _frames) return; // cant read past end of file

    if (frame != _currentFrame)
    {
        // we need to seek to our frame
        _fh->Seek(_frame0Offset + _channelsPerFrame * frame);
    }

    // read in the frame from disk
    _fh->Read(_frameBuffer, _channelsPerFrame);
    _currentFrame = frame + 1;

    size_t bytesToUse = std::min(buffersize - _offset, _channelsPerFrame);

    switch(applyMethod)
    {
    case APPLYMETHOD::METHOD_OVERWRITE:
        memcpy(buffer + _offset, _frameBuffer, bytesToUse);
        break;
    case APPLYMETHOD::METHOD_AVERAGE:
        for (size_t i = 0; i < bytesToUse; i++)
        {
            *(buffer + i) = ((int)*(buffer + _offset + i) + (int)*(_frameBuffer + i)) / 2;
        }
        break;
    case APPLYMETHOD::METHOD_MASK:
        for (size_t i = 0; i < bytesToUse; i++)
        {
            if (*(_frameBuffer + i) > 0)
            {
                *(buffer + _offset + i) = 0x00;
            }
        }
        break;
    case APPLYMETHOD::METHOD_UNMASK:
        for (size_t i = 0; i < bytesToUse; i++)
        {
            if (*(_frameBuffer + i) == 0)
            {
                *(buffer + _offset + i) = 0x00;
            }
        }
        break;
    case APPLYMETHOD::METHOD_MAX:
        for (size_t i = 0; i < bytesToUse; i++)
        {
            *(buffer + _offset + i) = std::max(*(buffer + _offset + i), *(_frameBuffer + i));
        }
        break;
    case APPLYMETHOD::METHOD_OVERWRITEIFBLACK:
        for (size_t i = 0; i < bytesToUse; i++)
        {
            if (*(buffer + _offset + i) == 0)
            {
                *(buffer + _offset + i) = *(_frameBuffer + i);
            }
        }
    }
}
