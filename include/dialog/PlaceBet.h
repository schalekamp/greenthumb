/**
 * Copyright 2017 Colin Doig.  Distributed under the MIT license.
 */
#ifndef DIALOG_PLACEBET_H
#define DIALOG_PLACEBET_H

#include <greentop/ExchangeApi.h>

#include <wx/button.h>
#include <wx/dialog.h>
#include <wx/radiobut.h>
#include <wx/textctrl.h>
#include <wx/stattext.h>

#include "entity/Market.h"
#include "worker/WorkerManager.h"

#include "OddsSpinCtrl.h"

namespace greenthumb {
namespace dialog {

wxDECLARE_EVENT(PLACE_ORDER_PENDING, wxCommandEvent);

/**
 * Place a bet.
 */
class PlaceBet : public wxDialog {
    public:

        /**
         * Constructor.
         *
         * @param parent Pointer to the parent window.
         * @param id The identifier of the dialog.
         * @param title The title of the dialog.
         * @param pos The dialog position.
         * @param size The dialog size.
         * @param style The window style.
         * @param name Used to associate a name with the window, allowing the application
         *               user to set Motif resource values for individual dialog boxes.
         */
        PlaceBet(wxWindow* parent, wxWindowID id, const wxString& title,
            const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize,
            long style = wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER, const wxString& name = wxDialogNameStr);

        /**
         * Sets the runner, price and back / lay.
         *
         * @param runnerName The runner name.
         * @param placeInstruction The place instruction.
         */
        void SetPlaceInstruction(const std::string& runnerName, const greentop::PlaceInstruction& pi);

        /**
         * Sets the market to place a bet on.
         *
         * @param market The market.
         * @param fullMarketName The event name and market name.
         */
        void SetMarket(const entity::Market& market, const std::string& fullMarketName);

    protected:
    private:

        OddsSpinCtrl* oddsSpin;
        wxTextCtrl* stakeSpin;
        wxStaticText* profit;
        wxStaticText* liability;
        greentop::Side side;
        wxWindow* backLayButton;
        wxWindow* cancelButton;
        wxStaticText* selection;
        wxStaticText* marketLabel;
        wxRadioButton* persistKeep;
        wxRadioButton* persistCancel;
        entity::Market market;
        greentop::PlaceInstruction pendingPlaceInstruction;
        uint64_t selectionId;
        worker::WorkerManager workerManager;
        greentop::Optional<double> handicap;

        void UpdateProfitAndLiability();
        void OnTextChange(wxCommandEvent& event);
        void OnSpinChange(const wxEvent& spinEvent);
        void OnSubmit(const wxCommandEvent& event);
        void OnCancel(wxCommandEvent& event);
        void PlaceOrderPending(const double stake, const double odds);
        void OnPlaceBet(wxThreadEvent& event);

};

}
}

#endif // DIALOG_PLACEBET_H
