/**
 * Copyright 2017 Colin Doig.  Distributed under the MIT license.
 */
#ifndef MARKET_HANDICAPPANEL_H
#define MARKET_HANDICAPPANEL_H

#include <map>
#include <set>

#include <wx/wx.h>
#include <wx/panel.h>

namespace greenthumb {
namespace market {

wxDECLARE_EVENT(HANDICAP_CHANGED, wxCommandEvent);

/**
 * Allow the user to select the handicap and display the current handicap.  Used in asian handicap markets.
 */
class HandicapPanel : public wxPanel {

    public:

        /**
         * Constructor.
         *
         * @param parent The parent window.
         * @param id An identifier for the panel.
         * @param pos The panel position.
         * @param size The panel size.
         * @param style The window style.
         * @param name Window name.
         */
        HandicapPanel(wxPanel* parent, const wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxDefaultSize, long style = wxBORDER_NONE, const wxString& name = wxPanelNameStr);

        /**
         * Adds a handicap.
         *
         * @param selectionId The runner's id.
         * @param handicap The handicap to apply to the runner.
         */
        void AddHandicap(const int64_t selectionId, const double handicap);

        /**
         * Gets the current handicap for the given runner.
         *
         * @param selectionId The runner's id.
         */
        const double GetHandicap(int64_t selectionId) const;

    private:
        /** Handicaps are converted to integers for storage in a std::set. */
        const static unsigned SCALE_FACTOR = 100;
        /** The selection that the handicaps are applicable to. */
        int64_t selectionId = 0;
        /** The set of allowed handicaps. */
        std::set<int64_t> runnerHandicaps;
        /** The current handicap. */
        std::set<int64_t>::iterator currentHandicap;
        /** Displays the current handicap. */
        wxStaticText* handicapText;

        /**
         * Change the current handicap to the previous value (if there is one).
         *
         * @param event The button event.
         */
        void OnClickPrevious(const wxCommandEvent& event);

        /**
         * Change the current handicap to the next value (if there is one).
         *
         * @param event The button event.
         */
        void OnClickNext(wxCommandEvent& event);

    DECLARE_NO_COPY_CLASS(HandicapPanel)

};

}
}

#endif // MARKET_HANDICAPPANEL_H