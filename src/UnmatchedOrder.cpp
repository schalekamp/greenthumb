/**
 * Copyright 2016 Colin Doig.  Distributed under the MIT license.
 */
#include <iomanip>

#include "ArtProvider.h"
#include "UnmatchedOrder.h"
#include "Util.h"

#include "entity/Config.h"

#include "worker/CancelOrders.h"
#include "worker/ReplaceOrders.h"

namespace greenthumb {

UnmatchedOrder::UnmatchedOrder(wxWindow* parent, const wxWindowID id, const wxPoint& pos, const wxSize& size,
    long style, const wxString& name) : CurrentOrder(parent, id, pos, size, style, name), workerManager(this) {

    wxFlexGridSizer* sizer = new wxFlexGridSizer(8, 5, 5);
    sizer->AddGrowableCol(0, 1);
    SetSizer(sizer);

    runnerName = new wxStaticText(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize);
    runnerName->SetMinSize(wxSize(150, -1));

    sizer->Add(runnerName, 0, wxALIGN_CENTRE_VERTICAL);

    oddsSpin = new OddsSpinCtrl(this, wxID_ANY);
    sizer->Add(oddsSpin, 0, wxALIGN_CENTRE_VERTICAL);

    wxStaticText* stakeLabel = new wxStaticText(this, wxID_ANY, "Stake: ");
    sizer->Add(stakeLabel, 0, wxALIGN_CENTRE_VERTICAL);
    stake = new wxStaticText(this, wxID_ANY, wxEmptyString);
    sizer->Add(stake, 0, wxALIGN_CENTRE_VERTICAL);

    profitOrLiabilityLabel = new wxStaticText(this, wxID_ANY, wxEmptyString);
    profitOrLiabilityLabel->SetMinSize(wxSize(75, -1));
    sizer->Add(profitOrLiabilityLabel, 0, wxALIGN_CENTRE_VERTICAL);
    profitOrLiability = new wxStaticText(this, wxID_ANY, wxEmptyString);
    sizer->Add(profitOrLiability, 0, wxALIGN_CENTRE_VERTICAL);

    wxWindowID submitButtonId = wxWindow::NewControlId();
    submitButton = new wxButton(this, submitButtonId, wxEmptyString, wxDefaultPosition,
        wxSize(30, -1), wxBORDER_NONE);
    submitButton->SetBitmap(ArtProvider::GetBitmap(ArtProvider::IconId::SERVER_GO));
    submitButton->Disable();

    sizer->Add(submitButton, 0, wxALIGN_CENTRE_VERTICAL);

    wxWindowID cancelId = wxWindow::NewControlId();

    wxButton* cancelButton = new wxButton(this, cancelId, wxEmptyString, wxDefaultPosition,
        wxSize(30, -1), wxBORDER_NONE);
    cancelButton->SetBitmap(ArtProvider::GetBitmap(ArtProvider::IconId::CLOSE));

    sizer->Add(cancelButton, 0, wxALIGN_CENTRE_VERTICAL);
    sizer->Layout();

    Bind(wxEVT_BUTTON, &UnmatchedOrder::OnClickSubmit, this, submitButtonId);
    Bind(wxEVT_BUTTON, &UnmatchedOrder::OnClickCancel, this, cancelId);
    Bind(wxEVT_SPINCTRLDOUBLE, &UnmatchedOrder::OnSpin, this, wxID_ANY);

    workerManager.Bind(worker::CANCEL_ORDERS);
    workerManager.Bind(worker::REPLACE_ORDERS);

}

void UnmatchedOrder::SetCurrentOrderSummary(const greentop::CurrentOrderSummary& cos) {

    currentOrderSummary = cos;

    if (market.HasRunner(currentOrderSummary.getSelectionId())) {
        greentop::RunnerCatalog runner = market.GetRunner(currentOrderSummary.getSelectionId());

        wxString label(runner.getRunnerName().c_str(), wxConvUTF8);
        runnerName->SetLabel(currentOrderSummary.getSide().getValue() + " " + label);

        oddsSpin->SetValue(currentOrderSummary.getPriceSize().getPrice());

        std::string currencySymbol = GetCurrencySymbol(entity::Config::GetConfigValue<std::string>("accountCurrency", "?"));

        std::ostringstream stakeLabelStream;
        double sizeRemaining = 0;
        greentop::Optional<double> optionalSizeRemaining = currentOrderSummary.getSizeRemaining();
        if (optionalSizeRemaining.isValid()) {
            sizeRemaining = optionalSizeRemaining.getValue();
        }
        stakeLabelStream << currencySymbol << std::fixed << std::setprecision(2)
            << sizeRemaining;
        wxString stakeLabel(stakeLabelStream.str().c_str(), wxConvUTF8);

        stake->SetLabel(stakeLabel);

        if (currentOrderSummary.getSide() == greentop::Side::BACK) {
            profitOrLiabilityLabel->SetLabel(_("Profit: "));
            SetBackgroundColour(wxColour(227, 235, 255));
        } else if (currentOrderSummary.getSide() == greentop::Side::LAY) {
            profitOrLiabilityLabel->SetLabel(_("Liability: "));
            SetBackgroundColour(wxColour(255, 224, 255));
        }

        UpdateProfitOrLiability();

        GetSizer()->Layout();
    }

}

void UnmatchedOrder::UpdateProfitOrLiability() {

    std::string currencySymbol = GetCurrencySymbol(entity::Config::GetConfigValue<std::string>("accountCurrency", "?"));

    std::ostringstream profitOrLiabilityStream;

    double profit;

    double sizeRemaining = 0;
    greentop::Optional<double> optionalSizeRemaining = currentOrderSummary.getSizeRemaining();
    if (optionalSizeRemaining.isValid()) {
        sizeRemaining = optionalSizeRemaining.getValue();
    }

    if (currentOrderSummary.getSide() == greentop::Side::BACK) {
        profit = sizeRemaining * (oddsSpin->GetValue() - 1);
    } else if (currentOrderSummary.getSide() == greentop::Side::LAY) {
        profit = sizeRemaining * (oddsSpin->GetValue() - 1);
    }

    profitOrLiabilityStream << currencySymbol << std::fixed << std::setprecision(2) << profit;

    wxString label(profitOrLiabilityStream.str().c_str(), wxConvUTF8);
    profitOrLiability->SetLabel(label);

}

void UnmatchedOrder::OnSpin(wxSpinDoubleEvent& event) {
    UpdateProfitOrLiability();
    submitButton->Enable();
}

void UnmatchedOrder::OnClickCancel(wxCommandEvent& event) {
    workerManager.RunWorker(new worker::CancelOrders(&workerManager,
        market.GetExchange(), currentOrderSummary.getMarketId(), currentOrderSummary.getBetId()));
}

void UnmatchedOrder::OnClickSubmit(wxCommandEvent& event) {
    workerManager.RunWorker(new worker::ReplaceOrders(&workerManager, market.GetExchange(),
        market.GetMarketCatalogue().getMarketId(), currentOrderSummary.getBetId(), oddsSpin->GetValue()));
}

}
