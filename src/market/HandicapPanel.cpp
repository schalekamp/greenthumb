/**
 * Copyright 2018 Colin Doig.  Distributed under the MIT license.
 */
#include <wx/wx.h>
#include <wx/sizer.h>
#include <wx/stattext.h>

#include "market/HandicapPanel.h"
#include "ArtProvider.h"
#include "Util.h"

namespace greenthumb {
namespace market {

wxDEFINE_EVENT(HANDICAP_CHANGED, wxCommandEvent);

HandicapPanel::HandicapPanel(wxPanel* parent, const wxWindowID id, const wxPoint& pos, const wxSize& size, long style,
    const wxString& name) : wxPanel(parent, id, pos, size, style, name) {

    wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
    SetSizer(sizer);

    int borderWidth = 10;
    int borderFlags = wxTOP | wxRIGHT | wxBOTTOM;

    wxSize iconSize = wxSize(36, 36);
    wxWindowID prevButtonId = wxWindow::NewControlId();
    wxButton* prevButton = new wxButton(this, prevButtonId, wxEmptyString, wxDefaultPosition, iconSize, wxBORDER_NONE);
    wxBitmap previousBitmap = ArtProvider::GetBitmap(ArtProvider::IconId::GO_PREVIOUS);
    prevButton->SetBitmap(previousBitmap);
    sizer->Add(prevButton);

    handicapText = new wxStaticText(this, wxID_ANY, "");
    sizer->Add(handicapText, 0, borderFlags, borderWidth);

    wxWindowID nextButtonId = wxWindow::NewControlId();
    wxButton* nextButton = new wxButton(this, nextButtonId, wxEmptyString, wxDefaultPosition, iconSize, wxBORDER_NONE);
    wxBitmap nextBitmap = ArtProvider::GetBitmap(ArtProvider::IconId::GO_NEXT);
    nextButton->SetBitmap(nextBitmap);
    sizer->Add(nextButton);

    Show(false);

    Bind(wxEVT_BUTTON, &HandicapPanel::OnClickPrevious, this, prevButtonId);
    Bind(wxEVT_BUTTON, &HandicapPanel::OnClickNext, this, nextButtonId);
}

void HandicapPanel::AddPage(const std::vector<std::pair<int64_t, double>>& page) {
    handicapPages.push_back(page);
    currentHandicap = handicapPages.begin();
    if (handicapPages.size() == 1) {
        if (page.size() > 0) {
            handicapText->SetLabel(DoubleToString(page.at(0).second, 1));
        }
        wxCommandEvent* handicapEvent = new wxCommandEvent(HANDICAP_CHANGED);
        QueueEvent(handicapEvent);
    }
}

void HandicapPanel::OnClickPrevious(const wxCommandEvent& event) {
    if (currentHandicap != handicapPages.begin()) {
        --currentHandicap;
        if (currentHandicap->size() > 0) {
            handicapText->SetLabel(DoubleToString(currentHandicap->at(0).second, 1));
            wxCommandEvent* handicapEvent = new wxCommandEvent(HANDICAP_CHANGED);
            QueueEvent(handicapEvent);
        }
    }
}

void HandicapPanel::OnClickNext(wxCommandEvent& event) {
    ++currentHandicap;
    if (currentHandicap != handicapPages.end()) {
        handicapText->SetLabel(DoubleToString(currentHandicap->at(0).second, 1));
        wxCommandEvent* handicapEvent = new wxCommandEvent(HANDICAP_CHANGED);
        QueueEvent(handicapEvent);
    } else {
        // backtrack, we don't want to past the end of the set.
        --currentHandicap;
    }
}

const double HandicapPanel::GetHandicap(int64_t selectionId) const {
    std::vector<std::pair<int64_t, double>> page = *currentHandicap;

    for (auto it = page.begin(); it != page.end(); ++it) {
        if (it->first == selectionId) {
            return it->second;
        }
    }
    return 0;
}

}
}
