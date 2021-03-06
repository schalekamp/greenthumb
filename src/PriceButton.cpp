/**
 * Copyright 2017 Colin Doig.  Distributed under the MIT license.
 */
#include "PriceButton.h"

namespace greenthumb {

PriceButton::PriceButton(wxWindow* parent, wxWindowID id, const wxString& label, const wxPoint& pos,
    const wxSize& size, long style, const wxValidator& validator, const wxString& name) :
    wxButton(parent, id, label, pos, size, style, validator, name) {

}

void PriceButton::SetSide(const greentop::Side& side) {
    placeInstruction.setSide(side);
}

const greentop::Side& PriceButton::GetSide() const {
    return placeInstruction.getSide();
}

void PriceButton::SetSelectionId(const uint64_t selectionId) {
    placeInstruction.setSelectionId(selectionId);
}

void PriceButton::SetPrice(const double price) {
    greentop::LimitOrder limitOrder(0.0, price, greentop::PersistenceType());
    placeInstruction.setLimitOrder(limitOrder);
}

void PriceButton::SetHandicap(const double handicap) {
    placeInstruction.setHandicap(handicap);
}

const greentop::PlaceInstruction& PriceButton::GetPlaceInstruction() const {
    return placeInstruction;
}

}
