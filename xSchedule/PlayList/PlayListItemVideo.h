#ifndef PLAYLISTITEMVIDEO_H
#define PLAYLISTITEMVIDEO_H

#include "PlayListItem.h"
#include <string>

class wxXmlNode;
class wxWindow;
class VideoReader;

class PlayListItemVideo : public PlayListItem
{
protected:

    #pragma region Member Variables
    std::string _videoFile;
	wxPoint _origin;
	wxSize _size;
    VideoReader* _videoReader;
    size_t _durationMS;
    #pragma endregion Member Variables

    void OpenFiles();
    void CloseFiles();

public:

    #pragma region Constructors and Destructors
    PlayListItemVideo(wxXmlNode* node);
    PlayListItemVideo();
    virtual ~PlayListItemVideo() {};
    virtual PlayListItem* Copy() const override;
    #pragma endregion Constructors and Destructors

    #pragma region Getters and Setters
    virtual size_t GetDurationMS() const override;
    virtual std::string GetName() const override;
    void SetLocation(wxPoint pt, wxSize size) { _origin = pt; _size = size; _dirty = true; }
    void SetVideoFile(const std::string& videoFile);
    std::string GetVideoFile() const { return _videoFile; }
    wxPoint GetPosition() const { return _origin; }
    wxSize GetSize() const { return _size; }
    #pragma endregion Getters and Setters

    virtual wxXmlNode* Save() override;
    void Load(wxXmlNode* node) override;

    #pragma region Playing
    virtual void Start() override;
    virtual void Stop() override;
    virtual void Frame(wxByte* buffer, size_t size, size_t ms, size_t framems) override;
    #pragma endregion Playing

#pragma region UI
    // returns nullptr if cancelled
    virtual void Configure(wxNotebook* notebook) override;
#pragma endregion UI
};
#endif